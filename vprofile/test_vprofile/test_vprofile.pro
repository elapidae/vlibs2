TEMPLATE = app
CONFIG += c++11
#CONFIG -= app_bundle

CONFIG -= console
#CONFIG -= qt  # test with qt.
CONFIG += qt
QT += gui widgets

VLIBS_DIR = $$PWD/../..
include( $$VLIBS_DIR/qmake/gtest.pri )
include( $$VLIBS_DIR/qmake/vlog.pri  )

include( $$VLIBS_DIR/qmake/vprofile.pri )
#DEFINES -= V_NEED_PROFILE
#DEFINES -= V_HAS_QT

SOURCES += main.cpp
