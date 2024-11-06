#ifndef VIDEOGRIDVIEW_H
#define VIDEOGRIDVIEW_H

#include "videodisplayviewpool.h"

#include <QWidget>

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

    void setGrid(VideoGrid grid);
    void setVideo(int index, const QImage &image);  // 设置指定位置的视频图像

protected:
    void paintEvent(QPaintEvent *event) override;

    VideoDisplayViewPool *m_pool;  // 控件池

    std::vector<VideoDisplayUnit*> m_displayViews;  // 存储视频显示控件指针

private:
    int m_video_grid_ = VideoGridFour;

    int selected_index_ = -1;   // 选中的视频控件索引

signals:

public slots:
    void onVideoDisplayUnitClicked(int id_videodisplayunit);  // 视频控件点击槽函数
};

#endif // VIDEOGRIDVIEW_H