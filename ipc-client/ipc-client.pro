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

CONFIG += c++11 debug

SOURCES += \
        main.cpp \
        ipcclientmodel.cpp \
        ipcclientview.cpp \
        ipcclientcontroller.cpp \
        videoviewwidget.cpp \
        videoreplywidget.cpp \
        eventwidget.cpp \
        settingwidget.cpp \
    videogridview.cpp \
    videodisplayunit.cpp \
    videodisplayviewpool.cpp

HEADERS += \
        ipcclientmodel.h \
        ipcclientview.h \
        ipcclientcontroller.h \
        videoviewwidget.h \
        videoreplywidget.h \
        eventwidget.h \
        settingwidget.h \
    videogridview.h \
    videodisplayunit.h \
    videodisplayviewpool.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
