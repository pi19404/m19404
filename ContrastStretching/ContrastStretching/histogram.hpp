//Header for all methods related to histogram - Extracting histogram, find minimum, maximum, certain percentage of pixels below / above

#ifndef HISTOGRAM
#define HISTOGRAM

#include <iostream>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

//Assumes default type single channel images CV_8UC3, generic implementation will be done later, if needed
class Histogram{
    public:
        //Default Constructor
        Histogram();

        //Returns histogram of an image
        cv::Mat BuildHistogram(cv::Mat srcImage);
        //Returns image threshold values for pixels less than lowerRatio% and greater than (100-upperRatio)% of total pixels
        std::vector<int> getThresh(cv::Mat srcImage, float s1, float s2);


    private:
        cv::Mat _histMat; //Histogram Mat

        int _histSize;  // Size of the histogram
        float _range[2]; //Range to be considered
        const float* _histRange;  //Range for all the matrices


};

#endif
