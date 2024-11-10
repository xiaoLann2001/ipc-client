#-------------------------------------------------
#
# Project created by QtCreator 2024-11-04T12:57:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ipc-client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 thread debug

SOURCES += \
    src/application/IPCClientApplication.cpp \
    src/application/IPCClientController.cpp \
    src/application/IPCClientView.cpp \
    src/event/eventview/eventwidget.cpp \
    src/setting/settingview/settingwidget.cpp \
    src/video/VideoStream/VideoStreamDecoder.cpp \
    src/video/VideoStreamController/VideoStreamController.cpp \
    src/video/VideoStreamManager/VideoStreamManager.cpp \
    src/video/VideoView/VideoDisplayUnit.cpp \
    src/video/VideoView/VideoDisplayUnitPool.cpp \
    src/video/VideoView/VideoGridView.cpp \
    src/video/VideoView/VideoViewWidget.cpp \
    src/main.cpp
    
    
    

HEADERS += \
    src/application/IPCClientApplication.h \
    src/application/IPCClientController.h \
    src/application/IPCClientView.h \
    src/event/eventview/eventwidget.h \
    src/setting/settingview/settingwidget.h \
    src/video/VideoStream/VideoStreamDecoder.h \
    src/video/VideoStreamController/VideoStreamController.h \
    src/video/VideoStreamManager/VideoStreamManager.h \
    src/video/VideoView/VideoDisplayUnit.h \
    src/video/VideoView/VideoDisplayUnitPool.h \
    src/video/VideoView/VideoGridView.h \
    src/video/VideoView/VideoViewWidget.h
    
    
    

INCLUDEPATH += src \
    src/application \
    src/core \
    src/device \
    src/event \
    src/setting \
    src/storage \
    src/video


INCLUDEPATH += 3rdparty/ffmpeg/include
LIBS += -L 3rdparty/ffmpeg/lib -lavcodec -lavformat -lswscale -lavutil

INCLUDEPATH += /usr/include/opencv2
LIBS += -L/usr/lib -lopencv_core -lopencv_imgproc -lopencv_highgui

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc

# run "lrelease ipc-client.pro" to generate .qm file
TRANSLATIONS += resources/translations/lang_zh_CN.ts \
                resources/translations/lang_en.ts
