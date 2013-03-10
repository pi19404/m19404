#ifndef COMMON1_H
#define COMMON1_H

/**
 * @file common.h
 * @brief file containing common header files
 * @author Prashant Iyengar
 */

/** @brief file containing common header files*/
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "stdio.h"
#include "string.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <sys/time.h>
using namespace cv;
using namespace std;
#include <string>
#include <sstream>
#include <iostream>
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv/cv.h"
#include <stdlib.h>
#include <vector>
#include <stack>
#include <iostream>
#include <map>
#include <ctime>
#include "highgui.h"


#ifndef TIMEMEASURE
#define TIMEMEASURE

#include "stdio.h"
#include "string.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <sys/time.h>

//Class to measure time elapsed between any two events in the code
class TimeMeasure{
    private:
        //Holds information regarding various points
        std::vector<struct timeval> timePoints;
        std::vector<std::string> timeDetails;

    public:
        //COnstructor
        TimeMeasure(){}

        //Method to add a new time check point
        void addTimePoint(std::string timeDescription){
            struct timeval now;

            gettimeofday(&now, NULL);
            timePoints.push_back(now);
            if(timePoints.size() == 0){
                timeDetails.push_back("Start");
            }
            else{
                timeDetails.push_back(timeDescription);
            }
        }

        void printTimePoints(){
            for(int i = 1; i < timePoints.size(); i++){
                struct timeval result;
                long int diff =timeval_subtract(&result,&timePoints[i],&timePoints[i-1]);

                //char *b=timeval_print(&result);
                std::cout<<timeDetails[i-1]<<" => "<<timeDetails[i]<<" - "<<diff<<" us" <<std::endl;


//                int timePassed = (timePoints[i].tv_usec - timePoints[i-1].tv_usec) ;//+ 10000000*(timePoints[i].tv_sec - timePoints[i-1].tv_sec);
                //std::printf("%s => %s  -  %d ms", timeDetails[i-1], timeDetails[i], timePassed);

            }
        }

        char * timeval_print(struct timeval *tv)
        {
            char buffer[30];
            char buffer1[100];
            time_t curtime;

            //printf("%ld.%06ld", tv->tv_sec, tv->tv_usec);
            curtime = tv->tv_sec;
            //strftime(buffer, 30, "%m-%d-%Y  %T", localtime(&curtime));
            sprintf(buffer1," = %06ld\n",tv->tv_usec);
            return buffer1;
        }

        /* Return 1 if the difference is negative, otherwise 0.  */
        long int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
        {
            long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
            //result->tv_sec = diff / 1000000;
            //result->tv_usec = diff % 1000000;

            return (diff);
        }
};

#endif


#endif // COMMON1_H
