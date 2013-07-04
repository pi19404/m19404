
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <cv.h>
using namespace std;
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"
#include "CascadeClassifier.h"
#include "Global.h"
#include "FFS.H"



CascadeClassifier::CascadeClassifier():count(0),ac(NULL)
{
}

CascadeClassifier::~CascadeClassifier()
{
    Clear();
}

void CascadeClassifier::Clear()
{
    count = 0;
    delete[] ac; ac=NULL;
}

CascadeClassifier& CascadeClassifier::operator=(const CascadeClassifier& source)
{
    Clear();
    count = source.count;
    ac = new AdaBoostClassifier[max_nodes]; ASSERT(ac!=NULL);
    for(int i=0;i<count;i++) ac[i] = source.ac[i];
    return *this;
}

void CascadeClassifier::ReadFromFile(ifstream& f)
{
    Clear();
    f>>count;
    cerr << "starting node : " << count << endl;
    f.ignore(256,'\n');

    ac = new AdaBoostClassifier[max_nodes]; ASSERT(ac!=NULL);
    for(int i=0;i<count;i++)
    {
        cerr << "node " << i ;
    ac[i].ReadFromFile(f);
    }
    cerr << "completed reading the cascade classifier files " << endl;
}

void CascadeClassifier::WriteToFile(ofstream& f) const
{
    f<<count<<endl;
    for(int i=0;i<count;i++) ac[i].WriteToFile(f);
}

void CascadeClassifier::LoadDefaultCascade()
{
    ifstream f;
    cerr << "Cascade calssifier filename is " << cascade_filename << endl;
    f.open(cascade_filename);
    if(f.is_open())
        cerr << "cascade classifer file opened successfully " <<endl;
    else
        perror("");
    ReadFromFile(f);
    f.close();
}

IplImage * CascadeClassifier::DrawResults(IntImage& image,const vector<QRect>& results) const
{
    int i;
    unsigned int k;
    int x1,x2,y1,y2;

    IplImage * result=cvCreateImage(cvSize(image.width,image.height),8,3);

    IplImage* img;

    img = cvCreateImage(cvSize(image.width,image.height),IPL_DEPTH_8U,1);
    for(int i=0,ih=img->height,iw=img->width;i<ih;i++)
    {
        REAL* pdata = image.data[i];
        unsigned char* pimg = reinterpret_cast<unsigned char*>(img->imageData+img->widthStep*i);
        for(int j=0;j<iw;j++) pimg[j] = (unsigned char)pdata[j];
    }

    cvCvtColor(img,result,CV_GRAY2BGR);
    cvReleaseImage(&img);
    for(k=0;k<results.size();k++)
    {
        QRect a=results[k];

        y1 = (a.top()>=0)?a.top():0;
        y1 = (results[k].top()<image.height)?results[k].top():(image.height-1);
        y2 = (results[k].bottom()>=0)?results[k].bottom():0;
        y2 = (results[k].bottom()<image.height)?results[k].bottom():(image.height-1);
        x1 = (results[k].left()>=0)?results[k].left():0;
        x1 = (results[k].left()<image.width)?results[k].left():(image.width-1);
        x2 = (results[k].right()>=0)?results[k].right():0;
        x2 = (results[k].right()<image.width)?results[k].right():(image.width-1);

        cvDrawRect(result,cvPoint(x1,y1),cvPoint(x2,y2),CV_RGB(255,0,0),3,8);
        for(i=y1;i<=y2;i++)
        {
            image.data[i][x1] = 255;
            image.data[i][x2] = 255;
        }
        for(i=x1;i<=x2;i++)
        {
            image.data[y1][i] = 255;
            image.data[y2][i] = 255;
        }
    }
    cerr << "completed drawing results " << endl;
return result;
}

const int CascadeClassifier::ApplyImagePatch(const IntImage& im) const
{
    for(int i=0;i<count;i++) if(ac[i].ApplyImagePatch(im)==0) return 0;
    return 1;
}

