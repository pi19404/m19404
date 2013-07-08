#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common.h"
#include "camera.h"
#include <QTime>
#include <QEventLoop>
#include <QFutureWatcher>
#include <QProgressDialog>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    

public:
    double m_ratio;
    int m_size;
    int m_fs;
    int m_lc;
    Camera * camera_;
    int lowestRefreshRate_;
    QTime updateRate_;
    QTime refreshStartTime_;
    std::stringstream sstm;
    // MainWindow(QWidget *parent = 0);
     MainWindow(QWidget *parent = 0,int argc=0,char **argv=0);
    int CountFiles(QString path);
     QString openDir(QString message);
      QString saveFile(QString message);
      QString openFile(QString message);
          QProgressDialog *ProgressDialog;
           QFutureWatcher<void> FutureWatcher;
           std::string dd;
      bool continueCalc;
      bool exitWait;
      int index;
      bool train_exit;
      void waitUser()
      {
         //std::cerr << "waiiting" <<std::endl;
          do
          {
          QEventLoop  e ;
          e.processEvents(QEventLoop::AllEvents);
          }
          while(!continueCalc);
          continueCalc = false;
          //std::cerr << "exiting" <<std::endl;
      }

      void logMessage(std::string txt);
void train_data(QString file,QString dir);
~MainWindow();
    void UpdateStatus();
private slots:
    void on_actionAdaBoos_triggered();

    void on_actionAsymBosst_triggered();

    void on_actionFFS_2_triggered();

    void on_actionOriginal_triggered();

    void on_actionLAC_triggered();

    void on_actionFDA_triggered();

    void on_actionStart_Training_triggered();
    void process(IplImage *img);
    void on_actionOpen_File_triggered();

    void on_actionTest_All_triggered();



    void on_actionTest_Live_triggered();

    void update(const cv::Mat & image);


    void slot_finished();
    void on_pushButtonx_clicked(QString file,QString dir);




void stopProcessing();
void startProcessing();




    void on_actionCreate_Training_data_file_triggered();

    void on_actionCreate_training_file_triggered();

    void on_actionCamera_triggered(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_actionStop_Training_triggered();

    void on_actionCreate_Haar_Classifier_txt_file_triggered();

public:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H


