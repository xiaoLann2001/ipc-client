#ifndef IPC_CLIENT_CORE_SIGNALBUS_H_
#define IPC_CLIENT_CORE_SIGNALBUS_H_

#include <QObject>
#include <QMutex>

/**
 * @brief 事件总线是一个全局单例，用于应用内部的自定义信号和槽通信
*/
class IPCClientCoreSignalBus : public QObject
{
    Q_OBJECT

public:
    static IPCClientCoreSignalBus *instance() {
        static QMutex mutex;
        static IPCClientCoreSignalBus *m_instance = nullptr;
        if (!m_instance) {
            QMutexLocker locker(&mutex);
            if (!m_instance) {
                m_instance = new IPCClientCoreSignalBus();
            }
        }
        return m_instance;
    }

signals:
    // 应用全局信号开始
    






    // 应用全局信号结束
private:

    explicit IPCClientCoreSignalBus(QObject *parent = nullptr) : QObject(parent) {}

    Q_DISABLE_COPY(IPCClientCoreSignalBus)
};

#endif // IPC_CLIENT_CORE_SIGNALBUS_H_