#-------------------------------------------------
#
# Project created by QtCreator 2012-08-01T22:16:23
#
#-------------------------------------------------

QT       += core gui xml

TARGET = src
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        graphicsscene.cpp \
        modelarea.cpp \
        modelworld.cpp \
        core/WGObject.cc \
        core/Position.cc

HEADERS  += mainwindow.h \
        graphicsscene.hpp \
        modelarea.h \
        modelworld.h \
        core/WGObject.hh \
        core/Position.hh

FORMS    += mainwindow.ui

OTHER_FILES += \
    World.xml
