#ifndef IPCCLIENTAPPLICATION_H
#define IPCCLIENTAPPLICATION_H

#include <QApplication>

#include "IPCClientView.h"
#include "IPCClientController.h"

class IPCClientApplication : public QApplication
{
public:
    IPCClientApplication(int argc, char *argv[]);

private:
    IPCClientView *ipc_client_view;
    IPCClientController *ipc_client_controller;
};

#endif // IPCCLIENTAPPLICATION_H