#include "videostreammanager.h"
#include <QDebug>

VideoStreamManager::VideoStreamManager(QObject *parent)
    : QObject(parent) {}

VideoStreamManager::~VideoStreamManager() {
    for (auto stream : m_streams) {
        stream->stop();
        delete stream;
    }
}

void VideoStreamManager::addStream(const QString &url) {
    VideoStream *stream = new VideoStream(url);
    connect(stream, &VideoStream::newFrame, this, &VideoStreamManager::onNewFrame);
    m_streams.append(stream);
    stream->start();
}

void VideoStreamManager::removeStream(const QString &url) {
    for (auto stream : m_streams) {
        if (stream->url() == url) {
            stream->stop();
            m_streams.removeAll(stream);
            delete stream;
            break;
        }
    }
}

void VideoStreamManager::onNewFrame(QImage img) {
    emit newFrame(img);
}
