#include "VideoView/VideoDisplayUnit.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

VideoDisplayUnit::VideoDisplayUnit(QWidget *parent) 
    : QWidget(parent)
{
    m_isplaying_ = false;
}

VideoDisplayUnit::VideoDisplayUnit(int id, QWidget *parent)
    : QWidget(parent)
{
    m_id_ = id;
    m_isplaying_ = false;
}

void VideoDisplayUnit::setImage(const QImage &image)
{
    {
        QMutexLocker locker(&mtx_image_);
        m_image_ = image;
    }
    update();
    // repaint();
}

void VideoDisplayUnit::onPlay()
{
    {
        QMutexLocker locker(&mtx_playing_);
        m_isplaying_ = true;
    }
    update();
}

void VideoDisplayUnit::onPause()
{
    {
        QMutexLocker locker(&mtx_playing_);
        m_isplaying_ = false;
    }
    update();
}

void VideoDisplayUnit::onClose()
{
    {
        QMutexLocker locker(&mtx_image_);
        // 重置图像和播放状态，以及视频信息
        m_image_ = QImage();
    }
    {
        QMutexLocker locker(&mtx_playing_);
        m_isplaying_ = false;
    }
    m_info_ = "";
    update();
}

void VideoDisplayUnit::paintEvent(QPaintEvent *event)
{
    // static int count = 0;
    // qDebug() << "VideoDisplayUnit::paintEvent: " << count++;

    QPainter painter(this);
    painter.fillRect(this->rect(), QColor("#181818"));

    if (m_isplaying_ && !m_image_.isNull()) {
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
