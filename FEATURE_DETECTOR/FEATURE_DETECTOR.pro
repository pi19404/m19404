TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    t1.cpp \
    feature_detector.cpp \
    harris3d.cpp

LIBS += -L/usr/local/lib `pkg-config --libs opencv` -lrt -lm
INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2

HEADERS += \
    feature_detector.hpp \
    harris_corner.hpp \
    good_features_to_track.hpp \
    harris3d.h \
    timeMeasure.hpp
