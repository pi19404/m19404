/* ========================================================================
 * Copyright [2013][prashant iyengar] The Apache Software Foundation
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 * ========================================================================
 */
#ifndef FAST3D_H
#define FAST3D_H

#include "feature_detector.hpp"


namespace feature_detector
{
//comparing values and returning points corresponding to sorted values




class fast3d :public feature_detector
{


int aperture_size;          //aperture size for spatial edge detection
vector <Mat> image;     //vector queue of images for input to termporal  block filtering
Mat  Dt;   //vector queue containing output of termporal block filtering
Mat  Dx,Dy;    //vector queue which contains spatial derivatives
int index;                       // starting index of  bottom of queue
Mat final_return;
Mat current_frame;
int borderType;
int threshold;
int block_size;
int corner_count;
//Mat final_output;
double qualityLevel;
public:

bool status()
{
    return start;
}
void set_status(bool st)
{
    start=false;
}

fast3d()
{
    block_size=10;
    aperture_size=3;
    borderType=cv::BORDER_DEFAULT;
    current_frame=Mat();
    start=false;
    minDistance=10;
    qualityLevel=0.01;
    image.resize(aperture_size);
    index=0;
    threshold=10;
    maxCorners=100;
    start=false;
    corner_count=0;
}

// response comparison, for list sorting
bool compare_response(cv::KeyPoint first,cv::KeyPoint second)
{
  if (first.response < second.response) return true;
  else return false;
}

struct pred
{
    bool operator()(KeyPoint const & a, KeyPoint const & b) const
    {
        return a.response > b.response;
    }
};

void reset()
{
    start=false;
}

Mat ret_current_frame()
{
    return current_frame;
}

