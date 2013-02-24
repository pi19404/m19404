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
#include <iostream>
#include "RegionGrowing.hpp"
using namespace std;

int main(int argv, char** argc){


    if(argv <4 )
    {
        cerr << "RegionGrowing directory file x y " << endl;
        return -1;
    }

    string a1=(argc[1]);
    string a2=(argc[2]);
    int xx=atoi(argc[3]);
    int yy=atoi(argc[4]);

    string source=a1+"/"+a2;


    Mat a,src;
    a.create(240,320,CV_8UC(3));

    seed_fill rr;


    int k=0;
    do
    {

        src=imread(source);
        Mat x=src;

        if (src.empty()) break;         // check if at end
        resize(src,a, a.size(), 0, 0, INTER_AREA);


        imshow("input image",a);

        Mat region_grow=rr.RegionGrowing (a,xx,yy);

          cv::imwrite("/data1/xx"+a2+".png",region_grow);
          imshow("pre process",region_grow);
          k=cv::waitKey(0);


    }while(k!='r');


    return 0;
}

