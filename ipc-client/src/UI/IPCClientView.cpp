#include "IPCClientView.h"

#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

IPCClientView::IPCClientView(QWidget *parent) : QWidget(parent)
{
    uiInit();
    controlInit();
}

IPCClientView::~IPCClientView()
{
}

void IPCClientView::uiInit() 
{
    // 设置无边框窗口
    // setWindowFlags(Qt::FramelessWindowHint);

    // 创建页面布局，分为从上至下分别为顶部布局、页面切换控件、其他信息布局
    layout = new QVBoxLayout();

    // 创建顶部布局
    layout_top = new QHBoxLayout;
    layout_top->setContentsMargins(0, 0, 0, 0); // 设置布局边距

    // 应用 LOGO 和名称
    label_applogo = new QLabel("LOGO");
    label_appname = new QLabel("IPC-CLIENT 智能监控系统");
    // 设置 LOGO 和名称样式
    QPixmap pixmap_applogo(ICON_APPLOGO);
    label_applogo->setPixmap(pixmap_applogo);
    label_applogo->setScaledContents(true);
    label_applogo->setFixedSize(40, 40);
    label_appname->setStyleSheet("color: white; font-size: 20px;");

    // 创建菜单按钮
    layout_menu = new QHBoxLayout;
    pushButton_page_videoview = new QPushButton("view");
    pushButton_page_videosource = new QPushButton("source");
    pushButton_page_videoreply = new QPushButton("reply");
    pushButton_page_videoevent = new QPushButton("event");
    pushButton_page_videosetting = new QPushButton("setting");

    // 设置菜单按钮样式
    QString button_page_style = "                               \
        QPushButton {                                           \
            min-height: 40px;                                   \
            font-size: 16px;                                    \
        }";
    pushButton_page_videoview->setStyleSheet(button_page_style);
    pushButton_page_videosource->setStyleSheet(button_page_style);
    pushButton_page_videoreply->setStyleSheet(button_page_style);
    pushButton_page_videoevent->setStyleSheet(button_page_style);
    pushButton_page_videosetting->setStyleSheet(button_page_style);

    // 添加菜单按钮到布局
    layout_menu->addWidget(pushButton_page_videoview);
    layout_menu->addWidget(pushButton_page_videosource);
    layout_menu->addWidget(pushButton_page_videoreply);
    layout_menu->addWidget(pushButton_page_videoevent);
    layout_menu->addWidget(pushButton_page_videosetting);

    // 创建其他信息布局
    layout_otherinfo = new QVBoxLayout();
    layout_otherinfo->setContentsMargins(0, 0, 0, 0); // 设置布局边距
    layout_windowcontrol = new QHBoxLayout();
    layout_windowcontrol->setContentsMargins(0, 0, 0, 0); // 设置布局边距
    pushButton_minimize = new QPushButton(this);    // 最小化按钮
    pushButton_maximize = new QPushButton(this);    // 最大化按钮
    pushButton_close = new QPushButton(this);       // 关闭按钮
    
    // 设置窗口控制按钮样式
    pushButton_minimize->setIcon(QIcon(ICON_APPMINIMIZE));
    pushButton_maximize->setIcon(QIcon(ICON_APPMAXIMIZE));
    pushButton_close->setIcon(QIcon(ICON_APPCLOSE));
    pushButton_minimize->setIconSize(QSize(16, 16));
    pushButton_maximize->setIconSize(QSize(16, 16));
    pushButton_close->setIconSize(QSize(16, 16));
    // pushButton_maximize->setIconSize(pushButton_maximize->size());
    QString button_window_control_style = "                 \
        QPushButton {                                       \
            min-width: 12px;                                \
            min-height: 12px;                               \
            background-color: rgba(0, 0, 0, 0);             \
            color: white;                                   \
            border-radius: 3px;                             \
        }                                                   \
        QPushButton:hover {                                 \
            background-color: rgba(255, 255, 255, 64);      \
        }                                                   \
        QPushButton:!hover:!pressed {                       \
            background-color: rgba(0, 0, 0, 0);             \
        }";
    pushButton_minimize->setStyleSheet(button_window_control_style);
    pushButton_maximize->setStyleSheet(button_window_control_style);
    pushButton_close->setStyleSheet(button_window_control_style);

    // 添加窗口控制按钮到布局
    layout_windowcontrol->addWidget(pushButton_minimize);
    layout_windowcontrol->addWidget(pushButton_maximize);
    layout_windowcontrol->addWidget(pushButton_close);
    label_currenttime = new QLabel("1970-01-01-00:00:00");
    label_currenttime->setStyleSheet("color: white; font-size: 12px;");
    layout_otherinfo->addLayout(layout_windowcontrol);
    layout_otherinfo->addWidget(label_currenttime);

    layout_top->addWidget(label_applogo);
    layout_top->addWidget(label_appname);
    layout_top->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout_top->addLayout(layout_menu);
    layout_top->addLayout(layout_otherinfo);

    // 创建页面切换控件，显示主要内容
    stackedwidget = new QStackedWidget();
    widget_videoview = new VideoView();   // 视频监控视图
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

    // 添加顶部布局和页面切换控件到主布局
    layout->addLayout(layout_top);
    layout->addWidget(stackedwidget);

    // 设置布局
    setLayout(layout);

    // 设置窗口大小
    resize(1024, 600);
}

void IPCClientView::controlInit()
{
    // 为窗口控制按钮添加点击事件
    connect(pushButton_minimize, &QPushButton::clicked, [this]() { 
        qDebug() << "用户点击最小化按钮";
        this->showMinimized(); 
    });
    connect(pushButton_maximize, &QPushButton::clicked, [this]() {
        qDebug() << "用户点击最大化按钮";
        this->isMaximized() ? this->showNormal() : this->showMaximized();
        // setWindowState(windowState() | Qt::WindowMaximized); 
    });
    connect(pushButton_close, &QPushButton::clicked, this, &IPCClientView::pushButton_close_clicked);

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
}

void IPCClientView::paintEvent(QPaintEvent *event)
{
    // static int count = 0;
    // qDebug() << "IPCClientView::paintEvent" << count++;

    // 创建 QPainter 对象
    QPainter painter(this);

    // 设置背景填充颜色
    // painter.fillRect(this->geometry(), QColor("#282828"));   // 使用 geometry() 只会正确重绘一次
    painter.fillRect(this->rect(), QColor("#282828"));

    QWidget::paintEvent(event);
}