

#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <float.h>
using namespace std;
#include "IntImage.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"
#include "CascadeClassifier.h"
#include "Global.h"
#include "FFS.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AdaBoostClassifier::AdaBoostClassifier():count(0),thresh(0.0),alphas(NULL),scs(NULL)
{
}

AdaBoostClassifier::~AdaBoostClassifier()
{
    Clear();
}

void AdaBoostClassifier::Clear()
{
    delete[] scs; scs=NULL;
    delete[] alphas; alphas=NULL;
    count = 0;
    thresh = 0.0;
}

AdaBoostClassifier& AdaBoostClassifier::operator=(const AdaBoostClassifier& source)
{
    InitToGivenSize(source.count);
    count = source.count;
    thresh = source.thresh;
    memcpy(alphas,source.alphas,sizeof(*alphas)*count);
    for(int i=0;i<count;i++) scs[i] = source.scs[i];
    return *this;
}

void AdaBoostClassifier::InitToGivenSize(const int size)
{
    Clear();
    count = 0;
    thresh = 0.0;
    alphas = new REAL[size]; ASSERT(alphas!=NULL);
    scs = new SimpleClassifier[size]; ASSERT(scs!=NULL);
}

void AdaBoostClassifier::WriteToFile(ofstream& f) const
{
    f<<count<<endl;
    f<<thresh<<endl;
    for(int i=0;i<count;i++) f<<alphas[i]<<" ";
    f<<endl;
    for(int i=0;i<count;i++) scs[i].WriteToFile(f);
    f<<endl;
}

void AdaBoostClassifier::ReadFromFile(ifstream& f)
{
    Clear();
    if(!f.is_open())
        cerr << "adaboost file is not open" << endl;
    f>>count;
    cerr << " : no of weak learners " << count << endl;
    ASSERT(count>0);
    f.ignore(256,'\n');
    scs = new SimpleClassifier[count];
    ASSERT(scs!=NULL);
    alphas = new REAL[count];
    ASSERT(alphas!=NULL);

    f>>thresh; f.ignore(256,'\n');
    for(int i=0;i<count;i++)
        f>>alphas[i];
    f.ignore(256,'\n');
    for(int i=0;i<count;i++)
        scs[i].ReadFromFile(f);
    f.ignore(256,'\n');

   //cerr << "completed reading the adaboost files " << endl;
}

void AdaBoostClassifier::TrainAdaBoost(int rounds,const bool update,const int method)
{
    ofstream f;
    int i,j;
    REAL* curresult;
    int fn,fp;
    bool* used;
    cerr << "training adaboost round " << rounds << endl;
    InitToGivenSize(rounds);
    for(i=0;i<totalfeatures;i++)
        FillTheTable(table[i],classifiers[i]);
    InitializeWeights();
    used = new bool[totalfeatures];
    ASSERT(used!=NULL);
    for(i=0;i<totalfeatures;i++) used[i]=false;

    f.open(ada_log_filename);
    curresult = new REAL[totalcount];
    ASSERT(curresult!=NULL);
    f<<facecount<<"\t"<<totalcount-facecount<<endl;
    for(i=0;i<totalcount;i++) curresult[i] = 0.0;
    for(i=0;i<rounds;i++)
    {
        int err;

        if(asym_ratio>1.0) SkewWeight(asym_ratio);
        AddOneSimpleClassifier(used);
        for(j=0;j<totalcount;j++) curresult[j] += (alphas[i] * scs[i].Apply(trainset[j]));

        err = 0;
        for(j=0;j<totalcount;j++)
            if( (curresult[j]<thresh && trainset[j].label==1) || (curresult[j]>=thresh && trainset[j].label==0) )
                err++;
        f<<i<<"\t"<<err<<endl;
        f<<i<<"\t"<<alphas[i]<<"\t"<<scs[i].thresh<<" "<<scs[i].parity<<" "<<scs[i].type<<" "<<scs[i].x1<<" "<<scs[i].x2<<" "<<scs[i].x3<<" "<<scs[i].x4<<" "<<scs[i].y1<<" "<<scs[i].y2<<" "<<scs[i].y3<<" "<<scs[i].y4<<endl;
    }
    nth_element(curresult+facecount,curresult+facecount+int((totalcount-facecount)*(1-node_fp_goal)),curresult+totalcount);
    switch(method)
    {
        case GOAL_METHOD_MIN_ERROR:
            break;
        case GOAL_METHOD_VALID_DR:
            ValidateTheThreshold(*this,f);
            break;
        case GOAL_METHOD_FIX_FP:
            thresh = curresult[facecount+int((totalcount-facecount)*(1-node_fp_goal))];
            break;
    }
    fn = 0; for(i=0;i<facecount;i++) if(curresult[i]<thresh) fn++;
    fp = 0; for(i=facecount;i<totalcount;i++) if(curresult[i]>=thresh) fp++;
    f<<fn<<"\t"<<fp<<endl;
    f<<"Validation Error:"<<endl;
    fn = 0; for(i=0;i<validfacecount;i++) if(ApplyImagePatch(validset[i])==0) fn++;
    fp = 0; for(i=validfacecount;i<validcount;i++) if(ApplyImagePatch(validset[i])==1) fp++;
    f<<fn<<"\t"<<fp<<endl;
    f.close();

    if(update) AppendAdaBoostClassifier(*this);
    delete[] used; used=NULL;
    delete[] curresult; curresult = NULL;
}

