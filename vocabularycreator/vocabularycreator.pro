QT -= gui

CONFIG += c++11
CONFIG -= app_bundle

DESTDIR  = $$PWD/../bin

include(../3rdparty/opencv.pri)
include(../DBoW2.pri)

SOURCES += \
        main.cpp
