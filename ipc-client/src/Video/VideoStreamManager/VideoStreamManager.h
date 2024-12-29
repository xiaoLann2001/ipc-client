#ifndef VIDEOSTREAMMANAGER_H
#define VIDEOSTREAMMANAGER_H

#include <QObject>
#include <QMap>
#include <QSet>
#include <QMutex>
#include <QString>

#include "VideoStream/VideoStreamDecoder.h"
#include "AudioPlayer/AudioPlayer.h"

// 视频流管理器
class VideoStreamManager : public QObject {
    Q_OBJECT

public:
    explicit VideoStreamManager(QObject *parent = nullptr);
    ~VideoStreamManager();

    int createVideoStream(const QString &url);      // 创建视频流
    void deleteVideoStream(int handle);             // 删除视频流
    VideoStreamInfo* getVideoStreamInfo(int handle);// 获取视频流信息
    QImage getDecodedImage(int handle);             // 获取解码后的帧

signals:
    void newFrameAvailable(int handle);             // 新帧可用信号

private:
    int generateHandle();                           // 生成唯一句柄
    void recycleHandle(int handle);                 // 回收句柄

    QSet<int> recycledHandles;               // 存储回收的句柄
    QMap<int, VideoStreamDecoder*> m_streams;       // 句柄到视频流解码器的映射
    QMutex m_mutex_streamMap;                       // 保护 m_streams 的互斥锁
    QMap<int, AudioPlayer*> m_audioPlayers;         // 句柄到音频播放器的映射
    QMutex m_mutex_audioPlayerMap;                  // 保护 m_audioPlayers 的互斥锁
};

#endif // VIDEOSTREAMMANAGER_H
