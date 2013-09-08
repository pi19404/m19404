#include "../ColorConstancy/color_constancy.hpp"
#include "HistogramEqualization.hpp"

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <sstream>
#include <sys/time.h>

#include "../FusionEnhancement/laplacianBlend.hpp"


static string dname;
static string name;
int main1(int argc,char *argv[])
{

Mat image;
dname=argv[1];
name=argv[2];
Mat src;
image=imread(dname+"/"+name);
src.create(240,320,CV_8UC(3));
image=imread(dname+"/"+name);
resize(image,src,src.size(), 0, 0, INTER_NEAREST);

int nx=10;
int ny=10;
int bins=128;
float limit=20*0.1;
HistogramEqualization p;
color_correction::gray_world b1;
Mat a1=b1.run2(src,6,0);
Mat ahe=p.AHE(a1,nx,ny,bins,1,limit);
imshow("gray world RGB",a1);
imshow("CLAHE",ahe);
imwrite(dname+"/d1"+name,a1);
imwrite(dname+"/d2"+name,ahe);
//imshow("gray world Lab",b1.run1(input,1));
//imshow("Shades of gray",b1.run2(input,6,2));
//cv::waitKey(0);

}



/**
 * @brief this function generates the exposedness weight map
 * @param input image 1
 * @param input image 2
 * @return output image is wweight map
 */
Mat generateEMasks(Mat s1,Mat s2){

    //mean and vairance of exposedness mask
    float _gMean = 0.5;
    float _gVariance = 0.25;

   Mat dst;

   Mat src;
   s1.copyTo(src);

   //Mat src2;
   //s2.copyTo(src2);
   //converting to floating point images and extracting illumination channel
   src.convertTo(src,CV_32FC3,1.0,0);
   //src2.convertTo(src2,CV_32FC3,1.0,0);

   cvtColor(src,src,CV_BGR2HSV_FULL);
   //cvtColor(src2,src2,CV_BGR2HSV_FULL);

   src.convertTo(src,CV_32FC3,1.0/255.0,0);
   //src2.convertTo(src2,CV_32FC3,1.0/255.0,0);


   src.copyTo(dst);
//   dst.convertTo(dst,CV_32FC3,1,0);

   std::vector<cv::Mat_<float> > ichan,ichan1;
   std::vector<cv::Mat_<float> > dchan,dchan1;

    cv::split(src,ichan);
    //cv::split(src2,ichan1);

    cv::split(dst,dchan);
    //cv::split(dst,dchan1);




    int idx=2;



    cv::add(ichan[idx],-_gMean,dchan[idx]);



    multiply(dchan[idx],dchan[idx],dchan[idx], 1.0/(2*_gVariance*_gVariance));
    cv::exp(-dchan[idx], dchan[idx]);



    idx=2;
    //cv::add(ichan1[idx],-_gMean,dchan1[idx]);
    //multiply(dchan1[idx],dchan1[idx],dchan1[idx], 1.0/(2*_gVariance*_gVariance));
    //cv::exp(-dchan1[idx], dchan1[idx]);



    cvtColor(src,src,CV_HSV2BGR_FULL);
    //cvtColor(src2,src2,CV_HSV2BGR_FULL);


    dchan[idx].convertTo(dchan[idx],CV_32FC1,1.0,0);


     return dchan[idx];

}


/**
 * @brief function to compute global contrast
 * @param input image is BGR image
 * @return ouput image is a scalar weight map
 */
Mat localContrast(Mat s)
{
    Mat image;
    s.copyTo(image);

    Mat dst;
    cvtColor(image,image,CV_BGR2YCrCb);
    image.convertTo(image,CV_32FC3,1.0/255.0,0);
    std::vector<cv::Mat_<float> > d;
    cv::split(image,d);

    Mat tmp;
    //tmp.create(image.row,image.cols,CV_8UC1);
    d[0].copyTo(tmp);
    tmp.setTo(cv::Scalar::all(0));
    cv::Laplacian(d[0],d[0],image.depth(),3,1,0);
    cv::absdiff(d[0],tmp,d[0]);
    d[0].convertTo(d[0],CV_32FC1,1.0,0);

    return d[0];
}

/**
 * @brief function to compute the saliency weight map
 * @param input image is BGR image
 * @return  ouput image is scalar weight map
 */
Mat saliency(Mat s)
{

    Mat image;
    s.copyTo(image);
    Mat i,o1,o2,o3;
    cvtColor(image,image,CV_BGR2Lab);
    cv::Scalar mean=cv::mean(image);
    image.copyTo(i);
    cv::medianBlur(i,i,9);

    //image.convertTo(image,CV_32FC3,1.0/255.0,0);
    //image.copyTo(i);
    i.convertTo(i,CV_32FC3,1.0/255.0,0);
    std::vector<cv::Mat_<float> > d;
    cv::split(i,d);
    cv::add(d[0],-mean[0]/255.0,o1);
    cv::multiply(o1,o1,o1,1.0);
    cv::add(d[1],-mean[1]/255.0,o2);
    cv::multiply(o2,o2,o2,1.0);
    cv::add(d[2],-mean[2]/255.0,o3);
    cv::multiply(o3,o3,o3,1.0);
    cv::add(o1,o2,o1);
    cv::add(o1,o3,o3);
    cv::pow(o3,0.5,o1);
    //o1.convertTo(o1,CV_8U,255.0,0);
    //cv::normalize(o1,o1,0,255,cv::NORM_MINMAX);


    return o1;

}

