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

#ifndef HISTOGRAM
#define HISTOGRAM

#include <iostream>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>


class Histogram{
    public:
        //Default Constructor
        Histogram();


        cv::Mat BuildHistogram(cv::Mat srcImage);

        std::vector<int> getThresh(cv::Mat srcImage, float s1, float s2);


    private:
        cv::Mat _histMat; 		//Histogram Mat
        int _histSize;  		// Size of the histogram
        float _range[2]; 		//Range to be considered
        const float* _histRange;  	//Range for all the matrices


};

#endif
