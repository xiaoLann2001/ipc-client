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
    src/Application/IPCClientApplication.cpp \
    src/Application/IPCClientController.cpp \
    src/Application/IPCClientView.cpp \
    src/Setting/SettingView/SettingWidget.cpp \
    src/Video/VideoStream/VideoStreamDecoder.cpp \
    src/Video/VideoStreamController/VideoStreamController.cpp \
    src/Video/VideoStreamManager/VideoStreamManager.cpp \
    src/Video/VideoView/VideoDeviceListView.cpp \
    src/Video/VideoView/VideoDisplayUnit.cpp \
    src/Video/VideoView/VideoDisplayUnitPool.cpp \
    src/Video/VideoView/VideoGridView.cpp \
    src/Video/VideoView/VideoPtzView.cpp \
    src/Video/VideoView/VideoViewWidget.cpp \
    src/main.cpp \
    src/Video/VideoView/VideoOperationView.cpp
    
    
    
    

HEADERS += \
    src/Application/IPCClientApplication.h \
    src/Application/IPCClientController.h \
    src/Application/IPCClientView.h \
    src/Setting/SettingView/SettingWidget.h \
    src/Video/VideoStream/VideoStreamDecoder.h \
    src/Video/VideoStreamController/VideoStreamController.h \
    src/Video/VideoStreamManager/VideoStreamManager.h \
    src/Video/VideoView/VideoDeviceListView.h \
    src/Video/VideoView/VideoDisplayUnit.h \
    src/Video/VideoView/VideoDisplayUnitPool.h \
    src/Video/VideoView/VideoGridView.h \
    src/Video/VideoView/VideoPtzView.h \
    src/Video/VideoView/VideoViewWidget.h \
    src/Video/VideoView/VideoOperationView.h
    
    
    
    

INCLUDEPATH += src \
    src/Application \
    src/Core \
    src/Event \
    src/Setting \
    src/Storage \
    src/Video


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
