#include "mainwindow.h"
#include <QApplication>
#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/unordered_map.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


using namespace cv;


#define ELEM(type,start,step,size,row, col) ((type*)(start+step*(row)+(col)*size))
// The preprocessor macro may be defined in the beginning of the header file



#include "a.hpp"




Mat CreateGaborFilterImage(char* demoName,   float lambda, float theta, float psi, float gaussvar, float gamma,Size size=Size(21,21));
Mat CreateGaborFilterImage(char* demoName,   float lambda, float theta, float psi, float gaussvar, float gamma,Size size)
{


    Mat ima1=cv::getGaborKernel(size,gaussvar,theta,lambda,gamma,psi,CV_32F);
//    cerr << ima.cols <<":" << ima.rows << ":"<<ima.channels() <<endl;
    //Mat gaborKernel = getGaborKernel(size, 3.0, -Math.PI/4, Math.PI, 10.0, Math.PI*0.5, CvType.CV_64F);
    //Imgproc.filter2D(intermediate, output, -1, gaborKernel);
    //Bitmap temp = Bitmap.createBitmap(intermediate.cols(), intermediate.rows(), Config.ARGB_8888);
    //Utils.matToBitmap(output, temp);
    Mat d1=Mat(size.width,size.height,CV_8UC(3));
    //ima.convertTo(d1,CV_8U,255.0f);
    d1.setTo(cv::Scalar::all(0));

    int mx=d1.cols/2;
    int my=d1.rows/2;
    Mat ima;
    cv::normalize(ima1,ima,-1,1,cv::NORM_MINMAX);
    float *ptr1=ima.ptr<float>();
    uchar *ptr2=d1.ptr<uchar>();
    float MinGaborValue = std::numeric_limits<float>::max();
    float MaxGaborValue = std::numeric_limits<float>::min();
    for (int dy=0; dy<size.height; dy++)
        {
            for (int dx=0; dx<size.width; dx++)
            {

                double val=ptr1[dx+dy*size.width];
                //cerr << val <<endl;
                int R=0;
                if (val>=0.1f)
                {
                    R = (int)floor((val * 255.0f));

                    int sx=(size.width/2+(dx))%size.width;
                    sx=dx;
                    int sy=dy;//(size.height/2+(dy))%size.height;
                    ptr2[3*(sx)+(3*(sy*d1.cols))+0]=R;
                }
                else if(val <=-0.1f)
                {
                    R = (int)floor((-val * 255.0f));
                    int sx=(size.width/2+(dx))%size.width;
                    int sy=dy;//(size.height/2+(dy))%size.height;
                    sx=dx;
                    //int sy=(size.width/2+(dy))%size.height;
                    ptr2[3*(sx)+(3*(sy*d1.cols))+2]=R;
                }
      /*          else
                {
                    R=0;
                    int sx=(size.width/2+(dx))%size.width;
                    int sy=dy;//(size.height/2+(dy))%size.height;
                    sx=dx;
                    ptr2[3*(sx)+(3*(sy*d1.cols))+0]=R;
                    ptr2[3*(sx)+(3*(sy*d1.cols))+1]=R;
                    ptr2[3*(sx)+(3*(sy*d1.cols))+2]=R;
                }*/

                if (val<MinGaborValue)
                MinGaborValue = val;
                  if (val>MaxGaborValue)
                   MaxGaborValue = val;

            }
        }
    //cerr << MinGaborValue << ":" << MaxGaborValue << endl;
    Mat d2;
    cv::resize(d1,d2,Size(256,256));
    //d1.copyTo(d2);
    //cv::normalize(d2,d2,0,255,cv::NORM_MINMAX);
/*
    CvFont font;
        double hScale=1.0;
        double vScale=1.0;
        int    lineWidth=1;
        cvInitFont(&font,CV_FONT_HERSHEY_PLAIN, hScale,vScale,0,lineWidth);
        char txt[100];
        /*cvPutText (img, "Gabor filter values visualization by Juergen Mueller.",                             cvPoint(10,20), &font, cvScalar(0,255,255) );
        cvPutText (img, "variable naming conventions as used by http://en.wikipedia.org/wiki/Gabor_filter.", cvPoint(10,50), &font, cvScalar(0,255,255) );
        cvPutText (img, "Red: positive values. Blue: negative values",                                       cvPoint(10,70), &font, cvScalar(0,255,255) );


        cvPutText(img, demoName, cvPoint(10,100), &font, cvScalar(0,255,0) );
        sprintf_s(txt, "1. wavelength  (lambda) : %.2f", wavelength);    cvPutText (img, txt, cvPoint(10,120), &font, cvScalar(0,255,255) );
        sprintf_s(txt, "2. orientation (theta)  : %.2f", orientation);   cvPutText (img, txt, cvPoint(10,140), &font, cvScalar(0,255,255) );
        sprintf_s(txt, "3. gaussvar    (sigma)  : %.2f", gaussvar);      cvPutText (img, txt, cvPoint(10,160), &font, cvScalar(0,255,255) );
        sprintf_s(txt, "4. phaseoffset (phi)    : %.2f", phaseoffset);   cvPutText (img, txt, cvPoint(10,180), &font, cvScalar(0,255,255) );
        sprintf_s(txt, "5. aspectratio (gamma)  : %.2f", aspectratio);   cvPutText (img, txt, cvPoint(10,200), &font, cvScalar(0,255,255) );

        sprintf_s(txt, "min gabor val : %.2f", MinGaborValue);   cvPutText (img, txt, cvPoint(10,230), &font, cvScalar(0,255,255) );
        sprintf_s(txt, "max gabor val : %.2f", MaxGaborValue);   cvPutText (img, txt, cvPoint(10,250), &font, cvScalar(0,255,255) );
*/
    imshow(demoName,d2);
    waitKey(1000);
    return ima1;

}


int kernel_size=3;
int pos_sigma= 2;
int pos_lm = 2;
int pos_th = 90;
int pos_psi = 90;
Mat dest;
void Process(Mat src_f)
{
    Mat rr;
    double sig = pos_sigma;
    double lm = 0.5+pos_lm/100.0;
    double th = pos_th*CV_PI/180;
    double ps = pos_psi*CV_PI/180;
    cv::Mat kernel=CreateGaborFilterImage
            ("1. Wavelength is changed.", lm, th, ps, sig, 1,Size(kernel_size,kernel_size));
    //cv::Mat kernel = mkKernel(kernel_size, sig, th, lm, ps);
    cv::filter2D(src_f, rr, CV_32F, kernel);

    rr.convertTo(dest,CV_8UC(1),255.0,0);
    Mat kernel1=cv::getStructuringElement(cv::MORPH_ELLIPSE,Size(10,1));
    cv::bitwise_not(dest,dest);
    cv::morphologyEx(dest,dest,cv::MORPH_OPEN,kernel1,Point(-1,-1),3);



    cv::imshow("Process window", dest);


    cv::Mat Lkernel(kernel_size*20, kernel_size*20, CV_32F);
    cv::resize(kernel, Lkernel, Lkernel.size());
    Lkernel /= 2.;
    Lkernel += 0.5;
    cv::imshow("Kernel", Lkernel);
    cv::Mat mag;
    cv::pow(dest, 2.0, mag);
    cv::imshow("Mag", mag);
}

struct Point2d1 {
    int x;
    int y;
    float SWT;
};

struct Point2dFloat {
    float x;
    float y;
};


struct Ray {
        Point2d1 p;
        Point2d1 q;
        std::vector<Point2d1> points;
};

bool Point2dSort (const Point2d1 &lhs, const Point2d1 &rhs) {
    return lhs.SWT < rhs.SWT;
}




