#ifndef VIDEOCONTROLCOMMAND_H
#define VIDEOCONTROLCOMMAND_H

#include <QVariant>

// 自定义视频控制命令结构体
struct VideoControlCommand
{
    enum Command
    {
        Add,            // 添加视频
        Close,          // 关闭视频
        Play,           // 播放视频
        Pause,          // 暂停视频
        Fullscreen,     // 全屏
        Snapshot,       // 截图
        Record,         // 录制
        AI,             // AI
    };

    int id;             // 视频ID
    Command cmd;        // 命令
    QVariant param;     // 参数

    VideoControlCommand(int id, Command command, const QVariant &param = QVariant())
        : id(id), cmd(command), param(param)
    {
    }
};

#endif // VIDEOCONTROLCOMMAND_H