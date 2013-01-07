
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
Mat make_lut(Mat r,int bins);
Mat make_histogram(Mat image,int bins);
Mat clip_histogram(unsigned long clip,Mat hist,int bins);
Mat map_histogram(int bins,int,int,unsigned long,Mat hist);
Mat interpolate(Mat image,int xsize,int ysize,int xdivs,int ydivs,int mode);
Mat AHE(Mat image,int xdivs,int ydivs,int bins,int mode,float limit);
Mat plot_hist(Mat image);
Mat run(Mat image,int type);
};
