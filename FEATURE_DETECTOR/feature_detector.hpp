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
