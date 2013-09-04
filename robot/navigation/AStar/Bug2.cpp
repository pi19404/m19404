#include "Bug2.hpp"


void Bug2::run()
{

        changeState();
        if(m.value==m.Motion)Motion();
        else if(m.value==m.Boundary)
        Boundary();
        else if(m.value==m.Nav)
        Follow();


    if(m.value!=m.Done)
    trajectory.push_back(position);

}

void Bug2::draw(Mat &image)
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

    for(int i=0;i<hl.size();i++)
    {
        cv::circle(image,Point(hl[i].x,-hl[i].y),3,Scalar(0,255,0),3,-1);
    }
}



void Bug2::Follow()
{




         if(m.value==m.Nav)
         {
             //cv::waitKey(0);

             //check if point on MLine
               mflag=mline.isMLine(position);
                     if(mflag==true)
                     {
             //detected hit point
             //possible leave point
                         if(hl.size()>0)
                         {
                             float dx1=FPoint::dist(hl[0],position);

                             if(dx1>=900)
                             {


             //distance from hit point to goal
                                 float d1=0;
                                 if(hl.size()>0)
                                 d1=FPoint::dist(hl[0],goal.position);
                                 else
                                 d1=FPoint::dist(mline.hit,goal.position);
             //distance from leave point to goal
                             float d2=FPoint::dist(position,goal.position);
                             if(d2<d1)
                             {
             //enter motition towards goal state
                         m.value=m.Motion;
                         float prev_heading=heading;
             //initialize heading
                         computeHeading();
             //computer unit vectors along heading
                         float dx=cos(heading*PI/180);
                         float dy=sin(heading*PI/180);
             //update the position
                         position.x=position.x+1*dx;
                         position.y=position.y+1*dy;
                         prevdelta=ScanPoint(FPoint2f(1*dx,1*dy),0);
             //clear the stack
                         hl.pop_back();
                         //cerr << "AAAAAAAAAAAAAAAAA" << dx1 <<":" << d1 <<":" << d2<< endl;
//                         cv::waitKey(0);
//                         return;
                             }
                             }
                         }


                     }



             float dx=cos(heading*PI/180);
             float dy=sin(heading*PI/180);

             //if robot moves away from boundary,try to go to previous position
             //move along the edge
             if(scount==0)
             {
                 position.x-=prevdelta.position.x;
                 position.y-=prevdelta.position.y;
                 heading=heading+prevdelta.index;
                 dx=cos(heading*PI/180);
                 dy=sin(heading*PI/180);
                 prevdelta=ScanPoint(FPoint2f(0,0),-1,0);
                 position.x=position.x+(dx);
                 position.y=position.y+(dy);
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
                         bi=bi+1;
                         bi=((int)bi)%360;
                         if(bi<-180)
                             bi=bi+360;
                         if(bi>180)
                             bi=bi-360;
                         //i=0;
                         //break;
                     }
                 }

                 heading=heading+bi;
                 dx=cos(heading*PI/180);
                 dy=sin(heading*PI/180);
                 prevdelta=ScanPoint(FPoint2f(dx,dy),-1,best_index);
                 position.x=position.x+(dx);
                 position.y=position.y+(dy);

             }






             if(hl.size()>0 )
             {
             //float prev_heading=heading;
            //initialize heading
             //computeHeading();
             //float cur_heading=(int)heading;
             //heading=prev_heading;
             freeheading=false;
             ScanPoint pp;
             for(int k=0;k<scan.size();k++)
             {
                 ScanPoint p=scan[k];
                 float h1=p.index+heading;
                 if(h1<=-180)
                     h1=h1+360;
                 if(h1>180)
                     h1=h1-360;
                 h1=(int)h1%360;

                 if(h1==cur_heading && p.distance==-1)
                 {

                     freeheading=true;
                     pp=p;
                 }

             }

             if(freeheading==true)
             {
                 //cv::waitKey(0);
                 float distance1=FPoint::dist(hl[0],goal.position);
                 float distance2=FPoint::dist(goal.position,pp.position);
                 if(distance1>distance2 && heading<cur_heading-30)
                 {
                 m.value=m.Motion;
                 computeHeading();
                 float dx=cos(heading*PI/180);
                 float dy=sin(heading*PI/180);
                //update the position
                 position.x=position.x+1*dx;
                 position.y=position.y+1*dy;
                 prevdelta=ScanPoint(FPoint2f(dx,dy),-1,0);
                 //cerr << ":" << cur_heading <<":" << heading << endl;
                 hl.pop_back();
                 }

             }

               }








}


         heading=((int)heading)%360;
         if(heading<=-180)
             heading=heading+360;
         if(heading>180)
             heading=heading-360;



}

void Bug2::changeState()
{

        heading=(int)heading%360;

        if(heading<-180)
           heading=heading-360;
        if(heading>180)
            heading=heading-360;

        if(scan.size()>0 && m.value==MotionState::Motion)
        {
            for(int i=0;i<scan.size();i++)
            {
                if(scan[i].index==0 && scan[i].distance!=-1)
                {
  /*                  if(hl.size()>0)
                    {
                        float dd=FPoint::dist(prevdelta.position,position);
                        if(dd>radius+1)
                          m.value=MotionState::Boundary;
                        else
                         m.value=m.Motion;
                    }
                    else*/
                    {
                        m.value=MotionState::Boundary;
                        hl.push_back(FPoint2f(position.x,position.y));
                    }
                }
            }

        }


        if(((position.x-goal.position.x)*(position.x-goal.position.x)+(position.y-goal.position.y)*(position.y-goal.position.y)-25)<0)
        {

            m.value=MotionState::Done;
        }

        \

}

