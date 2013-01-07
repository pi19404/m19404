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

#include "HistogramEqualization.hpp"
#include "stdio.h"
Mat HistogramEqualization::plot_hist(Mat image)
{
     Mat b_hist, g_hist, r_hist;
    /// Establish the number of bins
    int histSize = 256;
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    /// Set the ranges ( for B,G,R) )


    bool uniform = true; bool accumulate = false;

     int hist_w = 320; int hist_h = 240;
     int bin_w = cvRound( (double) hist_w/histSize );

     Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

     Mat src;
     vector<Mat> bgr_planes;
     cvtColor(image,src,CV_BGR2HSV_FULL);
     split( src,bgr_planes );

     calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );



    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

      /// Draw for each channel
      for( int i = 1; i < histSize; i++ )
      {
          line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                           Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                           Scalar( 0, 0, 255), 2, 8, 0  );
      }
    return histImage;
}

Mat HistogramEqualization::run(Mat image,int type)
{
    Mat dst,src;
    Mat hist;
    if(type==GLOBAL)
    {
        vector<Mat> bgr_planes;
        cvtColor(image,src,CV_BGR2HSV_FULL);
        split( src,bgr_planes );


        cv::equalizeHist(bgr_planes[2],bgr_planes[2]);
        cv::merge(bgr_planes,src);
        cv::cvtColor(src,dst,CV_HSV2BGR_FULL);

    }
    else if(type==LOCAL)
    {

    }
    return dst;
}



void HistogramEqualization::make_lut(Mat r,int bins)
{
Mat dst;
lut.create(256,1,CV_8UC(1));
lut.setTo(cv::Scalar::all(0));
double min,max;
Point i1,i2;
cv::minMaxLoc(r,&min,&max,&i1,&i2);

int binsize=ceil(((max-min)/bins));
lut.setTo(cv::Scalar::all(0));
for(int i=min;i<=max;i++)
{
    lut.at<uchar>(i,1)=(((float)(i-min)*bins/(float)(max-min)));

}
//LUT(r,lut,dst);
//return dst;
}

Mat HistogramEqualization::make_histogram(Mat image,int bins)
{
    Mat hist1;
    hist1.create(bins,1,CV_32FC(1));
    hist1.setTo(cv::Scalar::all(0));


    for(int i=0;i<image.rows;i++)
    {
        for(int j=0;j<image.cols;j++)
        {
            int val0=image.at<uchar>(i,j);
            int val1=lut.at<uchar>(val0,1);
            hist1.at<float>(val1,1)=round(hist1.at<float>(val1,1))+1;


        }
    }
    return hist1;
}




Mat HistogramEqualization::interpolate(Mat image,int xsize,int ysize,int xdivs,int ydivs,int mode)
{
    int yu=0,yl=0,xr=0,xl=0;
    int subX=0,subY=0;
    int xi=0,yi=0;
    for(int j=0;j<=ydivs;j++)
    {

        if(j==0)//top row
        {
            subY=ysize/2;
            yu=0;
            yl=0;
        }
        else if(j==ydivs) //bottom row
        {
            subY=ysize/2;
            yu=ydivs-1;
            yl=ydivs-1;
        }
        else    //other rows
        {
            subY=ysize;
            yu=j-1;
            yl=yu+1;
        }
        xi=0;
        for(int i=0;i<=xdivs;i++)
        {

            if(i==0)    //left
            {
                subX=xsize/2;
                xr=i;
                xl=i;

            }
            else if(i==xdivs) //right column
            {
                subX=xsize/2;
                xr=xdivs-1;
                xl=xdivs-1;
            }
            else
            {
                subX=xsize;
                xl=i-1;
                xr=xl+1;
            }

            Mat h1=hist[(yu*xdivs)+xl];

            Mat h2=hist[(yu*xdivs)+xr];

            Mat h3=hist[(yl*xdivs)+xl];

            Mat h4=hist[(yl*xdivs)+xr];



            unsigned int x1=0,y1=0,x2=0,y2=0;



            Rect roi(xi,yi,subX,subY);
            Mat ix=image(roi);

            xi=xi+subX;

            for(int l=0;l<subY;l++)
            {
                y1=l;
                y2=subY-y1;
                for(int m=0;m<subX;m++)
                {

                   int val0=(int)ix.at<uchar>(l,m);

                   int val=(int)lut.at<uchar>(val0,1);

                    float val1=h1.at<float>(val,1);

                    float val2=h2.at<float>(val,1);

                    float val3=h3.at<float>(val,1);

                    float val4=h4.at<float>(val,1);



                    x1=m;
                    x2=subX-x1;

                    if(mode==1)
                    {
                    float fval=((x2*y2)*val1)+(x1*y2*val2)+(y1*x2*val3)+(x1*y1*val4);

                    ix.at<uchar>(l,m)=ceil((float)fval/(float)(subX*subY));
                    }
                    else if(mode==0)
                    {
                        ix.at<uchar>(l,m)=val;
                    }
                    else if(mode==2)
                    {
                        ix.at<uchar>(l,m)=val4;
                    }



                }
            }
        }
        yi=yi+subY;
    }

    return image;
}



