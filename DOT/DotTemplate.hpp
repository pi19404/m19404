#ifndef CV_DOT_TEMPLATE
#define CV_DOT_TEMPLATE
#include <iostream>

#include <iostream>
#include <iostream>

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <sstream>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/imgproc/imgproc.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <sstream>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/imgproc/imgproc.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write
using namespace std;
using namespace cv;


class DOT{

    Size regionDim;     //size of the region
    Size Samples;       //size of samples in horizontal and veritcal directions

    int S;
    Size _templateSize;

    Size templates;
    int N;
    int  M;
    int G;


    vector<vector<unsigned char> > template_list;


    int numOfGradientBins;
    int samplingSize ;
    int numOfGradients;
    double minimumGradientMagnitude;

public:

    char bits_set_in_16bit [0x1u<<16];
    char bits_unset_in_16bit [0x1u<<16];


    int iterated_bitcount( short int a_n )
        {
            int l_count=0;

            while(a_n)
            {
                l_count += a_n & 0x1u;
                a_n >>= 1;
            }
            return l_count;
        }

        void compute_bits_set_in_16bit( void )
        {
            for( unsigned int l_i=0; l_i<(0x1u<<16); ++l_i )
            {
                bits_set_in_16bit[l_i] = iterated_bitcount(l_i);
            }
            for( unsigned int l_i=0; l_i<(0x1u<<16); ++l_i )
            {
                bits_unset_in_16bit[l_i] = 16-iterated_bitcount(l_i);
            }
            return;
        }


    DOT()
    {

        regionDim.width=7;
        regionDim.height=7;
        numOfGradientBins=8;
         samplingSize = 7;
         numOfGradients=7;
         G=121;     //number of used samples
         setSamplingSize(7);
         compute_bits_set_in_16bit();

    }

    int  getSamplingSize()
    {
        return  S;
    }

    int getWidth()
    {
        return S*M;
    }

    int getHeight()
    {
        return S*N;
    }

    void setSamplingSize(int size)
    {
        S=size;
        M=154/S;
        N=154/S;
    }


    vector<unsigned char> createTemplateFast(Mat image,Rect box)
    {



        //required for orientation indexing
        const float divisor = 180.0f/(numOfGradientBins-1.0f);
        //vector to store DOT
        vector<unsigned char> discretizedData;
        vector<unsigned char> maxg;

        discretizedData.resize (M*N);
        maxg.resize (M*N);
        std::vector<unsigned char>::iterator dataIter = discretizedData.begin ();
        std::vector<unsigned char>::iterator dataIter1 = maxg.begin ();



        Mat temp=image(box);
        Mat resize_image=Mat(S*(N+1)+2,S*(M+1)+2,image.type ());

        cv::resize (temp,resize_image,resize_image.size(),0,0,INTER_CUBIC);
        //computing the derivative image
        Mat magnitude,angle;
        derivativeImage (resize_image,magnitude,angle);

        Mat tmp_magnitude;
       magnitude.copyTo (tmp_magnitude);

        //M & N is the number of region in the x and y directions
        //S is the number of samples per region about each dimension

        //center of the region
        int l_off_x=1+S/2;
        int l_off_y=1+S/2;

        int D=ceil(S/2);

         float gradientMagnitudeThreshold = 0.9f;
        float globalMaxGradient = 0.0f;

        int count=0;
        //loop over the regions
         for( int l_r=0; l_r<N; ++l_r )
        {
             //cerr << "1***********************" <<endl;
          for( int l_c=0; l_c<M; ++l_c )
          {
              //cerr << "2***********************" <<endl;
              ///divide into overlapping regions ,this loop can be parallelized
              for( int l_y=-D; l_y<=D; l_y+=D )
              {
                  int l_yall = l_off_y+l_y;

                  for( int l_x=-D; l_x<=D; l_x+=D )
                  {
                      int l_xall		= l_off_x+l_x;
                      int l_counter	= 0;


                      //cerr << l_xall <<":" << l_yall << ":" <<l_xall+S <<":" << l_yall+S <<":" << magnitude.rows <<": " << magnitude.cols << endl;


                      double minValue,maxValue;
                      Rect r=Rect(l_xall,l_yall,S,S);

                      Mat roi=magnitude(r);
                      cv::minMaxLoc (roi,&minValue,&maxValue,0,0);
                      double localMaxGradient=maxValue;


                    if (localMaxGradient > globalMaxGradient)
                    {
                        globalMaxGradient=localMaxGradient;
                    }

                    *dataIter1+=localMaxGradient;

                      Point maxGradientPos;
                      float maxGradient;

                      while (l_counter < numOfGradients)
                      {

                          r=Rect(l_xall,l_yall,S,S);
                          roi=magnitude(r);
                          maxValue=0;
                          minValue=0;
                          Point  maxLocation;
                          cv::minMaxLoc (roi,&minValue,&maxValue,0,&maxLocation);
                          maxGradient = static_cast<float> (maxValue);
                          maxGradientPos.x = maxLocation.x;
                          maxGradientPos.y = maxLocation.y;

                          //no reliable gradients detected set the bit 7 to 1
                              if (maxGradient < minimumGradientMagnitude)
                             {
//                                  *dataIter |= 1 << (numOfGradientBins-1);
                                  break;
                              }
                              if(maxGradient < localMaxGradient*gradientMagnitudeThreshold || l_counter >=numOfGradients)
                              {
                                  break;
                              }
                            //set the bit corresponding to orientation direction to 1
                                    float * data=angle.ptr<float> (maxGradientPos.y+l_yall);
                                    float val=data[maxGradientPos.x+l_xall];
                                    const int angle = static_cast<int>(val-0.5f);
                                    const int binIndex = static_cast<int> ((angle >= 180.0f ? (angle-180.0f) : angle)/divisor);

                                    *dataIter |= 1 << binIndex;


                            data=magnitude.ptr<float> (maxGradientPos.y+l_yall);
                            data[maxGradientPos.x+l_xall]=-1;
                            ++l_counter;

                      }


                  }
               }

              if(*dataIter!=0)
              {

              }
              ++dataIter;
              ++dataIter1;

              count++;
                l_off_x += S;
          }

                    l_off_y += S;
                    l_off_x  = S/2+1;
        }



         //set smallest gradients of the template to zero such that we only
         //have NUM_OF_USED_SAMPLES bins with non-zero information
         //restoring the magnitude image

         tmp_magnitude.copyTo (magnitude);
         for (int binCounter = 0; binCounter < ((M*N)-G); ++binCounter)
         {
                int minIndex=0;
                double minValue=10e100;

                //finding the element corresponding to minimum gradient magnitude
                int min_pos = distance(maxg.begin(),min_element(maxg.begin(),maxg.end()));
                minIndex=min_pos;
                minValue=maxg[minIndex];
                maxg[minIndex]=10e100;
                discretizedData[minIndex]=0;
         }


         return discretizedData;
    }


