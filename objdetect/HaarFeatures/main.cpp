//#include "mainwindow.h"
//#include <QApplication>
#include <QtCore/QDir>
#include <stdio.h>


#include <cv.h>
#include <highgui.h>
#include <QtCore/QDir>
#include <math.h>

#include <iostream>
#include <opencv2/core/core.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "time_measure.hpp"
#include "ml/ml.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/opencv_modules.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <mlpack/core.hpp>
#include <stdio.h>
#include "opencv2/core/internal.hpp"




using namespace mlpack;
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
    Size winSize;
    HaarFeaturesSet()
    {
        winSize=cv::Size(24,24);
    }

    vector<HaarFeatures> feature;
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


        //cerr << feature.size () << endl;

    }

};

class FeaturesSet
{

public:
    int lable;

    vector<float> feature;
    FeaturesSet()
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
    void compute(Mat image2,HaarFeaturesSet f)
    {


        Mat image;
        cv::resize(image2,image,Size(winSize.width+1,winSize.height+1),0,0);



            cv::integral(image,sum,sqsum,CV_32F);
            sptr=sum.ptr<float>();
            ssptr=sqsum.ptr<float>();

            for(int i=0;i<f.feature.size();i++)
            {
                HaarFeatures h=f.feature[i];
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
                float variance=(mean*area)-ssum;
                variance=variance/area;

                val=val/(variance+0.0001); //variance normalization
                feature.push_back(val);

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

};
class DirList
{
public :
int count;
QString dir;
QDir data_path;
QStringList classes;
int position;
QStringList files;

public :
DirList(QString dir1)
{
    dir=QString(dir1);
    data_path=QDir(dir1.toStdString().c_str());
    count=CountFiles();
    files = data_path.entryList();
    position=0;
}

int CountFiles()
{
   int sum = 0;
   //QDir dir(dir.path());
//  data_path.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
//   QFileInfoList lst = data_path.entryInfoList();
//   for(int i = 0; i < lst.size(); i++)
//   {
//       sum = sum + CountFiles(lst.at(i).canonicalPath());
//   }
 data_path.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

   return data_path.entryInfoList().size();
}


QString GetFiles()
{

    QString trainSet;
    bool ret=false;
    if(position>=count)
    {
        position=0;

    }

    while(ret==false)
    {

    trainSet = files.at(position);
    if (trainSet.startsWith("."))
    {
            position=position+1;
            continue;
    }
        position=position+1;
        ret=true;
    }
    return QString("%1/%2").arg(data_path.path()).arg(trainSet);
}
};



/*class WeakLearner : public DesisionStump
{
    HaarFeatures h;

};*/

class DesisionStump
{

        float threshold;
        //the function finds the best split wrt L2 error by trying
        //out all possible splits.

     /*   findSplit(const arma::mat& data,const arma::mat &labels)
        {

            // Get the values for the dimension.
            arma::rowvec dimVec = data.row(0);
            //sorting the list
            uvec indices  = arma::sort_index(dimVec);

            float min=dimVec[0];
            float max=dimVec[dimVec.n_elem-1];

            const int points=dimVec.n_elem;
            //consider the maximum error,if all points are placed in incorrect class.

            double minDimError = std::pow(points, 2.0) / (max - min);
            for(int i=0;i<dimVec.n_elem-1;i++)
            {
                const double split = (dimVec[i] + dimVec[i + 1]) / 2.0;
                if (split == dimVec[i])
                    continue;

                if ((split - min > 0.0) && (max - split > 0.0))
                {

                    double negLeftError = std::pow(i + 1, 2.0) / (split - min);
                    double negRightError = std::pow(points - i - 1, 2.0) / (max - split);



                }

            }


        }*/
};


/*
CvDTree  dtree;
Mat subsample_mask;
dtree.train(data,subsample_mask);
if(cv::countNonZero(subsample_mask)==0)
break;
*/


class train_sample
{
    public:
    int label;
    vector<float> data;
    FeaturesSet features;
    QString filename;
};


class train_data
{
public:
    vector<train_sample> data;
};

//object for each feature contains value as well as label

class Feature_Data
{
public:
    int label;
    float features;
};

class Data
{
public:
    //vector of vector<Feature_Data> corresponding to number of features
    //entry for feature data corresponding to number of training samples
    vector<vector<Feature_Data> > data;
};


class Feat
{
public:
    vector<Rect> haarx;
};




int main(int argc,char **argv)
{

    TimeMeasure time;
    HaarFeaturesSet f;
    FeaturesSet fs;

    time.addTimePoint("Recntagular feature computation");
    f.generateFeatures();
    time.addTimePoint("Time take to generate rectangular");

    train_data t;
    DirList *list=new DirList("/media/windows/pi19404/Haar/sample_database/");

    time.addTimePoint("computing Haar Features");

    while(list->position<list->count)
    {

        QString DirName =list->GetFiles();
        DirList *list1=new DirList(DirName);
        qDebug(QString("%1%2").arg("Directory Name :",list1->dir).toStdString().c_str());

        while(list1->position<list1->count)
        {
            QString FileName =list1->GetFiles();
            train_sample s;
            s.label=DirName.toInt();
            QString abspath=QString("%1").arg(FileName);
            qDebug(abspath.toStdString().c_str());
            Mat image=cv::imread(abspath.toStdString());
            fs.compute(image,f);

            FeaturesSet h=fs;
            s.features=h;
            s.filename=abspath;
            t.data.push_back(s);


        }



    }
    time.addTimePoint("complted computing haar features");
    time.printTimePoints();

    Data d;

    d.data.resize(f.feature.size());
    for(int i=0;i<d.data.size();i++)
    {
        //vector<Feature_Data> k=;
        //train_sample k1=t.data[i];
        //d.data[i].lable=k1.label;
        d.data[i].resize(t.data.size());

    }

    qDebug("training weak learners");
    for(int i=0;i<t.data.size();i++)
    {
        train_sample s=t.data[i];
        for(int j=0;j<f.feature.size();j++)
        {
            Feature_Data f;
            f.label=s.label;
            f.features=s.features.feature[j];
            //d.data[j][i]=f;


        }


    }
    qDebug("completed weak learners");



}
    //training_data t;





