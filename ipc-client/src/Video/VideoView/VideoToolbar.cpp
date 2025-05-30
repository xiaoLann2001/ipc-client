#include "VideoToolbar.h"

#include <QDebug>
#include <QToolTip>

VideoToolbar::VideoToolbar(QWidget *parent)
    : QWidget(parent)
{
    toolbarInit(); // 初始化悬浮窗口
    controlInit(); // 初始化信号与槽
}

void VideoToolbar::setToolbarText(const QString &text)
{
    label_videoInfo->setText(text);
}

void VideoToolbar::setToolbarMode(bool haveAdd)
{
    if (haveAdd)
    { // 已添加视频流
        push_button_add->hide();
        push_button_playOrPause->show();
        push_button_snapshot->show();
        push_button_record->show();
        push_button_fullscreen->show();
        push_button_ai->show();
        push_button_close->show();
    }
    else
    { // 未添加视频流
        push_button_add->show();
        push_button_playOrPause->hide();
        push_button_snapshot->hide();
        push_button_record->hide();
        push_button_fullscreen->hide();
        push_button_ai->hide();
        push_button_close->hide();
    }
}

void VideoToolbar::syncToolbarMode()
{
    // 发送同步信号到视频控制总线
    emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Sync));
}

void VideoToolbar::toolbarInit()
{
    // 初始化悬浮窗，默认不可见
    // this->setFixedWidth(parentWidget()->width());
    this->setStyleSheet("background-color: rgba(0, 0, 0, 127); color: white; font-size: 12px; border-radius: 5px;");
    this->setFixedHeight(30); // 悬浮窗高度固定为30

    // 添加水平布局
    layout = new QHBoxLayout();
    layout->setContentsMargins(5, 0, 5, 0); // 设置布局边距
    label_videoInfo = new QLabel();         // 默认显示的文本
    // m_label->setStyleSheet("color: white; font-size: 12px;");

    // 未添加视频流时的控制按钮
    push_button_add = new QPushButton(); // 添加视频按钮

    // 已添加视频流时的控制按钮
    push_button_playOrPause = new QPushButton(); // 播放或暂停按钮
    push_button_snapshot = new QPushButton();    // 截图按钮
    push_button_record = new QPushButton();      // 录像按钮
    push_button_fullscreen = new QPushButton();  // 全屏按钮
    push_button_ai = new QPushButton();          // AI按钮
    push_button_close = new QPushButton();       // 关闭视频按钮

    // 设置切换按钮
    push_button_playOrPause->setCheckable(true); // 播放/暂停按钮，true 为正在播放状态，显示暂停，false 为暂停状态，显示播放
    push_button_fullscreen->setCheckable(true);  // 全屏/恢复按钮，true 为全屏状态，显示恢复，false 为恢复状态，显示全屏

    push_button_playOrPause->setChecked(false); // 默认为未播放状态
    push_button_fullscreen->setChecked(false);  // 默认为非全屏状态

    // 设置按钮图标
    push_button_add->setIcon(QIcon(ICON_ADD));
    push_button_playOrPause->setIcon(QIcon(ICON_PLAY));
    push_button_snapshot->setIcon(QIcon(ICON_SNAPSHOT));
    push_button_record->setIcon(QIcon(ICON_RECORD));
    push_button_fullscreen->setIcon(QIcon(ICON_FULLSCREEN));
    push_button_ai->setIcon(QIcon(ICON_AI));
    push_button_close->setIcon(QIcon(ICON_CLOSE));
    // 设置按钮图标大小
    push_button_add->setIconSize(QSize(20, 20));
    push_button_playOrPause->setIconSize(QSize(20, 20));
    push_button_snapshot->setIconSize(QSize(20, 20));
    push_button_record->setIconSize(QSize(20, 20));
    push_button_fullscreen->setIconSize(QSize(20, 20));
    push_button_ai->setIconSize(QSize(20, 20));
    push_button_close->setIconSize(QSize(20, 20));

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

void VideoToolbar::controlInit()
{
    // 控制按钮信号连接
    connect(push_button_add, &QPushButton::clicked, [this]() {
        qDebug() << "用户点击添加按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Add)); 
    });
    connect(push_button_playOrPause, &QPushButton::clicked, [this]() {
        qDebug() << "用户点击播放/暂停按钮";
        if (!push_button_playOrPause->isChecked()) {
            emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Play));
        } else {
            emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Pause));
        } 
    });
    connect(push_button_snapshot, &QPushButton::clicked, [this]() {
        qDebug() << "用户点击截图按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Snapshot)); 
    });
    connect(push_button_record, &QPushButton::clicked, [this]() {
        qDebug() << "用户点击录像按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Record)); 
    });
    connect(push_button_fullscreen, &QPushButton::clicked, [this]() {
        qDebug() << "用户点击全屏按钮";
        if (!push_button_fullscreen->isChecked()) {
            emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Fullscreen));
        } else {
            emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Restore));
        } 
    });
    connect(push_button_ai, &QPushButton::clicked, [this]() {
        qDebug() << "用户点击AI按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::AI)); 
    });
    connect(push_button_close, &QPushButton::clicked, [this]() {
        qDebug() << "用户点击关闭按钮";
        emit VideoSignalBus::instance()->videoControlSignal(VideoControlCommand(m_id_, VideoControlCommand::Command::Close)); 
    });

    // 应答信号连接
    connect(VideoSignalBus::instance(), &VideoSignalBus::videoControlResponse, [this](const VideoControlResponse &response) {
        if (response.id == m_id_) {
            switch (response.cmd) {
            case VideoControlResponse::Command::Add:
                if (response.resp == VideoControlResponse::Response::Success) {
                    // 添加成功
                    setToolbarMode(true);
                } else {
                    // 添加失败
                    setToolbarMode(false);
                }
                break;
            case VideoControlResponse::Command::Close:
                if (response.resp == VideoControlResponse::Response::Success) {
                    // 关闭成功
                    setToolbarMode(false);
                } else {
                    // 关闭失败
                    setToolbarMode(true);
                }
                break;
            case VideoControlResponse::Command::Play:
                if (response.resp == VideoControlResponse::Response::Success) {
                    // 播放成功
                    // push_button_playOrPause->setChecked(true);   // isChecked() 由Qt自动维护，不需要手动设置
                    push_button_playOrPause->setIcon(QIcon(ICON_PAUSE));
                }
                break;
            case VideoControlResponse::Command::Pause:
                if (response.resp == VideoControlResponse::Response::Success) {
                    // 暂停成功
                    push_button_playOrPause->setIcon(QIcon(ICON_PLAY));
                }
                break;
            case VideoControlResponse::Command::Fullscreen:
                if (response.resp == VideoControlResponse::Response::Success) {
                    // 全屏成功
                    push_button_fullscreen->setIcon(QIcon(ICON_RESTORE));
                }
                break;
            case VideoControlResponse::Command::Restore:
                if (response.resp == VideoControlResponse::Response::Success) {
                    // 恢复成功
                    push_button_fullscreen->setIcon(QIcon(ICON_FULLSCREEN));
                }
                break;
            case VideoControlResponse::Command::Snapshot:
                break;
            case VideoControlResponse::Command::Record:
                break;
            case VideoControlResponse::Command::AI:
                break;
            case VideoControlResponse::Command::Sync:
                // 同步状态
                if (response.resp == VideoControlResponse::Response::Success) {
                    // 如果有添加视频流，则显示已添加状态
                    setToolbarMode(true);
                } else {
                    // 否则显示未添加状态
                    setToolbarMode(false);
                }
                break;
            }
        } 
    });
}

