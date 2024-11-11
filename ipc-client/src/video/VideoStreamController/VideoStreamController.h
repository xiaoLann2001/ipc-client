#include <QObject>
#include <QMap>
#include "VideoStreamManager/VideoStreamManager.h"
#include "VideoView/VideoViewWidget.h"

class VideoStreamController : public QObject {
    Q_OBJECT

public:
    explicit VideoStreamController(VideoStreamManager *manager, VideoViewWidget *view, QObject *parent = nullptr);
    ~VideoStreamController();

public slots:
    // 用于响应视图层的槽函数
    void onAddVideoStream(int videoDisplayUnitId, const QString &url);  // 添加视频流到指定控件
    void onStopVideoDisplay(int videoDisplayUnitId);                    // 停止指定控件的视频显示
    void onOutOfGrid(int videoDisplayUnitId);                           // 视频控件超出网格
    void onVideoGridChanged(int page);                                  // 视频网格变化
    void onVideoPageChanged(int page);                                  // 视频页码变化

    void onAddIPCClicked();                                             // 添加 IPC 按钮点击



    // 用于响应模型层的槽函数
    void onNewFrameAvailable(int handle);                               // 新帧可用


private:
    void bindViewToStream(int videoDisplayUnitId, int handle);          // 将视频流绑定到指定控件

    QMap<int, int> m_displayUnitToHandle;                               // 控件 ID 到视频流句柄的映射
    QMap<int, int> m_handleToDisplayUnit;                               // 视频流句柄到控件 ID 的映射

    VideoStreamManager *m_manager;
    VideoViewWidget *m_view;
};