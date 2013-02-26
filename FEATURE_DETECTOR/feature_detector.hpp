#ifndef FEATURE_DETECTOR_HPP
#define FEATURE_DETECTOR_HPP
#include <iostream>

#include <iostream>
#include <iostream>

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <sstream>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/imgproc/imgproc.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write

using namespace cv;
using namespace std;
namespace  feature_detector
{
template<class T>
struct accessor {

    explicit accessor(const T& data) : value(data) {}
    T operator()() const { return value; }
    T& operator()() { return value; }
    void operator()(const T& data) { value = data; }

private:

    accessor(const accessor&);
    accessor& operator=(const accessor&);
    T value;

};

class feature_detector
{

public:
    feature_detector()
    {
        maxCorners=100;
    }
protected:

    uint maxCorners;

    //uint ;        //max corners to be returned
    int minDistance;        //min distance between detected points
    cv::vector<cv::Point2f> corners;
    ///pure virtual function to be redefined by the base class
    //main function to be called for feature detection which accepts the
    //input image and return a vector of 2D points.
public:
    virtual vector<cv::Point2f> run(Mat src)=0;


    //method to set the maxCorners value
    void setMaxCorners(uint value)
    {
        maxCorners=value;
    }

    //method to set minDistance value
    void setminDistance(uint value)
    {
        minDistance=value;
    }

};
}


/* End of file */



#endif // FEATURE_DETECTOR_HPP
