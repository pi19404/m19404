#include "AStar.hpp"

float  GNode::getCost(GNode goal,int maxc)
{
    if(heuristic==10e8)
    {
        float val1=this->getHeuristics(this->position,goal.position);
        this->setHeuristics(val1);
    }


    return this->computeCost(curr_cost,heuristic,this->depth,maxc);
}

void GNode::setPosition(Point pos)
{
    position=pos;
}

void GNode::setHeuristics(float value)
{
    heuristic=value;
}

double GNode::getDistance(GNode n1,GNode n)
{
    float a=fabs(n1.position.x-n.position.x);
    float b=fabs(n1.position.y-n.position.y);
    return sqrt((double)(a*a+b*b));
}

GNode::GNode(int delta1)
{
     open=true;
     delta=delta1;
     heuristic=10e8;
     depth=0;
     nopen=0;
    //parent.resize(0);
}


void GNode::getConnected(GNode n)
{


    edge.resize(0);

    for (int a=-delta; a<=delta; a=a+delta)
        for (int b=-delta; b<=delta; b=b+delta) {

            if (a==0 && b==0) continue;
            if(abs(a)==1 && abs(b)==1)continue;


            GNode tn(delta);
            tn.position.x=position.x+a;
            tn.position.y=position.y+b;
            tn.curr_cost=getDistance(n,tn)+n.curr_cost;



            GNode gg(delta);
            gg.position=n.position;
            gg.curr_cost=n.curr_cost;
            gg.depth=n.depth;
            gg.parent=n.parent;
            tn.depth=n.depth+1;
            tn.parent.push_back(gg);
            edge.push_back(tn);

        }
}


AStar::AStar()
{
    source.position=Point(30,30);
    goal.position=Point(500,500);
    s.width=800;
    s.height=600;


    ccopen=0;
    cclose=0;
    rccopen=0;
    min.curr_cost=9999999;
    minr=5;


}



int AStar::run(int &copen1,int &cclose1,int &ropen)
{
    //GNode min1=min;
    //int index=min.index;

    //get the minimum node
    min=getMin();
    if(min.position==goal.position)
    {
        open.resize(0);
        return 0;
    }

    //if goal is accessible from current node
    if(isAccessible(goal,min,1)==true)
    {
        cclose++;
        closed.push_back(min);
        goal.parent.push_back(min);
        goal.curr_cost=min.curr_cost+min.getDistance(min,goal);
        GNode a=goal;
        min=goal;
        return 0;
    }

    //add to closed list
    closed.push_back(min);
    cclose++;

    //draw debug image
    cv::circle(image,Point(min.position.x,-min.position.y),3,Scalar(0,10*min.depth,10*min.depth),-1,8);

    //expand node
    for(int i=0;i<min.edge.size();i++)
    {
        GNode e=min.edge[i];

        if(!isAccessible(e,min,0))
            continue;

        if(e.position==source.position)
            continue;
        //skip if closed
        if(isClosed(e)==true )
        {
            continue;
        }

        float val1=(int)e.getCost(goal,maxc);
        float val2=(int)min.getCost(goal,maxc);

        //add open
        if(addOpen(e))
            cv::circle(image,Point(e.position.x,-e.position.y),3,Scalar(255,0,0),-1,-1);


    }

    //update statistics
    copen1=ccopen;
    cclose1=cclose;
    ropen=rccopen;

    return -1;

}


double GNode::getHeuristics(Point n1,Point n2)
{
    int dx = fabs(n1.x - n2.x);
    int dy = fabs(n1.y - n2.y);
    return sqrt((double)(dx*dx + dy*dy)); // Euclidean distance as heuristics
    return (dx+dy);
}


double  GNode::getHeuristics(GNode& n1, GNode& n2)
{
    int dx = fabs(n1.position.x - n2.position.x);
    int dy = fabs(n1.position.y - n2.position.y);
    return sqrt(((double)(dx*dx + dy*dy))); // Euclidean distance as heuristics
    return (dx+dy);
}


bool AStar::isClosed(GNode c)
{
    std::deque<GNode>::iterator it=closed.begin();
    for(it=closed.begin();it!=closed.end();it++)
    {
        GNode ix=*it;
        if(ix.position==c.position)
        {
/*            float d1=ix.getCost(goal,maxc);
            float d2=c.getCost(goal,maxc);
            if(d2<d1 && ix.open==true && c.open==true && ix.nopen<=5)
            {
            c.nopen=(ix).nopen+1;
            closed.erase(it);
            //c.getConnected(c);
            ///addOpen(c);
            }
*/
            return true;
        }
    }
    return false;
}



