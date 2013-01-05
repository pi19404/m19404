//#include "opencv2/video/tracking.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "highgui.h"
//#include "cv.h"
//#include <iostream>

//using namespace cv;
//using namespace std;


//static const Vec3b bcolors[] =
//{
//    Vec3b(0,0,255),
//    Vec3b(0,128,255),
//    Vec3b(0,255,255),
//    Vec3b(0,255,0),
//    Vec3b(255,128,0),
//    Vec3b(255,255,0),
//    Vec3b(255,0,0),
//    Vec3b(255,0,255),
//    Vec3b(255,255,255)
//};

//static void help()
//{
//    cout <<
//            "\nThis program demonstrates dense optical flow algorithm by Gunnar Farneback\n"
//            "Mainly the function: calcOpticalFlowFarneback()\n"
//            "Call:\n"
//            "./fback\n"
//            "This reads from video camera 0\n" << endl;
//}
//static void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
//                    double, const Scalar& color)
//{
//    for(int y = 0; y < cflowmap.rows; y += step)
//        for(int x = 0; x < cflowmap.cols; x += step)
//        {
//            const Point2f& fxy = flow.at<Point2f>(y, x);
//            if(sqrt(fxy.x*fxy.x+fxy.y*fxy.y) >1)
//            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),color);
//            //circle(cflowmap, Point(x,y), 2, color, -1);
//        }

//}



//int main(int, char**)
//{


//       vector<vector<Point> > contours;

//       MSER mser = MSER(20,60,14400,0.05,0.5,200,1.01,0.003,5);

//    VideoCapture cap(0);
//    help();
//    if( !cap.isOpened() )
//        return -1;

//    Mat I1,I2;
//    Mat prevgray, gray, flow, cflow, frame;
//    namedWindow("flow", 1);
//    namedWindow("divergence", 1);
//    namedWindow("mser", 1);
//    Mat    a;
//    a.create(120,80,CV_8UC(1));
//    Mat b;
//    b.create(120,80,CV_32FC(1));
//    b.setTo(cv::Scalar::all(0));
//    for(;;)
//    {
//        cap >> frame;
//       resize(frame,a, a.size(), 0, 0, INTER_NEAREST);
//        cvtColor(a, gray, CV_BGR2GRAY);


//        if( prevgray.data )
//        {
////            cv::calcOpticalFlowSF(prevgray,gray,flow,1,10,10);
//            calcOpticalFlowFarneback(prevgray, gray,flow, 0.5,1,5,3, 5, 1.2, 0);
//            cerr << flow.depth() << ":" << flow.channels()<< ":" << CV_32FC1 << ":" << CV_8UC1 << endl;
//            cvtColor(prevgray, cflow, CV_GRAY2BGR);
//            vector<Mat> ch;
//            cv::split(flow,ch);
//            cv::absdiff(ch[0],b,ch[0]);
//            cv::absdiff(ch[1],b,ch[1]);
//            cv::add(ch[0],ch[1],I2);
//            cv::normalize(I2,I1,0,255,CV_MINMAX);
//            I1.convertTo(I1,CV_8U,1,0);
//            //cv::threshold(I1,I1,30,255,CV_THRESH_BINARY|CV_THRESH_OTSU);

//            mser(I1, contours);

//            for( int i = (int)contours.size()-1; i >= 0; i-- )
//                {
//                    const vector<Point>& r = contours[i];

//                    for ( int j = 0; j < (int)r.size(); j++ )
//                    {
//                        Point pt = r[j];
//                        a.at<Vec3b>(pt) = bcolors[i%9];

//                    }
//                    //cerr <<" computer MSER " << contours.size() << ":" << r.size() <<":" << i<<endl;
//                    // find ellipse (it seems cvfitellipse2 have error or sth?)
//                    //RotatedRect box = fitEllipse( r );

//                    //box.angle=(float)CV_PI/2-box.angle;
//                    //ellipse( ellipses, box, Scalar(196,255,255), 2 );
//                }

//            imshow("flow",I2);
//            imshow("divergence",I1);
//            imshow("mser",a);
//        }
//        cv::waitKey(1);
//        std::swap(prevgray, gray);

//    }
//    return 0;
//}
