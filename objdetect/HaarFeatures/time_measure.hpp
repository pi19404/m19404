#ifndef TIME_MEASURE_HPP
#define TIME_MEASURE_HPP
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

        static long int timeval_subtract(struct timeval *t2, struct timeval *t1)
        {
            long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
            diff=diff/1000000;
            //result->tv_sec = diff / 1000000;
            //result->tv_usec = diff % 1000000;

            return (diff);
        }
};




#endif // TIME_MEASURE_HPP
