#include <QtGui/QApplication>
#include "mainwindow.h"
#include "DotTemplate.hpp"



void my_mouse_callback( int event, int x, int y, int flags, void* param );


Rect box;
bool drawing_box = false;
bool learn=false;

void callbackButton2(int state, void *pointer){

    learn=true;

}

// Implement mouse callback
void my_mouse_callback( int event, int x, int y, int flags, void* param ){
  //  IplImage* image = (IplImage*) param;

    switch( event ){
        case CV_EVENT_MOUSEMOVE:
            if( drawing_box ){
                box.width = x-box.x;
                box.height = y-box.y;
            }
            break;

        case CV_EVENT_LBUTTONDOWN:
            drawing_box = true;
            box = Rect( x, y, 0, 0 );
            break;

        case CV_EVENT_LBUTTONUP:
            drawing_box = false;
            if( box.width < 0 ){
                box.x += box.width;
                box.width *= -1;
            }
            if( box.height < 0 ){
                box.y += box.height;
                box.height *= -1;
            }

//            draw_box( image, box );
            break;
    }
}

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
    cv::namedWindow ("image");
    cv::setMouseCallback ("image",my_mouse_callback,0);
    //cv::createButton("Clear  Templates",callbackButton1,NULL,CV_PUSH_BUTTON,0);
    cv::createButton("Add Template",callbackButton2,NULL,CV_PUSH_BUTTON,0);

    //char* nameb1 = "button1";
    //char* nameb2 = "button2";
    //cvCreateButton(nameb1,callbackButton,nameb1,CV_CHECKBOX,1);
    //cvCreateButton(nameb2,callbackButton,nameb2,CV_CHECKBOX,0);
    //cvCreateButton("button5",callbackButton1,NULL,CV_RADIOBOX,0);


    do
    {
          inputVideo >> src;
          if (src.empty()) break;         // exit the loop if all files have been read
          cv::flip (src,src,2);
          cv::resize (src,a1,Size(320,240),0,0,cv::INTER_AREA);          
          Mat res=xx.process (a1,box,learn);
          //cv::cvtColor(res,res,CV_GRAY2BGR);
          cv::rectangle (res,box,Scalar(0,255,255),3);
          imshow("image",res);
         cv::waitKey (1);
    }while(true);
    //return a.exec();
}
