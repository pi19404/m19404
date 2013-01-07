/*
 * This module describes methods for different color constancy algorithms like gray_world,shades of gray,gray_edge,max_edge,max_edge and modified contrast stretching
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

/**
 * @file color_correction
 * @brief Class containing basic color correction/balance algorithms
 * @author pi19404 <pi19404@gmail.com>
 *
 */


#include "color_constancy.hpp"



Mat  color_correction::contrast_stretching::run(Mat src)
{


        Mat dst;
        src.copyTo(dst);

          Mat src1;
          src.copyTo(src1);


          int N=src.rows*src.cols;

          float s1=3,s2=3;





          vector<Mat> bgr_planes;
          split( src, bgr_planes );


          /// Establish the number of bins
          int histSize = 256;

          /// Set the ranges ( for B,G,R) )
          float range[] = { 0, 256} ;
          const float* histRange = { range };

          bool uniform = true; bool accumulate = false;

          Mat b_hist, g_hist, r_hist;
          float cbhist[histSize],cghist[histSize],crhist[histSize];
          int vmin1=0,vmin2=0,vmin3=0;
          int vmax1 = histSize-1 ,vmax2=histSize-1,vmax3=histSize-1;
          vmax2=vmax1;
          vmax3=vmax1;


          /// Compute the histograms:
          calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
          calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
          calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );



          //compute the CDF
          for(int i=0;i<histSize;i++)
          {
              if(i==0)
              {
                cbhist[i]=b_hist.at<float>(i);
                cghist[i]=g_hist.at<float>(i);
                crhist[i]=r_hist.at<float>(i);
              }
              else
              {
                cbhist[i]=(cbhist[i-1]+b_hist.at<float>(i));
                cghist[i]=(cghist[i-1]+g_hist.at<float>(i));
                crhist[i]=(crhist[i-1]+r_hist.at<float>(i));
               }

          }


          //find the lower and uppder saturation thresholds for all the three channels

          while( vmin1<histSize-1 && cbhist[vmin1] <= (float)N * s1 / 100)
          {
              vmin1 = vmin1 + 1;

          }

          while(vmax1 < histSize  && cbhist[vmax1] > (float)N * (1 - s2 / 100))
              vmax1 = vmax1 - 1;

          if(vmax1 < histSize -1)
              vmax1 =vmax1 + 1;


          while(vmin2<histSize-1 && cghist[vmin2] <= (float)N * s1 / 100)
              vmin2 = vmin2 + 1;

          while(vmax2 < histSize && cghist[vmax2] >(float) N * (1 - s2 / 100))
              vmax2 = vmax2 - 1;


          while(vmin3<histSize-1 && crhist[vmin3] <= (float)N * s1 / 100)
              vmin3 = vmin3 + 1;

          while(vmax3 < histSize &&  crhist[vmax3] > (float)N * (1 - s2 / 100))
              vmax3 = vmax3 - 1;



          if(vmax2 < histSize -1)
              vmax2 =vmax2 + 1;

          if(vmax3 < histSize -1)
              vmax3 =vmax3 + 1;






        //perform contrast stretching
        cstretch(src1,dst,vmin1,vmax1,0);
        cstretch(src1,dst,vmin2,vmax2,1);
        cstretch(src1,dst,vmin3,vmax3,2);

        //dst.copyTo(src);
        return dst;
    }