std::vector< std::vector<Point2d1> >
findLegallyConnectedComponents (IplImage * SWTImage,
                                std::vector<Ray> & rays)
{
        IplImage *out=cvCreateImage(cvGetSize(SWTImage),IPL_DEPTH_32F, 1);
        for( int row = 0; row < out->height; row++ ){
            float* ptr = (float*)(out->imageData + row * out->widthStep);
            for ( int col = 0; col < out->width; col++ ){
                *ptr++ = -1;
            }
        }
        boost::unordered_map<int, int> map;
        boost::unordered_map<int, Point2d1> revmap;

        typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
        int num_vertices = 0;
        // Number vertices for graph.  Associate each point with number
        for( int row = 0; row < SWTImage->height; row++ ){
            float * ptr = (float*)(SWTImage->imageData + row * SWTImage->widthStep);
            for (int col = 0; col < SWTImage->width; col++ ){
                if (*ptr > 0) {
                    map[row * SWTImage->width + col] = num_vertices;
                    Point2d1 p;
                    p.x = col;
                    p.y = row;
                    revmap[num_vertices] = p;
                    num_vertices++;
                }
                ptr++;
            }
        }

        Graph g(num_vertices);

        for( int row = 0; row < SWTImage->height; row++ ){
            float * ptr = (float*)(SWTImage->imageData + row * SWTImage->widthStep);
            for (int col = 0; col < SWTImage->width; col++ ){
                if (*ptr > 0) {
                    float current = *ptr;
                    // check pixel to the right, right-down, down, left-down
                    int this_pixel = map[row * SWTImage->width + col];
                    if (col+1 < SWTImage->width) {
                        float right = CV_IMAGE_ELEM(SWTImage, float, row, col+1);
                        if (right > 0 && ((*ptr)/right <= 3.0 || right/(*ptr) <= 3.0))
                            boost::add_edge(this_pixel, map.at(row * SWTImage->width + col + 1), g);
                            CV_IMAGE_ELEM(out, float,row,col)=min(current,right);
                            CV_IMAGE_ELEM(out, float,row,col+1)=min(current,right);
                    }
                    if (col-1 >0) {
                        float right = CV_IMAGE_ELEM(SWTImage, float, row, col-1);
                        if (right > 0 && ((*ptr)/right <= 3.0 || right/(*ptr) <= 3.0))
                            boost::add_edge(this_pixel, map.at(row * SWTImage->width + col -1), g);
                            CV_IMAGE_ELEM(out, float,row,col)=min(current,right);
                            CV_IMAGE_ELEM(out, float,row,col-1)=min(current,right);
                    }


                    if (row-1 > 0) {
                        if (col < SWTImage->width) {
                            float right_down = CV_IMAGE_ELEM(SWTImage, float, row-1, col);
                            if (right_down > 0 && ((*ptr)/right_down <= 3.0 || right_down/(*ptr) <= 3.0))
                                boost::add_edge(this_pixel, map.at((row-1) * SWTImage->width + col ), g);
                                CV_IMAGE_ELEM(out, float,row,col)=min(current,right_down);
                                CV_IMAGE_ELEM(out, float,row-1,col)=min(current,right_down);


                        }
                        if (col+1 < SWTImage->width) {
                            float right_down = CV_IMAGE_ELEM(SWTImage, float, row-1, col+1);
                            if (right_down > 0 && ((*ptr)/right_down <= 3.0 || right_down/(*ptr) <= 3.0))
                                boost::add_edge(this_pixel, map.at((row-1) * SWTImage->width + col+1 ), g);
                                CV_IMAGE_ELEM(out, float,row,col)=min(current,right_down);
                                CV_IMAGE_ELEM(out, float,row-1,col+1)=min(current,right_down);


                        }
                        if (col-1 >0) {
                            float right_down = CV_IMAGE_ELEM(SWTImage, float, row-1, col-1);
                            if (right_down > 0 && ((*ptr)/right_down <= 3.0 || right_down/(*ptr) <= 3.0))
                                boost::add_edge(this_pixel, map.at((row-1) * SWTImage->width + col-1 ), g);
                                CV_IMAGE_ELEM(out, float,row,col)=min(current,right_down);
                                CV_IMAGE_ELEM(out, float,row-1,col-1)=min(current,right_down);


                        }

                    }
                    if (row+1 < SWTImage->height) {
                        if (col+1 < SWTImage->width) {
                            float right_down = CV_IMAGE_ELEM(SWTImage, float, row+1, col+1);
                            if (right_down > 0 && ((*ptr)/right_down <= 3.0 || right_down/(*ptr) <= 3.0))
                                boost::add_edge(this_pixel, map.at((row+1) * SWTImage->width + col + 1), g);
                                CV_IMAGE_ELEM(out, float,row,col)=min(current,right_down);
                                CV_IMAGE_ELEM(out, float,row+1,col+1)=min(current,right_down);


                        }


                        float down = CV_IMAGE_ELEM(SWTImage, float, row+1, col);
                        if (down > 0 && ((*ptr)/down <= 3.0 || down/(*ptr) <= 3.0))
                            boost::add_edge(this_pixel, map.at((row+1) * SWTImage->width + col), g);
                            CV_IMAGE_ELEM(out, float,row,col)=min(current,down);
                            CV_IMAGE_ELEM(out, float,row+1,col)=min(current,down);

                        if (col-1 >= 0) {
                            float left_down = CV_IMAGE_ELEM(SWTImage, float, row+1, col-1);
                            if (left_down > 0 && ((*ptr)/left_down <= 3.0 || left_down/(*ptr) <= 3.0))
                                boost::add_edge(this_pixel, map.at((row+1) * SWTImage->width + col - 1), g);
                                CV_IMAGE_ELEM(out, float,row,col)=min(current,left_down);
                                CV_IMAGE_ELEM(out, float,row+1,col-1)=min(current,left_down);

                        }
                    }

                }
                ptr++;
            }
        }

        std::vector<int> c(num_vertices);

        int num_comp = connected_components(g, &c[0]);

        std::vector<std::vector<Point2d1> > components;
        components.reserve(num_comp);
        std::cout << "Before filtering, " << num_comp << " components and " << num_vertices << " vertices" << std::endl;
        for (int j = 0; j < num_comp; j++) {
            std::vector<Point2d1> tmp;
            components.push_back( tmp );
        }
        for (int j = 0; j < num_vertices; j++) {
            Point2d1 p = revmap[j];
            (components[c[j]]).push_back(p);
        }

        cvCopy(out,SWTImage);
        return components;
}

std::vector< std::vector<Point2d1> >
findLegallyConnectedComponentsRAY (IplImage * SWTImage,
                                std::vector<Ray> & rays)
{

    IplImage *out=cvCreateImage(cvGetSize(SWTImage),IPL_DEPTH_32F, 1);
    for( int row = 0; row < out->height; row++ ){
        float* ptr = (float*)(out->imageData + row * out->widthStep);
        for ( int col = 0; col < out->width; col++ ){
            *ptr++ = -1;
        }
    }

        boost::unordered_map<int, int> map;
        boost::unordered_map<int, Point2d1> revmap;

        typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
        int num_vertices = 0;
        // Number vertices for graph.  Associate each point with number
        for( int row = 0; row < SWTImage->height; row++ ){
            float * ptr = (float*)(SWTImage->imageData + row * SWTImage->widthStep);
            for (int col = 0; col < SWTImage->width; col++ ){
                if (*ptr > 0) {
                    map[row * SWTImage->width + col] = num_vertices;
                    Point2d1 p;
                    p.x = col;
                    p.y = row;
                    revmap[num_vertices] = p;
                    num_vertices++;
                }
                ptr++;
            }
        }

        Graph g(num_vertices);

        // Traverse and add edges to graph
        for (std::vector<Ray>::const_iterator it = rays.begin(); it != rays.end(); it++) {
                float lastSW = 0;
                int lastRow = 0;
                int lastCol = 0;
                for (std::vector<Point2d1>::const_iterator it2 = it->points.begin(); it2 != it->points.end(); it2++) {
                        float currentSW = CV_IMAGE_ELEM(SWTImage, float, it2->y, it2->x);
                        if (lastSW == 0) {}
                        else if (lastSW/currentSW<=3.0 || currentSW/lastSW<=3.0){
                                boost::add_edge(map.at(it2->y * SWTImage->width + it2->x), map.at(lastRow * SWTImage->width + lastCol), g);
                                CV_IMAGE_ELEM(out, float,it2->y,it2->x)=min(currentSW,lastSW);
                                CV_IMAGE_ELEM(out, float,lastRow,lastCol)=min(currentSW,lastSW);


                        }
                        lastSW = currentSW;
                        lastRow = it2->y;
                        lastCol = it2->x;
                }
                lastSW = 0;
                lastRow = 0;
                lastCol = 0;
        }

        std::vector<int> c(num_vertices);

        int num_comp = connected_components(g, &c[0]);

        std::vector<std::vector<Point2d1> > components;
        components.reserve(num_comp);
        std::cout << "Before filtering, " << num_comp << " components and " << num_vertices << " vertices" << std::endl;
        for (int j = 0; j < num_comp; j++) {
            std::vector<Point2d1> tmp;
            components.push_back( tmp );
        }
        for (int j = 0; j < num_vertices; j++) {
            Point2d1 p = revmap[j];
            (components[c[j]]).push_back(p);
        }
        cvCopy(out,SWTImage);
        return components;
}