IplImage* CascadeClassifier::ApplyOriginalSize(IntImage& original,const CString filename)
{
    IntImage procface;
    IntImage image,square;
    REAL sq,ex,value;
    int result;
    QRect rect;
    REAL ratio;
    vector<QRect> results;

    procface.Copy(original);
    ratio = 1.0;
    results.clear();
    REAL paddedsize = REAL(1)/REAL((sx+1)*(sy+1));
    while((procface.height>sx+1) && (procface.width>sy+1))
    {
        procface.CalcSquareAndIntegral(square,image);
        for(int i=0,size_x=image.height-sx;i<size_x;i+=1)
            for(int j=0,size_y=image.width-sy;j<size_y;j+=1)
            {
                ex = image.data[i+sx][j+sy]+image.data[i][j]-image.data[i+sx][j]-image.data[i][j+sy];
                if(ex<mean_min || ex>mean_max) continue;
                sq = square.data[i+sx][j+sy]+square.data[i][j]-square.data[i+sx][j]-square.data[i][j+sy];
                if(sq<sq_min) continue;
                ex *= paddedsize;
                ex = ex * ex;
                sq *= paddedsize;
                sq = sq - ex;
                ASSERT(sq>=0);
                if(sq>0) sq = sqrt(sq); else sq = 1.0;
                if(sq<var_min) continue;
                result = 1;
                for(int k=0;k<count;k++)
                {
                    value = 0.0;
                    for(int t=0,size=ac[k].count;t<size;t++)
                    {
                        REAL f1 = 0;
                        REAL** p = image.data + i;
                        SimpleClassifier& s = ac[k].scs[t];

                        switch(s.type)
                        {
                            case 0:
                                f1 =   p[s.x1][j+s.y3] - p[s.x1][j+s.y1] + p[s.x3][j+s.y3] - p[s.x3][j+s.y1]
                                    + REAL(2)*(p[s.x2][j+s.y1] - p[s.x2][j+s.y3]);
                                break;
                            case 1:
                                f1 =   p[s.x3][j+s.y1] + p[s.x3][j+s.y3] - p[s.x1][j+s.y1] - p[s.x1][j+s.y3]
                                    + REAL(2)*(p[s.x1][j+s.y2] - p[s.x3][j+s.y2]);
                                break;
                            case 2:
                                f1 =   p[s.x1][j+s.y1] - p[s.x1][j+s.y3] + p[s.x4][j+s.y3] - p[s.x4][j+s.y1]
                                    + REAL(3)*(p[s.x2][j+s.y3] - p[s.x2][j+s.y1] + p[s.x3][j+s.y1]  - p[s.x3][j+s.y3]);
                                break;
                            case 3:
                                f1 =   p[s.x1][j+s.y1] - p[s.x1][j+s.y4] + p[s.x3][j+s.y4] - p[s.x3][j+s.y1]
                                    + REAL(3)*(p[s.x3][j+s.y2] - p[s.x3][j+s.y3] + p[s.x1][j+s.y3] - p[s.x1][j+s.y2]);
                                break;
                            case 4:
                                f1 =   p[s.x1][j+s.y1] + p[s.x1][j+s.y3] + p[s.x3][j+s.y1] + p[s.x3][j+s.y3]
                                    - REAL(2)*(p[s.x2][j+s.y1] + p[s.x2][j+s.y3] + p[s.x1][j+s.y2] + p[s.x3][j+s.y2])
                                    + REAL(4)*p[s.x2][j+s.y2];
                                break;
                            default:
                            break;
                        }
                        if(s.parity!=0)
                            if(f1<sq*s.thresh)
                                value += ac[k].alphas[t];
                            else ;
                        else
                            if(f1>=sq*s.thresh)
                                value += ac[k].alphas[t];
                            else ;
                    }
                    if(value<ac[k].thresh)
                    {
                        result = 0;
                        break;
                    }
                }
                if(result!=0)
                {
                    const REAL r = 1.0/ratio;
                    rect.setLeft((long)(j*r));
                    rect.setTop((long)(i*r));
                    rect.setRight((long)((j+sy)*r));
                    rect.setBottom((long)((i+sx)*r));
                    results.push_back(rect);
                }
            }
        ratio = ratio * REAL(0.8);
        procface.Resize(image,REAL(0.8));
        SwapIntImage(procface,image);
    }

    total_fp += results.size();

    PostProcess(results,2);
    PostProcess(results,0);
    IplImage * res=DrawResults(original,results);
    return res;
//	original.Save(filename+"_result.JPG");
}

inline int SizeOfRect(const QRect& rect)
{

    return rect.height()*rect.width();
}

