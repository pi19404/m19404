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
#ifndef GOAL_HPP
#define GOAL_HPP
#include "common.hpp"
#include "Point.hpp"

class Goal
{
public:
    FPoint2f position;

    Goal()
    {
        position.x=500;
        position.y=-500;
    }

    void draw(Mat &image)
    {
        cv::circle(image,Point((int)position.x,(int)-/*w+*/position.y),3,Scalar(255,0,0),1,8);
    }



    void setPos(FPoint current)
    {
        position.x=current.x;
        position.y=current.y;
    }

    FPoint2f getPost()
    {
        return position;
    }

    Point getPosti()
    {
        return Point((int)position.x,(int)position.y);
    }
};

#endif // GOAL_HPP
