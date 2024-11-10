#include "IPCClientController.h"
#include <QDebug>


IPCClientController::IPCClientController(IPCClientView *view)
{
    // 视频监控视图
    m_videoStreamManager = new VideoStreamManager();
    m_videoViewWidget = view->getVideoViewWidget();
    m_videoStreamController = new VideoStreamController(m_videoStreamManager, m_videoViewWidget);
}
