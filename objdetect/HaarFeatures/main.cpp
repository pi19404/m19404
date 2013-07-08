//#include "mainwindow.h"
//#include <QApplication>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "time_measure.hpp"
using namespace std;
using namespace cv;
class Rectangles
{
public:

    Rectangles()
    {

    }

    void init(int x1,int y1,int width,int height,int weight)
    {
        _r.x=x1;
        _r.y=y1;
        _r.width=width;
        _r.height=height;
        _weight=weight;
    }

    Rectangles(int x1,int y1,int width,int height,int weight)
    {
        _r.x=x1;
        _r.y=y1;
        _r.width=width;
        _r.height=height;
        _weight=weight;
    }

    cv::Rect _r;
    int _weight;
};

class HaarFeatures{
public:
    vector<Rectangles> features;
    int type;
    float value;

};

class HaarFeaturesSet
{

public:
    vector<HaarFeatures> feature;
    HaarFeaturesSet()
    {
        winSize=cv::Size(24,24);
    }

public:
    cv::Size winSize;
    Mat sum,sqsum;
    float *sptr;
    float *ssptr;
    /**
     * @brief compute the Haar Features for image
     * @param image
     */
    void compute(Mat image)
    {
            TimeMeasure t;
            cv::integral(image,sum,sqsum,CV_32F);
            sptr=sum.ptr<float>();
            ssptr=sqsum.ptr<float>();

            for(int i=0;i<feature.size();i++)
            {
                HaarFeatures h=feature[i];
                float val=0;
                float area=0;
                float ssum=0;
                float mean=0;
                for(int j=0;j<h.features.size();j++)
                {
                    Rectangles r=h.features[j];
                    float tmp=compute_rect1(r._r);
                    float tmp1=compute_rect2(r._r);

                    val=val+(r._weight*tmp); //haar feature value

                    ssum=ssum+tmp1; //commputation of squared sum
                    mean=mean+tmp; //computation of mean
                    area=area+(r._r.width*r._r.height); //computation are area
                }
                //val=val/area;
                float variance=mean*area-ssum;
                variance=variance/area;

                feature[i].value=val/variance; //variance normalization
            }
    }

    /**
     * @brief compute_rect1 Compute the Sum of pixels in Rectangular Area
     * @param r
     * @return
     */


    float compute_rect1(Rect r)
    {
        float x=0;
        int index1=r.x+sum.cols*r.y;
        int index2=r.x+r.width+sum.cols*r.y;
        int index3=r.x+sum.cols*(r.y+r.width);
        int index4=(r.x+r.width)+sum.cols*(r.y+r.width);

        x=sptr[index1]+sptr[index4]-sptr[index3]-sptr[index4];
        return x;

    }

    /**
     * @brief compute_rect1 Compute the Squared Sum of pixels in Rectangular Area
     * @param r
     * @return
     */
    float compute_rect2(Rect r)
    {
        float x=0;
        int index1=r.x+sum.cols*r.y;
        int index2=r.x+r.width+sum.cols*r.y;
        int index3=r.x+sum.cols*(r.y+r.width);
        int index4=(r.x+r.width)+sum.cols*(r.y+r.width);

        x=sptr[index1]+sptr[index4]-sptr[index3]-sptr[index4];
        return x;

    }
    void generateFeatures()
    {
        int offset = winSize.width + 1;

        for( int dx = 1; dx <= winSize.width; dx++ )
        {
            for( int dy = 1; dy <= winSize.height; dy++ )
            {

                for( int y = 0; y < winSize.height; y++ )
                {
                for( int x = 0; x < winSize.width; x++ )
                {

                            HaarFeatures f1,f2,f3,f4,f5;
                            Rectangles r1;//(x,y,2*dx,dy,-1);
                            Rectangles r2;//(x+dx,y,dx,2*dy,+2);
                            Rectangles r3(x+dx,y+dy,dx,dy,2);


                            r1.init (x,y,2*dx,dy,-1);
                            r2.init (x+dx,y,dx,dy,2);

                            if(r1._r.x+r1._r.width <= winSize.width && r1._r.y+r1._r.height <= winSize.height &&r2._r.x+r2._r.width <= winSize.width && r2._r.y+r2._r.height <= winSize.height )
                            {
                            f1.features.push_back (r1);
                            f1.features.push_back (r2);
                            f1.type=0;
                            feature.push_back (f1);
                            }


                            r1.init (x,y,1*dx,2*dy,-1);
                            r2.init (x,y+dy,dx,dy,2);
                            if(r1._r.x+r1._r.width <= winSize.width && r1._r.y+r1._r.height <= winSize.height &&r2._r.x+r2._r.width <= winSize.width && r2._r.y+r2._r.height <= winSize.height )
                            {
                            f2.features.push_back (r1);
                            f2.features.push_back (r2);
                            f2.type=1;
                            feature.push_back (f2);
                            }


                            r1.init (x,y,3*dx,dy,-1);
                            r2.init (x+dx,y,dx,dy,3);

                            if(r1._r.x+r1._r.width <= winSize.width && r1._r.y+r1._r.height <= winSize.height &&r2._r.x+r2._r.width <= winSize.width && r2._r.y+r2._r.height <= winSize.height )
                            {
                            f3.features.push_back (r1);
                            f3.features.push_back (r2);
                            f3.type=1;
                            feature.push_back (f3);
                            }

                            r1.init (x,y,dx,3*dy,-1);
                            r2.init (x,y+dy,dx,dy,3);
                            if(r1._r.x+r1._r.width <= winSize.width && r1._r.y+r1._r.height <= winSize.height &&r2._r.x+r2._r.width <= winSize.width && r2._r.y+r2._r.height <= winSize.height )
                            {
                            f4.features.push_back (r1);
                            f4.features.push_back (r2);
                            f4.type=1;
                            feature.push_back (f4);
                            }



                            r1.init(x,y,2*dx,2*dy,-1);
                            r2.init(x,y,dx,dy,2);


                            if(r1._r.x+r1._r.width <= winSize.width && r1._r.y+r1._r.height <= winSize.height &&r2._r.x+r2._r.width <= winSize.width && r2._r.y+r2._r.height <= winSize.height )
                            {
                            f5.features.push_back (r1);
                            f5.features.push_back (r2);
                            f5.features.push_back (r3);
                            feature.push_back (f5);
                            }

                    }

                }
                //if(dy==3)
                  //  break;
            }
        }


        cerr << feature.size () << endl;

    }

};



int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    HaarFeaturesSet s;
    TimeMeasure t;
    t.addTimePoint("Recntagular feature computation");
    s.generateFeatures();
    t.addTimePoint("Time take to generate rectangular feature template");
    //t.printTimePoints();

    cv::Mat image=cv::imread(argv[1],0);
    Mat image2;
    cv::resize(image,image2,Size(25,25),0,0);
    //s.generateFeatures ();
    t.addTimePoint("computing Haar Features");
    s.compute(image2);
    t.addTimePoint("complted computing haar features");
    t.printTimePoints();
    return 0;

}
