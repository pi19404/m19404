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
#ifndef BUG2_HPP
#define BUG2_HPP
#include "common.hpp"
#include "Point.hpp"
#include "Robot.hpp"
#include "Goal.hpp"
//class to check the robot lies on the MLine


//class implementing Bug2 algorithm
class Bug2 :public Robot
{


public:
    vector <Point> hl;
    MLine mline;
    enum method{BUG2,TBUG};
    int _method;
    bool mflag;
    bool freeheading;
    Bug2()
    {

    }

    //bounday following for Bug2 algorithm
    void changeState();
    void Follow();
    void draw(Mat &image);
    //main function
    void run();

};
#endif // BUG2_HPP