bool AStar::addOpen(GNode &node)
{

    std::deque<GNode>::iterator it=open.begin();
    std::deque<GNode>::iterator it1=open.end();

    bool flag=false;
    it=open.begin();
    int insert=0;
    int k=0;
    bool insert_flag=false;

    if(node.nopen>=5 &&1==0)
    {
        return false;
        node.open=false;
    }
    for(it=open.begin();it!=open.end();it++)
    {
        float val1=(*it).getCost(goal,maxc);
        float val2=node.getCost(goal,maxc);

        if(((*it).position==node.position))
        {
            if((*it).nopen>5 &&1==0)
            {
                return true;
            }
            //(val1>val2 && node.open==true)||
            if(insert_flag==true)
            {
                node.nopen=(*it).nopen+1;
                it=open.erase(it);
                rccopen++;


            }
            /*if(val2>=val1 && insert_flag==false)
            {
                insert_flag=true;
            }

            if(val2<val1 &&insert_flag==false)
            {
                node.nopen=(*it).nopen;
            }

            if(insert_flag==true)
            {
                    it1=open.begin()+insert;
                    (*it1).nopen=node.nopen;

                //cerr << (*it1).position.x <<":" << (*it1).position.x << endl;
                break;
            }*/

            insert_flag=true;
            break;

        }
        if(val2<val1 &&insert_flag==false &&node.open==true)
        {

            node.nopen=node.nopen+1;
            it=open.insert(it,node);
            //it++;
            flag=true;
            insert_flag=true;
            insert=k;
            ccopen++;
            //cerr << (*it).position.x <<":" << (*it).position.x << endl;
        }
        k++;

    }


    if(insert_flag==false)
    {
        node.nopen=node.nopen+1;
        open.push_back(node);
        //it=open.insert(it,node);
        ccopen++;
        insert=k;
        flag=true;
    }

    return flag;


}


GNode AStar::getMin()
{
    GNode min1=open.front();
    open.pop_front();
    min1.getConnected(min1);
    return min1;
}


bool AStar::isAccessible(GNode &n,GNode &n1,int mode)
{




    Point2f prev;
    prev.x=10e8;
    float dx=n.position.x-n1.position.x;
    float dy=n.position.y-n1.position.y;
    float mag=sqrt(dx*dx+dy*dy);
    dx=dx/mag;
    dy=dy/mag;

    Point2f p=n1.position;
    p.x=n1.position.x;
    p.y=n1.position.y;


    while(1)
    {
        if(p.x==n.position.x && p.y==n.position.y)
        {
            break;
        }


        float dx2=p.x-n1.position.x;
        float dy2=p.y-n1.position.y;
        float mag1=sqrt(dx2*dx2+dy2*dy2);

        float dx1=p.x-n.position.x;
        float dy1=p.y-n.position.y;
        float dd=sqrt((dx1*dx1)+(dy1*dy1));
        if(dd<=minr && mode==1)
        {
            return true;
        }


        if(mag1>=mag)
        {
            return true;
        }

        if(prev.x==p.x && prev.y==p.y )
        {
            p.x=p.x+(1)*dx;
            p.y=p.y+(1)*dy;
            continue;
        }

    prev=p;
    Point2f ff=Point2f(p.x,-p.y);

    if(ff.y >=obstacle_map.rows || ff.x>=obstacle_map.cols || ff.x<0 ||ff.y<0)
    {
        n1.open=false;
        //cv::circle(image,Point(n.position.x,-n.position.y),3,Scalar(0,0,255),3,-1);
        return false;
    }

    int val=(int)obstacle_map.ptr<uchar>(((int)ff.y))[((int)ff.x)];
    if(val>0)
    {

        n.open=false;
        return false;
    }
    p.x=p.x+(1)*dx;
    p.y=p.y+(1)*dy;
    //break;

   }
    return true;

}











/*
 *
 *  //check if node is inside obstacle or not
    bool isAccessible(Point n)
    {
        //cerr << n.position.x <<":" << n.position.y << endl;
        Point2f ff=Point2f(n.x,-n.y);


        for(int i=0;i<o.size();i++)
        {

            Obstacle o1=o[i];
            double d=pointPolygonTest( o1.contours[0], ff,false);
            bool flag,flag1;
            //check if point in inside the normal obstacle
            if(o1.type==0)
            {
                flag=d=0;
                //best=-999999;
            }
            //check if point in outside the boundary obstacle
            else
            {
                flag=d<0;
                //best=999999;
            }
            if(flag==true)
            {
                return false;
            }

        }
        return true;

    }


    double computeCost(GNode parent,GNode goal)
    {
         float w=0.5;
         float w1=getHeuristics(parent,goal);
         float w2=parent.curr_cost;

         float ww=parent.depth<=maxc?(1-parent.depth/maxc):0;

         ww=1;
         //w=(1/w2);

         return w2+(1+5*ww)*w1;
    }

*/
