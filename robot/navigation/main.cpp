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
#include "Obstacle.hpp"
#include "Goal.hpp"
#include "DebugText.hpp"
#include "MotionState.hpp"
#include "Sensor.hpp"
#include "Simulator.hpp"
#include "Robot.hpp"
#include "Bug2.hpp"

Simulator s;
int k=0;
static void onMouse(int event, int x, int y, int, void* )
{
 if( event != EVENT_LBUTTONDOWN )
     return;

 if(k==0)
 {
     s.r1.position.x=x;
     s.r1.position.y=-y;
     s.r1.start=s.r1.position;
     k++;
 }
 else if(k==1)
 {
     s.r1.goal.position.x=x;
     s.r1.goal.position.y=-y;
     k++;

 }
 else if(k==2)
 {
     s.r1.radius=40;
     s.s.radius=s.r1.radius;
     s.r1.mline=Robot::MLine(s.r1.position,s.r1.goal.position,s.r1.radius);
     s.status=true;
     s.r1.computeHeading();
 }

}

int main(int argc, char *argv[])
{


    s=Simulator(argv[1],argv[2]);
    imshow(s.winname,s.image1);
    setMouseCallback(s.winname,onMouse,0);
    s.run();

    cv::waitKey(0);


}

