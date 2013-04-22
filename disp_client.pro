#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T14:46:40
#
#-------------------------------------------------

QT       += core gui network sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = disp_client
TEMPLATE = app

INCLUDEPATH += . ../libmaia/
LIBS += ../libmaia/libmaia.a

SOURCES += main.cpp\
        dispwindow.cpp \
    client.cpp \
    gpsdata.cpp \
    mypaintlayer.cpp \
    trer.cpp

HEADERS  += dispwindow.h \
    client.h \
    gpsdata.h \
    mypaintlayer.h \
    trer.h
LIBS     += -lmarblewidget -lQtGui -lQtCore
FORMS    += dispwindow.ui
