#-------------------------------------------------
#
# Project created by QtCreator 2015-04-09T05:26:51
#
#-------------------------------------------------

QT       += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DModelLoader
TEMPLATE = app


SOURCES += \
    sceneselector.cpp \
    sceneselect.cpp \
    scenebase.cpp \
    scene.cpp \
    openglwindow.cpp \
    modelloader.cpp \
    mainwindow.cpp

HEADERS  += \
    sceneselector.h \
    sceneselect.h \
    scenebase.h \
    scene.h \
    openglwindow.h \
    modelloader.h \
    mainwindow.h

FORMS    += \
    mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Programas/assimp-3.1.1/lib/release/ -lassimp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Programas/assimp-3.1.1/lib/debug/ -lassimp
else:unix: LIBS += -L$$PWD/../../../Programas/assimp-3.1.1/lib/ -lassimp

INCLUDEPATH += $$PWD/../../../Programas/assimp-3.1.1/include
DEPENDPATH += $$PWD/../../../Programas/assimp-3.1.1/include
