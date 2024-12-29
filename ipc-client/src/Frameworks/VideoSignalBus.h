#ifndef VIDEO_SIGNALBUS_H_
#define VIDEO_SIGNALBUS_H_

#include <QObject>
#include <QMutex>

#include "VideoControlCommand.h"    // 自定义视频控制命令结构体

/**
 * @brief 视频模块事件总线
*/
class VideoSignalBus : public QObject
{
    Q_OBJECT

public:
    static VideoSignalBus *instance() {
        static QMutex mutex;
        static VideoSignalBus *m_instance = nullptr;
        if (!m_instance) {
            QMutexLocker locker(&mutex);
            if (!m_instance) {
                m_instance = new VideoSignalBus();
            }
        }
        return m_instance;
    }

signals:
    // 视频模块信号开始

    void videoControlSignal(const VideoControlCommand &command);    // 视频控制信号

    // 视频模块信号结束
private:

    explicit VideoSignalBus(QObject *parent = nullptr) : QObject(parent) {}

    Q_DISABLE_COPY(VideoSignalBus)
};

#endif // VIDEO_SIGNALBUS_H_