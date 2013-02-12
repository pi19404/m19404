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

using namespace std;

int main(int argc,char **argv)
{

    VideoCapture inputVideo;
    Mat  a,b,image1,image;
        if(argc <4)
        {
            cerr << "Usage : ColorQuantizer {directory} {filename} {nchannels} {mode}"   << endl;

            return 1;
        }

        string a1=(argv[1]);
        string a2=(argv[2]);
        string a3=(argv[3]);
        string a4=(argv[4]);


        string source=a1+"/"+a2;

        int i=0;


        int width=320;
        int height=240;

        int nchannel=atoi(a3.c_str());
        int method=atoi(a4.c_str());
        int ndiv=4;
        image=imread(source);
         a.create(height,width,CV_8UC(nchannel));

           if(nchannel==1)
           {
               cvtColor(image,image1,CV_BGR2GRAY);
            resize(image1,a, a.size(), 0, 0, INTER_NEAREST);
           }
           else
               resize(image,a, a.size(), 0, 0, INTER_NEAREST);



           do
           {









               if(nchannel==1)
               {
                   cvtColor(image,image1,CV_BGR2GRAY);
                   resize(image1,a, a.size(), 0, 0, INTER_NEAREST);
               }
               else
               {
                   resize(image,a, a.size(), 0, 0, INTER_NEAREST);
               }

               imshow("input image",a);
               string odir;
               string idir;

               if(method==0)
               {
               int div=256/ndiv;
               cerr << div <<endl;
               a.convertTo(b,a.type(),1.0/div,0);
               //a=(a/div);
               b.convertTo(b,b.type(),div,0);

               odir="o0"+a2;
               idir="i0"+a2;
               }
               else
               {
                   double x1,x2;
                   cv::minMaxLoc(a,&x1,&x2,NULL,NULL);
                    int div=x2/ndiv;
                    a.convertTo(b,a.type(),1.0/div,0);
                    b.convertTo(b,b.type(),div,0);
                    odir="o1"+a2;
                    idir="i1"+a2;
               }
               //a=a*div;
               imwrite(idir,a);
               imwrite(odir,b);
               imshow("output image",b);
               cv::waitKey(0);
           }while(1==0) ;


}

