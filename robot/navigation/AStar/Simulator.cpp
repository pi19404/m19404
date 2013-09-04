#include "Simulator.hpp"


Simulator::Simulator(char* fname,char *fname2)
{
    w=1200;
    h=600;
    image1 = Mat::zeros( h,w, CV_8UC3 );
    r1.goal=r2;


    ReadObstacles(fname,fname2);


    image1.setTo(cv::Scalar::all(0));
    s.o=obstacles;
    status=false;
    winname="image";
}

Simulator::Simulator()
{
    winname="image";
}


void Simulator::draw(Mat &image)
{
    Mat roi=image(Rect(0,0,map.cols,map.rows));
    map.copyTo(roi);
    cv::rectangle(roi,Rect(0,0,roi.cols-1,roi.rows-1),Scalar(255,255,255),3,8);
}


void Simulator::run()
{
    int count=0;
    while(1)
    {
    image1.copyTo(image);
    ///s.o[0].draw(image);
    //cerr << count << endl;
    if(count==5 || status==false)
    draw(image);

    vector<ScanPoint> i;
    int countx=0;
    if(status==true)
    {
    r1.run();
    countx=s.run(r1);
    r1.scan1=s.scan1;
    r1.countx=countx;

    i=s.getPoints();
    r1.scan=i;

    r1.scount=countx;
    r1.best_index=s.besti;

    if(count==5 || status==false)
    {
    r1.draw(image);
    s.draw(image,i,r1);
    }

    }

    if(count==5 || status==false)
    r1.drawHeading(image);





    if(count==5 || status==false)
    {
    char text[1000];
    sprintf(text,"Obstacles :%d",this->obstacles.size());
    d.print(image,text,true);
    sprintf(text,"Detected Obstacles :%d",s.scan1.size());
    d.print(image,text,false);
    sprintf(text,"Robot :%f,%f",r1.position.x,-/*w+*/r1.position.y);
    d.print(image,text,false);
    sprintf(text,"Goal :%f,%f",r1.goal.position.x,-/*w+*/r1.goal.position.y);
    d.print(image,text,false);
    sprintf(text,"Heading Angle :%f",r1.heading);
    d.print(image,text,false);
    sprintf(text,"Distance :%f",r1.distance);
    d.print(image,text,false);
    sprintf(text,"Intersection :%d",countx);
    d.print(image,text,false);
    sprintf(text,"State :%d",r1.m.value);
    d.print(image,text,false);
    sprintf(text,"Best :%d",(int)(r1.best_index+r1.heading));
    d.print(image,text,false);
    sprintf(text,"HPoints :%d",(int)(r1.hl.size()));
    d.print(image,text,false);
    sprintf(text,"Free heading :%d",(r1.freeheading==true));
    d.print(image,text,false);
    sprintf(text,"Heading to goal :%d",(int)(r1.cur_heading));
    d.print(image,text,false);


//    sprintf(text,"Next :%d-%d-%d",(int)r1.next.position.x,(int)r1.next.position.y,(int)(r1.next.index+r1.heading));
//    d.print(image,text,false);

    imshow(this->winname,image);
    int k=cv::waitKey(10);
    if(k=='e')
        break;
    if(k=='p')
        cv::waitKey(0);
    }

    count++;
    if(count>5)
        count=0;
    }

}

void Simulator::ReadObstacles(const char fname[],const char fname2[])
{

    Mat tmp=cv::imread(fname,1);
    Mat map1;
    cv::resize(tmp,map,Size(800,h),0,0,INTER_LINEAR);
    cvtColor(map,map1,CV_BGR2GRAY);


    vector<Vec4i> hierarchy;

    findContours( map1, contours, hierarchy, cv::RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Obstacle *obst;
    for(int i=0;i<contours.size();i++)
    {
        obst = new Obstacle();
        obst->type=0;
        obst->contours.push_back(contours[i]);
        obstacles.push_back(*obst);
    }


    tmp=cv::imread(fname2,1);
    Mat map2;
    cv::resize(tmp,map2,Size(800,h),0,0,INTER_LINEAR);
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
