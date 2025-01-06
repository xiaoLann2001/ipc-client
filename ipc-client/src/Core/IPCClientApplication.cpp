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

    // 测试函数
    testFunc();

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
    m_videoView = m_ipc_client_view->getVideoView();
    m_videoController = new VideoController(m_videoStreamManager, m_videoView);
}

void IPCClientApplication::onApplicationClose()
{
    // 退出应用程序
    exit(0);
}

// #include "HandleManager.h"

void IPCClientApplication::testFunc()
{
    // // 测试函数
    // HandleManager<CustomObject> manager;

    // // 创建两个对象并生成句柄
    // auto handle1 = manager.createHandle(new CustomObject("Object1"));
    // auto handle2 = manager.createHandle(new CustomObject("Object2"));

    // // 获取资源
    // auto resource1 = manager.getResource(handle1);
    // auto resource2 = manager.getResource(handle2);

    // qDebug() << "Resource 1 name:" << resource1->getName();
    // qDebug() << "Resource 2 name:" << resource2->getName();

    // // 回收句柄
    // manager.releaseHandle(handle1);
    // manager.releaseHandle(handle2);

    // // 尝试获取已回收的资源
    // auto invalidResource = manager.getResource(handle1);
    // qDebug() << "Resource 1 after release:" << (invalidResource != nullptr);

}