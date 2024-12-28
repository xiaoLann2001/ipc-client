#ifndef VIDEOCONTROLCOMMAND_H
#define VIDEOCONTROLCOMMAND_H

#include <QVariant>

struct VideoControlCommand
{
    enum Command
    {
        Add,
        Play,
        Pause,
        Snapshot,
        Record,
        Fullscreen,
        AI,
        Close
    };

    int id;
    Command command;
    QVariant param;

    VideoControlCommand(int id, Command command, const QVariant &param = QVariant())
        : id(id), command(command), param(param)
    {
    }
};

#endif // VIDEOCONTROLCOMMAND_H