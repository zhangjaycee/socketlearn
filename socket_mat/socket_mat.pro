#-------------------------------------------------
#
# Project created by QtCreator 2015-08-25T21:00:47
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = socket_mat
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

 INCLUDEPATH += /usr/local/include \
                 /usr/local/include/opencv \
                 /usr/local/include/opencv2

 LIBS += /usr/local/lib/libopencv_highgui.so \
         /usr/local/lib/libopencv_core.so\
         /usr/local/lib/libopencv_imgproc.so\
         /usr/local/lib/libopencv_objdetect.so\
        /usr/local/lib/libopencv_contrib.so

HEADERS += \
    jc_wr.h