#define PI 3.1412
void SWT(const Mat &edgeImage,const Mat &gradientX,const Mat &gradientY,vector<Ray> &rays,Mat &swtimage)
{

    swtimage=Mat(edgeImage.rows,edgeImage.cols,CV_32FC(1));
    swtimage.setTo(cv::Scalar::all(-1));
    for( int row = 0; row <edgeImage.rows; row++ ){
        const uchar *ptr=edgeImage.ptr<uchar>(row);
        const float *xptr=gradientX.ptr<float>(row);
        const float *yptr=gradientY.ptr<float>(row);
        for ( int col = 0; col < edgeImage.cols; col++ ){
            uchar value=ptr[col];

            if((int)value<=0)
            continue;

            //first element of the ray
            Point2d1 p;
            p.x=col;
            p.y=row;

            //Computing the orientation of gradient


            float step_size=.05;


            //magitude of gradient
            ///cerr << gx <<":" << gy <<endl;
            //uchar *c=ELEM(uchar,swtimage.data,swtimage.step,swtimage.elemSize(),row,col);
            //c[0]=255;
            //continue;
            Ray r[2];


            for(int k=0;k<1;k++)
            {

                vector<Point2d1> points;
                r[k].p=p;
                points.push_back(p);

                float cur_x=(float)col+0.5;
                float cur_y=(float)row+0.5;


                float gx=xptr[col];
                float gy=yptr[col];

                float mag=sqrt((gx*gx)+(gy*gy));
                //unit vector along direction of gradient
                gx=gx/mag;
                gy=gy/mag;

                //moving along both the directions from the edge
                gx=k==0?-gx:gx;
                gy=k==0?-gy:gy;

                int new_x=col;
                int new_y=row;


                while(true)
                {


                    //moving along ray in direction of edge
                    cur_x+=(step_size*gx);
                    cur_y+=(step_size*gy);


                    if(((int)(floor(cur_x))==new_x) && ((int)(floor(cur_y))==new_y))
                    continue;

                    new_x=(int)(floor(cur_x));
                    new_y=(int)(floor(cur_y));

                    //if edge of image has been reached
                    if(new_x<0 || new_x>=swtimage.cols ||new_y<0 || new_y>=swtimage.rows)
                    break;



                        //adding the point to the ray
                    Point2d1 pnew;
                    pnew.x=new_x;
                    pnew.y=new_y;
                    points.push_back(pnew);




                    uchar value1=((uchar *)edgeImage.ptr<uchar>(new_y))[new_x];
                    if(((int)value1)<=0)
                    continue;

                    r[k].q=pnew;
                    float length=sqrt( ((float)r[k].q.x - (float)r[k].p.x)*((float)r[k].q.x - (float)r[k].p.x) +
                    ((float)r[k].q.y - (float)r[k].p.y)*((float)r[k].q.y - (float)r[k].p.y));
                    //cerr << (int)value1 <<":" ;

                    if(length >10*sqrt(2))
                        break;
                    //checking for terminating condition

                    //getting the gradient at the end location

                    float gx1=((float *)(gradientX.ptr<float>(new_y)))[new_x];
                    float gy1=((float *)(gradientY.ptr<float>(new_y)))[new_x];

                    //unit vectors along the gradient
                    float mag1=sqrt((gx1*gx1)+(gy1*gy1));
                    gx1= gx1/mag1;
                    gy1= gy1/mag1;

                    gx1=k==0?-gx1:gx1;
                    gy1=k==0?-gy1:gy1;

                    //computing angle between vector
                    float angle=acos((gx1 * -gx)+(gy1 * -gy));
                    float angle1=acos((-gx1*-gx)+(-gy1*-gy));
                    //comparing the angle to a threshold
                    //cerr << new_x <<":" << new_y << gx <<":" <<gx1 << ":" << angle << endl;
                    if((abs(angle) < PI/6 ))
                    {


                    //setting the stroke width for all the elements alongt the ray
                    for(std::vector<Point2d1>::iterator pit = points.begin(); pit != points.end(); pit++) {
                    Point2d1 pit1=*pit;

                    float *c=ELEM(float,swtimage.data,swtimage.step,swtimage.elemSize(),pit1.y,pit1.x);
                    int swtv=(int)c[0];
                    if(swtv<0)
                    {
                            swtv=length;
                    }
                    else
                    {
                           swtv=std::min((double)swtv,(double)length);

                    }
                    pit->SWT=swtv;
                    }
                    r[k].points=points;

                }
                break;

                }


            }
            //if(r[0].points.size()!=0)
            //rays.push_back(r[0]);

            if(r[0].points.size()==0 && r[1].points.size()==0)
                continue;

            Ray final;
            final=r[0];

            if(r[0].points.size()==0 && r[1].points.size()>0)
                final=(r[1]);
            if(r[1].points.size()==0 && r[0].points.size()>0)
                final=(r[0]);
            if(r[1].points.size()>0 && r[0].points.size()>0)
            {

                if(r[1].points[0].SWT <=r[0].points[0].SWT)
                {
                    final=(r[1]);
                }
                else
                {
                    final=(r[0]);
                }

            }

                for (std::vector<Point2d1>::iterator pit = final.points.begin(); pit != final.points.end(); pit++) {
                    //updte the values of SWT
                    float *c=(ELEM(float,swtimage.data,swtimage.step,swtimage.elemSize(),pit->y,pit->x));
                    if((int)c[0]<0)
                    c[0]=pit->SWT;
                    else
                    c[0]=std::min((double)c[0],(double)pit->SWT);
              }
                rays.push_back(final);


        //



    }


}


    //withing each ray contains a sequence of points.
    //sort the points as per the SWT length values
    //compute the value of median of points
    //compare this with the current SWTimage value and replace
    //with minimum
    //this acts like a median filter for SWt image

    for (std::vector<Ray>::iterator rit = rays.begin(); rit != rays.end(); rit++) {
        for (std::vector<Point2d1>::iterator pit = rit->points.begin(); pit != rit->points.end(); pit++) {
            //updte the values of SWT
            pit->SWT = ELEM(uchar,swtimage.data,swtimage.step,swtimage.elemSize(),pit->y,pit->x)[0];
        }
        //sort the values
        std::sort(rit->points.begin(), rit->points.end(), &Point2dSort);
        //compute the median
        float median = (rit->points[rit->points.size()/2]).SWT;
        for (std::vector<Point2d1>::iterator pit = rit->points.begin(); pit != rit->points.end(); pit++) {
            *ELEM(uchar,swtimage.data,swtimage.step,swtimage.elemSize(),pit->y,pit->x)=std::min((double)*ELEM(uchar,swtimage.data,swtimage.step,swtimage.elemSize(),pit->y,pit->x),(double)median);

        }

    }
    //connected component analysis is performed to group pixels with similar SWT length values
    IplImage sw=IplImage(swtimage);
    imshow("D1",swtimage);
    std::vector<std::vector<Point2d1> > components =findLegallyConnectedComponents(&sw,rays);
    Mat swt1=Mat(&sw);
    swt1.copyTo(swtimage);
    //cv::waitKey(0);

}


void strokeWidthTransform (IplImage * edgeImage,
                           IplImage * gradientX,
                           IplImage * gradientY,
                           bool dark_on_light,
                           IplImage * SWTImage,
                           std::vector<Ray> & rays) {
    // First pass
    float prec = .05;
    for( int row = 0; row < edgeImage->height; row++ ){
        const uchar* ptr = (const uchar*)(edgeImage->imageData + row * edgeImage->widthStep);
        for ( int col = 0; col < edgeImage->width; col++ ){
            if (*ptr > 0) {
                Ray r;

                Point2d1 p;
                p.x = col;
                p.y = row;
                r.p = p;
                std::vector<Point2d1> points;
                points.push_back(p);

                float curX = (float)col + 0.5;
                float curY = (float)row + 0.5;
                int curPixX = col;
                int curPixY = row;
                float G_x = CV_IMAGE_ELEM ( gradientX, float, row, col);
                float G_y = CV_IMAGE_ELEM ( gradientY, float, row, col);
                // normalize gradient
                float mag = sqrt( (G_x * G_x) + (G_y * G_y) );
                if (dark_on_light){
                    G_x = -G_x/mag;
                    G_y = -G_y/mag;
                } else {
                    G_x = G_x/mag;
                    G_y = G_y/mag;

                }
                while (true) {
                    curX += G_x*prec;
                    curY += G_y*prec;
                    if ((int)(floor(curX)) != curPixX || (int)(floor(curY)) != curPixY) {
                        curPixX = (int)(floor(curX));
                        curPixY = (int)(floor(curY));
                        // check if pixel is outside boundary of image
                        if (curPixX < 0 || (curPixX >= SWTImage->width) || curPixY < 0 || (curPixY >= SWTImage->height)) {
                            break;
                        }
                        Point2d1 pnew;
                        pnew.x = curPixX;
                        pnew.y = curPixY;
                        points.push_back(pnew);

                        if (CV_IMAGE_ELEM ( edgeImage, uchar, curPixY, curPixX) > 0) {
                            r.q = pnew;
                            // dot product
                            float G_xt = CV_IMAGE_ELEM(gradientX,float,curPixY,curPixX);
                            float G_yt = CV_IMAGE_ELEM(gradientY,float,curPixY,curPixX);
                            mag = sqrt( (G_xt * G_xt) + (G_yt * G_yt) );
                            if (dark_on_light){
                                G_xt = -G_xt/mag;
                                G_yt = -G_yt/mag;
                            } else {
                                G_xt = G_xt/mag;
                                G_yt = G_yt/mag;

                            }

                            if (abs(acos(G_x * -G_xt + G_y * -G_yt)) < 30*PI/180 ) {
                                float length = sqrt( ((float)r.q.x - (float)r.p.x)*((float)r.q.x - (float)r.p.x) + ((float)r.q.y - (float)r.p.y)*((float)r.q.y - (float)r.p.y));
                                for (std::vector<Point2d1>::iterator pit = points.begin(); pit != points.end(); pit++) {
                                    Point2d1 pit1=*pit;
                                    if (CV_IMAGE_ELEM(SWTImage, float, (int)pit1.y, (int)pit1.x) < 0) {
                                        CV_IMAGE_ELEM(SWTImage, float,(int) pit1.y, (int)pit1.x) = length;
                                    } else {
                                        CV_IMAGE_ELEM(SWTImage, float, (int)pit1.y, (int)pit1.x) =
                                                std::min(length, CV_IMAGE_ELEM(SWTImage, float,(int) pit1.y,(int) pit1.x));
                                    }
                                    (*pit).SWT=CV_IMAGE_ELEM(SWTImage, float, pit->y, pit->x);
                                }
                                r.points = points;
                                rays.push_back(r);
                            }
                            break;
                        }
                    }
                }
            }
            ptr++;
        }
    }


}