Mat HistogramEqualization::clip_histogram(unsigned  long clip,Mat hist1,int bins)
{
    Mat hist;
    hist1.copyTo(hist);
    //hist.setTo(cv::Scalar::all(0));
    int index=0;
    long excess=0;


            index=index+1;
            for(int k=0;k<bins;k++)
            {

                long nexcess=hist1.at<float>(k,1)-clip;
                if(nexcess>0)
                    excess=nexcess+excess;      //count excess number of bins

            }

        //

            //clip is desired level of histogram
            //incr is average increment for pixels below clip
            //if peak is above clip saturate it
            //if peak is below clip saturate it to clip

    if(excess>0)
    {
    unsigned long BinIncr = ceil(excess / bins);              /* average binincrement */
    if(BinIncr==0)
        BinIncr=1;
    unsigned long upper =  clip - BinIncr;                    /* Bins larger than ulUpper set to cliplimit */


    for (int i = 0; i < bins; i++) {
    float val=hist1.at<float>(i,1);
        if (val > (unsigned long)clip)
        {
          val = (unsigned long)clip;
        }
      else
        {
            if(val > upper && val<clip) {              /* high bin count */
                if(excess>0)
                {
                    excess-=val-upper;
                }
              val=clip;
          }
          else {                                        /* low bin count */
              float v=val;
              if(excess>0)
              {
              excess=excess-BinIncr;
              val = val+BinIncr;
              }
              //if(BinIncr<0)


          }

        }
        hist.at<float>(i,1)=val;
       }


    if(excess>0)
    {
double old_excess;
int index1=0;
do{
    index1=0;
    old_excess=excess;
    while(excess>0)
    {

        int step=round(bins/excess);  //average number of bins excess/bins
        if(step<1)
            step=1;
        for(int i=index1;i<bins;i=i+step)
        {
            float val=hist.at<float>(i,1);
            if(val<clip)
            {
                excess=excess-1;
                val=val+1;
            }

            hist.at<float>(i,1)=val;
            if(excess<1)
                break;

         }
        index1=index1+1;
    }
}while ((excess>1) && (excess < old_excess));
    }
            }
            else
            {
                    hist1.copyTo(hist);
            }
    return hist;
}

Mat HistogramEqualization::map_histogram(int bins,int Min,int Max,unsigned long size,Mat hist1)
{
        Mat hist2;
        hist1.copyTo(hist2);

        for (int i = 0; i < bins; i++) {
            hist2.at<float>(i,1)=0;
        }


        unsigned long ulSum=0;
        const float fScale = (float)(((float)(Max - Min)) / (float)size);

        unsigned long ulMin =  Min;
        for (int i = 0; i < bins; i++) {
            ulSum =ulSum+hist1.at<float>(i,1);

            hist2.at<float>(i,1)=((float)ulMin+(float)(ulSum*fScale));

            if((int)hist2.at<float>(i,1) >Max)
                hist2.at<float>(i,1)=(float)Max;

        }
        return hist2;
}

