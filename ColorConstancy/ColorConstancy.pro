#-------------------------------------------------
#
# Project created by QtCreator 2012-12-30T18:57:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorConstancy
TEMPLATE = app

INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
LIBS += -L/usr/local/lib `pkg-config --libs opencv` -lrt -lm

SOURCES += main.cpp\
        mainwindow.cpp \
    color_constancy.cpp

HEADERS  += mainwindow.h \
    color_constancy.hpp

FORMS    += mainwindow.ui
