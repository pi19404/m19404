#include "tangentbug.hpp"


void TBug2::run()
{

        changeState();
        if(m.value==m.Motion)
        {
            dfollowed=9999999;
            dreach=99999999;
            Motion();
        }
        else if(m.value==m.Boundary)
        Boundary();
        else if(m.value==m.Nav)
        Follow();


    if(m.value!=m.Done)
    trajectory.push_back(position);

}

void TBug2::draw(Mat &image)
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

    for(int i=0;i<scan1.size();i++)
    {
        vector<ScanPoint> s1=scan1[i];
        for(int j=0;j<s1.size();j++)
        {
            ScanPoint p1=s1[j];
            cv::circle(image,Point(p1.position.x,-p1.position.y),3,Scalar(0,255,0),3,-1);
        }
    }
    if(mp.index!=-1)
    cv::circle(image,Point(mp.position.x,-mp.position.y),10,Scalar(0,255,0),3,8);
}



void TBug2::Follow()
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
            int bi=best_index;
            if(mp.index>=0)
             bi=bi+90;
            else
            bi=bi-90;
            //int bi=best_index+90;
            bi=((int)bi)%360;
            if(bi<=-180)
                bi=bi+360;
            if(bi>180)
                bi=bi-360;

            int i=-1;
            for(i=0;i<scan.size();i++)
            {
                if(scan[i].index==bi && scan[i].distance!=-1)
                {
                    bi=bi+(mp.index>0?1:-1)*1;
                    bi=((int)bi)%360;
                    if(bi<-180)
                        bi=bi+360;
                    if(bi>180)
                        bi=bi-360;
                    i=0;
                    //break;
                }
            }

            for(i=0;i<scan.size();i++)
            {
                ScanPoint p=scan[i];
                if(p.distance!=-1)
                {
                    float angle=heading+p.index;
                    float mag=radius-p.distance;

                    float dx1=mag*cos(angle*PI/180);
                    float dy1=mag*sin(angle*PI/180);

                    dx1=position.x+dx1;
                    dy1=position.x+dy1;

                    float dd=sqrt(FPoint::dist(Point2f(dx1,-dy1),goal.position));
                    if(dd<dfollowed)
                        dfollowed=dd;
                }
            }

            ScanPoint s;
            float dreach=99999999;
            for(int i=0;i<scan.size();i++)
            {
                ScanPoint p=scan[i];
                //if(p.distance==-1)
                {

                    float angle=heading+p.index;
                    float mag=radius-p.distance;

                    float dx1=mag*cos(angle*PI/180);
                    float dy1=mag*sin(angle*PI/180);

                    dx1=position.x+dx1;
                    dy1=position.x+dy1;

                    float dreach2=sqrt(FPoint::dist(Point2f(dx1,-dy1),goal.position));

                    //float dreach2=sqrt(FPoint::dist(p.position,goal.position));
                    if(dreach2<dreach)
                    {
                        dreach=dreach2;
                        s=p;
                    }
                }
            }

            //cerr << ":::" << dreach <<":" << dfollowed << endl;
            if(dreach<dfollowed && dfollowed <99999999)
            {
                //heading=heading+s.index;
                dreach=99999999;
                dfollowed=99999999;

                m.value=m.Boundary;
                computeHeading();
                //heading=cur_heading+bi;
                dx=cos(heading*PI/180);
                dy=sin(heading*PI/180);
                prevdelta=ScanPoint(FPoint2f(dx,dy),-1,0);
                //position.x=position.x+(dx);
                //position.y=position.y+(dy);


            }
            else
            {
            heading=heading+bi;
            dx=cos(heading*PI/180);
            dy=sin(heading*PI/180);
            prevdelta=ScanPoint(FPoint2f(dx,dy),-1,best_index);
            position.x=position.x+(dx);
            position.y=position.y+(dy);

            }

        }






    }
}

