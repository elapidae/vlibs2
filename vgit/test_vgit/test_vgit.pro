TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

#CONFIG -= qt  # test with qt.

SOURCES += main.cpp

VLIBS_DIR = $$PWD/../..
include( $$VLIBS_DIR/qmake/gtest.pri )

MAIN_DIR = $$PWD
include( $$VLIBS_DIR/qmake/vgit.pri )

INCLUDEPATH += $$PWD
