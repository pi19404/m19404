/* ========================================================================
 * Copyright [2013][prashant iyengar@pi19404@gmail.com] The Apache Software Foundation
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
#include "Robot.hpp"


void Robot::computeHeading()
{
    if(m.value==MotionState::Motion)
    {
    heading=atan2((position.y-goal.position.y),(position.x-goal.position.x))*180/3.1412;
    heading=((round(heading)))-180;
    heading=heading;
    }
    best_index=heading;
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
         if(scan.size()==0)
         {

             position.x-=prevdelta.x;
             position.y-=prevdelta.y;
             heading=best_index;
             //m.value=m.Motion;
         }
         else
         {
             //move direction perpendicular to best scan line
             heading=best_index+90;
         }




         prevdelta=FPoint2f(dx,dy);
         position.x=position.x+5*dx;
         position.y=position.y+5*dy;




     }


 }


void Robot::Boundary()
{
    if(m.value==m.Boundary)
    {
//            float angle=heading;
//            angle=heading+1;
//            angle=(int)angle%360;

        //change the heading direction by 90 deg
        //to best scan line
        for(int i=0;i<scan.size();i++)
        {
            if(scan[i].index==best_index)
            {
                if(scan[i].position.x!=-1 && scan[i].position.y!=-1)
                {
                heading=best_index+90;
                //change state to boundary navigation
                m.value=m.Nav;
                return;
                }
                else
                {
                    break;
                }
            }

        }
        //no obstacle detected continue existing path
        if(scan.size()==0)
        {

            float dx=cos(heading*PI/180);
            float dy=sin(heading*PI/180);
            prevdelta=FPoint2f(dx,dy);
            position.x=position.x+dx;
            position.y=position.y+dy;
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
   prevdelta=FPoint2f(dx,dy);
   }
}


void Robot::drawHeading(Mat &image)
{

    Point p1=getPosti();
    Point p2=goal.getPosti();
    float dx=cos(heading*PI/180);
    float dy=sin(heading*PI/180);


    cv::line(image,Point(p1.x,-/*w+*/p1.y),Point(p1.x+20*dx,-(p1.y+20*dy)),Scalar(0,255,255),3,8);
    //cv::line(image,Point(p1.x,-/*w+*/p1.y),Point(p2.x,-/*w+*/p2.y),Scalar(0,0,255),1,8);
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
    radius=5;
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



