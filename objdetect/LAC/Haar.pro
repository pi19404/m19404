#-------------------------------------------------
#
# Project created by QtCreator 2012-09-27T11:49:43
#
#-------------------------------------------------

QT       += core gui

TARGET = Haar
TEMPLATE = app

QMAKE_CXXFLAGS  += -g

#INCLUDEPATH += J:\SEMX\opencv\opencv\build\include  J:\SEMX\opencv\opencv\build\include\opencv J:\SEMX\opencv\opencv\build\include\opencv2 I:\GOOGLE\FaceDetect_V1.3
#LIBS += -LJ:\SEMX\opencv\opencv\build\gpu\x86\lib -LJ:\SEMX\opencv\opencv\build\gpu\x86\bin  -lopencv_core231d  -lopencv_highgui231d -lopencv_imgproc231d
INCLUDEPATH += /usr/local/include /usr/local/include/opencv /usr/local/include/opencv2
# /media/UBUNTU/softwares-linux/FaceDetect_V1.3
LIBS += -L/usr/local/lib `pkg-config --libs opencv`
DEFINES =PROJECT_PREFIX Training PROJECT_NAME a123
SOURCES += main.cpp\
        mainwindow.cpp \
    facedetectdoc.cpp \
    Learn.cpp \
    IntImage.cpp \
    CascadeClassifier.cpp \
    Global.cpp \
    SimpleClassifier.cpp \
    AdaBoostClassifier.cpp \
    FFS.cpp \
    ObjWidget.cpp \
    QtOpenCV.cpp \
    Settings.cpp \
    camera.cpp

HEADERS  += mainwindow.h \
    common.h \
    facedetectdoc.h \
    support.h \
    cstring1.h \
    ObjWidget.h \
    QtOpenCV.h \
    Settings.h \
    camera.h \
    SimpleClassifier.h \
    Learn.h \
    IntImage.h \
    Global.h \
    CascadeClassifier.h \
    AdaBoostClassifier.h \
    FFS.H \
    HaarFeature.hpp

FORMS    += mainwindow.ui
