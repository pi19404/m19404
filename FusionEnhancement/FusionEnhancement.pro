#-------------------------------------------------
#
# Project created by QtCreator 2012-12-30T20:23:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FusionEnhancement
TEMPLATE = app

INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
LIBS += -L/usr/local/lib `pkg-config --libs opencv` -lrt -lm

SOURCES += main.cpp\
        mainwindow.cpp\
        ../ColorConstancy/color_constancy.cpp \
    laplacianBlend.cpp

HEADERS  += mainwindow.h \
    laplacianBlend.hpp

FORMS    += mainwindow.ui
