#ifndef AUDIOHELPER_H
#define AUDIOHELPER_H

#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QString>
#include <QtMath>

class AudioHelper
{
public:
    // 初始化音频格式
    static void initAudioFormat(QAudioFormat &format, int sampleRate, int channelCount, int sampleSize)
    {
        format.setSampleRate(sampleRate);
        format.setChannelCount(channelCount);
        format.setSampleSize(sampleSize);
        format.setCodec("audio/pcm");
        format.setByteOrder(QAudioFormat::LittleEndian);
        // format.setSampleType(QAudioFormat::UnSignedInt);
        format.setSampleType(QAudioFormat::SignedInt);
    }

    // 获取音频设备信息（输入或输出）
    static QAudioDeviceInfo getAudioDevice(const QString &deviceName, bool input)
    {
        QList<QAudioDeviceInfo> devices;
        if (input) {
            devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
        } else {
            devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
        }

        for (const QAudioDeviceInfo &device : devices) {
            if (device.deviceName() == deviceName) {
                return device;
            }
        }

        // 如果没找到对应设备，返回默认设备
        return (input ? QAudioDeviceInfo::defaultInputDevice() : QAudioDeviceInfo::defaultOutputDevice());
    }
};

#endif // AUDIOHELPER_H
