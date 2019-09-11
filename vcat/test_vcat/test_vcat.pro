TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

VLIBS_DIR = $$PWD/../..
include( $$VLIBS_DIR/qmake/vcat.pri )
include( $$VLIBS_DIR/qmake/gtest.pri )

INCLUDEPATH += $$PWD

