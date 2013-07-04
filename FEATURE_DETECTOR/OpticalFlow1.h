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
        _MinDist=1;
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
        if(corners[_prev_index].size()<=_MinPoints)
            return;


        corners[_cur_index].resize(corners[_prev_index].size());
        cv::calcOpticalFlowPyrLK (_gray[_prev_index],_gray[_cur_index],
                                  corners[_prev_index],corners[_cur_index],status,error,winSize,pyrLevel,term,flags);

        flags=CV_LKFLOW_PYR_A_READY;




        _tracked_count=corners[_cur_index].size();

    }


    static float maximum(float x, float y, float z) {
        int max = x; /* assume x is the largest */

        if (y > max) { /* if y is larger than max, assign y to max */
            max = y;
        } /* end if */

        if (z > max) { /* if z is larger than max, assign z to max */
            max = z;
        } /* end if */

        return max; /* max is the largest value */
    }


    bool start;
    Mat flow,cflow;
    Mat run(Mat image,vector<float> descriptors)
    {


         //Ptr<DenseOpticalFlow> tvl1 = createOptFlow_DualTVL1();
        image.copyTo (_image[_cur_index]);
        cvtColor(image,_gray[_cur_index],CV_BGR2GRAY);



        //cerr << _cur_index << ":"  << _prev_index << ":" <<endl;
         Mat aa=image;
        if(_cur_index==1||start==true)
        {
                        start=true;
                        //calcOpticalFlowSF(_gray[_prev_index],_gray[_cur_index],flow,1, 2, 4);// 4.1, 25.5, 18, 55.0, 25.5, 0.35, 18, 55.0,           25.5, 10);

    //tvl1->calc(_gray[_prev_index],_gray[_cur_index], flow);
                        calcOpticalFlowFarneback(_gray[_prev_index],_gray[_cur_index], flow,0.5,3, 15, 5, 5, 1.2, 0);
                        //cvtColor(_gray[_prev_index], cflow, CV_GRAY2BGR);                                                
                        //aa=drawOptFlowMap(flow, image,1, 1.5, CV_RGB(0, 255, 0));

        //                imshow("XX",aa);

        }


        _prev_index = _cur_index;
        _cur_index = (_cur_index+1)%2;

        return aa;

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

            subpix.RefineCorner (detector.ret_current_frame (),corners[_cur_index]);
            if(_valid_count>=minFeatures)
            {
                    ///plot features compute histogram of Optical Flow
                for(int k=0;k<_valid_count;k++)
                {
                    cv::line (image,corners[_prev_index][k],corners[_cur_index][k],Scalar(0,0,255),1);
                    cv::circle (image,corners[_prev_index][k],3,color,-1);
                }

            }

            corners[_cur_index].resize(_valid_count);
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
