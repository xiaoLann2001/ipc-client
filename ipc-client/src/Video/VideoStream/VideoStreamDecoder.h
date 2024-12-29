#ifndef VIDEOSTREAMDECODER_H
#define VIDEOSTREAMDECODER_H


#include <QThread>
#include <QMutex> 
#include <QWaitCondition>
#include <QQueue>
#include <QImage>
#include <QString>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#define AudioDecoderEnabled 1

// 视频流信息
class VideoStreamInfo {
public:
    VideoStreamInfo(QString url, int width, int height, const QString pixfmt, double frameRate, const QString &video_codecName)          
    :   m_url(url),
        m_videoWidth(width),
        m_videoHeight(height),
        m_videoPixFmt(pixfmt),
        m_videoFrameRate(frameRate),
        m_videoCodecName(video_codecName) {}

    QString m_url;              // 视频流地址
    int m_videoWidth;           // 视频宽度
    int m_videoHeight;          // 视频高度
    QString m_videoPixFmt;      // 视频像素格式
    double m_videoFrameRate;    // 视频帧率
    QString m_videoCodecName;   // 视频编解码器名称
};

#if AudioDecoderEnabled
class AudioStreamInfo {
public:
    AudioStreamInfo(QString url, int sampleRate, int channels, int sampleDepth, const QString &codecName)
    :   m_url(url),
        m_sampleRate(sampleRate),
        m_channels(channels),
        m_sampleDepth(sampleDepth),
        m_codecName(codecName) {}

    QString m_url;              // 流地址
    int m_sampleRate;           // 音频采样率
    int m_channels;             // 音频通道数
    int m_sampleDepth;          // 音频采样深度
    QString m_codecName;        // 音频编解码器名称
};
#endif

class VideoStreamDecoder : public QThread {
    Q_OBJECT

public:
    explicit VideoStreamDecoder(const QString &url, QObject *parent = nullptr);
    ~VideoStreamDecoder();

#if 0
    void stop();                            // 停止解码线程
#endif
    void pause();                           // 暂停解码线程
    void resume();                          // 恢复解码线程

    VideoStreamInfo* getVideoStreamInfo();  // 获取视频流信息

    QImage getDecodedImage();               // 获取解码后的图像帧
#if AudioDecoderEnabled
    QByteArray getDecodedAudio();           // 获取解码后的音频帧
#endif

protected:
    void run() override;                    // 线程运行函数

signals:
    // void videoFrameReady(const MediaFrame& mediaFrame); // 解码后的视频帧
    // void audioFrameReady(const MediaFrame& mediaFrame); // 解码后的音频帧
    void videoFrameReady();     // 解码后的视频帧
#if AudioDecoderEnabled
    void audioFrameReady();     // 解码后的音频帧
#endif

    // void newFrameAvailable();               // 队列有新帧时发送信号

private:
    bool init();                            // 初始化
    void initlib();                         // 初始化 FFmpeg 库
    void initOption();                      // 初始化参数
    bool initInput();                       // 初始化输入
    bool initVideo();                       // 初始化视频流
#if AudioDecoderEnabled
    bool initAudio();                       // 初始化音频流
#endif
    bool initFrame();                       // 初始化帧
    
    AVFrame* cloneFrame(const AVFrame *src);// 深拷贝帧

    void decodeVideo(const AVPacket &packet);    // 解码视频帧
#if AudioDecoderEnabled
    void decodeAudio(const AVPacket &packet);    // 解码视频帧
#endif
    void cleanup();                         // 清理资源

    AVDictionary *options;          // FFmpeg 参数

    // 视频流信息
    QString m_url;                  // 视频流地址
    AVFormatContext *m_pFormatCtx;  // 视频流格式上下文
    AVCodecContext *m_pCodecCtx;    // 视频流编解码器上下文
    const AVCodec *m_pCodec;        // 视频流编解码器
    AVFrame *m_pFrame;              // 视频帧
    AVFrame *m_pFrameRGB;           // RGB 格式视频帧
    SwsContext *m_pSwsCtx;          // 视频帧转换器
    int m_videoStreamIdx;           // 视频流索引

#if AudioDecoderEnabled
    // 音频流信息
    AVCodecContext *m_pAudioCodecCtx;  // 音频编解码器上下文
    const AVCodec *m_pAudioCodec;   // 音频编解码器
    AVFrame *m_pAudioFrame;         // 音频帧
    int m_audioStreamIdx;           // 音频流索引
#endif
    // 视频流基本信息
    VideoStreamInfo *m_info;

    // 线程控制
    bool m_stop;                    // 是否停止线程
    QMutex m_stopMutex;             // 线程停止互斥锁
    bool m_paused;                  // 暂停标志
    QMutex m_pauseMutex;            // 用于保护 m_paused 的互斥锁
    QWaitCondition m_pauseCondition;// 用于在暂停时等待恢复的条件变量

    // 图像缓冲队列（最大长度，例如10帧）
    QQueue<QImage> m_frameQueue;    // 图像队列
    const int m_maxQueueSize = 20;  // 队列最大长度
    QMutex m_queueMutex;            // 队列互斥锁
#if AudioDecoderEnabled
    // 音频缓冲队列（最大长度，例如10帧）
    QQueue<QByteArray> m_audioQueue;    // 音频队列
    const int m_maxAudioQueueSize = 20; // 音频队列最大长度
    QMutex m_audioQueueMutex;           // 音频队列互斥锁
#endif
};

#endif // VIDEOSTREAM_H
