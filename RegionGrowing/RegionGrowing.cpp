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

#include "RegionGrowing.hpp"


seed_fill::seed_fill()
{
    threshold=5;
    bsize=10;
    dist_scale=4;
    flag=false;
}

/** main function to call for region growing */
Mat seed_fill::RegionGrowing(Mat a,int x,int y,Scalar color)
{
    flag=false;
    //setting up input and output matrices
    a.copyTo(input);
    a.copyTo(output);

    //cv::Canny(input,edge,150,255,3,true);
    area=0;
    nMinY=0;
    nMinX=0;
    nMaxY=input.rows-1;
    nMaxX=input.cols-1;
    int morph_size=1;
    Mat element = getStructuringElement( cv::MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    ///cv::morphologyEx (input,input,cv::MORPH_OPEN,element,Point(1,1),1);

    output.setTo(cv::Scalar::all(0));

    //getting seed pixel values
    fill_color=GetPixel(x,y);
    fill_color=color;

    //generating the edge map
    Mat edge1;
    cv::GaussianBlur (input,input,Size(3,3),1);
    cv::Laplacian (input,edge1,input.depth (),1,1);

    //
    //cv::split(input,im);
    //cv::morphologyEx (edge1,edge1,cv::MORPH_CLOSE,element,Point(1,1),1);
    vector <Mat> em;
    cv::split(edge1,em);cv::add (em[0],em[1],edge);cv::add (edge,em[2],edge);
    edge=edge;



    double K=32;
    edge.convertTo (edge,CV_32FC(3),-1.0/(255.0*K),0);
    cv::exp (edge,edge);
    cv::threshold(edge,edge, std::exp(-1/K)+0.001, 0, THRESH_TOZERO );
    edge=1-edge;
    //imshow("edge map",edge);
    edge=255*edge;


    double maxedge,minedge;
    cv::minMaxLoc (edge,&minedge,&maxedge,0,0);
    edge.convertTo (edge,CV_8UC(1),1,0);

    SeedFill_1(x,y);

    cvtColor(output,output,CV_BGR2GRAY);
    input.setTo (Scalar(0,255,255),output);


    return input;
}

/**
    method to compute distance between color components
*/
float seed_fill::ColorDist1(Scalar a,Scalar b)
{
    float dist=0;
    a[0]=0;
    b[0]=0;
    float n=a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
    float d1=(((a[0]*a[0])+(a[1]*a[1])+(a[2]*a[2])));
    float d2=(((b[0]*b[0])+(b[1]*b[1])+(b[2]*b[2])));
    d1=sqrt(d1);
    d2=sqrt(d2);
    if(d1 < 0.01 || d2<0.01)
        dist=0;
    else
        dist=n/(d1*d2);
    dist=dist*dist;
    dist=1-dist;
    dist=255*sqrt(dist);
    return dist;
}


/**
  method to compute color difference between color components
  */
float seed_fill::ColorDist2(Scalar a,Scalar b)
{
    float a1=a[2]-a[1];
    float a2=a[1]-a[0];
    float a3=a[0]-a[2];

    float b1=b[2]-b[1];
    float b2=b[1]-b[0];
    float b3=b[0]-b[2];



    float dist=(((a1-b1)*(a1-b1)+(a2-b2)*(a2-b2)+(a3-b3)*(a3-b3)));
    dist=sqrt(dist/3);
    return dist;
}


/**
  method to compute color difference between color components
  */
float seed_fill::ColorDist3(Scalar a,Scalar b)
{
    float a1=a[2];
    float a2=a[1];
    float a3=a[0];

    float b1=b[2];
    float b2=b[1];
    float b3=b[0];



    float dist=(((a1-b1)*(a1-b1)+(a2-b2)*(a2-b2)+(a3-b3)*(a3-b3)));
    dist=sqrt(dist/3);
    //cerr << dist <<":" << endl;
    return dist;
}


float seed_fill::ColorDist(Scalar a,Scalar b,int x,int y)
{


    float dist=0;
    float dist1=0;

    Scalar c;
    c[0]=255;c[1]=255,c[2]=255;

    //computing color distance from seed pixel
    dist=ColorDist3(a,b);
    //computing distance from white pixel
    dist1=ColorDist3(a,c);



    int width=input.cols;
    int height=input.rows;

    //Computing the mean value of color and edge map


    int s1=x-bsize/2>=0?x-bsize/2:0;
    int s2=y-bsize/2>=0?y-bsize/2:0;
    int s3=(x-bsize/2+bsize)<=width?bsize:x-bsize/2+bsize-width-1;
    int s4=(y-bsize/2+bsize)<=height?bsize:y-bsize/2+bsize-height-1;
    Rect r=Rect(s1,s2,s3,s4);

    float f1=0,f2=0,f3=0;
    float f4=0,f5=0,f6=0;
    Mat rx=edge;
    uchar *ptr1=(uchar*)input.data;
    uchar *ptr=(uchar*)rx.data;

    for(int i1=0;i1<r.height;i1++)
    {

        for(int j1=0;j1<r.width;j1++)
        {
            //computing the mean values for the edge map
             f1=f1+ptr[(i1+r.y)*rx.step+rx.channels ()*(j1+r.x)];

           //computing the mean values of white color pixe;s
            f4=f4+ptr1[(i1+r.y)*input.step+input.channels()*(j1+r.x)];
            f5=f5+ptr1[(i1+r.y)*input.step+input.channels ()*(j1+r.x)+1];
            f6=f6+ptr1[(i1+r.y)*input.step+input.channels ()*(j1+r.x)+2];

        }
    }
    f1=f1/(r.width*r.height);

    f4=f4/(r.width*r.height);
    f5=f5/(r.width*r.height);
    f6=f6/(r.width*r.height);



    Scalar cc=Scalar(ceil(f4),ceil(f5),ceil(f6));
    float dist2=ColorDist3(cc,c);

    float mx=f1;//+f2+f3;

    //compring the distance threshold
    //color threshold check for overexposed areas
    //||((dist1+mx) < threshold && ( (dist1)<dist) ))
    if(((dist) < threshold ) && mx <threshold)
    {

            return 0;
    }
    else
    {
        //cerr << ": " << mx;
        return dist;

    }


}

/*
updating the mean value of region
  */
float ColorDist(Scalar a,Scalar b,int x,int y);

Scalar seed_fill::meanx(Scalar a,Scalar b,int size)
{
    Scalar c;
    c[0]=(float)(a[0]*size+b[0])/(float)(size+1);
   c[1]=(float)(a[1]*size+b[1])/(float)(size+1);
   c[2]=(float)(a[2]*size+b[2])/(float)(size+1);

   //cerr << "mean values " << c[0] <<":" << c[1] << ": " << c[2] << endl;

   return c;

}

/*
  method that gets the pixel value at location x,y
  */
Scalar seed_fill::GetPixel(int x,int y)
{
    //'//uchar * data=(uchar *)input.data;
    uchar *d1=(uchar *)(input.data);
    int index=3*x+(input.step*y);
    int v1=d1[index];
    int v2=d1[index+1];
    int v3=d1[index+2];
    Scalar xx=Scalar(v1,v2,v3,0);
    return xx;
}


/*
   method which checks if the pixel at location x,y has been processed
  */
bool seed_fill::isSet(int x,int y)
{
    uchar * d1=(uchar *)output.data;
    //uchar *d1=(uchar *)(data+(output.step*y));
    int index=3*x+output.step*y;
    int v1=d1[index];
    int v2=d1[index+1];
    int v3=d1[index+2];
    if(v1+v2+v3>0)
        return true;
    else
        return false;
}


/* setting the label for pixel at location x,y*/
void seed_fill::SetPixel(int x,int y)
{

    uchar *d1=(output.data);
    int index=3*x+output.step*y;
    d1[index]=0;
    d1[index+1]=255;
    d1[index+2]=255;
    area=area+1;
}

void seed_fill::Clear()
{
    flag=false;
    //input.copyTo (output);
    output.setTo (cv::Scalar::all (0));
    //output.setTo (cv::Scalar::all (0));
}

/*setting the label for pixel at location x,y with color b*/
void seed_fill::SetPixel(int x,int y,Scalar b)
{

    uchar * data=output.data;
    int index=3*x+(output.step*y);
    data[index]=b[0];
    data[index+1]=b[1];
    data[index+2]=b[2];
    area=area+1;
}



/* seed fill recurive function */
void seed_fill::SeedFill_1(int x, int y)
{

    //get the pixel at location x,y
    Scalar b=GetPixel(x,y);
    //check the distance of color at location with x,y
    float c= ColorDist(b,fill_color,x,y);
    //check the flag to see if pixel has been processed
    bool set=isSet(x,y);
    if( c< threshold && x >=0 && x <input.cols && y >=0 && y <input.rows && set==false) { // sample pixel color
        //4 connected neighbors
        SetPixel(x, y);
        SeedFill_1(x-1,y);
        SeedFill_1(x,y+1);
        SeedFill_1(x+1,y);
        SeedFill_1(x,y-1);

    }
}

bool seed_fill::check_dist(int x,int y)
{
    float dist=sqrt(0.5*(((seed_point.x-x)*(seed_point.x-x))+((seed_point.y-y)*(seed_point.y-y))));
    return dist;
}

bool seed_fill::check_color(int x,int y)
{
    Scalar b  = GetPixel(x,y);
    float c= ColorDist(b,fill_color,x,y);
    bool set=isSet(x,y);
    float dist=check_dist(x,y);
    int max_dim=max(input.rows,input.cols);
    bool flag=false;
    flag=( (c< threshold && x >=seed_point.x-max_dim/dist_scale && x <seed_point.x+max_dim/dist_scale && y >=seed_point.x-max_dim/dist_scale && y <seed_point.x+max_dim/dist_scale &&set==false ));
    return flag;
}

void seed_fill::SeedFill_3(int x, int y)
{

        //bool set=isSet(x,y);
        //if(set==false)
            LineFill_3(x,x,y,fill_color);
    }

    // Fill background with given color

void seed_fill_stack::SeedFill_4(int x, int y)
{

    flag=false;
    area=0;
    seed_point.x=x;
    seed_point.y=y;
    //cv::namedWindow ("iii");
    int left,right;
    int xa;
    int xb;
    if( x < nMinX || x > nMaxX || y < nMinX || y > nMaxY )
         return;

    //pushing initial seed pixel
    lines.push (line_element(x,x,y,-1));

    //performing the operation till stack is not empty
    while(lines.size ()>0)
    {

        //cerr << lines.size() <<":" << (nMaxX*nMaxY)/4 << endl;
        if(area>(2*nMaxX*nMaxY)/dist_scale)
        {
            Clear();
            return;
        }

        line_element p=lines.top ();
        lines.pop ();
        for( xa = p.left; xa >= nMinX && check_color(xa, p.y) == true ; xa-- );
        if(xa<p.left)
        xa=xa+1;


        for(xb=p.right ;xb < nMaxX && check_color(xb, p.y) == true ; xb++);
        if(xb>p.right)
            xb=xb-1;


        left=xa;
        do
        {
        //cerr << "1:" <<xa << ": " << xb << ":" << left << ":" << p.y << endl;
        for(xa=left;xa<xb && check_color(xa, p.y) == true ;xa++)
        {
                        SetPixel(xa, p.y, fill_color);
        }

        //cerr << "2:" <<xa << ": " << xb << ":" << left << endl;
        if(xa>left)
        {
            lines.push (line_element(left,xa-1,p.y-1,-p.nexty));
            lines.push (line_element(left,xa-1,p.y+1,-p.nexty));
        }

        for(;xa<xb && check_color(xa, p.y)  == false ;xa++);
        left=xa;

        //cerr << "3:" <<xa << ": " << xb << ":" << left << endl;
        }while(xa<xb);



    }
    if(area<100)
    flag=false;
    else
    flag=true;
}

    void seed_fill::LineFill_3(int x1, int x2, int y,
            Scalar fill_color)
    {
        int xL,xR;
        if( y < nMinY || nMaxY < y )
            return;
        for( xL = x1; xL >= nMinX; --xL ) { // scan left
            if( check_color(xL,y)==false)
                break;
            SetPixel(xL, y);
        }
        for( xR = x2;  xR <= nMaxX; ++xR ) { // scan right
            if( check_color(xR,y)==false)
                break;
            SetPixel(xR, y);
        }


        if( xL < x1 ) {
            LineFill_3(xL, xR, y-1, fill_color); // fill child
            LineFill_3(xL, xR, y+1, fill_color); // fill child
            ++x1;
        }

        if(  xR > x2 ) {
            LineFill_3(xL, xR, y-1, fill_color); // fill child
            LineFill_3(xL, xR, y+1, fill_color); // fill child
            --x2;
        }
        for( xR = x1; xR <= x2 && xR <= nMaxX; ++xR ) {  // scan betweens
            if( check_color(xR,y) ==true)
            SetPixel(xR, y);
            else {
                if( x1 < xR ) {
                    // fill child
                    LineFill_3(x1, xR-1, y-1, fill_color);
                    // fill child
                    LineFill_3(x1, xR-1, y+1, fill_color);
                    x1 = xR;
                }
                // Note: This function still works if this step is removed.
                for( ; xR <= x2 && xR <= nMaxX; ++xR) { // skip over border
                    if( check_color(xR,y)==true) {
                        x1 = xR--;
                        break;
                    }
                }
            }
        }



}

    /** main function to call for region growing */
    Mat seed_fill_stack::RegionGrowing(Mat a,int x,int y,Scalar color)
    {

        Mat edge1;
        cv::GaussianBlur (a,edge1,Size(3,3),2);
        cv::Laplacian (edge1,edge1,edge1.depth (),1,1);
        edge1.convertTo (edge1,CV_32FC(3),1,0);

        cv::cvtColor (a,a,CV_BGR2HSV_FULL);
        //setting up input and output matrices
        a.copyTo(input);
        a.copyTo(output);
        seed_point.x=x;
        seed_point.y=y;
       area=0;
        //cv::Canny(input,edge,150,255,3,true);

        nMinY=0;
        nMinX=0;
        nMaxY=input.rows-1;
        nMaxX=input.cols-1;
        int morph_size=1;
        Mat element = getStructuringElement( cv::MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
        //cv::morphologyEx (input,input,cv::MORPH_OPEN,element,Point(1,1),1);

        output.setTo(cv::Scalar::all(0));
        //getting seed pixel values
        fill_color=GetPixel(x,y);
        fill_color=color;
        int x1,y1,w1,h1;
        x1=max(x-1*bsize,0);
        y1=max(y-1*bsize,0);
        w1=x1+2*bsize>=a.cols?a.cols-x1:2*bsize;
        h1=y1+2*bsize>=a.rows?a.rows-y1:2*bsize;
        Rect r=Rect(x1,y1,w1,h1);
        Mat roi1=a(r);
        fill_color=cv::mean(roi1);
        //generating the edge map
        //
        //cv::split(input,im);
        //cv::morphologyEx (edge1,edge1,cv::MORPH_CLOSE,element,Point(1,1),1);
        vector <Mat> im,em;
        cv::split(edge1,em);
        cv::add (em[0],em[1],edge);
        cv::add (edge,em[2],edge);
        edge=edge;

        //threshold(dst,dst, maxVal*qualityLevel, 0, THRESH_TOZERO );

        double K=0.25;
        edge.convertTo (edge,CV_32FC(3),-1.0/(255.0*K),0);
        cv::exp (edge,edge);
        cv::threshold(edge,edge, std::exp(-1/K)+0.001, 0, THRESH_TOZERO );
        edge=1-edge;
        //imshow("edge map",edge);
        edge=255*edge;
        //cv::normalize(edge,edge,0,255,CV_MINMAX);

        double maxedge,minedge;
        cv::minMaxLoc (edge,&minedge,&maxedge,0,0);
        edge.convertTo (edge,CV_8UC(1),1,0);
        //cerr << "max edge is  " << maxedge << " min edge " << minedge <<endl;
        //cv::add (im[0],em[0],im[0]);
       // cv::add (im[1],em[1],im[1]);
       // cv::add (im[2],em[2],im[2]);

        SeedFill_4(x,y);
        //imshow("input -",input);
        cvtColor(output,output,CV_BGR2GRAY);
        input.setTo (Scalar(0,255,255),output);
        cv::cvtColor (input,input,CV_HSV2BGR_FULL);

        return input;
    }

