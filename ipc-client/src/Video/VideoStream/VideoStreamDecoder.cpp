#include "VideoStream/VideoStreamDecoder.h"
#include <QDebug>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}

VideoStreamDecoder::VideoStreamDecoder(const QString &url, QObject *parent)
    : QThread(parent), m_url(url) {
    // 使用前，需要将 AVDictionary 指针初始化为 nullptr
    options = nullptr;
    
    // 视频解码器信息                  
    m_pFormatCtx = nullptr;
    m_pCodecCtx = nullptr;
    m_pCodec = nullptr;
    m_pFrame = nullptr;
    m_pFrameRGB = nullptr;
    m_pSwsCtx = nullptr;
    m_videoStreamIdx = -1;
#if AudioDecoderEnabled
    // 音频解码器信息
    m_pAudioCodecCtx = nullptr;
    m_pAudioCodec = nullptr;
    m_pAudioFrame = nullptr;
    m_audioStreamIdx = -1;
#endif
    // 视频流基本信息
    m_info = nullptr;

    // 线程控制
    m_stop = false;
    m_paused = false;
}

VideoStreamDecoder::~VideoStreamDecoder() {
    stop();
    cleanup();
}

VideoStreamInfo* VideoStreamDecoder::getVideoStreamInfo() { 
#if AudioDecoderEnabled
    // 获取视频的基本信息
    if (m_info == nullptr) {
        m_info = new VideoStreamInfo(m_url, m_pCodecCtx->width, m_pCodecCtx->height, 
                av_get_pix_fmt_name(m_pCodecCtx->pix_fmt),
                av_q2d(m_pFormatCtx->streams[m_videoStreamIdx]->avg_frame_rate), 
                m_pCodec->long_name);
    }
#endif
    return m_info; 
}

void VideoStreamDecoder::stop() {
    {
        // 使用互斥锁保护共享变量，注意加锁的粒度，如果将整个 stop 函数加锁，会导致线程无法退出
        QMutexLocker locker(&m_stopMutex);
        m_stop = true;
    }
    {
        QMutexLocker locker(&m_queueMutex);
        m_frameQueue.clear();
    }
#if AudioDecoderEnabled
    {
        QMutexLocker locker(&m_audioQueueMutex);
        m_audioQueue.clear();
    }
#endif
    resume();  // 唤醒等待的线程
    wait();    // 等待线程退出
}

void VideoStreamDecoder::pause() {
    QMutexLocker locker(&m_pauseMutex);
    m_paused = true;  // 设置为暂停状态
}

void VideoStreamDecoder::resume() {
    {
        QMutexLocker locker(&m_pauseMutex);
        m_paused = false;  // 设置为继续解码状态
    }
    m_pauseCondition.wakeOne();  // 唤醒等待的解码线程
}

void VideoStreamDecoder::run() {
    // 初始化视频流
    if (!init()) {
        cleanup();
        return;
    }

    // 读取并解码视频流
    AVPacket packet;
    while (true) {
        {
            QMutexLocker locker(&m_stopMutex);
            if (m_stop) return;
        }

        // 如果处于暂停状态，等待继续
        {
            QMutexLocker locker(&m_pauseMutex);
            if (m_paused) {
                m_pauseCondition.wait(&m_pauseMutex);  // 等待继续信号
            }
        }

        // 读取数据包
        if (av_read_frame(m_pFormatCtx, &packet) < 0) {
            qWarning() << "Error reading frame, possibly due to stream disconnect or server crash";
            break;
        }

        if (packet.stream_index == m_videoStreamIdx) {
            // 解码视频帧
            decodeVideo(packet);
        } 
#if AudioDecoderEnabled
        else if (packet.stream_index == m_audioStreamIdx) {
            // 解码音频帧
            decodeAudio(packet);
        }
#endif
        
        // 释放数据包
        av_packet_unref(&packet);
    }

    // 清理资源
    cleanup();
}

