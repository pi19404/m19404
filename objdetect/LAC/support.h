#ifndef SUPPORT
#define SUPPORT
#include "common.h"
#include "mainwindow.h"
#include <QFileInfo>
#include <cstring1.h>

static bool copyFile(const CStdString source1, CStdString destination1, bool overwrite, bool move)
{
    QString source(source1);
     QString destination(destination1);
    QFileInfo sourceInfo(source), destinationInfo(destination);

    if(source.compare(destination) == 0)
            return true;

        //load both files
        QFile oldFile(source);
        QFile newFile(destination);
        bool openOld = oldFile.open( QIODevice::ReadOnly );
        bool openNew = newFile.open( QIODevice::WriteOnly );

        //if either file fails to open bail
        if(!openOld || !openNew) { return false; }

        //copy contents
        uint BUFFER_SIZE = 16000;
        char* buffer = new char[BUFFER_SIZE];
        while(!oldFile.atEnd())
        {
            int len = oldFile.read( buffer, BUFFER_SIZE );
            newFile.write( buffer, len );
        }

        //deallocate buffer
        delete[] buffer;
        buffer = NULL;
        return true;
}
#endif
