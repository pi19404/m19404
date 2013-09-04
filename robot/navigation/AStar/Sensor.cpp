#ifndef SENSOR_CPP
#define SENSOR_CPP
#include "Sensor.hpp"

Sensor::Sensor()
{
    radius=40;
}


void Sensor::draw(Mat &image,vector<ScanPoint>i,Robot r)
{
    Point p2=r.position;
    cv::circle(image,Point(p2.x,-/*w+*/p2.y),radius,Scalar(255,0,0),3,-1);
    for(int k=0;k<i.size();k++)
    {
        Point p=i[k].position;
        if(i[k].distance!=-1)
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

int Sensor::run(Robot robot)
{
    //cerr << endl << "*****************" <<endl;
    r=robot;


    float heading=((int)((robot.heading)));
    float sensorRange=radius;
    float intersectionPointWithSensorCircleX;
    float intersectionPointWithSensorCircleY;
    points.resize(360);
    heading=round(heading);
    final.resize(0);
    float delta=1;

    float best=-999999;
    float best1=999999;
    besti=0;
    int count=0;

    for(int i=0;i<points.size();i++)
    {
        points[i].index=-999999;

    }

    for( int i = -180; i < 180; i=i+delta)
    {


        for(int k2=5;k2<=sensorRange;k2=k2+10)
        //int k2=sensorRange;
        {

            if(k2==5)
                k2=sensorRange+3;

        intersectionPointWithSensorCircleX = cos(toRadians((heading+i)))*(k2);
        intersectionPointWithSensorCircleY = sin(toRadians((heading+i)))*(k2);


        //point on the circle
        intersectionPointWithSensorCircleX+=robot.position.x;
        intersectionPointWithSensorCircleY+=robot.position.y;

        Point2f ff=Point2f(intersectionPointWithSensorCircleX,-intersectionPointWithSensorCircleY);
        Point2f ff1=Point2f(intersectionPointWithSensorCircleX,intersectionPointWithSensorCircleY);

        if((points[i+180].index==-999999) && (count>0||k2==sensorRange+3) )
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
                 //best=-999999;
             }
             //check if point in outside the boundary obstacle
             else
             {
                 flag=d<=0;
                 //best=999999;
             }

             if(flag)
             {


                 bool flag2=true;
          /*       if(count>0)
                 for(int k1=0;k1<points.size();k1++)
                 {
                     if(points[k1].index==i )
                     {
                        flag2=false;
                        break;
                     }
                 }

                 if(flag2==true)*/
                 {
                  if(k2!=sensorRange+3)
                  points[i+180]=ScanPoint(FPoint2f(ff.x,-ff.y),(int)fabs(d),i);
                  //cerr << i <<":" << d << "--";
                  count++;



                 if(o1.type==0)
                 flag1=d>=best;
                 else
                 flag1=(-d)>best;

                 if(flag1)
                 {                      
                      best=abs(d);
                      besti=i;
                 }

                 }
             }

        }

        if(k2==sensorRange+3)
            k2=10;

    }
    }
    count=0;

    //cerr << endl << "*****************" <<endl;
    int j=0;
    float count1=0;
    for(int k=-180;k<180;k++)
    {
        ScanPoint p=points[k+180];
        if(p.index==-999999)
        {
            intersectionPointWithSensorCircleX = cos(toRadians((heading+k)))*(sensorRange);
            intersectionPointWithSensorCircleY = sin(toRadians((heading+k)))*(sensorRange);


            //point on the circle
            intersectionPointWithSensorCircleX+=robot.position.x;
            intersectionPointWithSensorCircleY+=robot.position.y;

            Point2f ff=Point2f(intersectionPointWithSensorCircleX,-intersectionPointWithSensorCircleY);
            Point2f ff1=Point2f(intersectionPointWithSensorCircleX,intersectionPointWithSensorCircleY);

            //points.push_back();
            points[k+180]=ScanPoint(FPoint2f(ff.x,-ff.y),-1,k);
        }

    }


    int total=0;
    bool flag=false;
    bool start=false;
    int k=0;
    vector<ScanPoint> tmp;
    scan1.resize(0);
    while(1)
    {
        ScanPoint p1=points[k];
        if(p1.distance==-1)
        {
            if(flag==true)
            {

                int in1=k-1;
                in1=in1%points.size();
                ScanPoint p3=tmp[0];
                bool term=true;
                int c1=0;
                while(c1<=10)
                {
                    int k1=(k+c1)%points.size();
                    ScanPoint p2=points[k1];
                    if(p2.distance!=-1)
                    {
                        points[k1].distance=p3.distance;
                        term=false;
                    }
                    c1++;
                }
                if(term==true)
                {
                p1=points[k-1];
                p1.distance=(radius)-p1.distance;
                tmp.push_back(p1);
                vector<ScanPoint> tmp1=tmp;
                scan1.push_back(tmp1);
                tmp.resize(0);
                flag=false;
                }
                if(p1.distance!=-1)
                    count++;

            }
            start=true;
            total=total+1;
        }

        if(points[k].distance!=-1 )
        {
            total=total+1;
            if(start==true &&flag==false)
            {
            flag=true;
            p1.distance=(radius)-p1.distance;
            tmp.push_back(p1);
            count1++;
            }
        }

        if(total==points.size())
            break;
         k++;
         k=k%(points.size());


    }



    vector<float> best_angles;
    for(int k=0;k<scan1.size();k++)
    {
        vector<ScanPoint> ss=scan1[k];
        int count1=0;
        float distance=-999999;
        int index=0;
        int counti=0;
        bool start=false;
        float min=0;
        float max=0;

        if(ss[0].index<=ss[1].index)
        {
            min=1;
            //max=ss[1].index;
        }
        else
        {
            min=-1;//ss[1].index;
            //max=ss[2].index;
        }

        while(counti<points.size())
        {
            if(points[count1].index>=ss[0].index && points[count1].index <= ss[1].index)
            {
                start=true;
                if(points[count1].distance >= distance && points[count1].distance!=-1)
                {
                    distance=points[count1].distance;
                    index=points[count1].index;
                }

            }
            if(start==true)
            counti++;

            count1=count1+min;
            if(count1<0)
                count1=count1+points.size();
            count1=count1%points.size();
        }
        if(distance!=999999)
        best_angles.push_back(index);
    }


    if(scan1.size()>0)
    {
    float dx=0;
    float dy=0;
    for(int i=0;i<best_angles.size();i++)
    {
        dx+=cos(best_angles[i]*PI/180);
        dy+=sin(best_angles[i]*PI/180);
    }

    //float besti1=best_angles[0];
    float besti1=0;
    if(fabs(dx)>0.01)
    besti1=atan2(dy,dx)*180/PI;
    else
    besti1=besti;
    //besti1=((round(besti1)))-180;
    if(besti1<=-180)
        besti1=besti1+360;
    if(besti1>180)
        besti1=besti1-360;

    besti1=(int)besti1%360;

    besti=besti1;
    }


    return count;
}


#endif // SENSOR_CPP