void PostProcess(vector<QRect>& result,const int combine_min)
{
    vector<QRect> res1;
    vector<QRect> resmax;
    vector<int> res2;
    bool yet;
    QRect rectInter;

    for(unsigned int i=0,size_i=result.size();i<size_i;i++)
    {
        yet = false;
        QRect& result_i = result[i];
        for(unsigned int j=0,size_r=res1.size();j<size_r;j++)
        {
            QRect& resmax_j = resmax[j];
            if(result_i.intersects(resmax_j))
            {
                rectInter=result_i.intersected(resmax_j);
                if(
                    SizeOfRect(rectInter)>0.6*SizeOfRect(result_i)
                 && SizeOfRect(rectInter)>0.6*SizeOfRect(resmax_j)
                  )
                {
                    QRect& res1_j = res1[j];
                    resmax_j=resmax_j.united(result_i);

                    res1_j.setBottom(res1_j.bottom()+result_i.bottom());
                    res1_j.setTop(res1_j.top()+result_i.top());
                    res1_j.setLeft(res1_j.left()+result_i.left());
                    res1_j.setRight(res1_j.right()+result_i.right());
                    res2[j]++;
                    yet = true;
                    break;
                }
            }
        }
        if(yet==false)
        {
            res1.push_back(result_i);
            resmax.push_back(result_i);
            res2.push_back(1);
        }
    }

    for(unsigned int i=0,size=res1.size();i<size;i++)
    {
        const int count = res2[i];
        QRect& res1_i = res1[i];
        res1_i.setTop(res1_i.top()/count);
        res1_i.setBottom(res1_i.bottom()/count);
        res1_i.setLeft(res1_i.left()/count);
        res1_i.setRight(res1_i.right()/count);

    }

    result.clear();
    for(unsigned int i=0,size=res1.size();i<size;i++)
        if(res2[i]>combine_min)
            result.push_back(res1[i]);
}

void CascadeClassifier::ApplyOriginalSizeForInputBoosting(const CString filename,int& pointer) const
{
    IntImage procface;
    IntImage image,square;
    REAL sq,ex,value;
    int result;
    QRect rect;
    REAL ratio;
   // cerr << "loading file name is " << filename <<endl ;
    procface.Load(filename);
    if(procface.height <=0 || procface.width<=0)
    {
     //   cerr << "error loadin the file" << endl;
   //     perror("");
        return;

    }
  //  cerr << "file loaded successfully " << endl;
    ratio = 1.0;
    REAL paddedsize = REAL(1)/REAL((sx+1)*(sy+1));
    while((procface.height>sx+1) && (procface.width>sy+1))
    {
        procface.CalcSquareAndIntegral(square,image);
        for(int i=0,size_x=image.height-sx;i<size_x;i+=bootstrap_increment[bootstrap_level])
            for(int j=0,size_y=image.width-sy;j<size_y;j+=bootstrap_increment[bootstrap_level])
            {
                ex = image.data[i+sx][j+sy]+image.data[i][j]-image.data[i+sx][j]-image.data[i][j+sy];
                if(ex<mean_min || ex>mean_max) continue;
                sq = square.data[i+sx][j+sy]+square.data[i][j]-square.data[i+sx][j]-square.data[i][j+sy];
                if(sq<sq_min) continue;
                ex *= paddedsize;
                ex = ex * ex;
                sq *= paddedsize;
                sq = sq - ex;
                ASSERT(sq>=0);
                if(sq>0) sq = sqrt(sq); else sq = 1.0;
                if(sq<var_min) continue;
                result = 1;
                for(int k=0;k<count;k++)
                {
                    value = 0.0;
                    for(int t=0,size=ac[k].count;t<size;t++)
                        value += (ac[k].alphas[t]*ac[k].scs[t].Apply(ac[k].scs[t].GetOneFeatureTranslation(image.data+i,j)/sq));
                    if(value<ac[k].thresh)
                    {
                        result = 0;
                        //cerr << "achieved the deciired threshold " << k <<":" << value<ac[k].thresh<< endl;
                        break;
                    }
                }
                if(result==1)
                {
                    for(int k=1;k<=sx;k++)
                        for(int t=1;t<=sy;t++)
                            trainset[pointer].data[k][t]=image.data[i+k][j+t]-image.data[i+k][j]-image.data[i][j+t]+image.data[i][j];
                    pointer++;
                    if(pointer==totalcount) return;
                }
            }
        ratio = ratio * bootstrap_resizeratio[bootstrap_level];
        procface.Resize(image,bootstrap_resizeratio[bootstrap_level]);
        SwapIntImage(procface,image);
    }
   // cerr << "complted " << endl;
}

void AppendAdaBoostClassifier(const AdaBoostClassifier& ada)
{
    CascadeClassifier cas;
    ifstream f;
    ofstream of;

    f.open(cascade_filename);
    cas.ReadFromFile(f);
    f.close();

    cas.ac[cas.count] = ada;
    cas.count++;

    of.open(cascade_filename);
    cas.WriteToFile(of);
    of.close();
}
