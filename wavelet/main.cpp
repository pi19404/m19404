#include "mainwindow.h"
#include <QApplication>

#include "odwt.h"


/* Demo for 1D wavelet decomposition and reconstruction using haar wavelets
 *
 */

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //wavelet analysis
    vector<float> a;
    a.push_back (1);a.push_back (2);a.push_back (3);a.push_back (4);


    Mat A=Mat(7,7,CV_32FC(1));

    for(int i=0;i<A.rows;i++)
    {
    float * ptr=A.ptr<float> (i);
    for(int j=0;j<A.cols;j++)
    {
        ptr[j]=j+1;
        //ptr[i]=aptr[i]+bptr[i];
        //cerr << ptr[j] <<":" ;
        //cerr << aptr[i] <<":" ;
    }
    cerr << endl;
    }

    vector<float> b;
    b.push_back (1/sqrt(2));b.push_back (1/sqrt(2));
    Mat B=Mat(1,b.size (),CV_32FC(1));

    float *ptr=B.ptr<float>(0);
    for(int i=0;i<b.size ();i++)
    ptr[i]=b[i];




    //time reversed kernel
    vector<float> c;
    c.push_back (1/sqrt(2));c.push_back (-1/sqrt(2));
    Mat C=Mat(1,c.size (),CV_32FC(1));
    ptr=C.ptr<float>(0);
    for(int i=0;i<c.size ();i++)
    ptr[i]=c[i];


    vector<vector<Mat> > output;
    int N=2;
    mldwt2D(A,B,C,N,output);

    //dwt2D(A,B,C,output);

    for(int l=0;l<N;l++)
    {
    for(int k=0;k<4;k++)
    {
    for(int i=0;i<output[l][k].rows;i++)
    {
    float * ptr=output[l][k].ptr<float> (i);
    for(int j=0;j<output[l][k].cols;j++)
    {
        cerr << ptr[j] <<":" ;
    }
    cerr << endl;
    }
    cerr << "**********************" <<endl;
    }
    cerr << "~~~~~~~~~~~~~" <<endl;
    }


    cerr << "RECONSTRUCTION **********************" <<endl;

    vector<float> d;
    d.push_back (1/sqrt(2));
    d.push_back (1/sqrt(2));
    Mat D=Mat(1,d.size (),CV_32FC(1));
    ptr=D.ptr<float>(0);
    for(int i=0;i<c.size ();i++)
    ptr[i]=d[i];

    vector<float> e;
    e.push_back (-1/sqrt(2));
    e.push_back (1/sqrt(2));
    Mat E=Mat(1,e.size (),CV_32FC(1));
    ptr=E.ptr<float>(0);
    for(int i=0;i<e.size ();i++)
    {
    ptr[i]=e[i];
    }


    Mat x;
    mldwt2D(output,D,E,x,Size(A.cols,A.rows));
    //idwt2D(output,D,E,x,Size(0,0));

    for(int i=0;i<x.rows;i++)
    {
        float *ptr=x.ptr<float>(i);
        for(int j=0;j<x.cols;j++)
        {
            cerr << ptr[j] << ":" ;
        }
        cerr <<endl;
    }

    cerr <<"**********************" <<endl;


    /*
    Mat A=Mat(1,20,CV_32FC(1));
    float *ptr=A.ptr<float>(0);
    for(int i=0;i<20;i++)
    ptr[i]=i+1;

    //length of signal
    int l=20;

    //time reversed kernel
    vector<float> b;
    b.push_back (1/sqrt(2));b.push_back (1/sqrt(2));
    Mat B=Mat(1,b.size (),CV_32FC(1));

    ptr=B.ptr<float>(0);
    for(int i=0;i<b.size ();i++)
    ptr[i]=b[i];




    //time reversed kernel
    vector<float> c;
    c.push_back (1/sqrt(2));c.push_back (-1/sqrt(2));
    Mat C=Mat(1,c.size (),CV_32FC(1));
    ptr=C.ptr<float>(0);
    for(int i=0;i<c.size ();i++)
    ptr[i]=c[i];


    vector<vector<float> >app,det;
    //calling wavelet decomposition
    mldwt1D (A,B,C,N,app,det);



    for(int i=0;i<app.size ();i++)
    {
        vector<float> tmp=app[i];
        for(int i=0;i<tmp.size ();i++)
        std::cerr << tmp[i] <<":" ;
         std::cerr <<endl<<"----------------"<< endl;
    }

    std::cerr <<endl<<"************"<< endl;
    for(int i=0;i<det.size ();i++)
    {
        vector<float> tmp=det[i];
        for(int i=0;i<tmp.size ();i++)
        std::cerr << tmp[i] <<":" ;
         std::cerr <<endl<<"----------------"<< endl;
    }


    //reconstruction filters
    vector<float> d;
    d.push_back (1/sqrt(2));
    d.push_back (1/sqrt(2));
    Mat D=Mat(1,d.size (),CV_32FC(1));
    ptr=D.ptr<float>(0);
    for(int i=0;i<c.size ();i++)
    ptr[i]=d[i];

    vector<float> e;
    e.push_back (-1/sqrt(2));
    e.push_back (1/sqrt(2));
    Mat E=Mat(1,e.size (),CV_32FC(1));
    ptr=E.ptr<float>(0);
    for(int i=0;i<e.size ();i++)
    {
    ptr[i]=e[i];

    }


    Mat x;
    //calling wavlet reconstrution filters
    mlidwt1D (app[app.size ()-1],det,D,E,x,l);


    ptr=x.ptr <float>();
    for(int i=0;i<x.cols;i++)
    {
        std::cerr << ptr[i] <<":" ;
    }
    std::cerr << endl;

*/


    return 0;
}
