#ifndef IPCCLIENTCONTROLLER_H
#define IPCCLIENTCONTROLLER_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QPushButton>

#include "IPCClientView.h"

class IPCClientController : public QObject
{
    Q_OBJECT
public:
    IPCClientController(IPCClientView *view);

    void controlInit();

signals:
    void closeApplication();

private slots:
    void onPushButtonCloseClicked();

private:
    IPCClientView *m_ipc_client_view;
};

#endif // IPCCLIENTCONTROLLER_H