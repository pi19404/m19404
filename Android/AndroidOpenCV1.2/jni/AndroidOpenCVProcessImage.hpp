#ifndef ANDROIDPROCESSIMAGE_HPP
#define ANDROIDPROCESSIMAGE_HPP
#include <string>
#include <sstream>
#include <iostream>
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv/cv.h"
#include <stdlib.h>
#include <vector>
#include <stack>
#include <iostream>
#include <map>
#include <ctime>


using namespace std;
using namespace cv;


namespace OpenCV
{

class meanshift
{


    Mat hsv;
    Rect selection;
    Mat hist;
    //histogram parameters
    int hsize;
    int ch[4];
    float hranges[2],sranges[2];
    float**phranges;

    void buildHistogram()
    {

        Mat hue;
        hue.create(hsv.size(), hsv.depth());
        mixChannels(&hsv, 1, &hue, 1, ch, 1);
        Mat roi(hue, selection);
        calcHist(&roi, 1, 0, Mat(), hist, 1, &hsize,(const float **) phranges);
        normalize(hist, hist, 0, 255, CV_MINMAX);
        trackObject=true;
    }

public:
    bool trackObject;


public:
    meanshift()
    {
        trackObject=false;
        ch[0] = 0;
        ch[1]=0;
        ch[2] = 1;
        ch[3]=1;

        hranges[0] =0;
        hranges[1]=256;
        sranges[0] =0;
        sranges[1]=256;

        phranges = (float **)malloc(2*sizeof(float[2]));
        phranges[0]=&hranges[0];
        phranges[1]=&sranges[0];
        hsize=16;
    }
private:
    Mat calcProbability()
    {
        Mat hue,backproj;
        hue.create(hsv.size(), hsv.depth());
        mixChannels(&hsv, 1, &hue, 1, ch, 1);
        calcBackProject(&hue, 1, 0, hist, backproj, (const float**)phranges);
        return backproj;
    }
public:
    int track(Mat &image,Rect &rect)
    {
        int status=0;
        cvtColor(image, hsv, CV_BGR2HSV_FULL);
        selection=rect;
        if(rect.width!=0&&rect.height!=0)
        {
            if(trackObject==false)
                buildHistogram ();
            if(trackObject==true)
            {
                Rect trackWindow=selection;
                Mat backproj=calcProbability ();
                RotatedRect trackBox = CamShift(backproj,trackWindow,TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
                if( trackWindow.area() <= 1 )
                {
                    int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
                    trackWindow = Rect(0,0,0,0);
                    trackObject=false;
                }
                else
                {
                    Point2f rect_points[4]; trackBox.points( rect_points );
                          for( int j = 0; j < 4; j++ )
                              line( image, rect_points[j], rect_points[(j+1)%4], Scalar(0,0,255), 1, 8 );

                }

            }
        }
        else
        {
            trackObject=false;
        }

        status=1;
        return status;
    }




};


/**
     * @brief The haarcascade class is higher level interface for OpenCV functions which perform haar cascade detection
     *of object.
     */
class haarcascade
{

private:
    String face_cascade_name ;
    String eyes_cascade_name ;
    CascadeClassifier face_cascade;


    //constructor for class which loads the cascade files
public:
    haarcascade()
    {

    }

    haarcascade(char * name)
    {
        face_cascade_name=name;
        //-- 1. Load the cascades
        if( !face_cascade.load( face_cascade_name ) )
        {
            printf("--(!)Error loading\n");
        };

    }

    //method to detect faces in the image frame and return rectangular ROI if found
    Rect detect(Mat &frame)
    {
        int flag=0;
        Mat f1(240/2,320/2,frame.type ());
        cv::flip(frame,frame,1);
        cv::resize(frame,f1,f1.size(),0,0,INTER_CUBIC);
        //cv::flip(f1,f1,1);

        //ROI of face locations
        std::vector<Rect> faces;
        Mat frame_gray;

        //converting to grayscale
        cvtColor( f1, frame_gray, CV_BGR2GRAY );
        //pre processing frame using histogram equalization
        equalizeHist( frame_gray, frame_gray );
        //-- multiscale detection of faces
        face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        float maxarea=0;
        Rect rect;
        for( int i = 0; i < faces.size(); i++ )
        {
            Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
            int scalex=frame.cols/f1.cols;
            int scaley=frame.rows/f1.rows;
            Point ncenter;
            ncenter.x=center.x*scalex;
            ncenter.y=center.y*scaley;
            Point corner;
            corner.x=faces[i].x;
            corner.y=faces[i].y;
            center.x=corner.x*scalex;
            center.y=corner.y*scaley;
            if(faces[i].width*faces[i].height>maxarea)
            {
                maxarea=faces[i].width*faces[i].height;
                //scale the center and dimension to original image scale
                rect=Rect(center.x,center.y,faces[i].width*scalex,faces[i].height*scaley);
                //rect=Rect(faces[i].x,faces[i].y,faces[i].width,faces[i].height);
            }
        }
        return rect;
    }

};


class OpenCVProcess{
private:
    haarcascade face;
    meanshift tracker;
    Rect roi;
public:


    OpenCVProcess() {}

    int run(int width, int height, signed char *_yuv, int* _bgra)
    {

        //decoding the camera data to BGRA Mat image
        Mat myuv(height + height/2, width, CV_8UC1, (unsigned char *)_yuv);
        Mat mbgra(height, width, CV_8UC4, (unsigned char *)_bgra);
        cvtColor(myuv, mbgra, CV_YUV420sp2BGR, 4);
        Mat image;
        cvtColor(mbgra,image,CV_BGRA2BGR);
        int flag=run(image);
        cvtColor(image,mbgra,CV_BGR2BGRA);
        return flag;
    }

    int run(Mat bgr)
    {

        //if tracking status false launch localization routine
        if(tracker.trackObject==false)
        {
        //face localization using haar cascade detetor
        roi=face.detect (bgr);
        }
        //face tracking using mean shift tracker
        int flag=tracker.track (bgr,roi);
        //returning the status flag
        return flag;
    }

    //method to initialize the haar cascade detector
    void initDetector(const char* name)
    {
        face=haarcascade((char *)name);
    }

    //method to reinitialize the tracing
    void initTracker()
    {
        tracker.trackObject=false;
    }

};

}

#endif // ANDROIDPROCESSIMAGE_HPP
