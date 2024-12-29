#ifndef VIDEODISPLAYTOOLTIP_H
#define VIDEODISPLAYTOOLTIP_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QPainter>
#include <QHBoxLayout>
#include <QEvent>

#include "VideoSignalBus.h"

class VideoDisplayTooltip : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDisplayTooltip(QWidget *parent = nullptr);

    void setTooltipText(const QString &text);           // 设置悬浮窗口显示文本
    void setTooltipMode(bool haveAddIPC);               // 设置悬浮窗口显示模式
    int getId() const { return m_id_; }                 // 获取 ID
    void setId(int id) { m_id_ = id; }                  // 设置 ID

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
    // 已通过信号总线发送
    // void videoControlSignal(VideoControlCommand command);

private:
    void tooltipInit();                     // 初始化悬浮窗口
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

#endif // VIDEODISPLAYTOOLTIP_H