bool VideoStreamDecoder::init() {
    // 判断该摄像机是否能联通
    // if (checkConn && isRtsp) {
    //     if (!checkUrl(url, checkTime)) {
    //         return false;
    //     }
    // }

    // 初始化 FFmpeg 库
    initlib();
    // 初始化参数
    initOption();
    // 初始化输入
    if (!initInput()) {
        return false;
    }
    // 初始化视频
    if (!initVideo()) {
        return false;
    }
#if AudioDecoderEnabled
    // 初始化音频
    if (!initAudio()) {
        return false;
    }
#endif
    // 初始化帧
    this->initFrame();

    // 输出视频信息
    av_dump_format(m_pFormatCtx, 0, m_url.toStdString().data(), 0);

    return true;
}

void VideoStreamDecoder::initlib() {
    // 初始化 FFmpeg 相关的解码库，一个软件中只需要初始化一次就行
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    static bool isInit = false;
    if (!isInit) {
        // av_register_all();          // 注册所有的编解码器
        avformat_network_init();    // 初始化网络流格式库
        isInit = true;
    }
}

void VideoStreamDecoder::initOption() {
    // 在打开码流前指定各种参数比如:探测时间/超时时间/最大延时等
    QString transport = "udp";
    // 设置缓存大小
    av_dict_set(&options, "buffer_size", "425984", 0);
    // 以tcp/udp方式打开
    av_dict_set(&options, "rtsp_transport", transport.toLatin1().constData(), 0);
    // 设置超时断开连接时间,单位微秒,3000000表示3秒
    av_dict_set(&options, "stimeout", "3000000", 0);
    // 设置最大时延,单位微秒,1000000表示1秒
    av_dict_set(&options, "max_delay", "1000000", 0);
    // 自动开启线程数
    av_dict_set(&options, "threads", "auto", 0);

    // 设置USB摄像机分辨率
    int videoWidth = 1920, videoHeight = 1080;
    if (m_url.startsWith("video")) {
        QString size = QString("%1x%2").arg(videoWidth).arg(videoHeight);
        av_dict_set(&options, "video_size", size.toLatin1().constData(), 0);
    }
}

bool VideoStreamDecoder::initInput() {
    // 实例化格式处理上下文
    m_pFormatCtx = avformat_alloc_context();

    // 先判断是否是本地设备(video=设备名字符串), 打开的方式不一样
    int result = -1;
    if (m_url.startsWith("video")) {
#if defined(Q_OS_WIN)
        const AVInputFormat *ifmt = av_find_input_format("dshow");
#elif defined(Q_OS_LINUX)
        const AVInputFormat *ifmt = av_find_input_format("video4linux2");
#elif defined(Q_OS_MAC)
        const AVInputFormat *ifmt = av_find_input_format("avfoundation");
#endif
        // 以指定格式打开视频流
        result = avformat_open_input(&m_pFormatCtx, m_url.toStdString().data(), ifmt, &options);
    } else {
        // 以默认格式打开视频流
        result = avformat_open_input(&m_pFormatCtx, m_url.toStdString().data(), NULL, &options);
    }
    if (result < 0) {
        qDebug() << "open input error" << m_url;
        return false;
    }

    // 释放设置参数
    if (options != NULL) {
        av_dict_free(&options);
        options = NULL;
    }

    // 获取流信息
    result = avformat_find_stream_info(m_pFormatCtx, NULL);
    if (result < 0) {
        qDebug() << "find stream info error";
        return false;
    }

    return true;
}

