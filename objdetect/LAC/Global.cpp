
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
#include "error.h"
#define CString CStdString
#define REAL double
CString option_filename = "options.txt";
string dir="/tmp";
int total_fp;

CString trainset_filename;
CString validset_filename;
CString classifier_filename;
CString ada_log_filename;
CString cascade_filename;
CString FFS_WeakClassifiers_filename;
CString FFS_log_filename;
CString FileUsage_log_filename;
CString Bootstrap_database_filename;
CString Backup_directory_name;
CString TestSet_filename;
int sx; //height
int sy; //width
int train_method;
int linear_classifier;
int bootstrap_level;
int max_bootstrap_level;
vector<REAL> bootstrap_resizeratio;
vector<int> bootstrap_increment;
HaarFeaturesSet s;
int totalfeatures;
int max_files;
int goal_method;
REAL node_det_goal;
REAL node_fp_goal;
int first_feature;
REAL asym_ratio;
int max_nodes;
vector<int> nof;
int starting_node;
int facecount,validfacecount;

IntImage* trainset=NULL;
IntImage* validset=NULL;
int totalcount;
int validcount;

CascadeClassifier* cascade=NULL;
REAL* weights=NULL;
int** table=NULL;
SimpleClassifier* classifiers=NULL;

REAL* features=NULL;
int* labels=NULL;

int* fileused=NULL;
int bootstrap_size;
CString* bootstrap_filenames;

REAL mean_min, mean_max, sq_min, sq_max, var_min, var_max;




void WriteRangeFile(void)
// images are already integral images.
{
    CString filename;
    ofstream f;

    filename = cascade_filename + ".range";
    f.open(filename);
    mean_min = REAL(sx*sy*256); sq_min = REAL(sx*sy*256*256); var_min = 256;
    mean_max = 0; sq_max = 0; var_max = -256;
    for(int i=0;i<facecount;i++)
    {
        REAL ex, sq, v;

        ex = sq = 0;
        for(int j=1;j<trainset[i].height;j++)
            for(int k=1;k<trainset[i].width;k++)
            {
                v = trainset[i].data[j][k]+trainset[i].data[j-1][k-1]-trainset[i].data[j-1][k]-trainset[i].data[j][k-1];
                sq += (v*v);
            }
        ex = trainset[i].data[sx][sy];
        mean_min = min(mean_min,ex);
        mean_max = max(mean_max,ex);
        sq_min = min(sq_min,sq);
        sq_max = max(sq_max,sq);
        var_min = min(var_min,trainset[i].variance);
        var_max = max(var_max,trainset[i].variance);
    }
    mean_min *= REAL(0.90);
    mean_max *= REAL(1.10);
    sq_min *= REAL(0.90);
    sq_max *= REAL(1.10);
    var_min *= REAL(0.90);
    var_max *= REAL(1.10);
    f<<mean_min<<endl<<mean_max<<endl<<sq_min<<endl<<sq_max<<endl<<var_min<<endl<<var_max<<endl;
    f.close();
}

void ReadRangeFile(void)
{
    CString filename;
    ifstream f;

    filename = cascade_filename + ".range";
    f.open(filename);
    f>>mean_min>>mean_max>>sq_min>>sq_max>>var_min>>var_max;
    f.close();
}

void ReadOneTrainingSample(ifstream& is,IntImage& image)
{
    int i,j;
    char buf[256];

    ASSERT(sx<=256 && sy<=256);
    is>>image.label;

    is.ignore(256,'\n');
    ASSERT( (image.label == 0) || (image.label == 1) );

    is>>image.height>>image.width; 
    is.ignore(256,'\n');
    ASSERT(image.height==sx);
    ASSERT(image.width==sy);

    image.SetSize(QSize(image.width+1,image.height+1));
    for(i=0;i<image.height;i++) image.data[i][0] = 0;
    for(i=0;i<image.width;i++) image.data[0][i] = 0;
    for(i=1;i<image.height;i++)
    {
        is.read(buf,image.width-1);
        for(j=1;j<image.width;j++)
        {
            image.data[i][j] = ((REAL)(int((unsigned char)(buf[j-1]))));
            ASSERT(image.data[i][j]>=0 && image.data[i][j] <= 255);
        }
    }
    is.ignore(256,'\n');
}


