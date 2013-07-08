#include <QtGui/QApplication>
#include "mainwindow.h"
#include "string"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(0,argc,argv);
    w.show();





    return a.exec();
}
