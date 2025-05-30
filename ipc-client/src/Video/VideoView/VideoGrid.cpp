#include "VideoView/VideoGrid.h"
#include "VideoToolbar.h"

#include <QPainter>
#include <QMenu>
#include <QDebug>

// view层只关注本页面的显示，不关注翻页，翻页由controller层和model层协同完成
// view提供翻页的信号，controller层接收信号，调用model层的翻页函数，将视频数据传递给view层
// 指定的控件显示
VideoGrid::VideoGrid(QWidget *parent) : QWidget(parent)
{
    m_video_grid_ = 0;
    m_selected_index_ = -1;
    m_maximized_index_ = -1;

    UIInit();
    controlInit();

#if TESE_REPAINT_COUNT
    connect(&m_test_timer_, &QTimer::timeout, this, [this]()
            {
        qDebug() << "VideoGrid::m_test_repaint_count_: " << m_test_repaint_count_;
        m_test_repaint_count_ = 0; });
    m_test_timer_.start(1000);
#endif
}

VideoGrid::~VideoGrid()
{
    delete m_displayUnitPool_;
    delete m_displayTooltipPool_;
}

int VideoGrid::getGrid()
{
    return m_video_grid_;
}

void VideoGrid::setGrid(VideoGridNum grid)
{
    if (m_video_grid_ == grid && m_maximized_index_ == -1) // 如果布局未改变且没有控件最大化
        return;

    // 如果有控件最大化，先还原
    if (m_maximized_index_ != -1)
    {
        QMutexLocker locker(&mtx_maximized_index_);
        m_maximized_index_ = -1;
    }

    // 如果有控件选中，先取消选中
    if (m_selected_index_ != -1)
    {
        QMutexLocker locker(&mtx_selected_index_);
        m_selected_index_ = -1;
    }

    // 更新网格布局
    {
        QMutexLocker locker(&mtx_grid_);
        m_video_grid_ = grid;

        // 重新计算显示位置并更新控件数量
        calculateGridLayout();
    }
    adjustDisplayUnits();

    update(); // 触发重绘
}

void VideoGrid::UIInit()
{
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setAttribute(Qt::WA_NoSystemBackground, true);

    m_displayUnitPool_ = new CustomQWidgetPool<VideoPainter>(16);    // 创建显示控件池
    m_displayTooltipPool_ = new CustomQWidgetPool<VideoToolbar>(16); // 创建悬浮窗池

    this->setGrid(VideoGridFour);
}

void VideoGrid::controlInit()
{
    // 连接信号总线的视频控制信号到槽函数
    connect(VideoSignalBus::instance(), &VideoSignalBus::videoControlSignal, [this](const VideoControlCommand &command) {
        if (command.cmd == VideoControlCommand::Command::Fullscreen ||
            command.cmd == VideoControlCommand::Command::Restore) {
            this->onVideoMaximizeOrRestore(command.id);
        } });

    // 连接信号总线的视频帧信号到槽函数
    connect(VideoSignalBus::instance(), &VideoSignalBus::videoFrame, this, &VideoGrid::onVideoSetImage);
}

void VideoGrid::calculateGridLayout()
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

    int spacing = 2;
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

void VideoGrid::adjustDisplayUnits()
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
            removeDisplayUnit();
        }
    }
    else if (target_grid > m_displayviews_.size())
    {
        // 添加控件
        while (target_grid > m_displayviews_.size())
        {
            addDisplayUnit();
        }
    }
}

void VideoGrid::removeDisplayUnit()
{
    // 获取显示控件
    VideoPainter *display = m_displayviews_.back();
    if (!display)
    {
        qDebug() << "VideoGrid::adjustDisplayUnits: acquire display failed!";
        return;
    }

    // 获取悬浮窗控件，悬浮窗控件的父窗口为显示控件
    VideoToolbar *toolbar = display->findChild<VideoToolbar *>();
    if (!toolbar)
    {
        qDebug() << "VideoGrid::adjustDisplayUnits: acquire tooltip failed!";
        return;
    }

    // 从显示控件列表中移除
    m_displayviews_.pop_back();

    display->hide();
    display->removeEventFilter(toolbar);
    display->removeEventFilter(this);
    display->setId(-1);

    toolbar->hide();
    toolbar->setToolbarMode(false);
    toolbar->setToolbarText("");
    toolbar->setId(-1);

    m_displayUnitPool_->release(display);
    m_displayTooltipPool_->release(toolbar);
}

