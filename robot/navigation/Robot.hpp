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

#ifndef ROBOT_HPP
#define ROBOT_HPP
#include "common.hpp"
#include "Point.hpp"
#include "Goal.hpp"
#include "MotionState.hpp"
#include "Obstacle.hpp"
class Robot
{
public:
    Goal goal;
    FPoint2f position;
    float heading;
    float distance;

    //current motion state
    MotionState m;

    //trajectory of points
    vector<FPoint2f> trajectory;

    //hit and leave point
    vector<FPoint2f> hit,leave;

    bool free;
    FPoint2f prevdelta;
    int radius;


    //draw robot information
    void draw(Mat &image);

    //constructors
    Robot();

    //function to compute heading towards goal
    void computeHeading();

    //draw heading information
    void drawHeading(Mat &image);

    void setPos(FPoint current)
    {
        position.x=current.x;
        position.y=current.y;
    }

    FPoint2f getPost()
    {
        return position;
    }

    vector<ScanPoint> scan;
    int best_index;
    Point getPosti()
    {
        return Point((int)position.x,(int)position.y);
    }

    void changeState()
    {
        heading=(int)heading%360;

        if(scan.size()>0 && m.value==MotionState::Motion)
        {
            for(int i=0;i<scan.size();i++)
            {
                if(scan[i].index==best_index && scan[i].position.x!=-1 && scan[i].position.y!=-1)
                {
                    m.value=MotionState::Boundary;
                }
            }

        }


        if(((position.x-goal.position.x)*(position.x-goal.position.x)+(position.y-goal.position.y)*(position.y-goal.position.y)-25)<0)
        {

            m.value=MotionState::Done;
        }

    }

    //function that perform moton towards the goal operation
    void Motion();

    //function for boundary detection
    void Boundary();

    //boundary navigation
    void Follow();

    //main function
    void run();


};

#endif // ROBOT_HPP
