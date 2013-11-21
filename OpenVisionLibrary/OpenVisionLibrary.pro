TEMPLATE = lib
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
#    ImgProc/InterPolation.cpp \
#    ImgProc/Convolution.cpp \
#    ImgProc/ContrastStretching.cpp \
    ImgProc/Histogram.cpp \
    ImgProc/meanshift.cpp \
    ImgProc/test_meanshift.cpp

HEADERS += \
#    ImgProc/InterPolation.hpp \
#    ImgProc/Convolution.hpp \
#    Common/OpenCVCommon.hpp \
#    ImgProc/Noise.hpp \
#    ImgProc/ContrastStretching.hpp \
    ImgProc/Histogram.hpp \
    ImgProc/meanshift.h \
    Common/OpenCVCommon.hpp

LIBS += -L/usr/local/lib `pkg-config --libs opencv` -lrt -lm
INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
