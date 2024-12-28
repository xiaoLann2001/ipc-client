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
    explicit VideoDisplayUnit(int id, QWidget *parent = nullptr);

    void setImage(const QImage &image);
    int getId() const { return m_id_; }

protected:
    // 绘图事件处理函数
    void paintEvent(QPaintEvent *event) override;
    
    // 与悬浮窗有关事件
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:

public slots:
    void onPlay();      // 播放槽函数
    void onPause();     // 暂停槽函数
    void onClose();     // 停止槽函数

private:
    // 初始化悬浮窗口
    void tooltipInit();

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

    // 悬浮窗口
    VideoDisplayTooltip *m_tooltip_;    // 悬浮窗口
};

#endif // VIDEODISPLAYUNIT_H