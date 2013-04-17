#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T14:46:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = disp_client
TEMPLATE = app


SOURCES += main.cpp\
        dispwindow.cpp

HEADERS  += dispwindow.h
LIBS     += -lmarblewidget -lQtGui -lQtCore
FORMS    += dispwindow.ui
