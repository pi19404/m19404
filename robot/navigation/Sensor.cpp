#ifndef SENSOR_CPP
#define SENSOR_CPP
#include "Sensor.hpp"

Sensor::Sensor()
{
    radius=5;
}


void Sensor::draw(Mat &image,vector<ScanPoint>i,Robot r)
{
    Point p2=r.position;
    cv::circle(image,Point(p2.x,-/*w+*/p2.y),2*radius,Scalar(255,0,0),3,-1);
    for(int k=0;k<i.size();k++)
    {
        Point p=i[k].position;
        cv::line(image,Point(p2.x,-p2.y),Point(p.x,-p.y),Scalar(255,255,255),1,8);
    }


}


//compute the distance between points
float Sensor::dist(FPoint p,FPoint q)
{
 float dx=p.x-q.x;
 dx=dx*dx;
 float dy=p.y-q.y;
 dy=dy*dy;
 return sqrt(dx+dy);

}

//compute the distance between points
float Sensor::dist(FPoint2f p,FPoint2f q)
{
 float dx=p.x-q.x;
 dx=dx*dx;
 float dy=p.y-q.y;
 dy=dy*dy;
 return sqrt(dx+dy);

}


bool Sensor::validate(float fx,float fy,FPoint p1,FPoint p2)
{
    bool flag=false;
    if(p1.x <= p2.x)
    {
        if(fx>=p1.x&& fx<=p2.x)
        {
            flag=true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(fx>=p2.x&& fx<=p1.x)
        {
            flag=true;
        }
        else
        {
            return false;
        }

    }


    if(p1.y <= p2.y)
    {
        if(fy>=p1.y&& fy<=p2.y)
        {
            flag=true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if(fy>=p2.y&& fy<=p1.y)
        {
            flag=true;
        }
        else
        {
            return false;
        }

    }


    return true;
}


float Sensor::toRadians(float angle)
{
    return (((int)angle)%360)*PI/180;
}

vector<ScanPoint> Sensor::run(Robot robot)
{
    r=robot;


    float heading=((int)((robot.heading)));
    float sensorRange=radius;
    float intersectionPointWithSensorCircleX;
    float intersectionPointWithSensorCircleY;
    points.resize(0);
    final.resize(0);
    float delta=1;

    float best=-999999;
    float best1=999999;

    for( int i = -180; i <= 179; i=i+delta)
    {

        intersectionPointWithSensorCircleX = cos(toRadians((heading+i)))*sensorRange;
        intersectionPointWithSensorCircleY = sin(toRadians((heading+i)))*sensorRange;


        //point on the circle
        intersectionPointWithSensorCircleX+=robot.position.x;
        intersectionPointWithSensorCircleY+=robot.position.y;

        Point2f ff=Point2f(intersectionPointWithSensorCircleX,-intersectionPointWithSensorCircleY);
        Point2f ff1=Point2f(intersectionPointWithSensorCircleX,intersectionPointWithSensorCircleY);

//cycle over all the obstacles
        for(int k=0;k<o.size();k++)
        {
            Obstacle o1=o[k];


             double d=pointPolygonTest( o1.contours[0], ff,true);
             bool flag,flag1;
             //check if point in inside the normal obstacle
             if(o1.type==0)
             {
                 flag=d>=0;
                 best=-999999;
             }
             //check if point in outside the boundary obstacle
             else
             {
                 flag=d<0;
                 best=999999;
             }

             if(flag)
             {


                 points.push_back(ScanPoint(FPoint2f(ff.x,-ff.y),heading+i));
                 if(o1.type==0)
                 flag1=d>=best;
                 else
                 flag1=d<best;

                 if(flag1)
                 {
                      best=d;
                      besti=heading+i;

                 }
             }
        }
    }


    //cerr << best << ":" <<besti << endl;
    return points;
}


#endif // SENSOR_CPP
