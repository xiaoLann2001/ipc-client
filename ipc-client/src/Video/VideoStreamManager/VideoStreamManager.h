#ifndef VIDEOSTREAMMANAGER_H
#define VIDEOSTREAMMANAGER_H

#include <QObject>
#include <QMutex>
#include <QVector>
#include <QString>

#include "VideoStream/VideoStreamDecoder.h"
#include "HandleManager.h"

/**
 * @brief 视频流管理器，封装了视频流的创建、删除等操作，向外提供句柄使用视频流资源
 */
class VideoStreamManager : public QObject {
    Q_OBJECT

public:
    explicit VideoStreamManager(QObject *parent = nullptr);
    ~VideoStreamManager();

    // 创建和删除视频流，创建后返回句柄，删除时传入句柄
    int createVideoStream(const QString &url);      // 创建视频流
    void deleteVideoStream(int handle);             // 删除视频流

    // 通过句柄使用视频流资源
    VideoStreamInfo* getVideoStreamInfo(int handle);// 获取视频流信息
    QImage getDecodedImage(int handle);             // 获取解码后的图像帧

signals:
    void newFrameAvailable(int handle);             // 新帧可用信号，传递时带上句柄

private:
    // 句柄管理
    HandleManager<VideoStreamDecoder> handleManager;// 句柄管理器
};

#endif // VIDEOSTREAMMANAGER_H
