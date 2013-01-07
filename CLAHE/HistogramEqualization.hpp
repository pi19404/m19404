/*
 * This file contains class method for histogram equalization,image normalization,
 * adaptive histogram equalization,adaptive histogram equalization with interpolation and CLAHE
 * using opencv
 * Copyright (C) 2012 by pi19404
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <iostream> // for standard I/O
#include <string>   // for strings

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/imgproc/imgproc.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write

using namespace cv;
using namespace std;
/**
 * @brief The HistogramEqualization class perform global & local histogram equalization
 *        and produces histogram plots for display
 */
class HistogramEqualization
{

public :
HistogramEqualization()
{
        lut.create(1,256, CV_8UC(1));

}

const static int GLOBAL=0,LOCAL=1;
Mat lut;
vector<Mat> hist;
public:
/**
 * @brief this method constructs lookup table for transformation function for histogram equalization ie CDF of histogram
 * @param input image
 * @param number of bins of histogram
 * @return transformed image using lookup table
 */
Mat make_lut(Mat r,int bins);
/**
 * @brief method constructs the histogram for given image
 * @param image is input image
 * @param bins is number of histogram bins
 * @return constructed histogram
 */
Mat make_histogram(Mat image,int bins);
/**
 * @brief This method clips the histogram and redistributes pixel to obtain uniform histogram
 * @param clip absolute clipping level of histogram
 * @param hist input histogram
 * @param bins input number of bins
 * @return  clipped histogram
 */
Mat clip_histogram(unsigned long clip,Mat hist,int bins);
/**
 * @brief This method computes the CDF for histogram for equalization
 * @param bins number of bins of histogram
 * @param hist input histogram
 * @return transformed histogram
 */


Mat map_histogram(int bins,int,int,unsigned long,Mat hist);
/**
 * @brief This function performs bi-linear interpolation of image using value of pixels from transformation
 *function of adjacent blocks
 * @param image is input image
 * @param xsize  size of block in x direction
 * @param ysize  size of block in y direction
 * @param xdivs  number of blocks in x direction
 * @param ydivs  number of blocks in y direction
 * @param mode   parameter indicating to perform normalization,adaptive equalization,adaptive equalization with interpolation
 * @return
 */
Mat interpolate(Mat image,int xsize,int ysize,int xdivs,int ydivs,int mode);
/**
 * @brief The main function called for performing varios operations
 * @param image input image
 * @param xdivs number of block in x direction for adaptive HE
 * @param ydivs number of blocks in y direction for adaptive HE
 * @param bins   nuber of bins of histogram
 * @param mode   parameter for perform normalization,adaptive equalization,adaptive equalization with interpolation
 * @param limit  clipping limit is 0.1*limit*xsize*ysize/bins
 * @return transformed image
 */
Mat AHE(Mat image,int xdivs,int ydivs,int bins,int mode,float limit);
/**
 * @brief plot_hist function to plot histogram
 * @param image
 * @return
 */
Mat plot_hist(Mat image);
/**
 * @brief run is function for global Histogram equalization
 * @param image
 * @param type
 * @return
 */
Mat run(Mat image,int type);

};
