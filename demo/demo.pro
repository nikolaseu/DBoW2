TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt
DESTDIR  = $$PWD/../bin

include(../3rdparty/opencv.pri)
include(../DBoW2.pri)

SOURCES += demo.cpp
