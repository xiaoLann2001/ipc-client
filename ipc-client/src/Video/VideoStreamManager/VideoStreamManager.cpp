#include "VideoStreamManager/VideoStreamManager.h"
#include <QDebug>

VideoStreamManager::VideoStreamManager(QObject *parent) : QObject(parent) {}

VideoStreamManager::~VideoStreamManager() { 
    // 获取所有句柄到列表，避免迭代过程中删除元素
    QList<int> handles;
    {
        QMutexLocker locker(&m_mutex_streamMap);
        handles = m_streams.keys();
    }
    // 删除所有视频流
    for (int handle : handles) {
        deleteVideoStream(handle);
    }
}

int VideoStreamManager::createVideoStream(const QString &url) {
    // 创建解码器
    VideoStreamDecoder *decoder = new VideoStreamDecoder(url);

    // // 获取视频流信息
    // VideoStreamInfo *info = decoder->getVideoStreamInfo();

    // // 创建音频播放器
    // AudioPlayer *audioPlayer = new AudioPlayer(this);
    // if (audioPlayer->openAudioOutput(info->m_audioSampleRate, info->m_audioChannels, info->m_audioSampleDepth)) {
    //     qDebug() << "Audio output opened";
    // } else {
    //     qDebug() << "Failed to open audio output";
    //     audioPlayer->deleteLater();
    //     audioPlayer = nullptr;
    // }

    // 生成句柄
    int handle = generateHandle();
    
    // 将解码器存储到映射中
    m_streams[handle] = decoder;

    // // 将音频播放器存储到映射中
    // if (audioPlayer) {
    //     m_audioPlayers[handle] = audioPlayer;
    // }

    // 连接解码器的 finished 信号到 清理操作
    connect(decoder, &QThread::finished, [handle, this]() {
        qDebug() << "Decoder finished: " << handle;
        this->deleteVideoStream(handle);
    });

    // 连接解码器的视频帧信号到信号
    connect(decoder, &VideoStreamDecoder::videoFrameReady, [this, handle]() {
        emit newFrameAvailable(handle);
    });

    // // 连接解码器的音频帧信号到槽函数
    // if (audioPlayer) {
    //     connect(decoder, &VideoStreamDecoder::audioFrameReady, [this, handle](){
    //         QByteArray audioData = m_streams[handle]->getDecodedAudio();
    //         if (!audioData.isEmpty()) {
    //             // 发送音频数据到音频播放器
    //             m_audioPlayers[handle]->playAudioFrame(audioData);
    //         }
    //     });
    // }

    // 启动解码器
    decoder->start();

    // 返回句柄
    return handle;
}

void VideoStreamManager::deleteVideoStream(int handle) {
    // 保护 m_streams 的互斥锁
    QMutexLocker locker(&m_mutex_streamMap);

    if (m_streams.contains(handle)) {
        VideoStreamDecoder *decoder = m_streams.take(handle);
        if (decoder) {
            // 断开所有连接，防止自动删除
            disconnect(decoder, nullptr, nullptr, nullptr); 
            // 删除解码器
            decoder->deleteLater();
        }
        m_streams.remove(handle);
        recycleHandle(handle);
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

QImage VideoStreamManager::getDecodedImage(int handle) {
    if (m_streams.contains(handle)) {
        return m_streams[handle]->getDecodedImage();
    }
    return QImage();
}
