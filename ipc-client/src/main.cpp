
#include <IPCClientApplication.h>

int main(int argc, char *argv[])
{
    // 设置 OpenGL 上下文共享属性
    IPCClientApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

    IPCClientApplication application(argc, argv);

    return application.exec();
}