void AdaBoostClassifier::TrainLDS(int rounds,const bool update,const int method)
{
    REAL* curresult;
    int i,j;
    ofstream f;
    int fn,fp;

    if(train_method==TRAIN_ADA)
        TrainAdaBoost(rounds,false,method);
    else if(train_method==TRAIN_FFS)
        ForwardFeatureSelection(*this,rounds,false,method);

    if(linear_classifier==LC_LAC || linear_classifier==LC_FDA)
    {
        WithinClassScatter(*this);
        curresult = new REAL[totalcount]; ASSERT(curresult!=NULL);
        for(i=0;i<totalcount;i++) curresult[i]=0.0;
        for(i=0;i<totalcount;i++)
            for(j=0;j<count;j++)
                curresult[i] += alphas[j]*scs[j].Apply(trainset[i]);

        nth_element(curresult+facecount,curresult+facecount+int((totalcount-facecount)*(1-node_fp_goal)),curresult+totalcount);
        if(train_method==TRAIN_ADA)
            f.open(ada_log_filename, ios_base::out|ios_base::app);
        else if(train_method==TRAIN_FFS)
            f.open(FFS_log_filename, ios_base::out|ios_base::app);
        switch(method)
        {
            case GOAL_METHOD_MIN_ERROR:
                break;
            case GOAL_METHOD_VALID_DR:
                ValidateTheThreshold(*this,f);
                break;
            case GOAL_METHOD_FIX_FP:
                thresh = curresult[facecount+int((totalcount-facecount)*(1-node_fp_goal))];
                break;
        }
        fn = 0; for(i=0;i<facecount;i++) if(ApplyImagePatch(trainset[i])==0) fn++;
        fp = 0; for(i=facecount;i<totalcount;i++) if(ApplyImagePatch(trainset[i])==1) fp++;
        f<<fn<<"\t"<<fp<<endl;
        f<<"LDS	 Error:"<<endl;
        fn = 0; for(i=0;i<validfacecount;i++) if(ApplyImagePatch(validset[i])==0) fn++;
        fp = 0; for(i=validfacecount;i<validcount;i++) if(ApplyImagePatch(validset[i])==1) fp++;
        f<<fn<<"\t"<<fp<<endl;
        f.close();
        delete[] curresult; curresult=NULL;
    }
    if(update) AppendAdaBoostClassifier(*this);
}

void SingleFeatureClassifier(const int* const labels,SimpleClassifier& sc,const int* const indexes)
{
    int i;
    REAL min1; REAL min2;
    REAL e1,e2;
    int pos1,pos2;

    // compute errors1, suppose parity is 1, that is f(x)<thresh ==> h(x) = 1
    // compute errors2, suppose parity is 0, that is f(x)>thresh ==> h(x) = 1
    e1 = 0.0; for(i=0;i<totalcount;i++) if(labels[i]!=0) e1+=weights[i];
    e2 = REAL(1.0)-e1;
    min1 = e1; pos1 = 0;
    min2 = e2; pos2 = 0;
    for(i=0;i<totalcount;i++)
    {
        if(labels[indexes[i]]!=0) e1-=weights[indexes[i]]; else e1+=weights[indexes[i]];
        if(e1<min1) { min1=e1; pos1=i; }
        e2 = 1 - e1;
        if(e2<min2) { min2=e2; pos2=i; }
    }

    pos1++; if(pos1==totalcount) pos1--;
    pos2++; if(pos2==totalcount) pos2--;
    if(min1<min2)
    {
        sc.parity = 1;
        sc.error = min1;
        sc.thresh = sc.GetOneFeature(trainset[indexes[pos1]]);
    }
    else
    {
        sc.parity = 0;
        sc.error = min2;
        sc.thresh = sc.GetOneFeature(trainset[indexes[pos2]]);
    }
}

void AdaBoostClassifier::AddOneSimpleClassifier(bool* used)
{
    int i;
    REAL alpha,beta;
    SimpleClassifier minsc;
    REAL minerror;
    int minindex;

    minerror = REAL(1.01); minindex  = -1;
    for(i=0;i<totalcount;i++) labels[i] = trainset[i].label;
    NormalizeWeight();
    fprintf(stderr,"%f %f %f %f\n",*min_element(weights,weights+facecount)*10000,*max_element(weights,weights+facecount)*10000,*min_element(weights+facecount,weights+totalcount)*10000,*max_element(weights+facecount,weights+totalcount)*10000);
    for(i=0;i<totalfeatures;i++)
    {
        if(used[i]) continue;
        SingleFeatureClassifier(labels,classifiers[i],table[i]);
        if(classifiers[i].error<minerror)
        {
            minerror = classifiers[i].error;
            minsc = classifiers[i];
            minindex = i;
        }
    }

    used[minindex] = true;
    beta = minsc.error / (REAL(1.0)-minsc.error);
    for(i=0;i<totalcount;i++)
        if(minsc.Apply(trainset[i]) == trainset[i].label)
            weights[i] *= beta;
    if(beta<REAL(1e-8)) beta = REAL(1e-8);
    alpha = -log(beta);

    scs[count] = minsc;
    alphas[count] = alpha;
    thresh += (REAL(0.5)*alpha);
    count++;
}
