#include "VideoView/VideoPainter.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

VideoPainter::VideoPainter(QWidget *parent) 
    : QWidget(parent)
{

}

VideoPainter::VideoPainter(int id, QWidget *parent)
    : QWidget(parent)
{
    m_id_ = id;
}

void VideoPainter::setImage(const QImage &image)
{
    {
        QMutexLocker locker(&mtx_image_);
        m_image_ = image;
    }
    update();
}

void VideoPainter::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor("#181818"));

    if (!m_image_.isNull()) {
        QSize widgetSize = this->size();      // 获取显示区域的大小
        QSize imageSize = m_image_.size();   // 获取视频帧的大小

        // 计算保持比例的缩放比例
        qreal scaleX = static_cast<qreal>(widgetSize.width()) / imageSize.width();
        qreal scaleY = static_cast<qreal>(widgetSize.height()) / imageSize.height();
        qreal scale = qMin(scaleX, scaleY);  // 保持宽高比的缩放比例

        // 计算缩放后的图像尺寸
        QSize newSize(imageSize.width() * scale, imageSize.height() * scale);
        // 计算将图像居中显示的偏移量
        int xOffset = (widgetSize.width() - newSize.width()) / 2;
        int yOffset = (widgetSize.height() - newSize.height()) / 2;

        // 绘制图像，保持比例并居中
        painter.drawImage(QPoint(xOffset, yOffset), m_image_.scaled(newSize, Qt::KeepAspectRatio));
    }

    QWidget::paintEvent(event);
}
