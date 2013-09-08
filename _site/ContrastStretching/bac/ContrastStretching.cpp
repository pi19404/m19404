#include "ContrastStretching.hpp"



ContrastStretching::ContrastStretching()
{
    _mode=Mode(Mode::DEFAULT);
    lowerRatio.push_back (0.015);
    lowerRatio.push_back (0.015);
    lowerRatio.push_back (0.03);
    upperRatio.push_back (0.015);
    upperRatio.push_back (0.015);
    upperRatio.push_back (0.03);
    threshold=70;
}

void ContrastStretching::setmode(Mode mode)
{
    _mode=mode;
}

Mode ContrastStretching::getmode()
{
    return _mode;
}

Scalar ContrastStretching::getMin()
{
    return _min;
}

Scalar ContrastStretching::getMax()
{
    return _max;
}

Scalar ContrastStretching::getLUT(Scalar a)
{
    Scalar _tmp;
    _tmp[0]=_LUT[a[0]*_channels];
    _tmp[1]=_LUT[a[1]*_channels+1];
    _tmp[2]=_LUT[a[2]*_channels+2];
    return _tmp;
}


Mat ContrastStretching::GlobalContrastStretching(Mat image)
{


    if(_image.rows==0 || _image.cols==0)
    {
        _image=image.clone ();
        _channels=_image.channels ();
        _rows=_image.rows;
        _cols=_image.cols;
    }
    else
    {
        _image=image.clone ();
    }
    if(_LUT.size ()!=256*_image.channels ())
    {
        _LUT.resize (_image.channels ()*256);
    }

        cv::split(_image,_channel);
        MakeGlobalLUT();
        uchar * ptr=_image.data;;

        for(int i=0;i<_rows;i++)
        {
            for(int j=0;j<_cols;j++)
            {
                for(int c=0;c<_channels;c++)
                ptr[i*_image.step+j*_channels+c]=_LUT[_channels*ptr[i*_image.step+j*_channels+c]+c];
            }
        }


    return _image;

}


void ContrastStretching::MakeGlobalLUT()
{


    if(_mode==Mode::DEFAULT)
    {
    for(int i=0;i<_channels;i++)
    {
        cv::minMaxLoc (_channel[i],&_min1,&_max1,0,0);
        _min[i]=_min1;
        _max[i]=_max1;
        for(int k=0;k<256;k++){
             _LUT[k*_channels+i]=k-_min1>0?k>_max1?255:255*(k-_min1)/(_max1-_min1):0;
        }
    }
    }
    else if(_mode==Mode::V1)
    {
        for(int i=0;i<_channels;i++)
        {
                Histogram imgHist;
                std::vector<int> imgThresh;
                imgThresh = imgHist.getThresh(_channel[i], lowerRatio[i], upperRatio[i]);
                _min1=0;
                _max1=0;
                cv::minMaxLoc (_channel[i],&_min1,&_max1,0,0);
                _min[i]=imgThresh[0];
                _max[i]=imgThresh[1];
                for(int k=0;k<256;k++)
                {
                    //if(_max1>threshold)
                    {
                        //if(_max[i]<threshold/2)
                          //  _max[i]=threshold/2;
                        _LUT[k*_channels+i]=(k-_min[i]<0)?0:(k>_max[i])?255:(255*(k-_min[i])/(_max[i]-_min[i]));
                    }
                      ///else
                         //    _LUT[k*_channels+i]=k;
                }
        }

    }


}

void ContrastStretching::set_channels(vector<int> channel)
{
    channels.clear ();
    channels=channel;
}


