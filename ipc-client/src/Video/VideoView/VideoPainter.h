#ifndef VIDEOPAINTER_H
#define VIDEOPAINTER_H

#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QMouseEvent>
#include <QGestureEvent>
#include <QMutex>

#include "VideoToolbar.h"
#include "VideoSignalBus.h"

/**
 * @brief 视频显示单元，使用 QPainter 绘制图像
 */
class VideoPainter : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPainter(QWidget *parent = nullptr);
    explicit VideoPainter(int id, QWidget *parent = nullptr);

    void setId(int id) { m_id_ = id; }
    int getId() const { return m_id_; }
    void setImage(const QImage &image);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_id_ = -1;                 // 唯一 ID

    QImage m_image_;                // 图像
    QMutex mtx_image_;              // 互斥锁，保护图像
};

#endif // VIDEOPainter_H