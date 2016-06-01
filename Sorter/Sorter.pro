#-------------------------------------------------
#
# Project created by QtCreator 2016-05-12T14:59:10
#
#-------------------------------------------------

QT       -= core gui

TARGET = Sorter
TEMPLATE = lib

SOURCES += src/Sorter.cpp \
        src/stdafx.cpp \
        src/utils.cpp \
        ../include/easyexif/exif.cpp

CONFIG += precompile_header
PRECOMPILED_HEADER = src/stdafx.h
HEADERS += src/Sorter.h \
        src/stdafx.h \
        src/utils.h \
        src/logger.h \
        src/targetver.h \
        ../include/easyexif/exif.h

INCLUDEPATH += ../include

CONFIG(debug, debug|release) {
    DESTDIR = ../bin/debug
    OBJECTS_DIR = ./debug
}
else {
    DESTDIR = ../bin/release
    OBJECTS_DIR = ./release
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
