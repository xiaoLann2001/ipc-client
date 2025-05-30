#ifndef VIDEOTOOLBAR_H
#define VIDEOTOOLBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QPainter>
#include <QHBoxLayout>
#include <QEvent>

#include "VideoSignalBus.h"

const QString ICON_ADD = ":/icons/toolbar/toolbar_add.png";
const QString ICON_PLAY = ":/icons/toolbar/toolbar_play.png";
const QString ICON_PAUSE = ":/icons/toolbar/toolbar_pause.png";
const QString ICON_SNAPSHOT = ":/icons/toolbar/toolbar_snapshot.png";
const QString ICON_RECORD = ":/icons/toolbar/toolbar_record.png";
const QString ICON_FULLSCREEN = ":/icons/toolbar/toolbar_fullscreen.png";
const QString ICON_RESTORE = ":/icons/toolbar/toolbar_restore.png";
const QString ICON_AI = ":/icons/toolbar/toolbar_ai.png";
const QString ICON_CLOSE = ":/icons/toolbar/toolbar_close.png";

class VideoToolbar : public QWidget
{
    Q_OBJECT
public:
    explicit VideoToolbar(QWidget *parent = nullptr);

    void setId(int id) { m_id_ = id; }                  // 设置 ID
    int getId() const { return m_id_; }                 // 获取 ID
    void setToolbarText(const QString &text);           // 设置悬浮窗口显示文本
    void setToolbarMode(bool haveAddIPC);               // 设置悬浮窗口显示模式
    void syncToolbarMode();                             // 同步悬浮窗口显示模式

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    // 已通过信号总线发送
    // void videoControlSignal(VideoControlCommand command);

private:
    void toolbarInit();                     // 初始化悬浮窗口
    void controlInit();                     // 初始化控制按钮
    // void updateTooltip();                // 更新悬浮窗口

    int m_id_;                              // 唯一 ID

    QHBoxLayout *layout;                    // 悬浮窗口布局
    QLabel *label_videoInfo;                // 悬浮窗口标签

    // 未添加视频流时的控制按钮
    QPushButton *push_button_add;           // 添加视频按钮

    // 已添加视频流时的控制按钮
    QPushButton *push_button_playOrPause;   // 播放或暂停按钮
    QPushButton *push_button_snapshot;      // 截图按钮
    QPushButton *push_button_record;        // 录像按钮
    QPushButton *push_button_fullscreen;    // 全屏按钮
    QPushButton *push_button_ai;            // AI启停按钮
    QPushButton *push_button_close;         // 关闭视频按钮
};

#endif // VIDEOTOOLbar_H