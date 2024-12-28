#include "VideoView/VideoGridView.h"
#include "VideoDisplayTooltip.h"

#include <QPainter>
#include <QMenu>
#include <QDebug>

// view层只关注本页面的显示，不关注翻页，翻页由controller层和model层协同完成
// view提供翻页的信号，controller层接收信号，调用model层的翻页函数，将视频数据传递给view层
// 指定的控件显示

VideoGridView::VideoGridView(QWidget *parent) : QWidget(parent)
{
    m_video_grid_ = 0;
    m_selected_index_ = -1;
    m_maximized_index_ = -1;

    m_pool_ = new VideoDisplayUnitPool(16); // 创建控件池
}

int VideoGridView::getGrid()
{
    return m_video_grid_;
}

void VideoGridView::setGrid(VideoGrid grid)
{
    // 如果有控件最大化，先还原
    if (m_maximized_index_ != -1)
    {
        std::lock_guard<std::mutex> lock(mtx_maximized_index_);
        m_maximized_index_ = -1;
    }

    // 更新网格布局
    {
        std::lock_guard<std::mutex> lock(mtx_grid_);
        m_video_grid_ = grid;

        // 重新计算显示位置并更新控件数量
        calculateGridLayout();
    }
    
    update(); // 触发重绘
}

void VideoGridView::calculateGridLayout()
{
    int rows = 0, cols = 0;
    switch (m_video_grid_)
    {
    case VideoGridOne:    rows = 1; cols = 1; break;
    case VideoGridFour:   rows = 2; cols = 2; break;
    case VideoGridNine:   rows = 3; cols = 3; break;
    case VideoGridSixteen: rows = 4; cols = 4; break;
    }

    int spacing = 3;
    int displayWidth = (width() - (cols + 1) * spacing) / cols;
    int displayHeight = (height() - (rows + 1) * spacing) / rows;

    // 缓存子窗口的几何信息
    m_display_geometries_.clear();
    for (int i = 0; i < m_video_grid_; ++i)
    {
        int row = i / cols;
        int col = i % cols;
        int x = col * (displayWidth + spacing) + spacing;
        int y = row * (displayHeight + spacing) + spacing;
        m_display_geometries_.push_back(QRect(x, y, displayWidth, displayHeight));
    }

    // 更新控件数量
    adjustDisplayUnits();
}

void VideoGridView::adjustDisplayUnits()
{
    int numDisplays = m_displayviews_.size() - m_video_grid_;
    if (numDisplays > 0)
    {
        // 释放控件
        for (int i = 0; i < numDisplays; ++i)
        {
            VideoDisplayUnit *display = m_displayviews_.back();
            m_displayviews_.pop_back();
            display->hide();
            // 移除事件过滤器
            display->removeEventFilter(this);
            // 断开子窗口的信号连接
            // disconnect(display, &VideoDisplayUnit::rightClicked, this, &VideoGridView::videoDisplayUnitRightClicked);
            // disconnect(display, &VideoDisplayUnit::clicked, this, &VideoGridView::onVideoDisplayUnitClicked);
            // disconnect(display, &VideoDisplayUnit::requestMaximizeOrRestore, this, &VideoGridView::onVideoDisplayUnitRequestMaximizeOrRestore);
            m_pool_->release(display);
        }
    }
    else if (numDisplays < 0)
    {
        numDisplays = -numDisplays;
        for (int i = 0; i < numDisplays; ++i)
        {
            VideoDisplayUnit *display = m_pool_->acquire(this);
            if (display)
            {
                // 连接子窗口的信号到上层
                // connect(display, &VideoDisplayUnit::rightClicked, this, &VideoGridView::videoDisplayUnitRightClicked);
                // connect(display, &VideoDisplayUnit::clicked, this, &VideoGridView::onVideoDisplayUnitClicked);
                // connect(display, &VideoDisplayUnit::requestMaximizeOrRestore, this, &VideoGridView::onVideoDisplayUnitRequestMaximizeOrRestore);
                // 为子窗口安装事件过滤器
                display->installEventFilter(this);
                display->show();
                m_displayviews_.push_back(display);
            }
        }
    }
}

void VideoGridView::onVideoPlay(int index)
{
    if (index >= 0 && static_cast<size_t>(index) < m_displayviews_.size())
    {
        m_displayviews_[index]->onPlay();
    }
}

void VideoGridView::onVideoClose(int index)
{
    if (index >= 0 && static_cast<size_t>(index) < m_displayviews_.size())
    {
        m_displayviews_[index]->onClose();
    }
}

void VideoGridView::onVideoSetImage(int index, const QImage &image)
{
    if (index >= 0 && static_cast<size_t>(index) < m_displayviews_.size())
    {
        m_displayviews_[index]->setImage(image);
    }
}

