#ifndef VIDEOSIGNALBUS_H
#define VIDEOSIGNALBUS_H

#include <QObject>
#include <QMutex>
#include <QVariant>

// 视频控制命令结构体
struct VideoControlCommand
{
    enum Command
    {
        Add,            // 添加视频
        Close,          // 关闭视频
        Play,           // 播放视频
        Pause,          // 暂停视频
        Fullscreen,     // 全屏
        Restore,        // 恢复
        Snapshot,       // 截图
        Record,         // 录制
        AI,             // AI
        Sync,           // 状态同步
    };

    int id;             // 视频窗口 ID
    Command cmd;        // 命令
    QVariant param;     // 参数

    VideoControlCommand(int id, Command command, const QVariant &param = QVariant())
        : id(id), cmd(command), param(param)
    {
    }
};

// 视频控制命令应答结构体
struct VideoControlResponse
{
    enum Command
    {
        Add,            // 添加视频
        Close,          // 关闭视频
        Play,           // 播放视频
        Pause,          // 暂停视频
        Fullscreen,     // 全屏
        Restore,        // 恢复
        Snapshot,       // 截图
        Record,         // 录制
        AI,             // AI
        Sync,           // 状态同步
    };

    enum Response
    {
        Success,        // 成功
        Failed,         // 失败
    };

    int id;             // 视频窗口 ID
    Command cmd;        // 命令
    Response resp;      // 应答
    QVariant param;     // 参数

    VideoControlResponse(int id, Command command, Response response, const QVariant &param = QVariant())
        : id(id), cmd(command), resp(response), param(param)
    {
    }
};

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

    void videoControlResponse(const VideoControlResponse &response); // 视频控制应答

    void videoFrame(const int id, const QImage &frame);             // 视频帧

    void audioFrame(const int id, const QByteArray &frame);         // 音频帧

    // 视频模块信号结束
private:

    explicit VideoSignalBus(QObject *parent = nullptr) : QObject(parent) {}

    Q_DISABLE_COPY(VideoSignalBus)
};

#endif // VIDEOSIGNALBUS_H_