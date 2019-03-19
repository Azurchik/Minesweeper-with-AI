#-------------------------------------------------
#
# Project created by QtCreator 2018-07-12T10:13:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.5.0.1
QMAKE_TARGET_COMPANY     = Azurchik
QMAKE_TARGET_PRODUCT     = Minesweeper
QMAKE_TARGET_DESCRIPTION = Game
QMAKE_TARGET_COPYRIGHT   = Bazko Yurii

TARGET = Minesweeper
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    area.cpp \
    cell.cpp \
    controller.cpp \
    bcell.cpp \
    block.cpp \
    bot.cpp \
    group.cpp \
    valuecell.cpp \
    table.cpp \
    options.cpp \
    advert.cpp \
    statistics.cpp

HEADERS += \
        mainwindow.h \
    area.h \
    cell.h \
    controller.h \
    bcell.h \
    block.h \
    bot.h \
    group.h \
    valuecell.h \
    table.h \
    options.h \
    advert.h \
    statistics.h

FORMS += \
        mainwindow.ui \
    options.ui \
    advert.ui \
    statistics.ui


RC_ICONS = Minesweeper.ico

RESOURCES += \
    resource.qrc
