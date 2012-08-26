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
        modelarea.cpp \
        modelworld.cpp \
        core/WGObject.cpp \
        core/Position.cpp

HEADERS  += mainwindow.h \
        modelarea.h \
        modelworld.h \
        core/WGObject.hpp \
        core/Position.hpp

FORMS    += mainwindow.ui

OTHER_FILES += \
    World.xml
