#ifndef VIDEODISPLAYUNIT_H
#define VIDEODISPLAYUNIT_H

#include <QWidget>

class VideoDisplayUnit : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDisplayUnit(int id, QWidget *parent = nullptr);
    void setImage(const QImage &image);
    int getId() const { return id_videodisplayunit_; }

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    QImage m_image_;

    int id_videodisplayunit_ = -1; // 唯一 ID

signals:
    void clicked(int id_videodisplayunit); // 点击信号, 传递唯一 ID

public slots:
};

#endif // VIDEODISPLAYUNIT_H