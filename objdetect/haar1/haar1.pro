#-------------------------------------------------
#
# Project created by QtCreator 2013-07-16T21:09:27
#
#-------------------------------------------------

QT       += core gui

INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2 /usr/include/libxml2
# /media/UBUNTU/softwares-linux/FaceDetect_V1.3
LIBS += -L/usr/local/lib `pkg-config --libs opencv` -lmlpack


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = haar1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