REAL Integral(REAL **data,const cv::Rect & r )
{
    REAL f1=data[r.x][r.y]+data[r.x+r.width][r.y+r.height]-data[r.x][r.y+r.height]-data[r.x+r.width][r.y];
    return f1;
}


void GetFeatureValues0(REAL* const features,const int from,const int to,const vector<Rectangles> &features1)
{
    int i;
    REAL f1;
    REAL** data;

    for(i=from;i<to;i++)
    {

        data = trainset[i].data;
        f1=0;
        REAL f2=0;
        for(int j=0;j<features1.size ();j++)
        {
            Rectangles r=features1[j];
            f2=Integral (data,r._r);
            f2=f2*r._weight;
            f1=f1+f2;
            //f1 =   data[x1][y3] - data[x1][y1] + data[x3][y3] - data[x3][y1]+ 2*(data[x2][y1] - data[x2][y3]);


        }
        features[i] = f1 / trainset[i].variance;
    }
}


void GetFeatureValues0(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int y1,const int y3)
{
    int i;
    REAL f1;
    REAL** data;

    for(i=from;i<to;i++)
    {
        data = trainset[i].data;
        f1 =   data[x1][y3] - data[x1][y1] + data[x3][y3] - data[x3][y1]
             + 2*(data[x2][y1] - data[x2][y3]);
        features[i] = f1 / trainset[i].variance;
    }
}

void GetFeatureValues1(REAL* const features,const int from,const int to,const int x1,const int x3,const int y1,const int y2,const int y3)
{
    int i;
    REAL f1;
    REAL** data;

    for(i=from;i<to;i++)
    {
        data = trainset[i].data;
        f1 =   data[x3][y1] + data[x3][y3] - data[x1][y1] - data[x1][y3]
             + 2*(data[x1][y2] - data[x3][y2]);
        features[i] = f1 / trainset[i].variance;
    }
}

void GetFeatureValues2(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int x4,const int y1,const int y3)
{
    int i;
    REAL f1;
    REAL** data;

    for(i=from;i<to;i++)
    {
        data = trainset[i].data;
        f1 =   data[x1][y1] -data[x1][y3] + data[x4][y3] - data[x4][y1]
             + 3*(data[x2][y3] - data[x2][y1] + data[x3][y1] - data[x3][y3]);
        features[i] = f1 / trainset[i].variance;
    }
}

void GetFeatureValues3(REAL* const features,const int from,const int to,const int x1,const int x3,const int y1,const int y2,const int y3,const int y4)
{
    int i;
    REAL f1;
    REAL** data;

    for(i=from;i<to;i++)
    {
        data = trainset[i].data;
        f1 =   data[x1][y1] - data[x1][y4] + data[x3][y4] - data[x3][y1]
             + 3*(data[x3][y2] - data[x3][y3] + data[x1][y3] - data[x1][y2] );
        features[i] = f1 / trainset[i].variance;
    }
}

void GetFeatureValues4(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int y1,const int y2,const int y3)
{
    int i;
    REAL f1;
    REAL** data;

    for(i=from;i<to;i++)
    {
        data = trainset[i].data;
        f1 =   data[x1][y1] + data[x1][y3] + data[x3][y1] + data[x3][y3]
             - 2*(data[x2][y1] + data[x2][y3] + data[x1][y2] + data[x3][y2])
             + 4*data[x2][y2];
        features[i] = f1 / trainset[i].variance;
    }
}

void QuickSort(REAL* values,int* labels,const int lo,const int hi)
{
    int i=lo, j=hi;
    REAL v; int l;
    REAL x = values[(lo+hi)/2];

    do
    {
        while (values[i]<x) i++;
        while (values[j]>x) j--;
        if (i<=j)
        {
            v=values[i]; values[i]=values[j]; values[j]=v;
            l=labels[i]; labels[i]=labels[j]; labels[j]=l;
            i++; j--;
        }
    } while (i<=j);

    if (lo<j) QuickSort(values,labels,lo, j);
    if (i<hi) QuickSort(values,labels,i, hi);
}