void SWTMedianFilter (IplImage * SWTImage,
                     std::vector<Ray> & rays) {
    for (std::vector<Ray>::iterator rit = rays.begin(); rit != rays.end(); rit++) {
        for (std::vector<Point2d1>::iterator pit = rit->points.begin(); pit != rit->points.end(); pit++) {



            pit->SWT = CV_IMAGE_ELEM(SWTImage, float, (int)pit->y, (int)pit->x);
        }
        std::sort(rit->points.begin(), rit->points.end(), &Point2dSort);
        float median = (rit->points[rit->points.size()/2]).SWT;
        for (std::vector<Point2d1>::iterator pit = rit->points.begin(); pit != rit->points.end(); pit++) {
            CV_IMAGE_ELEM(SWTImage, float, (int)pit->y, (int)pit->x) = std::min(pit->SWT, median);
        }
    }

}


void renderComponents (IplImage * SWTImage, std::vector<std::vector<Point2d1> > & components, IplImage * output) {
    for (std::vector<std::vector<Point2d1> >::iterator it = components.begin(); it != components.end();it++) {
        for (std::vector<Point2d1>::iterator pit = it->begin(); pit != it->end(); pit++) {
            CV_IMAGE_ELEM(output, float, pit->y, pit->x) = CV_IMAGE_ELEM(SWTImage, float, pit->y, pit->x);
        }
    }
    for( int row = 0; row < output->height; row++ ){
        float* ptr = (float*)(output->imageData + row * output->widthStep);
        for ( int col = 0; col < output->width; col++ ){
            if (*ptr == 0) {
                *ptr = -1;
            }
            ptr++;
        }
    }
    float maxVal = 0;
    float minVal = 1e100;
    for( int row = 0; row < output->height; row++ ){
        const float* ptr = (const float*)(output->imageData + row * output->widthStep);
        for ( int col = 0; col < output->width; col++ ){
            if (*ptr == 0) { }
            else {
                maxVal = std::max(*ptr, maxVal);
                minVal = std::min(*ptr, minVal);
            }
            ptr++;
        }
    }
    float difference = maxVal - minVal;
    for( int row = 0; row < output->height; row++ ){
        float* ptr = (float*)(output->imageData + row * output->widthStep);\
        for ( int col = 0; col < output->width; col++ ){
            if (*ptr < 1) {
                *ptr = 1;
            } else {
                *ptr = ((*ptr) - minVal)/difference;
            }
            ptr++;
        }
    }

}


void renderComponentsWithBoxes (IplImage * SWTImage, std::vector<std::vector<Point2d1> > & components,
                                std::vector<std::pair<Point2d1,Point2d1> > & compBB, IplImage * output) {
    IplImage * outTemp =
            cvCreateImage ( cvGetSize ( output ), IPL_DEPTH_32F, 1 );

    renderComponents(SWTImage,components,outTemp);
    std::vector<std::pair<CvPoint,CvPoint> > bb;
    bb.reserve(compBB.size());
    for (std::vector<std::pair<Point2d1,Point2d1> >::iterator it=compBB.begin(); it != compBB.end(); it++ ) {
        CvPoint p0 = cvPoint(it->first.x,it->first.y);
        CvPoint p1 = cvPoint(it->second.x,it->second.y);
        std::pair<CvPoint,CvPoint> pair(p0,p1);
        bb.push_back(pair);
    }

    IplImage * out =
            cvCreateImage ( cvGetSize ( output ), IPL_DEPTH_8U, 1 );
    cvConvertScale(outTemp, out, 255, 0);
    cvCvtColor (out, output, CV_GRAY2RGB);
    //cvReleaseImage ( &outTemp );
    //cvReleaseImage ( &out );

    int count = 0;
    for (std::vector<std::pair<CvPoint,CvPoint> >::iterator it= bb.begin(); it != bb.end(); it++) {
        CvScalar c;
        if (count % 3 == 0) c=cvScalar(255,0,0);
        else if (count % 3 == 1) c=cvScalar(0,255,0);
        else c=cvScalar(0,0,255);
        count++;
        cvRectangle(output,it->first,it->second,c,2);
    }
}

struct Point3dFloat {
    float x;
    float y;
    float z;
};


struct Chain {
    int p;
    int q;
    float dist;
    bool merged;
    Point2dFloat direction;
    std::vector<int> components;
};


std::vector<std::pair<CvPoint,CvPoint> > findBoundingBoxes( std::vector<std::vector<Point2d1> > & components,
                                                           std::vector<Chain> & chains,
                                                           std::vector<std::pair<Point2d1,Point2d1> > & compBB,
                                                           IplImage * output) {
    std::vector<std::pair<CvPoint,CvPoint> > bb;
    bb.reserve(chains.size());
    for (std::vector<Chain>::iterator chainit = chains.begin(); chainit != chains.end(); chainit++) {
        int minx = output->width;
        int miny = output->height;
        int maxx = 0;
        int maxy = 0;
        for (std::vector<int>::const_iterator cit = chainit->components.begin(); cit != chainit->components.end(); cit++) {
                miny = std::min(miny,compBB[*cit].first.y);
                minx = std::min(minx,compBB[*cit].first.x);
                maxy = std::max(maxy,compBB[*cit].second.y);
                maxx = std::max(maxx,compBB[*cit].second.x);
        }
        CvPoint p0 = cvPoint(minx,miny);
        CvPoint p1 = cvPoint(maxx,maxy);
        std::pair<CvPoint,CvPoint> pair(p0,p1);
        bb.push_back(pair);
    }
    return bb;
}

std::vector<std::pair<CvPoint,CvPoint> > findBoundingBoxes( std::vector<std::vector<Point2d1> > & components,
                                                           IplImage * output) {
    std::vector<std::pair<CvPoint,CvPoint> > bb;
    bb.reserve(components.size());
    for (std::vector<std::vector<Point2d1> >::iterator compit = components.begin(); compit != components.end(); compit++) {
        int minx = output->width;
        int miny = output->height;
        int maxx = 0;
        int maxy = 0;
        for (std::vector<Point2d1>::iterator it = compit->begin(); it != compit->end(); it++) {
                miny = std::min(miny,(int)it->y);
                minx = std::min(minx,(int)it->x);
                maxy = std::max(maxy,(int)it->y);
                maxx = std::max(maxx,(int)it->x);
        }
        CvPoint p0 = cvPoint(minx,miny);
        CvPoint p1 = cvPoint(maxx,maxy);
        std::pair<CvPoint,CvPoint> pair(p0,p1);
        bb.push_back(pair);
    }
    return bb;
}


bool sharesOneEnd( Chain c0, Chain c1) {
    if (c0.p == c1.p || c0.p == c1.q || c0.q == c1.q || c0.q == c1.p) {
        return true;
    }
    else {
        return false;
    }
}

bool chainSortDist (const Chain &lhs, const Chain &rhs) {
    return lhs.dist < rhs.dist;
}

bool chainSortLength (const Chain &lhs, const Chain &rhs) {
    return lhs.components.size() > rhs.components.size();
}

