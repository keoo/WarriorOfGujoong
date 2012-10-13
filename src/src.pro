#-------------------------------------------------
#
# Project created by QtCreator 2012-08-01T22:16:23
#
#-------------------------------------------------

QT       += core gui xml

TARGET = src
TEMPLATE = app

OBJECTS_DIR=../obj
MOC_DIR=../obj
UI_DIR=../obj
DESTDIR=./

SOURCES += main.cpp\
        mainwindow.cpp \
        scene/graphicsscene.cpp \
        modelarea.cpp \
        modelworld.cpp \
        core/WGObject.cpp \
        core/Position.cpp \
        core/Perso.cpp \
        core/Item.cpp \
    scene/graphicsobject.cpp \
    computemoves.cpp \
    persostatistics.cpp \
    scene/actionmenuwindow.cpp \
    scene/graphictile.cpp

HEADERS  += mainwindow.h \
        scene/graphicsscene.hpp \
        modelarea.h \
        modelworld.h \
        core/WGObject.hpp \
        core/Position.hpp \
        core/Item.hpp \
        core/Perso.hpp \
        core/PState.hpp \
    scene/graphicsobject.hpp \
    computemoves.hpp \
    persostatistics.hpp \
    scene/actionmenuwindow.hpp \
    constants/ChainConstants.hpp \
    util/direction.hpp \
    scene/graphictile.hpp

FORMS    += mainwindow.ui \
    persostatistics.ui

OTHER_FILES += \
    World.xml
