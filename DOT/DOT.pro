#-------------------------------------------------
#
# Project created by QtCreator 2013-04-30T13:35:33
#
#-------------------------------------------------

QT       += core gui

TARGET = DOT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    DotTemplate.hpp \
    t1.hpp

FORMS    += mainwindow.ui


LIBS +=   -lrt -lm -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab

INCLUDEPATH += /media/LENOVO/OPENCV/opencv/include/opencv /media/LENOVO/OPENCV/opencv/include/opencv2 /home/pi19404/Downloads/dot/include
