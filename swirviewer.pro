#-------------------------------------------------
#
# Project created by QtCreator 2020-03-11T08:06:43
#
#-------------------------------------------------

QT       += core gui network printsupport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = swirviewer
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

CONFIG += c++11

win32 {
INCLUDEPATH += $$_PRO_FILE_PWD_\include \
#    $$OPENCV_DIR\include

LIBS += -L$$_PRO_FILE_PWD_/lib \
#    -L$$OPENCV_DIR\x64\vc16\lib \
    -lavcodec\
    -lavutil\
    -lavformat\
    -lswscale\
    -lswresample \

CONFIG(debug, debug|release) {
    LIBS += -lopencv_world340d
} else {
    LIBS += -lopencv_world340
}

}

unix {
INCLUDEPATH += /usr/local/include \
    /usr/local/include/opencv4

LIBS += -L/usr/local/lib \
    -lavcodec\
    -lavutil\
    -lavformat\
    -lswscale\
    -lswresample \
    -lopencv_world \
}

SOURCES += \
        glcanvas.cpp \
        imagebuffer.cpp \
        imagebutton.cpp \
        main.cpp \
        mainwindow.cpp \
        patchdialog.cpp \
        utils.cpp

HEADERS += \
        glcanvas.h \
        global.h \
        imagebuffer.h \
        imagebutton.h \
        mainwindow.h \
        patchdialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    swir.qrc

RC_ICONS = swir.ico
RC_FILE += version.rc
