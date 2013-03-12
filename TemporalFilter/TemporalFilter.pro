#-------------------------------------------------
#
# Project created by QtCreator 2013-01-01T01:17:01
#
#-------------------------------------------------

QT       -= core gui

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TemporalFilter
TEMPLATE = app

INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
LIBS += -L/usr/local/lib `pkg-config --libs opencv`


SOURCES += main.cpp \
    TemporalFilter.cpp


HEADERS += \
    TemporalFilter.hpp