bool VideoStreamDecoder::initVideo() {
    // 查找视频流
    for (size_t i = 0; i < m_pFormatCtx->nb_streams; ++i) {
        if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_videoStreamIdx = i;
            break;
        }
    }
    if (m_videoStreamIdx == -1) {
        qDebug() << "Could not find video stream";
        cleanup();
        return false;
    }
    
    // 获取视频流的编码参数
    AVCodecParameters *video_codecParams = m_pFormatCtx->streams[m_videoStreamIdx]->codecpar;
    AVCodecID video_codecId = video_codecParams->codec_id;

    // 根据编码类型选择解码器
    switch (video_codecId) {
        case AV_CODEC_ID_H264:
            m_pCodec = avcodec_find_decoder_by_name("h264_rkmpp");  // 硬解码器
            if (!m_pCodec) m_pCodec = avcodec_find_decoder(video_codecId); // 如果硬解码器不可用，使用软件解码器
            break;
        case AV_CODEC_ID_HEVC:
            m_pCodec = avcodec_find_decoder_by_name("hevc_rkmpp");  // 硬解码器
            if (!m_pCodec) m_pCodec = avcodec_find_decoder(video_codecId); // 如果硬解码器不可用，使用软件解码器
            break;
        default:
            m_pCodec = avcodec_find_decoder(video_codecId);         // 使用软件解码器
            break;
    }
    if (m_pCodec == nullptr) {
        qDebug() << "Could not find video codec";
        cleanup();
        return false;
    }
    // 打印解码器信息
    // qDebug() << "video codec: " << m_pCodec->long_name;

    // 初始化解码器上下文
    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
    if (!m_pCodecCtx) {
        qWarning() << "Could not allocate codec context";
        cleanup();
        return false;
    }

    // 将流的编码参数复制到解码器上下文
    if (avcodec_parameters_to_context(m_pCodecCtx, video_codecParams) < 0) {
        qWarning() << "Failed to copy codec parameters to decoder context";
        cleanup();
        return false;
    }

    // 打开解码器
    if (avcodec_open2(m_pCodecCtx, m_pCodec, nullptr) < 0) {
        qWarning() << "Could not open codec";
        cleanup();
        return false;
    }

    // QString videoInfo = QString("视频流信息 -> 索引: %1  解码: %2  格式: %3  时长: %4 秒  分辨率: %5*%6")
    //                         .arg(m_videoStreamIdx).arg(m_pCodec->name).arg(m_pFormatCtx->iformat->name)
    //                         .arg((m_pFormatCtx->duration) / 1000000).arg(m_pCodecCtx->width).arg(m_pCodecCtx->height);
    // qDebug() << videoInfo;

    return true;
}

#if AudioDecoderEnabled
bool VideoStreamDecoder::initAudio() {
    // 查找音频流
    for (size_t i = 0; i < m_pFormatCtx->nb_streams; ++i) {
        if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            m_audioStreamIdx = i;
            break;
        }
    }
    if (m_audioStreamIdx == -1) {
        qDebug() << "Could not find audio stream";
        return false;
    }

    // 获取音频流的编码参数
    AVCodecParameters *audio_codecParams = m_pFormatCtx->streams[m_audioStreamIdx]->codecpar;
    AVCodecID audio_codecId = audio_codecParams->codec_id;

    // 根据编码类型选择解码器
    m_pAudioCodec = avcodec_find_decoder(audio_codecId);  // 使用软件解码器

    // 打印解码器信息
    // qDebug() << "audio codec: " << m_pAudioCodec->long_name;

    // 初始化解码器上下文
    m_pAudioCodecCtx = avcodec_alloc_context3(m_pAudioCodec);
    if (!m_pAudioCodecCtx) {
        qWarning() << "Could not allocate audio codec context";
        return false;
    }

    // 将流的编码参数复制到解码器上下文
    if (avcodec_parameters_to_context(m_pAudioCodecCtx, audio_codecParams) < 0) {
        qWarning() << "Failed to copy audio codec parameters to decoder context";
        return false;
    }

    // 打开解码器
    if (avcodec_open2(m_pAudioCodecCtx, m_pAudioCodec, nullptr) < 0) {
        qWarning() << "Could not open audio codec";
        return false;
    }

    // QString audioInfo = QString("音频流信息 -> 索引: %1  解码: %2  比特率: %3  声道数: %4  采样: %5")
    //                             .arg(m_audioStreamIdx).arg(m_pAudioCodec->name).arg(m_pFormatCtx->bit_rate)
    //                             .arg(m_pAudioCodecCtx->channels).arg(m_pAudioCodecCtx->sample_rate);
    // qDebug() << audioInfo;

    return true;
}
#endif