void QuickSort(REAL* values,int* labels,REAL* weights,int* parity,const int lo,const int hi)
{
    int i=lo, j=hi;
    REAL v; int l; REAL w; int p;
    REAL x = values[(lo+hi)/2];

    do
    {
        while (values[i]<x) i++;
        while (values[j]>x) j--;
        if (i<=j)
        {
            v=values[i]; values[i]=values[j]; values[j]=v;
            l=labels[i]; labels[i]=labels[j]; labels[j]=l;
            w=weights[i];weights[i]=weights[j];weights[j]=w;
            p=parity[i];parity[i]=parity[j];parity[j]=p;
            i++; j--;
        }
    } while (i<=j);

    if (lo<j) QuickSort(values,labels,weights,parity, lo, j);
    if (i<hi) QuickSort(values,labels,weights,parity, i, hi);
}

void FillTheTable(int* const row,const SimpleClassifier& sc)
{
    int i;
    int x1,x2,x3,x4,y1,y2,y3,y4;

    x1 = sc.x1;	y1 = sc.y1;
    x2 = sc.x2;	y2 = sc.y2;
    x3 = sc.x3; y3 = sc.y3;
    x4 = sc.x4;	y4 = sc.y4;
    switch(sc.type)
    {
        case 0:
            GetFeatureValues0(features,0,totalcount,x1,x2,x3,y1,y3);
            break;
        case 1:GetFeatureValues1(features,0,totalcount,x1,x3,y1,y2,y3);break;
        case 2:GetFeatureValues2(features,0,totalcount,x1,x2,x3,x4,y1,y3);break;
        case 3:GetFeatureValues3(features,0,totalcount,x1,x3,y1,y2,y3,y4);break;
        case 4:GetFeatureValues4(features,0,totalcount,x1,x2,x3,y1,y2,y3);break;
    }
    for(i=0;i<totalcount;i++) row[i] = i;
    QuickSort(features,row,0,totalcount-1);
}

const int CountTrainFaces()
{
    int i,count;

    count = 0;
    for(i=0;i<totalcount;i++)
        count += trainset[i].label;
    return count;
}

const int CountValidFaces()
{
    int i,count;

    count = 0;
    for(i=0;i<validcount;i++)
        count += validset[i].label;
    return count;
}

void InitializeWeights()
{
    int i;

    for(i=0;i<facecount;i++) weights[i] = REAL(0.5)/facecount;
    for(i=facecount;i<totalcount;i++) weights[i] = REAL(0.5)/(totalcount-facecount);
}

void ReadClassifiers()
{
    ifstream f;
    int i;

    //f.open(classifier_filename);
    for(i=0;i<totalfeatures;i++)
    {
    classifiers[i].ReadFromFile(f);
//    classifiers[i].type=s.feature[i].type;
    //copy(s.feature[i].features.begin(),s.feature[i].features.end(),classifiers[i].features.begin ());
  //  classifiers[i].features=s.feature[i].features;
    }
    //f.close();
}

void IgnoreComments(ifstream& f)
{
    f.ignore(65536,'\n');
}

void ReadlnString(ifstream& f, CString& s,string dir)
{
    char buf[256];
    f.getline(buf,255,'\n');
    s = buf;
    s=dir +"/"+buf;
}

void ReadlnString(ifstream& f, CString& s)
{
    char buf[256];
    f.getline(buf,255,'\n');
    s = buf;
}


