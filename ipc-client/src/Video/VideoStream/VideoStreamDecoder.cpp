#include "VideoStream/VideoStreamDecoder.h"
#include <QDebug>
#include <QImage>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

VideoStreamDecoder::VideoStreamDecoder(const QString &url, QObject *parent)
    : QThread(parent),
      m_url(url),
      m_pFormatCtx(nullptr),
      m_pCodecCtx(nullptr),
      m_pFrame(nullptr),
      m_pFrameRGB(nullptr),
      m_pSwsCtx(nullptr),
      m_videoStreamIdx(-1) {}

VideoStreamDecoder::~VideoStreamDecoder() {
    stop();
    cleanup();
}

VideoStreamInfo* VideoStreamDecoder::getVideoStreamInfo() { 
    return m_info; 
}

void VideoStreamDecoder::stop() {
    {
        // 使用互斥锁保护共享变量，注意加锁的粒度，如果将整个 stop 函数加锁，会导致线程无法退出
        QMutexLocker locker(&m_mutex);
        m_stop = true;
    }
    {
        QMutexLocker locker(&m_queueMutex);
        m_frameQueue.clear();
    }
    wait();
}

void VideoStreamDecoder::run() {
    // 初始化视频流
    if (-1 == initialize()) {
        cleanup();      // 初始化失败，清理资源
        return;
    }

    // 读取并解码视频流
    AVPacket packet;
    while (true) {
        {
            QMutexLocker locker(&m_mutex);
            if (m_stop) return;
        }

        // 读取数据包
        if (av_read_frame(m_pFormatCtx, &packet) < 0) {
            qWarning() << "Error reading frame, possibly due to stream disconnect or server crash";
            break;
        }

        if (packet.stream_index == m_videoStreamIdx) { // 判断是否是视频流
            // 解码视频帧
            decode(packet);
            // 释放数据包
            av_packet_unref(&packet);
        }
    }

    // 清理资源
    cleanup();
}

int VideoStreamDecoder::initialize() {
    // 初始化 FFmpeg 相关的库
    // av_register_all();          // 注册所有的编解码器
    avformat_network_init();    // 初始化网络流格式库

    // 打开视频流
    if (avformat_open_input(&m_pFormatCtx, m_url.toStdString().c_str(), nullptr, nullptr) != 0) {
        qWarning() << "Could not open video stream";
        return -1;
    }

    // 获取流信息
    if (avformat_find_stream_info(m_pFormatCtx, nullptr) < 0) {
        qWarning() << "Could not find stream information";
        cleanup();
        return -1;
    }

    // 查找视频流，一般来说，RTSP 流中可能包含视频、音频等多个流，我们需要找到第一个视频流
    for (size_t i = 0; i < m_pFormatCtx->nb_streams; ++i) {  // 使用 size_t
        if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_videoStreamIdx = i;
            break;
        }
    }
    if (m_videoStreamIdx == -1) {
        qWarning() << "Could not find video stream";
        cleanup();
        return -1;
    }

    // 获取解码器，并初始化解码上下文
    // m_pCodecCtx = avcodec_alloc_context3(nullptr);  // 分配解码上下文
    // avcodec_parameters_to_context(m_pCodecCtx, m_pFormatCtx->streams[m_videoStreamIdx]->codecpar); // 将流参数拷贝到解码上下文
    // // 查找解码器
    // m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);
    // if (m_pCodec == nullptr) {
    //     qWarning() << "Codec not found";
    //     return false;
    // }
    
    // 获取视频流的编码参数
    AVCodecParameters *codecParams = m_pFormatCtx->streams[m_videoStreamIdx]->codecpar;
    AVCodecID codecId = codecParams->codec_id;

    // 根据编码类型选择解码器
    switch (codecId) {
        case AV_CODEC_ID_H264:
            m_pCodec = avcodec_find_decoder_by_name("h264_rkmpp");  // 硬解码器
            if (!m_pCodec) m_pCodec = avcodec_find_decoder(codecId); // 如果硬解码器不可用，使用软件解码器
            break;
        case AV_CODEC_ID_HEVC:
            m_pCodec = avcodec_find_decoder_by_name("hevc_rkmpp");  // 硬解码器
            if (!m_pCodec) m_pCodec = avcodec_find_decoder(codecId); // 如果硬解码器不可用，使用软件解码器
            break;
        default:
            m_pCodec = avcodec_find_decoder(codecId);  // 使用软件解码器
            break;
    }
    if (m_pCodec == nullptr) {
        qWarning() << "Codec not found";
        cleanup();
        return -1;
    }

    // 打印解码器信息
    qDebug() << "Codec: " << m_pCodec->long_name;

    // 初始化解码器上下文
    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
    if (!m_pCodecCtx) {
        qWarning() << "Could not allocate codec context";
        cleanup();
        return -1;
    }

    // 将流的编码参数复制到解码器上下文
    if (avcodec_parameters_to_context(m_pCodecCtx, codecParams) < 0) {
        qWarning() << "Failed to copy codec parameters to decoder context";
        cleanup();
        return -1;
    }

    // 打开解码器
    if (avcodec_open2(m_pCodecCtx, m_pCodec, nullptr) < 0) {
        qWarning() << "Could not open codec";
        cleanup();
        return -1;
    }

    // 获取视频的基本信息
    m_info = new VideoStreamInfo(m_url, m_pCodecCtx->width, m_pCodecCtx->height, 
            av_q2d(m_pFormatCtx->streams[m_videoStreamIdx]->avg_frame_rate), 
            m_pCodec->long_name);

    // 初始化 RGB 帧
    m_pFrame = av_frame_alloc();
    if (!m_pFrame) {
        qWarning() << "Could not allocate frame";
        cleanup();
        return -1;
    }

    m_pFrameRGB = av_frame_alloc();
    if (!m_pFrameRGB) {
        qWarning() << "Could not allocate RGB frame";
        cleanup();
        return -1;
    }

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, m_pCodecCtx->width, m_pCodecCtx->height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(m_pFrameRGB->data, m_pFrameRGB->linesize, buffer, AV_PIX_FMT_RGB24,
                         m_pCodecCtx->width, m_pCodecCtx->height, 1);

    m_pSwsCtx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt,
                               m_pCodecCtx->width, m_pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BILINEAR,
                               nullptr, nullptr, nullptr);

    return 0;
}

