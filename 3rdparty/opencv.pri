# Edit this section to make sure the paths match your system configuration

# Windows
win32 {
    OPENCV_DIR = C:/Users/nikol/Projects/opencv-4.1.0-vc14_vc15/build
    OPENCV_VER = 410

    OPENCV_INCLUDE_DIR = $$OPENCV_DIR/include
    OPENCV_LIB_DIR = $$OPENCV_DIR/x64/vc15/lib
}

# Linux
unix:!macx {
    OPENCV_INCLUDE_DIR = "/usr/local/include"
    OPENCV_LIB_DIR = "/usr/local/lib"
}

# macOS
macx: {
    OPENCV_INCLUDE_DIR = "/usr/local/opt/opencv/include/opencv4"
    OPENCV_LIB_DIR = "/usr/local/opt/opencv/lib"
}

# Android
android: {
    include(opencv_android.pri)
}

win32: {
CV_LIB_NAMES = world
}
else: {
CV_LIB_NAMES = core imgproc imgcodecs videoio calib3d features2d flann highgui #rgbd viz
}

for(lib, CV_LIB_NAMES) {
    CV_LIBS += -lopencv_$$lib
}

win32: {
    CONFIG(release, debug|release) {
        CV_LIB_PREFIX = $$OPENCV_VER
    } else {
        CV_LIB_PREFIX = $${OPENCV_VER}d
    }
    for(lib, CV_LIBS) {
        CV_LIBS_NEW += $$lib$$CV_LIB_PREFIX
    }
    CV_LIBS = $$CV_LIBS_NEW
}

LIBS += -L$$OPENCV_LIB_DIR $$CV_LIBS
INCLUDEPATH *= $$OPENCV_INCLUDE_DIR
