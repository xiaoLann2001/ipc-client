#ifndef VIDEOGRIDVIEW_H
#define VIDEOGRIDVIEW_H

// #include "VideoView/VideoDisplayUnitPool.h"
#include "VideoDisplayUnit.h"
#include "VideoDisplayTooltip.h"
#include "CustomQWidgetPool.h"

#include "VideoSignalBus.h"

#include <QWidget>
#include <QStackedWidget>
#include <QMutex>
#include <QMutexLocker>
#include <QPainter>
#include <QVector>

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
    ~VideoGridView();

    int getGrid();                  // 获取网格数
    void setGrid(VideoGrid grid);   // 设置网格布局

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    void videoDisplayUnitClicked(int id_videodisplayunit);  // 传递点击信号
    void videoDisplayUnitRightClicked(int id_videodisplayunit, const QPoint &pos);  // 传递右击信号

public slots:
    void onVideoDisplayUnitClicked(int id_videodisplayunit);// 视频控件点击槽函数
    void onVideoDisplayUnitRequestMaximizeOrRestore(int id_videodisplayunit);  // 视频控件最大化或还原槽函数
    void onVideoPlay(int index);                            // 开启播放指定位置的视频
    void onVideoClose(int index);                           // 停止播放指定位置的视频
    void onVideoSetImage(int index, const QImage &image);   // 设置指定位置的视频图像

private:
    void UIInit();                              // 初始化视频显示控件和悬浮窗控件

    void calculateGridLayout();                 // 计算网格布局
    void adjustDisplayUnits();                  // 调整控件数量
    void addDisplayUnit();                      // 添加视频显示控件
    void removeDisplayUnit();                   // 移除视频显示控件

    int m_video_grid_ = 0;                      // 视频网格布局
    int m_selected_index_ = -1;                 // 选中的视频控件索引
    int m_maximized_index_ = -1;                // 最大化的视频控件索引

    // VideoDisplayUnitPool *m_pool_;                               // 控件池
    CustomQWidgetPool<VideoDisplayUnit> *m_displayUnitPool_;        // 显示控件池
    CustomQWidgetPool<VideoDisplayTooltip> *m_displayTooltipPool_;  // 悬浮窗控件池
    QVector<VideoDisplayUnit*> m_displayviews_;                     // 存储视频显示控件指针
    QVector<QRect> m_display_geometries_;                           // 存储视频显示控件的几何信息

    QMutex mtx_grid_;                           // 网格互斥锁, 保护网格变量
    QMutex mtx_selected_index_;                 // 选中索引互斥锁, 保护选中索引变量
    QMutex mtx_maximized_index_;                // 最大化索引互斥锁, 保护最大化索引变量
};

#endif // VIDEOGRIDVIEW_H
