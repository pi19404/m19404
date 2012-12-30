#ifndef COLOR_CONSTANCY_HPP
#define COLOR_CONSTANCY_HPP

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;




/**
 * @file   color_correction
 * @brief  This file provide some common algoriths to achiece color constancy like gray world,shades of gray in RGB and Lab Color space.
 *         max RGB,gray edge ,max edge algorithms,a modified contrast streching algorithm
 * @author pi19404
 **/


/** @brief class defining generic interfaces and methods common for color correction methods**/
class  color_correction
{

public:
/** @brief class defining methods to perform color correction using modified contrast stretching **/
class contrast_stretching
{

/**  @brief method to perform modified contrast stretching.It takes as input image and lower and upper threshold index
 *         and performs contrast stretching for pixels in the range vmin and vmax.The other pixels are saturated to lower or higher values
 *         index denote the color channel for which contrast streching should be performed.
 **/
private:
void cstretch(Mat src,Mat dst,int min,int max,int index);

public:
/** The main method to call to perform color correction using modified contrast stretching The only input required is the input image
*   The method computes the lower and higher threshold pixel values and then calls the contrast_stretching method **/
Mat  run(Mat);
};



/** class containing methods to perform gray world and shades of gray color correction in RGB and Lab Color space **/
class gray_world
{

public:
/** The method to compute the estimate of illumination vector .Inputs are image and norm and outputs are ml,ma,mb the estimate of illumination vector/scaling factors**/
void process(Mat src1,int p,float *ml,float *ma,float *mb);



/** function to estimae the illumination vector or scaling factor for RGB color space computation
*   inputs are the sourc image,minkownski norm factor (p) ,and three normalization defined by (m=0,1,2)
*   the outputs are the factor ml,ma,mb for each of the color channel */
void process(Mat src1,float *ml,float *ma,float *mb,int p,int m);


 /** this is a main function to call to perform color correction in RGB color space **/
Mat  run2(Mat,int p,int m);

/** This is the main method which to call for color correction in Lab Color space.This performs basic pre-processing and calls the gray world function **/
Mat  run1(Mat,int p);

};



/** class to perform color correction using gray edge algorithm */
class gray_edge
{

    /** defines convolution type **/
    enum ConvolutionType {
    /* Return the full convolution, including border */
      CONVOLUTION_FULL,

    /* Return only the part that corresponds to the original image */
      CONVOLUTION_SAME,

    /* Return only the submatrix containing elements that were not influenced by the border */
      CONVOLUTION_VALID
    };

    /** method to perform 2D convolution */
    void conv2(const Mat &img, const Mat& kernel, ConvolutionType type, Mat& dest);


    /** method to perform gray edge color corrections,input are the src,minkowski norm and normalization method
     *  outputs are illumination vector estimate/scaling factor */
    void  process(Mat src1,float *ml,float *ma,float *mb,int p,int m);

public:
    /** this is a main function to call to perform color correction using gray edge algorithm in RGB color space **/
    Mat  run(Mat,int p,int m);
};





class maxRGB
{



public:

    /** main function to call for maxRGB color correction **/
    Mat  run(Mat,int p,int m);


    /** function coputes the illumination estimate max RGB algorithm.The function takes input image,minkowski norm and normalization method .
    *   the output are ml,,ma,mb for each of the color channels
    **/

    void  process(Mat src1,float *ml,float *ma,float *mb,int p,int m);


};

class max_edge
{
public:
    /** main function to call to perform max-edge color correction */
    Mat  run(Mat,int p,int m);

    /** defines convolution type **/
    enum ConvolutionType {
    /* Return the full convolution, including border */
      CONVOLUTION_FULL,

    /* Return only the part that corresponds to the original image */
      CONVOLUTION_SAME,

    /* Return only the submatrix containing elements that were not influenced by the border */
      CONVOLUTION_VALID
    };

    /** method to perform 2D convolution */
    void conv2(const Mat &img, const Mat& kernel, ConvolutionType type, Mat& dest);

    /** function that computes illumination vector for max_edge algorithm**/
    void  process(Mat src1,float *ml,float *ma,float *mb,int p,int m);
};









};

#endif // COLOR_CONSTANCY_HPP
