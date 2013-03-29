#ifndef ANDROIDPROCESSIMAGE_HPP
#define ANDROIDPROCESSIMAGE_HPP
#include <string>
#include <sstream>
#include <iostream>
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv/cv.h"
#include <stdlib.h>
#include <vector>
#include <stack>
#include <iostream>
#include <map>
#include <ctime>


using namespace std;
using namespace cv;


namespace OpenCV
{
    class OpenCVProcess{

    public:
        OpenCVProcess()
        {

        }

        void run(int width, int height, signed char *_yuv, int* _bgra)
        {
            Mat myuv(height + height/2, width, CV_8UC1, (unsigned char *)_yuv);
            Mat mbgra(height, width, CV_8UC4, (unsigned char *)_bgra);
            //Please make attention about BGRA byte order
            //ARGB stored in java as int array becomes BGRA at native level
            cvtColor(myuv, mbgra, CV_YUV420sp2BGR, 4);
            Mat image;
            cvtColor(mbgra,image,CV_BGRA2GRAY);
            cvtColor(image,mbgra,CV_GRAY2BGRA);
        }

    };

}

#endif // ANDROIDPROCESSIMAGE_HPP
