#include "IPCClientApplication.h"

#include <QFont>
#include <QFontDatabase>
#include <QTranslator>
#include "Logger.h"

IPCClientApplication::IPCClientApplication(int argc, char *argv[])
    : QApplication(argc, argv)
{
    // 设置字体
    int fontId = QFontDatabase::addApplicationFont(":/fonts/wenquanyi.ttf");
    if (fontId != -1)
    {
        QFont font("文泉驿等宽微米黑", 10);
        setFont(font);
    }

    // 设置翻译
    QTranslator translator;
    translator.load(":/translations/zh_CN.qm");
    installTranslator(&translator);

    // 安装消息处理器
    qInstallMessageHandler(MessageHandler);

    // 应用采取MVC模式，实现界面与数据分离
    ipc_client_view = new IPCClientView();
    ipc_client_controller = new IPCClientController(ipc_client_view);

    ipc_client_view->show();
}
