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

#include "Common/OpenCVCommon.hpp"

/**
 * @brief The Histogram class provides interface to class which handles histogram related operations
 */
class Histogram{
    public:

        /**
         * @brief Histogram is default constructor for histogram class
         */
        Histogram();


        /**
         * @brief BuildHistogram method constructs a histogram of source  input image which is required to be a single channel image
         * @param srcImage input single channel 8 bit image
         * @return  histogram matrix
         */
        cv::Mat BuildHistogram(cv::Mat srcImage);

        /**
         * @brief getThresh methods returns the pixel index corresponding to s1% from lower tail and s2% from the upper tails of histogram
         * @param srcImage is the input image
         * @param s1 is ration which indicates the % of pixels from the lower tail of histogram
         * @param s2 is ration which indicates the % of pixels from the upper tails of histogram
         * @return
         */
        std::vector<int> getThresh(cv::Mat srcImage, float s1, float s2);


    private:
        /**
         * @brief _histMat is temporary matrix used to store the histogram of the image
         */
        cv::Mat _histMat;
        /**
         * @brief _histSize is the size of the hisgoram
         */
        int _histSize;  // Size of the histogram

        /**
         * @brief _range is range of pixel intensities to be considered for the histogram calculations
         */
        float _range[2]; //Range to be considered

        const float* _histRange;  //Range for all the matrices


};

#endif