void color_correction::contrast_stretching::cstretch(Mat src,Mat dst,int min,int max,int index)
{
    int norm[256];
    if(max<=min)
    {
        cv::Mat_<cv::Vec3b>::const_iterator it= src.begin<cv::Vec3b>();
        cv::Mat_<cv::Vec3b>::const_iterator itend= src.end<cv::Vec3b>();
        cv::Mat_<cv::Vec3b>::iterator itout= dst.begin<cv::Vec3b>();

        for ( ; it!= itend; ++it, ++itout)
        {


            cv::Vec3b color1=*it;
            color1[index]=255/2;
            *itout=color1;



        }

    }
    else
    {


    int i=0;
    for (i = 0; i < min; i++)
          norm[i] = 0;

    for (i=min; i < max; i++)
    {
               norm[i] = ((i - min) * 255/((max - min) +0.5));
              // cerr << i << ":" << norm[i] << endl;
    }

    for (i = max; i < 255 + 1; i++)
                norm[i] = 255;

    cv::Mat_<cv::Vec3b>::const_iterator it= src.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::const_iterator itend= src.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itout= dst.begin<cv::Vec3b>();

    for ( ; it!= itend; ++it, ++itout)
    {

        cv::Vec3b color=*it;
        cv::Vec3b color1=*itout;
        //cerr << (int)color[2] <<"," <<norm[color[2]] << ":";
        color1[index]=norm[color[index]];
        *itout=color1;



    }

    }
}




void color_correction::gray_world::process(Mat src1,int p,float *ml,float *ma,float *mb)
{

    *ma=0;
    *mb=0;
    *ml=0;

      for(int i=0;i<src1.rows;i++)
      {

          for(int j=0;j<src1.cols;j++)
          {
              Vec3b v1=src1.at<cv::Vec3b>(i,j);
              float lc=pow(v1.val[0],p);
              float ac=pow(v1.val[1],p);
              float bc=pow(v1.val[2],p);
              *ma=*ma+ac;
              *mb=*mb+bc;
              *ml=*ml+lc;

          }
      }


      *ma=pow((float)*ma/(src1.cols*src1.rows),(float)1/p);
      *mb=pow((float)*mb/(src1.cols*src1.rows),(float)1/p);
      *ml=pow((float)*ml/(src1.cols*src1.rows),(float)1/p);



}



void color_correction::gray_world::process(Mat src1,float *ml,float *ma,float *mb,int p,int m)
{

     *ma=0;
     *mb=0;
     *ml=0;

       for(int i=0;i<src1.rows;i++)
       {

           for(int j=0;j<src1.cols;j++)
           {
               Vec3b v1=src1.at<cv::Vec3b>(i,j);
               float lc=pow(v1.val[0],p);
               float ac=pow(v1.val[1],p);
               float bc=pow(v1.val[2],p);
               *ma=*ma+ac;
               *mb=*mb+bc;
               *ml=*ml+lc;

           }
       }


       *ma=pow((float)*ma/(src1.cols*src1.rows),(float)1/p);
       *mb=pow((float)*mb/(src1.cols*src1.rows),(float)1/p);
       *ml=pow((float)*ml/(src1.cols*src1.rows),(float)1/p);

       (*ma)=128+(0.5*255 *(*ma));
       (*mb)=128+(0.5*255 *(*mb));
       (*ml)=128+(0.5*255 *(*ml));

       float r=0;

       if(m==0)
       {
           r=(*ma+*mb+*ml)/3;

           *ma=r/(*ma);
           *mb=r/(*mb);
           *ml=r/(*ml);

       }
       if(m==1)
       {
        r=(*ma+*mb+*ml)/3;
        r=max(*ma,*mb);
        r=max(r,*ml);

        *ma=r/(*ma);
        *mb=r/(*mb);
        *ml=r/(*ml);


       }
       if(m==2)
       {
           r=sqrt((*ma)*(*ma)+(*mb)*(*mb)+(*ml)*(*ml));
          *ma=(float)(*ma)/(float)r;
           *mb=(float)(*mb)/(float)r;
           *ml=(float)(*ml)/(float)r;

           cerr <<  *ml << endl;
           cerr <<  *ma << endl;
           cerr <<  *mb << endl;

           r=max(*ma,*mb);
           r=max(r,*ml);

           *ma=(float)r/(float)(*ma);
           *mb=(float)r/(float)(*mb);
           *ml=(float)r/(float)(*ml);
       }


}



