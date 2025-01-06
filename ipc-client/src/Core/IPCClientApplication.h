#ifndef IPCCLIENTAPPLICATION_H
#define IPCCLIENTAPPLICATION_H

#include <QApplication>

// 主界面
#include "IPCClientView.h"
#include "IPCClientController.h"

// 视频监控模块
#include "VideoStreamManager/VideoStreamManager.h"
#include "VideoView/VideoView.h"
#include "VideoController/VideoController.h"

class IPCClientApplication : public QApplication
{
public:
    IPCClientApplication(int argc, char *argv[]);

    void fontInit();
    void translationInit();
    void mainUiInit();
    void videoInit();


    void testFunc();

private slots:
    void onApplicationClose();

private:
    // 主界面
    IPCClientView *m_ipc_client_view;
    IPCClientController *m_ipc_client_controller;

    // 视频监控模块
    VideoStreamManager *m_videoStreamManager;
    VideoView *m_videoView;
    VideoController *m_videoController;
};

#endif // IPCCLIENTAPPLICATION_H