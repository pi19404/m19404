
#include <iostream>


#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <sstream>

using namespace std;
using namespace cv;

//comparing values and returning points corresponding to sorted values
template<class T> struct index_cmp1 {
index_cmp1(const T arr) : arr(arr) {}
bool operator()(const size_t a, const size_t b) const
{ return arr[a] > arr[b]; }
const T arr;
};


/*
Mat DistanceMatrix(int num_features,cv::Point2f features)
{
    Mat distance;
    distance.create(num_features,num_features,CV_32FC(1));

    float r1,r2,c1,c2;
    for (int row=0; row<num_features; row++) {
        r1=features[row].x;
        r2=features[row].y;
        for (int col=row+1; col<num_features; col++)
        {
            c1=features[col].x;
            c2=features[col].y;
            double dx = c1-r1;
            double dy= c2-r2;
           distances.put(row,col,Math.sqrt(dx*dx+dy*dy));
        }

    }


}

 bool TooCloseToOthers(int x,int y,int indx,int len,CvPoint2D32f features)
  {


    //  double a2[]=new double[2];
      for (int fcnt=0; fcnt<len; fcnt++) {
          if (indx==fcnt) continue;

          features.position(fcnt);
          //a2=features.get();
          double dx = x-features.x();
          double dy = y-features.y();
          double dist = Math.sqrt((dx*dx)+(dy*dy));
              if (dist<feature_extract.get_min_distance()) {
                return true;
            }
      }


      return false;
  }

vector<Point2f> FindFeatures(Mat eig,int num_features,int minDistance)
{
    vector <Point2f> corners;
    //corners.resize(num_features);
    Mat distance=DistanceMatrix(num_features,features);
int cnt=0;
    for (int fcnt=0; fcnt<num_features; fcnt++) {
             for (int scnd=fcnt+1; scnd<num_features; scnd++) {
                 double dist;
                 if (fcnt<scnd) {

                     const float* dx1 = (const float*)(distance.data + fcnt*distance.step);
                     dist=dx1[scnd];


                 }
                 else if(fcnt>scnd)
                 {
                     const float* dx1 = (const float*)(distance.data + scnd*distance.step);
                     dist=dx1[fcnt];

                 }
                 else
                 {
                     continue;
                 }


                 bool too_close = dist<minDistance;
                 if(too_close==true)
                  {
                     break;
                 }

             }

             if(too_close==true)
              {
                 break;
             }
             else
             {
                 corners.push_back(features[fcnt]);
                 cnt++;
             }

    }

    return corners;
}
        else if(1==2)
        {
            Mat eig;
            dst.copyTo(eig);
            Mat image;
            src.copyTo(image);
            Size imgsize = src.size();

            vector<const float*> tmpCorners;

        //collect a list of pointers to eigenvalues that are not edge pixels
            for( int y = 1; y < imgsize.height - 1; y++ )
            {
                const float* eig_data = (const float*)eig.ptr(y);
                const float* tmp_data = (const float*)tmp.ptr(y);
                //const uchar* mask_data = mask.data ? mask.ptr(y) : 0;

                for( int x = 1; x < imgsize.width - 1; x++ )
                {
                    float val = eig_data[x];
                    if( val != 0 && val == tmp_data[x] )
                        tmpCorners.push_back(eig_data + x);
                }
            }

            //sort the eigen values in descending order
            sort( tmpCorners, greaterThanPtr<float>() );

            size_t i, j, total = tmpCorners.size(), ncorners = 0;

                 // Partition the image into larger grids
                int w = image.cols;
                int h = image.rows;

                const int cell_size = cvRound(minDistance);
                const int grid_width = (w + cell_size - 1) / cell_size;
                const int grid_height = (h + cell_size - 1) / cell_size;

                std::vector<std::vector<Point2f> > grid(grid_width*grid_height);

                minDistance *= minDistance;

                for( i = 0; i < total; i++ )
                {

                    int ofs = (int)((const uchar*)tmpCorners[i] - eig.data);
                    int y = (int)(ofs / eig.step);
                    int x = (int)((ofs - y*eig.step)/sizeof(float));

                    bool good = true;

                    int x_cell = x / cell_size;
                    int y_cell = y / cell_size;

                    int x1 = x_cell - 1;
                    int y1 = y_cell - 1;
                    int x2 = x_cell + 1;
                    int y2 = y_cell + 1;

                    // boundary check
                    x1 = std::max(0, x1);
                    y1 = std::max(0, y1);
                    x2 = std::min(grid_width-1, x2);
                    y2 = std::min(grid_height-1, y2);

                    for( int yy = y1; yy <= y2; yy++ )
                    {
                        for( int xx = x1; xx <= x2; xx++ )
                        {
                            vector <Point2f> &m = grid[yy*grid_width + xx];

                            if( m.size() )
                            {
                                for(j = 0; j < m.size(); j++)
                                {
                                    float dx = x - m[j].x;
                                    float dy = y - m[j].y;

                                    if( dx*dx + dy*dy < minDistance )
                                    {
                                        good = false;
                                        goto break_out;
                                    }
                                }
                            }
                        }
                    }

                    break_out:

                    if(good)
                    {
                        // printf("%d: %d %d -> %d %d, %d, %d -- %d %d %d %d, %d %d, c=%d\n",
                        //    i,x, y, x_cell, y_cell, (int)minDistance, cell_size,x1,y1,x2,y2, grid_width,grid_height,c);
                        grid[y_cell*grid_width + x_cell].push_back(Point2f((float)x, (float)y));

                        corners.push_back(Point2f((float)x, (float)y));
                        ++ncorners;

                        if( maxCorners > 0 && (int)ncorners == maxCorners )
                            break;
                    }
                }
            }
        else if(1==2)
        {

            //find the feature with maximum eigen values which at specified distance from each other
            //corners=FindFeatures(maxCorners,minDistance,eig);

        }


*/


int main1()
{

    vector<float> a;
    a.push_back(8.1); a.push_back(0.1); a.push_back(7.1);
    vector <int> b;
    vector <int>c;
    for (unsigned i = 0; i < a.size(); ++i)
    {
    //b.push_back(i,Point(i,i);
   b.push_back(i);
    c.push_back(i);
    }
    sort(b.begin(), b.end(), index_cmp1<vector<float>&>(a));
    for (unsigned i = 0; i < a.size(); ++i)
    cerr << ":" << b.at(i) ;
    cerr << endl;

}
