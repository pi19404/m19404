#include "gsim.h"
#include "common.hpp"
GSim::GSim(char* fname,char *fname2,char * val,int factor=4,int minr=5)
{
    w=1200;
    h=600;
    image1 = Mat::zeros( h,w, CV_8UC3 );
    start=false;

    this->factor=factor;
    this->g.minr=minr/factor;
    //factor=4;
    int del=atoi(val);
    //del=atoi(val[1])*10+del;
    //cerr << del << endl;
    g.delta=del/factor;

    ReadObstacles(fname,fname2);


    image1.setTo(cv::Scalar::all(0));
    status=false;
    winname="image";
    Mat roi=image1(Rect(0,0,map.cols,map.rows));
    map.copyTo(roi);
}

GSim::GSim()
{
    winname="image";
    status=false;
    factor=4;
}


Mat GSim::draw(Mat &image)
{

    Mat d;
    image.copyTo(d);
    Mat roi=image(Rect(0,0,map.cols,map.rows));
    cv::rectangle(roi,Rect(0,0,map.cols,map.rows),Scalar(255,255,255),3,8);
    cv::circle(roi,Point(g.source.position.x,-g.source.position.y),3,Scalar(0,0,255),1,-1);
    cv::circle(roi,Point(g.goal.position.x,-g.goal.position.y),3,Scalar(255,0,0),1,-1);


    Mat tmp2;
    cv::resize(roi,tmp2,Size(800,h),0,0,INTER_CUBIC);
    roi=d(Rect(0,0,800,h));
    tmp2.copyTo(roi);
    return d;
    //imshow("FFFF",roi);



}