/**
 * @brief function to calculate the local contrast weight map for image
 * @param image is input image
 * @return output image is weight map
 */
Mat LocalContrast(Mat s)
{

    Mat image;
    s.copyTo(image);
       std::vector<cv::Mat_<float> > d;

    cvtColor(image,image,CV_Lab2BGR);
      image.convertTo(image,CV_32FC3,1.0/255.0,0);
    cv::split(image,d);

     Mat blurred,diff;
     double sigma = 7, threshold = 0;
     cv::GaussianBlur(d[0], blurred, Size(), sigma, sigma);

     d[0].convertTo(d[0],CV_32F,1.0,0);

     blurred.convertTo(blurred,CV_32F,1.0,0);


     cv::absdiff(d[0],blurred,diff);
     //Mat sharpened = d[0] * (1 + amount) + blurred * (-amount);


     return diff;


}

Mat Saturation(Mat src)
{
    Mat image;
    src.copyTo(image);
    std::vector<cv::Mat_<float> > d;
    image.convertTo(image,CV_32FC3,1.0/255.0,0);
    cv::split(image,d);
    Mat_<float> tmp;
    cv::add(d[0],d[1],tmp);
    cv::add(tmp,d[2],tmp);
    tmp=tmp/3;

    Mat dst1,dst2,dst3;


//    cv::multiply(d[0]-tmp,d[0]-tmp,dst1,1,0);
//    cv::multiply(d[1]-tmp,d[1]-tmp,dst2,1,0);
//    cv::multiply(d[2]-tmp,d[2]-tmp,dst3,1,0);

    cv::addWeighted(d[0],1,tmp,-1,1,d[0]);
    cv::addWeighted(d[1],1,tmp,-1,1,d[1]);
    cv::addWeighted(d[2],1,tmp,-1,1,d[2]);

    cv::convertScaleAbs(d[0],dst1,1.0,0);
    cv::convertScaleAbs(d[1],dst2,1.0,0);
    cv::convertScaleAbs(d[2],dst3,1.0,0);

    //dst1.convertTo(dst1,CV_8U,255.0,0);
     cv::add(dst1,dst2,dst1);
     cv::add(dst1,dst3,dst1);

//    dst1.convertTo(dst1,CV_32F,1,0);
    dst1=dst1/3;
//      cv::pow(dst1,0.5,dst1);
     //cv::convertScaleAbs(dst1,dst1,255.0,0);
     //dst1.convertTo(dst1,CV_32F,1.0,0);
    return dst1;

}


int main(int argv, char** argc){
    /*std::string file = "images/testImage";

    for(int j = 1 ; j < 12 ; j++){
    std::stringstream fileName;
    fileName<<file;
    fileName<<j;
   fileName<<".png";

    std::cout<<fileName.str()<<std::endl;
    */

    string a1=(argc[1]);
    string a2=(argc[2]);

    string source=a1+"/"+a2;
    VideoCapture inputVideo(source);              // Open input
    //cv::VideoCapture inputVideo(0);
    if (!inputVideo.isOpened())
    {
        cout  << "Could not open the input video: " << source << endl;
        return -1;
    }

    //cv::namedWindow("Image", CV_WINDOW_NORMAL);
    //cv::namedWindow("input1", CV_WINDOW_NORMAL);
    //cv::namedWindow("iput2", CV_WINDOW_NORMAL);
    //cv::namedWindow("outout", CV_WINDOW_NORMAL);

    Mat src, res;
    vector<Mat> spl;
    Mat a;
    a.create(240,320,CV_8UC(3));
    inputVideo >> src;
    resize(src,a, a.size(), 0, 0, INTER_NEAREST);

    Mat testImage;

    color_correction::contrast_stretching p1;
    color_correction::gray_world p2;
    std::vector<float> upperThresh;
    upperThresh.push_back(0.015);
    upperThresh.push_back(0.015);
    upperThresh.push_back(0.5);

    HistogramEqualization p;




    int i=0;
    clock_t begin = clock();
    for(;;) //Show the image captured in the window and repeat
    {
        inputVideo >> src;              // read

        if (src.empty()) break;         // check if at end

        resize(src,a, a.size(), 0, 0, INTER_AREA);
        Mat a2;
        a.copyTo(a2);

        p.colorspace=HistogramEqualization::COLOR_SPACE::RGB;

        //push channels for which to perform local histogram equalization
        p.channel.push_back(2);
        p.channel.push_back(0);
        p.channel.push_back(1);
        p.limits.push_back(50*0.1);
        p.limits.push_back(50*0.1);
        p.limits.push_back(50*0.1);

        Mat ahe=p.AHE(src,nx,ny,bins,0,limit);

        imshow("output",ahe);
        cv::waitKey(1);
        i=i+1;
        if(i%30==0 && i>0)
        {
            clock_t end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            cerr << "a " << (double)30/elapsed_secs << endl;
            begin = clock();
            i=0;
        }

    }

    cv::waitKey(0);

    return 0;
}
