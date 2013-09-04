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
#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP
#include "common.hpp"
#include "Point.hpp"

class Obstacle
{


public:
    vector<FPoint> Points;
    vector<Point> p1;
    vector<Point2f> p2;
    vector<vector<Point> > contours;
    int  type;
    Obstacle(){};

    void process(Mat src)
    {
         Mat tt;

        /** Create some points */

          p1.resize(0);
          p2.resize(0);
          for(int i=0;i<Points.size();i++)
          {
              Point p=Points[i];
              p1.push_back(Point(p.x,-p.y));
              p2.push_back(Point2f(p.x,-p.y));
          }

          /// Draw it in src
          for( int j = 0; j < p2.size(); j++ )
             {
              line( src, p1[j],  p1[(j+1)%p2.size()], Scalar::all(255),10, 8 );
             }

          //cvtColor(src,tt,CV_BGR2GRAY);
          vector<Mat> ch;
          cv::split(src,ch);
          /// Get the contours
           vector<Vec4i> hierarchy;
           findContours( ch[0], contours, hierarchy, cv::RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

          //cerr << contours.size() << endl;


    }

    void draw( Mat & img )
    {
      int lineType = 8;


      const Point* ppt[1] = {&p1[0]};
      int npt[] = { p1.size()};

      fillPoly( img,
                ppt,
                npt,
                1,
                Scalar( 0, 255, 0 ),
                lineType );
     }
};

#endif // OBSTACLE_HPP
