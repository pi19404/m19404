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
        //method to detect faces in the image fram
        int detect(Mat &frame)
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
               for( int i = 0; i < faces.size(); i++ )
               {
                   Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
                   int scalex=frame.cols/f1.cols;
                   int scaley=frame.rows/f1.rows;
                   Point ncenter;
                   ncenter.x=center.x*scalex;
                   ncenter.y=center.y*scaley;
                   ellipse( frame, ncenter, Size( faces[i].width*0.5*scalex, faces[i].height*0.5*scaley), , 0, 360, Scalar( 255, 0, 255 ), 1);
               }
               //cvtColor(frame_gray,f1,CV_GRAY2BGR);
               //cv::resize (f1,frame,frame.size(),0,0,INTER_CUBIC);
               flag=1;
               return flag;
        }

    };


    class OpenCVProcess{
    private:
        haarcascade face;
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
            int flag=face.detect (image);
            cvtColor(image,mbgra,CV_BGR2BGRA);
            return flag;
        }

        void initDetector(const char* name)
        {
            face=haarcascade((char *)name);
        }

    };

}

#endif // ANDROIDPROCESSIMAGE_HPP
