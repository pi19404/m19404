#ifndef LAPLACIANBLEND
#define LAPLACIANBLEND

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

//Class that contains methods for Laplacian blend - assumes two images + corresponding masks
class LaplacianBlending {
    private:
        Mat_<Vec3f> left;
        Mat_<Vec3f> right;
        Mat_<float> leftMask;
        Mat_<float> rightMask;

        vector<Mat_<Vec3f> > leftLapPyr,rightLapPyr,resultLapPyr;
        Mat leftSmallestLevel, rightSmallestLevel, resultSmallestLevel;
        vector<Mat_<Vec3f> > leftMaskGaussianPyr; //masks are 3-channels for easier multiplication with RGB
        vector<Mat_<Vec3f> > rightMaskGaussianPyr; //masks are 3-channels for easier multiplication with RGB
        int levels;

        //Methods
        //Building pyramids
        void buildPyramids();
        //Method for building gaussian pyramid
        void buildGaussianPyramid();
        //Building laplcian pyramids for the image
        void buildLaplacianPyramid(const Mat& img, vector<Mat_<Vec3f> >& lapPyr, Mat& smallestLevel);
        //Reconstructing image from pyramid
        Mat_<Vec3f> reconstructImgFromLapPyramid();
        //Method to blending the pyramids
        void blendLapPyrs();

    public:
        //Constructor
        LaplacianBlending(const Mat_<Vec3f>& _left, const Mat_<Vec3f>& _right, const Mat_<float>& _leftMask, const Mat_<float>& _rightMask, int _levels);
        //Reconstructiong method
        Mat_<Vec3f> blend();
    };

#endif
