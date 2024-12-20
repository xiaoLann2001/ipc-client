
#include <IPCClientApplication.h>

#include <QFont>
#include <QFontDatabase>
#include <QTranslator>

int main(int argc, char *argv[])
{
    // 设置 OpenGL 上下文共享属性
    IPCClientApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

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
