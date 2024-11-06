#ifndef IPCCLIENTCONTROLLER_H
#define IPCCLIENTCONTROLLER_H

#include "ipcclientmodel.h"
#include "ipcclientview.h"

class IPCClientController
{
public:
    IPCClientController(IPCClientModel *model, IPCClientView *view);
};

#endif // IPCCLIENTCONTROLLER_H