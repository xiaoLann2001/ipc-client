#include <QObject>
#include <QMap>
#include <QInputDialog>
#include <QMenu>

#include "VideoStreamManager/VideoStreamManager.h"
#include "VideoView/VideoViewWidget.h"

#include "VideoSignalBus.h"

class VideoController : public QObject {
    Q_OBJECT

public:
    explicit VideoController(VideoStreamManager *manager, VideoViewWidget *view, QObject *parent = nullptr);
    ~VideoController();

public slots:
    // 用于响应视图层的槽函数
    void onVideoControlSignal(const VideoControlCommand &command);      // 视频控制命令

    void onAddVideoStream(int videoDisplayUnitId, const QString &url);  // 添加视频流到指定控件
    void onStopVideoDisplay(int videoDisplayUnitId);                    // 停止指定控件的视频显示
    void onOutOfGrid(int videoDisplayUnitId);                           // 视频控件超出网格
    void onVideoGridChanged(int page);                                  // 视频网格变化
    void onVideoPageChanged(int page);                                  // 视频页码变化

    void onAddIPCClicked();                                             // 添加 IPC 按钮点击

    void onVideoViewClicked(int videoDisplayUnitId);                    // 视频控件点击
    void onVideoViewRightClicked(int videoDisplayUnitId, const QPoint &pos);  // 视频控件右击



    // 用于响应模型层的槽函数
    void onNewFrameAvailable(int handle);                               // 新帧可用


private:
    void controlInit();                                                 // 初始化信号与槽

    void bindViewToStream(int videoDisplayUnitId, int handle);          // 将视频流绑定到指定控件
    void unbindViewFromStream(int videoDisplayUnitId);                  // 将视频流从指定控件解绑
    void createVideoStreamfromViewId(int videoDisplayUnitId, const QString &url); // 根据控件 ID 创建视频流
    void deleteVideoStreamfromViewId(int videoDisplayUnitId);                     // 根据控件 ID 删除视频流

    QString getUserInputFromDialog();                                   // 获取 IPC 地址对话框
    QString getUserOptionFromMenu(const QPoint &pos);                   // 获取用户选项菜单，需要传递鼠标位置

    QMap<int, int> m_displayUnitToHandle;                               // 控件 ID 到视频流句柄的映射
    QMap<int, int> m_handleToDisplayUnit;                               // 视频流句柄到控件 ID 的映射
    
    int m_currentGrid;                                                  // 当前网格数
    int m_currentPage;                                                  // 当前页码
    int m_currentVideoDisplayUnitId = -1;                               // 当前视频控件 ID

    VideoStreamManager *m_manager;                                      // 视频流管理器
    VideoViewWidget *m_view;                                            // 视频显示控件

    QInputDialog *m_inputDialog;                                        // IPC 地址输入对话框
    QMenu *m_menu;                                                      // 右键菜单
    QAction *m_addIPCAction;                                            // 添加 IPC 动作
    QAction *m_removeIPCAction;                                         // 移除 IPC 动作
};