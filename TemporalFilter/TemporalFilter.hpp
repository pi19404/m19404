#ifndef TEMPORALFILTER_HPP
#define TEMPORALFILTER_HPP
#include "cv.h"
#include <fstream>
#include <iostream>
#include "cv.h"
#include "highgui.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;
using namespace std;


class Temporal_Filter
{
float PI;    //approaximation of pi
int ksize;      //kernel size for log filter mask discrete approximation
float var;      // variance of log filter gaussian
int M;            // mean of log filter gaussian

vector<Mat> image;  // vector of input images
vector<Mat> log1;
float *mask;  // temporal filter calculated kernel mask
float *output;//  output of temporal filter

int k;
int index1;



public:

Temporal_Filter();
//int t[ksize];
/**
 * @brief function to perform 1D filtering operation
 * @param inputs are 1D signal and filter mask kernel
 *
 */
void Log_filter(float input[],float mask[]);

/**
 * @brief method that performs temporal filtering
 * @param input1 is the input image
 * @return  output is the temporal filtered image
 */
Mat temporal_filter(Mat input1);


};

#endif // TEMPORALFILTER_HPP
