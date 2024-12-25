#include <QDebug>

#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(QObject *parent)
{
}

AudioPlayer::~AudioPlayer()
{
    // closeAudioInput();
    closeAudioOutput();
}

bool AudioPlayer::openAudioOutput(int sampleRate, int channelCount, int sampleSize)
{
    return openAudioOutput("", sampleRate, channelCount, sampleSize);
}

bool AudioPlayer::openAudioOutput(const QString &deviceName, int sampleRate, int channelCount, int sampleSize)
{
    closeAudioOutput();
    QAudioFormat format;
    AudioHelper::initAudioFormat(format, sampleRate, channelCount, sampleSize);

    if (deviceName.isEmpty()) {
        audioOutput = new QAudioOutput(format, this);
    } else {
        QAudioDeviceInfo device = AudioHelper::getAudioDevice(deviceName, false);
        if (!device.isFormatSupported(format)) {
            qDebug() << "Audio format not supported by backend, cannot open audio output";
            return false;
        }
        audioOutput = new QAudioOutput(device, format, this);
    }

    deviceOutput = audioOutput->start();
    if (!deviceOutput) {
        qDebug() << "Failed to open audio output";
        return false;
    }

    return true;
}

void AudioPlayer::closeAudioOutput()
{
    muted = false;
    if (audioOutput) {
        audioOutput->stop();
        audioOutput->deleteLater();
        audioOutput = nullptr;
    }
}

void AudioPlayer::playAudioFrame(const QByteArray &audioData)
{
    if (!audioOutput || !deviceOutput) {
        return;
    }

    if (muted) {
        return;
    }

    qint64 bytesWritten = deviceOutput->write(audioData);
    if (bytesWritten < 0) {
        qDebug() << "Error writing audio data to output device";
    }
}