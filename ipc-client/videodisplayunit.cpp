#include "videodisplayunit.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

VideoDisplayUnit::VideoDisplayUnit(int id, QWidget *parent) : QWidget(parent)
{
    m_id_ = id;
    m_ismaximized_ = false;

    // 初始化悬浮窗，默认不可见
    m_tooltip_ = new QWidget(this);
    m_tooltip_->setStyleSheet("background-color: rgba(0, 0, 0, 200); color: white; font-size: 12px; border-radius: 5px;");
    m_tooltip_->setFixedHeight(20);  // 悬浮窗高度固定为20
    m_tooltip_->hide();  // 默认悬浮窗是隐藏的

    // 添加标签显示信息
    m_label_ = new QLabel("Video ID: " + QString::number(id), m_tooltip_);
    m_label_->setStyleSheet("color: white; font-size: 12px;");
    m_label_->move(5, 2);  // 设置文本的位置
}

void VideoDisplayUnit::setImage(const QImage &image)
{
    m_image_ = image;
    update();
}

void VideoDisplayUnit::updateTooltip()
{
    // 显示悬浮窗
    m_tooltip_->setFixedWidth(this->width());  // 设置悬浮窗宽度为当前控件宽度
    m_tooltip_->move(0, 0);  // 将悬浮窗放置在控件顶部
    m_tooltip_->show();  // 显示悬浮窗

    // 将悬浮窗提升到最前面
    m_tooltip_->raise();  
}

void VideoDisplayUnit::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(this->rect(), QColor("#1e1e1e"));

    if (!m_image_.isNull()) {
        painter.drawImage(0, 0, m_image_);
    }
}

void VideoDisplayUnit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // qDebug() << "VideoDisplayUnit::mousePressEvent: " << id_videodisplayunit_;
        emit clicked(m_id_);     // 发送点击信号并传递 唯一ID
    }
    QWidget::mousePressEvent(event);
}

void VideoDisplayUnit::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit requestMaximizeOrRestore(m_id_);    // 发送请求最大化或还原信号并传递 唯一ID
    }
    QWidget::mouseDoubleClickEvent(event);
}

void VideoDisplayUnit::enterEvent(QEvent *event)
{
    updateTooltip();  // 鼠标进入控件时，显示悬浮窗

    QWidget::enterEvent(event);
}

void VideoDisplayUnit::leaveEvent(QEvent *event)
{
    // 当鼠标离开控件时，隐藏悬浮窗
    m_tooltip_->hide();  // 隐藏悬浮窗

    QWidget::leaveEvent(event);
}

void VideoDisplayUnit::resizeEvent(QResizeEvent *event)
{
    // 当控件大小改变时，调整悬浮窗的宽度
    m_tooltip_->setFixedWidth(this->width());

    QWidget::resizeEvent(event);
}
