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


Mat image;
Mat a;
int main(int argc,char *argv[])
{


    string a1=(argv[1]);
    string a2=(argv[2]);

    if(argc <2 )
   {
        cerr << "USAGE : program {path}  {video file name}";

    }

    string source=a1+"/"+a2;
    VideoCapture inputVideo(source);              // Open input
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


   //creating OCLX object
    OCLX o;




    Mat output(image.rows,image.cols,image.type());
    Mat output1(image.rows,image.cols,image.type());

    //structures to hold kernel and program
    cl_kernel kernel[10];
    cl_program program;

     struct timeval end,result,now;
     long int diff;
    Mat b;

   //initialising opencl structues
    o.init();

    //loading the program and kernel source
    o.load_demo(&program,&kernel[0]);



    for(;;)
    {




        gettimeofday(&now, NULL);

        inputVideo >> image;


        resize(image,a, a.size(), 0, 0, INTER_NEAREST);
        a.copyTo(b);
        imshow("image",a);

        //running parallel program
        gettimeofday(&now,NULL);
        o.program1(a,output,&kernel[0],program);
        gettimeofday(&end,NULL);
        diff=timeval_subtract(&result,&end,&now);
        cerr << "Time to execute parallel lgorithm " << (float)diff << "  us" << endl;

        imshow("win1",a);


        //running serial lgorithm
        gettimeofday(&now,NULL);
        cv::cvtColor(b,b,CV_BGR2HSV_FULL);
        cv::cvtColor(b,b,CV_HSV2BGR_FULL);
        cv::cvtColor(b,b,CV_BGR2GRAY);


        gettimeofday(&end,NULL);
        diff=timeval_subtract(&result,&end,&now);
        cerr << "Time to execute serial " << (float)diff << "  us" << endl;
        imshow("win2",b);
        cv::waitKey(1);


}

    o.destroy();


}






