TEMPLATE = lib
CONFIG  -= qt
TARGET   = DBoW2
DESTDIR  = $$PWD/../bin

include(../3rdparty/opencv.pri)

HEADERS += \
    ../include/DBoW2/BowVector.h \
    ../include/DBoW2/DBoW2.h \
    ../include/DBoW2/FBrief.h \
    ../include/DBoW2/FClass.h \
    ../include/DBoW2/FeatureVector.h \
    ../include/DBoW2/FORB.h \
    ../include/DBoW2/FSurf64.h \
    ../include/DBoW2/QueryResults.h \
    ../include/DBoW2/ScoringObject.h \
    ../include/DBoW2/TemplatedDatabase.h \
    ../include/DBoW2/TemplatedVocabulary.h
    
SOURCES += \
    BowVector.cpp \
    FBrief.cpp \
    FeatureVector.cpp \
    FORB.cpp \
    FSurf64.cpp \
    QueryResults.cpp \
    ScoringObject.cpp

INCLUDEPATH += ../include/DBoW2
