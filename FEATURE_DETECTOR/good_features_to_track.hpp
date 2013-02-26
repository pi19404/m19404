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
#define GOOD_FEATURES_TO_TRACK_HPP
#include "feature_detector.hpp"

namespace  feature_detector
{
class good_features_to_track : public feature_detector
{


int block_size;           // local block size used for eigen value computation
int aperture_size;      //aperture size for edge detection
double qualityLevel; //prameter for eigen value threshold
int borderType;
public:


//comparing values and returning points corresponding to sorted values
template<class T> struct index_cmp {
index_cmp(const T arr) : arr(arr) {}
bool operator()(const size_t a, const size_t b) const
{ return arr[a] > arr[b]; }
const T arr;
};

//constructor for good feature to track class
good_features_to_track()
{
    maxCorners=100;
    aperture_size=3;
    qualityLevel=0.01;
    minDistance=10;
    block_size=3;
    borderType=cv::BORDER_DEFAULT;
}
//constructor for the class good feature to track
good_features_to_track(uint _maxCorners,int _minDistance,int _aperture_size,double _qualityLevel,int _block_size)
{
    maxCorners=_maxCorners;
    aperture_size=_aperture_size;
    qualityLevel=_qualityLevel;
    minDistance=_minDistance;
    block_size=_block_size;
    borderType=cv::BORDER_DEFAULT;
}


//setter methods for aperture size
void setApertureSize(int value)
{
    aperture_size=value;
}
//setter methods for quality level
void setqualityLevel(double value)
{
    qualityLevel=value;
}
//setter methods for block size
void setblock_size(int value)
{
    block_size=value;
}


//implementation of method for feature detection for good feature to track
 vector<cv::Point2f> run(Mat src)
 {

     corners.resize(0);

     Mat dst;
     Mat tmp;
     dst.create( src.size(), CV_32F );

    Mat xx1(src.rows,src.cols,CV_8UC(3));
    xx1.setTo (cv::Scalar::all (0));



     //scaling by block size in x and y directions.
     //scaling by aperture size 2^(aperture size/2)
    double scale = (double)(1 << ((aperture_size > 0 ? aperture_size : 3) - 1)) * block_size;

     scale *=255.;
     scale = 1./scale;

        //computing the derivatives in x and y direction
        Mat Dx, Dy;
        Sobel( src, Dx,CV_32F, 1, 0, aperture_size, scale, 0, borderType );
        Sobel( src, Dy,CV_32F, 0, 1, aperture_size, scale, 0, borderType );

        Size size = src.size();

        //matrix containing matrix components at each point
        Mat matrix( size, CV_32FC3 );

        //accessing rows
        for(int i = 0; i < size.height; i++ )
        {
            //accessing pointer to the rows
           const float* dx1 = (const float*)(Dx.data + i*Dx.step);
           const float* dy1= (const float*)(Dy.data + i*Dy.step);
           float* vals = (float*)(matrix.data + i*matrix.step);

           //accessing the columns of the matrix
            for(int  j = 0; j < size.width; j++ )
            {
                    //assigning values to the elements of rows
                float dx = dx1[j];
                float dy = dy1[j];

                vals[j*3] = dx*dx;
                vals[j*3+1] = dx*dy;
                vals[j*3+2] = dy*dy;

                //computing the eigen values at each point


            }

        }

        //taking weighted average of pixel values to compute eigen values of block about the pixel
        //we can use other types of filters for weighted averages
        boxFilter(matrix, matrix, matrix.depth(), Size(block_size, block_size),Point(-1,-1), false, borderType );

        //computing the eigen values

        for(int  i = 0; i < size.height; i++ )
        {
            float* vals = (float*)(matrix.data + i*matrix.step);
            float* o1 = (float*)(dst.data + dst.step*i);
            for(int  j = 0; j < size.width; j++ )
            {
                float a = vals[j*3];
                float b = vals[j*3+1];
                float c = vals[j*3+2];

                double u = (a + c)*0.5;
                double v = std::sqrt((a - c)*(a - c)*0.25 + b*b);
                double l1 = u - v;      //minimum eigen values
                double l2 = u + v;    //maximum eigen values

                o1[j] = l2;                 //using the maximum eigen value to determine strong edge
            }
        }


        //thresholding the eigen values
        double maxVal = 0;
        minMaxLoc(dst, 0, &maxVal, 0, 0,Mat() );
        //threshold(dst,dst, maxVal*qualityLevel, 0, THRESH_TOZERO );

        // included in opencv code hence added

        dilate(dst,tmp, Mat());


        //selecting points at minimum distance from each other

        //if(1==1)
        {
        const int cell_size =cvRound(minDistance);
        int xx=floor(src.cols/cell_size);
        int yy=floor(src.rows/cell_size);

        std::vector  <double>eig;

        vector <Point2f> tmp_corners ;






        //accessing the cell blocks of size min distance

        for(int x=0;x<xx;x++)
        {
            for(int y=0;y<yy;y++)
            {
                int w=cell_size;
                if((x+1)*(cell_size)>src.cols)
                    w=(x+1)*(cell_size)-src.cols;
                int h=cell_size;
                if((y+1)*(cell_size)>src.rows)
                    h=(y+1)*(cell_size)-src.rows;

                Rect roi(x*cell_size,y*cell_size,w,h);
                //cerr <<  roi.x << ":" << roi.width <<":" <<  dst.cols << endl;
                //cerr <<  roi.y << ":" << roi.height <<":" <<  dst.rows << ":" << (y+1)*(cell_size) << ":" << src.rows <<endl;
                //selecting the subregion
                Mat r=dst(roi);
                //selecting the maximum eigen value pixel in the subregion
                //double maxVal;
                Point minLoc;
                double m1=0;
                minMaxLoc( r, 0, &m1,0,&minLoc, Mat());
                if(m1>=maxVal*qualityLevel)
                {
                //threshold(r,r,m1, 0, THRESH_TOZERO );
                minLoc.x=minLoc.x+x*cell_size;
                minLoc.y=minLoc.y+y*cell_size;
                //
                eig.push_back(m1);
                tmp_corners.push_back(minLoc);
                }

            }

        }
        //Mat tmpx;
        //cv::normalize(dst,tmpx,0,255,CV_MINMAX);
        //dst.convertTo(tmpx,dst.type(),255.0,0);





        if(eig.size()<maxCorners)
            maxCorners=(eig.size());

        //initialising index vector
        vector <int> b;
        for (unsigned i = 0; i < eig.size(); ++i)
        {

            b.push_back(i);
            circle(tmp,tmp_corners[i], 3, Scalar(255,255,255), -1, 8);

        }

        //sorting the eigen value vector
        sort(b.begin(), b.end(), index_cmp<vector<double>&>(eig));
        int ncorners=0;
        for (int i = 0; i < eig.size(); ++i)
        {
            bool good=true;
            //populating vectors of corners detected

            for(int k=0;k<corners.size();k++)
            {
                double dx=tmp_corners[b[i]].x-corners[k].x;
                double dy=tmp_corners[b[i]].y-corners[k].y;
                if( dx*dx + dy*dy < minDistance* minDistance )
                {
                    good =false;
                    break;

                }

            }


            if(good==true)
            {

                circle(xx1,tmp_corners[b[i]],3, Scalar(255,ncorners,ncorners), -1, 8);
                corners.push_back(tmp_corners[b[i]]);
                ncorners++;

            }

            if(ncorners >=maxCorners)
                break;
        }
}


        return corners;

 }


};
}
#endif // GOOD_FEATURES_TO_TRACK_HPP
