#ifndef ASTAR_HPP
#define ASTAR_HPP
#include <Obstacle.hpp>

//class defining basic 2D point
class Node
{
    public:
    Point position;
    int index;
    // copy constructor
    bool operator==(const Node& n)
    {
        return (position.x==n.position.x && position.y==n.position.y);
    }

};

//class defining node for 2D graph
class GNode : Node
{
public:
    //conntains vector of edges
    vector<GNode> edge;
    //contains pointer to the parent
    vector<GNode> parent;
    //current location
    Point position;
    //actual cost to come to node
    double curr_cost;
    //heuristic cost to goal node
    double heuristic;
    //depth of the node
    int depth;
    //dist to adjacent grid node
    int delta;
    //if node can be openened
    bool open;
    int index;
    //number of times node has been opened
    int nopen;
    //check if two nodes are equal
    bool operator==(const GNode& n)
    {
        return (position.x==n.position.x && position.y==n.position.y);
    }

    //constructor
    GNode(int delta1=5);

    //heuristic distance between two nodes
    double  getHeuristics(GNode& n1, GNode& n2);
    double  getHeuristics(Point n1,Point n2);

    //set the position
    void setPosition(Point pos);

    //set the heuristic value
    void setHeuristics(float value);

    //get the cost of the node
    float getCost(GNode goal,int maxc=0);

    //get the heuristic distance
    double getDistance(GNode n1,GNode n);

    //get connected nodes
    void getConnected(GNode n);

    float computeCost(float v1,float v2,float ww,float maxc)
    {
         float w=0.5;
          ww=ww<=maxc?(1-ww/maxc):0;

         ww=0;
         //w=(1/w2);

         return v1+(1+5*ww)*v2;
    }


};



//graph class
class AStar
{
    public:
    //obstacle map
    Mat obstacle_map;
    //contains source & goal location
    GNode source;
    GNode goal;

    //open and closed list
    deque<GNode> open;
    deque<GNode> closed;

    //mininmum resolution of workspace
    int minr;

    //current minimum node
    GNode min;

    Size s;

    //vector of obstacles
    vector<Obstacle> o;

    //minimum distance between adjacent grid nodes
    int delta;
    //max obstacle perimeter
    int maxc;

    //stats about nodes open closed,reopened
    int ccopen;
    int cclose;
    int rccopen;


    //scale factor for drwing
    float factor;



    //get the best node
    GNode getMin();
    //check if node is accessible/connected
    bool isAccessible(GNode &n,GNode &n1,int mode);
    //check if node is in closed list
    bool isClosed(GNode c);
    //add node to open list
    bool addOpen(GNode &node);




    //constructor
    AStar();

    //output image
    Mat image;


    //main method
    int run(int &copen1,int &cclose1,int &ropen);



};










#endif // ASTAR_HPP
