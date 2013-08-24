#include "Bug2.hpp"


void Bug2::run()
{

        changeState();
        Motion();
        Boundary();
        Follow();


    if(m.value!=m.Done)
    trajectory.push_back(position);

}


void Bug2::Follow()
{

    if(m.value==m.Nav)
    {
        float dx=cos(heading*PI/180);
        float dy=sin(heading*PI/180);


        if(scan.size()==0)
        {
            position.x-=prevdelta.x;
            position.y-=prevdelta.y;
            heading=best_index;

        }
        else
        {
            heading=best_index+90;
        }




        prevdelta=FPoint2f(dx,dy);
        position.x=position.x+3*dx;
        position.y=position.y+3*dy;

//check if point on MLine
        if(mline.isMLine(position)==true)
        {
//detected hit point
            if(hl.size()==0)
            {
            hl.push_back(position);
            }
//possible leave point
            else
            {
//distance from hit point to goal
                float d1=FPoint::dist(mline.hit,goal.position);
//distance from leave point to goal
                float d2=FPoint::dist(position,goal.position);
                if(d2>d1)
                    return;
//enter motition towards goal state
            m.value=m.Motion;
//initialize heading
            computeHeading();
//computer unit vectors along heading
            float dx=cos(heading*PI/180);
            float dy=sin(heading*PI/180);
//update the position
            position.x=position.x+3*dx;
            position.y=position.y+3*dy;
            prevdelta=FPoint2f(dx,dy);
//clear the stack
            hl.pop_back();
            }

        }


    }


}

