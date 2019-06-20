DBOW2_DIR = $$PWD

INCLUDEPATH *= $$DBOW2_DIR/include

CONFIG(release, debug|release) {
    LIBS += -L$$DBOW2_DIR/bin/ -lDBoW2
} else {
    LIBS += -L$$DBOW2_DIR/bin/ -lDBoW2_d
}
