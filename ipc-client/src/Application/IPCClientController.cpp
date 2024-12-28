#include <QDebug>
#include <QMessageBox>

#include "IPCClientController.h"

IPCClientController::IPCClientController(IPCClientView *view)
    : m_ipc_client_view(view)
{
    controlInit();
}

void IPCClientController::controlInit()
{
    // 信号槽连接
    connect(m_ipc_client_view, &IPCClientView::pushButton_close_clicked, this, &IPCClientController::onPushButtonCloseClicked);
}

void IPCClientController::onPushButtonCloseClicked()
{
    qDebug() << "用户点击关闭按钮";

    // 使用 QMessageBox 弹出确认退出对话框
    QMessageBox::StandardButton reply = QMessageBox::question(m_ipc_client_view, 
        "确认退出", 
        "是否确认退出？", 
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // 发送关闭应用程序信号
        emit closeApplication();
    }
}