void LoadOptions()
{



    //s.generateFeatures ();
    ifstream f;
    int i;
    cerr << "loading options from file " << option_filename << endl;
    dir=option_filename;
    option_filename=option_filename+"/options.txt";
    f.open(option_filename);
    if(f.is_open()==false)
    {
        cerr << "error in loading options " <<endl;
        exit(1);
    }

    IgnoreComments(f); ReadlnString(f,trainset_filename,dir);
    IgnoreComments(f); ReadlnString(f,validset_filename,dir);
    IgnoreComments(f); ReadlnString(f,classifier_filename,dir);
    IgnoreComments(f); ReadlnString(f,ada_log_filename,dir);
    IgnoreComments(f); ReadlnString(f,cascade_filename,dir);
    IgnoreComments(f); ReadlnString(f,FFS_WeakClassifiers_filename,dir);
    IgnoreComments(f); ReadlnString(f,FFS_log_filename,dir);
    IgnoreComments(f); ReadlnString(f,FileUsage_log_filename,dir);
    IgnoreComments(f); ReadlnString(f,Bootstrap_database_filename,dir);
    IgnoreComments(f); ReadlnString(f,Backup_directory_name,dir);
    IgnoreComments(f); ReadlnString(f,TestSet_filename,dir);

    IgnoreComments(f); f>>sx; IgnoreComments(f);
    IgnoreComments(f); f>>sy; IgnoreComments(f);
    IgnoreComments(f); f>>train_method; IgnoreComments(f);
    IgnoreComments(f); f>>linear_classifier; IgnoreComments(f);
    IgnoreComments(f); f>>bootstrap_level; IgnoreComments(f);
    IgnoreComments(f); f>>max_bootstrap_level; IgnoreComments(f);
    bootstrap_resizeratio.resize(max_bootstrap_level);
    IgnoreComments(f); for(i=0;i<max_bootstrap_level;i++) f>>bootstrap_resizeratio[i]; IgnoreComments(f);
    bootstrap_increment.resize(max_bootstrap_level);
    IgnoreComments(f); for(i=0;i<max_bootstrap_level;i++) f>>bootstrap_increment[i]; IgnoreComments(f);   
    IgnoreComments(f); f>>totalfeatures; IgnoreComments(f);
    //totalfeatures=s.feature.size ();
    IgnoreComments(f); f>>max_files; IgnoreComments(f);
    IgnoreComments(f); f>>goal_method; IgnoreComments(f);
    IgnoreComments(f); f>>node_det_goal; IgnoreComments(f);
    IgnoreComments(f); f>>node_fp_goal; IgnoreComments(f);
    IgnoreComments(f); f>>first_feature; IgnoreComments(f);
    IgnoreComments(f); f>>asym_ratio; IgnoreComments(f);
    IgnoreComments(f); f>>max_nodes; IgnoreComments(f);
    nof.resize(max_nodes);
    IgnoreComments(f); for(i=0;i<max_nodes;i++) f>>nof[i]; 	IgnoreComments(f);
    f.close();


}

void ReadTrainSet(CString filename)
{
    ifstream f;
    int i;
    f.open(filename, ios_base::binary| ios_base::in);
    if(f.is_open()==true)
        cerr << "file opened successfully  " << endl;
    else
        perror("eee");
    f>>totalcount;
    //cerr << "total count of samples " << totalcount <<endl;
    f.ignore(256,'\n');
    ASSERT(totalcount > 0);
    delete[] trainset; trainset=NULL;
    trainset = new IntImage[totalcount]; ASSERT(trainset != NULL);
    for(i=0;i<totalcount;i++) ReadOneTrainingSample(f,trainset[i]);
    for(i=0;i<totalcount;i++) trainset[i].CalculateVarianceAndIntegralImageInPlace();
    f.close();
    facecount = CountTrainFaces();
}

void ReadValidSet(CString filename)
{
    ifstream f;
    int i;
  //  cerr << "opening valid set filename " << filename << endl;
  //  cerr << "opening valid set filename " << filename.capacity() << endl;
    f.open(filename, ios_base::binary | ios_base::in);
    if(f.is_open()==true)
    {
    f>>validcount;
    f.ignore(256,'\n');
    }
    else
        perror("=:w");

    ASSERT(validcount > 0);
    delete[] validset; validset=NULL;
    validset = new IntImage[validcount];
    ASSERT(validset != NULL);
    for(i=0;i<validcount;i++) ReadOneTrainingSample(f,validset[i]);
    for(i=0;i<validcount;i++) validset[i].CalculateVarianceAndIntegralImageInPlace();
    f.close();
    validfacecount = CountValidFaces();
}

void InitializeAuxiliaryVariables()
{
    int i;

    ASSERT(trainset != NULL);
    ASSERT(totalcount > 0);

    delete[] weights; weights = NULL;
    weights = new REAL[max(totalcount,validcount)]; ASSERT(weights != NULL);
    InitializeWeights();

    if(table!=NULL)
    {
        for(i=0;i<totalfeatures;i++)
        {
            delete[] table[i]; table[i] = NULL;
        }
    }
    delete[] table; table = NULL;
    table = new int*[totalfeatures]; ASSERT(table != NULL);
    for(i=0;i<totalfeatures;i++)
    {
        table[i] = new int[totalcount];
        ASSERT(table[i] != NULL);
    }

    delete[] features; features = NULL;
    features = new REAL[max(totalcount,validcount)]; ASSERT(features != NULL);

    delete[] labels; labels = NULL;
    labels = new int[max(totalcount,validcount)]; ASSERT(labels!=NULL);
}

