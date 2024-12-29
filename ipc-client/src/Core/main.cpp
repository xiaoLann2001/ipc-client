
#include <IPCClientApplication.h>

int main(int argc, char *argv[])
{
    // 设置 OpenGL 上下文共享属性
    IPCClientApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    // 设置高分辨率屏幕支持
    IPCClientApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    IPCClientApplication application(argc, argv);

    return application.exec();
}