/**
 * @brief 工具悬浮窗事件过滤器，监控父对象大小改变事件，调整悬浮窗口的宽度
 * @param watched 受监视的父对象
 * @param event 分发给父对象的事件
 * @return bool 事件是否被处理
 */
bool VideoToolbar::eventFilter(QObject *watched, QEvent *event)
{
    // 当监视对象大小改变时，调整悬浮窗口的宽度
    if (event->type() == QEvent::Resize)
    {
        if (parentWidget() != nullptr)
        {
            this->setFixedWidth(parentWidget()->width());
        }
        return true;
    }
    else if (event->type() == QEvent::Enter)
    {
        syncToolbarMode(); // 同步悬浮窗口显示模式

        // 当鼠标进入父窗口时，显示悬浮窗
        if (!this->isVisible())
        {
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
        if (this->isVisible())
        {
            this->hide();
        }
        return true;
    }

    return QWidget::eventFilter(watched, event);
}

void VideoToolbar::paintEvent(QPaintEvent *event)
{
    // label_videoInfo->setFixedWidth(parentWidget()->width());

    QPainter painter(this);
    painter.fillRect(rect(), QColor(0, 0, 0, 127)); // 设置背景颜色

    QWidget::paintEvent(event);
}

void VideoToolbar::resizeEvent(QResizeEvent *event)
{
    // label_videoInfo->setFixedWidth(parentWidget()->width());

    QWidget::resizeEvent(event);
}
