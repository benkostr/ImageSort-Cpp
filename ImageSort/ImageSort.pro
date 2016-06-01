#-------------------------------------------------
#
# Project created by QtCreator 2016-05-12T14:20:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageSort
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp
RESOURCES = resources/images.qrc \
    resources/images.qrc

INCLUDEPATH += ../include\
            ../Sorter/src

CONFIG += precompile_header
PRECOMPILED_HEADER = ../Sorter/src/stdafx.h
HEADERS  += src/mainwindow.h \
            ../Sorter/src/stdafx.h

FORMS    += src/mainwindow.ui

ImageSort.depends = Sorter

CONFIG(debug, debug|release) {
    LIBS += -L../bin/debug -lSorter
    DESTDIR = ../bin/debug
    OBJECTS_DIR = ./debug
}
else {
    LIBS += -L../bin/release -lSorter
    DESTDIR = ../bin/release
    OBJECTS_DIR = ./release
}

win32:RC_FILE = resources/imagesort.rc
