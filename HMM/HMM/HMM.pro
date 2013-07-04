#-------------------------------------------------
#
# Project created by QtCreator 2013-06-28T17:22:58
#
#-------------------------------------------------

#QT       += core gui

TARGET = HMM
#TEMPLATE = app


SOURCES += main.cpp\ 
    main1.cpp \
    main2.cpp

#     mainwindow.cpp \
#HEADERS  += mainwindow.h

#FORMS    += mainwindow.ui



INCLUDEPATH+=/media/windows/pi19404/PX/mlpack-1.0.6/include/ /usr/local/include/mlpack /usr/include/libxml2/

LIBS =-L/media/windows/pi19404/PX/mlpack-1.0.6/lib -L/usr/local/lib -lmlpack -larmadillo -lboost_program_options-mt -lboost_unit_test_framework-mt -lboost_random-mt -lboost_program_options-mt -lboost_unit_test_framework-mt -lxml2 -lboost_random-mt -lxml2

QMAKE_CXXFLAGS=-Wextra -g -DDEBUG=1

#-fPIC  -Wall -Wextra -g -O0 -pg   -shared -Wl,-soname,

