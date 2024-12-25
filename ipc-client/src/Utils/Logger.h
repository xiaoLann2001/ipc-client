#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

// 自定义日志处理器
void MessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    // 获取文件名、函数名、行号等信息
    QString fileName = QFileInfo(context.file).fileName();
    QString functionName = context.function;
    int lineNumber = context.line;

    // 打开日志文件
    // QFile logFile("app.log");
    // logFile.open(QIODevice::Append | QIODevice::Text);
    // QTextStream out(&logFile);

    // 打开标准输出
    QTextStream out(stdout);

    // 根据日志级别选择不同的输出
    switch (type) {
    case QtDebugMsg:
        out << "[Debug]";
        break;
    case QtInfoMsg:
        out << "[Info]";
        break;
    case QtWarningMsg:
        out << "[Warning]";
        break;
    case QtCriticalMsg:
        out << "[Critical]";
        break;
    case QtFatalMsg:
        out << "[Fatal]";
        break;
    }

    // 输出文件名、函数名、行号和日志内容
    out << "[" << fileName << ":" << lineNumber << "][" << functionName << "] " << message << "\n";

    // logFile.close();
}

#endif // LOGGER_H
