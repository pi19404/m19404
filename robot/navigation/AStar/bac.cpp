////#include <QApplication>

////#include "mainwindow.h"
//#include <iostream>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/core/core.hpp"
//#include "math.h"
//#include "stdio.h"
//using namespace cv;
//using namespace std;
//int w=600;
//int h=1200;
//#define PI 3.1412

//class FPoint : public Point
//{
//public:
//    FPoint(){};
//    FPoint(float x,float y)
//    {
//        this->x=(int)x;
//        this->y=(int)y;
//    }

//};


//class FPoint2f : public Point2f
//{
//public:
//    FPoint2f(){};
//    FPoint2f(float x,float y)
//    {
//        this->x=(int)x;
//        this->y=(int)y;
//    }

//};

//class Obstacle
//{


//public:
//    vector<FPoint> Points;
//    Obstacle()
//    {
//        /** Create some points */

////        Points.push_back(Point( 200,200 ));
////        Points.push_back(Point( 400,200));
////        Points.push_back(Point( 400,500));
////        Points.push_back(Point( 200,500));
////        Points.push_back(Point( 200,200));
//          Points.push_back(FPoint( 800,-300));
//          Points.push_back(FPoint( 100,-300));

//    }

//    void draw( Mat & img )
//    {
//      int lineType = 8;

//      vector<Point> p1;
//      p1.resize(0);
//      for(int i=0;i<Points.size();i++)
//      {
//          Point p=Points[i];
//          p1.push_back(Point(p.x,-p.y));
//      }

//      const Point* ppt[1] = {&p1[0]};
//      int npt[] = { p1.size()};

//      fillPoly( img,
//                ppt,
//                npt,
//                1,
//                Scalar( 0, 255, 0 ),
//                lineType );
//     }
//};


//class DebugText
//{
//public:
//    Point position;


//    DebugText()
//    {
//        position.x=810;
//        position.y=20;
//    }

//    void print(Mat &img,string text,bool flag=false)
//    {
//        if(flag==true)
//        {
//            position.y=20;
//        }
//        else
//        {
//            position.y=position.y+20;
//        }
//        int fontFace = cv::FONT_HERSHEY_PLAIN;
//        double fontScale = 1;
//        int thickness = 1;
//        putText(img, text, position, fontFace, fontScale,
//                Scalar(255,0,0), thickness, 8);

//    }


//};

//class Goal
//{
//public:
//    FPoint2f position;

//    Goal()
//    {
//        position.x=500;
//        position.y=-500;
//    }

//    void draw(Mat &image)
//    {
//        cv::circle(image,Point((int)position.x,(int)-/*w+*/position.y),3,Scalar(255,0,0),1,8);
//    }



//    void setPos(FPoint current)
//    {
//        position.x=current.x;
//        position.y=current.y;
//    }

//    FPoint2f getPost()
//    {
//        return position;
//    }

//    Point getPosti()
//    {
//        return Point((int)position.x,(int)position.y);
//    }
//};


//class MotionState
//{
//public:
//    enum {Motion,Boundary,Nav};
//    int value;
//};

///*
//class Simulation
//{
//public:
//    Size dimension;
//    vector<Obstacle> o;
//    Robot robot;
//    Goal goal;

//    void putObstacle(Obstacle oo)
//    {
//        o.push_back(oo);
//    }

//    void draw(Mat &image)
//    {
//        for(int i=0;i<s.o.size();i++)
//        {
//            o[i].draw(image);
//        }

//    }
//};
//*/


//class ScanPoint{
//public:
//    FPoint2f position;
//    int index;

//    ScanPoint()
//    {

//    }

//    ScanPoint(Point2f pos,int i=0)
//    {
//        position.x=pos.x;
//        position.y=pos.y;
//        index=i;
//    }

//    void draw(Mat &image)
//    {
//        Point pp=position;
//        cv::circle(image,Point(pp.x,-/*w+*/pp.y),3,Scalar(255,0,0),1,-1);
//    }

