//	enum TRAIN_METHODS {TRAIN_ADA = 1, TRAIN_FFS= 2};
//	enum LINEAR_CLASSIFIER {LC_ORIGINAL = 1, LC_LAC = 2, LC_FDA = 3};
#include "cstring1.h"
#include "common.h"
#include "SimpleClassifier.h"
#include "AdaBoostClassifier.h"

struct CascadeClassifier;

extern CStdString 	option_filename;
extern string dir;
extern int total_fp;
#define REAL double
extern CStdString trainset_filename;
extern CStdString validset_filename;
extern CStdString classifier_filename;
extern CStdString ada_log_filename;
extern CStdString cascade_filename;
extern CStdString FFS_WeakClassifiers_filename;
extern CStdString FFS_log_filename;
extern CStdString FileUsage_log_filename;
extern CStdString Bootstrap_database_filename;
extern CStdString Backup_directory_name;
extern CStdString TestSet_filename;
extern int sx;
extern int sy;
extern int train_method;
extern int linear_classifier;
extern int bootstrap_level;
extern int max_bootstrap_level;
extern vector<REAL> bootstrap_resizeratio;
extern vector<int> bootstrap_increment;
extern int totalfeatures;
extern int max_files;
extern int goal_method;
extern REAL node_det_goal;
extern REAL node_fp_goal;
extern int first_feature;
extern REAL asym_ratio;
extern int max_nodes;
extern vector<int> nof;
extern int starting_node;
extern int facecount, validfacecount;

extern IntImage* trainset;
extern IntImage* validset;
extern int totalcount;
extern int validcount;	

extern CascadeClassifier* cascade;
extern REAL* weights;
extern int** table;
extern SimpleClassifier* classifiers;

extern REAL* features;
extern int* labels;

extern int* fileused;
extern int bootstrap_size;
extern CStdString* bootstrap_filenames;

extern REAL mean_min,mean_max,sq_min,sq_max,var_min,var_max;

void ReadOneTrainingSample(ifstream& is,IntImage& image);
void GetFeatureValues0(REAL* const features,const int from,const int to,const vector<Rectangles> &features1);
void GetFeatureValues0(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int y1,const int y3);
void GetFeatureValues1(REAL* const features,const int from,const int to,const int x1,const int x3,const int y1,const int y2,const int y3);
void GetFeatureValues2(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int x4,const int y1,const int y3);
void GetFeatureValues3(REAL* const features,const int from,const int to,const int x1,const int x3,const int y1,const int y2,const int y3,const int y4);
void GetFeatureValues4(REAL* const features,const int from,const int to,const int x1,const int x2,const int x3,const int y1,const int y2,const int y3);
void QuickSort(REAL* values,int* labels,const int lo,const int hi);
void QuickSort(REAL* values,int* labels,REAL* weights,int* parity,const int lo,const int hi);
void FillTheTable(int* const row,const SimpleClassifier& sc);
void ClearUpGlobalData(void);
const int CountTrainFaces(void);
const int CountValidFaces(void);
void ReadTrainSet(QString filename);
void ReadValidSet(QString filename);
void SaveTrainSet(QString filename);
void SaveValidSet(QString filename);
void InitializeAuxiliaryVariables();
void InitializeWeights(void);
void ReadClassifiers(void);
void InitGlobalData(string option);
void ReadlnString(ifstream& f, CString& s,string s1);
void NormalizeWeight(void);
void SkewWeight(const REAL skew_ratio);
void ValidateTheThreshold(AdaBoostClassifier& ada,ofstream& f);

void MatrixInversion(REAL** a,int n);
void WithinClassScatter(AdaBoostClassifier& ada);

void ReadRangeFile(void);
void WriteRangeFile(void);


#ifndef GLOBALL
#define GLOBALL

class HaarFeaturesSet
{

public:
    vector<HaarFeatures> feature;
    HaarFeaturesSet()
    {
        winSize=cv::Size(24,24);
    }

public:
    cv::Size winSize;
    void generateFeatures()
    {
        int offset = winSize.width + 1;

        for( int dx = 1; dx <= winSize.width; dx++ )
        {
            for( int dy = 1; dy <= winSize.height; dy++ )
            {

                for( int y = 0; y < winSize.height; y++ )
                {
                for( int x = 0; x < winSize.width; x++ )
                {

                            HaarFeatures f1,f2,f3,f4,f5;
                            Rectangles r1;//(x,y,2*dx,dy,-1);
                            Rectangles r2;//(x+dx,y,dx,2*dy,+2);
                            Rectangles r3(x+dx,y+dy,dx,dy,2);


                            r1.init (x,y,2*dx,dy,-1);
                            r2.init (x+dx,y,dx,dy,2);

                            if(r1._r.x+r1._r.width <= winSize.width && r1._r.y+r1._r.height <= winSize.height &&r2._r.x+r2._r.width <= winSize.width && r2._r.y+r2._r.height <= winSize.height )
                            {
                            f1.features.push_back (r1);
                            f1.features.push_back (r2);
                            f1.type=0;
                            feature.push_back (f1);
                            }


                            r1.init (x,y,1*dx,2*dy,-1);
                            r2.init (x,y+dy,dx,dy,2);
                            if(r1._r.x+r1._r.width <= winSize.width && r1._r.y+r1._r.height <= winSize.height &&r2._r.x+r2._r.width <= winSize.width && r2._r.y+r2._r.height <= winSize.height )
                            {
                            f2.features.push_back (r1);
                            f2.features.push_back (r2);
                            f2.type=1;
                            feature.push_back (f2);
                            }


                            r1.init (x,y,3*dx,dy,-1);
                            r2.init (x+dx,y,dx,dy,3);

                            if(r1._r.x+r1._r.width <= winSize.width && r1._r.y+r1._r.height <= winSize.height &&r2._r.x+r2._r.width <= winSize.width && r2._r.y+r2._r.height <= winSize.height )
                            {
                            f3.features.push_back (r1);
                            f3.features.push_back (r2);
                            f3.type=1;
                            feature.push_back (f3);
                            }

                            r1.init (x,y,dx,3*dy,-1);
                            r2.init (x,y+dy,dx,dy,3);
                            if(r1._r.x+r1._r.width <= winSize.width && r1._r.y+r1._r.height <= winSize.height &&r2._r.x+r2._r.width <= winSize.width && r2._r.y+r2._r.height <= winSize.height )
                            {
                            f4.features.push_back (r1);
                            f4.features.push_back (r2);
                            f4.type=1;
                            feature.push_back (f4);
                            }



                            r1.init(x,y,2*dx,2*dy,-1);
                            r2.init(x,y,dx,dy,2);


                            if(r1._r.x+r1._r.width <= winSize.width && r1._r.y+r1._r.height <= winSize.height &&r2._r.x+r2._r.width <= winSize.width && r2._r.y+r2._r.height <= winSize.height )
                            {
                            f5.features.push_back (r1);
                            f5.features.push_back (r2);
                            f5.features.push_back (r3);
                            feature.push_back (f5);
                            }

                    }

                }
                //if(dy==3)
                  //  break;
            }
        }


       // cerr << feature.size () << endl;

    }

};

extern HaarFeaturesSet s;
#endif