void VideoStreamDecoder::decode(const AVPacket &packet) {
    // 发送数据包到解码器
    int response = avcodec_send_packet(m_pCodecCtx, &packet);
    if (response < 0) {
        qWarning() << "Error while sending packet to decoder";
        return;
    }

    // 从解码器接收所有解码帧
    while (response >= 0) {
        response = avcodec_receive_frame(m_pCodecCtx, m_pFrame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) break;
        if (response < 0) {
            qWarning() << "Error while receiving frame from decoder";
            return;
        }

        // 转换为 RGB 格式
        sws_scale(m_pSwsCtx, m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height,
                m_pFrameRGB->data, m_pFrameRGB->linesize);

        // 将 RGB 数据转换为 QImage
        QImage img(m_pFrameRGB->data[0], m_pCodecCtx->width, m_pCodecCtx->height, 
                m_pFrameRGB->linesize[0], QImage::Format_RGB888);
        // 将图像放入队列
        {
            QMutexLocker locker(&m_queueMutex);
            if (m_frameQueue.size() >= m_maxQueueSize) {
                m_frameQueue.dequeue();  // 从队列中移除一帧
            }
            m_frameQueue.enqueue(img);  // 将图像放入队列
        }
        // 发送信号, 通知有新帧
        emit newFrameAvailable();
    }
}

QImage VideoStreamDecoder::getDecodedFrame() {
    QMutexLocker locker(&m_queueMutex);
    if (!m_frameQueue.isEmpty()) {
        return m_frameQueue.dequeue();  // 从队列中取出一帧
    }
    return QImage();    // 返回空图像
}

void VideoStreamDecoder::cleanup() {
    if (m_pSwsCtx) {
        sws_freeContext(m_pSwsCtx);
        m_pSwsCtx = nullptr;
    }
    if (m_pFrameRGB) {
        av_frame_free(&m_pFrameRGB);
        m_pFrameRGB = nullptr;
    }
    if (m_pFrame) {
        av_frame_free(&m_pFrame);
        m_pFrame = nullptr;
    }
    if (m_pCodecCtx) {
        avcodec_free_context(&m_pCodecCtx);
        m_pCodecCtx = nullptr;
    }
    if (m_pFormatCtx) {
        avformat_close_input(&m_pFormatCtx);
        m_pFormatCtx = nullptr;
    }
}