Mat HistogramEqualization::AHE(Mat image,int xdivs,int ydivs,int bins,int mode,float limit)
{

    Mat dst,src;

    image.copyTo(src);
    vector<Mat> bgr_planes;
    cvtColor(src,src,CV_BGR2HSV_FULL);
    split( src,bgr_planes );





    Mat b_hist, g_hist, r_hist;
   /// Establish the number of bins
   int histSize = 128;
   /// Set the ranges ( for B,G,R) )
   float range[] = { 0, 256 } ;
   const float* histRange = { range };
   /// Set the ranges ( for B,G,R) )


   bool uniform = true; bool accumulate = false;


    int width=src.cols;
    int height=src.rows;

    //check if image dimensions are multiples of xdivs and ydivs
    int newW=width,newH=height;
    if(width%xdivs!=0)
        newW = width+(width%xdivs);
    if(height%ydivs!=0)
        newH = height+(height%ydivs);



    Mat r;
    r.create(newH,newW,CV_8UC(1));
    cv::resize(bgr_planes[2],r,r.size(),0,0,CV_INTER_CUBIC);

    double max=0,min=0;
    cv::Point i1;
    cv::minMaxLoc(r,&min,&max,&i1,&i1);



    make_lut(r,bins);


    unsigned long xsize=((float)newW/(float)xdivs);
    unsigned long ysize=((float)newH/(float)ydivs);
    hist.resize(xdivs*ydivs);


    for(int j=0;j<ydivs;j++)
    {


        for(int i=0;i<xdivs;i++)
        {

            Rect roi(i*xsize,j*ysize,xsize,ysize);

            Mat roiImg=r(roi);


            Mat result=make_histogram(roiImg,bins);
            Mat result1;
            unsigned long clip=((float)(limit*xsize*ysize)/(float)bins);

            if(limit>1)
            result1=clip_histogram(clip,result,bins);
            else
            result1=result;
            unsigned long size1=(unsigned long)xsize * (unsigned long)ysize;

            Mat o=map_histogram(bins,(int)min,(int)max,(unsigned long)size1,result1);
            o.copyTo(hist[(j*xdivs)+i]);


        }

    }


    interpolate(r,xsize,ysize,xdivs,ydivs,mode);



    cv::resize(r,bgr_planes[2],bgr_planes[0].size(),0,0,CV_INTER_CUBIC);

    cv::resize(bgr_planes[1],r,r.size(),0,0,CV_INTER_CUBIC);

    max=0,min=0;
    cv::minMaxLoc(r,&min,&max,&i1,&i1);



    make_lut(r,bins);


    xsize=((float)newW/(float)xdivs);
    ysize=((float)newH/(float)ydivs);
    hist.resize(xdivs*ydivs);


    for(int j=0;j<ydivs;j++)
    {


        for(int i=0;i<xdivs;i++)
        {

            Rect roi(i*xsize,j*ysize,xsize,ysize);

            Mat roiImg=r(roi);


            Mat result=make_histogram(roiImg,bins);
            Mat result1;
            unsigned long clip=((float)(limit*xsize*ysize)/(float)bins);

            if(limit>1)
            result1=clip_histogram(clip,result,bins);
            else
            result1=result;
            unsigned long size1=(unsigned long)xsize * (unsigned long)ysize;

            Mat o=map_histogram(bins,(int)min,(int)max,(unsigned long)size1,result1);
            o.copyTo(hist[(j*xdivs)+i]);


        }

    }


    interpolate(r,xsize,ysize,xdivs,ydivs,mode);



    cv::resize(r,bgr_planes[1],bgr_planes[1].size(),0,0,CV_INTER_CUBIC);

    cv::merge(bgr_planes,dst);
    cv::cvtColor(dst,dst,CV_HSV2BGR_FULL);


    return dst;
}
