#ifndef REGIONGROWING_HPP
#define REGIONGROWING_HPP

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <sstream>

#include <opencv.hpp>
#include <highgui/highgui.hpp>

using namespace cv;
using namespace std;



class seed_fill
{


Mat input;            //input image
Mat output;         //output image
Scalar fill_color;  //seed pixel color
Mat edge;           //edge map
int threshold;      //set threshold default is 80

public:
seed_fill();
/*constructor for seed fill
a is input image,x and y are seed pixel locations
*/
Mat RegionGrowing(Mat a,int x,int y);


/**
    method to compute distance between color components
*/
float ColorDist1(Scalar a,Scalar b);

/**
  method to compute color difference between color components
  */
float ColorDist2(Scalar a,Scalar b);

/**
  method to compute color difference between color components
  */
float ColorDist3(Scalar a,Scalar b);

/**
  method to compute color difference between color components
*/


/*
updating the mean value of region
  */
float ColorDist(Scalar a,Scalar b,int x,int y);

Scalar meanx(Scalar a,Scalar b,int size);

/*
  method that gets the pixel value at location x,y
  */
Scalar GetPixel(int x,int y);

/*
   method which checks if the pixel at location x,y has been processed
  */
bool isSet(int x,int y);

/* setting the label for pixel at location x,y*/
void SetPixel(int x,int y);

/*setting the label for pixel at location x,y with color b*/
void SetPixel(int x,int y,Scalar b);

/* seed fill recurive function */
void SeedFill_1(int x, int y);


};


#endif // REGIONGROWING_HPP
