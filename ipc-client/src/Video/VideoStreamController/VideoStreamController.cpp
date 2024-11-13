#include "VideoStreamController/VideoStreamController.h"
#include <QDebug>
#include <QInputDialog>
#include <QMenu>

VideoStreamController::VideoStreamController(VideoStreamManager *manager, VideoViewWidget *view, QObject *parent)
    : QObject(parent), m_manager(manager), m_view(view) {
    
    connect(m_manager, &VideoStreamManager::newFrameAvailable, this, &VideoStreamController::onNewFrameAvailable);

    connect(m_view, &VideoViewWidget::addIPCClicked, this, &VideoStreamController::onAddIPCClicked);

    connect(m_view, &VideoViewWidget::videoGridChanged, this, &VideoStreamController::onVideoGridChanged);
    
    
    connect(m_view, &VideoViewWidget::videoGridViewClicked, this, &VideoStreamController::onVideoViewClicked);
    connect(m_view, &VideoViewWidget::videoGridViewRightClicked, this, &VideoStreamController::onVideoViewRightClicked);
}

VideoStreamController::~VideoStreamController() {
    // 停止所有视频流
    for (auto it = m_displayUnitToHandle.begin(); it != m_displayUnitToHandle.end(); ++it) {
        m_manager->deleteVideoStream(it.value());
    }
}

// 将视频流绑定到指定控件
void VideoStreamController::bindViewToStream(int videoDisplayUnitId, int handle) {
    m_displayUnitToHandle[videoDisplayUnitId] = handle;
    m_handleToDisplayUnit[handle] = videoDisplayUnitId;
}

// 将视频流从指定控件解绑
void VideoStreamController::unbindViewFromStream(int videoDisplayUnitId) {
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        int handle = m_displayUnitToHandle[videoDisplayUnitId];
        m_displayUnitToHandle.remove(videoDisplayUnitId);
        m_handleToDisplayUnit.remove(handle);
    }
}

// 根据控件 ID 创建视频流
void VideoStreamController::createVideoStreamfromViewId(int videoDisplayUnitId, const QString &url) {
    int handle = m_manager->createVideoStream(url);  // 创建视频流
    bindViewToStream(videoDisplayUnitId, handle);     // 将视频流绑定到指定控件
}

// 根据控件 ID 删除视频流
void VideoStreamController::deleteVideoStreamfromViewId(int videoDisplayUnitId) {
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        int handle = m_displayUnitToHandle[videoDisplayUnitId];
        m_manager->deleteVideoStream(handle);
        unbindViewFromStream(videoDisplayUnitId);
    }
}

/*--------------------对话框和右键菜单获取用户输入------------------------------*/

QString VideoStreamController::getUserInputFromDialog() {
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
        return dialog.textValue();
    } else {
        return QString();
    }
}

QString VideoStreamController::getUserOptionFromMenu(const QPoint &pos) {
    QMenu menu;
    QAction *addIPCAction = menu.addAction(tr("添加IPC"));
    QAction *removeIPCAction = menu.addAction(tr("移除IPC"));

    // 显示菜单
    QAction *selectedAction = menu.exec(pos);
    if (selectedAction == addIPCAction) {
        return "addIPC";
    } else if (selectedAction == removeIPCAction) {
        return "removeIPC";
    } else {
        return QString();
    }
}

/*--------------------对话框和右键菜单获取用户输入结束---------------------------*/


/*--------------------用于响应视图层的槽函数------------------------------------*/

void VideoStreamController::onAddVideoStream(int videoDisplayUnitId, const QString &url) {
    Q_UNUSED(videoDisplayUnitId);
    Q_UNUSED(url);
}

void VideoStreamController::onStopVideoDisplay(int videoDisplayUnitId) {
    Q_UNUSED(videoDisplayUnitId);
}

void VideoStreamController::onOutOfGrid(int videoDisplayUnitId) {
    Q_UNUSED(videoDisplayUnitId);
}

void VideoStreamController::onVideoGridChanged(int grid) {
    Q_UNUSED(grid);
}

void VideoStreamController::onVideoPageChanged(int page) {
    Q_UNUSED(page);
}


void VideoStreamController::onAddIPCClicked() {
    // QString url = getUserInputFromDialog();
    // if (!url.isEmpty()) {
    //     // 创建视频流
    //     createVideoStreamfromViewId(m_currentVideoDisplayUnitId, url);
    // }
}

void VideoStreamController::onVideoViewClicked(int videoDisplayUnitId) {
    // 若当前视频控件 ID 为传入的视频控件 ID，则取消选中
    if (m_currentVideoDisplayUnitId == videoDisplayUnitId) {
        m_currentVideoDisplayUnitId = -1;
    } else {
        if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
            m_currentVideoDisplayUnitId = videoDisplayUnitId;
        }
    }
}

void VideoStreamController::onVideoViewRightClicked(int videoDisplayUnitId, const QPoint &pos) {
    QString option = getUserOptionFromMenu(pos);
    if (option == "addIPC") {
        QString url = getUserInputFromDialog();
        if (!url.isEmpty()) {
            // 创建视频流
            createVideoStreamfromViewId(videoDisplayUnitId, url);
        }
        m_view->onVideoPlay(videoDisplayUnitId);
    } else if (option == "removeIPC") {
        // 删除视频流
        m_view->onVideoStop(videoDisplayUnitId);
        deleteVideoStreamfromViewId(videoDisplayUnitId);
    }
}

/*--------------------用于响应视图层的槽函数结束--------------------------------*/











/*--------------------用于响应模型层的槽函数------------------------------------*/

void VideoStreamController::onNewFrameAvailable(int handle) {
    if (m_handleToDisplayUnit.contains(handle)) {
        // qDebug() << "New frame available for handle: " << handle;
        int videoDisplayUnitId = m_handleToDisplayUnit[handle];
        QImage img = m_manager->getDecodedFrame(handle);
        m_view->onNewFrame(videoDisplayUnitId, img);
    }
}

/*--------------------用于响应模型层的槽函数结束--------------------------------*/