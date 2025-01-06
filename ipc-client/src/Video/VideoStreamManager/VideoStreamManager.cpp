#include "VideoStreamManager/VideoStreamManager.h"
#include <QDebug>

VideoStreamManager::VideoStreamManager(QObject *parent) : QObject(parent) {}

VideoStreamManager::~VideoStreamManager()
{

}

int VideoStreamManager::createVideoStream(const QString &url)
{
    // 创建解码器
    VideoStreamDecoder *decoder = new VideoStreamDecoder(url);

    // 创建句柄，关联资源
    int handle = handleManager.createHandle(decoder);

    // 连接解码器的 finished 信号到 清理操作
    connect(decoder, &QThread::finished, [handle, this]()
    {
        qDebug() << "Decoder finished: " << handle;
        this->deleteVideoStream(handle); 
    });

    // 连接解码器的视频帧信号到信号
    connect(decoder, &VideoStreamDecoder::videoFrameReady, [this, handle]()
    { 
        emit newFrameAvailable(handle); 
    });

    // 启动解码器
    decoder->start();

    // 返回句柄
    return handle;
}

void VideoStreamManager::deleteVideoStream(int handle)
{
    // 获取解码器
    VideoStreamDecoder *decoder = handleManager.getResource(handle);
    if (!decoder)
    {
        qWarning() << "Decoder not found: " << handle;
        return;
    }

    // 断开所有连接，防止自动删除
    disconnect(decoder, nullptr, nullptr, nullptr);

    // 释放句柄
    handleManager.releaseHandle(handle); // 资源由共享指针管理，无需手动释放
}

VideoStreamInfo *VideoStreamManager::getVideoStreamInfo(int handle)
{ // 获取视频流信息
    VideoStreamDecoder *decoder = handleManager.getResource(handle);
    if (!decoder)
    {
        return nullptr;
    }
    return decoder->getVideoStreamInfo();
}

QImage VideoStreamManager::getDecodedImage(int handle)
{
    VideoStreamDecoder *decoder = handleManager.getResource(handle);
    if (!decoder)
    {
        return QImage();
    }
    return handleManager.getResource(handle)->getDecodedImage();
}
