
#include <IPCClientApplication.h>

#include <QFont>
#include <QFontDatabase>
#include <QTranslator>

int main(int argc, char *argv[])
{
    IPCClientApplication application(argc, argv);

    int fontId = QFontDatabase::addApplicationFont(":/fonts/wenquanyi.ttf");
    if (fontId != -1)
    {
        QFont font("文泉驿等宽微米黑", 10);
        application.setFont(font);
    }

    QTranslator translator;
    translator.load(":/translations/zh_CN.qm");
    application.installTranslator(&translator);

    return application.exec();
}