bool VideoStreamDecoder::initFrame() {
    // 初始化 RGB 帧
    m_pFrame = av_frame_alloc();
    if (!m_pFrame) {
        qWarning() << "Could not allocate frame";
        cleanup();
        return false;
    }

    m_pFrameRGB = av_frame_alloc();
    if (!m_pFrameRGB) {
        qWarning() << "Could not allocate RGB frame";
        cleanup();
        return false;
    }

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, m_pCodecCtx->width, m_pCodecCtx->height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    
    // 开辟缓存存储一帧数据
    av_image_fill_arrays(m_pFrameRGB->data, m_pFrameRGB->linesize, buffer, AV_PIX_FMT_RGB24,
                         m_pCodecCtx->width, m_pCodecCtx->height, 1);

    // 初始化转换器
    AVPixelFormat srcFormat = m_pCodecCtx->pix_fmt;
    AVPixelFormat dstFormat = AV_PIX_FMT_RGB24;
    int flag = SWS_FAST_BILINEAR;
    m_pSwsCtx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, srcFormat,
                               m_pCodecCtx->width, m_pCodecCtx->height, dstFormat, flag,
                               nullptr, nullptr, nullptr);
    if (!m_pSwsCtx) {
        qWarning() << "Could not initialize the conversion context";
        cleanup();
        return false;
    }
#if AudioDecoderEnabled
    // 初始化音频帧
    m_pAudioFrame = av_frame_alloc();
    if (!m_pAudioFrame) {
        qWarning() << "Could not allocate audio frame";
        return false;
    }
#endif
    return true;
}

AVFrame* VideoStreamDecoder::cloneFrame(const AVFrame* src) {
    AVFrame* dst = av_frame_alloc();
    if (!dst) return nullptr;

    // 分配内存并复制数据
    if (av_frame_copy(dst, src) < 0) {
        av_frame_free(&dst);
        return nullptr;
    }
    if (av_frame_copy_props(dst, src) < 0) {
        av_frame_free(&dst);
        return nullptr;
    }
    return dst;
}

void VideoStreamDecoder::decodeVideo(const AVPacket &packet) {
    // 判断数据包是否为视频流
    if (packet.stream_index != m_videoStreamIdx) return;
    
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

        emit videoFrameReady();
    }
}

#if AudioDecoderEnabled
void VideoStreamDecoder::decodeAudio(const AVPacket &packet) {
    // 判断数据包是否为音频流
    if (packet.stream_index != m_audioStreamIdx) return;

    // 发送数据包到解码器
    int response = avcodec_send_packet(m_pAudioCodecCtx, &packet);
    if (response < 0) {
        qWarning() << "Error while sending packet to audio decoder";
        return;
    }

    // 从解码器接收所有解码帧
    while (response >= 0) {
        response = avcodec_receive_frame(m_pAudioCodecCtx, m_pAudioFrame);
        if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) break;
        if (response < 0) {
            qWarning() << "Error while receiving frame from audio decoder";
            return;
        }

        // 获取解码后的 PCM 数据
        uint8_t *pcmData = m_pAudioFrame->data[0];
        int pcmSize = m_pAudioFrame->linesize[0];

        // 存入音频缓冲队列
        {
            QMutexLocker locker(&m_audioQueueMutex);
            if (m_audioQueue.size() >= m_maxAudioQueueSize) {
                m_audioQueue.dequeue();  // 从队列中移除一帧
            }
            m_audioQueue.enqueue(QByteArray((char *)pcmData, pcmSize));
        }

        emit audioFrameReady();
    }
}
#endif

QImage VideoStreamDecoder::getDecodedImage() {
    QMutexLocker locker(&m_queueMutex);
    if (!m_frameQueue.isEmpty()) {
        return m_frameQueue.dequeue();  // 从队列中取出一帧
    }
    return QImage();    // 返回空图像
}

#if AudioDecoderEnabled
QByteArray VideoStreamDecoder::getDecodedAudio() {
    QMutexLocker locker(&m_audioQueueMutex);
    if (!m_audioQueue.isEmpty()) {
        return m_audioQueue.dequeue();  // 从队列中取出一帧
    }
    return QByteArray();    // 返回空音频
}
#endif

void VideoStreamDecoder::cleanup() {
    // 释放视频相关资源
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

#if AudioDecoderEnabled
    // 释放音频相关资源
    if (m_pAudioFrame) {
        av_frame_free(&m_pAudioFrame);
        m_pAudioFrame = nullptr;
    }
    if (m_pAudioCodecCtx) {
        avcodec_free_context(&m_pAudioCodecCtx);
        m_pAudioCodecCtx = nullptr;
    }
#endif

    // 释放视频流基本信息
    if (m_info) {
        delete m_info;
        m_info = nullptr;
    }

    // 释放参数
    if (options != NULL) {
        av_dict_free(&options);
        options = NULL;
    }
}