std::vector<Chain> makeChains( IplImage * colorImage,
                 std::vector<std::vector<Point2d1> > & components,
                 std::vector<Point2dFloat> & compCenters,
                 std::vector<float> & compMedians,
                 std::vector<Point2d1> & compDimensions,
                 std::vector<std::pair<Point2d1,Point2d1> > & compBB) {
    assert (compCenters.size() == components.size());
    // make vector of color averages
    std::vector<Point3dFloat> colorAverages;
    colorAverages.reserve(components.size());
    for (std::vector<std::vector<Point2d1> >::iterator it = components.begin(); it != components.end();it++) {
        Point3dFloat mean;
        mean.x = 0;
        mean.y = 0;
        mean.z = 0;
        int num_points = 0;
        for (std::vector<Point2d1>::iterator pit = it->begin(); pit != it->end(); pit++) {
            mean.x += (float) CV_IMAGE_ELEM (colorImage, unsigned char, (int)pit->y, (pit->x)*3 );
            mean.y += (float) CV_IMAGE_ELEM (colorImage, unsigned char, (int) pit->y, (pit->x)*3+1 );
            mean.z += (float) CV_IMAGE_ELEM (colorImage, unsigned char,  (int)pit->y, (pit->x)*3+2 );
            num_points++;
        }
        mean.x = mean.x / ((float)num_points);
        mean.y = mean.y / ((float)num_points);
        mean.z = mean.z / ((float)num_points);
        colorAverages.push_back(mean);
    }

    // form all eligible pairs and calculate the direction of each
    std::vector<Chain> chains;
    for ( unsigned int i = 0; i < components.size() - 1; i++ ) {
        for ( unsigned int j = i + 1; j < components.size(); j++ ) {
            // TODO add color metric
            if ( (compMedians[i]/compMedians[j] <= 2.0 || compMedians[j]/compMedians[i] <= 2.0) &&
                 (compDimensions[i].y/compDimensions[j].y <= 2.0 || compDimensions[j].y/compDimensions[i].y <= 2.0)) {
                float dist = (compCenters[i].x - compCenters[j].x) * (compCenters[i].x - compCenters[j].x) +
                             (compCenters[i].y - compCenters[j].y) * (compCenters[i].y - compCenters[j].y);
                float colorDist = (colorAverages[i].x - colorAverages[j].x) * (colorAverages[i].x - colorAverages[j].x) +
                                  (colorAverages[i].y - colorAverages[j].y) * (colorAverages[i].y - colorAverages[j].y) +
                                  (colorAverages[i].z - colorAverages[j].z) * (colorAverages[i].z - colorAverages[j].z);
                if (dist < 9*(float)(std::max(std::min(compDimensions[i].x,compDimensions[i].y),std::min(compDimensions[j].x,compDimensions[j].y)))
                    *(float)(std::max(std::min(compDimensions[i].x,compDimensions[i].y),std::min(compDimensions[j].x,compDimensions[j].y)))
                    && colorDist < 1600) {
                    Chain c;
                    c.p = i;
                    c.q = j;
                    std::vector<int> comps;
                    comps.push_back(c.p);
                    comps.push_back(c.q);
                    c.components = comps;
                    c.dist = dist;
                    float d_x = (compCenters[i].x - compCenters[j].x);
                    float d_y = (compCenters[i].y - compCenters[j].y);
                    /*
                    float d_x = (compBB[i].first.x - compBB[j].second.x);
                    float d_y = (compBB[i].second.y - compBB[j].second.y);
                    */
                    float mag = sqrt(d_x*d_x + d_y*d_y);
                    d_x = d_x / mag;
                    d_y = d_y / mag;
                    Point2dFloat dir;
                    dir.x = d_x;
                    dir.y = d_y;
                    c.direction = dir;
                    chains.push_back(c);

                    /*std::cerr << c.p << " " << c.q << std::endl;
                    std::cerr << c.direction.x << " " << c.direction.y << std::endl;
                    std::cerr << compCenters[c.p].x << " " << compCenters[c.p].y << std::endl;
                    std::cerr << compCenters[c.q].x << " " << compCenters[c.q].y << std::endl;
                    std::cerr << std::endl;
                    std::cerr << colorDist << std::endl; */
                }
            }
        }
    }
    std::cout << chains.size() << " eligible pairs" << std::endl;
    std::sort(chains.begin(), chains.end(), &chainSortDist);

    std::cerr << std::endl;
    const float strictness = PI/6.0;
    //merge chains
    int merges = 1;
    while (merges > 0) {
        for (unsigned int i = 0; i < chains.size(); i++) {
            chains[i].merged = false;
        }
        merges = 0;
        std::vector<Chain> newchains;
        for (unsigned int i = 0; i < chains.size(); i++) {
            for (unsigned int j = 0; j < chains.size(); j++) {
                if (i != j) {
                    if (!chains[i].merged && !chains[j].merged && sharesOneEnd(chains[i],chains[j])) {
                        if (chains[i].p == chains[j].p) {
                            if (acos(chains[i].direction.x * -chains[j].direction.x + chains[i].direction.y * -chains[j].direction.y) < strictness) {
                                  /*      if (chains[i].p == chains[i].q || chains[j].p == chains[j].q) {
                                            std::cout << "CRAZY ERROR" << std::endl;
                                        } else if (chains[i].p == chains[j].p && chains[i].q == chains[j].q) {
                                            std::cout << "CRAZY ERROR" << std::endl;
                                        } else if (chains[i].p == chains[j].q && chains[i].q == chains[j].p) {
                                            std::cout << "CRAZY ERROR" << std::endl;
                                        }
                                        std::cerr << 1 <<std::endl;

                                        std::cerr << chains[i].p << " " << chains[i].q << std::endl;
                                        std::cerr << chains[j].p << " " << chains[j].q << std::endl;
                                std::cerr << compCenters[chains[i].q].x << " " << compCenters[chains[i].q].y << std::endl;
                                std::cerr << compCenters[chains[i].p].x << " " << compCenters[chains[i].p].y << std::endl;
                                std::cerr << compCenters[chains[j].q].x << " " << compCenters[chains[j].q].y << std::endl;
                                std::cerr << std::endl; */

                                chains[i].p = chains[j].q;
                                for (std::vector<int>::iterator it = chains[j].components.begin(); it != chains[j].components.end(); it++) {
                                    chains[i].components.push_back(*it);
                                }
                                float d_x = (compCenters[chains[i].p].x - compCenters[chains[i].q].x);
                                float d_y = (compCenters[chains[i].p].y - compCenters[chains[i].q].y);
                                chains[i].dist = d_x * d_x + d_y * d_y;

                                float mag = sqrt(d_x*d_x + d_y*d_y);
                                d_x = d_x / mag;
                                d_y = d_y / mag;
                                Point2dFloat dir;
                                dir.x = d_x;
                                dir.y = d_y;
                                chains[i].direction = dir;
                                chains[j].merged = true;
                                merges++;
                                /*j=-1;
                                i=0;
                                if (i == chains.size() - 1) i=-1;
                                std::stable_sort(chains.begin(), chains.end(), &chainSortLength);*/
                            }
                        } else if (chains[i].p == chains[j].q) {
                            if (acos(chains[i].direction.x * chains[j].direction.x + chains[i].direction.y * chains[j].direction.y) < strictness) {
/*
                                if (chains[i].p == chains[i].q || chains[j].p == chains[j].q) {
                                    std::cout << "CRAZY ERROR" << std::endl;
                                } else if (chains[i].p == chains[j].p && chains[i].q == chains[j].q) {
                                    std::cout << "CRAZY ERROR" << std::endl;
                                } else if (chains[i].p == chains[j].q && chains[i].q == chains[j].p) {
                                    std::cout << "CRAZY ERROR" << std::endl;
                                }
                                std::cerr << 2 <<std::endl;

                                std::cerr << chains[i].p << " " << chains[i].q << std::endl;
                                std::cerr << chains[j].p << " " << chains[j].q << std::endl;
                                std::cerr << chains[i].direction.x << " " << chains[i].direction.y << std::endl;
                                std::cerr << chains[j].direction.x << " " << chains[j].direction.y << std::endl;
                                std::cerr << compCenters[chains[i].q].x << " " << compCenters[chains[i].q].y << std::endl;
                                std::cerr << compCenters[chains[i].p].x << " " << compCenters[chains[i].p].y << std::endl;
                                std::cerr << compCenters[chains[j].p].x << " " << compCenters[chains[j].p].y << std::endl;
                                std::cerr << std::endl; */

                                chains[i].p = chains[j].p;
                                for (std::vector<int>::iterator it = chains[j].components.begin(); it != chains[j].components.end(); it++) {
                                    chains[i].components.push_back(*it);
                                }
                                float d_x = (compCenters[chains[i].p].x - compCenters[chains[i].q].x);
                                float d_y = (compCenters[chains[i].p].y - compCenters[chains[i].q].y);
                                float mag = sqrt(d_x*d_x + d_y*d_y);
                                chains[i].dist = d_x * d_x + d_y * d_y;

                                d_x = d_x / mag;
                                d_y = d_y / mag;

                                Point2dFloat dir;
                                dir.x = d_x;
                                dir.y = d_y;
                                chains[i].direction = dir;
                                chains[j].merged = true;
                                merges++;
                                /*j=-1;
                                i=0;
                                if (i == chains.size() - 1) i=-1;
                                std::stable_sort(chains.begin(), chains.end(), &chainSortLength); */
                            }
                        } else if (chains[i].q == chains[j].p) {
                            if (acos(chains[i].direction.x * chains[j].direction.x + chains[i].direction.y * chains[j].direction.y) < strictness) {
     /*                           if (chains[i].p == chains[i].q || chains[j].p == chains[j].q) {
                                    std::cout << "CRAZY ERROR" << std::endl;
                                } else if (chains[i].p == chains[j].p && chains[i].q == chains[j].q) {
                                    std::cout << "CRAZY ERROR" << std::endl;
                                } else if (chains[i].p == chains[j].q && chains[i].q == chains[j].p) {
                                    std::cout << "CRAZY ERROR" << std::endl;
                                }
                                std::cerr << 3 <<std::endl;

                                std::cerr << chains[i].p << " " << chains[i].q << std::endl;
                                std::cerr << chains[j].p << " " << chains[j].q << std::endl;

                                std::cerr << compCenters[chains[i].p].x << " " << compCenters[chains[i].p].y << std::endl;
                                std::cerr << compCenters[chains[i].q].x << " " << compCenters[chains[i].q].y << std::endl;
                                std::cerr << compCenters[chains[j].q].x << " " << compCenters[chains[j].q].y << std::endl;
                                std::cerr << std::endl; */
                                chains[i].q = chains[j].q;
                                for (std::vector<int>::iterator it = chains[j].components.begin(); it != chains[j].components.end(); it++) {
                                    chains[i].components.push_back(*it);
                                }
                                float d_x = (compCenters[chains[i].p].x - compCenters[chains[i].q].x);
                                float d_y = (compCenters[chains[i].p].y - compCenters[chains[i].q].y);
                                float mag = sqrt(d_x*d_x + d_y*d_y);
                                chains[i].dist = d_x * d_x + d_y * d_y;


                                d_x = d_x / mag;
                                d_y = d_y / mag;
                                Point2dFloat dir;
                                dir.x = d_x;
                                dir.y = d_y;

                                chains[i].direction = dir;
                                chains[j].merged = true;
                                merges++;
                                /*j=-1;
                                i=0;
                                if (i == chains.size() - 1) i=-1;
                                std::stable_sort(chains.begin(), chains.end(), &chainSortLength); */
                            }
                        } else if (chains[i].q == chains[j].q) {
                            if (acos(chains[i].direction.x * -chains[j].direction.x + chains[i].direction.y * -chains[j].direction.y) < strictness) {
                     /*           if (chains[i].p == chains[i].q || chains[j].p == chains[j].q) {
                                    std::cout << "CRAZY ERROR" << std::endl;
                                } else if (chains[i].p == chains[j].p && chains[i].q == chains[j].q) {
                                    std::cout << "CRAZY ERROR" << std::endl;
                                } else if (chains[i].p == chains[j].q && chains[i].q == chains[j].p) {
                                    std::cout << "CRAZY ERROR" << std::endl;
                                }
                                std::cerr << 4 <<std::endl;
                                std::cerr << chains[i].p << " " << chains[i].q << std::endl;
                                std::cerr << chains[j].p << " " << chains[j].q << std::endl;
                                std::cerr << compCenters[chains[i].p].x << " " << compCenters[chains[i].p].y << std::endl;
                                std::cerr << compCenters[chains[i].q].x << " " << compCenters[chains[i].q].y << std::endl;
                                std::cerr << compCenters[chains[j].p].x << " " << compCenters[chains[j].p].y << std::endl;
                                std::cerr << std::endl; */
                                chains[i].q = chains[j].p;
                                for (std::vector<int>::iterator it = chains[j].components.begin(); it != chains[j].components.end(); it++) {
                                    chains[i].components.push_back(*it);
                                }
                                float d_x = (compCenters[chains[i].p].x - compCenters[chains[i].q].x);
                                float d_y = (compCenters[chains[i].p].y - compCenters[chains[i].q].y);
                                chains[i].dist = d_x * d_x + d_y * d_y;

                                float mag = sqrt(d_x*d_x + d_y*d_y);
                                d_x = d_x / mag;
                                d_y = d_y / mag;
                                Point2dFloat dir;
                                dir.x = d_x;
                                dir.y = d_y;
                                chains[i].direction = dir;
                                chains[j].merged = true;
                                merges++;
                                /*j=-1;
                                i=0;
                                if (i == chains.size() - 1) i=-1;
                                std::stable_sort(chains.begin(), chains.end(), &chainSortLength);*/
                            }
                        }
                    }
                }
            }
        }
        for (unsigned int i = 0; i < chains.size(); i++) {
            if (!chains[i].merged) {
                newchains.push_back(chains[i]);
            }
        }
        chains = newchains;
        std::stable_sort(chains.begin(), chains.end(), &chainSortLength);
    }

    std::vector<Chain> newchains;
    newchains.reserve(chains.size());
    for (std::vector<Chain>::iterator cit = chains.begin(); cit != chains.end(); cit++) {
        if (cit->components.size() >= 3) {
            newchains.push_back(*cit);
        }
    }
    chains = newchains;
    std::cout << chains.size() << " chains after merging" << std::endl;
    return chains;
}


