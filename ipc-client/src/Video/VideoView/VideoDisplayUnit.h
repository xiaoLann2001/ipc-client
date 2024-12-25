#ifndef VIDEODISPLAYUNIT_H
#define VIDEODISPLAYUNIT_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <QGestureEvent>
#include <mutex>

class VideoDisplayUnit : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDisplayUnit(int id, QWidget *parent = nullptr);
    void setImage(const QImage &image);
    int getId() const { return m_id_; }
    void updateTooltip();
    void setTooltipLabel(const QString &text) { m_label_->setText(text); }

protected:
    // 绘图事件处理函数
    void paintEvent(QPaintEvent *event) override;
    
    // 与悬浮窗有关事件
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QImage m_image_;

    int m_id_ = -1;                 // 唯一 ID
    bool m_ismaximized_ = false;    // 是否最大化
    bool m_isplaying_ = false;      // 是否正在播放
    std::mutex mtx_image_;          // 图像互斥锁

    QWidget *m_tooltip_;    // 悬浮窗口
    QLabel *m_label_;       // 悬浮窗口标签

signals:

public slots:
    void onPlay();      // 播放槽函数
    void onStop();      // 停止槽函数
};

#endif // VIDEODISPLAYUNIT_H