#-------------------------------------------------
#
# Project created by QtCreator 2013-08-02T00:57:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = textlinedetector
TEMPLATE = app

LIBS +=   -lrt -lm -L/media/UBUNTU/softwares-linux/Qt5/5.0.1/gcc_64/lib -L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_ts -lopencv_video -lopencv_videostab

INCLUDEPATH += /usr/local/include/opencv  /usr/local/include/opencv2 /media/UBUNTU/repository/m19404/FEATURE_DETECTOR


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    trg.hpp \
    a.hpp

FORMS    += mainwindow.ui
