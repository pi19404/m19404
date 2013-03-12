TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    t1.cpp \
    feature_detector.cpp \
    harris3d.cpp \
    fast.cpp

LIBS +=  `pkg-config --libs opencv` -lrt -lm
INCLUDEPATH += -I/media/UBUNTU/softwares-linux/OpenCV-2.4.3//include/opencv -I/media/UBUNTU/softwares-linux/OpenCV-2.4.3//include
#/usr/local/include /usr/local/include/opencv /usr/local/include/opencv2

HEADERS += \
    feature_detector.hpp \
    harris_corner.hpp \
    good_features_to_track.hpp \
    harris3d.h \
    timeMeasure.hpp \
    fast.h \
    fast3d.hpp \
    OpticalFlow1.h
