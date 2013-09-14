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

#include "ImgProc/Histogram.hpp"

Histogram::Histogram(){
    _histSize.push_back(256);
    _histSize.push_back(256);
    _histSize.push_back(256);
    _histRange.push_back(0);
    _histRange.push_back(180);
    _histRange.push_back(0);
    _histRange.push_back(256);
    _histRange.push_back(0);
    _histRange.push_back(256);



}

float Histogram::compareHist(Histogram h2,int method=comparison_method::NORM_L1)
{

    Mat H1=this->getHist();
    Mat H2=h2.getHist();
    const Mat* arrays[] = {&H1, &H2, 0};


    //planes,number of channels of histogram
    Mat planes[2];


    //N Dimensional array iterator
    NAryMatIterator it(arrays, planes);

    int len = it.planes[0].rows*it.planes[0].cols;
    float result=0;
    float a=0,b=0,s1=0,s2=0;
    float s11=0,s12=0,s22=0;
    cerr << it.nplanes << endl;
    cerr << it.planes[0].rows << ":" <<  it.planes[0].cols << endl;
    for( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        const float* h1 = (const float*)it.planes[0].data;
        const float* h2 = (const float*)it.planes[1].data;


    if(method==comparison_method::NORM_L1)
    {
        for( int j = 0; j < len; j++ )
        {
            double a = abs(h1[j] - h2[j]);
            if( fabs(a) > DBL_EPSILON )
                result += a;
        }
        result=result/len;
    }
    else if(method==comparison_method::NORM_L2)
    {
        for( int j = 0; j < len; j++ )
        {
            double a = (h1[j] - h2[j])*(h1[j] - h2[j]);
            if( fabs(a) > DBL_EPSILON )
                result += a;
        }
        result=result/len;

    }
    else if(method == comparison_method::CHI_SQUARED)
    {
        for( int j = 0; j < len; j++ )
        {
            double a = h1[j] - h2[j];
            double b = h1[j];
            if( fabs(b) > DBL_EPSILON )
                result += a*a/b;
        }

    }
    else if( method == comparison_method::INTERSECTION )
    {
        for( int  j = 0; j < len; j++ )
            result += std::min(h1[j], h2[j]);
    }
    else if( method == comparison_method::BHATTACHRYA )
    {
        for( int j = 0; j < len; j++ )
        {
            double a = h1[j];
            double b = h2[j];
            result += std::sqrt(a*b);
            s1 += a;
            s2 += b;
        }



    }
    else if( method == comparison_method::CORRELATION )
    {
        for( int j = 0; j < len; j++ )
        {
            double a = h1[j];
            double b = h2[j];

            s12 += a*b;
            s1 += a;
            s11 += a*a;
            s2 += b;
            s22 += b*b;
        }

    }
    else
    {
        CV_Error( CV_StsBadArg, "Unknown comparison method" );
    }

    }

    if(method==comparison_method::CORRELATION)
    {
        s1=s1/len;
        s2=s2/len;

        size_t total = len;
        double scale = 1./total;
        double num = s12 -(total*s1*s2);
        double denom2 = (s11 - s1*s1*total)*(s22 - s2*s2*total);
        result = std::abs(denom2) > DBL_EPSILON ? num/std::sqrt(denom2) : 1.;

    }
    else if( method == comparison_method::BHATTACHRYA )
    {
        s1 *= s2;
        s1 = fabs(s1) > FLT_EPSILON ? 1./std::sqrt(s1) : 1.;
        result = std::sqrt(std::max(1. - result*s1, 0.));

    }

    return result;
}

Mat Histogram::getHist()
{
    return _histMat;
}


cv::Mat Histogram::BuildHistogram(cv::Mat srcImage){
    cv::Mat histMat;
    //compute  histogram
    int *c=(int *)calloc(sizeof(int),_channels.size());
    for(int i=0;i<_channels.size();i++)
    {
        c[i]=_channels[i];

    }

    int *h=(int *)calloc(sizeof(int),_channels.size());
    for(int i=0;i<_channels.size();i++)
    {
        h[i]=_histSize[_channels[i]];

    }

    float **ranges=(float **)calloc(sizeof(float*),_channels.size());
    int size=_channels.size();
    for(int i=0;i<size;i++)
    {
        float *x=(float *)calloc(sizeof(float),2);
        int index=2*_channels[i];
        x[0]=_histRange[index];
        x[1]=_histRange[index+1];
        ranges[i]=x;
        //cerr << x[0] << ":" <<x[1] <<endl;
    }
    cv::calcHist(&srcImage,1,c, cv::Mat(),_histMat,_channels.size(),h,(const float **)ranges, true, false);
    for(int i=0;i<size;i++)
    {
        free(ranges[i]);
    }
    free(ranges);
    free(c);
    free(h);
    //normalize histogram
    normalize( _histMat, _histMat,1,0,NORM_L1);
    //copy histogram
    histMat = _histMat.clone();
    //return histogram
    return histMat;
}

Mat Histogram::drawHist()
{
   int hist_w = 512; int hist_h = 400;
   Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );



   for( int i = 1; i < _histSize[0]; i++ )
    {
       int bin_w = cvRound( (double) hist_w/_histSize[0]);
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(255*_histMat.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(255*_histMat.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
        /*line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                         Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                         Scalar( 0, 0, 255), 2, 8, 0  );*/
    }

   return histImage;
}

void Histogram::setChannel(vector<int>channels)
{
    _channels=channels;

}

void Histogram::setHistSize(vector<int> size)
{
    _histSize=size;
}

void Histogram::setRange(vector<float> range)
{
    _histRange=range;
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



