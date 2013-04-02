TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
    AndroidOpenCVProcessImage.hpp

LIBS +=  `pkg-config --libs opencv` -lrt -lm
INCLUDEPATH += -I/media/UBUNTU/softwares-linux/OpenCV-2.4.3//include/opencv -I/media/UBUNTU/softwares-linux/OpenCV-2.4.3//include