    //to be done
    //ignore the gradient at  the borders
    void derivativeImage(Mat input,Mat &magnitude,Mat &angle)
    {

        Mat dx,dy;
        cv::GaussianBlur (input,input,Size(5,5),1);
        cv::Sobel (input,dx,CV_32F,1,0,3);
        cv::Sobel (input,dy,CV_32F,0,1,3);

        //caculating magnitude and orientation of each channel seperately
        Mat mag,ang;
        cv::cartToPolar (dx,dy,mag,ang,true);


        Mat mag1,ang1;
        ang1=Mat(ang.rows,ang.cols,CV_32FC(1));
        mag1=Mat(mag.rows,mag.cols,CV_32FC(1));

        vector<Mat> ch1;




        for(int i=0;i<mag.rows;i++)
        {
            float *ptr=mag.ptr <float>(i);
            float *ptr1=ang.ptr <float>(i);
            for(int j=0;j<mag.cols;j++)
            {
                float max=ptr[j*3];
                float max1=ptr1[j*3];

                if(ptr[j*3+1]>max)
                {
                    max=ptr[j*3+1];
                    max1=ptr1[j*3+1];
                }
                if(ptr[j*3+2]>max)
                {
                    max=ptr[j*3+2];
                    max1=ptr1[j*3+2];

                }

                ptr1[j*3]=max1;
                ptr[j*3]=max;

            }
        }

        cv::split(mag,ch1);
        ch1[0].copyTo(magnitude);
        cv::split(ang,ch1);
        ch1[0].copyTo(angle);


    }




    vector<unsigned char> computeDOT(Mat image)
    {

        Mat magnitude,angle;
        derivativeImage (image,magnitude,angle);

        //compute the dominant orientation for 7x7 regions in the image
        vector<unsigned char> ctemplate=computeGradient(magnitude,angle);

        //process this list to find region with the best match
        return ctemplate;

    }

    class candidate
    {
        int response;
        Rect roi;
    };

