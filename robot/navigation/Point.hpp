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
#ifndef POINT1_HPP
#define POINT1_HPP
#include "common.hpp"




class FPoint : public Point
{
public:
    FPoint(){};
    FPoint(float x,float y)
    {
        this->x=(int)x;
        this->y=(int)y;
    }

    static float dist(Point2f p,Point2f q)
    {
     float dx=p.x-q.x;
     dx=dx*dx;
     float dy=p.y-q.y;
     dy=dy*dy;
     return (dx+dy);

    }
};


class FPoint2f : public Point2f
{
public:
    FPoint2f(){};
    FPoint2f(float x,float y)
    {
        this->x=(float)x;
        this->y=(float)y;
    }

};


class ScanPoint{
public:
    FPoint2f position;
    int index;
    float distance;
    ScanPoint()
    {
        index=-1;
        distance=0;
    }

    ScanPoint(FPoint2f pos,float dist1,int i=0)
    {
        position.x=pos.x;
        position.y=pos.y;
        index=i;
        distance=dist1;
    }

    void draw(Mat &image)
    {
        Point pp=position;
        cv::circle(image,Point(pp.x,-/*w+*/pp.y),3,Scalar(255,0,0),1,-1);
    }

};


class Line
{
public:
    FPoint2f p1;
    FPoint2f p2;
    float slope;
    float intercept;
    Line(FPoint2f _p1,FPoint2f _p2)
    {
        p1=_p1;
        p2=_p2;

        if((int)p1.x==(int)p2.x)
        {
            slope=10000;
            intercept=10000;
        }
        else if((int)p1.y==(int)p2.y)
        {
            slope=0;
            intercept=p2.y;
        }
        else
        {
            slope=atan2((p2.y-p1.y),(p2.x-p1.x))*180/3.1412;

            slope=tan((slope*PI/180));
            intercept=p2.y-(slope*p2.x);

        }

        //cerr << p1.x <<":" << p2.x << ":---" << endl;
        //slope=slope;
        ///intercept=intercept;
    }

    bool validate(float fx,float fy,FPoint2f p1,FPoint2f p2)
    {
        bool flag=false;
        if((int)p1.x <= (int)p2.x)
        {
            if((int)fx>=(int)p1.x&& (int)fx<=(int)p2.x)
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
            if((int)fx>=(int)p2.x&& (int)fx<=(int)p1.x)
            {
                flag=true;
            }
            else
            {
                return false;
            }

        }


        if((int)p1.y <= (int)p2.y)
        {
            if((int)fy>=(int)p1.y&& (int)fy<=(int)p2.y)
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
            if((int)fy>=(int)p2.y&& (int)fy<=(int)p1.y)
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

    FPoint2f checkInterSection(Line l)
    {


        bool flag=false;
        float m1=l.slope;
        float i1=l.intercept;
        float xx=0;
        float yy=0;
        if(m1==slope && intercept!=i1)
        {
            return FPoint2f(-1,-1);
        }
        if(slope >=1000 && m1>=1000 && l.p1.x!=p1.x)
        {
            return FPoint2f(-1,-1);
        }
        else if(slope >=1000 && m1>=1000 && l.p1.x==p1.x)
        {
            xx=p1.x;
            yy=l.p2.y;
        }
        else if(slope>=1000)
        {
            xx=p1.x;
            yy=m1*p1.x+i1;
        }
        else if(m1>=1000)
        {
            xx=l.p1.x;
            yy=slope*p1.x+intercept;
        }
        else if(m1==0)
        {
            yy=(m1*intercept-slope*i1)/(m1-i1);
            xx=(-intercept+yy)/slope;

        }
        else if(slope==0)
        {
            yy=(m1*intercept-slope*i1)/(m1-i1);
            xx=(-i1+yy)/m1;
        }
        else
        {
            yy=(m1*intercept-slope*i1)/(m1-i1);
            xx=(-i1+yy)/m1;


        }


        if(validate(xx,yy,p1,p2)==false)
        {
            return FPoint2f(-1,-1);
        }


        if(validate(xx,yy,l.p1,l.p2)==false)
        {
            return FPoint2f(-1,-1);
        }





        flag=true;

        if(flag==true)
        {
            return FPoint2f(xx,yy);
        }
        else
        {
            return FPoint2f(-1,-1);
        }
    }

    FPoint2f checkInterSection1(Line l)
    {


        bool flag=false;
        float m1=l.slope;
        float i1=l.intercept;
        float xx=0;
        float yy=0;
        if(m1==slope && intercept!=i1)
        {
            return FPoint2f(-1,-1);
        }
        if(slope >=1000 && m1>=1000 && l.p1.x!=p1.x)
        {
            return FPoint2f(-1,-1);
        }
        else if(slope >=1000 && m1>=1000 && l.p1.x==p1.x)
        {
            xx=p1.x;
            yy=l.p2.y;
        }
        else if(slope>=1000)
        {
            xx=p1.x;
            yy=m1*p1.x+i1;
        }
        else if(m1>=1000)
        {
            xx=l.p1.x;
            yy=slope*p1.x+intercept;
        }
        else if(m1==0)
        {
            yy=(m1*intercept-slope*i1)/(m1-i1);
            xx=(-intercept+yy)/slope;

        }
        else if(slope==0)
        {
            yy=(m1*intercept-slope*i1)/(m1-i1);
            xx=(-i1+yy)/m1;
        }
        else
        {
            yy=(m1*intercept-slope*i1)/(m1-i1);
            xx=(-i1+yy)/m1;


        }
        //xx=0;
        //xx=xx+;
        //yy=yy+l.p1.y;;

        //if(l.p1.x>300 && l.p1.y<-300)
        {


        //    cv::waitKey(0);

        }

        if(validate(xx,yy,p1,p2)==false)
        {
            return FPoint2f(-1,-1);
        }


        if(validate(xx,yy,l.p1,l.p2)==false)
        {
            return FPoint2f(-1,-1);
        }



        flag=true;

        if(flag==true)
        {
            return FPoint2f(xx,yy);
        }
        else
        {
            return FPoint2f(-1,-1);
        }
    }

};

#endif // POINT1_HPP
