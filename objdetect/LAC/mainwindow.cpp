#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"
#include "Learn.h"
#include "Global.h"
#include "IntImage.h"
#include "CascadeClassifier.h"
#include "common.h"
#include <QtCore/QDir>
#include <stdio.h>
#include <QtGui/QWidget>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMenu>
#include <QtGui/QMenu>
#include <QtGui/QFileDialog>
#include <QtGui/QAction>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QVBoxLayout>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QInputDialog>
#include <QtGui/QPen>
#include <QtConcurrentRun>
#include <cv.h>
#include <highgui.h>
#include <QtCore/QDir>
#include <Settings.h>


MainWindow::MainWindow(QWidget *parent,int argc,char **argv) :QMainWindow(parent),ui(new Ui::MainWindow)
{

    exitWait=false;
    continueCalc=true;

    ui->setupUi(this);
    ui->imageView_source->setGraphicsViewMode(true);
    ui->imageView_source->setTextLabel(tr("Press \"space\" to start camera..."));
    connect(ui->actionStart_camera, SIGNAL(triggered()), this, SLOT(startProcessing()));
    connect(ui->actionStop_camera, SIGNAL(triggered()), this, SLOT(stopProcessing()));
 Settings::loadSettings(Settings::iniDefaultPath(),0);
   // if(!camera_)
    {


        camera_ = new Camera(this);
    }

 train_exit=false;
 dd=argv[1];
       InitGlobalData(argv[1]);


       ProgressDialog=new QProgressDialog("Processing","Cancel", 0,0, this);
       ProgressDialog->setWindowModality(Qt::WindowModal);
       ProgressDialog->setValue(0);

        connect(&this->FutureWatcher, SIGNAL(finished()), this, SLOT(slot_finished()));

       cascade->LoadDefaultCascade();
       sstm << "completed default cascade " << endl;
       //WriteSimpleClassifiers();
       logMessage(sstm.str ());sstm.flush ();

       this->setCursor(Qt::ArrowCursor);
    //init haar features




}


void MainWindow::on_actionCreate_Haar_Classifier_txt_file_triggered()
{
    WriteSimpleClassifiers(dd);
}


void MainWindow::slot_finished()
{
  this->ProgressDialog->hide();
}

void MainWindow::on_pushButtonx_clicked(QString file,QString dir)
{


  ProgressDialog->show ();

  QFuture<void> future = QtConcurrent::run(this,&MainWindow::train_data,file,dir);
   this->FutureWatcher.setFuture(future);
  this->ProgressDialog->setMinimum(0);
    this->ProgressDialog->setMaximum(0);
    this->ProgressDialog->setWindowModality(Qt::WindowModal);
    this->ProgressDialog->exec();

   //future.waitForFinished();

  //ProgressDialog->close();

}

/*
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    exitWait=false;
    continueCalc=true;

    ui->setupUi(this);
    ui->imageView_source->setGraphicsViewMode(true);
    ui->imageView_source->setTextLabel(tr("Press \"space\" to start camera..."));
    connect(ui->actionStart_camera, SIGNAL(triggered()), this, SLOT(startProcessing()));
    connect(ui->actionStop_camera, SIGNAL(triggered()), this, SLOT(stopProcessing()));
 Settings::loadSettings(Settings::iniDefaultPath(),0);
   // if(!camera_)
    {


        camera_ = new Camera(this);
    }

       InitGlobalData();


       cascade->LoadDefaultCascade();
       sstm << "completed default cascade " << endl;
       logMessage(sstm.str ());sstm.flush ();
}
*/
void MainWindow::startProcessing()
{

    if(camera_->start())
    {
        connect(camera_, SIGNAL(imageReceived(const cv::Mat &)), this, SLOT(update(const cv::Mat &)));

        //ui->label_timeRefreshRate->setVisible(true);

    }
     sstm << "startProcessing " << endl;
     logMessage(sstm.str ());sstm.flush ();

}

