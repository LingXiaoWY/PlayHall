#-------------------------------------------------
#
# Project created by QtCreator 2022-10-25T22:31:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlayHall
TEMPLATE = app


SOURCES += main.cpp\
        maindialog.cpp \
    ckernel.cpp \
    logindialog.cpp \
    fiveinlinezone.cpp \
    roomitem.cpp \
    roomdialog.cpp

HEADERS  += maindialog.h \
    ckernel.h \
    logindialog.h \
    fiveinlinezone.h \
    roomitem.h \
    roomdialog.h

FORMS    += maindialog.ui \
    logindialog.ui \
    fiveinlinezone.ui \
    roomitem.ui \
    roomdialog.ui

include(./netapi/netapi.pri)
include(./md5/md5.pri)
include(./FIVE_IN_LINE/FiveInLine.pri)
INCLUDEPATH += ./netapi/net
INCLUDEPATH += ./netapi/mediator
INCLUDEPATH += ./md5/
INCLUDEPATH += ./FIVE_IN_LINE/

RESOURCES += \
    resource.qrc
