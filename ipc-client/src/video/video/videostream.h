#ifndef VIDEOSTREAM_H
#define VIDEOSTREAM_H

#include <QThread>
#include <QImage>
#include <QString>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

class VideoStream : public QThread {
    Q_OBJECT

public:
    explicit VideoStream(const QString &url, QObject *parent = nullptr);
    ~VideoStream();

    void stop();
    void run() override;
    QString url() const { return m_url; }

signals:
    void newFrame(QImage img);

private:
    bool initialize();
    void decode(const AVPacket &packet);
    void cleanup();

    QString m_url;
    AVFormatContext *m_pFormatCtx;
    AVCodecContext *m_pCodecCtx;
    const AVCodec *m_pCodec;
    AVFrame *m_pFrame;
    AVFrame *m_pFrameRGB;
    SwsContext *m_pSwsCtx;
    int m_videoStream;
    bool m_stop = false;
};

#endif // VIDEOSTREAM_H
