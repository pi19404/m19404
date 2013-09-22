/**
IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

 By downloading, copying, installing or using the software you agree to this license.
 If you do not agree to this license, do not download, install,
 copy or use the software.


                          License Agreement
               For Open Source Computer Vision Library

Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
Copyright (C) 2008-2011, Willow Garage Inc., all rights reserved.
Third party copyrights are property of their respective owners.

@Authors
	  prashant iyengar ,pi19404@gmail.com

@References
	 Nicolas Limare, Jose-Luis Lisani, Jean-Michel Morel, Ana Belén Petro, and Catalina Sbert, Simplest Color 		 Balance, Image Processing On Line, vol. 2011. http://dx.doi.org/10.5201/ipol.2011.llmps-scb

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * The name of the copyright holders may not be used to endorse or promote products
    derived from this software without specific prior written permission.

This software is provided by the copyright holders and contributors "as is" and
any express or implied warranties, including, but not limited to, the implied
warranties of merchantability and fitness for a particular purpose are disclaimed.
In no event shall the Intel Corporation or contributors be liable for any direct,
indirect, incidental, special, exemplary, or consequential damages
(including, but not limited to, procurement of substitute goods or services;
loss of use, data, or profits; or business interruption) however caused
and on any theory of liability, whether in contract, strict liability,
or tort (including negligence or otherwise) arising in any way out of
the use of this software, even if advised of the possibility of such damage.
**/


#include "contraststretching.hpp"



ContrastStretching::ContrastStretching()
{
    _mode=Mode(Mode::DEFAULT);
    lowerRatio.push_back (0.015);
    lowerRatio.push_back (0.015);
    lowerRatio.push_back (0.03);
    upperRatio.push_back (0.015);
    upperRatio.push_back (0.015);
    upperRatio.push_back (0.03);
    threshold=70;
}

void ContrastStretching::setmode(Mode mode)
{
    _mode=mode;
}

Mode ContrastStretching::getmode()
{
    return _mode;
}

Scalar ContrastStretching::getMin()
{
    return _min;
}

Scalar ContrastStretching::getMax()
{
    return _max;
}

Scalar ContrastStretching::getLUT(Scalar a)
{
    Scalar _tmp;
    _tmp[0]=_LUT[a[0]*_channels];
    _tmp[1]=_LUT[a[1]*_channels+1];
    _tmp[2]=_LUT[a[2]*_channels+2];
    return _tmp;
}


Mat ContrastStretching::GlobalContrastStretching(Mat image)
{


    if(_image.rows==0 || _image.cols==0)
    {
        _image=image.clone ();
        _channels=_image.channels ();
        _rows=_image.rows;
        _cols=_image.cols;
    }
    else
    {
        _image=image.clone ();
    }
    if(_LUT.size ()!=256*_image.channels ())
    {
        _LUT.resize (_image.channels ()*256);
    }

        cv::split(_image,_channel);
        MakeGlobalLUT();
        uchar * ptr=_image.data;;

        for(int i=0;i<_rows;i++)
        {
            for(int j=0;j<_cols;j++)
            {
                for(int c=0;c<_channels;c++)
                ptr[i*_image.step+j*_channels+c]=_LUT[_channels*ptr[i*_image.step+j*_channels+c]+c];
            }
        }


    return _image;

}


void ContrastStretching::MakeGlobalLUT()
{


    if(_mode==Mode::DEFAULT)
    {
    for(int i=0;i<_channels;i++)
    {
        cv::minMaxLoc (_channel[i],&_min1,&_max1,0,0);
        _min[i]=_min1;
        _max[i]=_max1;
        for(int k=0;k<256;k++){
             _LUT[k*_channels+i]=k-_min1>0?k>_max1?255:255*(k-_min1)/(_max1-_min1):0;
        }
    }
    }
    else if(_mode==Mode::V1)
    {
        for(int i=0;i<_channels;i++)
        {
                Histogram imgHist;
                std::vector<int> imgThresh;
                imgThresh = imgHist.getThresh(_channel[i], lowerRatio[i], upperRatio[i]);
                _min1=0;
                _max1=0;
                cv::minMaxLoc (_channel[i],&_min1,&_max1,0,0);
                _min[i]=imgThresh[0];
                _max[i]=imgThresh[1];
                for(int k=0;k<256;k++)
                {
                    if(_max1>threshold)
                    {
                        if(_max[i]<threshold/2)
                            _max[i]=threshold/2;
                        _LUT[k*_channels+i]=(k-_min[i]<0)?0:(k>_max[i])?255:(255*(k-_min[i])/(_max[i]-_min[i]));
                    }
                      else
                          _LUT[k*_channels+i]=k;
                }
        }

    }


}








