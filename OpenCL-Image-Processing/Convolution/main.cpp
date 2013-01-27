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


Mat image,output;
Mat a,b;

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



       int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

       Size S = Size((int) 240,    // Acquire input size
                     (int) 320*2);

       a.create(240,320,CV_8UC(3));
       inputVideo >> image;
       resize(image,a, a.size(), 0, 0, INTER_NEAREST);
       a.copyTo(b);

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

double diff1=0,diff2=0,diff3=0;
    int width=320;
    int height=240;
    int mask_height=3;
    int mask_width=3;

    //loading the program and kernel source
    char options1[1000];
    sprintf(options1,"-DMASK_WIDTH=%d -DMASK_HEIGHT=%d -DUNROLL11=1 -g",mask_width,mask_height);//+"-DMASK_HEIGHT="+mask_height+"-DTS=16";
    cerr << options1 << endl;
    o.conv2d(&program,&kernel[0],options1);


    unsigned char  input[width*height];
    unsigned char  mask[mask_height*mask_width];
    unsigned char  output1[width*height];
    unsigned char  outputc[width*height];
    unsigned char  output2[width*height];
    unsigned char  output3[width*height];
    /*for(int i=0;i<mask_width*mask_height;i++)
    {
        mask[i]=1;

    }*/
    mask[0*mask_width+0]=1;
    mask[0*mask_width+1]=1;
    mask[0*mask_width+2]=1;
    mask[1*mask_width+0]=1;
    mask[1*mask_width+1]=1;
    mask[1*mask_width+2]=1;
    mask[2*mask_width+0]=1;
    mask[2*mask_width+1]=1;
    mask[2*mask_width+2]=1;

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


        resize(image,a, a.size(), 0, 0, INTER_NEAREST);
        //a.copyTo(b);
        imshow("input image",a);


        //running parallel program
        gettimeofday(&now,NULL);

        o.kernel_conv2(a.data,a.cols,a.rows,mask,a.step,mask_height,b.data,&kernel[0],program);

        gettimeofday(&end,NULL);
        diff=timeval_subtract(&result,&end,&now);
        diff1=diff1+diff;




        imshow("naive parallel",b);

        b.setTo(cv::Scalar::all(0));
        gettimeofday(&now,NULL);

        o.kernel_conv2(a.data,a.cols,a.rows,mask,a.step,mask_height,b.data,&kernel[1],program);
        gettimeofday(&end,NULL);
        diff=timeval_subtract(&result,&end,&now);
        diff2=diff2+diff;

        imshow("optimized parallel",b);


        //running serial lgorithm
        gettimeofday(&now,NULL);

        //    cv::GaussianBlur(a,b,Size(mask_width,mask_height),1);
        cv::boxFilter(a,b,a.depth(),Size(3,3));

        gettimeofday(&end,NULL);
        diff=timeval_subtract(&result,&end,&now);
        diff3=diff3+diff;




/*
        for(int j=0;j<height;j++)
        {
        for(int i=0;i<width;i++)
           cerr << (int)output1[j*width+i];
        cerr <<endl;
        }
        cerr <<endl;
        cerr <<endl;

        for(int j=0;j<height;j++)
        {
        for(int i=0;i<width;i++)
            cerr << (int)output2[j*width+i] <<"  ";//-output3[j*width+i];
        cerr <<endl;
        }
        cerr <<endl;
        cerr <<endl;
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
            cerr  << "Error is " << error << endl;
            error=0;
            diff1=0;diff2=0;diff3=0;
            }
            xx=xx+1;

        imshow("opencv",b);
        cv::waitKey(1);


    }while(1==1);

    o.destroy();


}






