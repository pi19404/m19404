
#include "RegionGrowing.hpp"


seed_fill::seed_fill()
{
    threshold=80;
}

/** main function to call for region growing */
Mat seed_fill::RegionGrowing(Mat a,int x,int y)
{
    //setting up input and output matrices
    a.copyTo(input);
    a.copyTo(output);
    output.setTo(cv::Scalar::all(0));
    //getting seed pixel values
    fill_color=GetPixel(x,y);

    //generating the edge map
    cv::Canny(input,edge,150,255,3,true);

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

    Scalar c=Scalar(255,255,255);

    //computing color distance
    dist=ColorDist3(a,b);

    //computing distance from white pixel
    dist1=ColorDist3(a,c);



    //ROI for computing mean value of edge map
    Rect r=Rect(x-3>=0?x-3:0,y-3>=0?y-3:0,3,3);
    Mat rx=edge(r);
    Scalar m=cv::mean(rx);
    float mx=m[0];


    //color threshold check for overexposed areas
    if(dist1+mx<threshold/2 && dist1+mx<dist)
    {
        return dist1;
    }
    else
    {

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

}



/*setting the label for pixel at location x,y with color b*/
void seed_fill::SetPixel(int x,int y,Scalar b)
{

    uchar * data=output.data;
    int index=3*x+(output.step*y);
    data[index]=b[0];
    data[index+1]=b[1];
    data[index+2]=b[2];

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
