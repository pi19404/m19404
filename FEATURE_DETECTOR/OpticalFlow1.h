#ifndef OPTICALFLOW1_H
#define OPTICALFLOW1_H

#include "FeatureDetector.hpp"
#include "Fast3D.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/video/video.hpp"
#include "SubPixelCorner.hpp"
class opticalflow1
{
public:
    vector<Mat> _image;          //vector to hold the image
    vector<Mat> _gray;            //vector to hold gray scale image

    vector< vector<Point2f> > corners;  //vector to hold the corners in previous and current frame
    vector<uchar> status;       //contains the status of detected corner points
    vector<float> error;           //contains error information of detected corner points

    FeatureDetection::Fast3D detector;  //fast3d detector for feature detection

    Size winSize;
    int pyrLevel;
    cv::TermCriteria term;
    int flags;

    float _MaxErr;
    int _MinDist;
    int _MinPoints;     //minimum number of points required to initiate tracking
    int _MaxPoints;    //maximum number of points to be tracked
    int _count;             //current count of points to be tracked
    int _tracked_count;
    int _valid_count;
    double max_fe;      //minimum percentage of maximum points to be tracked
    int minFeatures;


    bool need_to_init;
    bool motion;
    bool prev_init;

    int _cur_index;

    int _prev_index;

    int counta;
    int minFailCount;
    bool status1;
    Scalar color;
    opticalflow1()
    {
        int size=2;     //history
        _image.resize (size);    //allocating space for color image
        _gray.resize (size);       //allocating space for grayscale images
        corners.resize (size);

        need_to_init=true;   //flag to indicate if feature initialization needs to be performed
        motion=false;           //flag to indicate if motion has occured
        prev_init=false;        //flag to indicate the previous initialization sstate
        _MaxPoints=30;     //maximum number of feature to be processed
        _count=0;
        _MinPoints=10;
        _MinDist=3;
        _MaxErr=550;

        detector.setMaxCorners (_MaxPoints);
        status.resize (_MaxPoints);
        error.resize (_MaxPoints);
        corners[0].resize(_MaxPoints);
        corners[1].resize(_MaxPoints);

        winSize=Size(15,15);
        pyrLevel=3;
        term=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.1);