void SaveTrainSet(CString filename)
{
    ofstream f;
    int i;

    f.open(filename,ios_base::out | ios_base::binary);
    f<<totalcount<<endl;
    unsigned char writebuf[24*24];
    for(i=0;i<totalcount;i++)
    {
        int k,t;
        f<<trainset[i].label<<endl;
        f<<sx<<" "<<sy<<endl;
        for(k=0;k<sx;k++)
            for(t=0;t<sy;t++)
                writebuf[k*sx+t] = (unsigned char)((int)(trainset[i].data[k+1][t+1]-trainset[i].data[k][t+1]-trainset[i].data[k+1][t]+trainset[i].data[k][t]));
        f.write((char*)writebuf,sx*sy);
        f<<endl;
    }
    f.close();
}

void SaveValidSet(CString filename)
{
    ofstream f;
    int i;

    f.open(filename,ios_base::out | ios_base::binary);
    f<<validcount<<endl;
    unsigned char writebuf[24*24];
    for(i=0;i<validcount;i++)
    {
        int k,t;
        f<<validset[i].label<<endl;
        f<<sx<<" "<<sy<<endl;
        for(k=0;k<sx;k++)
            for(t=0;t<sy;t++)
                writebuf[k*sx+t] = (unsigned char)((int)(validset[i].data[k+1][t+1]-validset[i].data[k][t+1]-validset[i].data[k+1][t]+validset[i].data[k][t]));
        f.write((char*)writebuf,sx*sy);
        f<<endl;
    }
    f.close();
}

void ReadBootstrapFileNames()
{
    ifstream f;
    int i;

    f.open(Bootstrap_database_filename);
    f>>bootstrap_size; IgnoreComments(f);
    bootstrap_filenames = new CString[bootstrap_size]; ASSERT(bootstrap_filenames!=NULL);
    for(i=0;i<bootstrap_size;i++)
    {
        ReadlnString(f,bootstrap_filenames[i],dir);
    }
    f.close();
}

void InitGlobalData(string option)
{
    option_filename=option;
    int i;

    srand((unsigned)time(NULL));
    LoadOptions();
    cerr << "reading trainset_filename" << trainset_filename << endl;
    ReadTrainSet(trainset_filename);
    ReadValidSet(validset_filename);
    InitializeAuxiliaryVariables();
    ReadBootstrapFileNames();

    cascade = new CascadeClassifier;
    ASSERT(cascade != NULL);
    cerr << "Loading default cascade " << endl;
    cascade->LoadDefaultCascade();
    cerr << "starting node is " << cascade->count +1 << endl;
    starting_node = cascade->count+1;

    cerr << "total features " << totalfeatures << endl;
    classifiers = new SimpleClassifier[totalfeatures]; ASSERT(classifiers != NULL);
    ReadClassifiers();
    cerr << "reading classifier configuration" << endl;
    fileused = new int[max_files]; ASSERT(fileused != NULL);
    for(i=0;i<max_files;i++) fileused[i] = 0;

    ReadRangeFile();
    cerr << "completed iniitalization of global data " <<endl;
    s.feature.clear ();

}

void ClearUpGlobalData()
{
    int i;

    option_filename.Empty();
    trainset_filename.Empty();
    validset_filename.Empty();
    classifier_filename.Empty();
    ada_log_filename.Empty();
    cascade_filename.Empty();
    FFS_WeakClassifiers_filename.Empty();
    FFS_log_filename = "rate_FFS.txt";
    FileUsage_log_filename.Empty();
    Bootstrap_database_filename.Empty();
    Backup_directory_name.Empty();
    TestSet_filename.Empty();
    sx = sy = 0;
    train_method = 0;
    linear_classifier = 0;
    bootstrap_level = 0;
    max_bootstrap_level = 0;
    bootstrap_resizeratio.clear();
    bootstrap_increment.clear();
    first_feature = 0;
    max_files = 0;
    goal_method = 0;
    node_det_goal = 0.0;
    node_fp_goal = 0.0;
    asym_ratio = 0.0;
    max_nodes = 0;
    nof.clear();

    delete[] trainset;	trainset = NULL;
    delete[] validset;  validset = NULL;
    totalcount = validcount = 0;

    delete cascade; cascade = NULL;

    delete[] weights; weights = NULL;
    for(i=0;i<totalfeatures;i++)
    {
        delete[] table[i];	table[i] = NULL;
    }
    totalfeatures = 0;
    delete[] table;	table = NULL;
    delete[] classifiers; classifiers = NULL;

    delete[] features; features = NULL;
    delete[] labels; labels = NULL;

    delete[] fileused; fileused = NULL;

    bootstrap_size = 0;
    delete[] bootstrap_filenames; bootstrap_filenames = NULL;
}

