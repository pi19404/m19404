#ifndef TANGENTBUG_HPP
#define TANGENTBUG_HPP
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
#include "common.hpp"
#include "Point.hpp"
#include "Robot.hpp"
#include "Goal.hpp"
//class to check the robot lies on the MLine


//class implementing Bug2 algorithm
class TBug2 :public Robot
{

public:
    class MLine
    {
    public:
        float slope;
        float intercept;
        float dx;
        float dy;
        float dr;
        float D;
        Point2f hit;
        Point2f goal;
        float radius;

    public:
        MLine(){};
        MLine(Point2f hit,Point2f goal,float r)
        {
            this->hit=hit;
            this->goal=goal;
            dx=(goal.x-hit.x);
            dy=(goal.y-hit.y);

            dr=sqrt(dx*dx+dy*dy);
            slope=(goal.y-hit.y)/(goal.x-hit.x);
            intercept=hit.y-(hit.x*slope);
            radius=r;
        }

        bool isMLine(Point2f position)
        {

            D=(hit.x-position.x)*(goal.y-position.y)-(hit.y-position.y)*(goal.x-position.x);

            float result=(radius*radius*dr*dr)-(D*D);
    //        cerr << result <<":" << endl;
            //float result=position.y-(position.x*slope+intercept);
            if(result>=0)
                return true;
            else
                return false;
        }


    };
public:
    vector <Point> hl;
    MLine mline;
    enum method{BUG2,TBUG};
    int _method;
    bool mflag;
    bool freeheading;
    float dist1;
    TBug2()
    {

    }
    ScanPoint mp;
    vector<vector<ScanPoint> > scan1;
    //bounday following for Bug2 algorithm
    void changeState();
    void Follow();
    void draw(Mat &image);
    void Boundary();
    float dreach;
    int countx;
    float dfollowed;
    //main function
    void run();

};
#endif // BUG2_HPP