        max_fe=0.3;
        minFeatures=0.1*_MaxPoints;
        _MinPoints=minFeatures;
        _cur_index=0;
        _prev_index=0;
        minFailCount=5;
        counta=0;
        status1=false;
        start=false;
        _valid_count=0;

    }

    bool acceptTrackedPoint(int i)
    {
        if(status[i]==0)
            return false;

        if( (corners[_cur_index][i].x >_gray[_cur_index].cols )||(corners[_cur_index][i].y >_gray[_cur_index].rows)||
                (corners[_cur_index][i].x <0)||(corners[_cur_index][i].y <0))
        return false;

        if(error[i]>_MaxErr)
            return false;
        Point2f p=corners[_prev_index][i];
        Point2f q=corners[_cur_index][i];
        double normd=sqrt((p.x-q.x)*(p.x-q.x)+(p.y-q.y)*(p.y-q.y));
        if(normd < _MinDist)
        {
            return false;
        }


        return true;
        //Point2f p=
    }

    void UpdatePoints(int i)
    {
        corners[_cur_index][_valid_count]=corners[_cur_index][i];
        corners[_prev_index][_valid_count]=corners[_prev_index][i];
        _valid_count=_valid_count+1;
    }

    void check_result()
    {
        if(prev_init==false)
        {
            if(status1==false)
            {
                need_to_init=true;
                ///detector.reset ();
                counta=0;
            }
        }
        if((_valid_count < minFeatures ) && _valid_count >0 && minFeatures >0)
        {
            counta=counta+1;
            if(counta>minFailCount)
            {
            need_to_init=true;
            status1=false;
            //detector.reset ();
            counta=0;
            }
        }
        else if((_valid_count >= minFeatures ) && _valid_count >0 && minFeatures >0)
        {
            need_to_init=false;
            status1=true;
            counta=0;
        }
        else if(_valid_count<=0)
        {
            counta=counta+1;
            if(counta>minFailCount)
            {
            need_to_init=true;
            status1=false;
            //detector.reset ();
            counta=0;
            }

        }

    }


    FeatureDetection::SubPixelCorner subpix;
    //track features using KLT tracker
    void track_features()
    {
        _valid_count=0;
        _tracked_count=0;
        if(_count<=_MinPoints)
            return;


        corners[_cur_index].resize(_count);
        cv::calcOpticalFlowPyrLK (_gray[_prev_index],_gray[_cur_index],
                                  corners[_prev_index],corners[_cur_index],status,error,winSize,pyrLevel,term,flags);

        flags=CV_LKFLOW_PYR_A_READY;




        _tracked_count=corners[_cur_index].size();

    }

    static void drawOptFlowMap( const Mat& flow, Mat& cflowmap, int step,
                       double, const Scalar& color)
   {

        Mat yy;

        cvtColor(cflowmap,yy,CV_BGR2GRAY);
        yy.convertTo(yy,CV_32FC(1),1.0/255.0,0);
       for(int y = 0; y < cflowmap.rows; y = y+1)
       {
           float * ptr=yy.ptr<float> (y);

           for(int x = 0; x < cflowmap.cols; x = x+1)
           {
               //cerr << x <<":" << y <<endl;
               const Point2f& fxy = flow.at<Point2f>(y, x);
              ptr[1*x]=fxy.x*fxy.x+fxy.y*fxy.y;//-fxy.x*fxy.y;//fxy.y*fxy.y
              //ptr[3*x+1]=fxy.x+fxy.y;
              //ptr[3*x+2]=fxy.x+fxy.y;
               //line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), color);
               //circle(cflowmap, Point(x,y), 2, color, -1);
           }
       }

       cv::normalize(yy,yy,0,255,CV_MINMAX);
       cvtColor(yy,yy,CV_GRAY2BGR);
       yy.convertTo (cflowmap,CV_8UC(3),1.0,0);

   }

    bool start;
    Mat flow,cflow;
    Mat run(Mat image)
    {


        image.copyTo (_image[_cur_index]);
        cvtColor(image,_gray[_cur_index],CV_BGR2GRAY);


/*
        //cerr << _cur_index << ":"  << _prev_index << ":" <<endl;
        if(_cur_index==1||start==true)
        {
                        start=true;
                        calcOpticalFlowFarneback(_gray[_prev_index],_gray[_cur_index], flow,0.5,3, 15, 5, 5, 1.2, 0);
                        //cvtColor(_gray[_prev_index], cflow, CV_GRAY2BGR);
                        drawOptFlowMap(flow, image,5, 1.5, CV_RGB(0, 255, 0));
                        imshow("flow", image);

        }

        _prev_index = _cur_index;
        _cur_index = (_cur_index+1)%2;

        return image;
        */
        //if(1==1)
        {
        prev_init=need_to_init;

        //cerr << "nedd to init " << need_to_init << endl;
        //check if features needs to be initialized again
        vector <Point2f> curr_corners=detector.run (_gray[_cur_index]);;
        for(int k=0;k<curr_corners.size ();k++)
        {
        //    cv::line (image,corners[_prev_index][k],corners[_cur_index][k],Scalar(0,0,255),1);
            cv::circle (image,curr_corners[k],3,Scalar(0,255,0),-1);
        }


        if(need_to_init==true||motion==true)
        {

            color=Scalar(0,255,255);
            //_count=curr_corners.size ();
            int k=0;
            corners[_cur_index].resize(detector.get_count ());
            for(k=0;k<detector.get_count ();k++)
                corners[_cur_index][k]=curr_corners[k];
            //corners[_cur_index]=curr_corners;

            //obtain the count of detected features
            _count=k;
            //cerr << "Detecting " << _count <<":" << detector.status () << endl;
            //check if detector has processed the frames

            if(detector.status ()==true && _count >_MinPoints)
                need_to_init=false;

        }
        //if features have been initialized track the features
        else if(_count >_MinPoints && need_to_init==false)
        {
            color=Scalar(255,0,0);

            //track features
            track_features ();


            cerr << "Tracking " << _tracked_count<< endl;
            for(int i=0;i<_tracked_count;i++)
            {
                    if(acceptTrackedPoint (i))
                        UpdatePoints (i);

            }

            //


            if(prev_init==true)
            minFeatures=(int)(3*max_fe *_MaxPoints);
            else if(prev_init==false && _tracked_count >= max_fe/2*_MaxPoints)
            minFeatures=(int)(1.5*max_fe*_tracked_count);
            else if(prev_init==false && _tracked_count < max_fe/2*_MaxPoints)
            minFeatures=_MaxPoints;

            corners[_cur_index].resize(_valid_count);
            subpix.RefineCorner (_gray[_cur_index],corners[_cur_index]);
            if(_valid_count>=minFeatures)
            {
                    ///plot features compute histogram of Optical Flow
                for(int k=0;k<_valid_count;k++)
                {
                    cv::line (image,corners[_prev_index][k],corners[_cur_index][k],Scalar(0,0,255),1);
                    cv::circle (image,corners[_prev_index][k],3,color,-1);
                }

            }

        need_to_init=true;
        }


        check_result();
         //return image;
        }
        _prev_index = _cur_index;
        _cur_index = (_cur_index+1)%2;

        return image;


    }


};


#endif // OPTICALFLOW1_H
