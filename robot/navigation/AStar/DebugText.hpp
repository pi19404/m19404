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
#ifndef DEBUGTEXT_HPP
#define DEBUGTEXT_HPP
#include "common.hpp"
#include "Point.hpp"

class DebugText
{
public:
    Point position;


    DebugText()
    {
        position.x=810;
        position.y=20;
    }

    void print(Mat &img,string text,bool flag=false)
    {
        if(flag==true)
        {
            position.y=20;
            Rect roi=Rect(position.x,position.y,img.cols-position.x,img.rows-position.y);
            Mat r=img(roi);
            r.setTo(cv::Scalar::all(0));

        }
        else
        {
            position.y=position.y+20;
        }
        int fontFace = cv::FONT_HERSHEY_PLAIN;
        double fontScale = 1;
        int thickness = 1;
        putText(img, text, position, fontFace, fontScale,
                Scalar(255,0,0), thickness, 8);

    }


};

#endif // DEBUGTEXT_HPP