//};


//class Robot
//{
//public:
//    Goal goal;
//    FPoint2f position;
//    float heading;
//    float distance;
//    MotionState m;
//    vector<FPoint2f> trajectory;
//    ScanPoint best;
//    ScanPoint next;
//    ScanPoint first;
//    ScanPoint second;
//    bool free;
//    FPoint2f prevdelta;


//    FPoint2f hit,leave;
//    void draw(Mat &image)
//    {
//        Point pp=getPosti();
//        cv::circle(image,Point(pp.x,-/*w+*/pp.y),3,Scalar(0,0,255),1,8);
//        goal.draw(image);
//    }


//    Robot()
//    {
//        position.x=250;
//        position.y=-250;
//        m.value=MotionState::Motion;
//    }

//    void computeHeading()
//    {
//        heading=atan2((position.y-goal.position.y),(position.x-goal.position.x))*180/3.1412;

//        heading=((round(heading)))-180;
//        heading=heading;
//        distance=sqrt((goal.position.x-position.x)*(goal.position.x-position.x)+(goal.position.y-position.y)*(goal.position.y-position.y));
//    }

//    void drawHeading(Mat &image)
//    {

//        Point p1=getPosti();
//        Point p2=goal.getPosti();
//        cv::line(image,Point(p1.x,-/*w+*/p1.y),Point(p2.x,-/*w+*/p2.y),Scalar(0,0,255),1,8);
//        cv::line(image,Point(p1.x,-/*w+*/p1.y),Point(w,-/*w+*/p1.y),Scalar(0,0,255),1,8);
//        for(int i=0;i<trajectory.size();i++)
//        {
//            Point p=trajectory[i];
//            cv::circle(image,Point(p.x,-/*w+*/p.y),1,Scalar::all(255),1,8);
//        }

//    }

//    void setPos(FPoint current)
//    {
//        position.x=current.x;
//        position.y=current.y;
//    }

//    FPoint2f getPost()
//    {
//        return position;
//    }

//    Point getPosti()
//    {
//        return Point((int)position.x,(int)position.y);
//    }

//    void run()
//    {
//        computeHeading();

//        if(m.value==m.Motion)
//        {

//            float dx=cos(heading*PI/180);
//            float dy=sin(heading*PI/180);


//            position.x=position.x+dx;
//            position.y=position.y+dy;


//        }

//        else if(m.value==m.Boundary)
//        {

//            //cerr << (best.position.x-next.position.x) << ":" << (best.position.y -next.position.y) << endl;

//            hit=best.position;
//            if(best.position.x!=-1)
//            {

//                if(next.position.x!=-1)
//                {


//                    float dx=best.position.x-next.position.x;
//                    float dy=best.position.y-next.position.y;

//                    position.x=position.x+dx;
//                    position.y=position.y+dy;
//                }
//                else
//                {
//                    float dx=cos(heading*PI/180);
//                    float dy=sin(heading*PI/180);


//                    position.x=position.x+dx;
//                    position.y=position.y+dy;

//                }


//            }
//            else
//            {
//                m.value=MotionState::Nav;
//            }
//            m.value=MotionState::Nav;




//        }
//        else if(m.value=m.Nav)
//        {


//            if(first.position.x!=-1 && second.position.x!=-1 && !(first.position.x==second.position.x && first.position.y==second.position.y))
//            {

//                float dx=first.position.x-second.position.x;
//                float dy=first.position.y-second.position.y;
//                position.x=position.x+dx;
//                position.y=position.y+dy;

//                prevdelta=FPoint2f(dx,dy);
//            }
//            else
//            {
//                if(prevdelta.x<0)
//                {
//                    position.x+=0.8;
//                    position.y-=0.8;

//                }
//                if(prevdelta.x>0)
//                {
//                    position.x+=0.8;
//                    position.y+=0.8;

//                }
//            }

