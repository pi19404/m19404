/* ========================================================================
 * Copyright [2013][prashant iyengar@pi19404@gmail.com] The Apache Software Foundation
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at`
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
#include "Robot.hpp"


void Robot::computeHeading()
{

    float tmp_heading=atan2((position.y-goal.position.y),(position.x-goal.position.x))*180/3.1412;
    tmp_heading=((round(tmp_heading)))-180;
    tmp_heading=tmp_heading;
    cur_heading=tmp_heading;
    if(cur_heading<=-180)
        cur_heading=cur_heading+360;
    if(cur_heading>180)
        cur_heading=cur_heading-360;

    cur_heading=(int)cur_heading%360;

    if(m.value==MotionState::Motion)
    {
        heading=cur_heading;
        best_index=0;//heading;
    }

    distance=sqrt((goal.position.x-position.x)*(goal.position.x-position.x)+(goal.position.y-position.y)*(goal.position.y-position.y));
}


void Robot::run()
{


        changeState();
        Motion();
        Boundary();
        Follow();


    if(m.value!=m.Done)
    trajectory.push_back(position);

}

void Robot::Follow()
 {

     if(m.value==m.Nav)
     {
         float dx=cos(heading*PI/180);
         float dy=sin(heading*PI/180);

         //if robot moves away from boundary,try to go to previous position
         //move along the edge
         if(scount==0)
         {

             position.x-=prevdelta.position.x;
             position.y-=prevdelta.position.y;
             heading=heading+prevdelta.index;

         }
         else
         {
             //move direction perpendicular to best scan line
             //heading=heading+best_index+90;
             int bi=best_index+90;
             bi=((int)bi)%360;
             if(bi<=-180)
                 bi=bi+360;
             if(bi>180)
                 bi=bi-360;

             for(int i=0;i<scan.size();i++)
             {
                 if(scan[i].index==bi && scan[i].distance!=-1)
                 {
                     bi=best_index-90;
                     bi=((int)bi)%360;
                     if(bi<=-180)
                         bi=bi+360;
                     if(bi>180)
                         bi=bi-360;

                     break;
                 }
             }
             heading=heading+bi;
             prevdelta=ScanPoint(FPoint2f(dx,dy),-1,best_index);
             position.x=position.x+(dx);
             position.y=position.y+(dy);

         }








     }

     if(heading<=-180)
         heading=heading+360;
     heading=((int)heading)%360;

 }


void Robot::Boundary()
{
    if(m.value==m.Boundary)
    {

        //change the heading direction by 90 deg
        //to best scan line
        for(int i=0;i<scan.size();i++)
        {
            if(scan[i].index==best_index)
            {
                if(scan[i].distance!=-1)
                {
                //heading=heading+best_index+90;
                //change state to boundary navigation
                //prevdelta=ScanPoint(FPoint2f(0,0),-1,90);
                m.value=m.Nav;

                int bi=best_index+90;
                bi=((int)bi)%360;
                if(bi<=-180)
                    bi=bi+360;
                if(bi>180)
                    bi=bi-360;

                for(int i=0;i<scan.size();i++)
                {
                    if(scan[i].index==bi && scan[i].distance!=-1)
                    {
                        bi=best_index-90;
                        bi=((int)bi)%360;
                        if(bi<=-180)
                            bi=bi+360;
                        if(bi>180)
                            bi=bi-360;

                        break;
                    }
                }
                heading=heading+bi;
                float dx=cos(heading*PI/180);
                float dy=sin(heading*PI/180);
                prevdelta=ScanPoint(FPoint2f(1*dx,1*dy),-1,best_index);
                position.x=position.x+(1*dx);
                position.y=position.y+(1*dy);
                break;
                }
                else
                {

                    break;
                }
            }

        }
        //no obstacle detected continue existing path
        if(scount==0)
        {
            float dx=cos(heading*PI/180);
            float dy=sin(heading*PI/180);            
            position.x=position.x+dx;
            position.y=position.y+dy;
            prevdelta=ScanPoint(FPoint2f(dx,dy),-1,0);
        }


    }

}



void Robot::Motion()
{
   if(m.value==m.Motion)
   {

//compute components of unit vector along the heading direction
   float dx=cos(heading*PI/180);
   float dy=sin(heading*PI/180);
   //update the current position
   position.x=position.x+1*dx;
   position.y=position.y+1*dy;
   //store the change in postion
   prevdelta=ScanPoint(FPoint2f(dx,dy),-1,0);
   }
}


void Robot::drawHeading(Mat &image)
{

    Point p1=getPosti();
    Point p2=goal.getPosti();

    float dx=cos(heading*PI/180);
    float dy=sin(heading*PI/180);


    cv::line(image,Point(p1.x,-/*w+*/p1.y),Point(p1.x+20*dx,-(p1.y+20*dy)),Scalar(0,255,255),3,8);
    cv::line(image,Point(start.x,-/*w+*/start.y),Point(p2.x,-/*w+*/p2.y),Scalar(0,0,255),1,8);
    cv::line(image,Point(p1.x,-/*w+*/p1.y),Point(p2.x,-/*w+*/p2.y),Scalar(0,0,255),1,8);

    //cv::line(image,Point(p1.x,-/*w+*/p1.y),Point(w,-/*w+*/p1.y),Scalar(0,0,255),1,8);

    for(int i=0;i<trajectory.size();i++)
    {
        Point p=trajectory[i];
        cv::circle(image,Point(p.x,-/*w+*/p.y),1,Scalar::all(255),1,8);
    }

}


Robot::Robot()
{
    position.x=250;
    position.y=-250;
    m.value=MotionState::Motion;
    computeHeading();
    radius=20;
}

void Robot::draw(Mat &image)
{
    Point pp=getPosti();
    if(m.value==m.Done)
    {
        cv::circle(image,Point(pp.x,-pp.y),radius,Scalar(0,0,255),3,-1);
    }
    else
    {
    cv::circle(image,Point(pp.x,-pp.y),3,Scalar(0,0,255),1,8);
    }
    goal.draw(image);
}



