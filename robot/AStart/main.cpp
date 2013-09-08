#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/highgui.h>
// =======================
// YAGSBPL libraries
#include "yagsbpl_base.h"
#include "planners/A_star.h"
#include "queue"





int main(int argc, char *argv[])
{

    // Profiling observation: Using int instead of double cost provides marginal improvement (~10%)
    GenericSearchGraphDescriptor<myNode,double> myGraph;

    // We describe the graph, cost function, heuristics, and the start & goal in this block
    // ------------------------------------------------------------------------------------
    // Set the functions
    myGraph.getHashBin_fp = &getHashBin;
    myGraph.isAccessible_fp = &isAccessible;
    myGraph.getSuccessors_fp = &getSuccessors;
    myGraph.getHeuristics_fp = &getHeuristics;
    // Set other variables
    myGraph.hashTableSize = 25; // Since in this problem, "getHashBin" can return a max of value 201.

    myNode tempNode;
    tempNode.x = (int)(0.8*MIN_X); tempNode.y = (int)(0.8*MIN_Y); // Start node
    myGraph.SeedNode = tempNode;
    tempNode.x = (int)(0.8*MAX_X); tempNode.y = (int)(0.8*MAX_Y); // Goal node
    myGraph.TargetNode = tempNode;





}
