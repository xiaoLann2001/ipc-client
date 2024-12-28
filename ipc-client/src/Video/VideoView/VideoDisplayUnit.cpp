#include "VideoView/VideoDisplayUnit.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

VideoDisplayUnit::VideoDisplayUnit(int id, QWidget *parent) 
    : QWidget(parent), m_id_(id)
{
    m_isplaying_ = false;

    tooltipInit();
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

void VideoDisplayUnit::tooltipInit()
{
    // 创建悬浮窗
    m_tooltip_ = new VideoDisplayTooltip(this);

    // 设置悬浮窗的 ID
    m_tooltip_->setId(m_id_);

    // 设置悬浮窗的显示文本
    m_tooltip_->setTooltipText("Video ID: " + QString::number(m_id_));

    // 设置悬浮窗的显示模式
    m_tooltip_->setTooltipMode(false);

    // 隐藏悬浮窗
    m_tooltip_->hide();

    // 连接悬浮窗的信号与槽

}

void VideoDisplayUnit::paintEvent(QPaintEvent *event)
{
    // static int count = 0;
    // qDebug() << "VideoDisplayUnit::paintEvent: " << count++;

    QWidget::paintEvent(event);

    // 默认背景为加号
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
}

// void VideoDisplayUnit::mousePressEvent(QMouseEvent *event)
// {
//     if (event->button() == Qt::LeftButton) {
//         // qDebug() << "VideoDisplayUnit::mousePressEvent: " << id_videodisplayunit_;
//         emit clicked(m_id_);     // 发送点击信号并传递 唯一ID
//     }
//     else if (event->button() == Qt::RightButton) {
//         // qDebug() << "VideoDisplayUnit::mousePressEvent: " << id_videodisplayunit_;
//         emit rightClicked(m_id_, event->globalPos());    // 发送右击信号并传递 唯一ID
//     }
//     QWidget::mousePressEvent(event);
// }

// void VideoDisplayUnit::mouseDoubleClickEvent(QMouseEvent *event)
// {
//     if (event->button() == Qt::LeftButton) {
//         emit requestMaximizeOrRestore(m_id_);    // 发送请求最大化或还原信号并传递 唯一ID
//     }
//     QWidget::mouseDoubleClickEvent(event);
// }

void VideoDisplayUnit::enterEvent(QEvent *event)
{
    // 当鼠标进入控件时，显示悬浮窗
    if (!m_tooltip_->isVisible()) {
        // 设置悬浮窗的位置为当前控件的顶部对齐
        // QPoint tooltipPos = this->mapToGlobal(QPoint(0, 0)); // 映射到全局坐标
        m_tooltip_->move(QPoint(0, 0));

        // 置顶并显示悬浮窗
        m_tooltip_->raise();
        m_tooltip_->show();
    }

    QWidget::enterEvent(event);
}

void VideoDisplayUnit::leaveEvent(QEvent *event)
{
    // 当鼠标离开控件时，隐藏悬浮窗
    if (m_tooltip_->isVisible()) {
        m_tooltip_->hide();
    }

    QWidget::leaveEvent(event);
}

void VideoDisplayUnit::resizeEvent(QResizeEvent *event)
{
    // 当控件大小改变时，调整悬浮窗的宽度
    m_tooltip_->setFixedWidth(this->width());
    // m_tooltip_->update();

    QWidget::resizeEvent(event);
}
