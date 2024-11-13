#ifndef VIDEOSTREAMMANAGER_H
#define VIDEOSTREAMMANAGER_H

#include <QObject>
#include <QMap>
#include <QSet>
#include <QString>

#include "VideoStream/VideoStreamDecoder.h"

// 视频流管理器
class VideoStreamManager : public QObject {
    Q_OBJECT

public:
    explicit VideoStreamManager(QObject *parent = nullptr);
    ~VideoStreamManager();

    int createVideoStream(const QString &url);      // 创建视频流
    void deleteVideoStream(int handle);             // 停止视频流
    VideoStreamInfo* getVideoStreamInfo(int handle);// 获取视频流信息
    QImage getDecodedFrame(int handle);             // 获取解码后的帧

signals:
    void newFrameAvailable(int handle);             // 新帧可用信号

private:
    int generateHandle();                           // 生成唯一句柄
    void recycleHandle(int handle);                 // 回收句柄

    static QSet<int> recycledHandles;               // 存储回收的句柄

    QMap<int, VideoStreamDecoder*> m_streams;       // 句柄到视频流解码器的映射
};

#endif // VIDEOSTREAMMANAGER_H