///*            if(free==true)
//            {
//                m.value=m.Boundary;
//                leave=first.position;
//            }
//*/
////            robotsDistanceToGoal = simulation.sense.robotsDistanceToGoal();

//  //          if( robotsDistanceToGoal < hitPoint.distanceToGoal && simulation.scanner.isMLineFree() )
//    //        {
////
//  //              mode = boundaryFollowing;
//    //            leavePoint = firstPoint;
//      //      }


//        }

//        //cerr << "SS" << m.value << ":" << first.position.x <<":" << second.position.x << ":" << first.index <<":" << second.index << endl;

//                    trajectory.push_back(position);

//    }


//};


//class Line
//{
//public:
//    FPoint2f p1;
//    FPoint2f p2;
//    float slope;
//    float intercept;
//    Line(FPoint2f _p1,FPoint2f _p2)
//    {
//        p1=_p1;
//        p2=_p2;

//        if((int)p1.x==(int)p2.x)
//        {
//            slope=10000;
//            intercept=10000;
//        }
//        else if((int)p1.y==(int)p2.y)
//        {
//            slope=0;
//            intercept=p2.y;
//        }
//        else
//        {
//            slope=atan2((p2.y-p1.y),(p2.x-p1.x))*180/3.1412;

//            //slope=90-slope;

//            //slope=((round(slope)))-180;
//            slope=tan((slope*PI/180));

//            //slope=-slope;
//            //slope=(p2.y-p1.y)/(p2.x-p1.x);

//            intercept=p2.y-(slope*p2.x);

//        }

//        //cerr << p1.x <<":" << p2.x << ":---" << endl;
//        //slope=slope;
//        ///intercept=intercept;
//    }

//    bool validate(float fx,float fy,FPoint2f p1,FPoint2f p2)
//    {
//        bool flag=false;
//        if((int)p1.x <= (int)p2.x)
//        {
//            if((int)fx>=(int)p1.x&& (int)fx<=(int)p2.x)
//            {
//                flag=true;
//            }
//            else
//            {
//                return false;
//            }
//        }
//        else
//        {
//            if((int)fx>=(int)p2.x&& (int)fx<=(int)p1.x)
//            {
//                flag=true;
//            }
//            else
//            {
//                return false;
//            }

//        }


//        if((int)p1.y <= (int)p2.y)
//        {
//            if((int)fy>=(int)p1.y&& (int)fy<=(int)p2.y)
//            {
//                flag=true;
//            }
//            else
//            {
//                return false;
//            }
//        }
//        else
//        {
//            if((int)fy>=(int)p2.y&& (int)fy<=(int)p1.y)
//            {
//                flag=true;
//            }
//            else
//            {
//                return false;
//            }

//        }


//        return true;
//    }

//    FPoint2f checkInterSection(Line l)
//    {


//        bool flag=false;
//        float m1=l.slope;
//        float i1=l.intercept;
//        float xx=0;
//        float yy=0;
//        if(m1==slope && intercept!=i1)
//        {
//            return FPoint2f(-1,-1);
//        }
//        if(slope >=1000 && m1>=1000 && l.p1.x!=p1.x)
//        {
//            return FPoint2f(-1,-1);
//        }
//        else if(slope >=1000 && m1>=1000 && l.p1.x==p1.x)
//        {
//            xx=p1.x;
//            yy=l.p2.y;
//        }
//        else if(slope>=1000)
//        {
//            xx=p1.x;
//            yy=m1*p1.x+i1;
//        }
//        else if(m1>=1000)
//        {
//            xx=l.p1.x;
//            yy=slope*p1.x+intercept;
//        }
//        else if(m1==0)
//        {
//            yy=(m1*intercept-slope*i1)/(m1-i1);
//            xx=(-intercept+yy)/slope;

