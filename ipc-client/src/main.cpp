
#include <application/ipcclientapplication.h>

#include <QFontDatabase>
#include <QTranslator>

int main(int argc, char *argv[])
{
    IPCClientApplication application(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/AndBasR.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Bstgreek.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ec_cour.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ecl_cour.ttf");

    QTranslator translator;
    translator.load(":/translations/zh_CN.qm");
    application.installTranslator(&translator);

    return application.exec();
}
