#ifndef FaceDetectDoc
#define FaceDetectDoc

#include "IntImage.h"


class CFaceDetectDoc
{
public :

    CFaceDetectDoc()
    {
        // TODO: add one-time construction code here

    }

    ~CFaceDetectDoc()
    {

    }


public:
    //IntImage image;

public:


    public:
    virtual bool OnNewDocument();

    bool OnOpenDocument(char * lpszPathName);

public:

     void OnTestAll();


     void OnTrain();
};


#endif
