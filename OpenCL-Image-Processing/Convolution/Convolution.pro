TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp


INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2 ../processing_library /opt/AMDAPP/include/CL
LIBS += -L/opt/AMDAPP/lib/x86_64 -lOpenCL -L/usr/local/lib `pkg-config --libs opencv` -lrt -lm


HEADERS += \
    ocl_common.h

OTHER_FILES += \
    conv1.cl \
    ../../../../../../tmp/header


PRECOMPILED_HEADER = /tmp/header