void GSim::run()
{
    Size S=Size(w,h);
    VideoWriter outputVideo;
    outputVideo.open("sim1.avi", CV_FOURCC('M', 'J', 'P', 'G'),15, S,true);

    if(outputVideo.isOpened()==true)
    {
        cerr << "recording video " <<endl;
    }
    int count=0;
    while(status==false)
    {
        image1.copyTo(image);

        char text[1000];
        sprintf(text,"Obstacles :%d",this->obstacles.size());
        d.print(image,text,true);
        sprintf(text,"Robot :%d,%d",g.source.position.x,-g.source.position.y);
        d.print(image,text,false);
        sprintf(text,"Gaol :%d,%d",g.goal.position.x,-g.goal.position.y);
        d.print(image,text,false);

        Mat d=draw(image);
        imshow(winname,d);

        //imshow(winname,image);
        int k=cv::waitKey(10);
        if(k=='e')
            break;
        if(k=='p')
            cv::waitKey(0);

         outputVideo <<d;

    }

    //pre compute

//    g.delta=20;

    int ret=-1;
    int copen=0;
    int cclose=0;
    float dist=0;

    int rcopen=0;
    int step=0;

    for(int i=g.delta;i>=g.minr;i++)
    {
    g.open.resize(0);
    g.closed.resize(0);
    g.source.delta=g.delta;
    g.source.curr_cost=0;
    g.cclose=0;
    g.ccopen=0;
    g.rccopen=0;
    g.min.curr_cost=999999;
    g.min.delta=g.delta;
    g.min.position.x=-g.goal.position.x;
    g.min.position.y=-g.goal.position.y;
    //g.min.parent.push_back(0;
    //g.source.parent.push_back(null);
    g.source.parent.resize(0);
    g.min.parent.resize(0);
    g.open.push_back(g.source);
    image1.copyTo(image);
    g.image=image;
    //image.copyTo(g.image);
    Mat dx=draw(image);
    imshow(winname,dx);

    g.o=this->obstacles;
    g.obstacle_map=this->obstacle_map;

    ret=-1;
    copen=0;
     cclose=0;
     dist=0;
      rcopen=0;
    step=0;

    while(1)
    {


        if(ret==0)
            break;

        if(g.open.size()<=0)
        {
            ret=99;
            g.delta=g.delta-g.minr;
            break;
        }



         if(ret!=99)
         ret=g.run(copen,cclose,rcopen);
         if(ret==0)
         {
             vector<GNode> path;
             GNode cur=g.min;
             while(1)
             {

                 /*if(g.getHeuristics(cur,g.source)==0)
                     break;*/

                 path.push_back(cur);



                 if(cur.parent.size()==0)
                     break;

                 GNode y1=cur.parent[0];
                 cur=y1;

             }

             //cerr << path.size() << endl;
             for(int i=path.size()-1;i>0;i--)
             {

                 GNode r=path[i];
                 //cerr << r.curr_cost << endl;
                 //cerr << path[i].position.x << ":" << path[i].position.y << ":" << endl;
                 Point p=Point(path[i].position.x,-path[i].position.y);
                 Point q=Point(path[i-1].position.x,-path[i-1].position.y);
                 cv::circle(g.image,p,3,Scalar(0,255,255),1,-1);
                 //cv::line(g.image,p1,p2,Scalar(255,0,0),3,8);
                    int arrowMagnitude=10/factor;
                     //Draw the principle line
                     cv::line(g.image,p,q,Scalar(0,255,0),3,8);
                     //const double PI = 3.141592653;
                     //compute the angle alpha
                     double angle = atan2((double)p.y-q.y, (double)p.x-q.x);
                     //compute the coordinates of the first segment
                     p.x = (int) ( q.x +  arrowMagnitude * cos(angle+PI/4));
                     p.y = (int) ( q.y +  arrowMagnitude * sin(angle+PI/4));
                     //Draw the first segment
                     cv::line(g.image,p,q,Scalar(0,0,255),1,8);
                     p.x = (int) ( q.x +  arrowMagnitude * cos(angle-PI/4));
                     p.y = (int) ( q.y +  arrowMagnitude * sin(angle-PI/4));
                     //Draw the second segment
                     cv::line(g.image,p,q,Scalar(0,0,255),1,8);


                 dist=r.curr_cost;

             }
             step=path.size();


         }





         char text[1000];
         sprintf(text,"Obstacles :%d",this->obstacles.size());
         d.print(g.image,text,true);

         sprintf(text,"Robot :%d,%d",g.source.position.x,-g.source.position.y);
         d.print(image,text,false);
         sprintf(text,"Gaol :%d,%d",g.goal.position.x,-g.goal.position.y);
         d.print(image,text,false);
         sprintf(text,"Opened  %d",copen);
         d.print(g.image,text,false);
         sprintf(text,"Closed  :%d",cclose);
         d.print(g.image,text,false);
         sprintf(text,"Re Opened  %d",rcopen);
         d.print(g.image,text,false);
         sprintf(text,"Distance  :%f",dist);
         d.print(g.image,text,false);
         sprintf(text,"Steps  :%d",step);
         d.print(g.image,text,false);
         sprintf(text,"Delta  :%d",g.delta);
         d.print(g.image,text,false);

         Mat dx=draw(g.image);
         imshow(winname,dx);
         outputVideo <<dx;
         int k=cv::waitKey(10);
         if(k=='e')
         {
             cv::waitKey(0);
             outputVideo.release();
         }
         if(k=='p')
         {
             cv::waitKey(0);
             break;
         }

    }



    char text[1000];
    sprintf(text,"Obstacles :%d",this->obstacles.size());
    d.print(g.image,text,true);

    sprintf(text,"Robot :%d,%d",g.source.position.x,-g.source.position.y);
    d.print(image,text,false);
    sprintf(text,"Gaol :%d,%d",g.goal.position.x,-g.goal.position.y);
    d.print(image,text,false);
    sprintf(text,"Opened  %d",copen);
    d.print(g.image,text,false);
    sprintf(text,"Closed  :%d",cclose);
    d.print(g.image,text,false);
    sprintf(text,"Re Opened  %d",rcopen);
    d.print(g.image,text,false);
    sprintf(text,"Distance  :%f",dist);
    d.print(g.image,text,false);
    sprintf(text,"Steps  :%d",step);
    d.print(g.image,text,false);
    sprintf(text,"Delta  :%d",g.delta);
    d.print(g.image,text,false);
    if(ret==99)
    {
    sprintf(text,"NOT FOUND");
    d.print(g.image,text,false);
    }

//    imshow(winname,g.image);
    dx=draw(g.image);
    imshow(winname,dx);
    outputVideo <<dx;

    int k=cv::waitKey(10);
//    cv::waitKey(0);
    if(ret==0)
        break;
    }


    if(outputVideo.isOpened()==true)
        outputVideo.release ();
    //cerr << "AAAAAAAAAAAA" << endl;
    cv::waitKey(0);
    exit(1);
}