void SkewWeight(const REAL skew_ratio)
{
    int i;

    for(i=0;i<totalcount;i++)
        if(trainset[i].label==1)
            weights[i] *= skew_ratio;

    NormalizeWeight();
}

void NormalizeWeight()
{
    REAL sum;
    int i;

    sum = 0.0;
    for(i=0;i<totalcount;i++) sum += weights[i];
    for(i=0;i<totalcount;i++) weights[i] /= sum;
//	for(i=0;i<totalcount;i++) if(weights[i]<1e-8) weights[i] = 1e-8;
}

void ValidateTheThreshold(AdaBoostClassifier& ada,ofstream& f)
{
    int i,j;
    int falsepos,falseneg;

    for(i=0;i<validfacecount;i++) features[i] = 0.0;
    for(i=0;i<validfacecount;i++)
        for(j=0;j<ada.count;j++)
            if(ada.scs[j].Apply(validset[i])!=0)
                features[i] += ada.alphas[j];
    nth_element(features,features+int(validfacecount*(1-node_det_goal)),features+validfacecount);
    ada.thresh = features[int(validfacecount*(1-node_det_goal))];

    falseneg = facecount;
    for(i=0;i<facecount;i++) falseneg -= ada.ApplyImagePatch(trainset[i]);
    falsepos = 0;
    for(i=facecount;i<totalcount;i++) falsepos += ada.ApplyImagePatch(trainset[i]);
    f<<"-----Use the validation set to determine a threshold ------"<<endl;
    f<<ada.thresh<<"\t"<<falseneg<<"\t"<<falsepos<<endl;
}

// Matrix inversion code, revised from Numerical recipe in C, and the graphviz Documentation
int lu_decompose(REAL** a,int n,REAL** lu,int* ps)
{
    int i,j,k;
    int pivotindex=0;
    REAL pivot,biggest,mult,tempf;
    REAL* scales;

    scales = new REAL[n]; ASSERT(scales!=NULL);
    for(i=0;i<n;i++) {
        biggest=0.0;
        for(j=0;j<n;j++)
            if(biggest<(tempf=fabs(lu[i][j]=a[j][i])))
                biggest=tempf;
        if(biggest!=0.0)
            scales[i] = REAL(1.0)/biggest;
        else {
            scales[i]=0.0;
            delete[] scales; scales=NULL;
            return 0; //zero row: singular matrix
        }
        ps[i]=i;
    }

    for(k=0;k<n-1;k++) {
        biggest=0.0;
        for(i=k;i<n;i++)
            if(biggest<(tempf=fabs(lu[ps[i]][k])*scales[ps[i]])) {
                biggest=tempf;
                pivotindex=i;
            }
        if(biggest==0.0)
        {
            delete[] scales; scales=NULL;
            return 0; //zero row: singular matrix
        }
        if(pivotindex!=k) {
            j=ps[k];
            ps[k] = ps[pivotindex];
            ps[pivotindex]=j;
        }

        pivot=lu[ps[k]][k];
        for(i=k+1;i<n;i++) {
            lu[ps[i]][k]=mult=lu[ps[i]][k]/pivot;
            if(mult!=0.0) {
                for(j=k+1;j<n;j++)
                    lu[ps[i]][j] -= mult*lu[ps[k]][j];
            }
        }
    }
    if(lu[ps[n-1]][n-1]==0.0)
    {
        delete[] scales; scales=NULL;
        return 0; // signular matrix
    }

    delete[] scales; scales=NULL;
    return 1;
}

void lu_solve(REAL* x,REAL* b,int n,REAL** lu,int* ps)
{
    int i,j;
    REAL dot;

    for(i=0;i<n;i++) {
        dot=0.0;
        for(j=0;j<i;j++)
            dot+=lu[ps[i]][j]*x[j];
        x[i]=b[ps[i]]-dot;
    }

    for(i=n-1;i>=0;i--) {
        dot=0.0;
        for(j=i+1;j<n;j++)
            dot+=lu[ps[i]][j]*x[j];
        x[i]=(x[i]-dot)/lu[ps[i]][i];
    }
}

