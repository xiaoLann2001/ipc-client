#include "VideoStreamController/VideoStreamController.h"
#include <QDebug>

VideoStreamController::VideoStreamController(VideoStreamManager *manager, VideoViewWidget *view, QObject *parent)
    : QObject(parent), m_manager(manager), m_view(view) {
    
    connect(m_manager, &VideoStreamManager::newFrameAvailable, this, &VideoStreamController::onNewFrameAvailable);

    onAddVideoStream(0, "rtsp://192.168.5.174/live/1");
    view->onVideoPlay(0);
}

VideoStreamController::~VideoStreamController() {
    // 停止所有视频流
    for (auto it = m_displayUnitToHandle.begin(); it != m_displayUnitToHandle.end(); ++it) {
        m_manager->stopVideoStream(it.value());
    }
}

void VideoStreamController::onAddVideoStream(int videoDisplayUnitId, const QString &url) {
    int handle = m_manager->createVideoStream(url);  // 创建视频流
    bindViewToStream(videoDisplayUnitId, handle);     // 将视频流绑定到指定控件
}

void VideoStreamController::onStopVideoDisplay(int videoDisplayUnitId) {
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        int handle = m_displayUnitToHandle[videoDisplayUnitId];
        m_manager->stopVideoStream(handle);
        m_displayUnitToHandle.remove(videoDisplayUnitId);
        m_handleToDisplayUnit.remove(handle);
    }
}

void VideoStreamController::onOutOfGrid(int videoDisplayUnitId) {
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        int handle = m_displayUnitToHandle[videoDisplayUnitId];
        m_manager->stopVideoStream(handle);
        m_displayUnitToHandle.remove(videoDisplayUnitId);
        m_handleToDisplayUnit.remove(handle);
    }
}

void VideoStreamController::onVideoGridPageChanged(int page) {
    Q_UNUSED(page);
}

void VideoStreamController::onNewFrameAvailable(int handle) {
    if (m_handleToDisplayUnit.contains(handle)) {
        // qDebug() << "New frame available for handle: " << handle;
        int videoDisplayUnitId = m_handleToDisplayUnit[handle];
        QImage img = m_manager->getDecodedFrame(handle);
        m_view->onNewFrame(videoDisplayUnitId, img);
    }
}

void VideoStreamController::bindViewToStream(int videoDisplayUnitId, int handle) {
    m_displayUnitToHandle[videoDisplayUnitId] = handle;
    m_handleToDisplayUnit[handle] = videoDisplayUnitId;
}