//        }
//        else if(slope==0)
//        {
//            yy=(m1*intercept-slope*i1)/(m1-i1);
//            xx=(-i1+yy)/m1;
//        }
//        else
//        {
//            yy=(m1*intercept-slope*i1)/(m1-i1);
//            xx=(-i1+yy)/m1;


//        }


//        if(validate(xx,yy,p1,p2)==false)
//        {
//            return FPoint2f(-1,-1);
//        }


//        if(validate(xx,yy,l.p1,l.p2)==false)
//        {
//            return FPoint2f(-1,-1);
//        }





//        flag=true;

//        if(flag==true)
//        {
//            return FPoint2f(xx,yy);
//        }
//        else
//        {
//            return FPoint2f(-1,-1);
//        }
//    }




//    FPoint2f checkInterSection1(Line l)
//    {


//        bool flag=false;
//        float m1=l.slope;
//        float i1=l.intercept;
//        float xx=0;
//        float yy=0;
//        if(m1==slope && intercept!=i1)
//        {
//            return FPoint2f(-1,-1);
//        }
//        if(slope >=1000 && m1>=1000 && l.p1.x!=p1.x)
//        {
//            return FPoint2f(-1,-1);
//        }
//        else if(slope >=1000 && m1>=1000 && l.p1.x==p1.x)
//        {
//            xx=p1.x;
//            yy=l.p2.y;
//        }
//        else if(slope>=1000)
//        {
//            xx=p1.x;
//            yy=m1*p1.x+i1;
//        }
//        else if(m1>=1000)
//        {
//            xx=l.p1.x;
//            yy=slope*p1.x+intercept;
//        }
//        else if(m1==0)
//        {
//            yy=(m1*intercept-slope*i1)/(m1-i1);
//            xx=(-intercept+yy)/slope;

//        }
//        else if(slope==0)
//        {
//            yy=(m1*intercept-slope*i1)/(m1-i1);
//            xx=(-i1+yy)/m1;
//        }
//        else
//        {
//            yy=(m1*intercept-slope*i1)/(m1-i1);
//            xx=(-i1+yy)/m1;


//        }
//        //xx=0;
//        //xx=xx+;
//        //yy=yy+l.p1.y;;

//        //if(l.p1.x>300 && l.p1.y<-300)
//        {


//        //    cv::waitKey(0);

//        }

//        if(validate(xx,yy,p1,p2)==false)
//        {
//            return FPoint2f(-1,-1);
//        }


//        if(validate(xx,yy,l.p1,l.p2)==false)
//        {
//            return FPoint2f(-1,-1);
//        }



//        flag=true;

//        if(flag==true)
//        {
//            return FPoint2f(xx,yy);
//        }
//        else
//        {
//            return FPoint2f(-1,-1);
//        }
//    }

//};

//class Sensor
//{
//public:
//    int radius;
//    vector<Obstacle> o;
//    vector<ScanPoint> points;
//    vector<ScanPoint> final;

//    Sensor()
//    {
//        radius=20;
//    }

//    void draw(Mat &image,vector<ScanPoint>i,Robot r)
//    {
//        Point p=r.position;
//        cv::circle(image,Point(p.x,-/*w+*/p.y),radius,Scalar(0,0,255),1,-1);
//        for(int k=0;k<i.size();k++)
//        {
//            p=i[k].position;
//            cv::circle(image,Point(p.x,-/*w+*/p.y),3,Scalar(0,255,255),1,-1);
//        }
//    }

//    float dist(FPoint p,FPoint q)
//    {
//     float dx=p.x-q.x;
//     dx=dx*dx;
//     float dy=p.y-q.y;
//     dy=dy*dy;
//     return sqrt(dx+dy);

//    }

//    float dist(FPoint2f p,FPoint2f q)
//    {
//     float dx=p.x-q.x;
//     dx=dx*dx;
//     float dy=p.y-q.y;
//     dy=dy*dy;
//     return sqrt(dx+dy);

//    }

