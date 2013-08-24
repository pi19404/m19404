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

#ifndef SENSOR_HPP
#define SENSOR_HPP
#include "common.hpp"
#include "Point.hpp"
#include "Robot.hpp"
class Sensor
{

    int radius;
    //vector containing obstacle information

    vector<ScanPoint> points;
    //vector containing information about points hitting obstacles
    vector<ScanPoint> final;

    Robot r;


public:
    vector<Obstacle> o;
    int besti;
    //constructors
    Sensor();

    //draw sensor output
    void draw(Mat &image,vector<ScanPoint>i,Robot r);

    //compute the distance between points
    float dist(FPoint p,FPoint q);
    //compute the distance between points
    float dist(FPoint2f p,FPoint2f q);

    //validate if obstacle is hit
    bool validate(float fx,float fy,FPoint p1,FPoint p2);

    //convert degrees to radiands
    float toRadians(float angle);

    //main sensor function to be called
    vector<ScanPoint> run(Robot robot);

};



#endif // SENSOR_HPP
