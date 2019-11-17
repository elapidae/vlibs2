TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

#CONFIG -= qt  # test with qt.
CONFIG -= console

VLIBS_DIR = $$PWD/../..
include( $$VLIBS_DIR/qmake/gtest.pri )
include( $$VLIBS_DIR/qmake/vlog.pri  )

INCLUDEPATH += $$PWD



SOURCES += main.cpp \
    ../impl_vlog/pre_posix.cpp \
    ../impl_vlog/rotate_log_file.cpp \
    ../impl_vlog/log_file.cpp

HEADERS += \
    ../impl_vlog/pre_posix.h \
    ../impl_vlog/rotate_log_file.h \
    ../impl_vlog/log_file.h
