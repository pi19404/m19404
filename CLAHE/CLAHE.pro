#-------------------------------------------------
#
# Project created by QtCreator 2013-01-06T01:00:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CLAHE
TEMPLATE = app


INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
LIBS += -L/usr/local/lib `pkg-config --libs opencv`


SOURCES += main.cpp\
        mainwindow.cpp \
    HistogramEqualization.cpp \
    ../ColorConstancy/color_constancy.cpp \
    ../FusionEnhancement/laplacianBlend.cpp \
    main1.cpp


HEADERS  += mainwindow.h \
    HistogramEqualization.hpp


