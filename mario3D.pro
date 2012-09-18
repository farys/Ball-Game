#-------------------------------------------------
#
# Project created by QtCreator 2012-06-07T07:19:50
#
#-------------------------------------------------

QT       += core gui
QT += opengl

TARGET = mario3D
TEMPLATE = app


SOURCES += main.cpp\
        game.cpp \
    c3ds.cpp \
    maptile.cpp \
    sceneactor.cpp \
    gamestate.cpp \
    point3f.cpp \
    player.cpp \
    character.cpp \
    cristal.cpp

HEADERS  += game.h \
    c3ds.h \
    maptile.h \
    sceneactor.h \
    gamestate.h \
    point3f.h \
    register.h \
    player.h \
    character.h \
    cristalmodel.h \
    cristal.h

FORMS    +=
LIBS += -L/usr/local/lib -lGLU
