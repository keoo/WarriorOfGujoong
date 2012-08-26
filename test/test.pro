#-------------------------------------------------
#
# Project created by QtCreator 2012-08-26T15:44:57
#
#-------------------------------------------------

QT       += testlib core xml gui

TARGET = tst_testtest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += \
    AutoTest.hpp

SOURCES += \
    main.cpp \
    tst_modeltest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"


# Sources for model test
SOURCES += \
    ../src/modelarea.cpp \
    ../src/modelworld.cpp

HEADERS += ../src/modelarea.h \
    ../src/modelworld.h




