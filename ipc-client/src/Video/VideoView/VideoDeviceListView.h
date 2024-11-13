#ifndef VIDEODEVICELISTVIEW_H
#define VIDEODEVICELISTVIEW_H

#include <QWidget>
#include <QLabel>
#include <QTreeView>
#include <QStandardItemModel>

class VideoDeviceListView : public QWidget
{
    Q_OBJECT
public:
    explicit VideoDeviceListView(QWidget *parent = nullptr);

    // void setModel(QStandardItemModel *model);

signals:

public slots:
    // QLabel *label_videodevicelist;              // 视频设备列表标签
    // QTreeView *treeview_videodevicelist;        // 视频设备列表树形视图
};

#endif // VIDEODEVICELISTVIEW_H