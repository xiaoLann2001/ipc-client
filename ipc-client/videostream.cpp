#include "videostream.h"
#include <QDebug>
#include <QImage>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

VideoStream::VideoStream(const QString &url, QObject *parent)
    : QThread(parent),
      m_url(url),
      m_pFormatCtx(nullptr),
      m_pCodecCtx(nullptr),
      m_pFrame(nullptr),
      m_pFrameRGB(nullptr),
      m_pSwsCtx(nullptr),
      m_videoStream(-1) {}

VideoStream::~VideoStream() {
    stop();
}

void VideoStream::stop() {
    m_stop = true;
    wait();
}

void VideoStream::run() {
    if (!initialize()) return;

    AVPacket packet;
    while (!m_stop && av_read_frame(m_pFormatCtx, &packet) >= 0) {
        if (packet.stream_index == m_videoStream) {
            decode(packet);
            av_packet_unref(&packet);
        }
    }
}

bool VideoStream::initialize() {
    av_register_all();
    avformat_network_init();

    // 打开视频流
    if (avformat_open_input(&m_pFormatCtx, m_url.toStdString().c_str(), nullptr, nullptr) != 0) {
        qWarning() << "Could not open video stream";
        return false;
    }

    if (avformat_find_stream_info(m_pFormatCtx, nullptr) < 0) {
        qWarning() << "Could not find stream information";
        return false;
    }

    // 查找视频流
    for (size_t i = 0; i < m_pFormatCtx->nb_streams; ++i) {  // 使用 size_t
        if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_videoStream = i;
            break;
        }
    }
    if (m_videoStream == -1) {
        qWarning() << "Could not find video stream";
        return false;
    }

    // 获取解码器
    m_pCodecCtx = avcodec_alloc_context3(nullptr);
    avcodec_parameters_to_context(m_pCodecCtx, m_pFormatCtx->streams[m_videoStream]->codecpar);
    m_pCodec = avcodec_find_decoder(m_pCodecCtx->codec_id);
    if (m_pCodec == nullptr) {
        qWarning() << "Codec not found";
        return false;
    }

    if (avcodec_open2(m_pCodecCtx, m_pCodec, nullptr) < 0) {
        qWarning() << "Could not open codec";
        return false;
    }

    m_pFrame = av_frame_alloc();
    if (!m_pFrame) {
        qWarning() << "Could not allocate frame";
        return false;
    }

    m_pFrameRGB = av_frame_alloc();
    if (!m_pFrameRGB) {
        qWarning() << "Could not allocate RGB frame";
        return false;
    }

    int numBytes = av_samples_get_buffer_size(AV_PIX_FMT_RGB24, m_pCodecCtx->width, m_pCodecCtx->height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(m_pFrameRGB->data, m_pFrameRGB->linesize, buffer, AV_PIX_FMT_RGB24,
                         m_pCodecCtx->width, m_pCodecCtx->height, 1);

    m_pSwsCtx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt,
                               m_pCodecCtx->width, m_pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BILINEAR,
                               nullptr, nullptr, nullptr);

    return true;
}

void VideoStream::decode(const AVPacket &packet) {
    int response = avcodec_send_packet(m_pCodecCtx, &packet);
    if (response < 0) {
        qWarning() << "Error while sending packet to decoder";
        return;
    }

    response = avcodec_receive_frame(m_pCodecCtx, m_pFrame);
    if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
        return;
    }
    if (response < 0) {
        qWarning() << "Error while receiving frame from decoder";
        return;
    }

    // 转换为 RGB 格式
    sws_scale(m_pSwsCtx, m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height,
              m_pFrameRGB->data, m_pFrameRGB->linesize);

    // 转为 QImage
    QImage img(m_pFrameRGB->data[0], m_pCodecCtx->width, m_pCodecCtx->height, m_pFrameRGB->linesize[0], QImage::Format_RGB888);
    emit newFrame(img);
}

void VideoStream::cleanup() {
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
