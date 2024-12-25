#include "VideoStreamManager/VideoStreamManager.h"
#include <QDebug>

QSet<int> VideoStreamManager::recycledHandles;  // 静态成员变量初始化

VideoStreamManager::VideoStreamManager(QObject *parent) : QObject(parent) {}

VideoStreamManager::~VideoStreamManager() {
    // 停止并清理所有视频流
    for (auto it = m_streams.begin(); it != m_streams.end(); ++it) {
        deleteVideoStream(it.key());
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
    if (m_streams.contains(handle)) {       // 如果句柄对应的解码器存在
        VideoStreamDecoder *decoder = m_streams[handle];    // 获取解码器
        if (decoder->isRunning()) {         // 如果解码器正在运行
            decoder->stop();                // 停止解码器
            decoder->wait();                // 等待解码器线程结束
        }
        decoder->deleteLater();             // 安全删除解码器
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

QImage VideoStreamManager::getDecodedImage(int handle) {
    if (m_streams.contains(handle)) {
        return m_streams[handle]->getDecodedImage();
    }
    return QImage();
}
