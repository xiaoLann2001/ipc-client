#include "VideoController/VideoController.h"
#include <QDebug>
#include <QInputDialog>
#include <QMenu>

VideoController::VideoController(VideoStreamManager *manager, VideoView *view, QObject *parent)
    : QObject(parent), m_manager(manager), m_view(view) {
    
    // 初始化 IPC 地址输入对话框和右键菜单
    getUserInputInit();

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

void VideoController::getUserInputInit() {
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
}

// 初始化信号与槽
void VideoController::controlInit() {
    // 连接信号总线的视频控制信号到槽函数
    connect(VideoSignalBus::instance(), &VideoSignalBus::videoControlSignal, this, &VideoController::onVideoControlSignal);

    // 连接模型层的信号到槽函数
    connect(m_manager, &VideoStreamManager::newFrameAvailable, this, &VideoController::onNewFrameAvailable);

    // 连接视图层的信号到槽函数
    connect(m_view, &VideoView::addIPCClicked, this, &VideoController::onAddIPCClicked);
    connect(m_view, &VideoView::videoGridChanged, this, &VideoController::onVideoGridChanged);
    connect(m_view, &VideoView::videoClicked, this, &VideoController::onVideoViewClicked);
    connect(m_view, &VideoView::videoRightClicked, this, &VideoController::onVideoViewRightClicked);
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

    // 创建播放控制器
    VideoPlayController *playController = new VideoPlayController();
    m_handleToPlayController[handle] = playController;  // 视频流句柄与播放控制器一一对应
    playController->setId(videoDisplayUnitId);      // 指定播放控制器的视频显示单元 ID，以便分发视频帧到对应的视频控件
    
    // 连接控制器的视频帧到播放控制器
    connect(this, &VideoController::videoFrameToControl, playController, &VideoPlayController::onVideoFrameToControl);

    // 播放视频
    playController->play();
}

// 根据控件 ID 删除视频流
void VideoController::deleteVideoStreamfromViewId(int videoDisplayUnitId) {
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        // 删除视频流解码器
        int handle = m_displayUnitToHandle[videoDisplayUnitId];
        m_manager->deleteVideoStream(handle);
        unbindViewFromStream(videoDisplayUnitId);

        // 删除播放控制器
        if (m_handleToPlayController.contains(handle)) {
            VideoPlayController *playController = m_handleToPlayController[handle];
            // 停止播放
            playController->stop();
            // 断开连接
            disconnect(this, &VideoController::videoFrameToControl, playController, &VideoPlayController::onVideoFrameToControl);
            // 释放播放控制器
            playController->deleteLater();
            m_handleToPlayController.remove(handle);
        }
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

    if (command.cmd == VideoControlCommand::Command::Add) {
        onAdd(command.id, command.param.toString());
    } else if (command.cmd == VideoControlCommand::Command::Close) {
        onClose(command.id);
    } else if (command.cmd == VideoControlCommand::Command::Play) {
        onPlay(command.id);
    } else if (command.cmd == VideoControlCommand::Command::Pause) {
        onPause(command.id);
    } else if (command.cmd == VideoControlCommand::Command::Fullscreen) {
        // 全屏，由视图层处理
    } else if (command.cmd == VideoControlCommand::Command::Snapshot) {
        // 截图，暂未实现
    } else if (command.cmd == VideoControlCommand::Command::Record) {
        // 录制，暂未实现
    } else if (command.cmd == VideoControlCommand::Command::AI) {
        // AI，暂未实现
    } else if (command.cmd == VideoControlCommand::Command::Sync) {
        onSync(command.id);
    } else {
        qDebug() << "Unknown command: " << command.cmd;
    }
}

void VideoController::onAdd(int videoDisplayUnitId, const QString &url) {
    // createVideoStreamfromViewId(videoDisplayUnitId, url);
    Q_UNUSED(url);
    // 获取用户输入的视频流地址
    QString url1 = getUserInputFromDialog();
    if (url1.isEmpty()) return;

    createVideoStreamfromViewId(videoDisplayUnitId, url1);

    emit VideoSignalBus::instance()->videoControlResponse(
        VideoControlResponse(videoDisplayUnitId, 
        VideoControlResponse::Command::Add, 
        VideoControlResponse::Response::Success));
}

void VideoController::onClose(int videoDisplayUnitId) {
    deleteVideoStreamfromViewId(videoDisplayUnitId);
    emit VideoSignalBus::instance()->videoControlResponse(
        VideoControlResponse(videoDisplayUnitId, 
        VideoControlResponse::Command::Close, 
        VideoControlResponse::Response::Success));
}

void VideoController::onPlay(int videoDisplayUnitId) {
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        int handle = m_displayUnitToHandle[videoDisplayUnitId];
        if (m_handleToPlayController.contains(handle)) {
            m_handleToPlayController[handle]->play();   // 调用对应视频流的播放控制器的播放函数
        }
    }
}

void VideoController::onPause(int videoDisplayUnitId) {
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        int handle = m_displayUnitToHandle[videoDisplayUnitId];
        if (m_handleToPlayController.contains(handle)) {
            m_handleToPlayController[handle]->pause();  // 调用对应视频流的播放控制器的暂停函数
        }
    }
}

void VideoController::onSync(int videoDisplayUnitId) {
    if (m_displayUnitToHandle.contains(videoDisplayUnitId)) {
        int handle = m_displayUnitToHandle[videoDisplayUnitId];
        if (m_handleToPlayController.contains(handle)) {
            // 获取播放控制器的状态
            bool isPlaying = m_handleToPlayController[handle]->isPlaying();
            emit VideoSignalBus::instance()->videoControlResponse(
                VideoControlResponse(videoDisplayUnitId, 
                VideoControlResponse::Command::Sync, 
                isPlaying ? VideoControlResponse::Response::Success : VideoControlResponse::Response::Failed));
        }
    }
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
    // 获取用户输入的视频流地址
    QString url = getUserInputFromDialog();
    if (url.isEmpty()) return;

    int realID = -1;

    // 若当前视频显示单元 ID 不为 -1，表示有被选中的窗口
    if (m_currentVideoDisplayUnitId != -1) {
        realID = m_currentVideoDisplayUnitId;
    }
    // 否则选择未绑定视频的最小 ID 的窗口显示
    else {
        for (int i = 0; i < m_view->getGrid(); ++i) {
            if (!m_displayUnitToHandle.contains(i)) {
                realID = i;
                break;
            }
        }
    }

    // 为实际窗口创建视频流
    if (realID != -1) createVideoStreamfromViewId(realID, url);
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
        // m_view->onVideoPlay(videoDisplayUnitId);
    } else if (option == "removeIPC") {
        // 删除视频流
        // m_view->onVideoStop(videoDisplayUnitId);
        deleteVideoStreamfromViewId(videoDisplayUnitId);
    }
}

/*--------------------用于响应视图层的槽函数结束--------------------------------*/











/*--------------------用于响应模型层的槽函数------------------------------------*/

/**
 * @brief 响应模型层新视频帧到达的信号，handle 是视频流的句柄
*/
void VideoController::onNewFrameAvailable(int handle) {
    if (m_handleToDisplayUnit.contains(handle)) {
        // 获取解码后的视频帧
        QImage img = m_manager->getDecodedImage(handle);
        
        // 发送视频帧到视频播放控制器
        if (m_handleToPlayController.contains(handle)) {
            // emit videoFrameToControl(img);
            m_handleToPlayController[handle]->onVideoFrameToControl(img);
        }
    }
}

/*--------------------用于响应模型层的槽函数结束--------------------------------*/