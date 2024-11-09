#include "videoviewwidget.h"

#include "videogridview.h"

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

VideoViewWidget::VideoViewWidget(QWidget *parent) : QWidget(parent)
{
    // 创建页面布局，分为从左至右分别为事件略缩图、视频监控视图、控制区域
    QHBoxLayout *layout = new QHBoxLayout();

    // 创建水平分隔器
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this); // 创建水平分隔器
    splitter->setHandleWidth(3); // 设置分隔条的宽度

    // 创建事件略缩图子窗口
    int eventthumbnail_minwidth = 160;
    int eventthumbnail_maxwidth = 320;
    QWidget *widget_eventthumbnail = new QWidget();
    // 创建左侧事件略缩图布局
    QVBoxLayout *layout_eventthumbnail = new QVBoxLayout();
    QLabel *label_eventthumbnail = new QLabel("事件略缩图");
    QScrollArea *scrollarea_eventthumbnail = new QScrollArea();
    scrollarea_eventthumbnail->setMinimumWidth(eventthumbnail_minwidth);
    scrollarea_eventthumbnail->setMaximumWidth(eventthumbnail_maxwidth);
    layout_eventthumbnail->addWidget(label_eventthumbnail);
    layout_eventthumbnail->addWidget(scrollarea_eventthumbnail);
    // 将子窗口布局加入到子窗口
    widget_eventthumbnail->setLayout(layout_eventthumbnail);
    // 子窗口尺寸限制
    widget_eventthumbnail->setMinimumWidth(eventthumbnail_minwidth);
    widget_eventthumbnail->setMaximumWidth(eventthumbnail_maxwidth);


    // 创建视频监控窗口
    int video_minwidth = 640;
    int video_minheight = 480;
    QWidget *widget_video = new QWidget();
    widget_video->setMinimumWidth(video_minwidth);
    widget_video->setMinimumHeight(video_minheight);
    // 创建页面布局，包括视频监控窗口、视频监控网格切换及状态信息布局
    QVBoxLayout *layout_video = new QVBoxLayout();
    // 创建视频监控视图
    VideoGridView *videogridview = new VideoGridView();
    // 创建视频监控网格切换及状态信息布局
    int status_maxheight = 20;  // 设置状态信息高度
    QHBoxLayout *layout_videogridandstatus = new QHBoxLayout();
    QLabel *label_videogrid = new QLabel("网格数");
    // 设置视频监控网格切换按钮高度
    label_videogrid->setMaximumHeight(status_maxheight);
    // 创建视频监控网格切换按钮
    QRadioButton *radiobutton_videogrid1 = new QRadioButton("1");
    QRadioButton *radiobutton_videogrid4 = new QRadioButton("4");
    QRadioButton *radiobutton_videogrid9 = new QRadioButton("9");
    QRadioButton *radiobutton_videogrid16 = new QRadioButton("16");
    // 设置视频监控网格切换按钮高度
    radiobutton_videogrid1->setMaximumHeight(status_maxheight);
    radiobutton_videogrid4->setMaximumHeight(status_maxheight);
    radiobutton_videogrid9->setMaximumHeight(status_maxheight);
    radiobutton_videogrid16->setMaximumHeight(status_maxheight);
    // 默认网格数为4
    radiobutton_videogrid4->setChecked(true);
    // 连接视频监控网格切换按钮信号与槽
    connect(radiobutton_videogrid1, &QRadioButton::clicked, [videogridview](){videogridview->setGrid(VideoGridOne);});
    connect(radiobutton_videogrid4, &QRadioButton::clicked, [videogridview](){videogridview->setGrid(VideoGridFour);});
    connect(radiobutton_videogrid9, &QRadioButton::clicked, [videogridview](){videogridview->setGrid(VideoGridNine);});
    connect(radiobutton_videogrid16, &QRadioButton::clicked, [videogridview](){videogridview->setGrid(VideoGridSixteen);});
    // 默认触发4网格显示
    radiobutton_videogrid4->clicked();
    // 翻页按钮布局
    QHBoxLayout *layout_videopage = new QHBoxLayout();
    QPushButton *pushbutton_prevpage = new QPushButton("上一页");
    QPushButton *pushbutton_nextpage = new QPushButton("下一页");
    QSpinBox *spinbox_page = new QSpinBox();
    // 将翻页按钮加入到布局
    layout_videopage->addWidget(pushbutton_prevpage);
    layout_videopage->addWidget(spinbox_page);
    layout_videopage->addWidget(pushbutton_nextpage);
    // 设置视频状态信息
    QLabel *label_videostatus = new QLabel("视频状态");
    label_videostatus->setMaximumHeight(status_maxheight);
    // 将视频监控网格切换按钮和视频状态信息加入到布局
    layout_videogridandstatus->addWidget(label_videogrid);
    layout_videogridandstatus->addWidget(radiobutton_videogrid1);
    layout_videogridandstatus->addWidget(radiobutton_videogrid4);
    layout_videogridandstatus->addWidget(radiobutton_videogrid9);
    layout_videogridandstatus->addWidget(radiobutton_videogrid16);
    layout_videogridandstatus->addLayout(layout_videopage);
    layout_videogridandstatus->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout_videogridandstatus->addWidget(label_videostatus);
    // 将视频监控视图和视频监控网格切换及状态信息布局加入到视频监控布局
    layout_video->addWidget(videogridview);
    layout_video->addLayout(layout_videogridandstatus);
    // 将视频监控布局加入到视频监控窗口
    widget_video->setLayout(layout_video);


    // 创建视频监控控制子窗口
    int control_minwidth = 160;
    int control_maxwidth = 240;
    QWidget *widget_control = new QWidget();
    // 子窗口尺寸限制
    widget_control->setMinimumWidth(control_minwidth + 10);
    widget_control->setMaximumWidth(control_maxwidth);
    // 创建控制区域布局
    QVBoxLayout *layout_control = new QVBoxLayout();
    QPushButton *pushbutton_addipc = new QPushButton("添加网络摄像头");
    QPushButton *pushbutton_ipclist = new QPushButton("网络摄像头列表");
    // 设置控制按钮宽度
    pushbutton_addipc->setMinimumWidth(control_minwidth);
    pushbutton_addipc->setMaximumWidth(control_maxwidth);
    pushbutton_ipclist->setMinimumWidth(control_minwidth);
    pushbutton_ipclist->setMaximumWidth(control_maxwidth);
    // 创建视频控制按钮3*3布局
    QGridLayout *gridlayout_videocontrol = new QGridLayout();
    QPushButton *pushbutton_videocontrol_pause = new QPushButton("暂停");
    QPushButton *pushbutton_videocontrol_resolution = new QPushButton("分辨率");
    QPushButton *pushbutton_videocontrol_fullscreen = new QPushButton("全屏");
    QPushButton *pushbutton_videocontrol_snapshot = new QPushButton("截图");
    QPushButton *pushbutton_videocontrol_record = new QPushButton("录像");
    QPushButton *pushbutton_videocontrol_album = new QPushButton("相册");
    QPushButton *pushbutton_videocontrol_osd = new QPushButton("osd");
    QPushButton *pushbutton_videocontrol_alarm = new QPushButton("报警");
    QPushButton *pushbutton_videocontrol_ai = new QPushButton("AI");
    // 设置视频控制按钮宽度
    pushbutton_videocontrol_pause->setMinimumWidth(control_minwidth / 3);
    pushbutton_videocontrol_pause->setMaximumWidth(control_maxwidth / 3);
    pushbutton_videocontrol_resolution->setMinimumWidth(control_minwidth / 3);
    pushbutton_videocontrol_resolution->setMaximumWidth(control_maxwidth / 3);
    pushbutton_videocontrol_fullscreen->setMinimumWidth(control_minwidth / 3);
    pushbutton_videocontrol_fullscreen->setMaximumWidth(control_maxwidth / 3);
    pushbutton_videocontrol_snapshot->setMinimumWidth(control_minwidth / 3);
    pushbutton_videocontrol_snapshot->setMaximumWidth(control_maxwidth / 3);
    pushbutton_videocontrol_record->setMinimumWidth(control_minwidth / 3);
    pushbutton_videocontrol_record->setMaximumWidth(control_maxwidth / 3);
    pushbutton_videocontrol_album->setMinimumWidth(control_minwidth / 3);
    pushbutton_videocontrol_album->setMaximumWidth(control_maxwidth / 3);
    pushbutton_videocontrol_osd->setMinimumWidth(control_minwidth / 3);
    pushbutton_videocontrol_osd->setMaximumWidth(control_maxwidth / 3);
    pushbutton_videocontrol_alarm->setMinimumWidth(control_minwidth / 3);
    pushbutton_videocontrol_alarm->setMaximumWidth(control_maxwidth / 3);
    pushbutton_videocontrol_ai->setMinimumWidth(control_minwidth / 3);
    pushbutton_videocontrol_ai->setMaximumWidth(control_maxwidth / 3);
    // 设置暂停键与播放键的切换
    connect(pushbutton_videocontrol_pause, &QPushButton::clicked, [pushbutton_videocontrol_pause](){
        if (pushbutton_videocontrol_pause->text() == "暂停")
            pushbutton_videocontrol_pause->setText("播放");
        else
            pushbutton_videocontrol_pause->setText("暂停");
    });
    // 加入到视频控制按钮布局
    gridlayout_videocontrol->addWidget(pushbutton_videocontrol_pause, 0, 0);
    gridlayout_videocontrol->addWidget(pushbutton_videocontrol_resolution, 0, 1);
    gridlayout_videocontrol->addWidget(pushbutton_videocontrol_fullscreen, 0, 2);
    gridlayout_videocontrol->addWidget(pushbutton_videocontrol_snapshot, 1, 0);
    gridlayout_videocontrol->addWidget(pushbutton_videocontrol_record, 1, 1);
    gridlayout_videocontrol->addWidget(pushbutton_videocontrol_album, 1, 2);
    gridlayout_videocontrol->addWidget(pushbutton_videocontrol_osd, 2, 0);
    gridlayout_videocontrol->addWidget(pushbutton_videocontrol_alarm, 2, 1);
    gridlayout_videocontrol->addWidget(pushbutton_videocontrol_ai, 2, 2);
    // 创建云台预置点布局
    QHBoxLayout *layout_ptz = new QHBoxLayout();
    QLabel *label_ptz_preset = new QLabel("云台预置点");
    QComboBox *combobox_ptz_preset = new QComboBox();
    // 设置云台预置点控件宽度
    label_ptz_preset->setMinimumWidth(control_minwidth / 3);
    label_ptz_preset->setMaximumWidth(control_maxwidth / 3);
    combobox_ptz_preset->setMinimumWidth(control_minwidth * 2 / 3);
    combobox_ptz_preset->setMaximumWidth(control_maxwidth * 2 / 3);
    layout_ptz->addWidget(label_ptz_preset);
    layout_ptz->addWidget(combobox_ptz_preset);
    // 云台角度信息
    QLabel *label_ptz_angle = new QLabel("云台角度");
    label_ptz_angle->setMinimumWidth(control_minwidth);
    label_ptz_angle->setMaximumWidth(control_maxwidth);
    // 创建云台预置点按钮布局3*3
    QGridLayout *gridlayout_ptz = new QGridLayout();
    QPushButton *pushbutton_ptz_up = new QPushButton("上");
    QPushButton *pushbutton_ptz_down = new QPushButton("下");
    QPushButton *pushbutton_ptz_left = new QPushButton("左");
    QPushButton *pushbutton_ptz_right = new QPushButton("右");
    QPushButton *pushbutton_ptz_reset = new QPushButton("复位");
    // 设置云台预置点按钮宽度
    pushbutton_ptz_up->setMinimumWidth(control_minwidth / 3);
    pushbutton_ptz_up->setMaximumWidth(control_maxwidth / 3);
    pushbutton_ptz_down->setMinimumWidth(control_minwidth / 3);
    pushbutton_ptz_down->setMaximumWidth(control_maxwidth / 3);
    pushbutton_ptz_left->setMinimumWidth(control_minwidth / 3);
    pushbutton_ptz_left->setMaximumWidth(control_maxwidth / 3);
    pushbutton_ptz_right->setMinimumWidth(control_minwidth / 3);
    pushbutton_ptz_right->setMaximumWidth(control_maxwidth / 3);
    pushbutton_ptz_reset->setMinimumWidth(control_minwidth / 3);
    pushbutton_ptz_reset->setMaximumWidth(control_maxwidth / 3);
    // 加入到云台预置点按钮布局
    gridlayout_ptz->addWidget(pushbutton_ptz_up, 0, 1);
    gridlayout_ptz->addWidget(pushbutton_ptz_down, 2, 1);
    gridlayout_ptz->addWidget(pushbutton_ptz_left, 1, 0);
    gridlayout_ptz->addWidget(pushbutton_ptz_right, 1, 2);
    gridlayout_ptz->addWidget(pushbutton_ptz_reset, 1, 1);
    // 云台步长控制布局
    QHBoxLayout *layout_ptz_step = new QHBoxLayout();
    QLabel *label_ptz_step = new QLabel("步长");
    QSlider *slider_ptz_step = new QSlider(Qt::Horizontal);
    QSpinBox *spinbox_ptz_step = new QSpinBox();
    // 设置步长控制控件宽度
    label_ptz_step->setMinimumWidth(control_minwidth / 6);
    label_ptz_step->setMaximumWidth(control_maxwidth / 6);
    slider_ptz_step->setMinimumWidth(control_minwidth * 3 / 6);
    slider_ptz_step->setMaximumWidth(control_maxwidth * 3 / 6);
    spinbox_ptz_step->setMinimumWidth(control_minwidth * 2 / 6);
    spinbox_ptz_step->setMaximumWidth(control_maxwidth * 2 / 6);
    // 设置步长控制范围
    slider_ptz_step->setRange(1, 10);
    spinbox_ptz_step->setRange(1, 10);
    // 默认步长为5
    slider_ptz_step->setValue(5);
    spinbox_ptz_step->setValue(5);
    // 步长控制联动
    connect(slider_ptz_step, &QSlider::valueChanged, spinbox_ptz_step, &QSpinBox::setValue);
    connect(spinbox_ptz_step, QOverload<int>::of(&QSpinBox::valueChanged), slider_ptz_step, &QSlider::setValue);
    layout_ptz_step->addWidget(label_ptz_step);
    layout_ptz_step->addWidget(slider_ptz_step);
    layout_ptz_step->addWidget(spinbox_ptz_step);
    // 加入到控制区域布局
    layout_control->addWidget(pushbutton_addipc);
    layout_control->addWidget(pushbutton_ipclist);
    layout_control->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout_control->addLayout(gridlayout_videocontrol);
    layout_control->addWidget(label_ptz_angle);
    layout_control->addLayout(layout_ptz);
    layout_control->addLayout(gridlayout_ptz);
    layout_control->addLayout(layout_ptz_step);
    // 将子窗口布局加入到子窗口
    widget_control->setLayout(layout_control);

    // 将子窗口加入到窗口布局
    splitter->addWidget(widget_eventthumbnail);
    splitter->addWidget(widget_video);
    splitter->addWidget(widget_control);

    // 将分隔器加入到窗口布局
    layout->addWidget(splitter);

    // 设置布局
    setLayout(layout);
}

// 重载绘图事件处理函数
void VideoViewWidget::paintEvent(QPaintEvent *event)
{
    // 调用基类的绘图事件
    QWidget::paintEvent(event);

    // 创建 QPainter 对象
    QPainter painter(this);

    // 设置背景填充颜色
    painter.fillRect(this->rect(), QColor("lightgray"));

    // // 其他绘制代码（如绘制形状或文本）可以在这里添加
    // painter.setPen(Qt::black);
    // painter.setBrush(Qt::yellow);
    // painter.drawRect(50, 50, 200, 100); // 在窗口的特定位置绘制一个矩形
}
