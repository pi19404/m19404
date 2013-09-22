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

#ifndef _OPENCV_your_header_name_HPP_
#define _OPENCV_your_header_name_HPP_ 
#ifdef __cplusplus
namespace cv {

#include "common.hpp"
#include "histogram.hpp"

class Mode
{

public:
  enum Value
  {
    DEFAULT, V1, CLAHE
  };
private:
  Value v_;

public:
  Mode (Value v): v_ (v)
   {
   }

  Mode()
  {
    v_=DEFAULT;
  }
   operator Value () const
   {
     return v_;
   }




/*      operator char const* () const
  {
    return color_labels_[v_];
  }*/




};


class ContrastStretching
{
public:

ContrastStretching();


private :
Mode _mode;
vector <uchar> _LUT;            //lookup table for transformation
vector <Mat> _channel;      // RGB channel buffer
double _min1,_max1;         //temporal variables
Scalar _min,_max;               //minimum and maximum pixel intensities used for transformation
Mat _image;                         //temporary buffer for image
int _channels,_rows,_cols; //temporary variables for image information
std::vector<float> lowerRatio;  //lower and upper ration thresholds for image
std::vector<float> upperRatio;
int threshold ; //distance threshold between new and old threshold


vector <int> channels;  //variable indicating which  channels should be processed

public:
void setmode(Mode mode);
Mode getmode();

//function to be called to compute lookup table for global contrast stretching
void MakeGlobalLUT();

public:
Scalar getMin();
Scalar getMax();

vector <uchar> getLUT()
{
    vector <uchar> tmp;
    tmp.resize(_LUT.size());
    copy(_LUT.begin(), _LUT.end(), tmp.begin());
    return tmp;
}

Scalar getLUT(Scalar a);
//function to be called for global contrast stretching
Mat GlobalContrastStretching(Mat image);




};

}
#endif
#endif

