#ifndef VIDEOSTREAMDECODER_H
#define VIDEOSTREAMDECODER_H


#include <QThread>
#include <QMutex> 
#include <QQueue>
#include <QImage>
#include <QString>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

// 视频流信息
class VideoStreamInfo {
public:
    VideoStreamInfo(QString url, int width, int height, double frameRate, 
                    const QString &codecName)          
    :   mVideoUrl(url),
        mVideoWidth(width),
        mVideoHeight(height),
        mVideoFrameRate(frameRate),
        mVideoCodecName(codecName) {}

    QString mVideoUrl;          // 视频流地址
    int mVideoWidth;            // 视频宽度
    int mVideoHeight;           // 视频高度
    double mVideoFrameRate;     // 视频帧率
    QString mVideoCodecName;    // 视频编解码器名称
};

class VideoStreamDecoder : public QThread {
    Q_OBJECT

public:
    explicit VideoStreamDecoder(const QString &url, QObject *parent = nullptr);
    ~VideoStreamDecoder();

    void stop();    // 停止解码线程

    VideoStreamInfo* getVideoStreamInfo() { return m_info; }    // 获取视频流信息

    QImage getDecodedFrame();   // 获取解码后的帧

protected:
    void run() override;    // 线程运行函数

signals:
    void newFrameAvailable();  // 队列有新帧时发送信号

private:
    bool initialize();  // 初始化视频流
    void decode(const AVPacket &packet);    // 解码视频帧
    void cleanup(); // 清理资源

    // 视频流信息
    QString m_url;                  // 视频流地址
    AVFormatContext *m_pFormatCtx;  // 视频流格式上下文
    AVCodecContext *m_pCodecCtx;    // 视频流编解码器上下文
    const AVCodec *m_pCodec;        // 视频流编解码器
    AVFrame *m_pFrame;              // 视频帧
    AVFrame *m_pFrameRGB;           // RGB 格式视频帧
    SwsContext *m_pSwsCtx;          // 视频帧转换器
    int m_videoStream;              // 视频流索引

    // 视频流基本信息
    VideoStreamInfo *m_info;

    // 线程控制
    bool m_stop = false;            // 是否停止线程
    QMutex m_mutex;                 // 互斥锁

    // 缓冲队列（最大长度，例如10帧）
    QQueue<QImage> m_frameQueue;    // 图像队列
    const int m_maxQueueSize = 10;  // 队列最大长度
    QMutex m_queueMutex;            // 队列互斥锁
};

#endif // VIDEOSTREAM_H
