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
#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP
#include "common.hpp"
#include "Point.hpp"
#include "Robot.hpp"
#include "Goal.hpp"
#include "Sensor.hpp"
#include "Bug2.hpp"
#include "DebugText.hpp"
#include "tangentbug.hpp"
class Simulator
{
    public:
    //vector containing obstacle information
    vector<Obstacle> obstacles;
    //function to read the obstacle
    void ReadObstacles(const char fname[],const char fname2[]);

    //dimensions of simulation environment
    int w;
    int h;

    Bug2 r1;
    Goal r2;

    //class for printing debug information
    DebugText d;

    //class which implements the range sensor
    Sensor s;


    vector<vector<Point> > contours;
    Mat image1,image,map;

    //display window name
    string winname;
    bool status;


    //constructors for simulator
    Simulator(char* fname,char *fname2);
    Simulator();


    //draw the simulation environment
    void draw(Mat &image);


    //main loop of the simulator
    void run();

};

#endif // SIMULATOR_HPP