Mat  color_correction::gray_world::run2(Mat src,int p,int m)
 {

    Mat src1;
    src.copyTo(src1);


     Mat dst;;

    src1.copyTo(dst);

     cv::Mat_<cv::Vec3b>::const_iterator it= src1.begin<cv::Vec3b>();
     cv::Mat_<cv::Vec3b>::const_iterator itend= src1.end<cv::Vec3b>();
     cv::Mat_<cv::Vec3b>::iterator itout= dst.begin<cv::Vec3b>();

     float ma=0,mb=0,ml=0;
     process(src1,&ml,&ma,&mb,p,m);
     for ( ; it!= itend; ++it, ++itout)
     {


         cv::Vec3b v1=*it;

         float l=v1.val[0];
         float a=v1.val[1];
         float b=v1.val[2];

         if(m==0)
         {
         a=a*(ma);
         b= b*(mb);
         l= l*(ml);
         }
         else
         {
         //if(a<(float)95*255/100)
         a=a*(ma);
         //if(b<(float)95*255/100)
         b= b*(mb);
        // if(l<(float)95*255/100)
         l= l*(ml);
         }

         if(a>255)
             a=255;
         if(b>255)
             b=255;
         if(l>255)
             l=255;


         v1.val[0]=l;

         v1.val[1]=a;

         v1.val[2]=b;

         *itout=v1;



     }


     return dst;
 }



Mat  color_correction::gray_world::run1(Mat src,int p)
{

      Mat src1,dst;
       src.copyTo(dst);
      src.copyTo(src1);


      vector<Mat> bgr_planes;

      float ma=0,mb=0,ml=0;



 cvtColor(src1,src1,CV_BGR2Lab);

     process(src1,p,&ml,&ma,&mb);
     ma=ma-128;
     mb=mb-128;

      ma=-ma;
      mb=-mb;
      cv::Mat_<cv::Vec3b>::const_iterator it= src1.begin<cv::Vec3b>();
      cv::Mat_<cv::Vec3b>::const_iterator itend= src1.end<cv::Vec3b>();
      cv::Mat_<cv::Vec3b>::iterator itout= dst.begin<cv::Vec3b>();



      for ( ; it!= itend; ++it, ++itout)
      {


          cv::Vec3b v1=*it;

          float l=(float)v1.val[0]*(float)100/(float)255;
          float a=v1.val[1]-128;
          float b=v1.val[2]-128;

          float a_delta =(float) ma * (float)(l/100) * (float)1.1;
          float b_delta =(float) mb * (float)(l/100) * (float)1.1;
          a=a+a_delta+128;
          b= b+b_delta+128;





          v1.val[0]=l*255/100;
          v1.val[1]=a;
          v1.val[2]=b;

          *itout=v1;



      }


      split( dst, bgr_planes );
      //cv::normalize(bgr_planes[1],bgr_planes[1],0,255,cv::NORM_MINMAX);
      //cv::normalize(bgr_planes[2],bgr_planes[2],0,255,cv::NORM_MINMAX);

      //cv::equalizeHist(bgr_planes[0],bgr_planes[0]);
      cv::merge(bgr_planes,dst);
      cvtColor(dst,dst,CV_Lab2BGR);




        return dst;
}



void color_correction::gray_edge::conv2(const Mat &img, const Mat& kernel, ConvolutionType type, Mat& dest) {
  Mat source = img;
  if(CONVOLUTION_FULL == type) {
    source = Mat();
    const int additionalRows = kernel.rows-1, additionalCols = kernel.cols-1;
    copyMakeBorder(img, source, (additionalRows+1)/2, additionalRows/2, (additionalCols+1)/2, additionalCols/2, BORDER_CONSTANT, Scalar(0));
  }

  Point anchor(kernel.cols - kernel.cols/2 - 1, kernel.rows - kernel.rows/2 - 1);
  int borderMode = BORDER_CONSTANT;
  flip(kernel,kernel,-1);
  filter2D(source, dest, img.depth(),kernel, anchor, 0, borderMode);

  if(CONVOLUTION_VALID == type) {
    dest = dest.colRange((kernel.cols-1)/2, dest.cols - kernel.cols/2)
               .rowRange((kernel.rows-1)/2, dest.rows - kernel.rows/2);
  }
}


