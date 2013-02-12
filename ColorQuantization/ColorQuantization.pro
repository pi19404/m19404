TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2 ../processing_library /opt/AMDAPP/include/CL
LIBS +=  -L/usr/local/lib `pkg-config --libs opencv` -lrt -lm
