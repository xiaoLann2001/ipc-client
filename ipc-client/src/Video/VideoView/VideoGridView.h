#ifndef VIDEOGRIDVIEW_H
#define VIDEOGRIDVIEW_H

#include "VideoView/VideoDisplayUnitPool.h"

#include <QWidget>
#include <QStackedWidget>
#include <mutex>

enum VideoGrid
{
    VideoGridOne = 1,
    VideoGridFour = 4,
    VideoGridNine = 9,
    VideoGridSixteen = 16
};

class VideoGridView : public QWidget
{
    Q_OBJECT
public:
    explicit VideoGridView(QWidget *parent = nullptr);
    int getGrid();  // 获取网格数
    void setGrid(VideoGrid grid);   // 设置网格布局

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    int m_video_grid_ = 0;          // 视频网格布局
    int m_selected_index_ = -1;     // 选中的视频控件索引
    int m_maximized_index_ = -1;    // 最大化的视频控件索引

    VideoDisplayUnitPool *m_pool_;  // 控件池
    std::vector<VideoDisplayUnit*> m_displayviews_;  // 存储视频显示控件指针
    std::vector<QRect> m_display_geometries_;  // 存储视频显示控件的几何信息

    std::mutex mtx_grid_;  // 网格互斥锁, 保护网格变量
    std::mutex mtx_selected_index_;  // 选中索引互斥锁, 保护选中索引变量
    std::mutex mtx_maximized_index_;  // 最大化索引互斥锁, 保护最大化索引变量

    void calculateGridLayout();  // 计算网格布局
    void adjustDisplayUnits();  // 调整控件数量
    void displayVideoGrid(QPainter& painter);  // 显示视频网格

signals:

    // 传递子窗口的信号到上层
    void videoDisplayUnitClicked(int id_videodisplayunit);  // 传递点击信号
    void videoDisplayUnitRightClicked(int id_videodisplayunit, const QPoint &pos);  // 传递右击信号

public slots:
    void onVideoDisplayUnitClicked(int id_videodisplayunit);  // 视频控件点击槽函数
    void onVideoDisplayUnitRequestMaximizeOrRestore(int id_videodisplayunit);  // 视频控件最大化或还原槽函数
    void onVideoPlay(int index);  // 开启播放指定位置的视频
    void onVideoStop(int index);  // 停止播放指定位置的视频
    void onVideoSetImage(int index, const QImage &image);  // 设置指定位置的视频图像
};

#endif // VIDEOGRIDVIEW_H
