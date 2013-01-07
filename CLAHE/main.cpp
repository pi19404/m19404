/*
 * This file is main file for CLAHE image enhancement technique
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
#include "HistogramEqualization.hpp"
#include <iostream> // for standard I/O
#include <string>   // for strings

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write


int n=10;
int nbins=256;
int clip_factor=14;

Mat src;
string dname,name;
/**
 * @function on_trackbar
 * @brief Callback for trackbar
 */
void on_trackbar( int, void* )
{

    HistogramEqualization p;
    imshow("original image",src);
    imshow("image histogram",p.plot_hist(src));

    Mat output=p.run(src,p.GLOBAL);
    imshow("equalized image",output);
    imshow("equalized image histogram",p.plot_hist(output));

    int nx=n;
    int ny=n;
    int bins=nbins;
    float limit=clip_factor*0.1;

    Mat ahe=p.AHE(src,nx,ny,bins,0,limit);

    imshow("normalized image",ahe);
    Mat ahe1=p.AHE(src,nx,ny,bins,2,0);

    imshow("local HE",ahe1);
    Mat ahe2=p.AHE(src,nx,ny,bins,1,0);

    imshow("with interp",ahe2);

    Mat ahe3=p.AHE(src,nx,ny,bins,1,limit);
    imshow("CLAHE",ahe3);


    string oname1=dname+"/o"+name;
    string oname=dname+"/o1"+name;
    string oname2=dname+"/o2"+name;
    string oname3=dname+"/o3"+name;
    string oname4=dname+"/o4"+name;
    string oname5=dname+"/o5"+name;
    string oname6=dname+"/o6"+name;
    string oname7=dname+"/o7"+name;
    string oname8=dname+"/o8"+name;
    string oname9=dname+"/o9"+name;
    string oname10=dname+"/o10"+name;


    cv::imwrite(dname+"/"+name,src);
    cv::imwrite(oname1,p.plot_hist(src));
    cv::imwrite(oname,output);
    cv::imwrite(oname2,p.plot_hist(output));
    cv::imwrite(oname3,ahe);
    cv::imwrite(oname4,p.plot_hist(ahe));
    cv::imwrite(oname5,ahe2);
    cv::imwrite(oname6,p.plot_hist(ahe2));
    cv::imwrite(oname7,ahe1);
    cv::imwrite(oname8,p.plot_hist(ahe1));
    cv::imwrite(oname9,ahe3);
    cv::imwrite(oname10,p.plot_hist(ahe3));



}

int main(int argc,char *argv[])
{

    Mat image;
    dname=argv[1];
    name=argv[2];

    src.create(240,320,CV_8UC(3));
    image=imread(dname+"/"+name);
    resize(image,src,src.size(), 0, 0, INTER_NEAREST);


     namedWindow("original image", 1);
    createTrackbar( "no blocks", "original image", &n, 100, on_trackbar );
    createTrackbar( "no bins", "original image", &nbins,256, on_trackbar );
    createTrackbar( "clip factor", "original image", &clip_factor,100, on_trackbar );


    on_trackbar( n, 0 );
    on_trackbar( nbins, 0 );
    on_trackbar( clip_factor, 0 );

    cv::waitKey(0);


}
