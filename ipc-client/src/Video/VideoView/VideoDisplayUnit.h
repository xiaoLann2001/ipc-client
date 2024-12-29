#ifndef VIDEODISPLAYUNIT_H
#define VIDEODISPLAYUNIT_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <QGestureEvent>
#include <QMutex>

#include "VideoDisplayTooltip.h"

class VideoDisplayUnit : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDisplayUnit(QWidget *parent = nullptr);
    explicit VideoDisplayUnit(int id, QWidget *parent = nullptr);

    void setId(int id) { m_id_ = id; }
    void setInfo(const QString &info) { m_info_ = info; }
    void setImage(const QImage &image);
    int getId() const { return m_id_; }

protected:
    // 绘图事件处理函数
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:
    void onPlay();      // 播放槽函数
    void onPause();     // 暂停槽函数
    void onClose();     // 停止槽函数

private:

    // 窗口标识
    int m_id_ = -1;                 // 唯一 ID

    // 视频信息
    QString m_info_;                // 视频信息，url，分辨率，码率等

    // 显示的图像
    QImage m_image_;
    QMutex mtx_image_;              // 互斥锁，保护图像

    // 窗口控制信息
    bool m_isplaying_ = false;      // 是否正在播放
    QMutex mtx_playing_;            // 互斥锁，保护播放状态
};

#endif // VIDEODISPLAYUNIT_H