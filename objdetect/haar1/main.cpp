#include <cv.h>
#include <highgui.h>
#include <QtCore/QDir>
#include <math.h>

#include <iostream>
#include <opencv2/core/core.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "time_measure.hpp"
#include "ml/ml.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/opencv_modules.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <mlpack/core.hpp>
#include <stdio.h>
#include "opencv2/core/internal.hpp"
#define ICV_HAAR_STAGES_NAME          "stages"

#define ICV_HAAR_SIZE_NAME            "size"
#define ICV_HAAR_STAGES_NAME          "stages"
#define ICV_HAAR_TREES_NAME           "trees"
#define ICV_HAAR_FEATURE_NAME         "feature"
#define ICV_HAAR_RECTS_NAME           "rects"
#define ICV_HAAR_TILTED_NAME          "tilted"
#define ICV_HAAR_THRESHOLD_NAME       "threshold"
#define ICV_HAAR_LEFT_NODE_NAME       "left_node"
#define ICV_HAAR_LEFT_VAL_NAME        "left_val"
#define ICV_HAAR_RIGHT_NODE_NAME      "right_node"
#define ICV_HAAR_RIGHT_VAL_NAME       "right_val"
#define ICV_HAAR_STAGE_THRESHOLD_NAME "stage_threshold"
#define ICV_HAAR_PARENT_NAME          "parent"
#define ICV_HAAR_NEXT_NAME            "next"

#define CC_CASCADE_PARAMS "cascadeParams"
#define CC_STAGE_TYPE     "stageType"
#define CC_FEATURE_TYPE   "featureType"
#define CC_HEIGHT         "height"
#define CC_WIDTH          "width"

#define CC_STAGE_NUM    "stageNum"
#define CC_STAGES       "stages"
#define CC_STAGE_PARAMS "stageParams"

#define CC_BOOST            "BOOST"
#define CC_MAX_DEPTH        "maxDepth"
#define CC_WEAK_COUNT       "maxWeakCount"
#define CC_STAGE_THRESHOLD  "stageThreshold"
#define CC_WEAK_CLASSIFIERS "weakClassifiers"
#define CC_INTERNAL_NODES   "internalNodes"
#define CC_LEAF_VALUES      "leafValues"

#define CC_FEATURES       "features"
#define CC_FEATURE_PARAMS "featureParams"
#define CC_MAX_CAT_COUNT  "maxCatCount"

#define CC_HAAR   "HAAR"
#define CC_RECTS  "rects"
#define CC_TILTED "tilted"

#define CC_LBP  "LBP"
#define CC_RECT "rect"



using namespace mlpack;
using namespace std;
using namespace cv;













class Feat
{
public:
    vector<Rect> haarx;
};


static Scalar randomColor(RNG& rng)
{
    int icolor = (unsigned)rng;
    return Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}