//    bool validate(float fx,float fy,FPoint p1,FPoint p2)
//    {
//        bool flag=false;
//        if(p1.x <= p2.x)
//        {
//            if(fx>=p1.x&& fx<=p2.x)
//            {
//                flag=true;
//            }
//            else
//            {
//                return false;
//            }
//        }
//        else
//        {
//            if(fx>=p2.x&& fx<=p1.x)
//            {
//                flag=true;
//            }
//            else
//            {
//                return false;
//            }

//        }


//        if(p1.y <= p2.y)
//        {
//            if(fy>=p1.y&& fy<=p2.y)
//            {
//                flag=true;
//            }
//            else
//            {
//                return false;
//            }
//        }
//        else
//        {
//            if(fy>=p2.y&& fy<=p1.y)
//            {
//                flag=true;
//            }
//            else
//            {
//                return false;
//            }

//        }


//        return true;
//    }

//    bool isMLineFree(Robot robot)
//    {

//        FPoint2f p3=FPoint2f(robot.position.x,robot.position.y);
//        FPoint2f p4=FPoint2f(robot.goal.position.x,robot.goal.position.y);

//        Line l(p3,p4);


//        for(int i=0;i<o.size();i++)
//        {
//            Obstacle o1=o[i];

//            for(int j=0;j<o1.Points.size()-1;j++)
//            {
//                FPoint2f p1=FPoint2f(o1.Points[j].x,o1.Points[j].y);
//                FPoint2f p2=FPoint2f(o1.Points[j+1].x,o1.Points[j+1].y);

//                Line l2(p1,p2);

//                FPoint2f inter=l2.checkInterSection1(l);
//                if(inter.x!=-1  && inter.y!=-1)
//                {
//                    return false;
//                }
//            }
//        }

//        return true;
//    }

//    float toRadians(float angle)
//    {
//        return (((int)angle)%360)*PI/180;
//    }

//    Robot r;
//    ScanPoint chooseBestPoint()
//    {
//        if(final.size()<1)
//            return ScanPoint(Point(-1,-1));

//        ScanPoint min=final[0];
//        float d1=dist(min.position,r.position);
//        for(int i=1;i<final.size();i++)
//        {
//            float d2=dist(final[i].position,r.position);
//            if(d2<=d1)
//            {
//                min=final[i];
//            }
//        }

//        return min;
//    }

//    ScanPoint chooseNext(ScanPoint best)
//    {
//        int best1=best.index;

//        for(int i=0;i<points.size();i++)
//        {
//            ScanPoint s=points[i];

//            if(s.index==best1-1)
//            {
//                //cerr << s.position.y <<":" << best.position.y << endl;
//                if(s.position.x!=best.position.x && s.position.y!=best.position.y)
//                    return s;
//            }
//            else if(s.index==best1+1)
//            {

//                if(s.position.x!=best.position.x && s.position.y!=best.position.y)
//                    return s;
//            }


//        }

//        return ScanPoint(Point(-1,-1));
//    }

//    ScanPoint getFirst()
//    {
//        if(final.size()>0)
//            return final[0];
//        else
//            return ScanPoint(Point(-1,-1));
//    }

//    ScanPoint getSecond()
//    {
//        if(final.size()>1)
//            return final[1];
//        else
//            return ScanPoint(Point(-1,-1));
//    }



//    vector<ScanPoint> run(Robot robot)
//    {
//        r=robot;


//        float heading=((int)((robot.heading)));
//        float sensorRange=radius;
//        float intersectionPointWithSensorCircleX;
//        float intersectionPointWithSensorCircleY;
//        points.resize(0);
//        final.resize(0);
//        float delta=1;
//        for( int i = -0; i <= 0; i=i+delta)
//        {

//            intersectionPointWithSensorCircleX = cos(toRadians((heading+i)))*sensorRange;
//            intersectionPointWithSensorCircleY = sin(toRadians((heading+i)))*sensorRange;


//            intersectionPointWithSensorCircleX+=robot.position.x;
//            intersectionPointWithSensorCircleY+=robot.position.y;


