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

    UIInit();
}

VideoGridView::~VideoGridView()
{
    delete m_displayUnitPool_;
    delete m_displayTooltipPool_;
}

int VideoGridView::getGrid()
{
    return m_video_grid_;
}

void VideoGridView::setGrid(VideoGrid grid)
{
    if (m_video_grid_ == grid && m_maximized_index_ == -1) // 如果布局未改变且没有控件最大化
        return;

    // 如果有控件最大化，先还原
    if (m_maximized_index_ != -1)
    {
        QMutexLocker locker(&mtx_maximized_index_);
        m_maximized_index_ = -1;
    }

    // 更新网格布局
    {
        QMutexLocker locker(&mtx_grid_);
        m_video_grid_ = grid;

        // 重新计算显示位置并更新控件数量
        calculateGridLayout();
    }
    adjustDisplayUnits();

    // update(); // 触发重绘
}

void VideoGridView::UIInit()
{
    // m_pool_ = new VideoDisplayUnitPool(16); // 创建控件池
    m_displayUnitPool_ = new CustomQWidgetPool<VideoDisplayUnit>(16);       // 创建显示控件池
    m_displayTooltipPool_ = new CustomQWidgetPool<VideoDisplayTooltip>(16); // 创建悬浮窗池

    this->setGrid(VideoGridFour);
}

