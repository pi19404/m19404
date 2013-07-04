

#include <fstream>
#include <vector>
#include <math.h>
using namespace std;
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"
#include "CascadeClassifier.h"
#include "Global.h"
#include "FFS.H"
#include "Learn.h"



void SimpleClassifier::ReadFromFile(ifstream& f)
{
  //  if(!f.is_open())
   //   cerr << "simple classifier file is not open" << endl;


    f>>thresh>>parity>>type;
    f>>x1>>x2>>x3>>x4>>y1>>y2>>y3>>y4;
    f.ignore(256,'\n');
    ASSERT(parity == 0 || parity == 1);
    ASSERT(type>=0 && type<=4);

}

void SimpleClassifier::WriteToFile(ofstream& f) const
{
    f<<thresh<<" ";
    f<<parity<<" ";
    f<<type<<" ";
    f<<x1<<" ";
    f<<x2<<" ";
    f<<x3<<" ";
    f<<x4<<" ";
    f<<y1<<" ";
    f<<y2<<" ";
    f<<y3<<" ";
    f<<y4<<" ";
    f<<endl;
}