//            Line l(FPoint2f(robot.position.x,robot.position.y),FPoint2f(intersectionPointWithSensorCircleX,intersectionPointWithSensorCircleY));

//            for(int k=0;k<o.size();k++)
//            {
//                Obstacle o1=o[k];

//                for(int j=0;j<o1.Points.size()-1;j++)
//                {
//                    FPoint2f p1=FPoint2f(o1.Points[j].x,o1.Points[j].y);
//                    FPoint2f p2=FPoint2f(o1.Points[j+1].x,o1.Points[j+1].y);

//                    Line l1(p1,p2);
//                    FPoint2f inter=l1.checkInterSection(l);

//                    if(inter.x==-1 || inter.y==-1)
//                        continue;

//                    points.push_back(ScanPoint(inter,i));

//                }
//            }

//        }



//        if(points.size()<1)
//            return final;

//        final.push_back(points[0]);
//        int i=0;
//        for(i=0;i<points.size()-1;i++)
//        {
//            if(abs(points[i].index-points[i+1].index)>3)
//            {
//                if(i!=0)
//                final.push_back(points[i]);
//                if((i+1)!=points.size()-1)
//                final.push_back(points[i+1]);
//            }

//        }

//        final.push_back(points[i]);
//        //if(points.size()>2)
//        //{
//          //cerr << i<<":" << points.size() << "::" <<points[1].index <<":" << points[0].index <<"::::::::" << points[2].index << endl;
//        //cerr << i<<":" << final.size() << ":" << points[1].index <<"::::::::" << points[1].position.x << endl;
//        //}

//        return final;
//    }



//    vector<ScanPoint> run1(Robot robot)
//    {


//        Point p3=robot.position;
//        Point p4=robot.goal.position;

//        float m2=(p4.y-p3.y)/(p4.x-p3.x);
//        float i2=p4.y-(m2*p4.x);

//        vector<ScanPoint> intersection;
//        intersection.resize(0);

//        return intersection;
//        /*
//        for(int i=0;i<o.size();i++)
//        {
//            Obstacle o1=o[i];

//            for(int j=0;j<o1.Points.size()-1;j++)
//            {
//                Point p1=o1.Points[j];
//                Point p2=o1.Points[j+1];


//                float dx=(p2.x-p1.x);
//                float dy=(p2.y-p1.y);
//                float dr=sqrt((dx*dx)+(dy*dy));
//                dx=dx/dr;
//                dy=dy/dr;

//                float d1=dist(p1,p3);
//                float d2=dist(p2,p3);
//                float t=0;
//                Point A;
//                if(d1<d2)
//                {
//                    A=p1;
//                }
//                else
//                {
//                    A=p2;
//                }

//                t=(dx*(p3.x-A.x))+ dy*((p3.y-A.y));

//                float Ex=(t*dx)+(A.x);
//                float Ey=(t*dy)+(A.y);
//                float L = dist(Point(Ex,Ey),p3);
//                float R=radius;
//                //cerr << ":" << L << endl;
//                if(L< R)
//                {
//                    float dt = sqrt(R*R-L*L);

//                    float Fx = (t-dt)*dx + A.x;
//                    float Fy = (t-dt)*dy + A.y;

//                    float Gx = (t+dt)*dx + A.x;
//                    float Gy = (t+dt)*dy + A.y;


//                    if(abs(L-R)<0.1)
//                    {
//                        if(validate(Fx,Fy,p1,p2)==true)
//                        intersection.push_back(Point(Fx,Fy));
//                    }
//                    else
//                    {
//                        float dist1=dist(Point(Fx,Fy),Point(Gx,Gy));
//                        if(dist1<10)
//                        {
//                        if(validate(Fx,Fy,p1,p2)==true)
//                        intersection.push_back(Point(Fx,Fy));
//                        }
//                        else
//                        {
//                            if(validate(Fx,Fy,p1,p2)==true)
//                            intersection.push_back(Point(Fx,Fy));

