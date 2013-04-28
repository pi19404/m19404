TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    t1.cpp \
    GoodFeatureToTrack.cpp \
    main_new.cpp \
    SubPixelCorner.cpp \
    HarrisCorner.cpp \
    FeatureDetector.cpp \
    Fast.cpp \
    Fast3D.cpp \
    Harris3D.cpp

LIBS +=  `pkg-config --libs opencv` -lrt -lm
INCLUDEPATH += -I/media/UBUNTU/softwares-linux/OpenCV-2.4.3//include/opencv -I/media/UBUNTU/softwares-linux/OpenCV-2.4.3//include -I/media/UBUNTU/repository/m19404/FEATURE_DETECTOR
#/usr/local/include /usr/local/include/opencv /usr/local/include/opencv2

HEADERS += \
    timeMeasure.hpp \
    OpticalFlow1.h \
    GoodFeatureToTrack.hpp \
    FeatureDetector.hpp \
    SubPixelCorner.hpp \
    HarrisCorner.hpp \
    Fast.hpp \
    Fast3D.hpp \
    Harris3D.hpp
