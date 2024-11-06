#ifndef VIDEOVIEWWIDGET_H
#define VIDEOVIEWWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QPaintEvent>

class VideoViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoViewWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:

public slots:

private:

};

#endif // VIDEOVIEWWIDGET_H
