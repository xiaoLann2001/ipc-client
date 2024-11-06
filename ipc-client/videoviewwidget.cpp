#include "videoviewwidget.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QColor>

VideoViewWidget::VideoViewWidget(QWidget *parent) : QWidget(parent)
{
    // 创建按钮
    QPushButton *pushButton_test = new QPushButton("test");

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout();
    
    // 将按钮添加到布局中
    layout->addWidget(pushButton_test);

    // 设置布局
    setLayout(layout);

//    // 设置窗口大小
//    resize(1024, 100);
}

// 重载绘图事件处理函数
void VideoViewWidget::paintEvent(QPaintEvent *event)
{
    // 调用基类的绘图事件
    QWidget::paintEvent(event);

    // 创建 QPainter 对象
    QPainter painter(this);

    // 设置背景填充颜色
    painter.fillRect(this->rect(), QColor("lightgreen")); // 用 lightgreen 填充整个背景

    // 其他绘制代码（如绘制形状或文本）可以在这里添加
    painter.setPen(Qt::black);
    painter.setBrush(Qt::yellow);
    painter.drawRect(50, 50, 200, 100); // 在窗口的特定位置绘制一个矩形
}