void renderChainsWithBoxes (IplImage * SWTImage,
                   std::vector<std::vector<Point2d1> > & components,
                   std::vector<Chain> & chains,
                   std::vector<std::pair<Point2d1,Point2d1> > & compBB,
                   IplImage * output) {
    // keep track of included components
    std::vector<bool> included;
    included.reserve(components.size());
    for (unsigned int i = 0; i != components.size(); i++) {
        included.push_back(false);
    }
    for (std::vector<Chain>::iterator it = chains.begin(); it != chains.end();it++) {
        for (std::vector<int>::iterator cit = it->components.begin(); cit != it->components.end(); cit++) {
            included[*cit] = true;
        }
    }
    std::vector<std::vector<Point2d1> > componentsRed;
    for (unsigned int i = 0; i != components.size(); i++ ) {
        if (included[i]) {
            componentsRed.push_back(components[i]);
        }
    }
    IplImage * outTemp =
            cvCreateImage ( cvGetSize ( output ), IPL_DEPTH_32F, 1 );

    std::cout << componentsRed.size() << " components after chaining" << std::endl;
    renderComponents(SWTImage,componentsRed,outTemp);
    std::vector<std::pair<CvPoint,CvPoint> > bb;
    bb = findBoundingBoxes(components, chains, compBB, outTemp);

    IplImage * out =
            cvCreateImage ( cvGetSize ( output ), IPL_DEPTH_8U, 1 );
    cvConvertScale(outTemp, out, 255, 0);
    cvCvtColor (out, output, CV_GRAY2RGB);
    cvReleaseImage ( &out );
    cvReleaseImage ( &outTemp);

    int count = 0;
    for (std::vector<std::pair<CvPoint,CvPoint> >::iterator it= bb.begin(); it != bb.end(); it++) {
        CvScalar c;
        if (count % 3 == 0) c=cvScalar(255,0,0);
        else if (count % 3 == 1) c=cvScalar(0,255,0);
        else c=cvScalar(0,0,255);
        count++;
        cvRectangle(output,it->first,it->second,c,2);
    }
}

void renderChains (IplImage * SWTImage,
                   std::vector<std::vector<Point2d1> > & components,
                   std::vector<Chain> & chains,
                   IplImage * output) {
    // keep track of included components
    std::vector<bool> included;
    included.reserve(components.size());
    for (unsigned int i = 0; i != components.size(); i++) {
        included.push_back(false);
    }
    for (std::vector<Chain>::iterator it = chains.begin(); it != chains.end();it++) {
        for (std::vector<int>::iterator cit = it->components.begin(); cit != it->components.end(); cit++) {
            included[*cit] = true;
        }
    }
    std::vector<std::vector<Point2d1> > componentsRed;
    for (unsigned int i = 0; i != components.size(); i++ ) {
        if (included[i]) {
            componentsRed.push_back(components[i]);
        }
    }
    std::cout << componentsRed.size() << " components after chaining" << std::endl;
    IplImage * outTemp =
            cvCreateImage ( cvGetSize ( output ), IPL_DEPTH_32F, 1 );
    renderComponents(SWTImage,componentsRed,outTemp);
    cvConvertScale(outTemp, output, 255, 0);

}


void normalizeImage (IplImage * input, IplImage * output) {
    assert ( input->depth == IPL_DEPTH_32F );
    assert ( input->nChannels == 1 );
    assert ( output->depth == IPL_DEPTH_32F );
    assert ( output->nChannels == 1 );
    float maxVal = 0;
    float minVal = 1e100;
    for( int row = 0; row < input->height; row++ ){
        const float* ptr = (const float*)(input->imageData + row * input->widthStep);
        for ( int col = 0; col < input->width; col++ ){
            if (*ptr < 0) { }
            else {
                maxVal = std::max(*ptr, maxVal);
                minVal = std::min(*ptr, minVal);
            }
            ptr++;
        }
    }

    float difference = maxVal - minVal;
    for( int row = 0; row < input->height; row++ ){
        const float* ptrin = (const float*)(input->imageData + row * input->widthStep);\
        float* ptrout = (float*)(output->imageData + row * output->widthStep);\
        for ( int col = 0; col < input->width; col++ ){
            if (*ptrin < 0) {
                *ptrout = 1;
            } else {
                *ptrout = ((*ptrin) - minVal)/difference;
            }
            ptrout++;
            ptrin++;
        }
    }
}


