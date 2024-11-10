#ifndef IPCCLIENTCONTROLLER_H
#define IPCCLIENTCONTROLLER_H

#include <QObject>

#include "ipcclientview.h"

class IPCClientController : public QObject
{
    Q_OBJECT
public:
    IPCClientController(IPCClientView *view);
};

#endif // IPCCLIENTCONTROLLER_H