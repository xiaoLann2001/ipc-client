#ifndef VIDEOPLAYCONTROLLER_H
#define VIDEOPLAYCONTROLLER_H

#include <QObject>
#include <QImage>
#include <QAtomicInteger>

#include "VideoStream/VideoStreamDecoder.h"
#include "VideoSignalBus.h"

class VideoPlayController : public QObject
{
    Q_OBJECT

public:
    VideoPlayController(QObject *parent = nullptr);
    ~VideoPlayController();

    void setId(int videoDisplayUnitId) { m_videoDisplayUnitId = videoDisplayUnitId; }
    int getId() const { return m_videoDisplayUnitId; }

    void play();
    void pause();
    void stop();

    bool isPlaying() const { return playbackState.loadAcquire(); }

signals:
    

public slots:
    // 响应控制器
    void onVideoFrameToControl(const QImage &image);

private:
    int m_videoDisplayUnitId = -1;   // 视频显示单元 ID

    // 播放状态，true 为播放，false 为暂停
    QAtomicInteger<bool> playbackState;

};

#endif // VIDEOPLAYCONTROLLER_H