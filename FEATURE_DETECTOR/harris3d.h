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
#ifndef HARRIS3D_H
#define HARRIS3D_H

#include "feature_detector.hpp"


namespace feature_detector
{
//comparing values and returning points corresponding to sorted values
template<class T> struct index_cmp {
index_cmp(const T arr) : arr(arr) {}
bool operator()(const size_t a, const size_t b) const
{ return arr[a] > arr[b]; }
const T arr;
};

class TemporalBoxFilter
{
    int ksize;          //kernel size for temporal filters
    float *kernel;    //kernel for temporal averaging filters

public:
    //constructor for TemporalBoxFilter which initializes the kernel
    TemporalBoxFilter()
    {

        ksize=3;
        kernel=(float *)malloc(ksize*sizeof(float));
        kernel[0]=1;
        kernel[1]=1;
        kernel[2]=1;

        cerr << "initialization completed " << endl;

    }

    //constructor for TemporalBoxFilter which initalizes kernel
    TemporalBoxFilter(int value)
    {

        float sigma=1;
        ksize=value;
        kernel=(float *)malloc(ksize*sizeof(float));
        for(int i=0;i<ksize;i++)
            kernel[i]=(1/sqrt(2*3.1412*sigma))*exp(-(i-ksize/2)*(i-ksize/2)/(2*sigma));
        cerr << "initialization completed " << endl;
    }


    void process(vector<Mat> image,int index,int bsize,Mat output)
      {

          //output.setTo (cv::Scalar::all (0));

          for(int i=0;i<image[0].rows;i++)
          {
              for(int j=0;j<image[0].cols;j++)
              {
                  //for(int k=0;k<bsize;k++)
                  {

                      float value[output.channels ()];
                      for(int m=0;m<output.channels ();m++)
                          value[m]=0;
                          for(int l=0;l<ksize;l++)
                           {

                               int sindex=(index+1+l)%(ksize);
                              if(sindex<0)
                                  sindex=sindex+(ksize);
                              float *ptr=(float *)image[sindex].ptr<float>(i);
                              for(int m=0;m<output.channels ();m++)
                              value[m]=value[m]+(kernel[l]*(float)ptr[image[sindex].channels ()*j+m]);
                          }

                           //value=value;
                          float *out_ptr=(float *)output.ptr<float>(i);
                          for(int m=0;m<output.channels ();m++)
                           out_ptr[j*output.channels ()+m]=value[m]/ksize;

                  }

                  //out1.at<float>(i,j)=out1.at<float>(i,j)/bsize;
                  //output.at<uchar>(i,j)=out1.at<float>(i,j);

              }
           }

                    //cerr <<endl;
      }

};

class SpatioTemporalDerivativeFilter
{

    public:
    vector <int> block_size;
    int bsize;
    int index;    
    float *kernel;
    int scale;
    int ksize;
    int aperture_size;
    int borderType;

    SpatioTemporalDerivativeFilter()
    {
        bsize=5;
        ksize=3;
        kernel=(float *)malloc(ksize*sizeof(float));
        kernel[0]=-1;
        kernel[1]=0;
        kernel[2]=1;
        aperture_size=3;
        borderType=cv::BORDER_DEFAULT;

        cerr << "initialization completed " << endl;

    }

     void set_bsize(vector <int>value)
    {
        block_size=value;
        bsize=value[2];
        ksize=3;
        kernel=(float *)malloc(ksize*sizeof(float));
        kernel[0]=-1;
        kernel[1]=2;
        kernel[2]=-1;
        aperture_size=3;
        borderType=cv::BORDER_DEFAULT;
    }

