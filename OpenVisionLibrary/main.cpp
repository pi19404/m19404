#include <iostream>

#include "ImgProc/Histogram.hpp"
using namespace std;
#include "ImgProc/meanshift.h"
Histogram h1;
Histogram h2;

int test_histogram(int argc, char** argv )
    {
      Mat i1,i2,i3,i4;

      /// Load three images with different environment settings
      if( argc < 3 )
        { printf("** Error. Usage: ./compareHist_Demo <image_settings0> <image_setting1> <image_settings2>\n");
          return -1;
        }

      //cerr<< argv[1] <<":" << argv[2] << endl;
      i1 = imread( argv[1], 1 );
      i2 = imread( argv[2], 1 );
      //cerr << i2.channels() << endl;
      //i3 = imread( argv[3], 1 );
      //i4 = imread( argv[4], 1 );

      cvtColor( i1,i3, CV_BGR2HSV );
      cvtColor( i2,i4, CV_BGR2HSV );
      //cvtColor( i3,i3, CV_BGR2HSV );
      //cvtColor( i4,i4, CV_BGR2HSV );

      Histogram h1,h2,h3,h4;
      vector<int> channel;
      channel.push_back(0);
      //channel.push_back(1);
      //channel.push_back(2);
      h1.setChannel(channel);
      h2.setChannel(channel);
      //h3.setChannel(channel);
      //h4.setChannel(channel);
      vector<int> size;
      size.push_back(50);
      size.push_back(60);
      size.push_back(40);
      h1.setHistSize(size);
      h2.setHistSize(size);

      h1.BuildHistogram(i3);
      h2.BuildHistogram(i4);
      //h3.BuildHistogram(i3);
      //h4.BuildHistogram(i4);
      Mat h11=h1.drawHist();
      imshow("Hist 1",h11);
      h11=h2.drawHist();
      imshow("Hist 2",h11);

       cv::waitKey(0);


      double r1=h1.compareHist(h2,Histogram::comparison_method::BHATTACHRYA);
      double r2=h1.compareHist(h2,Histogram::comparison_method::CHI_SQUARED);
      double r3=h1.compareHist(h2,Histogram::comparison_method::INTERSECTION);
      double r4=h1.compareHist(h2,Histogram::comparison_method::CORRELATION);

      cerr << r1 << ":" << r2 << ":" << r3 <<":" << r4 << endl;

      r1=cv::compareHist(h1.getHist(),h2.getHist(),CV_COMP_BHATTACHARYYA);
      r2=cv::compareHist(h1.getHist(),h2.getHist(),CV_COMP_CHISQR);
      r3=cv::compareHist(h1.getHist(),h2.getHist(),CV_COMP_INTERSECT);
      r4=cv::compareHist(h1.getHist(),h2.getHist(),CV_COMP_CORREL);

      cerr << r1 << ":" << r2 << ":" << r3 <<":" << r4 << endl;
       cv::waitKey(0);

}

cv::Rect* sArea;
bool areaDefined = false;
Mat img;
static bool pressed=false;
bool model=false;
void mouseEventHandler(int event, int x, int y,int flags, void *params) {
    static int x1,y1;


    if(pressed) {
        //rectangle(*(Mat*)params,Point(x1,y1),Point(x,y),Scalar(255,0,0) );
        sArea = new Rect(x1,y1,x-x1,y-y1);
    }
    if(event == CV_EVENT_LBUTTONDOWN) {
        x1 = x;
        y1 = y;
        pressed = true;
        areaDefined=false;
        model=false;
    }
    if(event == CV_EVENT_LBUTTONUP) {
        sArea = new Rect(x1,y1,x-x1,y-y1);

        pressed = false;
        areaDefined = true;
        model=false;
        //cerr << x1 <<":" << y1 <<":" << sArea->width <<":" << sArea->height << endl;
        sArea->x=80;
        sArea->y=88;
        sArea->width=56;
        sArea->height=82;
        rectangle(*(Mat*)params,*sArea,Scalar(0,255,0) );

    }
}



void callbackButton2(int state,void* userdata)
{
    vector<int> channel;
    channel.push_back(0);
    h1.setChannel(channel);
    vector<int> size,size1;
    size.push_back(30);
    size.push_back(60);
    size.push_back(40);
    h1.setHistSize(size);

    size1.push_back(180);
    size1.push_back(60);
    size1.push_back(40);

    h2.setChannel(channel);
    h2.setHistSize(size1);

    Mat roi=img(*sArea);
    cvtColor(roi,roi,CV_BGR2HSV);
    h1.BuildHistogram(roi);
    model=true;
    Mat i3=h1.drawHist();
    imshow("histogram 1",i3);

    h2.BuildHistogram(roi);
    Mat i2=h2.drawHist();
    imshow("histogram",i2);



    Mat hh=h2.applyKernel(6,2);
    Mat draw=h2.drawHist(hh);
    imshow("smooth 1",draw);

    hh=h2.applyKernel(12,2);
    draw=h2.drawHist(hh);
    imshow("smooth 2",draw);


    hh=h2.applyKernel(18,2);
    draw=h2.drawHist(hh);
    imshow("smooth 3",draw);
}

int test_meanshift()
{

        /*VideoCapture cam;
        cam = *new VideoCapture(0);
        if( !cam.isOpened() ) {
            cout << "Failed to open camera" << endl;
            return 0;
        }*/

        Mat img1=imread("/media/software/Dropbox/Dropbox/repository/im/documents/images/image020.jpg",1);

        Mat hsvImg;
        MatND hist1;
        char key = 10;
        //cam >> img;
         img1.copyTo(img);
        namedWindow("tracker",0);
        cvSetMouseCallback("tracker",mouseEventHandler,&img);
        cvCreateButton("Build Model",callbackButton2,0, CV_PUSH_BUTTON, 0);
        char text[100];
        sprintf(text,"similarity is %f",0);
        while(key != 27) {
            //cam >> img;
            img1.copyTo(img);
            //cv::flip(img,img,1);
            if(pressed==true || areaDefined==true)
            {
                rectangle(img,*sArea,Scalar(0,255,0),3);
            }
            if(model==true)
            {
                Mat roi=img(*sArea);
                cvtColor(roi,roi,CV_BGR2HSV);
                Mat a1=h1.likeyhoodImage(roi);
                h2.BuildHistogram(roi);
                //double val=h1.compareHist(h2,Histogram::comparison_method::BHATTACHRYA1);
                double val=0;
                cvtColor(a1,roi,CV_GRAY2BGR);

                int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
                double fontScale = 1;
                int thickness = 3;

                sprintf(text,"similarity is %f",val);

                //putText(img, text,Point(20,20), fontFace, fontScale,                        Scalar(0,255,255), thickness, 8);

            }
            imshow("tracker",img);
            key = cvWaitKey(10);

        }
}

int main(int argc,char **argv)
{
    cout << "Hello World!" << endl;
//    test_histogram(argc,argv);
    test_meanshift();
    return 0;
}


