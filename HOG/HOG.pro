
TEMPLATE = app
CONFIG += console
#CONFIG -= qt

QT       += core gui

INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
LIBS += -L/usr/local/lib `pkg-config --libs opencv` -lrt -lm


SOURCES += \
    HOG.cpp

