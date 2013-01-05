/*
 * This module describes fusion use of fusion method for image enhancement
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
