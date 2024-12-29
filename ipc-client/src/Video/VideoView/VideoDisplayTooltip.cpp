#include "VideoDisplayTooltip.h"

#include <QDebug>
#include <QToolTip>

VideoDisplayTooltip::VideoDisplayTooltip(QWidget *parent)
    : QWidget(parent)
{
    tooltipInit();          // 初始化悬浮窗口
    controlInit();          // 初始化信号与槽
}

void VideoDisplayTooltip::setTooltipText(const QString &text)
{
    label_videoInfo->setText(text);
}

void VideoDisplayTooltip::setTooltipMode(bool haveAdd)
{
    if (haveAdd) {      // 已添加视频流
        push_button_add->hide();
        push_button_playOrPause->show();
        push_button_snapshot->show();
        push_button_record->show();
        push_button_fullscreen->show();
        push_button_ai->show();
        push_button_close->show();
    } else {            // 未添加视频流
        push_button_add->show();
        push_button_playOrPause->hide();
        push_button_snapshot->hide();
        push_button_record->hide();
        push_button_fullscreen->hide();
        push_button_ai->hide();
        push_button_close->hide();
    }
}

void VideoDisplayTooltip::tooltipInit()
{
    // 初始化悬浮窗，默认不可见
    // this->setFixedWidth(parentWidget()->width());
    this->setStyleSheet("background-color: rgba(0, 0, 0, 127); color: white; font-size: 12px; border-radius: 5px;");
    this->setFixedHeight(30);                               // 悬浮窗高度固定为30

    // 添加水平布局
    layout = new QHBoxLayout();
    layout->setContentsMargins(5, 0, 5, 0);                 // 设置布局边距
    label_videoInfo = new QLabel();                         // 默认显示的文本
    // m_label->setStyleSheet("color: white; font-size: 12px;");

    // 未添加视频流时的控制按钮
    push_button_add = new QPushButton("Add");               // 添加视频按钮

    // 已添加视频流时的控制按钮
    push_button_playOrPause = new QPushButton("Play");      // 播放或暂停按钮
    push_button_snapshot = new QPushButton("Snapshot");     // 截图按钮
    push_button_record = new QPushButton("Record");         // 录像按钮
    push_button_fullscreen = new QPushButton("Fullscreen"); // 全屏按钮
    push_button_ai = new QPushButton("AI");                 // AI按钮
    push_button_close = new QPushButton("Close");           // 关闭视频按钮

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
    push_button_add->setStyleSheet(button_style);
    push_button_playOrPause->setStyleSheet(button_style);
    push_button_snapshot->setStyleSheet(button_style);
    push_button_record->setStyleSheet(button_style);
    push_button_fullscreen->setStyleSheet(button_style);
    push_button_ai->setStyleSheet(button_style);
    push_button_close->setStyleSheet(button_style);

    // 添加提示信息
    label_videoInfo->setToolTip("视频信息");
    push_button_add->setToolTip("添加视频源");
    push_button_playOrPause->setToolTip("播放/暂停");
    push_button_snapshot->setToolTip("截图");
    push_button_record->setToolTip("录像");
    push_button_fullscreen->setToolTip("全屏");
    push_button_ai->setToolTip("AI使能");
    push_button_close->setToolTip("关闭视频源");

    // 添加按钮到布局
    layout->addWidget(label_videoInfo);
    layout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Maximum));
    layout->addWidget(push_button_add);
    layout->addWidget(push_button_playOrPause);
    layout->addWidget(push_button_snapshot);
    layout->addWidget(push_button_record);
    layout->addWidget(push_button_fullscreen);
    layout->addWidget(push_button_ai);
    layout->addWidget(push_button_close);

    // 设置布局到悬浮窗口
    this->setLayout(layout);
}

void VideoDisplayTooltip::controlInit()
{
    // 控制按钮信号连接
    connect(push_button_add, &QPushButton::clicked, [this](){
        qDebug() << "用户点击添加按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Add));
    });
    connect(push_button_playOrPause, &QPushButton::clicked, [this](){
        qDebug() << "用户点击播放/暂停按钮";
        if (push_button_playOrPause->text() == "Play")
        {
            emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Play));
            push_button_playOrPause->setText("Pause");
        }
        else 
        {
            emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Pause));
            push_button_playOrPause->setText("Play");
        }
    });
    connect(push_button_snapshot, &QPushButton::clicked, [this](){
        qDebug() << "用户点击截图按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Snapshot));
    });
    connect(push_button_record, &QPushButton::clicked, [this](){
        qDebug() << "用户点击录像按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Record));
    });
    connect(push_button_fullscreen, &QPushButton::clicked, [this](){
        qDebug() << "用户点击全屏按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Fullscreen));
    });
    connect(push_button_ai, &QPushButton::clicked, [this](){
        qDebug() << "用户点击AI按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::AI));
    });
    connect(push_button_close, &QPushButton::clicked, [this](){
        qDebug() << "用户点击关闭按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Close));
    });
}

/**
 * @brief 工具悬浮窗事件过滤器，监控父对象大小改变事件，调整悬浮窗口的宽度
 * @param watched 受监视的父对象
 * @param event 分发给父对象的事件
 * @return bool 事件是否被处理
*/
bool VideoDisplayTooltip::eventFilter(QObject *watched, QEvent *event)
{
    // 当监视对象大小改变时，调整悬浮窗口的宽度
    if (event->type() == QEvent::Resize)
    {
        if (parentWidget() != nullptr) {
            this->setFixedWidth(parentWidget()->width());
        }
        return true;
    } 
    else if (event->type() == QEvent::Enter)
    {
        // 当鼠标进入父窗口时，显示悬浮窗
        if (!this->isVisible()) {
            // 设置悬浮窗的位置为父窗口控件的顶部对齐
            this->move(QPoint(0, 0));

            // 置顶并显示悬浮窗
            this->raise();
            this->show();
        }
        return true;
    }
    else if (event->type() == QEvent::Leave)
    {
        // 当鼠标离开控件时，隐藏悬浮窗
        if (this->isVisible()) {
            this->hide();
        }
        return true;
    }

    return QWidget::eventFilter(watched, event);
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



