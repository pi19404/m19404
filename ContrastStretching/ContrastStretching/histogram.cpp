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

#include "histogram.hpp"

Histogram::Histogram(){
    _histSize = 256;
    _range[0] = 0;
    _range[1] = 256;
    _histRange = {_range};


}

cv::Mat Histogram::BuildHistogram(cv::Mat srcImage){
    cv::Mat histMat;
    cv::calcHist(&srcImage, 1, 0, cv::Mat(), _histMat, 1, &_histSize, &_histRange, true, false);
    histMat = _histMat.clone();
    return histMat;
}


std::vector<int> Histogram::getThresh(cv::Mat image, float s1, float s2){
    cv::Mat histMat = BuildHistogram (image);
    std::vector<int> imgThresh;
    imgThresh.push_back(0);
    imgThresh.push_back(255);
    float N = image.rows * image.cols;
    float maxth = (1-s2)*N;
    float minth = s1*N;
    int mini=0,maxi=0;
    float cmini=0,cmaxi=N;
    bool lower=true,higher=true;
    for (int i = 0; i < histMat.rows; i++){
    cmini+= histMat.at<float>(i);
    cmaxi-= histMat.at<float>(256-i);
    if(cmini >= minth && lower==true){
    mini = i;
    lower=false;
    }
    if(cmaxi <= maxth && higher==true){
    maxi = 256-i;
    higher=false;
    }
    if(lower==false && higher ==false )
    break;
    }
    imgThresh[0]=mini;
    imgThresh[1]=maxi;
    return imgThresh;
}

