#ifndef MEANSHIFT_H
#define MEANSHIFT_H



#include "Common/OpenCVCommon.hpp"
#include "ImgProc/Histogram.hpp"
class meanShift
{
public:
    meanShift();
    Histogram h;



    void buildModel(Mat image)
    {
        h.BuildHistogram(image);
    }

    void run(Mat Image)
    {




    }



};

#endif // MEANSHIFT_H
