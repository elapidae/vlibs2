TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

#CONFIG -= qt  # test with qt.
CONFIG -= console

VLIBS_DIR = $$PWD/../..
include( $$VLIBS_DIR/qmake/gtest.pri )
include( $$VLIBS_DIR/qmake/vlog.pri  )

INCLUDEPATH += $$PWD

SOURCES += main.cpp
