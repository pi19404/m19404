#-------------------------------------------------
#
# Project created by QtCreator 2013-08-16T02:15:38
#
#-------------------------------------------------

QT       += core gui


LIBS +=   -lrt -lm  -lrt -lm -L/media/UBUNTU/softwares-linux/Qt5/5.0.1/gcc_64/lib  `pkg-config --libs opencv`

INCLUDEPATH += /usr/local/include/opencv  /usr/local/include/opencv2 /media/UBUNTU/repository/m19404/FEATURE_DETECTOR


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BugAlgorithm
TEMPLATE = app


SOURCES += main.cpp \
    bac.cpp \
    Simulator.cpp \
    Sensor.cpp \
    Robot.cpp \
    Bug2.cpp

HEADERS  += \
    Point.hpp \
    Obstacle.hpp \
    common.hpp \
    DebugText.hpp \
    Goal.hpp \
    MotionState.hpp \
    Simulator.hpp \
    Sensor.hpp \
    Robot.hpp \
    Bug2.hpp

#FORMS    += mainwindow.ui
