TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp


VLIBS_DIR = $$PWD/../../vlibs2
include( $$VLIBS_DIR/qmake/vlog.pri )
include( $$VLIBS_DIR/qmake/vbyte_buffer.pri )
include( $$VLIBS_DIR/qmake/vnetwork.pri )

LIBS += -pthread
