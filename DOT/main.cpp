#include <QtGui/QApplication>
#include "mainwindow.h"
#include "DotTemplate.hpp"


int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    Mat src;
   VideoCapture inputVideo;

    inputVideo.open (0);

    //check if video file is open
    if (!inputVideo.isOpened())
    {
        cout  << "Could not open the input video: "  << endl;
        return -1;
    }
    //get the first frame of the video file
    inputVideo >> src;
    //cv::flip (src,src,1);
    Mat a1;
    DOT xx;
    do
    {
          inputVideo >> src;
          if (src.empty()) break;         // exit the loop if all files have been read
          cv::flip (src,src,2);
          cv::resize (src,a1,Size(320,240),0,0,cv::INTER_AREA);
          imshow("image",a1);
          xx.process (a1);
         cv::waitKey (1);
    }while(true);
    //return a.exec();
}
