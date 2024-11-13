#include "IPCClientApplication.h"

IPCClientApplication::IPCClientApplication(int argc, char *argv[])
    : QApplication(argc, argv)
{
    // 应用采取MVC模式，实现界面与数据分离
    ipc_client_view = new IPCClientView();
    ipc_client_controller = new IPCClientController(ipc_client_view);

    ipc_client_view->show();
}
