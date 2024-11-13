#include "VideoView/VideoDisplayUnit.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

VideoDisplayUnit::VideoDisplayUnit(int id, QWidget *parent) : QWidget(parent)
{
    m_id_ = id;
    m_ismaximized_ = false;
    m_isplaying_ = false;

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
    // repaint();
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

void VideoDisplayUnit::onPlay()
{
    std::lock_guard<std::mutex> lock(mtx_image_);
    m_isplaying_ = true;
    update();
}

void VideoDisplayUnit::onStop()
{
    std::lock_guard<std::mutex> lock(mtx_image_);
    m_isplaying_ = false;
    update();
}

void VideoDisplayUnit::paintEvent(QPaintEvent *event)
{
    // static int count = 0;
    // qDebug() << "VideoDisplayUnit::paintEvent: " << count++;

    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor("#181818"));

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

void VideoDisplayUnit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // qDebug() << "VideoDisplayUnit::mousePressEvent: " << id_videodisplayunit_;
        emit clicked(m_id_);     // 发送点击信号并传递 唯一ID
    }
    else if (event->button() == Qt::RightButton) {
        // qDebug() << "VideoDisplayUnit::mousePressEvent: " << id_videodisplayunit_;
        emit rightClicked(m_id_, event->globalPos());    // 发送右击信号并传递 唯一ID
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
