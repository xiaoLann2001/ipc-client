#ifndef VIDEOSTREAMMANAGER_H
#define VIDEOSTREAMMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include "video/videostream.h"

class VideoStreamManager : public QObject {
    Q_OBJECT

public:
    explicit VideoStreamManager(QObject *parent = nullptr);
    ~VideoStreamManager();

    void addStream(const QString &url);
    void removeStream(const QString &url);

signals:
    void newFrame(QImage img);

private slots:
    void onNewFrame(QImage img);

private:
    QList<VideoStream *> m_streams;
};

#endif // VIDEOSTREAMMANAGER_H