Mat ContrastStretching::AHE(Mat image,int xdivs,int ydivs,int bins)
{

    Mat dst,src;

    image.copyTo(src);
    vector<Mat> bgr_planes;
    split( src,bgr_planes );

    int width=src.cols;
    int height=src.rows;

    //check if image dimensions are multiples of xdivs and ydivs
    int newW=width,newH=height;
    if(width%xdivs!=0)
        newW = width+(width%xdivs);
    if(height%ydivs!=0)
        newH = height+(height%ydivs);


//Histogram imgHist;
vector < vector<uchar> > hist;
std::vector<int> imgThresh_global[3];
setmode(Mode::V1);
//getting the global thresholds for channels
Histogram imgHist;
imgThresh_global[0] = imgHist.getThresh(bgr_planes[0], lowerRatio[0],upperRatio[0]);
imgThresh_global[1] = imgHist.getThresh(bgr_planes[1], lowerRatio[1],upperRatio[1]);
imgThresh_global[2] = imgHist.getThresh(bgr_planes[2], lowerRatio[2],upperRatio[2]);



Mat r;
r.create(newH,newW,CV_8UC(3));
cv::resize(src,r,r.size(),0,0,CV_INTER_CUBIC);


unsigned long xsize=((float)newW/(float)xdivs);
unsigned long ysize=((float)newH/(float)ydivs);
 hist.resize(xdivs*ydivs);


    for(int j=0;j<ydivs;j++)
    {
        for(int i=0;i<xdivs;i++)
        {
            Rect roi(i*xsize,j*ysize,xsize,ysize);
            Mat roiImg=r(roi);
            cv::split(roiImg,_channel);
            for(int k=0;k<_channels;k++)
            {

                if(channels[k]==1)
                {
                    Histogram imgHist;
                    std::vector<int> imgThresh;
                    imgThresh = imgHist.getThresh(_channel[k],lowerRatio[k], upperRatio[k]);
                    cv::minMaxLoc (_channel[k],&_min1,&_max1,0,0);
                    _min[k]=imgThresh[0];
                    _max[k]=imgThresh[1];
                    hist[(j*xdivs)+i].resize (_image.channels ()*256);
                    for(int l=0;l<256;l++)
                    {
                        if(_max1>2*threshold)
                        {
                            if(_max[k]<2*threshold/2)
                            {
                                _max[k]=2*threshold/2;
      //                          _min[k]=imgThresh_global[k][0];
                            }
                            else
                            {
                                _max[k]=imgThresh_global[k][1];
    //                            _min[k]=imgThresh_global[k][0];
                            }
                            hist[(j*xdivs)+i][l*_channels+k]=l-_min[k]<0?0:l>_max[k]?255:(255*(l-_min[k])/(_max[k]-_min[k]));

                        }
                               else
                                 _LUT[k*_channels+i]=k;
                    }
                }
            }

            }

    }

   cv::Mat r1= interpolate1(r,xsize,ysize,xdivs,ydivs,1,hist);

   cv::resize(r1,src,src.size(),0,0,CV_INTER_CUBIC);
   cv::split(src,_channel);
   setmode(Mode::V1);
   Mat out=GlobalContrastStretching(src);
   cv::split(out,bgr_planes);
    for(int i=0;i<3;i++)
    {
        if(channels[i]==0)
        {
            bgr_planes[i].copyTo(_channel[i]);
        }
}
    cv::merge(_channel,dst);

    return dst;

}


Mat ContrastStretching::interpolate1(Mat image,int xsize,int ysize,int xdivs,int ydivs,int mode,vector< vector<uchar> > hist)
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

            vector<uchar> h1=hist[(yu*xdivs)+xl];
            vector<uchar> h2=hist[(yu*xdivs)+xr];
            vector<uchar> h3=hist[(yl*xdivs)+xl];
            vector<uchar> h4=hist[(yl*xdivs)+xr];



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


                    cv::Vec3b pixel1=ix.at<cv::Vec3b>(l,m);


                    for(int c=0;c<3;c++)
                    {

                        if(channels[c]==1)
                        {
                    int val=(int)pixel1.val[c];

                   //int val=(int)lut.at<uchar>(val0,1);

                    float val1=h1[_channels*val+c];
                   float val2=h2[_channels*val+c];
                   float val3=h3[_channels*val+c];
                   float val4=h4[_channels*val+c];
                    x1=m;
                    x2=subX-x1;
                    //pixel.


                    float fval=((x2*y2)*val1)+(x1*y2*val2)+(y1*x2*val3)+(x1*y1*val4);



                    pixel1.val[c]=ceil((float)fval/(float)(subX*subY));;
                        }

                }

                    ix.at<cv::Vec3b>(l,m)=pixel1;

            }
        }
        }
        yi=yi+subY;
    }

    return image;
}







