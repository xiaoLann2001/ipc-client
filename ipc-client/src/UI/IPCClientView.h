#ifndef IPCCLIENTVIEW_H
#define IPCCLIENTVIEW_H

#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QLabel>

#include "VideoView/VideoViewWidget.h"

class IPCClientView : public QWidget
{
    Q_OBJECT

public:
    explicit IPCClientView(QWidget *parent = nullptr);
    ~IPCClientView();

    VideoViewWidget *getVideoViewWidget() { return widget_videoview; }

signals:
    void pushButton_close_clicked();

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void uiInit();
    void controlInit();

    // 创建页面布局，分为从上至下分别为顶部布局、页面切换控件、其他信息布局
    QVBoxLayout *layout;

    // 创建顶部布局
    QHBoxLayout *layout_top;

    // 应用 LOGO 和名称
    QLabel *label_applogo;
    QLabel *label_appname;

    // 创建菜单按钮
    QHBoxLayout *layout_menu;
    QPushButton *pushButton_page_videoview;
    QPushButton *pushButton_page_videosource;
    QPushButton *pushButton_page_videoreply;
    QPushButton *pushButton_page_videoevent;
    QPushButton *pushButton_page_videosetting;

    // 创建其他信息布局
    QVBoxLayout *layout_otherinfo;
    QHBoxLayout *layout_windowcontrol;

    // 创建窗口控制按钮
    QPushButton *pushButton_minimize;
    QPushButton *pushButton_maximize;
    QPushButton *pushButton_close;

    // 创建当前时间标签
    QLabel *label_currenttime;

    // 创建页面切换控件，显示主要内容
    QStackedWidget *stackedwidget;
    VideoViewWidget *widget_videoview;          // 视频监控窗口，由控制器传入
    // VideoSourceWidget *widget_videosource;
    // VideoReplyWidget *widget_videoreply;
    // EventWidget *widget_event;
    // SettingWidget *widget_setting;

};

#endif // WIDGET_H
