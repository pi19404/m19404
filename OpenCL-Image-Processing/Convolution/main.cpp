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
int main(int argc,char *argv[])
{


    if(argc <2)
    {
        cerr << "Usage : Convolution {directory} {filename}"   << endl;
        return 1;
    }
    mode=0;
    string a1=(argv[1]);
    string a2=(argv[2]);

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
    int nchannel=1;
    int dmode=1;
    int mask_height=3;
    int mask_width=3;


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
    sprintf(options1,"-DSTEP=%d -DWIDTH=%d -DHEIGHT=%d -DWIDTHSTEP=%u -DMASK_DIM=%d -DMASK_WIDTH=%d -DMASK_HEIGHT=%d -DUNROLL11=1 -g",a.channels(),a.cols,a.rows,step,mask_width,mask_width,mask_height);//+"-DMASK_HEIGHT="+mask_height+"-DTS=16";

    cerr << options1 << endl;

    o.conv2d(&program,&kernel[0],options1);


    unsigned char  input[width*height];
    unsigned char  mask[mask_height*mask_width];
    unsigned char  output1[width*height];
    unsigned char  outputc[width*height];
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
            cvtColor(image,image1,CV_BGR2GRAY);


        resize(image1,a, a.size(), 0, 0, INTER_NEAREST);

        if(dmode==1)
        imshow("input image",a);


        //running parallel program
        gettimeofday(&now,NULL);

        if(dmode==0)
            a.setTo(cv::Scalar::all(1));
        o.kernel_conv2(a.data,a.cols,a.rows,mask,a.step,a.channels(),mask_height,b.data,&kernel[0],program);

        gettimeofday(&end,NULL);
        diff=timeval_subtract(&result,&end,&now);
        diff1=diff1+diff;



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




        gettimeofday(&now,NULL);

        o.kernel_conv2(a.data,a.cols,a.rows,mask,step,mask_height,a.channels(),b.data,&kernel[1],program);
        gettimeofday(&end,NULL);
        diff=timeval_subtract(&result,&end,&now);
        diff2=diff2+diff;

        if(dmode==1)
        imshow("optimized parallel",b);

        if(dmode==0)
        {
            a.setTo(cv::Scalar::all(1));
            b.setTo(cv::Scalar::all(0));
            //temp.setTo(cv::Scalar::all(0));
        }

        //running serial lgorithm
        gettimeofday(&now,NULL);

        //    cv::GaussianBlur(a,b,Size(mask_width,mask_height),1);
        cv::boxFilter(a,b,a.depth(),Size(3,3));
        if(dmode==1)
        imshow("opencv",b);


        gettimeofday(&end,NULL);
        diff=timeval_subtract(&result,&end,&now);
        diff3=diff3+diff;

        if(dmode==0)
        {
            a.setTo(cv::Scalar::all(1));
            b.setTo(cv::Scalar::all(0));
            //temp.setTo(cv::Scalar::all(0));
        }

        gettimeofday(&now,NULL);
        o.kernel_conv_separable(a.data,width,height,mask,mask_width,mask_height,b.data,(float*)temp.data,&kernel[2],program);
        //o.kernel_conv_separable(a.data,width,height,mask,mask_width,mask_height,b.data,&kernel[2],program);
        gettimeofday(&end,NULL);
        diff=timeval_subtract(&result,&end,&now);
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