void VideoGridView::calculateGridLayout()
{
    int rows = 0, cols = 0;
    switch (m_video_grid_)
    {
    case VideoGridOne:
        rows = 1;
        cols = 1;
        break;
    case VideoGridFour:
        rows = 2;
        cols = 2;
        break;
    case VideoGridNine:
        rows = 3;
        cols = 3;
        break;
    case VideoGridSixteen:
        rows = 4;
        cols = 4;
        break;
    default:
        break;
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
}

void VideoGridView::adjustDisplayUnits()
{
    int target_grid = -1;

    {
        QMutexLocker locker(&mtx_grid_);
        target_grid = m_video_grid_;
    }

    if (target_grid <= 0)
    {
        qDebug() << "grid is invalid!";
        return;
    }

    if (target_grid < m_displayviews_.size())
    {
        // 释放控件
        while (target_grid < m_displayviews_.size())
        {
            addDisplayUnit();
        }
    }
    else if (target_grid > m_displayviews_.size())
    {
        while (target_grid > m_displayviews_.size())
        {
            removeDisplayUnit();
        }
    }
}

void VideoGridView::addDisplayUnit()
{
    // 获取显示控件
    VideoDisplayUnit *display = m_displayviews_.back();
    if (!display)
    {
        qDebug() << "VideoGridView::adjustDisplayUnits: acquire display failed!";
        return;
    }

    // 获取悬浮窗控件，悬浮窗控件的父窗口为显示控件
    VideoDisplayTooltip *tooltip = display->findChild<VideoDisplayTooltip *>();
    if (!tooltip)
    {
        qDebug() << "VideoGridView::adjustDisplayUnits: acquire tooltip failed!";
        return;
    }

    // 从显示控件列表中移除
    m_displayviews_.pop_back();

    display->hide();
    display->removeEventFilter(tooltip);
    display->removeEventFilter(this);
    display->setId(-1);

    tooltip->hide();
    tooltip->setTooltipMode(false);
    tooltip->setTooltipText("");
    tooltip->setId(-1);

    m_displayUnitPool_->release(display);
    m_displayTooltipPool_->release(tooltip);
}

void VideoGridView::removeDisplayUnit()
{
    // 获取显示控件
    VideoDisplayUnit *display = m_displayUnitPool_->acquire(this);
    if (!display)
    {
        qDebug() << "VideoGridView::adjustDisplayUnits: acquire display failed!";
        return;
    }

    // 获取悬浮窗控件，设置父窗口为显示控件
    VideoDisplayTooltip *tooltip = m_displayTooltipPool_->acquire(display);
    if (!tooltip)
    {
        qDebug() << "VideoGridView::adjustDisplayUnits: acquire tooltip failed!";
        m_displayUnitPool_->release(display); // 释放显示控件
        return;
    }

    tooltip->setId(m_displayviews_.size());                                    // 设置悬浮窗口 ID
    tooltip->setTooltipText("Video ID: " + QString::number(tooltip->getId())); // 设置悬浮窗口的提示信息
    tooltip->setTooltipMode(false);                                            // 设置悬浮窗口的显示模式为未添加视频
    tooltip->hide();                                                           // 隐藏悬浮窗口

    display->setId(m_displayviews_.size()); // 设置子窗口 ID
    display->installEventFilter(this);      // 安装本窗口的事件过滤器，用于处理子窗口的点击事件
    display->installEventFilter(tooltip);   // 安装悬浮窗的事件过滤器，用于显示悬浮窗
    display->show();                        // 显示子窗口

    m_displayviews_.push_back(display); // 添加显示控件到列表
}

void VideoGridView::onVideoPlay(int index)
{
    if (index >= 0 && index < m_displayviews_.size())
    {
        m_displayviews_[index]->onPlay();
    }
}

void VideoGridView::onVideoClose(int index)
{
    if (index >= 0 && index < m_displayviews_.size())
    {
        m_displayviews_[index]->onClose();
    }
}

void VideoGridView::onVideoSetImage(int index, const QImage &image)
{
    if (index >= 0 && index < m_displayviews_.size())
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
        QMutexLocker locker(&mtx_selected_index_);
        m_selected_index_ = -1; // 取消选中
    }
    else
    {
        // 查找点击的控件索引
        for (int i = 0; i < m_displayviews_.size(); ++i)
        {
            if (m_displayviews_[i]->getId() == id_videodisplayunit)
            {
                QMutexLocker locker(&mtx_selected_index_);
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
        QMutexLocker locker(&mtx_maximized_index_);
        m_maximized_index_ = -1; // 重置最大化索引
    }
    else
    {
        // 查找点击的控件索引
        for (int i = 0; i < m_displayviews_.size(); ++i)
        {
            if (m_displayviews_[i]->getId() == id_videodisplayunit)
            {
                QMutexLocker locker(&mtx_maximized_index_);
                m_maximized_index_ = i;
                break;
            }
        }
    }

    update(); // 触发重绘
}

bool VideoGridView::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            qDebug() << "子窗口被点击:" << watched;

            onVideoDisplayUnitClicked(static_cast<VideoDisplayUnit *>(watched)->getId());

            emit videoDisplayUnitClicked(static_cast<VideoDisplayUnit *>(watched)->getId());

            return true; // 表示事件已被处理
        }
        else if (mouseEvent->button() == Qt::RightButton)
        {
            qDebug() << "子窗口被右击:" << watched;

            emit videoDisplayUnitRightClicked(static_cast<VideoDisplayUnit *>(watched)->getId(), mouseEvent->globalPos());

            return true; // 表示事件已被处理
        }
    }
    else if (event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            qDebug() << "子窗口被双击:" << watched;

            onVideoDisplayUnitRequestMaximizeOrRestore(static_cast<VideoDisplayUnit *>(watched)->getId());

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

    QVector<QRect> geometries;
    int grid;
    int maximizedIndex;
    int selectedIndex;

    {
        QMutexLocker locker_grid(&mtx_grid_);
        QMutexLocker locker_maximized_index(&mtx_maximized_index_);
        QMutexLocker locker_selected_index(&mtx_selected_index_);

        geometries = m_display_geometries_;
        grid = m_video_grid_;
        maximizedIndex = m_maximized_index_;
        selectedIndex = m_selected_index_;
    }

    if (maximizedIndex >= 0 && maximizedIndex < grid)
    {
        // 显示最大化的控件
        m_displayviews_[maximizedIndex]->setGeometry(0, 0, width(), height());
        m_displayviews_[maximizedIndex]->show();
        m_displayviews_[maximizedIndex]->raise();

        // 隐藏其他控件
        for (int i = 0; i < m_displayviews_.size(); ++i)
        {
            if (i != maximizedIndex)
            {
                m_displayviews_[i]->hide();
            }
        }
    }
    else
    {
        // 应用缓存的几何数据到显示控件
        for (int i = 0; i < m_displayviews_.size() && i < geometries.size(); ++i)
        {
            m_displayviews_[i]->setGeometry(geometries[i]);
            m_displayviews_[i]->show();
        }

        // 绘制选中框
        if (selectedIndex >= 0 && selectedIndex < geometries.size())
        {
            painter.setPen(QPen(QColor("lightblue"), 3));
            // painter.setBrush(Qt::NoBrush);  // 无填充
            painter.drawRect(geometries[selectedIndex].adjusted(-2, -2, 1, 1));
        }
    }
}

void VideoGridView::resizeEvent(QResizeEvent *event)
{
    // 重新计算显示位置并更新控件布局
    {
        QMutexLocker locker(&mtx_grid_);
        calculateGridLayout();
    }
    // adjustDisplayUnits();
    QWidget::resizeEvent(event);
}
