#include <iostream>
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
#include <iostream>

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <sstream>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/imgproc/imgproc.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write
#include "AndroidOpenCVProcessImage.hpp"

using namespace std;
using namespace cv;



//#include <stdio.h>




//demo program main function
int main(int argc,char *argv[])
{
    VideoCapture inputVideo;
    Mat src,a;

    inputVideo.open (0);
    if (!inputVideo.isOpened())
    {
        //cout  << "Could not open the input video: " << source << endl;
        return -1;
    }
    inputVideo >> src;
    a.create(1*240,1*320,CV_8UC(1));
    resize(src,a, a.size(), 0, 0, INTER_NEAREST);
    OpenCV::OpenCVProcess p;
    p.initDetector ("/media/UBUNTU/softwares-linux/OpenCV-2.4.3/data/haarcascades/haarcascade_frontalface_alt.xml");
    int k=0;
    do//Show the image captured in the window and repeat
    {

        inputVideo >> src;              // read


        if (src.empty()) break;         // check if at end
        resize(src,a, a.size(), 0, 0, INTER_AREA);

        p.run (a);
        imshow("Face",a);



        k=cv::waitKey (1);
        }while(k!='e');



    return 0;
}