int main(int argc, char *argv[])
{


    RNG rng(0xFFFFFFFF);


     CvFileStorage* fs1 = cvOpenFileStorage( "/tmp/a.xml", 0, CV_STORAGE_READ );


        char buf[256];
       CvFileNode* seq_fn = NULL; /* sequence */
       CvFileNode* fn = NULL;
       CvFileNode* stages_fn = NULL;
       CvSeqReader stages_reader;
       int n;
       int i, j, k, l;
       int parent, next;

       CvFileNode *node=cvGetRootFileNode(fs1);
       char *x;
       //cerr <<"root node name " << x <<":" << endl <<endl;
       //CvStringHashNode* n1=cvGetHashedKey(fs1, "opencv_storage",-1,-1);
       node = cvGetFileNodeByName( fs1, 0, argv[1]);

       stages_fn = cvGetFileNodeByName( fs1, node, ICV_HAAR_STAGES_NAME );
       if( !stages_fn || !CV_NODE_IS_SEQ( stages_fn->tag) )
           CV_Error( CV_StsError, "Invalid stages node" );

       n = stages_fn->data.seq->total;
       Size orig_window_size=Size(0,0);
       //cascade = icvCreateHaarClassifierCascade(n);
       Log::Warn << "No of Stages " << n << endl;

       seq_fn = cvGetFileNodeByName( fs1, node, ICV_HAAR_SIZE_NAME );
       if( !seq_fn || !CV_NODE_IS_SEQ( seq_fn->tag ) || seq_fn->data.seq->total != 2 )
           CV_Error( CV_StsError, "size node is not a valid sequence." );
       fn = (CvFileNode*) cvGetSeqElem( seq_fn->data.seq, 0 );
       if( !CV_NODE_IS_INT( fn->tag ) || fn->data.i <= 0 )
           CV_Error( CV_StsError, "Invalid size node: width must be positive integer" );
       orig_window_size.width = fn->data.i;
       fn = (CvFileNode*) cvGetSeqElem( seq_fn->data.seq, 1 );
       if( !CV_NODE_IS_INT( fn->tag ) || fn->data.i <= 0 )
           CV_Error( CV_StsError, "Invalid size node: height must be positive integer" );
       orig_window_size.height = fn->data.i;


       Log::Warn << "Original window Size : " << orig_window_size.width << "X" << orig_window_size.height <<endl;


       vector <vector <Feat> > cascadex;
       cascadex.resize(n);
       cvStartReadSeq( stages_fn->data.seq, &stages_reader );
       for( i = 0; i < n; ++i )
       {

           vector<Feat>  strong=cascadex[i];
           CvFileNode* stage_fn;
           CvFileNode* trees_fn;
           CvSeqReader trees_reader;

           stage_fn = (CvFileNode*) stages_reader.ptr;
           if( !CV_NODE_IS_MAP( stage_fn->tag ) )
           {
               sprintf( buf, "Invalid stage %d", i );
               CV_Error( CV_StsError, buf );
           }

           trees_fn = cvGetFileNodeByName( fs1, stage_fn, ICV_HAAR_TREES_NAME );
           if( !trees_fn || !CV_NODE_IS_SEQ( trees_fn->tag )
               || trees_fn->data.seq->total <= 0 )
           {
               sprintf( buf, "Trees node is not a valid sequence. (stage %d)", i );
               CV_Error( CV_StsError, buf );
           }


            strong.resize(trees_fn->data.seq->total);
            Log::Warn << "Stage :" << i << "---" << "Count " << trees_fn->data.seq->total <<endl;

            cvStartReadSeq( trees_fn->data.seq, &trees_reader );
            for( j = 0; j < trees_fn->data.seq->total; ++j )
            {

                Feat rx=strong[j];

                CvFileNode* tree_fn;
                CvSeqReader tree_reader;
                int last_idx;
                tree_fn = (CvFileNode*) trees_reader.ptr;
                if( !CV_NODE_IS_SEQ( tree_fn->tag ) || tree_fn->data.seq->total <= 0 )
                {
                    sprintf( buf, "Tree node is not a valid sequence."
                             " (stage %d, tree %d)", i, j );
                    CV_Error( CV_StsError, buf );
                }

                cvStartReadSeq( tree_fn->data.seq, &tree_reader );
                for( k = 0, last_idx = 0; k < tree_fn->data.seq->total; ++k )
                {
                    CvFileNode* node_fn;
                    CvFileNode* feature_fn;
                    CvFileNode* rects_fn;
                    CvSeqReader rects_reader;

                    node_fn = (CvFileNode*) tree_reader.ptr;
                    if( !CV_NODE_IS_MAP( node_fn->tag ) )
                    {
                        sprintf( buf, "Tree node %d is not a valid map. (stage %d, tree %d)",
                                 k, i, j );
                        CV_Error( CV_StsError, buf );
                    }
                    feature_fn = cvGetFileNodeByName( fs1, node_fn, ICV_HAAR_FEATURE_NAME );
                    if( !feature_fn || !CV_NODE_IS_MAP( feature_fn->tag ) )
                    {
                        sprintf( buf, "Feature node is not a valid map. "
                                 "(stage %d, tree %d, node %d)", i, j, k );
                        CV_Error( CV_StsError, buf );
                    }
                    rects_fn = cvGetFileNodeByName( fs1, feature_fn, ICV_HAAR_RECTS_NAME );
                    if( !rects_fn || !CV_NODE_IS_SEQ( rects_fn->tag )
                        || rects_fn->data.seq->total < 1
                        || rects_fn->data.seq->total > CV_HAAR_FEATURE_MAX )
                    {
                        sprintf( buf, "Rects node is not a valid sequence. "
                                 "(stage %d, tree %d, node %d)", i, j, k );
                        CV_Error( CV_StsError, buf );
                    }


                    cvStartReadSeq( rects_fn->data.seq, &rects_reader );
                    for( l = 0; l < rects_fn->data.seq->total; ++l )
                    {
                        CvFileNode* rect_fn;
                        CvRect r;
                        rect_fn = (CvFileNode*) rects_reader.ptr;
                        if( !CV_NODE_IS_SEQ( rect_fn->tag ) || rect_fn->data.seq->total != 5 )
                        {
                            sprintf( buf, "Rect %d is not a valid sequence. "
                                     "(stage %d, tree %d, node %d)", l, i, j, k );
                            CV_Error( CV_StsError, buf );
                        }

                        fn = CV_SEQ_ELEM( rect_fn->data.seq, CvFileNode, 0 );
                        if( !CV_NODE_IS_INT( fn->tag ) || fn->data.i < 0 )
                        {
                            sprintf( buf, "x coordinate must be non-negative integer. "
                                     "(stage %d, tree %d, node %d, rect %d)", i, j, k, l );
                            CV_Error( CV_StsError, buf );
                        }
                        r.x = fn->data.i;
                        fn = CV_SEQ_ELEM( rect_fn->data.seq, CvFileNode, 1 );
                        if( !CV_NODE_IS_INT( fn->tag ) || fn->data.i < 0 )
                        {
                            sprintf( buf, "y coordinate must be non-negative integer. "
                                     "(stage %d, tree %d, node %d, rect %d)", i, j, k, l );
                            CV_Error( CV_StsError, buf );
                        }
                        r.y = fn->data.i;
                        fn = CV_SEQ_ELEM( rect_fn->data.seq, CvFileNode, 2 );
                        if( !CV_NODE_IS_INT( fn->tag ) || fn->data.i <= 0
                            || r.x + fn->data.i > orig_window_size.width )
                        {
                            sprintf( buf, "width must be positive integer and "
                                     "(x + width) must not exceed window width. "
                                     "(stage %d, tree %d, node %d, rect %d)", i, j, k, l );
                            CV_Error( CV_StsError, buf );
                        }
                        r.width = fn->data.i;
                        fn = CV_SEQ_ELEM( rect_fn->data.seq, CvFileNode, 3 );
                        if( !CV_NODE_IS_INT( fn->tag ) || fn->data.i <= 0
                            || r.y + fn->data.i > orig_window_size.height )
                        {
                            sprintf( buf, "height must be positive integer and "
                                     "(y + height) must not exceed window height. "
                                     "(stage %d, tree %d, node %d, rect %d)", i, j, k, l );
                            CV_Error( CV_StsError, buf );
                        }
                        r.height = fn->data.i;
                        fn = CV_SEQ_ELEM( rect_fn->data.seq, CvFileNode, 4 );
                        if( !CV_NODE_IS_REAL( fn->tag ) )
                        {
                            sprintf( buf, "weight must be real number. "
                                     "(stage %d, tree %d, node %d, rect %d)", i, j, k, l );
                            CV_Error( CV_StsError, buf );
                        }

                        Rect r1=Rect(r.x,r.y,r.width,r.height);
                        rx.haarx.push_back(r1);

                        CV_NEXT_SEQ_ELEM( sizeof( *rect_fn ), rects_reader );

                    }




                    CV_NEXT_SEQ_ELEM( sizeof( *node_fn ), tree_reader );

                }


                strong[j]=rx;
             //   cerr << rects_fn->data.seq->total << endl;

                CV_NEXT_SEQ_ELEM( sizeof( *tree_fn ), trees_reader );

            }


            CV_NEXT_SEQ_ELEM( sizeof( *stage_fn ), stages_reader );
            cascadex[i]=strong;
       }




       Mat image=Mat(orig_window_size.height,orig_window_size.width,CV_8UC(3));

       image.setTo(cv::Scalar::all(0));
       for(int i=0;i<cascadex.size();i++)
       {
           vector<Feat>  strong=cascadex[i];


           for(int j=0;j<strong.size();j++)
           {
               Scalar color=randomColor(rng);
               vector<Rect> r=strong[j].haarx;
               //cerr << r.size() << endl;
                int x1=100,x2=100,x3=0,x4=0;
               for(int k=0;k<r.size();k++)
               {
                   if(r[k].x<x1)
                       x1=r[k].x;

                   if(r[k].y< x2)
                       x2=r[k].y;

                   if(r[k].x+r[k].width>x3)
                       x3=r[k].x+r[k].width;

                   if(r[k].y+r[k].height>x4)
                       x4=r[k].y+r[k].height;



                   //Log::Warn << r[k].x <<":" << r[k].y <<":" <<r[k].width <<":" << r[k].height << endl;

               }
             //  cerr << x1 <<":" << x2 <<":" << x3-x1 <<":" << x4-x2  << endl;
               cv::rectangle(image,Rect(x1,x2,x3-x1,x4-x2),color,1,8);
           }
           Mat disp;
           cv::resize(image,disp,Size(256,256));
           imshow("TTT",disp);
           //cv::waitKey(0);

           char fname[100];
           sprintf(fname,"/data1/%d.png",i);
           imwrite(fname,image);
           image.setTo(cv::Scalar::all(0));
       }