void  color_correction::gray_edge::process(Mat src1,float *ml,float *ma,float *mb,int p,int m)
{

    *ma=0;
    *mb=0;
    *ml=0;


      Mat src;
      vector<Mat> bgr_planes;
      Mat src2;
        //src1.convertTo(src2,CV_32FC3,1.0/255.0,0);
      src1.convertTo(src,CV_32FC3,1.0,0);
      split( src, bgr_planes );

      Mat kernel=getGaussianKernel(100,6,CV_32F);


      Mat grad_x, grad_y,abs_angle_x;
      Mat grad1,grad2,grad3;
      Mat r1,g,b;

      cerr << bgr_planes[0].depth() << ":" << CV_32FC1<<  "dddddd" << endl;

      conv2( bgr_planes[0], kernel, CONVOLUTION_FULL,r1);
      conv2( bgr_planes[1], kernel, CONVOLUTION_FULL,g);
      conv2( bgr_planes[2], kernel, CONVOLUTION_FULL,b);

      //cv::imshow("dddd",b);
      // 1 2 1      1 0 -1
      // 0 0 0      2 0 -2
      // -1 -2 -1   1 0 -1
      double dx[]={1,2,1,0,0,0,-1,-2,-1};
      double dy[]={1,0,-1,2,0,-2,1,0,-1};

      Mat kx=Mat(3,3,CV_32FC1,dx);
      Mat ky=Mat(3,3,CV_32FC1,dy);


      grad_x.setTo(cv::Scalar::all(0));
      grad_y.setTo(cv::Scalar::all(0));

      conv2(r1, kx, CONVOLUTION_FULL,grad_x);
      conv2(r1, ky, CONVOLUTION_FULL,grad_y);


     cartToPolar(grad_x,grad_y,bgr_planes[0],abs_angle_x,false);
     grad_x.setTo(cv::Scalar::all(0));
     grad_y.setTo(cv::Scalar::all(0));

      conv2(g, kx, CONVOLUTION_FULL,grad_x);
      conv2(g, ky, CONVOLUTION_FULL,grad_y);

      cartToPolar(grad_x,grad_y,bgr_planes[1],abs_angle_x,false);
      grad_x.setTo(cv::Scalar::all(0));
      grad_y.setTo(cv::Scalar::all(0));

      conv2(b, kx, CONVOLUTION_FULL,grad_x);
      conv2(b, ky, CONVOLUTION_FULL,grad_y);

      cartToPolar(grad_x,grad_y,bgr_planes[2],abs_angle_x,false);







      cv::merge(bgr_planes,src);

      for(int i=0;i<src.rows;i++)
      {

          for(int j=0;j<src.cols;j++)
          {
              Vec3f v1=src.at<cv::Vec3f>(i,j);
              double lc=pow(v1.val[0],p);
              double ac=pow(v1.val[1],p);
              double bc=pow(v1.val[2],p);
              *ma=*ma+ac;
              *mb=*mb+bc;
              *ml=*ml+lc;
              //cerr << lc << ":" ;
//              *ml=max((double)*ml,(double)lc);
//              *ma=max((double)*ma,(double)ac);
//              *mb=max((double)*mb,(double)bc);
          }
      }


      *ma=pow((double)*ma,(double)1/p);
      *mb=pow((double)*mb,(double)1/p);
      *ml=pow((double)*ml,(double)1/p);


      double r=0;

      if(m==0)
      {
          r=(*ma+*mb+*ml)/3;

          *ma=r/(*ma);
          *mb=r/(*mb);
          *ml=r/(*ml);
          cerr <<  *ml << endl;
          cerr <<  *ma << endl;
          cerr <<  *mb << endl;

      }
      if(m==1)
      {
       r=(*ma+*mb+*ml)/3;
       r=max((double)*ma,(double)*mb);
       r=max((double)r,(double)*ml);

       *ma=r/(*ma);
       *mb=r/(*mb);
       *ml=r/(*ml);
       cerr <<  *ml << endl;
       cerr <<  *ma << endl;
       cerr <<  *mb << endl;

      }
      if(m==2)
      {
          r=sqrt((*ma)*(*ma)+(*mb)*(*mb)+(*ml)*(*ml));
         *ma=(double)(*ma)/(double)r;
          *mb=(double)(*mb)/(double)r;
          *ml=(double)(*ml)/(double)r;

          cerr <<  *ml << endl;
          cerr <<  *ma << endl;
          cerr <<  *mb << endl;

          r=max((double)*ma,(double)*mb);
          r=max((double)r,(double)*ml);

          *ma=sqrt((double)r/(double)(*ma));
          *mb=sqrt((double)r/(double)(*mb));
          *ml=sqrt((double)r/(double)(*ml));
      }




}


