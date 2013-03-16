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
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS

//System header files

#include <iostream>
#include <cstdlib>
#include "string.h"
#include <CL/cl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctime>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <iostream>
#include <ctime>
#include <sys/time.h>

//OpenCV header files
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#define DISPLAY 1
//OpenCL Header Files
#include <CL/opencl.h>
#include "ocl_common.h"


using namespace std;

//function to get the time difference
long int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    //result->tv_sec = diff / 1000000;
    //result->tv_usec = diff % 1000000;

    return (diff);
}


Mat image,output,image1;
Mat a,b,temp;

VideoCapture inputVideo;
int mode=1;


class filter{
unsigned char *mask;
unsigned char * separable;
int mask_size;
int nchannel;
Mat tmp;
int init_flags;

};


class box_filter
{
    public:
    unsigned char * mask;
    unsigned char *  separable;
    int mask_size;
    int nchannel;
    Mat tmp;
    int init_flags;
    int scale;
    box_filter(int size,int channel)
    {
        Mat result;
        mask_size=size;
        nchannel=channel;
        mask=(unsigned char *)malloc(mask_size*mask_size);
        separable=(unsigned char* )malloc(2*mask_size);

        for(int i=0;i<mask_size;i++)
            for(int j=0;j<mask_size;j++)
                mask[i*mask_size+j]=1;

        scale=0;
        for(int i=0;i<mask_size;i++)
        {

        mask[i]=1;
        mask[i+mask_size]=1;
        scale=scale+mask[i];
        }
    }

  double filter_cpu(Mat image,Mat result1)
    {
      struct timeval now,end,result;
      gettimeofday(&now,NULL);

        cv::boxFilter(image,result1,image.depth(),Size(mask_size,mask_size));
        gettimeofday(&end,NULL);
        double diff=timeval_subtract(&result,&end,&now);

        return diff;

    }

    double filer_2d(Mat a,Mat b,OCLX o,cl_kernel *kernel,cl_program program)
    {

        struct timeval now,end,result;
        gettimeofday(&now,NULL);

        o.kernel_conv2(a.data,a.cols,a.rows,mask,a.step,a.channels(),mask_size,b.data,&kernel[0],program);
        gettimeofday(&end,NULL);
        double diff=timeval_subtract(&result,&end,&now);

        return diff;

    }

   double filer_2ds(Mat a,Mat b,OCLX o,cl_kernel *kernel,cl_program program,Mat temp)
    {
        struct timeval now,end,result;
        gettimeofday(&now,NULL);


        o.kernel_conv_separable(a.data,a.cols,a.rows,mask,mask_size,mask_size,b.data,(float*)temp.data,kernel,program);
        gettimeofday(&end,NULL);
        double diff=timeval_subtract(&result,&end,&now);

        return diff;

    }

};


class gaussian_filter
{
    public:
    unsigned char * mask;
    unsigned char *  separable;
    double sigma;
    int scale;
    int mask_size;
    int nchannel;
    Mat tmp;
    int init_flags;

    gaussian_filter(int size,double sigmaX)
    {
        Mat result;

        sigma=sigmaX;
        mask_size=size;
        int m=size/2;
        mask=(unsigned char *)malloc(mask_size*mask_size);
        float * mask1=(float  *)malloc(mask_size*sizeof(float));
        separable=(unsigned char* )malloc(2*mask_size);
        double scale1 = -0.5/(sigmaX*sigmaX);
        double sum = 0;
        double min=9999;

        for(int i=0;i<mask_size;i++)
        {
            double t=std::exp((i-m)*(i-m)*scale1);
            if(t<min)
                min=t;
            mask1[i]=t;

        }
       scale=0;


        for(int i=0;i<mask_size;i++)
        {
           mask[i]=std::ceil(mask1[i]/min);
           mask[i+mask_size]=mask[i];
           //scale=scale+mask[i];
           //cerr  << (int)mask[i] <<":" ;
        }
             //       cerr << endl;
                    for(int j=0;j<mask_size;j++)
                    for(int i=0;i<mask_size;i++)
                    {
                        scale=scale+mask[i]*mask[j+mask_size];
                    }
    }