void VideoGridView::onVideoDisplayUnitClicked(int id_videodisplayunit)
{
    // qDebug() << "VideoGridView::onVideoDisplayUnitClicked: " << id_videodisplayunit;
    // 检查当前选中项是否已经是点击的控件
    if (m_selected_index_ != -1 && m_displayviews_[m_selected_index_]->getId() == id_videodisplayunit)
    {
        std::lock_guard<std::mutex> lock(mtx_selected_index_);
        m_selected_index_ = -1; // 取消选中
    }
    else
    {
        // 查找点击的控件索引
        for (size_t i = 0; i < m_displayviews_.size(); ++i)
        {
            if (m_displayviews_[i]->getId() == id_videodisplayunit)
            {
                std::lock_guard<std::mutex> lock(mtx_selected_index_);
                m_selected_index_ = i;
                break;
            }
        }
    }
    update(); // 触发重绘
}

void VideoGridView::onVideoDisplayUnitRequestMaximizeOrRestore(int id_videodisplayunit)
{
    // 如果有控件最大化，先还原
    if (m_maximized_index_ != -1)
    {
        std::lock_guard<std::mutex> lock(mtx_maximized_index_);
        m_maximized_index_ = -1; // 重置最大化索引
    }
    else
    {
        // 查找点击的控件索引
        for (size_t i = 0; i < m_displayviews_.size(); ++i)
        {
            if (m_displayviews_[i]->getId() == id_videodisplayunit)
            {
                std::lock_guard<std::mutex> lock(mtx_maximized_index_);
                m_maximized_index_ = i;
                break;
            }
        }
    }

    update(); // 触发重绘
}

bool VideoGridView::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton) {
            qDebug() << "子窗口被点击:" << watched;

            onVideoDisplayUnitClicked(static_cast<VideoDisplayUnit*>(watched)->getId());

            emit videoDisplayUnitClicked(static_cast<VideoDisplayUnit*>(watched)->getId());

            return true; // 表示事件已被处理
        }
        else if (mouseEvent->button() == Qt::RightButton) {
            qDebug() << "子窗口被右击:" << watched;

            emit videoDisplayUnitRightClicked(static_cast<VideoDisplayUnit*>(watched)->getId(), mouseEvent->globalPos());

            return true; // 表示事件已被处理
        }
    }
    else if (event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            qDebug() << "子窗口被双击:" << watched;

            onVideoDisplayUnitRequestMaximizeOrRestore(static_cast<VideoDisplayUnit*>(watched)->getId());

            return true; // 表示事件已被处理
        }
    }

    return QWidget::eventFilter(watched, event);
}

void VideoGridView::paintEvent(QPaintEvent *event)
{
    // static int count = 0;
    // qDebug() << "VideoGridView::paintEvent: " << count++;

    QWidget::paintEvent(event);

    QPainter painter(this);
    // painter.fillRect(rect(), QColor("darkgray"));

    // 获取数据所需的锁
    std::lock_guard<std::mutex> lock_grid(mtx_grid_);
    std::lock_guard<std::mutex> lock_selected_index(mtx_selected_index_);
    std::lock_guard<std::mutex> lock_maximized_index(mtx_maximized_index_);


    if (m_maximized_index_ >= 0 && m_maximized_index_ < m_video_grid_)
    {
        // 显示最大化的控件
        m_displayviews_[m_maximized_index_]->setGeometry(0, 0, width(), height());
        m_displayviews_[m_maximized_index_]->show();
        m_displayviews_[m_maximized_index_]->raise();

        // 隐藏其他控件
        for (size_t i = 0; i < m_displayviews_.size(); ++i)
        {
            if (i != static_cast<size_t>(m_maximized_index_))
            {
                m_displayviews_[i]->hide();
            }
        }
    }
    else
    {
        displayVideoGrid(painter);
    }
}

void VideoGridView::displayVideoGrid(QPainter& painter)
{
    // 应用缓存的几何数据
    for (size_t i = 0; i < m_displayviews_.size() && i < m_display_geometries_.size(); ++i)
    {
        m_displayviews_[i]->setGeometry(m_display_geometries_[i]);
        m_displayviews_[i]->show();
    }

    // 绘制选中框
    if (m_selected_index_ >= 0 && static_cast<size_t>(m_selected_index_) < m_display_geometries_.size())
    {
        painter.setPen(QPen(QColor("lightblue"), 3));
        // painter.setBrush(Qt::NoBrush);  // 无填充
        painter.drawRect(m_display_geometries_[m_selected_index_].adjusted(-2, -2, 1, 1));
    }
}

void VideoGridView::resizeEvent(QResizeEvent *event)
{
    // 重新计算显示位置并更新控件布局
    calculateGridLayout();
    QWidget::resizeEvent(event);
}

