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

//function to perform intepolation for local contrast stretching
Mat interpolate1(Mat image,int xsize,int ysize,int xdivs,int ydivs,int mode,vector< vector<uchar> > hist);
//function to construct the Lookup table for local contrast stretching
void make_lut(Mat r,int bins);

//function to construct the histogram used for local contrast stretching
///Mat make_histogram(Mat image,int bins);

Mat  AHE(Mat image,int xdivs,int ydivs,int bins);

void set_channels(vector<int> channel);




};