void MainWindow::stopProcessing()
{
    if(camera_)
    {
        disconnect(camera_, SIGNAL(imageReceived(const cv::Mat &)), this, SLOT(update(const cv::Mat &)));
        camera_->stop();
    }
  sstm << "stopProcessing " << endl;
  logMessage(sstm.str ());sstm.flush ();
}


void MainWindow::update(const cv::Mat & image)
{


    if(!image.empty())
    {
        IplImage iplImage = image;

        QTime time;
        time.start();

        //Convert to grayscale
        IplImage * imageGrayScale = 0;
        if(iplImage.nChannels != 1 || iplImage.depth != IPL_DEPTH_8U)
        {
            imageGrayScale = cvCreateImage(cvSize(iplImage.width,iplImage.height), IPL_DEPTH_8U, 1);
            cvCvtColor(&iplImage, imageGrayScale, CV_BGR2GRAY);
        }
        cv::Mat img;
        if(imageGrayScale)
        {
            img = cv::Mat(imageGrayScale);
        }
        else
        {
            img =  cv::Mat(&iplImage);
        }
  if(this->isVisible())
        process(&iplImage);



    }

        cvWaitKey(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAdaBoos_triggered()
{
    //fprintf(stderr,"entered adaboost");
    m_fs = TRAIN_ADA;
    m_ratio = 1;

}

void MainWindow::on_actionAsymBosst_triggered()
{
    m_fs = TRAIN_ADA;
    m_ratio = 1.0268;

}



void MainWindow::on_actionFFS_2_triggered()
{
    m_fs = TRAIN_FFS;
    m_ratio = 1;    
    UpdateStatus();
}

void MainWindow::on_actionOriginal_triggered()
{
    m_lc = LC_ORIGINAL;

}

void MainWindow::on_actionLAC_triggered()
{
    m_lc = LC_LAC;
     UpdateStatus();

}

void MainWindow::on_actionFDA_triggered()
{
    m_lc = LC_FDA;


}

void MainWindow::UpdateStatus()
{
if(m_fs==TRAIN_ADA && m_ratio==1)
{
    ui->actionFFS_2->setEnabled(false);
    ui->actionAsymBosst->setEnabled(false);
}
else if(m_fs==TRAIN_ADA && m_ratio>1)
{
    ui->actionFFS_2->setEnabled(false);
    ui->actionAdaBoos->setEnabled(false);
}
else if(m_fs==TRAIN_FFS)
{
    ui->actionAdaBoos->setDisabled(true);
    ui->actionAsymBosst->setDisabled(true);
}
if(m_lc==LC_ORIGINAL)
{
    ui->actionLAC->setEnabled(false);
    ui->actionFDA->setEnabled(false);
}

    else if(m_lc==LC_LAC)
{
    ui->actionOriginal->setEnabled(false);
    ui->actionFDA->setEnabled(false);
}
    else if(m_lc==LC_FDA)
{
    ui->actionOriginal->setEnabled(false);
    ui->actionLAC->setEnabled(false);
}



}

void MainWindow::on_actionStart_Training_triggered()
{
    QElapsedTimer timer;
        timer.start();
//
  //      QProgressDialog ProgressDialog("Processing","Cancel", 0,0, this);
    //    ProgressDialog.setWindowModality(Qt::WindowModal);
      //  ProgressDialog.setValue(0);

    sstm << "Starting training " << endl;
    logMessage(sstm.str ());sstm.flush ();
    //this->setCursor(Qt::WaitCursor);
    sstm << " enterting ntrain " << endl;
    logMessage(sstm.str ());sstm.flush ();
    //InitGlobalData();

    //this->setCursor(Qt::ArrowCursor);
      ///ProgressDialog->show ();

      QFuture<void> future = QtConcurrent::run(InitTrain,this);
       this->FutureWatcher.setFuture(future);
      this->ProgressDialog->setMinimum(0);
        this->ProgressDialog->setMaximum(0);
        this->ProgressDialog->setWindowModality(Qt::WindowModal);
        this->ProgressDialog->exec();

       //future.waitForFinished();

      //ProgressDialog->close();




//   (this);
this->setCursor(Qt::ArrowCursor);

   sstm << "time taken to complete training "<< ((timer.elapsed()/1000)/60) << "minutes "  << endl;
   logMessage(sstm.str ());sstm.flush ();

  //    ProgressDialog.close();
}

void MainWindow::process(IplImage *img)
{
IntImage intimage;

if(img!=0 && img->width>0 && img->height >0)
{
intimage.SetSize(QSize(img->height,img->width));
int ih,iw;
for(int i=0,ih=img->height,iw=img->width;i<ih;i++)
{
    REAL* pdata = intimage.data[i];
    unsigned char* pimg = reinterpret_cast<unsigned char*>(img->imageData+img->widthStep*i);
    for(int j=0;j<iw;j++) pdata[j] = pimg[j];
}


ui->imageView_source->setData(img);
ui->imageView_source->update();
ui->imageView_source->setupGraphicsView();
cvWaitKey(1);
}
}



class DirList
{
public :
int count;
QString dir;
QDir data_path;
QStringList classes;
int position;
QStringList files;

public :
DirList(QString dir1)
{
    dir=QString(dir1);
    data_path=QDir(dir1.toStdString().c_str());
    count=CountFiles();
    files = data_path.entryList();
    position=0;
}

int CountFiles()
{
   int sum = 0;
   //QDir dir(dir.path());
//  data_path.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
//   QFileInfoList lst = data_path.entryInfoList();
//   for(int i = 0; i < lst.size(); i++)
//   {
//       sum = sum + CountFiles(lst.at(i).canonicalPath());
//   }
 data_path.setFilter(QDir::Files);

   return data_path.entryInfoList().size();
}


QString GetFiles()
{

    QString trainSet;
    bool ret=false;
    if(position>=count)
    {
        position=0;

    }

    while(ret==false)
    {

    trainSet = files.at(position);  
    if (trainSet.startsWith("."))
    {
            position=position+1;
            continue;
    }
        position=position+1;
        ret=true;
    }
    return QString("%1/%2").arg(data_path.path()).arg(trainSet);
}
};


void MainWindow::on_actionOpen_File_triggered()
{
IntImage a;
CascadeClassifier *cascade = new CascadeClassifier;
ASSERT(cascade != NULL);
 //InitGlobalData();
sstm << "Loading default cascade " << endl;
logMessage(sstm.str ());sstm.flush ();
cascade->LoadDefaultCascade();
sstm << "completed Loading default cascade " << endl;
logMessage(sstm.str ());sstm.flush ();

QString dir=openDir("Select Testing Directory");



DirList *list=new DirList(dir);

while(list->position<list->count)
{
  QString fileName =list->GetFiles();

sstm << "file name for testing is " << fileName.toStdString() << endl;
logMessage(sstm.str ());sstm.flush ();
a.Load(fileName.toStdString().c_str());
sstm << "completed loading file " << fileName.toStdString() << endl;
logMessage(sstm.str ());sstm.flush ();
IplImage *res=cascade->ApplyOriginalSize(a,fileName.toStdString().c_str());
sstm << "completed computing parameters for file " << fileName.toStdString() << endl;
logMessage(sstm.str ());sstm.flush ();
IplImage* img;

img = cvCreateImage(cvSize(a.width,a.height),IPL_DEPTH_8U,1);
for(int i=0,ih=img->height,iw=img->width;i<ih;i++)
{
    REAL* pdata = a.data[i];
    unsigned char* pimg = reinterpret_cast<unsigned char*>(img->imageData+img->widthStep*i);
    for(int j=0;j<iw;j++) pimg[j] = (unsigned char)pdata[j];
}

cv::Mat img1(res);
update(img1);
waitUser();

if(index==-1)
{
     list->position=list->position-2;
     if(list->position<-1)
         list->position=-1;
}
if(exitWait==true)
{
    break;

}

}
}



void ReadTrainSet(CString filename);
void MainWindow::on_actionTest_All_triggered()
{

    CascadeClassifier *cascade = new CascadeClassifier;
    ASSERT(cascade != NULL);
     //InitGlobalData();
    sstm << "Loading default cascade " << endl;
    logMessage(sstm.str ());sstm.flush ();
    cascade->LoadDefaultCascade();

    IntImage image;
  QElapsedTimer timer;
    ifstream f;
        int i,count;
        char buf[256];
       // double t1,t2;
        f.open(TestSet_filename);
            f>>count; f.ignore(256,'\n');
            sstm << "number of test images " << count <<endl;
            logMessage(sstm.str ());sstm.flush ();
            total_fp = 0;

               // t1=timeGetTime();
                for(i=0;i<count;i++)
                {
                    f.getline(buf,255,'\n');
                    if(cascade->count>0)
                    {

                        sstm << "loading image " << buf << endl;
                        logMessage(sstm.str ());sstm.flush ();
                        image.Load(buf);


                         sstm << "completed loading image " << buf << ":" << image.width << ":" << image.height<< endl;
                         logMessage(sstm.str ());sstm.flush ();



                        IplImage* img;

                        img = cvCreateImage(cvSize(image.width,image.height),IPL_DEPTH_8U,1);
                        for(int i=0,ih=img->height,iw=img->width;i<ih;i++)
                        {
                            REAL* pdata = image.data[i];
                            unsigned char* pimg = reinterpret_cast<unsigned char*>(img->imageData+img->widthStep*i);
                            for(int j=0;j<iw;j++) pimg[j] = (unsigned char)pdata[j];
                        }
                        IplImage * dst;
                        dst = cvCreateImage(cvSize(24,24),IPL_DEPTH_8U,1);
                        cvResize(img,dst);
                        cvSaveImage("/tmp/a1.jpg",dst);
                        image.Load("/tmp/a1.jpg");
                        sstm << "completed loading image 2" << buf << ":" << image.width << ":" << image.height<< endl;
                        logMessage(sstm.str ());sstm.flush ();
                        IplImage *res=cascade->ApplyOriginalSize(image,buf);
                        IntImage image1;
                        image.Resize(image1,10);
                        img = cvCreateImage(cvSize(image1.width,image1.height),IPL_DEPTH_8U,1);
                        for(int i=0,ih=img->height,iw=img->width;i<ih;i++)
                        {
                            REAL* pdata = image1.data[i];
                            unsigned char* pimg = reinterpret_cast<unsigned char*>(img->imageData+img->widthStep*i);
                            for(int j=0;j<iw;j++) pimg[j] = (unsigned char)pdata[j];
                        }


                        ui->imageView_source->setData(img);
                        //cv::Mat aa(res);
                        //cv::imshow("aaa",aa);
                        ui->imageView_source->update();
                        ui->imageView_source->setupGraphicsView();
                       //cvWaitKey(0);
                    }
                   // ui->label_nfeatures->setText(QString::number(keypoints.size()));
                   //
                    //ui->label_timeGui->setText(QString::number(time.restart()));




              //  ui->label_detectorDescriptorType->setText(QString("%1/%2").arg(Settings::currentDetectorType()).arg(Settings::currentDescriptorType()));


                    }


                f.close();


                CString s;
                    s.Format("Total #false positives = %d",total_fp);
                    sstm << s <<endl;
                    logMessage(sstm.str ());sstm.flush ();
                    s.Format("Total time: %g minutes.\n", ((timer.elapsed()/1000)/60) );
                    sstm << s <<endl;
                    logMessage(sstm.str ());sstm.flush ();



}



 QString MainWindow::saveFile(QString message)
 {
   QString filename = QFileDialog::getSaveFileName(
       this,
       tr(message.toStdString().c_str()),
               QString(dir.c_str ()),
       tr("Documents (*.*)") );
   if( !filename.isNull() )
   {
     qDebug( filename.toAscii() );
   }
   return filename;
 }

 QString MainWindow::openDir(QString message)
 {
   QString dirname = QFileDialog::getExistingDirectory(
       this,
       tr(message.toStdString().c_str()),
       QString(dir.c_str ()))

               ;
   if( !dirname.isNull() )
   {
     qDebug( dirname.toAscii() );
   }
   return dirname;
 }
 QString MainWindow::openFile(QString message)
  {
    //QFileDialog::getOp    enFileName( this, tr("Open Document"), train_dir.c_str(), tr("Document files (*.jpg *.bmp);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog );

    QString filename = QFileDialog::getOpenFileName(
        this,
        QObject::tr(message.toStdString().c_str()),
       QString(dir.c_str ()),
        QObject::tr("Doctument files (*.dat);;All files (*.*)") );
    if( !filename.isNull() )
    {
      qDebug( filename.toAscii() );
    }
    return filename;
  }


 void MainWindow::logMessage(string txt)
 {
     QString s=QString(txt.c_str ());
     ui->textEdit->append(s);
 }

 struct MyLessThan {
     bool operator()(const QString &s1, const QString &s2) const {
         int st1 = s1.toInt ();
         int st2 = s2.toInt ();

         if (st1 < st2)
             return false;
         if (st1 > st2)
         return true;
     }
 };

 void MainWindow::on_actionCreate_Training_data_file_triggered()
 {
     // InitGlobalData();
     QString dir=openDir("Select Training Directory");

    logMessage("training direcgtory is "+dir.toStdString());
    QString file=saveFile("Select Training File ");
    logMessage("training file is "+ file.toStdString());
//    train_data(file,dir);

//    sleep(3);

    on_pushButtonx_clicked (file,dir);

   }

 void MainWindow::train_data(QString file,QString dir)
 {



     int count=0;



    QDir data_path(dir.toStdString().c_str());
    //data_path.setFilter(QDir::NoDotAndDotDot);

    QStringList classes = data_path.entryList();
    QString trainSet;

    QStringList cSets;

    for (int i = 0; i < classes.size(); ++i) {
         trainSet = classes.at(i);
         sstm << "dir name is " << trainSet.toStdString() << endl;
         logMessage(sstm.str ());sstm.flush ();
         if (trainSet.startsWith("."))
             continue;
         cSets.append(trainSet);
         QDir file_path1(data_path.path()+QString("/%1").arg(trainSet));
         //if(trainSet.toInt ()==0)
         count=count+CountFiles(file_path1.path());

         //trainSet.remove();

    };

    MyLessThan le;
    qSort(cSets.begin(), cSets.end(), le);

    sstm << "number of training sampples " << count << endl;
    logMessage(sstm.str ());sstm.flush ();

    ofstream of;
    of.open(file.toStdString().c_str(),ios_base::out | ios_base::binary);
    of<<count<<endl;


    QString fileSet ;
    QStringList fileSets;
    QStringList fileList;
    string handPathString ;

 for(unsigned int trainNum=0; trainNum < cSets.size(); trainNum++) {
      trainSet = cSets.at(trainNum);
      sstm << "training directory name " << trainSet.toStdString () <<  endl;
      //exit(1);
      logMessage(sstm.str ());sstm.flush ();

     /*if(trainNum==0)
     {
         trainSet=QString("%1").arg("0");
     }
     else
     {
         trainSet=QString("%1").arg("1");

     }*/

       QDir file_path(data_path.path()+QString("/%1").arg(trainSet));
       fileList = file_path.entryList();




                 for (int i = 0; i < fileList.size(); i++) {
                   fileSet = fileList.at(i);
                   if (fileSet.startsWith("."))
                       continue;
                   fileSets.append(fileSet);
                   fileSet.clear();
                  };
                   fileList.clear();
                  unsigned int i=0;
                 for(i=0; i < fileSets.size(); i++) {
                fileSet = fileSets.at(i);

     QFileInfo handPath(data_path.path() + QString("/%1/%2").arg(trainSet).arg(fileSet));
    handPathString = handPath.filePath().toStdString();


    IplImage *aa = cvLoadImage(handPathString.c_str(),0);
    if(aa==0&&aa->width<=0&&aa->height<=0)
    {
        sstm << "error in loading file " << endl;
        logMessage(sstm.str ());sstm.flush ();
        perror("error in loading file");
        continue;
    }


        IplImage *bb = cvCreateImage(cvSize(sx,sy),aa->depth,aa->nChannels);
        cvEqualizeHist (aa,aa);
        if(trainSet.toInt()==1)
        {
        cvResize(aa,bb);
        cvSaveImage("tmp.bmp",bb);
        cvFlip (bb,bb,1);
        cvSaveImage("tmp1.bmp",bb);
        }
        else
        {
        cvSaveImage("tmp.bmp",aa);
        cvFlip (aa,aa,1);
        cvSaveImage("tmp1.bmp",aa);
        }
        cvReleaseImage(&aa);
        cvReleaseImage(&bb);








    IntImage b;
    b.Load("tmp.bmp");

    int l=1;

    unsigned char* writebuf;
    writebuf = new unsigned char[sx*sy];
    ASSERT(writebuf!=NULL);
    if(trainSet.toInt()==0||trainSet.toInt()==1)
    {
             of<<trainSet.toInt()<<endl;
             of<<sx<<" "<<sy<<endl;
             for(int k=0;k<sx;k++)
                for(int t=0;t<sy;t++)
                   writebuf[k*sy+t] = (unsigned char)((int)b.data[k][t]);
             of.write((char*)writebuf,sx*sy);
                 of<<endl;
                 delete[] writebuf; writebuf=NULL;
    }
    else
    {
        sstm << "error in data"  << endl;
        logMessage(sstm.str ());sstm.flush ();
    }
    b.Clear();


/*
    b.Load("tmp1.bmp");

    l=1;


    writebuf = new unsigned char[sx*sy];
    ASSERT(writebuf!=NULL);
    if(trainSet.toInt()==0||trainSet.toInt()==1)
    {
             of<<trainSet.toInt()<<endl;
             of<<sx<<" "<<sy<<endl;
             for(int k=0;k<sx;k++)
                for(int t=0;t<sy;t++)
                   writebuf[k*sy+t] = (unsigned char)((int)b.data[k][t]);
             of.write((char*)writebuf,sx*sy);
                 of<<endl;
                 delete[] writebuf; writebuf=NULL;
}
    else
    {
        sstm << "error in data"  << endl;
        logMessage(sstm.str ());sstm.flush ();
    }
*/
             }

                 fileSets.clear();
                 fileSet.clear();

 }
   of.flush();
   of.close();


 }



 int MainWindow::CountFiles(QString path)
{
    int sum = 0;
    QDir dir(path);
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    QFileInfoList lst = dir.entryInfoList();
    for(int i = 0; i < lst.size(); i++)
    {
        sum = sum + CountFiles(lst.at(i).canonicalPath());
    }
    dir.setFilter(QDir::Files);
    return dir.entryInfoList().size() + sum;
}

void MainWindow::on_actionTest_Live_triggered()
{
    logMessage("viewing trainng image");
   // InitGlobalData();

   //sstm << "number of traning examples " << facecount  << ":" << totalcount <<endl;
   //logMessage(sstm.str ());sstm.flush ();


   QString file=openFile("Select Training File ");
   sstm << "training file is " << file.toStdString() << endl;
   logMessage(sstm.str ());sstm.flush ();
   if(file.capacity()>0)
   {
    trainset_filename=file.toStdString();
   }
   // sstm << "Training  file is " << trainset_filename << endl;
    //logMessage(sstm.str ());sstm.flush ();
   ReadTrainSet(trainset_filename);
   sstm << "number of traning examples " << facecount <<":"<< totalcount<< endl;
   logMessage(sstm.str ());sstm.flush ();

   for(int k=0;k<facecount;k++)
   {
        int new1=0;
       if(trainset[k].label==1)
           new1=k+facecount;
       else
           new1=totalcount-facecount+k;

       IplImage* img;
       IntImage image;
       image.SetSize(QSize(sx+1,sy+1));
       for(int i=1;i<sx+1;i++)
        for(int j=1;j<sy+1;j++)
           {
                   image.data[i][j]=trainset[k].data[i][j]+trainset[k].data[i-1][j-1]-trainset[k].data[i][j-1]-trainset[k].data[i-1][j];
                   //trainset[k].data[i][j]=f;
          }

       img = cvCreateImage(cvSize(trainset[k].width,trainset[k].height),IPL_DEPTH_8U,1);
       for(int i=0,ih=img->height,iw=img->width;i<ih;i++)
       {
           REAL* pdata = image.data[i];
           unsigned char* pimg = reinterpret_cast<unsigned char*>(img->imageData+img->widthStep*i);
           for(int j=0;j<iw;j++) pimg[j] = (unsigned char)pdata[j];
       }

       IplImage *dst=cvCreateImage(cvSize(240,320),IPL_DEPTH_8U,1);
       cvResize(img,dst);
//       ui->imageView_source->setData(dst);
//       ui->imageView_source->update();
//       ui->imageView_source->setupGraphicsView();

       cv::Mat a(dst);
       //cv::imshow("training image",a);
       update(a);

       //if(k=='r')
         //  break;
       if(facecount!=totalcount)
       {
       image.SetSize(QSize(sx+1,sy+1));
       for(int i=1;i<sx+1;i++)
        for(int j=1;j<sy+1;j++)
           {
                   image.data[i][j]=trainset[new1].data[i][j]+trainset[new1].data[i-1][j-1]-trainset[new1].data[i][j-1]-trainset[new1].data[i-1][j];
                   //trainset[k].data[i][j]=f;
          }

       img = cvCreateImage(cvSize(trainset[new1].width,trainset[new1].height),IPL_DEPTH_8U,1);
       for(int i=0,ih=img->height,iw=img->width;i<ih;i++)
       {
           REAL* pdata = image.data[i];
           unsigned char* pimg = reinterpret_cast<unsigned char*>(img->imageData+img->widthStep*i);
           for(int j=0;j<iw;j++) pimg[j] = (unsigned char)pdata[j];
       }

      dst=cvCreateImage(cvSize(240,320),IPL_DEPTH_8U,1);
       cvResize(img,dst);
//       ui->imageView_source->setData(dst);
//       ui->imageView_source->update();
//       ui->imageView_source->setupGraphicsView();

       //cv::Mat a1(dst);
       //cv::imshow("training image",a1);
        //k=getch();
       //if(k=='r')
       //  break;
       }

       waitUser();
       if(index==-1)
       {
            k=k-2;
            if(k<-1)
                k=-1;
       }
       if(exitWait==true)
       {

           break;
       }
   }


}















void MainWindow::on_actionCreate_training_file_triggered()
{
    on_actionCreate_Training_data_file_triggered();
}
#define CLOCKS_PER_USEC ((double)(CLOCKS_PER_SEC)/1000000.0)
#define RefClockArch RefClockLinux

void MainWindow::on_actionCamera_triggered(bool checked)
{



}



void MainWindow::on_pushButton_2_clicked()
{
 continueCalc = true;
 index=1;
 exitWait=false;
}

void MainWindow::on_pushButton_clicked()
{
 continueCalc = true;
 index=-1;
 exitWait=false;
}

void MainWindow::on_pushButton_3_clicked()
{
    continueCalc = true;
    exitWait=true;
}

void MainWindow::on_actionStop_Training_triggered()
{
    train_exit=true;
}
