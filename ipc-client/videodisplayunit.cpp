#include "videodisplayunit.h"

#include <QPainter>
#include <QMouseEvent>

VideoDisplayUnit::VideoDisplayUnit(int id, QWidget *parent) : QWidget(parent)
{
    id_videodisplayunit_ = id;
}

void VideoDisplayUnit::setImage(const QImage &image)
{
    m_image_ = image;
    update();
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
        emit clicked(id_videodisplayunit_);     // 发送点击信号并传递 唯一ID
    }
    QWidget::mousePressEvent(event);
}
