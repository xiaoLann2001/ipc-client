#include "IPCClientApplication.h"

#include <QFont>
#include <QFontDatabase>
#include <QTranslator>

#include "Logger.h"

IPCClientApplication::IPCClientApplication(int argc, char *argv[])
    : QApplication(argc, argv)
{
    // 安装消息处理器
    qInstallMessageHandler(MessageHandler);

    // 设置字体
    fontInit();

    // 设置翻译
    translationInit();

    // 初始化主界面
    mainUiInit();

    // 初始化视频监控界面
    videoInit();

    // 初始化测试界面
    testUiInit();

    // 显示主界面
    m_ipc_client_view->show();
}

void IPCClientApplication::fontInit()
{
    // 设置字体
    int fontId = QFontDatabase::addApplicationFont(":/fonts/wenquanyi.ttf");
    if (fontId != -1)
    {
        QFont font("文泉驿等宽微米黑", 10);
        setFont(font);
    }
}

void IPCClientApplication::translationInit()
{
    // 设置翻译
    QTranslator translator;
    translator.load(":/translations/zh_CN.qm");
    installTranslator(&translator);
}

void IPCClientApplication::mainUiInit()
{
    // 应用采取MVC模式，实现界面与数据分离
    m_ipc_client_view = new IPCClientView();    // 主界面会创建其他子界面
    m_ipc_client_controller = new IPCClientController(m_ipc_client_view);

    // 信号槽连接
    connect(m_ipc_client_controller, &IPCClientController::closeApplication, this, &IPCClientApplication::onApplicationClose);
}

void IPCClientApplication::videoInit()
{
    // 创建视频监控模块
    m_videoStreamManager = new VideoStreamManager();
    m_videoViewWidget = m_ipc_client_view->getVideoViewWidget();
    m_videoStreamController = new VideoStreamController(m_videoStreamManager, m_videoViewWidget);
}

void IPCClientApplication::onApplicationClose()
{
    // 退出应用程序
    exit(0);
}

#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>

void IPCClientApplication::testUiInit()
{
    
}