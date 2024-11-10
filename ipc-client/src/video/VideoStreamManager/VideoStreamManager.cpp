#include "VideoStreamManager/VideoStreamManager.h"
#include <QDebug>

QSet<int> VideoStreamManager::recycledHandles;  // 静态成员变量初始化

VideoStreamManager::VideoStreamManager(QObject *parent) : QObject(parent) {}

VideoStreamManager::~VideoStreamManager() {
    // 停止并清理所有视频流
    for (auto it = m_streams.begin(); it != m_streams.end(); ++it) {
        stopVideoStream(it.key());
    }
}

int VideoStreamManager::createVideoStream(const QString &url) {
    int handle = generateHandle();
    VideoStreamDecoder *decoder = new VideoStreamDecoder(url, this);

    // 将解码器存储到映射中
    m_streams[handle] = decoder;

    // 连接解码器的新帧信号到槽函数
    connect(decoder, &VideoStreamDecoder::newFrameAvailable, [this, handle]() {
        emit newFrameAvailable(handle);
    });
    
    // 启动解码器
    decoder->start();

    // 返回句柄
    return handle;
}

void VideoStreamManager::stopVideoStream(int handle) {
    if (m_streams.contains(handle)) {       // 如果句柄对应的解码器存在
        VideoStreamDecoder *decoder = m_streams[handle];    // 获取解码器
        decoder->stop();                    // 停止解码器
        delete decoder;                     // 删除解码器
        m_streams.remove(handle);           // 从映射中移除
        recycleHandle(handle);              // 回收句柄
    }
}

VideoStreamInfo* VideoStreamManager::getVideoStreamInfo(int handle) {   // 获取视频流信息
    return m_streams[handle]->getVideoStreamInfo();
}

int VideoStreamManager::generateHandle() {
    static int nextHandle = 0;          // 下一个句柄

    if (!recycledHandles.isEmpty()) {   // 如果有可用的回收句柄
        // 获取并移除一个回收的句柄
        int handle = *recycledHandles.begin();
        recycledHandles.remove(handle);
        return handle;
    } else {
        // 如果没有可用的回收句柄，创建新句柄
        return nextHandle++;
    }
}

// 回收句柄的函数
void VideoStreamManager::recycleHandle(int handle) {
    recycledHandles.insert(handle);
}

QImage VideoStreamManager::getDecodedFrame(int handle) {
    if (m_streams.contains(handle)) {
        return m_streams[handle]->getDecodedFrame();
    }
    return QImage();
}