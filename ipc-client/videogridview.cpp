#include "videogridview.h"

#include <QPainter>

VideoGridView::VideoGridView(QWidget *parent) : QWidget(parent)
{
    m_pool = new VideoDisplayViewPool(16);  // 创建控件池
}

void VideoGridView::setGrid(VideoGrid grid)
{
    // 首先将当前显示控件释放回控件池
    for (auto display : m_displayViews) {
        display->hide();  // 隐藏控件，以免影响布局
        m_pool->release(display);  // 将控件释放回控件池
    }
    m_displayViews.clear();  // 清空当前显示控件的指针列表

    // 更新网格布局
    m_video_grid_ = grid;

    // 根据新的网格大小，从控件池获取所需数量的 VideoDisplayView 控件
    int numDisplays = static_cast<int>(grid);
    for (int i = 0; i < numDisplays; ++i) {
        VideoDisplayUnit* display = m_pool->acquire();
        if (display) {
            display->setParent(this);  // 设置父控件
            connect(display, &VideoDisplayUnit::clicked, this, &VideoGridView::onVideoDisplayUnitClicked);  // 连接点击信号
            display->show();  // 显示控件
            m_displayViews.push_back(display);  // 存储控件
        }
    }

    update();  // 触发重绘
}

void VideoGridView::setVideo(int index, const QImage &image)
{
    if (index >= 0 && static_cast<size_t>(index) < m_displayViews.size()) {
        m_displayViews[index]->setImage(image);
    }
}

void VideoGridView::onVideoDisplayUnitClicked(int id_videodisplayunit)
{
    // 查找点击的控件索引
    for (size_t i = 0; i < m_displayViews.size(); ++i) {
        if (m_displayViews[i]->getId() == id_videodisplayunit) {
            selected_index_ = i;
            update();   // 触发重绘
            break;
        }
    }
}

void VideoGridView::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(this->rect(), QColor("darkgray"));
    // painter.setPen(QColor("lightgray"));

    int rows = 0, cols = 0;
    switch (m_video_grid_) {
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

    int spacing = 2;  // 设置控件间隔为1像素

    // // 绘制网格线
    // for (int i = 1; i < cols; ++i) {
    //     painter.drawLine(i * width() / cols, 0, i * width() / cols, height());
    // }
    // for (int i = 1; i < rows; ++i) {
    //     painter.drawLine(0, i * height() / rows, width(), i * height() / rows);
    // }

    // 计算每个视频显示控件的大小，包含间隔
    int displayWidth = (width() - (cols + 1) * spacing) / cols;
    int displayHeight = (height() - (rows + 1) * spacing) / rows;

    // 设置每个视频显示控件的位置和大小
    for (int i = 0; static_cast<size_t>(i) < m_displayViews.size(); ++i) {
        int row = i / cols;
        int col = i % cols;
        VideoDisplayUnit* display = m_displayViews[i];

        // 根据行列设置控件的位置，间隔增加
        int x = col * (displayWidth + spacing) + spacing;
        int y = row * (displayHeight + spacing) + spacing;

        display->setGeometry(x, y, displayWidth, displayHeight);
        display->show();
    }

    // 绘制选框
    if (selected_index_ >= 0 && static_cast<size_t>(selected_index_) < m_displayViews.size()) {
        int row = selected_index_ / cols;
        int col = selected_index_ % cols;
        int x = col * (displayWidth + spacing);
        int y = row * (displayHeight + spacing);
        int borderWidth = 3;
        painter.setPen(QPen(QColor("red"), borderWidth));
        painter.drawRect(x, y, displayWidth + spacing, displayHeight + spacing);
    }
}
