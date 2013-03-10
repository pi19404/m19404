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
#include "ContrastStretching.hpp"
#include "Noise.hpp"

using namespace std;

int main(int argv, char** argc){


    string a1=(argc[1]);
    string a2=(argc[2]);



    string source=a1+"/"+a2;

    VideoCapture inputVideo;         // Open input

    VideoWriter outputVideo;

    int vmode=0;
    if(strcmp(argc[3],"V")==0)
    {
        inputVideo.open (source);
        vmode=1;
    }
    else if(strcmp(argc[3],"C")==0)
    {
        inputVideo.open (0);
        vmode=2;
    }
    else
    {
        vmode=0;
    }


    if(vmode==1||vmode==2)
    {
        if (!inputVideo.isOpened())
        {
            cout  << "Could not open the input video: " << source << endl;
            return -1;
        }

        Size S = Size(320*2,240*2);

                          // Open the output
        outputVideo.open("out.avi", CV_FOURCC('M', 'J', 'P', 'G'), inputVideo.get(CV_CAP_PROP_FPS), S,true);



        if (!outputVideo.isOpened())
        {
            cout  << "Could not open the output video for write: " << source << endl;
            return -1;
        }


    }



    Mat a,src;
    a.create(240,320,CV_8UC(3));

    TimeMeasure t;
    struct timeval start,end,result;


    gettimeofday(&start,NULL);
    int k=0,i=0;

    ContrastStretching stretch;


    do//Show the image captured in the window and repeat
    {

        if(vmode==1|vmode==2)
        inputVideo >> src;              // read
        else if(vmode==0)
        src=imread(source);
        ///cerr << source << endl;
        if (src.empty()) break;         // check if at end
        resize(src,a, a.size(), 0, 0, INTER_AREA);


        double min1,max1;;
        //cv::minMaxLoc (a,&min1,&max1,0,0);

        stretch.setmode (Mode::DEFAULT);
        Mat out1=stretch.GlobalContrastStretching (a);
        imshow("input image",a);
        imshow("Global contrast Stretcing",out1);

        Scalar m1=stretch.getMin ();
        Scalar m2=stretch.getLUT (m1);
        Scalar m3=stretch.getMax ();
        Scalar m4=stretch.getLUT (m3);

        resize(src,a, a.size(), 0, 0, INTER_AREA);
        cerr << "input image        -minimum  " << m1[0] << ":" << m1[1] <<":" << m1[2] << endl;
        cerr << "stretched image    -minimum  " << m2[0] << ":" << m2[1] <<":" << m2[2] << endl;
        cerr << "input image        -maximum  " << m3[0] << ":" << m3[1] <<":" << m3[2] << endl;
        cerr << "stretched image    -maximum  " << m4[0] << ":" << m4[1] <<":" << m4[2] << endl;


        //Adding Gaussian Noise to Image
         GaussianNoise noise;
         Mat b=noise.add (a,0,10);
         imshow("noisy image",b);

         out1=stretch.GlobalContrastStretching (b);
        imshow("noisy stretched image",out1);
        Scalar m5=stretch.getMin ();
        Scalar m6=stretch.getMax();
        m2=stretch.getLUT (m1);
        m4=stretch.getLUT (m3);
        cerr << endl;
        cerr << endl;
        cerr << "noisy input image        -minimum  " << m5[0] << ":" << m5[1] <<":" << m5[2] << endl;
        cerr << "stretched image    -minimum  " << m2[0] << ":" << m2[1] <<":" << m2[2] << endl;
        cerr << "noisy input image        -maximum  " << m6[0] << ":" << m6[1] <<":" << m6[2] << endl;
        cerr << "stretched image    -maximum  " << m4[0] << ":" << m4[1] <<":" << m4[2] << endl;

        cerr << endl;
        cerr << endl;

        stretch.setmode (Mode::V1);
        out1=stretch.GlobalContrastStretching (a);
        imshow("modified  stretch",out1);

        m2=stretch.getLUT (m1);
        m4=stretch.getLUT (m3);
        cerr << endl;
        cerr << endl;
        cerr << "noisy input image        -minimum  " << m5[0] << ":" << m5[1] <<":" << m5[2] << endl;
        cerr << "stretched image    -minimum  " << m2[0] << ":" << m2[1] <<":" << m2[2] << endl;
        cerr << "noisy input image        -maximum  " << m6[0] << ":" << m6[1] <<":" << m6[2] << endl;
        cerr << "stretched image    -maximum  " << m4[0] << ":" << m4[1] <<":" << m4[2] << endl;

          imshow("modified  stretch",out1);
        cerr << endl;
        cerr << endl;


        k=cv::waitKey (1);
       i=i+1;
       if(i%30==0 && i>0)
       {
           gettimeofday(&end,NULL);
           long int diff=t.timeval_subtract(&result,&end,&start);
           double elapsed_secs = diff ;
           cerr << "FPS is " << (double)30*1000000/elapsed_secs << endl;
           gettimeofday(&start,NULL);
           i=0;
       }

    }while(k!='r' && vmode!=0);
    cv::waitKey (0);
}
