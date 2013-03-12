TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    histogram.cpp \
    ContrastStretching.cpp

HEADERS += \
    ContrastStretching.hpp \
    common.hpp \
    Noise.hpp \
    histogram.hpp

LIBS += -L/usr/local/lib `pkg-config --libs opencv` -lrt -lm
INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
