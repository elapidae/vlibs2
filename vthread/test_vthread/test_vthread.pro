TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

CONFIG -= console

VLIBS_DIR = $$PWD/../..
include( $$VLIBS_DIR/qmake/gtest.pri    )
include( $$VLIBS_DIR/qmake/vthread.pri  )
include( $$VLIBS_DIR/qmake/vlog.pri     )

SOURCES += main.cpp
