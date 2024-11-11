#include "VideoStreamController/VideoStreamController.h"
#include <QDebug>
#include <QInputDialog>

VideoStreamController::VideoStreamController(VideoStreamManager *manager, VideoViewWidget *view, QObject *parent)
    : QObject(parent), m_manager(manager), m_view(view) {
    
    connect(m_manager, &VideoStreamManager::newFrameAvailable, this, &VideoStreamController::onNewFrameAvailable);

    connect(m_view, &VideoViewWidget::addIPCClicked, this, &VideoStreamController::onAddIPCClicked);
    connect(m_view, &VideoViewWidget::videoGridChanged, this, &VideoStreamController::onVideoGridChanged);
    // onAddVideoStream(0, "rtsp://192.168.5.174/live/1");
    // view->onVideoPlay(0);
}

VideoStreamController::~VideoStreamController() {
    // 停止所有视频流
    for (auto it = m_displayUnitToHandle.begin(); it != m_displayUnitToHandle.end(); ++it) {
        m_manager->stopVideoStream(it.value());
    }
}

void VideoStreamController::bindViewToStream(int videoDisplayUnitId, int handle) {
    m_displayUnitToHandle[videoDisplayUnitId] = handle;
    m_handleToDisplayUnit[handle] = videoDisplayUnitId;
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

void VideoStreamController::onVideoGridChanged(int grid) {
    Q_UNUSED(grid);
}

void VideoStreamController::onVideoPageChanged(int page) {
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

void VideoStreamController::onAddIPCClicked() {
    // 创建一个 QInputDialog 实例
    QInputDialog dialog;
    dialog.setWindowTitle(tr("添加IPC"));
    dialog.setLabelText(tr("请输入IPC地址:"));
    dialog.setTextValue("rtsp://");

    // 调整对话框大小
    dialog.resize(400, 100);

    // 设置对话框显示在主窗口中心
    if (m_view != nullptr) {
        // 获取主窗口的中心点位置
        QRect mainWindowRect = m_view->geometry();
        int centerX = mainWindowRect.center().x() - dialog.width() / 2;
        int centerY = mainWindowRect.center().y() - dialog.height() / 2;

        // 将对话框移动到主窗口中心
        dialog.move(m_view->mapToGlobal(QPoint(centerX, centerY)));
    }

    // 显示对话框并获取用户输入
    if (dialog.exec() == QDialog::Accepted) {
        QString ipcAddress = dialog.textValue();
        if (!ipcAddress.isEmpty()) {
            // 查询可用的视频显示单元 ID
            

            // 将输入的 IPC 地址添加到视频流管理器
            onAddVideoStream(0, ipcAddress);
            m_view->onVideoPlay(0);
        }
    }
}