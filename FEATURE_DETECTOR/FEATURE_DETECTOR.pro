TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    t1.cpp

LIBS += -L/usr/local/lib `pkg-config --libs opencv` -lrt -lm
INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