void componentStats(IplImage * SWTImage,
                                        const std::vector<Point2d1> & component,
                                        float & mean, float & variance, float & median,
                                        int & minx, int & miny, int & maxx, int & maxy)
{
        std::vector<float> temp;
        temp.reserve(component.size());
        mean = 0;
        variance = 0;
        minx = 1000000;
        miny = 1000000;
        maxx = 0;
        maxy = 0;
        for (std::vector<Point2d1>::const_iterator it = component.begin(); it != component.end(); it++) {
                float t = CV_IMAGE_ELEM(SWTImage, float, (int)it->y, (int)it->x);
                mean += t;
                temp.push_back(t);
                miny = std::min(miny,(int)it->y);
                minx = std::min(minx,(int)it->x);
                maxy = std::max(maxy,(int)it->y);
                maxx = std::max(maxx,(int)it->x);
        }
        mean = mean / ((float)component.size());
        for (std::vector<float>::const_iterator it = temp.begin(); it != temp.end(); it++) {
            variance += (*it - mean) * (*it - mean);
        }
        variance = variance / ((float)component.size());
        std::sort(temp.begin(),temp.end());
        median = temp[temp.size()/2];
}


void filterComponents(IplImage * SWTImage,
                      std::vector<std::vector<Point2d1> > & components,
                      std::vector<std::vector<Point2d1> > & validComponents,
                      std::vector<Point2dFloat> & compCenters,
                      std::vector<float> & compMedians,
                      std::vector<Point2d1> & compDimensions,
                      std::vector<std::pair<Point2d1,Point2d1> > & compBB )
{
        validComponents.reserve(components.size());
        compCenters.reserve(components.size());
        compMedians.reserve(components.size());
        compDimensions.reserve(components.size());
        // bounding boxes
        compBB.reserve(components.size());
        for (std::vector<std::vector<Point2d1> >::iterator it = components.begin(); it != components.end();it++) {
            // compute the stroke width mean, variance, median
            float mean, variance, median;
            int minx, miny, maxx, maxy;
            componentStats(SWTImage, (*it), mean, variance, median, minx, miny, maxx, maxy);

            // check if variance is less than half the mean
            if (variance > 0.5 * mean) {
                 continue;
            }

            float length = (float)(maxx-minx+1);
            float width = (float)(maxy-miny+1);

            // check font height
            if (width > 300) {
                continue;
            }

            float area = length * width;
            float rminx = (float)minx;
            float rmaxx = (float)maxx;
            float rminy = (float)miny;
            float rmaxy = (float)maxy;
            // compute the rotated bounding box
            float increment = 1./36.;
            for (float theta = increment * PI; theta<PI/2.0; theta += increment * PI) {
                float xmin,xmax,ymin,ymax,xtemp,ytemp,ltemp,wtemp;
                    xmin = 1000000;
                    ymin = 1000000;
                    xmax = 0;
                    ymax = 0;
                for (unsigned int i = 0; i < (*it).size(); i++) {
                    xtemp = (*it)[i].x * cos(theta) + (*it)[i].y * -sin(theta);
                    ytemp = (*it)[i].x * sin(theta) + (*it)[i].y * cos(theta);
                    xmin = std::min(xtemp,xmin);
                    xmax = std::max(xtemp,xmax);
                    ymin = std::min(ytemp,ymin);
                    ymax = std::max(ytemp,ymax);
                }
                ltemp = xmax - xmin + 1;
                wtemp = ymax - ymin + 1;
                if (ltemp*wtemp < area) {
                    area = ltemp*wtemp;
                    length = ltemp;
                    width = wtemp;
                }
            }
            // check if the aspect ratio is between 1/10 and 10
            if (length/width < 1./10. || length/width > 10.) {
                continue;
            }

            // compute the diameter TODO finish
            // compute dense representation of component
            std::vector <std::vector<float> > denseRepr;
            denseRepr.reserve(maxx-minx+1);
            for (int i = 0; i < maxx-minx+1; i++) {
                std::vector<float> tmp;
                tmp.reserve(maxy-miny+1);
                denseRepr.push_back(tmp);
                for (int j = 0; j < maxy-miny+1; j++) {\
                    denseRepr[i].push_back(0);
                }
            }
            for (std::vector<Point2d1>::iterator pit = it->begin(); pit != it->end(); pit++) {
                (denseRepr[pit->x - minx])[pit->y - miny] = 1;
            }
            // create graph representing components
            const int num_nodes = it->size();
            /*
            E edges[] = { E(0,2),
                          E(1,1), E(1,3), E(1,4),
                          E(2,1), E(2,3),
                          E(3,4),
                          E(4,0), E(4,1) };

            Graph G(edges + sizeof(edges) / sizeof(E), weights, num_nodes);
            */
            Point2dFloat center;
            center.x = ((float)(maxx+minx))/2.0;
            center.y = ((float)(maxy+miny))/2.0;

            Point2d1 dimensions;
            dimensions.x = maxx - minx + 1;
            dimensions.y = maxy - miny + 1;

            Point2d1 bb1;
            bb1.x = minx;
            bb1.y = miny;

            Point2d1 bb2;
            bb2.x = maxx;
            bb2.y = maxy;
            std::pair<Point2d1, Point2d1> pair(bb1,bb2);

            compBB.push_back(pair);
            compDimensions.push_back(dimensions);
            compMedians.push_back(median);
            compCenters.push_back(center);
            validComponents.push_back(*it);
        }
       std::vector<std::vector<Point2d1 > > tempComp;
       std::vector<Point2d1 > tempDim;
       std::vector<float > tempMed;
       std::vector<Point2dFloat > tempCenters;
       std::vector<std::pair<Point2d1,Point2d1> > tempBB;
       tempComp.reserve(validComponents.size());
       tempCenters.reserve(validComponents.size());
       tempDim.reserve(validComponents.size());
       tempMed.reserve(validComponents.size());
       tempBB.reserve(validComponents.size());
       for (unsigned int i = 0; i < validComponents.size(); i++) {
            int count = 0;
            for (unsigned int j = 0; j < validComponents.size(); j++) {
                if (i != j) {
                    if (compBB[i].first.x <= compCenters[j].x && compBB[i].second.x >= compCenters[j].x &&
                        compBB[i].first.y <= compCenters[j].y && compBB[i].second.y >= compCenters[j].y) {
                        count++;
                    }
                }
            }
            if (count < 2) {
                tempComp.push_back(validComponents[i]);
                tempCenters.push_back(compCenters[i]);
                tempMed.push_back(compMedians[i]);
                tempDim.push_back(compDimensions[i]);
                tempBB.push_back(compBB[i]);
            }
        }
        validComponents = tempComp;
        compDimensions = tempDim;
        compMedians = tempMed;
        compCenters = tempCenters;
        compBB = tempBB;

        compDimensions.reserve(tempComp.size());
        compMedians.reserve(tempComp.size());
        compCenters.reserve(tempComp.size());
        validComponents.reserve(tempComp.size());
        compBB.reserve(tempComp.size());

        std::cout << "After filtering " << validComponents.size() << " components" << std::endl;
}

