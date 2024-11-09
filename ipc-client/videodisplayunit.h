#ifndef VIDEODISPLAYUNIT_H
#define VIDEODISPLAYUNIT_H

#include <QWidget>
#include <QLabel>

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
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void enterEvent(QEvent *event) override;

    void leaveEvent(QEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private:
    QImage m_image_;

    int m_id_ = -1;                 // 唯一 ID
    bool m_ismaximized_ = false;    // 是否最大化

    QWidget *m_tooltip_;    // 悬浮窗口
    QLabel *m_label_;       // 悬浮窗口标签

signals:
    void clicked(int m_id_);    // 点击信号, 传递唯一 ID

    void requestMaximizeOrRestore(int m_id_);   // 请求最大化或还原信号, 传递唯一 ID

public slots:
};

#endif // VIDEODISPLAYUNIT_H