Mat  color_correction::gray_edge::run(Mat src1,int p,int m)
{

    Mat dst;

   src1.copyTo(dst);

    cv::Mat_<cv::Vec3b>::const_iterator it= src1.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::const_iterator itend= src1.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itout= dst.begin<cv::Vec3b>();

    float ma=0,mb=0,ml=0;
    process(src1,&ml,&ma,&mb,p,m);
    for ( ; it!= itend; ++it, ++itout)
    {


        cv::Vec3b v1=*it;

        float l=v1.val[0];
        float a=v1.val[1];
        float b=v1.val[2];

        if(m==0)
        {
        a=a*(ma);
        b= b*(mb);
        l= l*(ml);
        }
        else
        {
        //if(a<(float)95*255/100)
        a=a*(ma);
        //if(b<(float)95*255/100)
        b= b*(mb);
       // if(l<(float)95*255/100)
        l= l*(ml);
        }

        if(a>255)
            a=255;
        if(b>255)
            b=255;
        if(l>255)
            l=255;


        v1.val[0]=l;

        v1.val[1]=a;

        v1.val[2]=b;

        *itout=v1;



    }

 return dst;
}


Mat  color_correction::maxRGB::run(Mat src1,int p,int m)
{

    Mat dst;

   src1.copyTo(dst);

    cv::Mat_<cv::Vec3b>::const_iterator it= src1.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::const_iterator itend= src1.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itout= dst.begin<cv::Vec3b>();

    float ma=0,mb=0,ml=0;
    process(src1,&ml,&ma,&mb,p,m);
    for ( ; it!= itend; ++it, ++itout)
    {


        cv::Vec3b v1=*it;

        float l=v1.val[0];
        float a=v1.val[1];
        float b=v1.val[2];

        if(m==0)
        {
        a=a*(ma);
        b= b*(mb);
        l= l*(ml);
        }
        else
        {
        //if(a<(float)95*255/100)
        a=a*(ma);
        //if(b<(float)95*255/100)
        b= b*(mb);
       // if(l<(float)95*255/100)
        l= l*(ml);
        }

        if(a>255)
            a=255;
        if(b>255)
            b=255;
        if(l>255)
            l=255;


        v1.val[0]=l;

        v1.val[1]=a;

        v1.val[2]=b;

        *itout=v1;



    }

    return dst;
}



