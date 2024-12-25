#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

// #include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QString>

#include "AudioHelper.h"

class AudioPlayer : public QObject
{
    Q_OBJECT

public:
    AudioPlayer(QObject *parent = nullptr);
    ~AudioPlayer();

public slots:
    // void openAudioInput(int sampleRate, int channelCount, int sampleSize);
    // void openAudioInput(const QString &deviceName, int sampleRate, int channelCount, int sampleSize);
    // void closeAudioInput();

    bool openAudioOutput(int sampleRate, int channelCount, int sampleSize);
    bool openAudioOutput(const QString &deviceName, int sampleRate, int channelCount, int sampleSize);
    void closeAudioOutput();

    void playAudioFrame(const QByteArray &audioData);    // 播放音频帧

private:
    QAudioOutput *audioOutput = nullptr;
    QIODevice *deviceOutput = nullptr;
    bool muted = false;
    qreal audioLevel = 0.0;
};

#endif // AUDIOPLAYER_H
