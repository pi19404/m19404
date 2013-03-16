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
#include "timeMeasure.hpp"
using namespace std;

int main(int argv, char** argc){

    //string a1=(argc[1]);
    //string a2=(argc[2]);

    //objects to pre process image
    //preprocess p;
    //p.set_hfilter_flag(true);


    //string source=a1+"/"+a2;
 //   VideoCapture inputVideo(source);              // Open input
    seed_fill_stack fill;
    VideoCapture inputVideo(0);
    if (!inputVideo.isOpened())
    {
        cout  << "Could not open the input video: "<< endl;
        return -1;
    }
    int xx=150;
    int yy=150;

    Mat a,src;
    a.create(240,320,CV_8UC(3));
    inputVideo >> src;
    resize(src,a, a.size(), 0, 0, INTER_NEAREST);


    int k=0;
    Mat i4;


    do//Show the image captured in the window and repeat
    {
        inputVideo >> src;              // read

        ///src=imread(source);
        Mat x=src;

        if (src.empty()) break;         // check if at end
        resize(src,a, a.size(), 0, 0, INTER_AREA);


        Mat grow=fill.RegionGrowing (a,150,150,Scalar(0,0,0));
        cv::circle(grow,Point(150,150),10,Scalar(0,0,255),-1);
       imshow("DDDD",grow);


    k=cv::waitKey (1);
    }while(k!='r');

}

int main1(int argv, char** argc){


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
    TimeMeasure time1;
    seed_fill_stack rr;

    struct timeval start,end,result;
    int k1=0;
    int k=0;
    gettimeofday(&start,NULL);

    do
    {

        src=imread(source);
        Mat x=src;

        if (src.empty()) break;         // check if at end
        resize(src,a, a.size(), 0, 0, INTER_AREA);




        Mat region_grow=rr.RegionGrowing (a,xx,yy,Scalar(255,255,255));

        cv::circle(a,Point(xx,yy),10,Scalar(255,255,255),1,8);
        imshow("input image",a);
          //cv::imwrite("/data1/xx"+a2+".png",region_grow);
          imshow("pre process",region_grow);
          k1=cv::waitKey(1);

          k=k+1;
          if(k%30==0 && k>0)
          {
              gettimeofday(&end,NULL);
              long int diff=time1.timeval_subtract(&result,&end,&start);
              double elapsed_secs = diff ;
              cerr << "FPS is " << (double)30*1000000/elapsed_secs << endl;
              gettimeofday(&start,NULL);
              k=0;
          }

    }while(k1!='r');


    return 0;
}