        bool start;
        std::vector<cv::KeyPoint> keypoints;
     vector<cv::Point2f> run(Mat src)
    {
         int maxCorners1=maxCorners;
         keypoints.resize (0);
         Mat tmp;
         Mat xx1;
         src.copyTo (xx1);
         vector <cv::Point2f> corners;
        src.copyTo(image[index]);
        Size size=image[0].size();


        cv::GaussianBlur (image[index],image[index],Size(3,3),2);
        if(start==false && index == aperture_size-1)
        {
            start=true;
          }
        if(start==false && index<aperture_size)
        {

            Mat tmp_output;
            tmp_output.create(image[0].rows,image[0].cols,CV_32FC(1));
            Mat matrix1( size, CV_32FC(1));
            matrix1.copyTo (final_return);

        }
    else if(start==true)
            {

            double scale = (double)(1 << ((aperture_size > 0 ? aperture_size : 3) - 1)) * block_size;

             scale *=255.;
             scale = 1./scale;

                //computing the derivatives in x and y direction
                Mat Dx, Dy;

                int mindex=(index+1)%aperture_size; //index of the oldest image
                //computing the spatial derivatives
                Sobel( image[mindex], Dx,CV_32F, 1, 0, aperture_size, scale, 0, borderType );
                Sobel(  image[mindex], Dy,CV_32F, 0, 1, aperture_size, scale, 0, borderType );

                Mat mag;
                cv::magnitude (Dx,Dy,mag);  //computing the gradient magnitude
                double max1;
                cv::minMaxLoc (mag,0,&max1,0,0);    //commputing the maximum value of gradient
                //cv::threshold (mag,mag,max1*qualityLevel,0,THRESH_TOZERO);


                for(int i=0;i<mag.rows;i++)
                {
                    float *ptr=mag.ptr <float>(i);
                    for(int j=0;j<mag.cols;j++)
                    {
                        float val=ptr[j];
                        //point satisfies the spatial derivative test
                        if(val >=max1*0.1)  //checking for significant spatial derivatives
                        {



                            //checking the temporal derivative test
                                int index1=(index+1)%3;
                                int index2=(index+2)%3;
                                int index3=(index+3)%3;
                                int index4=i*image[0].step+j;
                                float v1=2*image[index2].data[index4]-image[index1].data[index4]-image[index3].data[index4];
                                v1=abs(v1);
                                if(v1>threshold)            //checking for significant temporal derivatives
                                {
                                //analyze the 3D block for spatio temporal block test.

                                     int l1,l2,l3,l4;
                                    l1=max(0,i-1);
                                    l2=min(mag.rows,i+1);
                                    l3=max(0,j-1);
                                    l4=min(mag.cols,j+1);
                                    int positive=0;
                                    int negative=0;
                                    for(int x1=l1;x1<=l2;x1++)
                                    {
                                        for(int x2=l3;x2<=l4;x2++)
                                        {
                                            if(x1!=i && x2!=j)
                                                continue;
                                            int index5=x1*image[0].step+x2;
                                            for(int x3=0;x3<3;x3++)
                                            {
                                            float val1=image[x3].data[index5];
                                            if(val1 > val+threshold)
                                            {
                                                positive++;
                                            }
                                            if(val1 < val-threshold)
                                            {
                                                negative++;
                                            }
                                            }


                                        }
                                    }


                                    if(positive>13 || negative >13)
                                    {
                                        cv::KeyPoint k;
                                        k.pt=Point(j,i);
                                        k.response=max(positive,negative);
                                        keypoints.push_back (k);

                                    }

                                }



                        }

                    }
                }



                //for computing the harris response function
                cv::boxFilter (Dx,Dx,Dx.depth (),Size(block_size,block_size));
                cv::boxFilter (Dy,Dy,Dy.depth (),Size(block_size,block_size));


                Mat final_return;
                Dx.copyTo (final_return);
                final_return.setTo (cv::Scalar::all (0));
                std::vector<cv::KeyPoint>::iterator it;


                for( it= keypoints.begin(); it!= keypoints.end();it++)
                {
                    //computing the harris response function

                    cv::KeyPoint k=*it;
                    float * ptr1=Dx.ptr<float>(k.pt.y);
                    float * ptr2=Dy.ptr<float>(k.pt.y);
                    float   v1=ptr1[(int)k.pt.x];
                    float   v2=ptr2[(int)k.pt.x];
                    float a=v1*v1;
                    float b=v1*v2;
                    float c=v2*v2;
                    double u = (a + c)*0.5;
                    double v = std::sqrt((a - c)*(a - c)*0.25 + b*b);
                    double l1 = u - v;      //minimum eigen values
                    double l2 = u + v;    //maximum eigen values

                    //storing the response function
                    k.response=l2;
                }

               //sort(keypoints.begin(), keypoints.end(), pred());


                //storing the response function in Matrix
                for( it= keypoints.begin(); it!= keypoints.end();it++)
                {

                    cv::KeyPoint k=*it;
                    float *ptr=final_return.ptr<float>(k.pt.y);
                    ptr[(int)k.pt.x]=k.response;
                 ///   corners.push_back (k.pt);


                }

             //thresholding the eigen values

                //evaluating the maximum values of response function
             double maxVal = 0;
             minMaxLoc(final_return, 0, &maxVal, 0, 0,Mat() );

             //dilate(final_return,tmp, Mat());



            //cell size for locally significant gradient
             const int cell_size =cvRound(minDistance);
             int xx=floor(src.cols/cell_size);
             int yy=floor(src.rows/cell_size);

             std::vector  <double>eig;

             vector <Point2f> tmp_corners ;






             //accessing the cell blocks of size min distance

             //copmputing the locally significaint gradient
             for(int x=0;x<xx;x++)
             {
                 for(int y=0;y<yy;y++)
                 {
                     int w=cell_size;
                     if((x+1)*(cell_size)>src.cols)
                         w=(x+1)*(cell_size)-src.cols;
                     int h=cell_size;
                     if((y+1)*(cell_size)>src.rows)
                         h=(y+1)*(cell_size)-src.rows;

                     Rect roi(x*cell_size,y*cell_size,w,h);
                     Mat r=final_return(roi);
                     Point minLoc;
                     double m1=0;
                     minMaxLoc( r, 0, &m1,0,&minLoc, Mat());
                     if(m1>maxVal*qualityLevel) //check for signficiant gradients
                     {
                     cv::threshold(r,r,maxVal*qualityLevel, 0, THRESH_TOZERO );
                     minLoc.x=minLoc.x+x*cell_size;
                     minLoc.y=minLoc.y+y*cell_size;
                     //
                     eig.push_back(m1);
                     tmp_corners.push_back(minLoc);
                     }

                 }

            }


             //xx1=cv::abs(final_return);
             //cv::normalize(xx1,xx1,0,255,CV_MINMAX);





             //cv::cvtColor(xx1,xx1,CV_GRAY2BGR);

             if(eig.size()<maxCorners1)
                 maxCorners1=eig.size();

             //cerr << ":" << eig.size ()  << ":" << maxCorners1 << endl;
             //initialising index vector
             vector <int> index_vector;
             for (unsigned i = 0; i < eig.size(); ++i)
             {

                 index_vector.push_back(i);


             }

             corners.resize (0);
             //sorting the eigen value vectoe ie response function according to magnitude
             sort(index_vector.begin(),index_vector.end(), index_cmp<vector<double>&>(eig));



             int ncorners=0;

             //selecting corner points at minimum distance from  each other
             for (int i = 0; i < eig.size(); ++i)
             {
                 bool good=true;
                 //populating vectors of corners detected

                 for(int k=0;k<ncorners;k++)
                 {
                     double dx=tmp_corners[index_vector[i]].x-corners[k].x;
                     double dy=tmp_corners[index_vector[i]].y-corners[k].y;
                     if( dx*dx + dy*dy < minDistance* minDistance )
                     {
                         //cerr << dx*dx + dy*dy <<":" ;
                         good =false;
                         break;

                     }

                 }


                 if(good==true)
                 {
                     corners.push_back(tmp_corners[index_vector[i]]);
                     ncorners=ncorners+1;

                 }

                 if(ncorners >=maxCorners1)
                     break;
             }

             corner_count=ncorners;
             int cindex=(index+2)%(aperture_size);
             if(cindex<0)
                 cindex=(cindex+aperture_size);
             image[cindex].copyTo(current_frame);

            }


                index =(index+1) % (aperture_size);


                return corners;



    }

      int get_count()
     {
            return corner_count;
        }
};
}
#endif // HARRIS3D_H