void GSim::ReadObstacles(const char fname[],const char fname2[])
{

    Mat tmp=cv::imread(fname,1);
    Mat map1;

    cv::resize(tmp,map,Size(800/factor,h/factor),0,0,INTER_LINEAR);
    cvtColor(map,map1,CV_BGR2GRAY);


    vector<Vec4i> hierarchy;

    findContours( map1, contours, hierarchy, cv::RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Obstacle *obst;
    maxc=9999999;
    for(int i=0;i<contours.size();i++)
    {
        obst = new Obstacle();
        obst->type=0;
        obst->contours.push_back(contours[i]);
        if(maxc>arcLength(contours[i],true))
        {
            maxc=arcLength(contours[i],true);
        }
        obstacles.push_back(*obst);
        g.maxc=maxc;
        g.factor=factor;
    }


    cvtColor(map,map1,CV_BGR2GRAY);
//    g.o=this->obstacles;
    map1.copyTo(obstacle_map);
    obstacle_map.setTo(cv::Scalar::all(0));

    char xx[1000];
    strcpy(xx,fname);

    //strcat((char*)xx,(char*)"map.png");
    //obstacle_map=imread((char*)(xx),0);
    //if(obstacle_map.data==0)
    {
        map1.copyTo(obstacle_map);
        //obstacle_map.setTo(cv::Scalar::all(0));

    /*
    for(int i=0;i<obstacle_map.rows;i=i+1)
    {
        uchar * a=obstacle_map.ptr<uchar>(i);
        uchar * a1=map1.ptr<uchar>(i);
        for(int j=0;j<obstacle_map.cols;j=j+1)
        {

            Point p=Point2f(j,i);

            for(int k=0;k<obstacles.size();k++)
            {

                Obstacle o1=obstacles[k];
                double d=pointPolygonTest( o1.contours[0],p,false);
                bool flag,flag1;
                //check if point in inside the normal obstacle
                if(o1.type==0)
                {
                    flag=d>=0;
                    //best=-999999;
                }
                //check if point in outside the boundary obstacle
                else
                {
                    flag=d<=0;
                    //best=999999;
                }
                if(flag==true)
                {
                    a[j]=255;
                }
                else
                {
                    a[j]=0;
                }

            }

            //cerr << "AAAAAAAAAAAAAaa:"<<i<<":"<<j <<endl;
        }
    }
    */
    ///imwrite(xx,obstacle_map);
    }



    //imshow("XX",obstacle_map);
    //cv::waitKey(0);

    return;
    tmp=cv::imread(fname2,1);
    Mat map2;
    cv::resize(tmp,map2,Size(800/factor,h/factor),0,0,INTER_LINEAR);
    cvtColor(map2,map1,CV_BGR2GRAY);


    findContours( map1, contours, hierarchy, cv::RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


    for(int i=0;i<contours.size();i++)
    {
        obst = new Obstacle();
        obst->type=1;
        obst->contours.push_back(contours[i]);
        obstacles.push_back(*obst);
    }











}
