#include "ipcclientview.h"

#include "videoviewwidget.h"
#include "videoreplywidget.h"
#include "eventwidget.h"
#include "settingwidget.h"

#include <QApplication>
#include <QWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QPushButton>
#include <QLabel>

IPCClientView::IPCClientView(QWidget *parent) :
    QWidget(parent)
{
    // 创建页面布局，分为从上至下分别为顶部布局、页面切换控件、其他信息布局
    QVBoxLayout *layout = new QVBoxLayout();

    // 创建顶部布局
    QHBoxLayout *layout_top = new QHBoxLayout;

    // 应用 LOGO 和名称
    QLabel *label_applogo = new QLabel("LOGO");
    QLabel *label_appname = new QLabel("IPC-CLIENT");

    // 创建菜单按钮
    QHBoxLayout *layout_menu = new QHBoxLayout;
    QPushButton *pushButton_page_videoview = new QPushButton("view");
    QPushButton *pushButton_page_videoreply = new QPushButton("reply");
    QPushButton *pushButton_page_videoevent = new QPushButton("event");
    QPushButton *pushButton_page_videosetting = new QPushButton("setting");
    layout_menu->addWidget(pushButton_page_videoview);
    layout_menu->addWidget(pushButton_page_videoreply);
    layout_menu->addWidget(pushButton_page_videoevent);
    layout_menu->addWidget(pushButton_page_videosetting);

    // 创建其他信息布局
    QVBoxLayout *layout_otherinfo = new QVBoxLayout();
    QHBoxLayout *layout_windowcontrol = new QHBoxLayout();
    QPushButton *pushButton_minimize = new QPushButton("-");
    QPushButton *pushButton_maximize = new QPushButton("=");
    QPushButton *pushButton_close = new QPushButton("x");
    layout_windowcontrol->addWidget(pushButton_minimize);
    layout_windowcontrol->addWidget(pushButton_maximize);
    layout_windowcontrol->addWidget(pushButton_close);
    QLabel *label_currenttime = new QLabel("1970-01-01-00:00:00");
    layout_otherinfo->addLayout(layout_windowcontrol);
    layout_otherinfo->addWidget(label_currenttime);

    layout_top->addWidget(label_applogo);
    layout_top->addWidget(label_appname);
    layout_top->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout_top->addLayout(layout_menu);
    layout_top->addLayout(layout_otherinfo);

    // 创建页面切换控件，显示主要内容
    QStackedWidget *stackedwidget = new QStackedWidget();
    VideoViewWidget *widget_videoview = new VideoViewWidget();
    VideoReplyWidget *widget_videoreply = new VideoReplyWidget();
    EventWidget *widget_event = new EventWidget();
    SettingWidget *widget_setting = new SettingWidget();
    stackedwidget->addWidget(widget_videoview);
    stackedwidget->addWidget(widget_videoreply);
    stackedwidget->addWidget(widget_event);
    stackedwidget->addWidget(widget_setting);
    stackedwidget->setCurrentIndex(0);
    stackedwidget->setMinimumSize(1024, 520);

    // 为菜单按钮添加点击事件
    connect(pushButton_page_videoview, &QPushButton::clicked, [stackedwidget]() { stackedwidget->setCurrentIndex(0); });
    connect(pushButton_page_videoreply, &QPushButton::clicked, [stackedwidget]() { stackedwidget->setCurrentIndex(1); });
    connect(pushButton_page_videoevent, &QPushButton::clicked, [stackedwidget]() { stackedwidget->setCurrentIndex(2); });
    connect(pushButton_page_videosetting, &QPushButton::clicked, [stackedwidget]() { stackedwidget->setCurrentIndex(3); });

    // 添加顶部布局和页面切换控件到主布局
    layout->addLayout(layout_top);
    layout->addWidget(stackedwidget);

    // 设置布局
    setLayout(layout);

    // 设置窗口大小
    resize(1024, 600);
}

IPCClientView::~IPCClientView()
{

}