void  color_correction::maxRGB::process(Mat src1,float *ml,float *ma,float *mb,int p,int m)
{

    Mat src;

    src1.convertTo(src,CV_32FC3,1.0,0);

    *ma=0;
    *mb=0;
    *ml=0;

      for(int i=0;i<src.rows;i++)
      {

          for(int j=0;j<src.cols;j++)
          {
              Vec3f v1=src.at<cv::Vec3f>(i,j);
              double lc=v1.val[0];
              double ac=v1.val[1];
              double bc=v1.val[2];
              *ml=max((double)*ml,(double)lc);
              *ma=max((double)*ma,(double)ac);
              *mb=max((double)*mb,bc);

          }
      }





      if(m==1)
      {
              double r=(*ma+*mb+*ml)/3;
            r=(double)sqrt((double)(*ma)*(*ma)+(double)(*mb)*(*mb)+(double)(*ml)*(*ml));
           *ma=(double)(*ma)/(double)r;
            *mb=(double)(*mb)/(double)r;
            *ml=(double)(*ml)/(double)r;


            r=max((double)*ma,(double)*mb);
            r=max((double)r,(double)*ml);

            *ma=(double)(r)/(double)*ma;
            *mb=(double)(r)/(double)*mb;
            *ml=(double)(r)/(double)*ml;



      }
    else
      {


          cerr <<  *ml << endl;
          cerr <<  *ma << endl;
          cerr <<  *mb << endl;

          double r=max((double)*ml,(double)*ma);
          r=max((double)r,(double)*mb);
          *ma=(double)(r)/(double)*ma;
          *mb=(double)(r)/(double)*mb;
          *ml=(double)(r)/(double)*ml;


      }


}



Mat  color_correction::max_edge::run( Mat src1,int p,int m)
{

    Mat dst;

   src1.copyTo(dst);

    cv::Mat_<cv::Vec3b>::const_iterator it= src1.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::const_iterator itend= src1.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itout= dst.begin<cv::Vec3b>();

    float ma=0,mb=0,ml=0;
    process(src1,&ml,&ma,&mb,p,m);
    for ( ; it!= itend; ++it, ++itout)
    {


        cv::Vec3b v1=*it;

        float l=v1.val[0];
        float a=v1.val[1];
        float b=v1.val[2];

        if(m==0)
        {
        a=a*(ma);
        b= b*(mb);
        l= l*(ml);
        }
        else
        {
        //if(a<(float)95*255/100)
        a=a*(ma);
        //if(b<(float)95*255/100)
        b= b*(mb);
       // if(l<(float)95*255/100)
        l= l*(ml);
        }

        if(a>255)
            a=255;
        if(b>255)
            b=255;
        if(l>255)
            l=255;


        v1.val[0]=l;

        v1.val[1]=a;

        v1.val[2]=b;

        *itout=v1;



    }

   return dst;
}



void color_correction::max_edge::conv2(const Mat &img, const Mat& kernel, ConvolutionType type, Mat& dest) {
  Mat source = img;
  if(CONVOLUTION_FULL == type) {
    source = Mat();
    const int additionalRows = kernel.rows-1, additionalCols = kernel.cols-1;
    copyMakeBorder(img, source, (additionalRows+1)/2, additionalRows/2, (additionalCols+1)/2, additionalCols/2, BORDER_CONSTANT, Scalar(0));
  }

  Point anchor(kernel.cols - kernel.cols/2 - 1, kernel.rows - kernel.rows/2 - 1);
  int borderMode = BORDER_CONSTANT;
  flip(kernel,kernel,-1);
  filter2D(source, dest, img.depth(),kernel, anchor, 0, borderMode);

  if(CONVOLUTION_VALID == type) {
    dest = dest.colRange((kernel.cols-1)/2, dest.cols - kernel.cols/2)
               .rowRange((kernel.rows-1)/2, dest.rows - kernel.rows/2);
  }
}


