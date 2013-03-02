#include "fast.h"

feature_detector::fast::fast()
{

    qualityLevel=0.1;
     maxCorners=100;
     minDistance=10;
    threshold=10;
    K=12;
    for( int i = -255; i <= 255; i++ )
        threshold_tab[i+255] = (uchar)(i < -threshold ? 1 : i > threshold ? 2 : 0);

    offset.push_back (Point2f(0,-3));
    offset.push_back (Point2f(1,-3));
    offset.push_back (Point2f(2,-2));
    offset.push_back (Point2f(3,-1));
    offset.push_back (Point2f(3,0));
    offset.push_back (Point2f(3,1));
    offset.push_back (Point2f(2,2));
    offset.push_back (Point2f(1,3));
    offset.push_back (Point2f(0,3));
    offset.push_back (Point2f(-1,3));
    offset.push_back (Point2f(-2,2));
    offset.push_back (Point2f(-3,1));
    offset.push_back (Point2f(-3,0));
    offset.push_back (Point2f(-3,-1));
    offset.push_back (Point2f(-2,-2));
    offset.push_back (Point2f(-1,-3));



init=false;
}

vector<Point2f> feature_detector::fast::run (Mat src)
{

    if(init==false)
{
        int a=0;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=8;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=2;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=10;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=4;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=12;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=6;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=14;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=1;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=9;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=3;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=11;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=5;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=13;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=7;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        a=15;checks.push_back ((int)offset[a].y*src.step+(int)offset[a].x);
        init=true;

        for(int i=0;i<offset.size ();i++)
            checks1.push_back ((int)offset[i].y*src.step+(int)offset[i].x);
    }
    vector<Point2f> corners;

    Mat dst;
    src.copyTo (dst);
    dst.convertTo (dst,CV_32FC(1),1,0);
    dst.setTo (cv::Scalar::all (0));
    //int value[offset.size ()];
    //parsing the image rejecting the border pixels
        for(int i = 3; i < src.rows-2; i++)
        {

            float * ptr1=dst.ptr <float>(i);
            uchar * ptr=src.data+i*src.step;
            for(int j=3;j<src.cols-2;j++)
            {
                //getting the value of the test point
                int v1=ptr[j];
                int flag=1;
                int a,b;

                for(int k=0;k<checks.size ();k=k+2)
                {

                a=checks[k];
                b=checks[k+1];
                int value1=threshold_tab[-ptr[j+a]+v1+255];
                int value2=threshold_tab[-ptr[j+b]+v1+255];
                flag=flag&(value1|value2);
                if(flag==0)
                break;

                }

                if(flag==0)
                continue;



                int count=0;
                int response_tmp=0;
                for(int k=0;k<2;k++)
                {

                count=0;
                response_tmp=0;
                //check line segment
                for(int l=0;l<offset.size();l++)
                {
                    //if pixel < test_pixel - t
                    int index=j+checks1[l];
                    int x=ptr[index];

                    int flag1=(k==0)?(x<v1-threshold):(x>v1+threshold);
                    count=(flag1==1)?count+1:0; //set count to 0 if no suitable pixels found
                    response_tmp=(flag1==1)?response_tmp+std::abs(x-v1-threshold):0;
                    if(count > K ) break;	//12 continous pixel detected
                }

                if(count>K)
                {
                //corners1.push_back(Point2f(j,i));

                ptr1[j]=response_tmp;
                //response.push_back (response_tmp);
                break;
                }

                }
            }
        }



        //thresholding the eigen values
        double maxVal = 0,minVal=0;
        minMaxLoc(dst, &minVal, &maxVal, 0, 0,Mat() );
        //cerr << maxVal <<":" <<minVal << endl;
        //threshold(dst,dst, maxVal*qualityLevel, 0, THRESH_TOZERO );

        // included in opencv code hence added

        //dilate(dst,tmp, Mat());


        //selecting points at minimum distance from each other

        //if(1==1)
        Mat tmp;
        src.copyTo (tmp);
        cvtColor(tmp,tmp,CV_GRAY2BGR);
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


        if(eig.size()<maxCorners)
            maxCorners=(eig.size());

        //initialising index vector
        vector <int> b;
        for (unsigned i = 0; i < eig.size(); ++i)
        {

            b.push_back(i);
//            circle(tmp,tmp_corners[i], 3, Scalar(255,255,255), -1, 8);

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
                    //cv::circle(tmp,tmp_corners[b[i]], 3, Scalar(255,k,0), -1);
                    good =false;
                    break;

                }

            }


            if(good==true)
            {

//                circle(xx1,tmp_corners[b[i]],3, Scalar(255,ncorners,ncorners), -1, 8);
                corners.push_back(tmp_corners[b[i]]);
                ncorners++;

            }

            if(ncorners >=maxCorners)
                break;
        }
}


        //imshow("XXXX",tmp);
//corner filtering
        //cerr << ":" << corners.size () << endl;

return corners;
}

