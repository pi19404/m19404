#include <QApplication>
#include "mainwindow.h"

#include "color_constancy.hpp"

int main(int argc, char *argv[])
{

    if(argc<2)
        cerr << "Usage : ColorConstancy filename " << endl;



    Mat image=imread(argv[1]);

    cerr << "reading image " << argv[2] << endl;

    color_correction::contrast_stretching a;
    color_correction::gray_world b1;
    color_correction::gray_edge b2;
    color_correction::maxRGB b3;
    color_correction::max_edge b4;

    Mat input;
    input.create(240,320,CV_8UC(3));
    resize(image,input, input.size(), 0, 0, INTER_NEAREST);



    imshow("original",input);
    imshow("contrast stretching",a.run(input));
    imshow("gray world RGB",b1.run2(input,1,0));
    imshow("gray world Lab",b1.run1(input,1));
    imshow("Shades of gray",b1.run2(input,6,0));
    imshow("maxRGB",b3.run(input,6,0));
    imshow("gray edge",b2.run(input,1,0));
    imshow("max edge",b4.run(input,1,0));



    cv::waitKey(0);

}
