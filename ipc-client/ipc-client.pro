#-------------------------------------------------
#
# Project created by QtCreator 2024-11-04T12:57:25
#
#-------------------------------------------------

QT       += core gui multimedia svg

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

SOURCES +=   \
    src/Core/IPCClientApplication.cpp \
    src/Core/IPCClientController.cpp \
    src/Core/main.cpp \
    src/UI/IPCClientView.cpp \
    src/Video/AudioPlayer/AudioPlayer.cpp \
    src/Video/VideoStream/VideoStreamDecoder.cpp \
    src/Video/VideoStreamController/VideoStreamController.cpp \
    src/Video/VideoStreamManager/VideoStreamManager.cpp \
    src/Video/VideoView/VideoDisplayTooltip.cpp \
    src/Video/VideoView/VideoDisplayUnit.cpp \
    src/Video/VideoView/VideoGridView.cpp \
    src/Video/VideoView/VideoViewWidget.cpp
    
HEADERS +=   \
    src/Core/IPCClientApplication.h \
    src/Core/IPCClientController.h \
    src/Core/Logger.h \
    src/Frameworks/IPCClientCoreSignalBus.h \
    src/Frameworks/VideoControlCommand.h \
    src/Frameworks/VideoSignalBus.h \
    src/UI/IPCClientView.h \
    src/Utils/CustomQWidgetPool.h \
    src/Video/AudioPlayer/AudioHelper.h \
    src/Video/AudioPlayer/AudioPlayer.h \
    src/Video/VideoStream/VideoStreamDecoder.h \
    src/Video/VideoStreamController/VideoStreamController.h \
    src/Video/VideoStreamManager/VideoStreamManager.h \
    src/Video/VideoView/VideoDisplayTooltip.h \
    src/Video/VideoView/VideoDisplayUnit.h \
    src/Video/VideoView/VideoGridView.h \
    src/Video/VideoView/VideoViewWidget.h
    
INCLUDEPATH +=  \
    src/Core \
    src/Frameworks \
    src/UI \
    src/Utils \
    src/Video

QMAKE_CXXFLAGS += -v

#LIBS += -lavcodec -lavformat -lswscale -lavutil -lswresample
#message("QMAKE_CXX : $$QMAKE_CXX")

contains(QMAKE_CXX, /usr/bin/aarch64-linux-gnu-g++) {
    INCLUDEPATH += /opt/sysroot/usr/local/ffmpeg/include
    LIBS += -L/opt/sysroot/usr/local/ffmpeg/lib -lavcodec -lavformat -lswscale -lavutil -lswresample
    LIBS += -L/opt/sysroot/usr/local/x264/lib -lx264
    LIBS += -L/opt/sysroot/usr/local/x265/lib -lx265
    LIBS += -L/opt/sysroot/usr/local/lib -lrockchip_mpp
} else {
    INCLUDEPATH += /usr/local/ffmpeg/include
    LIBS += -L/usr/local/ffmpeg/lib -lavcodec -lavformat -lswscale -lavutil -lswresample
}

FORMS +=

RESOURCES += \
    resources/resources.qrc


# run "lrelease ipc-client.pro" to generate .qm file
TRANSLATIONS += resources/translations/lang_zh_CN.ts \
                resources/translations/lang_en.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /root
!isEmpty(target.path): INSTALLS += target

DISTFILES +=  \
    resources/fonts/AndBasR.ttf \
    resources/fonts/Bstgreek.ttf \
    resources/fonts/EcritureA-Italique-Ligne.otf \
    resources/fonts/EcritureA-Italique-Orne-Ligne.otf \
    resources/fonts/EcritureA-Italique-Orne.otf \
    resources/fonts/EcritureA-Italique.otf \
    resources/fonts/EcritureA-Romain-Ligne.otf \
    resources/fonts/EcritureA-Romain-Orne-Ligne.otf \
    resources/fonts/EcritureA-Romain-Orne.otf \
    resources/fonts/EcritureA-Romain.otf \
    resources/fonts/EcritureB-Italique-Ligne.otf \
    resources/fonts/EcritureB-Italique-Orne-Ligne.otf \
    resources/fonts/EcritureB-Italique-Orne.otf \
    resources/fonts/EcritureB-Italique.otf \
    resources/fonts/EcritureB-Romain-Ligne.otf \
    resources/fonts/EcritureB-Romain-Orne-Ligne.otf \
    resources/fonts/EcritureB-Romain-Orne.otf \
    resources/fonts/EcritureB-Romain.otf \
    resources/fonts/GETypoLibre.otf \
    resources/fonts/ec_cour.ttf \
    resources/fonts/ecl_cour.ttf \
    resources/fonts/wenquanyi.ttf \
    resources/icons/applogo.png \
    resources/icons/maximize.png \
    resources/translations/lang_en.qm \
    resources/translations/lang_zh_CN.qm