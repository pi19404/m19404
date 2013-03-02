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
#ifndef REGIONGROWING_HPP
#define REGIONGROWING_HPP

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <sstream>

#include <opencv.hpp>
#include <highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <deque>
#include <stack>
using namespace cv;
using namespace std;


class seed_fill
{

protected:
Mat input;            //input image
Mat output;         //output image

Mat edge;           //edge map
int threshold;      //set threshold default is 80
int bsize;              //local neighborhood size for computing mean value
public:
Scalar fill_color;  //seed pixel color
/*constructor for seed fill
  */
seed_fill();

/*set threhsold */
void set_threhsold(int value)
{
    threshold=threshold;
}

/*
a is input image,x and y are seed pixel locations
*/
Mat RegionGrowing(Mat a,int x,int y,Scalar color);


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

bool check_color(int x,int y);
void SeedFill_3(int x, int y);
int nMinX, nMaxX, nMinY, nMaxY;
void LineFill_3(int x1, int x2, int y,Scalar fill_color);
};

//class defining the element held in stack
class line_element
{
public :
    int left;
    int right;
    int y;
    int nexty;

    //constructor for class
    line_element(int l1,int r1,int y2,int n1)
    {
        left=l1;
        right=r1;
        y=y2;
        nexty=n1;
    }
};

//class for stack based method for seeded region growing algorithm
class seed_fill_stack : public seed_fill
{
public:
  int bsize;            //variable defining local neighborhood size for mean edge and color computation
  stack<line_element>lines;     //stack containing line segment information
  void SeedFill_4(int x, int y);     //scan line algorithm
  Mat RegionGrowing(Mat a,int x,int y,Scalar color); //main function to be called for region growing

  seed_fill_stack()
  {
      //default value of local neighborhood size for mean value computation
      bsize=5;
  }
};


#endif // REGIONGROWING_HPP