void VideoGrid::addDisplayUnit()
{
    // 获取显示控件
    VideoPainter *display = m_displayUnitPool_->acquire(this);
    if (!display)
    {
        qDebug() << "VideoGrid::adjustDisplayUnits: acquire display failed!";
        return;
    }

    // 获取悬浮窗控件，设置父窗口为显示控件
    VideoToolbar *toolbar = m_displayTooltipPool_->acquire(display);
    if (!toolbar)
    {
        qDebug() << "VideoGrid::adjustDisplayUnits: acquire tooltip failed!";
        m_displayUnitPool_->release(display); // 释放显示控件
        return;
    }

    toolbar->setId(m_displayviews_.size());                                    // 设置悬浮窗口 ID
    toolbar->setToolbarText("Video ID: " + QString::number(toolbar->getId())); // 设置悬浮窗口的提示信息
    toolbar->setToolbarMode(false);                                            // 设置悬浮窗口的显示模式为未添加视频
    toolbar->hide();                                                           // 隐藏悬浮窗口

    display->setId(m_displayviews_.size()); // 设置子窗口 ID
    display->installEventFilter(this);      // 安装本窗口的事件过滤器，用于处理子窗口的点击事件
    display->installEventFilter(toolbar);   // 安装悬浮窗的事件过滤器，用于联动悬浮窗的宽度和位置
    display->show();                        // 显示子窗口

    m_displayviews_.push_back(display); // 添加显示控件到列表
}

void VideoGrid::onVideoSetImage(int index, const QImage &image)
{
    if (index >= 0 && index < m_displayviews_.size())
    {
        m_displayviews_[index]->setImage(image);
    }
}

void VideoGrid::onVideoClicked(int id_videodisplayunit)
{
    // qDebug() << "VideoGrid::onVideoDisplayUnitClicked: " << id_videodisplayunit;
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

void VideoGrid::onVideoMaximizeOrRestore(int id_videodisplayunit)
{
    // 如果有控件最大化，先还原
    if (m_maximized_index_ != -1)
    {
        QMutexLocker locker(&mtx_maximized_index_);
        m_maximized_index_ = -1; // 重置最大化索引
        emit VideoSignalBus::instance()->videoControlResponse(
             VideoControlResponse(m_displayviews_[id_videodisplayunit]->getId(),
             VideoControlResponse::Command::Restore,
             VideoControlResponse::Response::Success));
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
                emit VideoSignalBus::instance()->videoControlResponse(
                    VideoControlResponse(m_displayviews_[id_videodisplayunit]->getId(),
                    VideoControlResponse::Command::Fullscreen,
                    VideoControlResponse::Response::Success));
                break;
            }
        }
    }

    update(); // 触发重绘
}

bool VideoGrid::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            qDebug() << "子窗口被点击:" << watched;

            onVideoClicked(static_cast<VideoPainter *>(watched)->getId());

            emit videoClicked(static_cast<VideoPainter *>(watched)->getId());

            return true; // 表示事件已被处理
        }
        else if (mouseEvent->button() == Qt::RightButton)
        {
            qDebug() << "子窗口被右击:" << watched;

            emit videoRightClicked(static_cast<VideoPainter *>(watched)->getId(), mouseEvent->globalPos());

            return true; // 表示事件已被处理
        }
    }
    else if (event->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            qDebug() << "子窗口被双击:" << watched;

            onVideoMaximizeOrRestore(static_cast<VideoPainter *>(watched)->getId());

            return true; // 表示事件已被处理
        }
    }

    return QWidget::eventFilter(watched, event);
}

void VideoGrid::paintEvent(QPaintEvent *event)
{
#if TESE_REPAINT_COUNT
    // 测试重绘次数
    m_test_repaint_count_++;
#endif

    QWidget::paintEvent(event);

    QPainter painter(this);
    // painter.fillRect(rect(), QColor("darkgray"));
    painter.fillRect(rect(), QColor("#383838"));

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
            painter.setPen(QPen(QColor("lightblue"), 2));
            // painter.setBrush(Qt::NoBrush);  // 无填充
            painter.drawRect(geometries[selectedIndex].adjusted(-1, -1, 1, 1));
        }
    }
}

void VideoGrid::resizeEvent(QResizeEvent *event)
{
    // 重新计算显示位置并更新控件布局
    {
        QMutexLocker locker(&mtx_grid_);
        calculateGridLayout();
    }
    // adjustDisplayUnits();
    QWidget::resizeEvent(event);
}
