#include "TemporalFilter.hpp"

//#include <iostream> // for standard I/O
//#include <string>   // for strings

//#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
//#include <opencv2/highgui/highgui.hpp>  // Video write

//int fps=30;
//int F=0;
//int main(int argc, char *argv[])
//{


//    string ins="/home/pi19404/Downloads/person15_walking_d1_uncomp.avi";
//    VideoCapture inputVideo(ins);              // Open input



//    if (!inputVideo.isOpened())
//    {
//        cout  << "Could not open the input video: " << ins << endl;
//        return -1;
//    }

//    int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

//    // Transform from int to char via Bitwise operators
//    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};


//    //Size S=Size(320,240);
//    VideoWriter outputVideo;
//    string source="/tmp/out.avi";
//    Size S = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),    //Acquire input size
//                  (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
//    outputVideo.open(source, ex,inputVideo.get(CV_CAP_PROP_FPS),S,true);
//    if (!outputVideo.isOpened())
//       {
//           cout  << "Could not open the output video for write: " << source << endl;
//           return -1;
//       }

//    Temporal_Filter filter;

//    Mat simage;
//    Mat eimage;
//    Mat a,dst,image;


//    a.create(240,320,CV_8UC(1));
//    int k=0;


//    clock_t begin = clock();
//    while(1){

//    inputVideo >> image;              // read
//    if (image.empty()) break;         // check if at end

//    cvtColor(image,image,CV_BGR2GRAY);
//    resize(image,a, a.size(), 0, 0, INTER_NEAREST);

//    imshow("input",a);
//    Mat out;
//    a.copyTo(out);


//    dst=filter.temporal_filter(out);


//    imshow("output",dst);

//    outputVideo.write(dst);  //or
//    if(cv::waitKey(30)>0)
//        break;
//    k++;
//    if(k==fps)
//    {
//    clock_t end = clock();
//    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;


//    k=0;
//    cerr << "a " << (double)fps/elapsed_secs << endl;
//    begin = clock();
//    }

//    }
//    return 0;



//}
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

    Size S = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));

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

    for(;;) //Show the image captured in the window and repeat
    {
        inputVideo >> src;              // read

        if (src.empty()) break;         // check if at end
        imshow("input",src);
        cvtColor(src,src,CV_BGR2GRAY);
        //resize(image,a, a.size(), 0, 0, INTER_NEAREST);
        dst=filter.temporal_filter(src);

        cvtColor(dst,res,CV_GRAY2BGR);
       //outputVideo.write(res); //save or
       outputVideo << res;
       imshow("filtered",res);
       cv::waitKey(1);
    }

    cout << "Finished writing" << endl;
    return 0;
}
