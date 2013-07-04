
#include <math.h>
#include <fstream>
#include <vector>
#include <algorithm>
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

void TrainWeakClassifiers()
{
    int i,j;
    REAL* errors; REAL* threshes;
    int* parity; int* indexes;
    ofstream of;

    errors = new REAL[totalfeatures]; ASSERT(errors!=NULL);
    threshes = new REAL[totalfeatures]; ASSERT(threshes!=NULL);
    indexes = new int[totalfeatures]; ASSERT(indexes!=NULL);
    parity = new int[totalfeatures]; ASSERT(parity!=NULL);

    InitializeWeights();
    for(i=0;i<totalfeatures;i++)
    {
        FillTheTable(table[i],classifiers[i]);
        for(j=0;j<totalcount;j++) labels[j] = trainset[j].label;
        SingleFeatureClassifier(labels,classifiers[i],table[i]);
        errors[i] = classifiers[i].error;
        parity[i] = classifiers[i].parity;
        threshes[i] = classifiers[i].thresh;
    }
    for(i=0;i<totalfeatures;i++) indexes[i] = i;
    QuickSort(errors,indexes,threshes,parity,0,totalfeatures-1);

    of.open(FFS_WeakClassifiers_filename);
    for(i=0;i<totalfeatures;i++)
        of<<indexes[i]<<" "<<threshes[i]<<" "<<errors[i]<<" "<<parity[i]<<endl;
    of.close();

    delete[] errors; errors = NULL;
    delete[] threshes; threshes = NULL;
    delete[] indexes; indexes = NULL;
    delete[] parity; parity = NULL;
}

void LoadWeakClassifiers(int* const indexes)
{
    ifstream f;
    int i,j;

    f.open(FFS_WeakClassifiers_filename);
    for(i=0;i<totalfeatures;i++)
    {
        f>>indexes[i];
        f>>classifiers[indexes[i]].thresh;
        f>>classifiers[indexes[i]].error;
        f>>classifiers[indexes[i]].parity;
    }
    f.close();

    for(i=0;i<totalfeatures;i++)
        for(j=0;j<totalcount;j++)
            table[i][j]=classifiers[i].Apply(trainset[j]);
}

void BuildHistogram(const int* const curresult, const int size,int* const hist_pos, int* const hist_neg, const int facecount)
{
    int i;

    memset(hist_pos,0,sizeof(*hist_pos)*size);
    memset(hist_neg,0,sizeof(*hist_neg)*size);
    for(i=0;i<facecount;i++) hist_pos[curresult[i]]++;
    for(i=facecount;i<totalcount;i++) hist_neg[curresult[i]]++;
}

void FFS_Histogram(AdaBoostClassifier& ada,const int round,int* const curresult,ofstream& f,const int method)
{
    int i,j,k;
    int* hist_pos; int* hist_neg;
    int* tempresult;
    int error,minerror,minindex;
    int thresh,minthresh;
    bool* used;

    tempresult = new int[totalcount]; ASSERT(tempresult!=NULL);
    hist_pos = new int[round+1]; ASSERT(hist_pos!=NULL);
    hist_neg = new int[round+1]; ASSERT(hist_neg!=NULL);
    used = new bool[totalfeatures]; ASSERT(used!=NULL);
    for(i=0;i<totalfeatures;i++) used[i]=false;
    f<<"Begin Histogram FFS"<<endl;
    ada.InitToGivenSize(round);
    for(i=0;i<totalcount;i++) curresult[i] = 0;
    for(i=0;i<round;i++)
    {
        minerror = INT_MAX; minindex = -1; minthresh = INT_MAX;
        for(j=0;j<totalfeatures;j++)
        {
            if(used[j]) continue;
            int temp;
            for(k=0;k<totalcount;k++) tempresult[k] = curresult[k] + table[j][k];
            BuildHistogram(tempresult,i+2,hist_pos,hist_neg,facecount);
            error = temp = totalcount-facecount;
            for(k=0;k<=i+1;k++)
            {
                temp = temp - hist_neg[k] + hist_pos[k];
                if(temp<error)	{ error = temp; thresh = k+1; }
            }
            if(error<minerror)
            {
                minerror = error;
                minindex = j;
                minthresh = thresh;
            }
        }
        used[minindex]=true;
        ada.alphas[ada.count] = 1;
        ada.scs[ada.count] = classifiers[minindex];
        ada.count++;
        ada.thresh = REAL(minthresh);
        for(j=0;j<totalcount;j++) curresult[j] += table[minindex][j];
        f<<i+1<<"\t"<<minindex<<"\t"<<minerror<<"\t"<<classifiers[minindex].thresh<<endl;
    }
    BuildHistogram(curresult,round+1,hist_pos,hist_neg,facecount);
    for(i=1;i<=round;i++) { hist_neg[i] += hist_neg[i-1]; hist_pos[i] += hist_pos[i-1]; }
    i=0; while(hist_neg[i]<(totalcount-facecount)*(1-node_fp_goal)) i++;
    switch(method)
    {
        case GOAL_METHOD_MIN_ERROR:
            break;
        case GOAL_METHOD_VALID_DR:
            ValidateTheThreshold(ada,f);
            break;
        case GOAL_METHOD_FIX_FP:
            ada.thresh = i + REAL(0.5);
            break;
    }
    f<<ada.thresh<<"\t"<<hist_pos[i-1]<<"\t"<<totalcount-facecount-hist_neg[i-1]<<endl;

    int fn,fp;
    f<<"Training Error:"<<endl;
    fn = 0; for(i=0;i<facecount;i++) if(ada.ApplyImagePatch(trainset[i])==0) fn++;
    fp = 0; for(i=facecount;i<totalcount;i++) if(ada.ApplyImagePatch(trainset[i])==1) fp++;
    f<<fn<<"\t"<<fp<<endl;
    f<<"Validation Error:"<<endl;
    fn = 0; for(i=0;i<validfacecount;i++) if(ada.ApplyImagePatch(validset[i])==0) fn++;
    fp = 0; for(i=validfacecount;i<validcount;i++) if(ada.ApplyImagePatch(validset[i])==1) fp++;
    f<<fn<<"\t"<<fp<<endl;
    f<<"End of Histogram FFS"<<endl;
    delete[] hist_pos; hist_pos=NULL;
    delete[] hist_neg; hist_neg=NULL;
    delete[] tempresult; tempresult=NULL;
}

void ForwardFeatureSelection(AdaBoostClassifier& ada,const int round,const bool update,const int method)
{
    int* curresult;
    int* indexes;
    ofstream f;

    ada.Clear();

    curresult = new int[totalcount]; ASSERT(curresult!=NULL);
    indexes = new int[totalfeatures]; ASSERT(indexes != NULL);
    f.open(FFS_log_filename);
    //cerr << "training weak classifier " << endl;
    TrainWeakClassifiers();
    LoadWeakClassifiers(indexes);
    FFS_Histogram(ada,round,curresult,f,method);
    if(update) AppendAdaBoostClassifier(ada);

    delete[] curresult; curresult=NULL;
    delete[] indexes; indexes=NULL;
    f.flush();
    f.close();
}
