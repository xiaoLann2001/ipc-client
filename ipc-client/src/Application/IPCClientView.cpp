#include "IPCClientView.h"

#include <QIcon>

IPCClientView::IPCClientView(QWidget *parent) : QWidget(parent)
{
    // 创建页面布局，分为从上至下分别为顶部布局、页面切换控件、其他信息布局
    layout = new QVBoxLayout();

    // 创建顶部布局
    layout_top = new QHBoxLayout;

    // 应用 LOGO 和名称
    label_applogo = new QLabel("LOGO");
    QIcon applogo(QString::fromStdString(":/icons/applogo.svg"));
    label_applogo->setPixmap(applogo.pixmap(40, 40));
    label_appname = new QLabel("IPC-CLIENT智能监控系统");

    // 创建菜单按钮
    layout_menu = new QHBoxLayout;
    pushButton_page_videoview = new QPushButton("view");
    pushButton_page_videosource = new QPushButton("source");
    pushButton_page_videoreply = new QPushButton("reply");
    pushButton_page_videoevent = new QPushButton("event");
    pushButton_page_videosetting = new QPushButton("setting");
    layout_menu->addWidget(pushButton_page_videoview);
    layout_menu->addWidget(pushButton_page_videosource);
    layout_menu->addWidget(pushButton_page_videoreply);
    layout_menu->addWidget(pushButton_page_videoevent);
    layout_menu->addWidget(pushButton_page_videosetting);

    // 创建其他信息布局
    layout_otherinfo = new QVBoxLayout();
    layout_windowcontrol = new QHBoxLayout();
    pushButton_minimize = new QPushButton("-");
    pushButton_maximize = new QPushButton("=");
    pushButton_close = new QPushButton("x");
    layout_windowcontrol->addWidget(pushButton_minimize);
    layout_windowcontrol->addWidget(pushButton_maximize);
    layout_windowcontrol->addWidget(pushButton_close);
    label_currenttime = new QLabel("1970-01-01-00:00:00");
    layout_otherinfo->addLayout(layout_windowcontrol);
    layout_otherinfo->addWidget(label_currenttime);

    layout_top->addWidget(label_applogo);
    layout_top->addWidget(label_appname);
    layout_top->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout_top->addLayout(layout_menu);
    layout_top->addLayout(layout_otherinfo);

    // 创建页面切换控件，显示主要内容
    stackedwidget = new QStackedWidget();
    widget_videoview = new VideoViewWidget();   // 视频监控视图
//     widget_videosource = new VideoSourceWidget(); // 视频源视图
//     widget_videoreply = new VideoReplyWidget(); // 视频回放视图
//     widget_event = new EventWidget();           // 事件视图
//     widget_setting = new SettingWidget();       // 设置视图
    stackedwidget->addWidget(widget_videoview);
//     stackedwidget->addWidget(widget_videosource);
//     stackedwidget->addWidget(widget_videoreply);
//     stackedwidget->addWidget(widget_event);
//     stackedwidget->addWidget(widget_setting);
    stackedwidget->setCurrentIndex(0);
    stackedwidget->setMinimumSize(1024, 540);

    // 为菜单按钮添加点击事件
    connect(pushButton_page_videoview, &QPushButton::clicked, [this]()
            { stackedwidget->setCurrentIndex(0); });
//     connect(pushButton_page_videosource, &QPushButton::clicked, [this]()
//             { stackedwidget->setCurrentIndex(1); });
//     connect(pushButton_page_videoreply, &QPushButton::clicked, [this]()
//             { stackedwidget->setCurrentIndex(2); });
//     connect(pushButton_page_videoevent, &QPushButton::clicked, [this]()
//             { stackedwidget->setCurrentIndex(3); });
//     connect(pushButton_page_videosetting, &QPushButton::clicked, [this]()
//             { stackedwidget->setCurrentIndex(4); });

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