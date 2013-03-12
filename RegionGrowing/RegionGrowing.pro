TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    RegionGrowing.cpp

HEADERS += \
    RegionGrowing.hpp \
    timeMeasure.hpp

LIBS += -L/usr/local/lib `pkg-config --libs opencv` -lrt -lm
INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
