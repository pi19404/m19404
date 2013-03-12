/*
 * This file is main file for demo temporal filter program
 * using opencv
 * Copyright (C) 2012 by pi19404
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TemporalFilter.hpp"

#include <iostream> // for standard I/O
#include <string>   // for strings

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{

    Temporal_Filter filter;
    Mat dst;
 //   help();

    //if (argc != 4)
    {
      //  cout << "Not enough parameters" << endl;
       // return -1;
    }

    const string source      = argv[1];           // the source file name
    const bool askOutputType = false;  // If false it will use the inputs codec type

    VideoCapture inputVideo(source);              // Open input
    if (!inputVideo.isOpened())
    {
        cout  << "Could not open the input video: " << source << endl;
        return -1;
    }

    string::size_type pAt = source.find_last_of('.');                  // Find extension point
    const string NAME = argv[2];   // Form the new name with container
    int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

    // Transform from int to char via Bitwise operators
    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};

    Size S = Size(320,240);   // Acquire input size
                  //(int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));

    VideoWriter outputVideo;                                        // Open the output
    if (askOutputType)
        outputVideo.open(NAME, ex=-1, inputVideo.get(CV_CAP_PROP_FPS), S, true);
    else
        outputVideo.open(NAME, ex, inputVideo.get(CV_CAP_PROP_FPS), S, true);

    if (!outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write: " << source << endl;
        return -1;
    }

    cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
         << " of nr#: " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) << endl;
    cout << "Input codec type: " << EXT << endl;


    Mat src, res;
    vector<Mat> spl;
    Mat a;
    a.create(240,320,CV_8UC(3));


    for(;;) //Show the image captured in the window and repeat
    {
        inputVideo >> src;              // read

        if (src.empty()) break;         // check if at end

        cvtColor(src,src,CV_BGR2GRAY);
        resize(src,a, a.size(), 0, 0, INTER_NEAREST);
        imshow("input",a);
        dst=filter.temporal_filter(a);

        cvtColor(dst,res,CV_GRAY2BGR);
       //outputVideo.write(res); //save or
       outputVideo << res;
       imshow("filtered",res);
       cv::waitKey(1);
    }

    cout << "Finished writing" << endl;
    return 0;
}
