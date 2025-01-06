#include <QDebug>

#include "VideoPlayController/VideoPlayController.h"
#include "VideoSignalBus.h"

VideoPlayController::VideoPlayController(QObject *parent)
    : QObject(parent) {}

VideoPlayController::~VideoPlayController() {}

void VideoPlayController::play() {
    playbackState.storeRelease(true);   // 设置播放状态为播放
    qDebug() << "playbackState: Playing";
    emit VideoSignalBus::instance()->videoControlResponse(
        VideoControlResponse(m_videoDisplayUnitId, 
        VideoControlResponse::Command::Play, 
        VideoControlResponse::Response::Success));
}

void VideoPlayController::pause() {
    playbackState.storeRelease(false);  // 设置播放状态为暂停
    qDebug() << "playbackState: Paused";
    emit VideoSignalBus::instance()->videoControlResponse(
        VideoControlResponse(m_videoDisplayUnitId, 
        VideoControlResponse::Command::Pause, 
        VideoControlResponse::Response::Success));
}

void VideoPlayController::stop() {
    playbackState.storeRelease(false);  // 设置播放状态为暂停
    emit VideoSignalBus::instance()->videoFrame(m_videoDisplayUnitId, QImage());    // 发送空帧到视频播放控制器
    qDebug() << "playbackState: Stopped";
}

void VideoPlayController::onVideoFrameToControl(const QImage &image) {
    if (playbackState.loadAcquire()) {
        // 发送视频帧到视频播放控制器
        emit VideoSignalBus::instance()->videoFrame(m_videoDisplayUnitId, image);
    }
}

