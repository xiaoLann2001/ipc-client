#include "VideoController/VideoController.h"
#include <QDebug>
#include <QInputDialog>
#include <QMenu>

VideoController::VideoController(VideoStreamManager *manager, VideoViewWidget *view, QObject *parent)
    : QObject(parent), m_manager(manager), m_view(view) {

    // 初始化对话框
    m_inputDialog = new QInputDialog();
    // m_inputDialog->setModal(false);
    m_inputDialog->setWindowTitle(tr("添加IPC"));
    m_inputDialog->setLabelText(tr("请输入IPC地址:"));
    m_inputDialog->setTextValue("rtsp://192.168.5.185/live/1");
    m_inputDialog->resize(400, 100);

    // 初始化右键菜单
    m_menu = new QMenu();
    m_addIPCAction = m_menu->addAction(tr("添加IPC"));
    m_removeIPCAction = m_menu->addAction(tr("移除IPC"));

    // 初始化信号与槽
    controlInit();
}

VideoController::~VideoController() {
    // 停止所有视频流
    m_manager->deleteLater();

    // 未设置父类，需要手动释放
    delete m_inputDialog;
    delete m_menu;
    delete m_addIPCAction;
    delete m_removeIPCAction;
}

// 初始化信号与槽
void VideoController::controlInit() {
    // 连接模型层的信号到槽函数
    connect(m_manager, &VideoStreamManager::newFrameAvailable, this, &VideoController::onNewFrameAvailable);

    // 连接视图层的信号到槽函数
    connect(VideoSignalBus::instance(), &VideoSignalBus::videoControlSignal, this, &VideoController::onVideoControlSignal);
    connect(m_view, &VideoViewWidget::addIPCClicked, this, &VideoController::onAddIPCClicked);

    connect(m_view, &VideoViewWidget::videoGridChanged, this, &VideoController::onVideoGridChanged);

    connect(m_view, &VideoViewWidget::videoGridViewClicked, this, &VideoController::onVideoViewClicked);
    connect(m_view, &VideoViewWidget::videoGridViewRightClicked, this, &VideoController::onVideoViewRightClicked);
}

// 将视频流绑定到指定控件
void VideoController::bindViewToStream(int videoDisplayUnitId, int handle) {
    m_displayUnitToHandle[videoDisplayUnitId] = handle;
    m_handleToDisplayUnit[handle] = videoDisplayUnitId;
}

// 将视频流从指定控件解绑
void VideoController::unbindViewFromStream(int videoDisplayUnitId) {
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        int handle = m_displayUnitToHandle[videoDisplayUnitId];
        m_displayUnitToHandle.remove(videoDisplayUnitId);
        m_handleToDisplayUnit.remove(handle);
    }
}

// 根据控件 ID 创建视频流
void VideoController::createVideoStreamfromViewId(int videoDisplayUnitId, const QString &url) {
    // 若控件 ID 已经有视频流，则不再创建
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        qDebug() << "window " << videoDisplayUnitId << " already has a video stream.";
        return;
    }
    int handle = m_manager->createVideoStream(url);     // 创建视频流
    bindViewToStream(videoDisplayUnitId, handle);       // 将视频流绑定到指定控件
}

// 根据控件 ID 删除视频流
void VideoController::deleteVideoStreamfromViewId(int videoDisplayUnitId) {
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        int handle = m_displayUnitToHandle[videoDisplayUnitId];
        m_manager->deleteVideoStream(handle);
        unbindViewFromStream(videoDisplayUnitId);
    }
}

/*--------------------对话框和右键菜单获取用户输入------------------------------*/