//                            if(validate(Gx,Gy,p1,p2)==true)
//                            intersection.push_back(Point(Gx,Gy));

//                        }

//                    }

//                }






//                /*
//                float D=(p1.x*p2.y)-(p1.y*p2.x);

//                cerr << dx <<":" << dy << ":" << dr <<":" << D <<endl;
//                float del=((radius*radius)*(dr*dr))-(D*D);
//                cerr << "DEL" << del <<endl;
//                if(del>=0)
//                {
//                    float xp=0,yp=0;
//                    float del1=sqrt(del);
//                    float sy=dy<0?-1:1;

//                    float x1=(D*dy-sy*dx*del1)/(dr*dr);
//                    float y1=(-D*dx+abs(dy)*del1)/(dr*dr);
//                    float x2=(D*dy+sy*dx*del1)/(dr*dr);
//                    float y2=(-D*dx-abs(dy)*del1)/(dr*dr);

//                    float ver1=abs(y1*y1+x1*x1-(radius*radius));
//                    float ver2=abs(y1*y1+x1*x1-(radius*radius));


//                    if(ver1<0.01 && del>0.01)
//                    {
//                        intersection.push_back(Point(x1,y1));
//                    }

//                    if(ver1<0.02 && del>0.01)
//                    {
//                        intersection.push_back(Point(x2,y2));
//                    }

//                    if(del<0.01 && ver1<0.01)
//                    {
//                        intersection.push_back(Point(x1,y1));
//                    }

//                }

//            }
//        }

//        if(intersection.size()<=1)
//           return intersection;

//        for(int i=0;i<intersection.size()-1;i++)
//        {
//            float dist1=dist(intersection[i],intersection[i+1]);
//            if(dist1<10)
//                intersection.resize(1);
//        }
//        return intersection;
//*/
//    }


//};







//int main(int argc, char *argv[])
//{


//    /// Windows names
//    char rook_window[] = "Drawing 2: Rook";

//    Mat image1 = Mat::zeros( w, h, CV_8UC3 );

//    Obstacle a1;
//    Robot r1;
//    Goal r2;
//    DebugText d;
//    Sensor s;
//    r1.goal=r2;
//    Mat image;

//    s.o.push_back(a1);

//    while(1)
//    {
//    image1.copyTo(image);
//    a1.draw(image);
//    r1.draw(image);
//    r1.drawHeading(image);
//    r1.run();

//    vector<ScanPoint> i=s.run(r1);
//    r1.free=s.isMLineFree(r1);

//    r1.best=s.chooseBestPoint();
//    r1.next=s.chooseNext(r1.best);


//    r1.first=s.getFirst();
//    r1.second=s.getSecond();


//    s.draw(image,i,r1);


//    if(i.size()>0 && r1.free==false)
//    {
//        r1.m.value=MotionState::Boundary;
//    }

//    if(r1.first.position.x!=-1)
//    r1.first.draw(image);
//    if(r1.second.position.x!=-1)
//    r1.second.draw(image);



//    char text[1000];
//    sprintf(text,"Position :%f,%f",r1.position.x,-/*w+*/r1.position.y);
//    d.print(image,text,true);
//    sprintf(text,"Heading Angle :%f",r1.heading);
//    d.print(image,text,false);
//    sprintf(text,"Distance :%f",r1.distance);
//    d.print(image,text,false);
//    sprintf(text,"Intersection :%d",i.size());
//    d.print(image,text,false);
//    sprintf(text,"State :%d",r1.m.value);
//    d.print(image,text,false);


//    imshow("asdsad",image);
//    int k=cv::waitKey(30);
//    if(k=='e')
//        break;
//    }
//    //Q_INIT_RESOURCE(basicdrawing);

////    QApplication app(argc, argv);
////    Window window;
////    window.show();
////    return app.exec();



//}
