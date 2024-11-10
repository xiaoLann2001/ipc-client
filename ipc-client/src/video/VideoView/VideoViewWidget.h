#ifndef VIDEOVIEWWIDGET_H
#define VIDEOVIEWWIDGET_H

#include <QWidget>
#include <QPaintEvent>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QSplitter>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QScrollArea>
#include <QSlider>
#include <QSpinBox>
#include <QPainter>
#include <QColor>

#include "VideoView/VideoGridView.h"

class VideoViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoViewWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    


public slots:
    void onVideoPlay(int idx);
    void onVideoStop(int idx);
    void onNewFrame(int idx, QImage image);
    // void onVideoGridChanged(VideoGrid grid);
    // void onVideoGridPageChanged(int page);
    // void onVideoControlPause();
    // void onVideoControlResolution();
    // void onVideoControlFullScreen();
    // void onVideoControlSnapshot();
    // void onVideoControlRecord();
    // void onVideoControlAlbum();
    // void onVideoControlOSD();
    // void onVideoControlAlarm();
    // void onVideoControlAI();
    // void onPTZPresetChanged(int preset);
    // void onPTZUp();
    // void onPTZDown();
    // void onPTZLeft();
    // void onPTZRight();
    // void onPTZReset();
    // void onPTZStepChanged(int step);

private:
    QHBoxLayout *layout;                            // 视频监控视图页全局布局
    QSplitter *splitter;                            // 窗口分隔器

    // 页面左侧：创建事件略缩图子窗口
    QWidget *widget_eventthumbnail;                 // -- 事件略缩图子窗口
    QVBoxLayout *layout_eventthumbnail;             // -- 事件略缩图布局
    QLabel *label_eventthumbnail;                   // ---- 事件略缩图标签
    QScrollArea *scrollarea_eventthumbnail;         // ---- 事件略缩图滚动区域

    // 页面中间：创建视频监控窗口
    QWidget *widget_video;                          // -- 视频监控子窗口
    QVBoxLayout *layout_video;                      // -- 视频监控布局
    VideoGridView *videogridview;                   // ---- 视频监控网格视图
    QHBoxLayout *layout_videogridandstatus;         // ---- 视频监控网格切换及状态信息布局
    QLabel *label_videogrid;                        // ------ 视频监控网格切换标签
    QRadioButton *radiobutton_videogrid1;           // ------ 视频监控网格切换按钮 1x1
    QRadioButton *radiobutton_videogrid4;           // ------ 视频监控网格切换按钮 2x2
    QRadioButton *radiobutton_videogrid9;           // ------ 视频监控网格切换按钮 3x3
    QRadioButton *radiobutton_videogrid16;          // ------ 视频监控网格切换按钮 4x4
    QHBoxLayout *layout_videopage;                  // ------ 翻页按钮布局
    QPushButton *pushbutton_prevpage;               // -------- 上一页按钮
    QSpinBox *spinbox_page;                         // -------- 页码输入框
    QPushButton *pushbutton_nextpage;               // -------- 下一页按钮
    QLabel *label_videostatus;                      // ------ 视频状态标签

    // 页面右侧：创建视频监控控制子窗口
    QWidget *widget_control;                        // -- 视频监控控制子窗口
    QVBoxLayout *layout_control;                    // -- 视频监控控制布局
    QPushButton *pushbutton_addipc;                 // ---- 添加网络摄像头按钮
    QPushButton *pushbutton_ipclist;                // ---- 网络摄像头列表按钮
    QGridLayout *gridlayout_videocontrol;           // ---- 视频控制按钮3*3布局
    QPushButton *pushbutton_videocontrol_pause;     // ------ 暂停按钮
    QPushButton *pushbutton_videocontrol_resolution;// ------ 分辨率按钮
    QPushButton *pushbutton_videocontrol_fullscreen;// ------ 全屏按钮
    QPushButton *pushbutton_videocontrol_snapshot;  // ------ 截图按钮
    QPushButton *pushbutton_videocontrol_record;    // ------ 录像按钮
    QPushButton *pushbutton_videocontrol_album;     // ------ 相册按钮
    QPushButton *pushbutton_videocontrol_osd;       // ------ osd按钮
    QPushButton *pushbutton_videocontrol_alarm;     // ------ 报警按钮
    QPushButton *pushbutton_videocontrol_ai;        // ------ AI按钮
    QLabel *label_ptz_angle;                        // ---- 云台角度信息标签
    QHBoxLayout *layout_ptz_preset;                 // ---- 云台预置点布局
    QLabel *label_ptz_preset;                       // ------ 云台预置点标签
    QComboBox *combobox_ptz_preset;                 // ------ 云台预置点下拉框
    QGridLayout *gridlayout_ptz;                    // ---- 云台控制按钮3*3布局
    QPushButton *pushbutton_ptz_up;                 // ------ 云台控制按钮 上
    QPushButton *pushbutton_ptz_down;               // ------ 云台控制按钮 下
    QPushButton *pushbutton_ptz_left;               // ------ 云台控制按钮 左
    QPushButton *pushbutton_ptz_right;              // ------ 云台控制按钮 右
    QPushButton *pushbutton_ptz_reset;              // ------ 云台控制按钮 复位
    QHBoxLayout *layout_ptz_step;                   // ---- 云台步长控制布局
    QLabel *label_ptz_step;                         // ------ 步长标签
    QSlider *slider_ptz_step;                       // ------ 步长滑块
    QSpinBox *spinbox_ptz_step;                     // ------ 步长输入框
};

#endif // VIDEOVIEWWIDGET_H
