#ifndef IPCCLIENTCONTROLLER_H
#define IPCCLIENTCONTROLLER_H

#include <QObject>

#include "IPCClientView.h"

#include "VideoStreamManager/VideoStreamManager.h"
#include "VideoView/VideoViewWidget.h"
#include "VideoStreamController/VideoStreamController.h"

class IPCClientController : public QObject
{
    Q_OBJECT
public:
    IPCClientController(IPCClientView *view);

private:
    VideoStreamManager *m_videoStreamManager;
    VideoViewWidget *m_videoViewWidget;
    VideoStreamController *m_videoStreamController;
};

#endif // IPCCLIENTCONTROLLER_H