  void process(vector<Mat> image,int index,int bsize,vector <Mat> Dt,vector <Mat> Dx,vector <Mat> Dy)
    {



      for(int k=0;k<bsize;k++)
      {
          int sindex=(index+1+k)%(bsize+2);
          if(sindex<0)
              sindex=sindex+(bsize+2);

          Dt[k].setTo(cv::Scalar::all (0));
          Dx[k].setTo(cv::Scalar::all (0));
          Dy[k].setTo(cv::Scalar::all (0));

          double scale = (double)(1 << ((aperture_size > 0 ? aperture_size : 3) - 1)) * block_size[0];
          double scale1 = (double)(1 << ((aperture_size > 0 ? aperture_size : 3) - 1)) * block_size[1];

           scale *=255.;
           scale = 1./scale;

           scale1 *=255.;
           scale1 = 1./scale1;

          cv::Sobel(image[sindex],Dx[k],CV_32F, 1, 0, aperture_size, scale, 0, borderType );
          cv::Sobel(image[sindex],Dy[k],CV_32F, 0, 1, aperture_size, scale1, 0, borderType );

      }
        //output.setTo (cv::Scalar::all (0));
        //Mat out1(input[0].rows,input[0].cols,CV_32FC(1));
        //out1.setTo (cv::Scalar::all (0));



        for(int i=0;i<image[0].rows;i++)
        {
            for(int j=0;j<image[0].cols;j++)
            {
                for(int k=0;k<bsize;k++)
                {

                        float value=0;
                        for(int l=0;l<ksize;l++)
                         {


                             int sindex=(index+1+l+k)%(bsize+2);
                            if(sindex<0)
                                sindex=sindex+(bsize+2);
//                            if(i==0&&j==0)
//                                cerr << ":" << sindex ;
                            value=value+(kernel[l]*(float)image[sindex].at<uchar>(i,j));
                        }

                         //value=value;
                         Dt[k].at<float>(i,j)=value;

                }

                //out1.at<float>(i,j)=out1.at<float>(i,j)/bsize;
                //output.at<uchar>(i,j)=out1.at<float>(i,j);

            }
         }

                  //cerr <<endl;
    }


};

class harris3d :public feature_detector
{

vector <int> block_size;          // a vector to hold the block size of spatial and temporal dimensions.
int aperture_size;          //aperture size for spatial edge detection
vector <Mat> image;     //vector queue of images for input to termporal  block filtering
vector <Mat > Dt;   //vector queue containing output of termporal block filtering
vector <Mat > Dx,Dy;    //vector queue which contains spatial derivatives
int index;                       // starting index of  bottom of queue
vector<Mat> block_data;
Mat final_output;
Mat final_return;
SpatioTemporalDerivativeFilter t;       //object for computing spatio-temporal derivatives
TemporalBoxFilter t1;
Mat current_frame;
double qualityLevel;
public:
harris3d()
    {
    current_frame=Mat();
   start=false;
    block_size.resize(3);
    block_size[2]=7;            //block size in termporal scale
    block_size[0]=11;            //block size in  x spatial dimension
    block_size[1]=11;            //block size in y spatial dimension
    minDistance=10;
    qualityLevel=0.00001;
    image.resize (block_size[2]+2);
    index=0;
    t.set_bsize (block_size);
    t1=TemporalBoxFilter(block_size[2]);
    //output.resize (block_size[2]);
    }

//    harris3d(int value)
//    {
//        block_size.resize(3);
//        block_size[2]=value;
//        block_size[0]=3;
//        block_size[1]=3;

//        image.resize (block_size[2]+2);
//        index=0;
//    }

Mat ret_current_frame()
{
    return current_frame;
}

