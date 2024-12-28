#include "VideoDisplayTooltip.h"

#include <QDebug>
#include <QToolTip>

VideoDisplayTooltip::VideoDisplayTooltip(QWidget *parent)
    : QWidget(parent)
{
    tooltipInit();
    controlInit();
}

void VideoDisplayTooltip::setTooltipText(const QString &text)
{
    label_videoInfo->setText(text);
}

void VideoDisplayTooltip::setTooltipMode(bool haveAddIPC)
{
    if (haveAddIPC) {   // 已添加视频流
        push_button_addIPC->hide();
        push_button_playOrPause->show();
        push_button_snapshot->show();
        push_button_record->show();
        push_button_fullscreen->show();
        push_button_ai->show();
        push_button_closeIPC->show();
    } else {            // 未添加视频流
        push_button_addIPC->show();
        push_button_playOrPause->hide();
        push_button_snapshot->hide();
        push_button_record->hide();
        push_button_fullscreen->hide();
        push_button_ai->hide();
        push_button_closeIPC->hide();
    }
}

void VideoDisplayTooltip::tooltipInit()
{
    // 初始化悬浮窗，默认不可见
    this->setFixedWidth(parentWidget()->width());
    this->setStyleSheet("background-color: rgba(0, 0, 0, 127); color: white; font-size: 12px; border-radius: 5px;");
    this->setFixedHeight(30);     // 悬浮窗高度固定为30

    // 添加水平布局
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 0, 5, 0);         // 设置布局边距
    label_videoInfo = new QLabel();        // 默认显示的文本
    // m_label->setStyleSheet("color: white; font-size: 12px;");

    // 未添加视频流时的控制按钮
    push_button_addIPC = new QPushButton("Add");

    // 已添加视频流时的控制按钮
    push_button_playOrPause = new QPushButton("Play");
    push_button_snapshot = new QPushButton("Snapshot");
    push_button_record = new QPushButton("Record");
    push_button_fullscreen = new QPushButton("Fullscreen");
    push_button_ai = new QPushButton("AI");
    push_button_closeIPC = new QPushButton("Close");

    // 标签样式: 透明背景，白色字体，左对齐
    label_videoInfo->setStyleSheet(
        "background-color: rgba(0, 0, 0, 0);"
        "color: white;"
        "text-align: left;");

    // 控制按钮样式: 圆角矩形，透明背景，白色字体，悬浮时背景变色
    QString button_style = "                                \
        QPushButton {                                       \
            min-width: 25px;                                \
            min-height: 25px;                               \
            background-color: rgba(0, 0, 0, 0);             \
            color: white;                                   \
            border-radius: 5px;                             \
        }                                                   \
        QPushButton:hover {                                 \
            background-color: rgba(255, 255, 255, 64);      \
        }";
    push_button_addIPC->setStyleSheet(button_style);
    push_button_playOrPause->setStyleSheet(button_style);
    push_button_snapshot->setStyleSheet(button_style);
    push_button_record->setStyleSheet(button_style);
    push_button_fullscreen->setStyleSheet(button_style);
    push_button_ai->setStyleSheet(button_style);
    push_button_closeIPC->setStyleSheet(button_style);

    // 添加提示信息
    label_videoInfo->setToolTip("视频信息");
    push_button_addIPC->setToolTip("添加视频源");
    push_button_playOrPause->setToolTip("播放/暂停");
    push_button_snapshot->setToolTip("截图");
    push_button_record->setToolTip("录像");
    push_button_fullscreen->setToolTip("全屏");
    push_button_ai->setToolTip("AI使能");
    push_button_closeIPC->setToolTip("关闭视频源");

    // 控制按钮初始化
    layout->addWidget(label_videoInfo);             // 将标签加入到布局
    layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Maximum));  // 添加弹簧
    layout->addWidget(push_button_addIPC);          // 将添加按钮加入到布局
    layout->addWidget(push_button_playOrPause);     // 将播放/暂停按钮加入到布局
    layout->addWidget(push_button_snapshot);        // 将截图按钮加入到布局
    layout->addWidget(push_button_record);          // 将录像按钮加入到布局
    layout->addWidget(push_button_fullscreen);      // 将全屏按钮加入到布局
    layout->addWidget(push_button_ai);              // 将AI按钮加入到布局
    layout->addWidget(push_button_closeIPC);        // 将关闭按钮加入到布局
}

void VideoDisplayTooltip::controlInit()
{
    // 控制按钮信号连接
    connect(push_button_addIPC, &QPushButton::clicked, [this](){
        qDebug() << "用户点击添加按钮";
        emit videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Add));
    });
    connect(push_button_playOrPause, &QPushButton::clicked, [this](){
        qDebug() << "用户点击播放/暂停按钮";
        if (push_button_playOrPause->text() == "Play")
        {
            emit videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Play));
            push_button_playOrPause->setText("Pause");
        }
        else 
        {
            emit videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Pause));
            push_button_playOrPause->setText("Play");
        }
    });
    connect(push_button_snapshot, &QPushButton::clicked, [this](){
        qDebug() << "用户点击截图按钮";
        emit videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Snapshot));
    });
    connect(push_button_record, &QPushButton::clicked, [this](){
        qDebug() << "用户点击录像按钮";
        emit videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Record));
    });
    connect(push_button_fullscreen, &QPushButton::clicked, [this](){
        qDebug() << "用户点击全屏按钮";
        emit videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Fullscreen));
    });
    connect(push_button_ai, &QPushButton::clicked, [this](){
        qDebug() << "用户点击AI按钮";
        emit videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::AI));
    });
    connect(push_button_closeIPC, &QPushButton::clicked, [this](){
        qDebug() << "用户点击关闭按钮";
        emit videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Close));
    });
}

void VideoDisplayTooltip::paintEvent(QPaintEvent *event)
{
    // label_videoInfo->setFixedWidth(parentWidget()->width());

    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, 127));  // 设置背景颜色
    
    QWidget::paintEvent(event);
}

void VideoDisplayTooltip::resizeEvent(QResizeEvent *event)
{
    // label_videoInfo->setFixedWidth(parentWidget()->width());

    QWidget::resizeEvent(event);
}



