# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = lib
TARGET = sbase_qtgui
DESTDIR = ../../../lib/win32
QT += core gui
CONFIG += staticlib
DEFINES += SBASE_QTGUI_LIB
INCLUDEPATH += ./../../../include \
    ./../../../include/gui \
    ./../../../include/gui/qt \
    ./../../../include/gui/svg \
    ./GeneratedFiles \
    ./GeneratedFiles/$(Configuration) \
    . \
    ./../../../src/gui/qt
PRECOMPILED_HEADER = StdAfx.h
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(sbase_qtgui.pri)
win32:RC_FILE = sbase_qtgui.rc