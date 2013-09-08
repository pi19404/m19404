#include "histogram.hpp"

Histogram::Histogram(){
    _histSize = 256;
    _range[0] = 0;
    _range[1] = 256;
    _histRange = {_range};

    _offSetX = 10;
    _offSetY = 10;
    _widthPerBin = 2;
    _binHeight = 400;
}

cv::Mat Histogram::BuildHistogram(cv::Mat srcImage){
    cv::Mat histMat;
    cv::calcHist(&srcImage, 1, 0, cv::Mat(), _histMat, 1, &_histSize, &_histRange, true, false);
    histMat = _histMat.clone();
    return histMat;
}

std::vector<cv::Mat> Histogram::drawHistograms(cv::Mat srcImage){

    std::vector<cv::Mat> imgChannels;
    std::vector<cv::Mat> histImages;
    std::vector<cv::Scalar> drawColors;

    if(srcImage.channels() > 1){
        cv::split(srcImage, imgChannels);
        drawColors.push_back(cv::Scalar(255,0,0));
        drawColors.push_back(cv::Scalar(0,255,0));
        drawColors.push_back(cv::Scalar(0,0,255));
    }

    else{
        imgChannels.push_back(srcImage);
        drawColors.push_back(cv::Scalar(0,0,0));
    }

    //Calculate histogram of each channel
    for(int i = 0; i < imgChannels.size(); i++){
        cv::Mat newHistImage = cv::Mat(cv::Size(_offSetX*2 + _widthPerBin*256, _offSetY*2+_binHeight), CV_8UC3, cv::Scalar(255,255,255));
        cv::Mat histImage = BuildHistogram (imgChannels[i]);
        double maxVal;
        cv::minMaxLoc(histImage, NULL, &maxVal, NULL, NULL);
        for(int j = 0; j < 256; j++){
            int binVal = histImage.at<float>(1,j);
            cv::rectangle(newHistImage, cv::Point(_offSetX + j * _widthPerBin, _offSetY + (int)((float)binVal/maxVal*_binHeight)), cv::Point(_offSetX + (j+1)*_widthPerBin, _offSetY), drawColors[i], 2, 8, 0);
        }
        cv::flip(newHistImage, newHistImage, 0);
        histImages.push_back(newHistImage);
    }

    return histImages;
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