int main(int argc, char *argv[])
{
    Mat xx=imread(argv[1],0);
    Mat input1=imread(argv[1]);
    IplImage input=IplImage(input1);
{
    Mat grayImage,edgeImage,gaussianImage,gradientX,gradientY;
    cv::cvtColor ( input1, grayImage, CV_BGR2GRAY );

    // Create Canny Image
    double threshold_low = 175;
    double threshold_high = 320;

    cv::Canny(grayImage, edgeImage, threshold_low, threshold_high, 3) ;

    // Create gradient X, gradient Y
    grayImage.convertTo(gaussianImage,CV_32FC(1),1./255.,0);
    cv::GaussianBlur(gaussianImage,gaussianImage,Size(5,5),0);

    cv::Scharr(gaussianImage,gradientX,CV_32FC(1),1,0);
    cv::Scharr(gaussianImage,gradientY,CV_32FC(1),0,1);


    cv::GaussianBlur(gradientX, gradientX,Size(3, 3),0);
    cv::GaussianBlur(gradientY, gradientY,Size(3, 3),0);

    Mat swtimage,swtimage1;
    vector<Ray> rays;
    SWT(edgeImage,gradientX,gradientY,rays,swtimage);
    swtimage.copyTo(swtimage1);
    //swtimage.convertTo(swtimage1,CV_8UC(1),1.0,0);
    cv::normalize(swtimage1,swtimage1,0,255,CV_MINMAX);
    imshow("O",input1);
    imshow("DDDDD",swtimage1);
    imshow("O1",edgeImage);
}
{
        IplImage * grayImage =
                cvCreateImage ( cvGetSize ( &input ), IPL_DEPTH_8U, 1 );
        cvCvtColor ( &input, grayImage, CV_BGR2GRAY );

        // Create Canny Image
        double threshold_low = 175;
        double threshold_high = 320;
        IplImage * edgeImage =
                cvCreateImage( cvGetSize (&input),IPL_DEPTH_8U, 1 );
        cvCanny(grayImage, edgeImage, threshold_low, threshold_high, 3) ;
        cvSaveImage ( "canny.png", edgeImage);

        // Create gradient X, gradient Y
        IplImage * gaussianImage =
                cvCreateImage ( cvGetSize(&input), IPL_DEPTH_32F, 1);
        cvConvertScale (grayImage, gaussianImage, 1./255., 0);
        cvSmooth( gaussianImage, gaussianImage, CV_GAUSSIAN, 5, 5);
        IplImage * gradientX =
                cvCreateImage ( cvGetSize ( &input ), IPL_DEPTH_32F, 1 );
        IplImage * gradientY =
                cvCreateImage ( cvGetSize ( &input ), IPL_DEPTH_32F, 1 );
        cvSobel(gaussianImage, gradientX , 1, 0, CV_SCHARR);
        cvSobel(gaussianImage, gradientY , 0, 1, CV_SCHARR);
        cvSmooth(gradientX, gradientX, 3, 3);
        cvSmooth(gradientY, gradientY, 3, 3);
        cvReleaseImage ( &gaussianImage );
        cvReleaseImage ( &grayImage );

        // Calculate SWT and return ray vectors
        std::vector<Ray> rays;
        IplImage * SWTImage =
                cvCreateImage ( cvGetSize ( &input ), IPL_DEPTH_32F, 1 );
        for( int row = 0; row < input.height; row++ ){
            float* ptr = (float*)(SWTImage->imageData + row * SWTImage->widthStep);
            for ( int col = 0; col < input.width; col++ ){
                *ptr++ = -1;
            }
        }
        strokeWidthTransform ( edgeImage, gradientX, gradientY, true, SWTImage, rays );
//        SWTMedianFilter ( SWTImage, rays );

        //IplImage * output2 =
       //         cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_32F, 1 );
        //normalizeImage (SWTImage, output2);
        //IplImage * saveSWT =
//                cvCreateImage ( cvGetSize ( input ), IPL_DEPTH_8U, 1 );
//        cvConvertScale(output2, saveSWT, 255, 0);
//        cvSaveImage ( "SWT.png", saveSWT);
        //cvReleaseImage ( &output2 );
        //cvReleaseImage( &saveSWT );
        Mat out=Mat(SWTImage);
        cv::normalize(out,out,0,255,CV_MINMAX);
        imshow("AAAA",out);
}
    cv::waitKey(0);

    // Calculate SWT and return ray vectors
    /*std::vector<Ray> rays;
    IplImage * SWTImage =
            cvCreateImage ( cvGetSize ( &input ), IPL_DEPTH_32F, 1 );
    for( int row = 0; row < input.height; row++ ){
        float* ptr = (float*)(SWTImage->imageData + row * SWTImage->widthStep);
        for ( int col = 0; col < input.width; col++ ){
            *ptr++ = -1;
        }
    }
    bool dark_on_light=true;
    strokeWidthTransform ( edgeImage, gradientX, gradientY, dark_on_light, SWTImage, rays );
    SWTMedianFilter ( SWTImage, rays );





    IplImage * output2 =cvCreateImage ( cvGetSize ( &input ), IPL_DEPTH_32F, 1 );
    normalizeImage (SWTImage, output2);
    IplImage * saveSWT =cvCreateImage ( cvGetSize ( &input ), IPL_DEPTH_8U, 1 );
    cvConvertScale(output2, saveSWT, 255, 0);

    cvSaveImage ( "SWT.png", saveSWT);
    cvReleaseImage ( &output2 );
    cvReleaseImage( &saveSWT );

    return 0;
    // Calculate legally connect components from SWT and gradient image.
    // return type is a vector of vectors, where each outer vector is a component and
    // the inner vector contains the (y,x) of each pixel in that component.
    std::vector<std::vector<Point2d1> > components = findLegallyConnectedComponents(SWTImage, rays);

    // Filter the components
    std::vector<std::vector<Point2d1> > validComponents;
    std::vector<std::pair<Point2d1,Point2d1> > compBB;
    std::vector<Point2dFloat> compCenters;
    std::vector<float> compMedians;
    std::vector<Point2d1> compDimensions;
    filterComponents(SWTImage, components, validComponents, compCenters, compMedians, compDimensions, compBB );

    IplImage * output3 =
            cvCreateImage ( cvGetSize ( &input ), 8U, 3 );
    renderComponentsWithBoxes (SWTImage, validComponents, compBB, output3);
    cvSaveImage ( "components.png",output3);


    std::vector<Chain> chains;
    chains = makeChains(&input, validComponents, compCenters, compMedians, compDimensions, compBB);

    IplImage * output4 =
            cvCreateImage ( cvGetSize ( &input ), IPL_DEPTH_8U, 1 );
    renderChains ( SWTImage, validComponents, chains, output4 );


    IplImage * output5 =cvCreateImage ( cvGetSize ( &input ), IPL_DEPTH_8U, 3 );
    cvCvtColor (output4, output5, CV_GRAY2RGB);
    cvReleaseImage ( &output4 );
    cvSaveImage ( "text.png", output5);

    return 0;

    char *picName = argv[1];


    Mat image,image1,image2;
    xx.copyTo(image);



    imshow("OOOO",image);
    int hh=60;
    for(int i=0;i<image.rows;i=i+hh/2)
    {
    image1=image(Rect(0,i,(int)(image.cols/2),hh));

    cv::resize(image1,image2,Size(640/2,480/2),0,0,cv::INTER_CUBIC);
    cerr << "aaaaaaaaaaa" << image1.cols << ":" << image1.rows << endl;
    cv::threshold(image2,image2,0,255,CV_THRESH_OTSU);


    Process(image2);

    //trg::calc_lums(dest);
    //return 0;
    size_t picWidth = dest.cols;
    size_t picHeight = dest.rows;
    Mat dest1;
    cv::bitwise_not(dest,dest1);

    Mat dst, cdst;
    cvtColor(dest1, cdst, CV_GRAY2BGR);

    vector<Vec4i> lines;

    HoughLinesP(dest1, lines, 1, CV_PI/180, 30, 50,50 );

     for( size_t i = 0; i < lines.size(); i++ )
     {
       Vec4i l = lines[i];
       if(abs(l[2]-l[0])<0.001)
           continue;
       double angle=fabs(atan((l[3]-l[1])/(l[2]-l[0])));
       if((angle >-30  && angle <30) || (angle >150 && angle <181) )
       line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
     }
     cv::waitKey(0);
     imshow("DDDD",cdst);
     cv::waitKey(0);
    }
     return 0;

/*
    cvtColor(dest,dest,CV_GRAY2BGR);
    double mm,ma;
    cv::minMaxLoc(dest,&mm,&ma);
    cerr << "FFFF" << mm <<":" << ma << endl;
    /*size_t picWidth = 300;
    size_t picHeight = 225;
    char *picName = "/data1/222t.raw";

    char buf[picWidth * picHeight * 3];
    FILE *fp = fopen(picName, "rb");
    fread(buf, 1, sizeof(buf), fp);
    fclose(fp);


    std::vector<std::vector<trg::Rgb> > a;
    trg::Rgb r1={0,0,0};

    //a.resize(picHeight,std::vector<trg::Rgb>(picWidth));

    a.clear();
    a.resize(picHeight);
    for (int i = 0; i < picHeight; ++i) a[i].resize(picWidth, r1);

    uchar *buf=dest.data;
    for (size_t y = 0; y < picHeight; ++y) {
        for (size_t x = 0; x < picWidth; ++x) {
            size_t pos =  (3*y*picWidth)+(3 * x);
            trg::Rgb pixel = {buf[pos],buf[pos + 1],buf[pos+2]};
            a[y][x] = pixel;
        }
    }

    std::vector<trg::Rect> r = trg::get_textlike_regions(a);

    puts("====================================");
    puts(picName);
    puts("====================================");
    printf("Detected %d region(s):\n", r.size());
    puts("====================================");
    if (!r.empty()) {
        for (size_t i = 0; i < r.size(); ++i) {
            printf("%3d. (%d, %d) (%d, %d)\n", i + 1, r[i].x1, r[i].y1, r[i].x2, r[i].y2);
            cv::rectangle(image,Rect(r[i].x1,r[i].y1,abs(r[i].x2-r[i].x1),abs(r[i].y2-r[i].y1)),Scalar(0,255,255),1,8);
        }
    }
    else
        printf("...Nothing to report...\n");
    puts("====================================");
    puts("Press any key to exit...");
    //getchar();
    imshow("ASDAd",image);
    cv::waitKey(0);
return 0;
*/
}