        bool start;
     vector<cv::Point2f> run(Mat src)
    {
         Mat tmp;
         Mat xx1;
         src.copyTo (xx1);
         vector <cv::Point2f> corners;
        int ksize=block_size[2];


        src.copyTo(image[index]);
        Size size=image[0].size();


        cv::GaussianBlur (image[index],image[index],Size(3,3),2);
        if(start==false && index == ksize+2-1)
        {
            start=true;
          }
        if(start==false && index<ksize)
        {

            Mat tmp_output;
            tmp_output.create(image[0].rows,image[0].cols,CV_32FC(1));
            Dt.push_back (tmp_output);
            Dx.push_back (tmp_output.clone());
            Dy.push_back (tmp_output.clone ());
            Mat matrix( size, CV_32FC(6));
            block_data.push_back (matrix.clone ());
            matrix.copyTo (final_output);
            Mat matrix1( size, CV_32FC(1));
            matrix1.copyTo (final_return);
            //cerr << index << ": " << output[index].rows << ": " << output[index].cols << endl;
        }
    else if(start==true)
            {
                t.process(image,index,block_size[2],Dt,Dx,Dy);
   //             Mat xx;

                Mat xx=cv::abs(Dt[2]);
                cv::normalize(xx,xx,0,1,CV_MINMAX);
                imshow("Dt",xx);
                /*
                xx=cv::abs(Dx[2]);
                cv::normalize(xx,xx,0,1,CV_MINMAX);

                imshow("Dx",xx);
                xx=cv::abs(Dy[2]);
                cv::normalize(xx,xx,0,1,CV_MINMAX);

                imshow("Dy",xx);
*/

            //computing the matrix components A,





            //accessing temporal frames
            for(int k=0;k<ksize;k++)
            {
            //accessing rows
            for(int i = 0; i < size.height; i++ )
            {

                //accessing pointer to the rows
               const float* dx1 = (const float*)(Dx[k].ptr<float>(i));
               const float* dy1= (const float*)(Dy[k].ptr<float>(i));
               const float* dt1= (const float*)(Dt[k].ptr<float>(i));
               float* vals = (float*)((block_data[k].ptr<float>(i)));

               //accessing the columns of the matrix
                for(int  j = 0; j < size.width; j++ )
                {
                        //assigning values to the elements of rows
                    float dx = dx1[j];
                    float dy = dy1[j];
                    float dt = dt1[j];
                    vals[j*6] = dx*dx;
                    vals[j*6+1] = dy*dy;
                    vals[j*6+2] = dx*dy;
                    vals[j*6+3] = dt*dx;
                    vals[j*6+4] = dt*dy;
                    vals[j*6+5] = dt*dt;

                    //computing the eigen values at each point


                }

            }

            }
            //computing the averaage across spatial dimentions
            vector <Mat> channels;

            for(int k=0;k<ksize;k++)
            {

                cv::split(block_data[k],channels);
                for(int l=0;l<channels.size ();l++)
                {
                    cv::GaussianBlur (channels[l],channels[l],Size(block_size[0],block_size[1]),0.1);
                    //cv::boxFilter (channels[l],channels[l],channels[l].depth(),Size(block_size[0],block_size[1]));

                }
            }


            //computing the average across temporal dimentions
             t1.process (block_data,0,ksize,final_output);



            final_return.setTo (cv::Scalar::all (0));
             for(int  i = 0; i < size.height; i++ )
             {
                 float* vals = (float*)final_output.ptr<float>(i);
                 float* o1 = (float*)final_return.ptr<float>(i);
                 for(int  j = 0; j < size.width; j++ )
                 {
                     float a = vals[j*final_output.channels ()];
                     float b = vals[j*final_output.channels ()+1];
                     float c = vals[j*final_output.channels ()+2];
                     float d = vals[j*final_output.channels ()+3];
                     float e = vals[j*final_output.channels ()+4];
                     float f = vals[j*final_output.channels ()+5];

                     double u = (a*(c*f-e*e))-(b*(b*f-d*e))+(d*(b*e-c*d));
                     double v = a+c+f;

                     double l1 = u - (1/27)*v;
                     //double l2 = u + v;    //maximum eigen values

                     o1[j] = l1;                 //using the maximum eigen value to determine strong edge
                 }
             }
   //          double min;
     //        cv::minMaxLoc (final_return,&min,0,0,0);

             //xx=cv::abs(final_return);
             //cv::normalize(xx,xx,0,255,CV_MINMAX);

             //imshow("final_return",xx);

             //thresholding the eigen values
             double maxVal = 0;
             minMaxLoc(final_return, 0, &maxVal, 0, 0,Mat() );

             dilate(final_return,tmp, Mat());


             {
             const int cell_size =cvRound(minDistance);
             int xx=floor(src.cols/cell_size);
             int yy=floor(src.rows/cell_size);

             std::vector  <double>eig;

             vector <Point2f> tmp_corners ;






             //accessing the cell blocks of size min distance

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
                     //cerr <<  roi.x << ":" << roi.width <<":" <<  dst.cols << endl;
                     //cerr <<  roi.y << ":" << roi.height <<":" <<  dst.rows << ":" << (y+1)*(cell_size) << ":" << src.rows <<endl;
                     //selecting the subregion
                     Mat r=final_return(roi);
                     //selecting the maximum eigen value pixel in the subregion
                     //double maxVal;
                     Point minLoc;
                     double m1=0;
                     minMaxLoc( r, 0, &m1,0,&minLoc, Mat());
                     if(m1>=maxVal*qualityLevel)
                     {
                     //threshold(r,r,m1, 0, THRESH_TOZERO );
                     minLoc.x=minLoc.x+x*cell_size;
                     minLoc.y=minLoc.y+y*cell_size;
                     //
                     eig.push_back(m1);
                     tmp_corners.push_back(minLoc);
                     }

                 }

             }
             //Mat tmpx;
             //cv::normalize(dst,tmpx,0,255,CV_MINMAX);
             //dst.convertTo(tmpx,dst.type(),255.0,0);





             if(eig.size()<maxCorners)
                 maxCorners=(eig.size());

             //initialising index vector
             vector <int> b;
             for (unsigned i = 0; i < eig.size(); ++i)
             {

                 b.push_back(i);
                 //circle(tmp,tmp_corners[i], 3, Scalar(255,255,255), -1, 8);

             }

             //sorting the eigen value vector
             sort(b.begin(), b.end(), index_cmp<vector<double>&>(eig));
             int ncorners=0;
             for (int i = 0; i < eig.size(); ++i)
             {
                 bool good=true;
                 //populating vectors of corners detected

                 for(int k=0;k<corners.size();k++)
                 {
                     double dx=tmp_corners[b[i]].x-corners[k].x;
                     double dy=tmp_corners[b[i]].y-corners[k].y;
                     if( dx*dx + dy*dy < minDistance* minDistance )
                     {
                         good =false;
                         break;

                     }

                 }


                 if(good==true)
                 {

                  //   circle(xx1,tmp_corners[b[i]],3, Scalar(255,ncorners,ncorners), -1, 8);
                     corners.push_back(tmp_corners[b[i]]);
                     ncorners++;

                 }

                 if(ncorners >=maxCorners)
                     break;
             }
     }


             //int cindex=(index+2+(ksize/2))%(ksize+2);
             //cerr << "current frame is " << index <<endl ;
             int cindex=(index-ksize/2)%(ksize+2);
             if(cindex<0)
                 cindex=(cindex+ksize+2);
             image[cindex].copyTo(current_frame);
             //cerr << "written current frame " << index << ":" << cindex <<":" << current_frame.rows << ":" << current_frame.cols << endl;

            }

            //imshow("3d Harris ",xx1);
                index =(index+1) % (ksize+2);
                return corners;



    }

};
}
#endif // HARRIS3D_H