void MatrixInversion(REAL** a,int n)
{
    REAL** lu;
    int i,j;
    REAL* col;
    int* ps;

    lu = new REAL*[n]; ASSERT(lu!=NULL);
    for(i=0;i<n;i++)
    {
        lu[i] = new REAL[n];
        ASSERT(lu[i]!=NULL);
    }
    ps = new int[n]; ASSERT(ps!=NULL);
    if(lu_decompose(a,n,lu,ps)==0)
        cerr << "Singular Matrix 3!" << endl;
    col = new REAL[n]; ASSERT(col!=NULL);
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++) col[j]=0.0;
        col[i]=1.0;
        lu_solve(a[i],col,n,lu,ps);
    }
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
        {
            REAL temp;
            temp=a[i][j];
            a[i][j]=a[j][i];
            a[j][i]=temp;
        }
    for(i=0;i<n;i++)
    {
        delete[] lu[i];
        lu[i]=NULL;
    }
    delete[] lu; lu=NULL;
    delete[] ps; ps=NULL;
    delete[] col; col=NULL;
}
// Matrix inversion code finished.

void WithinClassScatter(AdaBoostClassifier& ada)
{
    int i,j,k;
    REAL* mean1;
    REAL* mean2;
    REAL** cov;
    REAL** table;

    table = new REAL*[ada.count]; ASSERT(table!=NULL);
    for(i=0;i<ada.count;i++)
    {
        table[i] = new REAL[totalcount]; ASSERT(table[i] != NULL);
    }
    for(i=0;i<ada.count;i++)
        for(j=0;j<totalcount;j++)
            table[i][j] = REAL(ada.scs[i].Apply(trainset[j]));

    cov = new REAL*[ada.count]; ASSERT(cov!=NULL);
    for(i=0;i<ada.count;i++)
    {
        cov[i] = new REAL[ada.count]; ASSERT(cov[i]!=NULL);
    }
    for(i=0;i<ada.count;i++)
        for(j=0;j<ada.count;j++)
            cov[i][j]=0.0;

    mean1 = new REAL[ada.count]; ASSERT(mean1!=NULL);
    for(i=0;i<ada.count;i++) mean1[i] = 0;
    for(i=0;i<ada.count;i++)
        for(j=0;j<facecount;j++)
            mean1[i] += table[i][j];
    for(i=0;i<ada.count;i++) mean1[i] /= facecount;
    for(i=0;i<facecount;i++)
        for(j=0;j<ada.count;j++)
            for(k=0;k<ada.count;k++)
                cov[j][k] += (table[j][i]-mean1[j])*(table[k][i]-mean1[k]);

    mean2 = new REAL[ada.count]; ASSERT(mean2!=NULL);
    for(i=0;i<ada.count;i++) mean2[i] = 0;
    for(i=0;i<ada.count;i++)
        for(j=facecount;j<totalcount;j++)
            mean2[i] += table[i][j];
    for(i=0;i<ada.count;i++) mean2[i] /= (totalcount-facecount);

    if(linear_classifier==LC_FDA)
    {
        for(i=facecount;i<totalcount;i++)
            for(j=0;j<ada.count;j++)
                for(k=0;k<ada.count;k++)
                    cov[j][k] += (table[j][i]-mean2[j])*(table[k][i]-mean2[k]);
    }
    for(i=0;i<ada.count;i++) cov[i][i] += REAL(0.1);

    MatrixInversion(cov,ada.count);

    for(i=0;i<ada.count;i++) ada.alphas[i]=0.0;
    for(i=0;i<ada.count;i++)
        for(j=0;j<ada.count;j++)
            ada.alphas[i] += cov[i][j]*(mean1[j]-mean2[j]);

    delete[] mean1; mean1=NULL;
    delete[] mean2; mean2=NULL;
    for(i=0;i<ada.count;i++)
    {
        delete[] cov[i]; cov[i]=NULL;
    }
    delete[] cov; cov=NULL;
    for(i=0;i<ada.count;i++)
    {
        delete[] table[i]; table[i]=NULL;
    }
    delete[] table; table=NULL;
}
