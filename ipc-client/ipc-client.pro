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
    src/application/ipcclientapplication.cpp \
    src/application/ipcclientcontroller.cpp \
    src/application/ipcclientview.cpp \
    src/event/eventview/eventwidget.cpp \
    src/setting/settingview/settingwidget.cpp \
    src/video/video/videostream.cpp \
    src/video/videomanager/videostreammanager.cpp \
    src/video/videoview/videodisplayunit.cpp \
    src/video/videoview/videodisplayviewpool.cpp \
    src/video/videoview/videogridview.cpp \
    src/video/videoview/videoviewwidget.cpp \
    src/main.cpp

HEADERS += \
    src/application/ipcclientapplication.h \
    src/application/ipcclientcontroller.h \
    src/application/ipcclientview.h \
    src/event/eventview/eventwidget.h \
    src/setting/settingview/settingwidget.h \
    src/video/video/videostream.h \
    src/video/videomanager/videostreammanager.h \
    src/video/videoview/videodisplayunit.h \
    src/video/videoview/videodisplayviewpool.h \
    src/video/videoview/videogridview.h \
    src/video/videoview/videoviewwidget.h

INCLUDEPATH += src \
    src/application \
    src/core \
    src/device \
    src/event \
    src/setting \
    src/storage \
    src/video


INCLUDEPATH += /usr/include/x86_64-linux-gnu
LIBS += -L/usr/lib/x86_64-linux-gnu -lavcodec -lavformat -lswscale -lavutil


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