void  color_correction::max_edge::process(Mat src1,float *ml,float *ma,float *mb,int p,int m)
{

    *ma=0;
    *mb=0;
    *ml=0;


      Mat src;
      vector<Mat> bgr_planes;
      Mat src2;
        //src1.convertTo(src2,CV_32FC3,1.0/255.0,0);
      src1.convertTo(src,CV_32FC3,1.0,0);
      split( src, bgr_planes );

      Mat kernel=getGaussianKernel(100,6,CV_32F);


      Mat grad_x, grad_y,abs_angle_x;
      Mat grad1,grad2,grad3;
      Mat r1,g,b;

      cerr << bgr_planes[0].depth() << ":" << CV_32FC1<<  "dddddd" << endl;

      conv2( bgr_planes[0], kernel, CONVOLUTION_FULL,r1);
      conv2( bgr_planes[1], kernel, CONVOLUTION_FULL,g);
      conv2( bgr_planes[2], kernel, CONVOLUTION_FULL,b);

      //cv::imshow("dddd",b);
      // 1 2 1      1 0 -1
      // 0 0 0      2 0 -2
      // -1 -2 -1   1 0 -1
      double dx[]={1,2,1,0,0,0,-1,-2,-1};
      double dy[]={1,0,-1,2,0,-2,1,0,-1};

      Mat kx=Mat(3,3,CV_32FC1,dx);
      Mat ky=Mat(3,3,CV_32FC1,dy);


      grad_x.setTo(cv::Scalar::all(0));
      grad_y.setTo(cv::Scalar::all(0));

      conv2(r1, kx, CONVOLUTION_FULL,grad_x);
      conv2(r1, ky, CONVOLUTION_FULL,grad_y);


     cartToPolar(grad_x,grad_y,bgr_planes[0],abs_angle_x,false);
     grad_x.setTo(cv::Scalar::all(0));
     grad_y.setTo(cv::Scalar::all(0));

      conv2(g, kx, CONVOLUTION_FULL,grad_x);
      conv2(g, ky, CONVOLUTION_FULL,grad_y);

      cartToPolar(grad_x,grad_y,bgr_planes[1],abs_angle_x,false);
      grad_x.setTo(cv::Scalar::all(0));
      grad_y.setTo(cv::Scalar::all(0));

      conv2(b, kx, CONVOLUTION_FULL,grad_x);
      conv2(b, ky, CONVOLUTION_FULL,grad_y);

      cartToPolar(grad_x,grad_y,bgr_planes[2],abs_angle_x,false);







      cv::merge(bgr_planes,src);

      for(int i=0;i<src.rows;i++)
      {

          for(int j=0;j<src.cols;j++)
          {
              Vec3f v1=src.at<cv::Vec3f>(i,j);
              double lc=v1.val[0];
              double ac=v1.val[1];
              double bc=v1.val[2];
//              *ma=*ma+ac;
//              *mb=*mb+bc;
//              *ml=*ml+lc;
              //cerr << lc << ":" ;
              *ml=max((double)*ml,(double)lc);
              *ma=max((double)*ma,(double)ac);
              *mb=max((double)*mb,(double)bc);
          }
      }


//      *ma=pow((double)*ma,(double)1/p);
//      *mb=pow((double)*mb,(double)1/p);
//      *ml=pow((double)*ml,(double)1/p);


      double r=0;

      if(m==0)
      {
          r=(*ma+*mb+*ml)/3;

          *ma=r/(*ma);
          *mb=r/(*mb);
          *ml=r/(*ml);
          cerr <<  *ml << endl;
          cerr <<  *ma << endl;
          cerr <<  *mb << endl;

      }
      if(m==1)
      {
       r=(*ma+*mb+*ml)/3;
       r=max((double)*ma,(double)*mb);
       r=max((double)r,(double)*ml);

       *ma=r/(*ma);
       *mb=r/(*mb);
       *ml=r/(*ml);
       cerr <<  *ml << endl;
       cerr <<  *ma << endl;
       cerr <<  *mb << endl;

      }
      if(m==2)
      {
          r=sqrt((*ma)*(*ma)+(*mb)*(*mb)+(*ml)*(*ml));
         *ma=(double)(*ma)/(double)r;
          *mb=(double)(*mb)/(double)r;
          *ml=(double)(*ml)/(double)r;

          cerr <<  *ml << endl;
          cerr <<  *ma << endl;
          cerr <<  *mb << endl;

          r=max((double)*ma,(double)*mb);
          r=max((double)r,(double)*ml);

          *ma=sqrt((double)r/(double)(*ma));
          *mb=sqrt((double)r/(double)(*mb));
          *ml=sqrt((double)r/(double)(*ml));
      }




}