QString VideoController::getUserInputFromDialog() {
    // 设置对话框显示在主窗口中心
    if (m_view != nullptr) {
        QRect mainWindowRect = m_view->geometry();
        int centerX = mainWindowRect.center().x() - m_inputDialog->width() / 2;
        int centerY = mainWindowRect.center().y() - m_inputDialog->height() / 2;
        m_inputDialog->move(m_view->mapToGlobal(QPoint(centerX, centerY)));
    }

    // 显示对话框并获取用户输入
    if (m_inputDialog->exec() == QDialog::Accepted) {
        return m_inputDialog->textValue();
    } else {
        return QString();
    }
}

QString VideoController::getUserOptionFromMenu(const QPoint &pos) {
    // 显示右键菜单
    QAction *selectedAction = m_menu->exec(pos);
    if (selectedAction == m_addIPCAction) {
        return "addIPC";
    } else if (selectedAction == m_removeIPCAction) {
        return "removeIPC";
    } else {
        return QString();
    }
}

/*--------------------对话框和右键菜单获取用户输入结束---------------------------*/


/*--------------------用于响应视图层的槽函数------------------------------------*/

// 用户视频控制命令槽函数
void VideoController::onVideoControlSignal(const VideoControlCommand &command) {
    qDebug() << "VideoControlCommand: " << command.id << " " << command.cmd;
    
    // 检查视频显示单元 ID 是否有效
    if (command.id < 0 || command.id >= m_view->getGrid()) {
        qDebug() << "Invalid video display unit id: " << command.id;
        return;
    }

    if (command.cmd == VideoControlCommand::Add) {
        onAddVideoStream(command.id, command.param.toString());
    } else if (command.cmd == VideoControlCommand::Close) {
        onStopVideoDisplay(command.id);
    }
}

void VideoController::onAddVideoStream(int videoDisplayUnitId, const QString &url) {
    Q_UNUSED(videoDisplayUnitId);
    Q_UNUSED(url);
}

void VideoController::onStopVideoDisplay(int videoDisplayUnitId) {
    Q_UNUSED(videoDisplayUnitId);
}

void VideoController::onOutOfGrid(int videoDisplayUnitId) {
    Q_UNUSED(videoDisplayUnitId);
}

void VideoController::onVideoGridChanged(int grid) {
    Q_UNUSED(grid);
}

void VideoController::onVideoPageChanged(int page) {
    Q_UNUSED(page);
}


void VideoController::onAddIPCClicked() {
    // 若当前视频显示单元 ID 不为 -1，表示有被选中的窗口
    if (m_currentVideoDisplayUnitId != -1) {
        QString url = getUserInputFromDialog();
        if (!url.isEmpty()) {
            // 创建视频流
            createVideoStreamfromViewId(m_currentVideoDisplayUnitId, url);
        }
        m_view->onVideoPlay(m_currentVideoDisplayUnitId);
    } else {    // 否则选择未绑定视频的最小 ID 的窗口显示
        for (int i = 0; i < m_view->getGrid(); ++i) {
            if (!m_displayUnitToHandle.contains(i)) {
                QString url = getUserInputFromDialog();
                if (!url.isEmpty()) {
                    // 创建视频流
                    createVideoStreamfromViewId(i, url);
                }
                m_view->onVideoPlay(i);
                break;
            }
        }
    }
}

void VideoController::onVideoViewClicked(int videoDisplayUnitId) {
    // 若当前视频控件 ID 为传入的视频控件 ID，则取消选中
    if (m_currentVideoDisplayUnitId == videoDisplayUnitId) {
        m_currentVideoDisplayUnitId = -1;
    } else {
        m_currentVideoDisplayUnitId = videoDisplayUnitId;
    }
}

void VideoController::onVideoViewRightClicked(int videoDisplayUnitId, const QPoint &pos) {
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

void VideoController::onNewFrameAvailable(int handle) {
    if (m_handleToDisplayUnit.contains(handle)) {
        int videoDisplayUnitId = m_handleToDisplayUnit[handle];
        QImage img = m_manager->getDecodedImage(handle);
        m_view->onNewFrame(videoDisplayUnitId, img);
    }
}

/*--------------------用于响应模型层的槽函数结束--------------------------------*/