  double filter_cpu(Mat image,Mat result1)
    {
      struct timeval now,end,result;
      gettimeofday(&now,NULL);

      cv::GaussianBlur(image,result1,Size(mask_size,mask_size),sigma,sigma);
        gettimeofday(&end,NULL);
        double diff=timeval_subtract(&result,&end,&now);

        return diff;

    }


   double filer_2ds(Mat a,Mat b,OCLX o,cl_kernel *kernel,cl_program program,Mat temp)
    {
        struct timeval now,end,result;
        gettimeofday(&now,NULL);


        o.kernel_conv_separable(a.data,a.cols,a.rows,mask,mask_size,mask_size,b.data,(float*)temp.data,kernel,program);
        gettimeofday(&end,NULL);
        double diff=timeval_subtract(&result,&end,&now);

        return diff;

    }

};


int main(int argc,char *argv[])
{


    if(argc <3)
    {
        cerr << "Usage : Convolution {directory} {filename} {filter}"   << endl;
        cerr <<" filters : Box Gaussian Sobel Laplacian" << endl;
        return 1;
    }
    mode=0;
    string a1=(argv[1]);
    string a2=(argv[2]);
    string a3=argv[3];


    string source=a1+"/"+a2;
    inputVideo.open(source);              // Open input
    inputVideo.set(CV_CAP_PROP_FPS,10);
    if (!inputVideo.isOpened())
    {
        cout  << "Could not open the input video: " << source << endl;
        return -1;
    }
    int i=0;


    int width=320;
    int height=240;
    int nchannel=3;
    int dmode=1;
    int mask_height=13;
    int mask_width=13;


       int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

       Size S = Size((int) 240,320*2);
                     // Acquire input size(int)

       a.create(height,width,CV_8UC(nchannel));
       inputVideo >> image;
       if(nchannel==1)
           cvtColor(image,image,CV_BGR2GRAY);
       resize(image,a, a.size(), 0, 0, INTER_NEAREST);
       a.copyTo(b);
       temp.create(max(a.rows,a.cols),max(a.rows,a.cols),CV_32FC(nchannel));
       float *temp1=(float *)malloc(max(a.rows,a.cols)*max(a.rows,a.cols)*nchannel);
   //creating OCLX object
    OCLX o;
    box_filter box(mask_height,nchannel);
    gaussian_filter gauss(mask_height,3);



    output.create(image.rows,image.cols,image.type());


    //structures to hold kernel and program
    cl_kernel kernel[10];
    cl_program program;

     struct timeval end,result,now;
     long int diff;


   //initialising opencl structues
    o.init();

double diff1=0,diff2=0,diff3=0,diff4=0;

    int step=a.step;
    //loading the program and kernel source
    char options1[2000];
    int fmode=0;
    if(strcmp(argv[3],"Gaussian")==0)
    {

    sprintf(options1,"-DSTEP=%d -DWIDTH=%d -DHEIGHT=%d -DWIDTHSTEP=%u -DMASK_DIM=%d -DMASK_WIDTH=%d -DMASK_HEIGHT=%d -DUNROLL11=1 -g -DSCALE=%d",a.channels(),a.cols,a.rows,step,mask_width,mask_width,mask_height,gauss.scale);//+"-
    fmode=1;
    cerr << options1 <<endl;
    }
    else if(strcmp(argv[3],"Box")==0)
    {
        sprintf(options1,"-DSTEP=%d -DWIDTH=%d -DHEIGHT=%d -DWIDTHSTEP=%u -DMASK_DIM=%d -DMASK_WIDTH=%d -DMASK_HEIGHT=%d -DUNROLL11=1 -g -DSCALE=%d",a.channels(),a.cols,a.rows,step,mask_width,mask_width,mask_height,box.scale);//+"-
    fmode=0;
    }



    cerr << options1 << endl;

    o.conv2d(&program,&kernel[0],options1);


    unsigned char  input[width*height];
    unsigned char  mask[mask_height*mask_width];
    unsigned char  output1[width*height];
    unsigned char  output2[width*height];
    unsigned char  output3[width*height];


    for(int i=0;i<mask_width*mask_height;i++)
    {

        mask[i]=1;

    }
    /*mask[0*mask_width+0]=1;
    mask[0*mask_width+1]=1;
    mask[0*mask_width+2]=1;
    mask[1*mask_width+0]=1;
    mask[1*mask_width+1]=1;
    mask[1*mask_width+2]=1;
    mask[2*mask_width+0]=1;
    mask[2*mask_width+1]=1;
    mask[2*mask_width+2]=1;*/

    for(int i=0;i<width*height;i++)
    {
        input[i]=1;
        output1[i]=0;
        output2[i]=0;
        output3[i]=0;

    }

  int xx=0;
  int error=0;
  do
  {








      inputVideo >> image;
      if(nchannel==1)
      {
          cvtColor(image,image1,CV_BGR2GRAY);
          resize(image1,a, a.size(), 0, 0, INTER_NEAREST);
      }
      else
      {
          resize(image,a, a.size(), 0, 0, INTER_NEAREST);
      }




        if(dmode==1)
        imshow("input image",a);


        //running parallel program


        if(dmode==0)
            a.setTo(cv::Scalar::all(1));

        //starting the code for 2D naive convolution

        if(fmode==0)
        {
        box.filer_2d(a,b,o,&kernel[0],program);

        diff1=diff1+diff;


        //printing the data
        if(dmode==0)
        {
        for(int j=0;j<height;j++)
        {
        for(int i=0;i<width;i++)
            cerr << (int)b.data[j*width+i] << "  ";
        cerr <<endl;
        }
        cerr <<endl;
        cerr <<endl;
        }


        if(dmode==1)
        imshow("naive parallel",b);

        if(dmode==0)
        {
            a.setTo(cv::Scalar::all(1));
            b.setTo(cv::Scalar::all(0));
        }



        //starting the 2D convolution optimized version

        diff=box.filer_2d(a,b,o,&kernel[1],program);
        diff2=diff2+diff;


        if(dmode==1)
        imshow("optimized parallel",b);

        if(dmode==0)
        {
            a.setTo(cv::Scalar::all(1));
            b.setTo(cv::Scalar::all(0));
            //temp.setTo(cv::Scalar::all(0));
        }

        //start the code for opencv host implementation

        diff=box.filter_cpu(a,b);
        diff3=diff3+diff;

        if(dmode==1)
        imshow("opencv",b);

        if(dmode==0)
        {
            a.setTo(cv::Scalar::all(1));
            b.setTo(cv::Scalar::all(0));
            //temp.setTo(cv::Scalar::all(0));
        }


        //starting the code for separable filters

        diff=box.filer_2ds(a,b,o,&kernel[2],program,temp);
        diff4=diff4+diff;



        if(dmode==1)
        {
        imshow("naive separable",b);
        }


        if(dmode==0)
        {
        for(int j=0;j<height;j++)
        {
        for(int i=0;i<width;i++)
            cerr << (int)b.data[j*width+i] <<"  ";//-output3[j*width+i];
        cerr <<endl;
        }
        cerr <<endl;
        cerr <<endl;
        }
        }
        else if(fmode==1)
        {
            diff=gauss.filter_cpu(a,b);
            diff3=diff3+diff;
            imshow("opencv",b);
            diff=gauss.filer_2ds(a,b,o,&kernel[2],program,temp);
            diff4=diff4+diff;
            imshow("naive separable",b);

        }
/*
        for(int j=0;j<height;j++)
        {
        for(int i=0;i<width;i++)
           cerr << (int)output3[j*width+i];
        cerr <<endl;
        }
        cerr <<endl;
        cerr <<endl;
*/

            if(xx%30==0 && xx>0)
            {

              xx=0;
            cerr << "Average Time to execute naive parallel algorithm " << (float)diff1*1/30 << "  us" << endl;
            cerr << "Average  Time to optimized parallel algorithm " << (float)diff2*1/30 << "  us" << endl;
            cerr << "Average Time to execute serial " << (float)diff3*1/30 << "  us" << endl;
            cerr << "Average Time to execute separable " << (float)diff4*1/30 << "  us" << endl;
            cerr  << "Error is " << error << endl;
            error=0;
            diff1=0;diff2=0;diff3=0,diff4=0;
            }
            xx=xx+1;

        cv::waitKey(1);


    }while(dmode==1);

    o.destroy();


}






