TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

CONFIG -= console
#CONFIG -= qt  # test with qt.

VLIBS_DIR = $$PWD/../..
include( $$VLIBS_DIR/qmake/gtest.pri )
include( $$VLIBS_DIR/qmake/vserial_port.pri )
include( $$VLIBS_DIR/qmake/vtimer.pri )

SOURCES += main.cpp
