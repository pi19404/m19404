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