    //functionn to compute the template for 7x7 region in the source image
    vector<unsigned char> computeGradient(Mat input,Mat ang,int numOfGradients=1)
    {


        //byte data to be returned
        vector<unsigned char> &discretizedData;

        Samples.width=(input.cols-2)/S;
        Samples.height=(input.rows-2)/S;

        // discretize
         int offsetX = 1;
         int offsetY = 1;

         const float divisor = 180.0f/(numOfGradientBins-1.0f);
         //iterate over the regions in the image
         std::vector<unsigned char>::iterator dataIter = discretizedData.begin();
         for (int rowIndex = 0; rowIndex <  Samples.height; ++rowIndex)
          {



            for (int colIndex = 0; colIndex < Samples.width; ++colIndex)
            {
              int counter = 0;

              Point maxGradientPos;
              float maxGradient;
              while (counter < numOfGradients)
              {




                  Rect r=Rect(offsetX,offsetY,samplingSize,samplingSize);
                  Mat roi=input(r);
                  double maxValue=0,minValue;
                  Point  maxLocation;
                  cv::minMaxLoc (roi,&minValue,&maxValue,0,&maxLocation);
                  maxGradient = static_cast<float> (maxValue);
                  maxGradientPos.x = maxLocation.x;
                  maxGradientPos.y = maxLocation.y;

                  //no reliable gradients detected set the bit 7 to 1
                  if (maxGradient < minimumGradientMagnitude)
                 {
                      *dataIter |= 1 << (numOfGradientBins-1);
                      break;
                  }
                  //set the bit corresponding to orientation direction to 1
                  else
                  {
                        float *data=ang.ptr<float> (maxGradientPos.y+offsetY);
                        float val=data[maxGradientPos.x+offsetX];
                         const int angle = static_cast<int>(val);
                         const int binIndex = static_cast<int> ((angle >= 180.0f ? (angle-180.0f) : angle)/divisor);

                         *dataIter |= 1 << binIndex;
                  }

                  float *data=input.ptr<float> (maxGradientPos.y+offsetY);
                  data[maxGradientPos.x+offsetX]=-1;
                ++counter;

              }

            ++dataIter;
              //move along x direction to next region
            offsetX += S;
          }

            //cerr << "***************" <<endl;
            //move along the y direction to the next region
          offsetY += S;
          //go back to start index along the x direction
          offsetX = 1;
          //break;
    }

         return discretizedData;

    }


    void DetectTemplate(vector<unsigned char> ctemplate)
    {
        //copy the parts of the image agains which templates are required to be computed
        //this forms the candidate regions for template matching


        vector<unsigned char> discretizedData=template_list[0];


        //find the template,template dimensions are MxN
        for( int l_r=0; l_r<magnitude.rows/N; ++l_r )
        {
            for( int l_c=0; l_c<magnitude.cols/M; ++l_c )
            {
                //determine the co-ordinate of the ROI
                discretizedData.resize (M*N);
                //extracting the data from the DOT  computed over the entire image
                for(int i=0;i<N;i++)
                {
                    for(int j=0;j<M;j++)
                    {
                        unsigned char c1=ctemplate[(l_r+i)*M+l_c+j];
                        unsigned char t1=discretizedData[i*M+j];
                        //performing bitwise and
                        unsigned char result= c1 &t1;
                        result=result &1;

                        //checking if first 7 bits are non zero


                    }
                }
                //perform template matching



                //compare with zero

                //count number of zero


            }
        }
    }

    Mat process(Mat image,Rect box,bool &flag)
    {
        //computing the number of regions along the horizontal and vertical directions

        Mat im;
        image.copyTo (im);
        Mat magnitude,angle;
        vector<char> discretizedData;
        //computing the derivatives

        //calculating the Dominant orientation templates
        //createTemplate(magnitude,angle,discretizedData);







        if(box.x>-1 && box.y >-1 && box.x+box.width <im.cols && box.y+box.height <im.rows && box.width>0&&box.height>0)
        {

        Mat temp=im(box);
        Mat template_image;

        cv::resize (temp,template_image,Size(getWidth(),getHeight()),0,0,INTER_CUBIC);

        derivativeImage(template_image,magnitude,angle);

        if(flag==true)
        {
           flag=false;
           vector<unsigned char>template_dot=createTemplateFast (im,box);
           template_list.resize (1);
           template_list[0]=template_dot;
        }

        if(template_list.size ()>0)
        {

            //compting the dominant orientation in the input image for each sub region
            vector<unsigned char> c_template=computeDOT(im);
            //detecting the template
            detectTemplate(c_template);


        }
        cv::normalize(magnitude,magnitude,0,255,CV_MINMAX);
        cv::cvtColor (magnitude,magnitude,CV_GRAY2BGR);
        cv::resize (magnitude,temp,Size(temp.cols,temp.rows),0,0,INTER_AREA);
        temp.copyTo (im(box));




        //result.setTo (cv::Scalar::all (0));
        }
        //imshow("image",image);

        return im;


        //imshow("der",angle);


    }


};






#endif
