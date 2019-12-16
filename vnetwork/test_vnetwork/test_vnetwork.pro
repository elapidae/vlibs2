TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

#CONFIG -= console
CONFIG += console

QT += network

VLIBS_DIR = $$PWD/../..
include( $$VLIBS_DIR/qmake/gtest.pri )
include( $$VLIBS_DIR/qmake/vnetwork.pri )

SOURCES += main.cpp
