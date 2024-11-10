#include "videoview/videogridview.h"

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

    m_pool_ = new VideoDisplayViewPool(16); // 创建控件池
}

void VideoGridView::setGrid(VideoGrid grid)
{
    // 如果有控件最大化，先还原
    if (m_maximized_index_ != -1)
    {
        std::lock_guard<std::mutex> lock(mtx_maximized_index_);
        m_maximized_index_ = -1; // 重置最大化索引
    }

    // 计算需要释放的控件数量，采取增量更新方式
    int numDisplays = 0;
    {
        std::lock_guard<std::mutex> lock(mtx_grid_);
        numDisplays = m_video_grid_ - grid;
    }

    if (numDisplays == 0) // 不需要增加或释放控件
    {
        update(); // 触发重绘
        return;
    }
    else if (numDisplays > 0)    // 需要释放控件
    {
        for (int i = 0; i < numDisplays; ++i)
        {
            VideoDisplayUnit *display = m_displayviews_.back();
            m_displayviews_.pop_back();
            display->hide(); // 隐藏控件，以免影响布局
            // 如果不断开点击信号连接，会导致点击信号触发时，槽函数被调用多次，可能会让有些控件连接不到槽函数
            disconnect(display, &VideoDisplayUnit::clicked,
                        this, &VideoGridView::onVideoDisplayUnitClicked); // 断开点击信号连接
            disconnect(display, &VideoDisplayUnit::rightClicked,
                        this, &VideoGridView::onVideoDisplayUnitRightClicked); // 断开右击信号连接
            disconnect(display, &VideoDisplayUnit::requestMaximizeOrRestore,
                        this, &VideoGridView::onVideoDisplayUnitRequestMaximizeOrRestore); // 断开最大化信号连接
            m_pool_->release(display);                                                    // 将控件释放回控件池
        }
    }
    else if (numDisplays < 0)   // 需要增加控件
    {
        numDisplays = -numDisplays;
        for (int i = 0; i < numDisplays; ++i)
        {
            VideoDisplayUnit *display = m_pool_->acquire(this); // 从控件池获取控件，设置父控件为当前控件
            if (display)
            {
                connect(display, &VideoDisplayUnit::clicked,
                        this, &VideoGridView::onVideoDisplayUnitClicked); // 连接点击信号
                connect(display, &VideoDisplayUnit::rightClicked,
                        this, &VideoGridView::onVideoDisplayUnitRightClicked); // 连接右击信号
                connect(display, &VideoDisplayUnit::requestMaximizeOrRestore,
                        this, &VideoGridView::onVideoDisplayUnitRequestMaximizeOrRestore); // 连接最大化信号
                display->show();                                                           // 显示控件
                m_displayviews_.push_back(display);                                        // 存储控件
            }
        }
    }

    {
        // 更新网格布局
        std::lock_guard<std::mutex> lock(mtx_grid_);
        m_video_grid_ = grid;
    }

    update(); // 触发重绘
}

void VideoGridView::onVideoPlay(int index)
{
    if (index >= 0 && static_cast<size_t>(index) < m_displayviews_.size())
    {
        m_displayviews_[index]->onPlay();
    }
}

void VideoGridView::onVideoStop(int index)
{
    if (index >= 0 && static_cast<size_t>(index) < m_displayviews_.size())
    {
        m_displayviews_[index]->onStop();
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

void VideoGridView::onVideoDisplayUnitRightClicked(int id_videodisplayunit, const QPoint &pos)
{
    // 在父窗口接收到右键点击的信号后弹出菜单
    QMenu menu(this);

    // 根据 ID 创建菜单项
    QAction *action1 = menu.addAction(QString("添加视频源"));
    QAction *action2 = menu.addAction(QString("Action2"));
    
    // 连接菜单项的动作
    connect(action1, &QAction::triggered, [this, id_videodisplayunit]() {
        qDebug() << "Action1 triggered: " << id_videodisplayunit;
    });
    connect(action2, &QAction::triggered, [this, id_videodisplayunit]() {
        qDebug() << "Action2 triggered: " << id_videodisplayunit;
    });

    // 在对应位置弹出菜单
    menu.exec(pos);
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

void VideoGridView::paintEvent(QPaintEvent *event)
{
    // static int count = 0;
    // qDebug() << "VideoGridView::paintEvent: " << count++;

    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor("darkgray"));

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
    }
    else
    {
        displayVideoGrid(painter);
    }
}

void VideoGridView::displayVideoGrid(QPainter& painter)
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
    }

    int spacing = 2; // 设置控件间隔为2像素
    int displayWidth = (width() - (cols + 1) * spacing) / cols;
    int displayHeight = (height() - (rows + 1) * spacing) / rows;

    for (int i = 0; i < m_video_grid_; ++i)
    {
        int row = i / cols;
        int col = i % cols;
        VideoDisplayUnit* display = m_displayviews_[i];

        int x = col * (displayWidth + spacing) + spacing;
        int y = row * (displayHeight + spacing) + spacing;

        display->setGeometry(x, y, displayWidth, displayHeight);
        display->show();
    }

    if (m_selected_index_ >= 0 && static_cast<size_t>(m_selected_index_) < m_displayviews_.size())
    {
        int index = m_selected_index_;
        int row = index / cols;
        int col = index % cols;
        int x = col * (displayWidth + spacing);
        int y = row * (displayHeight + spacing);
        int borderWidth = 3;
        painter.setPen(QPen(QColor("red"), borderWidth));
        painter.drawRect(x, y, displayWidth + spacing, displayHeight + spacing);
    }
}

