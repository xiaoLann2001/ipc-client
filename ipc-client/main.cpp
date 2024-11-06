#include "ipcclientmodel.h"
#include "ipcclientview.h"
#include "ipcclientcontroller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 应用采取MVC模式，实现界面与数据分离
    IPCClientModel ipc_client_model;
    IPCClientView ipc_client_view;
    IPCClientController ipc_client_controller(&ipc_client_model, &ipc_client_view);

    ipc_client_view.show();

    return a.exec();
}