void TBug2::Boundary()
{





         if(m.value==m.Boundary)
         {

             mp.index=-1;


             bool freeheading=false;
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

                 float distance1=FPoint::dist(hl[hl.size()-1],goal.position);
                 float distance2=FPoint::dist(position,goal.position);
                 if(distance2<distance1)
                 {
                 m.value=m.Motion;
                 computeHeading();
                 heading=((int)heading)%360;
                 if(heading<=-180)
                     heading=heading+360;
                 if(heading>180)
                     heading=heading-360;


                 float dx=cos(heading*PI/180);
                  float dy=sin(heading*PI/180);
                  prevdelta=ScanPoint(FPoint2f(dx,dy),-1,mp.index);
                  position.x=position.x+1*(dx);
                  position.y=position.y+1*(dy);
                  return;
                 }
                 else
                 {
                     freeheading=false;

                 }


             }

             if(freeheading==false)
             {
             ScanPoint prev_mp=mp;
             for(int i=0;i<scan1.size();i++)
             {

                 vector<ScanPoint> s1=scan1[i];
                 for(int j=0;j<s1.size();j++)
                 {
                     ScanPoint p1=s1[j];
                     float distance1=p1.distance+sqrt((FPoint::dist(p1.position,goal.position)));
                     if(distance1<dist1)
                     {
                       dist1=distance1;
                       mp=p1;
                     }


                 }


             }




            /*if(mp.index!=-1)// && (abs(mp.index)>=20 || abs(mp.index)<=160))
            {
                        heading=heading+((mp.index>0)?1:-1);
                        float dx=cos(heading*PI/180);
                         float dy=sin(heading*PI/180);
                         prevdelta=ScanPoint(FPoint2f(dx,dy),-1,mp.index);
                         position.x=position.x+1*(dx);
                         position.y=position.y+1*(dy);


            }

         if((mp.index==-1 && scan.size()!=0))// || (abs(mp.index)<=20 || abs(mp.index)>=160))*/
         {



             bool f1=true;
             if(mp.index==-1)
             {
             mp=prev_mp;
             f1=false;
             }


             int bi=best_index;
             if(mp.index>=0)
             bi=bi+90;
             if(mp.index<0)
             bi=bi-90;


             bi=((int)bi)%360;
             if(bi<=-180)
                 bi=bi+360;
             if(bi>180)
                 bi=bi-360;

             int i=-1;
             for(i=0;i<scan.size();i++)
             {
                 if(scan[i].index==bi && scan[i].distance!=-1)
                 {
                     bi=bi+(mp.index>=0?1:-1)*1;
                     bi=((int)bi)%360;
                     if(bi<-180)
                         bi=bi+360;
                     if(bi>180)
                         bi=bi-360;
                     i=0;

                 }
             }



                // ((abs(mp.index)<=20 || abs(mp.index)>=160) && mp.index!=-1)  ||
              if(countx>0)
              {

              heading=heading+bi;
              heading=((int)heading)%360;
              if(heading<=-180)
                  heading=heading+360;
              if(heading>180)
                  heading=heading-360;


              float dx=1*cos(heading*PI/180);
              float dy=1*sin(heading*PI/180);
             //update the position
              position.x=position.x+1*dx;
              position.y=position.y+1*dy;
              prevdelta=ScanPoint(FPoint2f(dx,dy),-1,best_index);


              if(f1==false)
              m.value=m.Nav;

              //cerr << "AAAAAAAAAAAAAAAAA" <<endl;
              return;
              }
              else
              {
                  float dx=1*cos(heading*PI/180);
                  float dy=1*sin(heading*PI/180);
                 //update the position
                  position.x=position.x+1*dx;
                  position.y=position.y+1*dy;
                  prevdelta=ScanPoint(FPoint2f(dx,dy),-1,best_index);
    //              if(!(mp.index==-1 && scan.size()!=0))
                  m.value=m.Nav;
                  return;

              }

         }


         }


         heading=((int)heading)%360;
         if(heading<=-180)
             heading=heading+360;
         if(heading>180)
             heading=heading-360;


         }

}

void TBug2::changeState()
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
                if(scan[i].index==best_index )
                {

                    //cerr << "DDDDDDDDDDDDDDD" << scan[i].distance << endl;
                    if(scan[i].distance!=-1)
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


                        dist1=10e10;
                        float dist11=FPoint::dist(position,goal.position);
                        if(hl.size()>0)
                        {
                        float dist21=FPoint::dist(hl[hl.size()-1],goal.position);
                        if(dist21<dist11)
                        {
                        hl.push_back(FPoint2f(position.x,position.y));
                        m.value=MotionState::Boundary;
                        }
                        }
                        else
                        {
                            hl.push_back(FPoint2f(position.x,position.y));
                            //

                        }
                        m.value=MotionState::Boundary;
                        }
                    }
                }
            }

        }


        if(((position.x-goal.position.x)*(position.x-goal.position.x)+(position.y-goal.position.y)*(position.y-goal.position.y)-25)<0)
        {

            m.value=MotionState::Done;
        }



}


