Mat CreateGaborFilterImage(char* demoName,   float lambda, float theta, float psi, float gaussvar, float gamma,Size size=Size(21,21));
Mat CreateGaborFilterImage(char* demoName,   float lambda, float theta, float psi, float gaussvar, float gamma,Size size)
{


    Mat ima1=cv::getGaborKernel(size,gaussvar,theta,lambda,gamma,psi,CV_32F);
//    cerr << ima.cols <<":" << ima.rows << ":"<<ima.channels() <<endl;
    //Mat gaborKernel = getGaborKernel(size, 3.0, -Math.PI/4, Math.PI, 10.0, Math.PI*0.5, CvType.CV_64F);
    //Imgproc.filter2D(intermediate, output, -1, gaborKernel);
    //Bitmap temp = Bitmap.createBitmap(intermediate.cols(), intermediate.rows(), Config.ARGB_8888);
    //Utils.matToBitmap(output, temp);
    Mat d1=Mat(size.width,size.height,CV_8UC(3));
    //ima.convertTo(d1,CV_8U,255.0f);
    d1.setTo(cv::Scalar::all(0));

    int mx=d1.cols/2;
    int my=d1.rows/2;
    Mat ima;
    cv::normalize(ima1,ima,-1,1,cv::NORM_MINMAX);
    float *ptr1=ima.ptr<float>();
    uchar *ptr2=d1.ptr<uchar>();
    float MinGaborValue = std::numeric_limits<float>::max();
    float MaxGaborValue = std::numeric_limits<float>::min();
    for (int dy=0; dy<size.height; dy++)
        {
            for (int dx=0; dx<size.width; dx++)
            {

                double val=ptr1[dx+dy*size.width];
                //cerr << val <<endl;
                int R=0;
                if (val>=0.1f)
                {
                    R = (int)floor((val * 255.0f));

                    int sx=(size.width/2+(dx))%size.width;
                    sx=dx;
                    int sy=dy;//(size.height/2+(dy))%size.height;
                    ptr2[3*(sx)+(3*(sy*d1.cols))+0]=R;
                }
                else if(val <=-0.1f)
                {
                    R = (int)floor((-val * 255.0f));
                    int sx=(size.width/2+(dx))%size.width;
                    int sy=dy;//(size.height/2+(dy))%size.height;
                    sx=dx;
                    //int sy=(size.width/2+(dy))%size.height;
                    ptr2[3*(sx)+(3*(sy*d1.cols))+2]=R;
                }
      /*          else
                {
                    R=0;
                    int sx=(size.width/2+(dx))%size.width;
                    int sy=dy;//(size.height/2+(dy))%size.height;
                    sx=dx;
                    ptr2[3*(sx)+(3*(sy*d1.cols))+0]=R;
                    ptr2[3*(sx)+(3*(sy*d1.cols))+1]=R;
                    ptr2[3*(sx)+(3*(sy*d1.cols))+2]=R;
                }*/

                if (val<MinGaborValue)
                MinGaborValue = val;
                  if (val>MaxGaborValue)
                   MaxGaborValue = val;

            }
        }
    //cerr << MinGaborValue << ":" << MaxGaborValue << endl;
    Mat d2;
    cv::resize(d1,d2,Size(256,256));
    //d1.copyTo(d2);
    //cv::normalize(d2,d2,0,255,cv::NORM_MINMAX);
/*
    CvFont font;
        double hScale=1.0;
        double vScale=1.0;
        int    lineWidth=1;
        cvInitFont(&font,CV_FONT_HERSHEY_PLAIN, hScale,vScale,0,lineWidth);
        char txt[100];
        /*cvPutText (img, "Gabor filter values visualization by Juergen Mueller.",                             cvPoint(10,20), &font, cvScalar(0,255,255) );
        cvPutText (img, "variable naming conventions as used by http://en.wikipedia.org/wiki/Gabor_filter.", cvPoint(10,50), &font, cvScalar(0,255,255) );
        cvPutText (img, "Red: positive values. Blue: negative values",                                       cvPoint(10,70), &font, cvScalar(0,255,255) );


        cvPutText(img, demoName, cvPoint(10,100), &font, cvScalar(0,255,0) );
        sprintf_s(txt, "1. wavelength  (lambda) : %.2f", wavelength);    cvPutText (img, txt, cvPoint(10,120), &font, cvScalar(0,255,255) );
        sprintf_s(txt, "2. orientation (theta)  : %.2f", orientation);   cvPutText (img, txt, cvPoint(10,140), &font, cvScalar(0,255,255) );
        sprintf_s(txt, "3. gaussvar    (sigma)  : %.2f", gaussvar);      cvPutText (img, txt, cvPoint(10,160), &font, cvScalar(0,255,255) );
        sprintf_s(txt, "4. phaseoffset (phi)    : %.2f", phaseoffset);   cvPutText (img, txt, cvPoint(10,180), &font, cvScalar(0,255,255) );
        sprintf_s(txt, "5. aspectratio (gamma)  : %.2f", aspectratio);   cvPutText (img, txt, cvPoint(10,200), &font, cvScalar(0,255,255) );

        sprintf_s(txt, "min gabor val : %.2f", MinGaborValue);   cvPutText (img, txt, cvPoint(10,230), &font, cvScalar(0,255,255) );
        sprintf_s(txt, "max gabor val : %.2f", MaxGaborValue);   cvPutText (img, txt, cvPoint(10,250), &font, cvScalar(0,255,255) );
*/
    imshow(demoName,d2);
    waitKey(1000);
    return ima1;

}


int kernel_size=21;
int pos_sigma= 5;
int pos_lm = 50;
int pos_th = 0;
int pos_psi = 90;
Mat src_f,dest;
void Process(int , void *)
{
    double sig = pos_sigma;
    double lm = 0.5+pos_lm/100.0;
    double th = pos_th*CV_PI/180;
    double ps = pos_psi*CV_PI/180;
    cv::Mat kernel=CreateGaborFilterImage
            ("1. Wavelength is changed.", lm, th, ps, sig, 1,Size(kernel_size,kernel_size));
    //cv::Mat kernel = mkKernel(kernel_size, sig, th, lm, ps);
    cv::filter2D(src_f, dest, CV_32F, kernel);
    cv::imshow("Process window", dest);
    cv::Mat Lkernel(kernel_size*20, kernel_size*20, CV_32F);
    cv::resize(kernel, Lkernel, Lkernel.size());
    Lkernel /= 2.;
    Lkernel += 0.5;
    cv::imshow("Kernel", Lkernel);
    cv::Mat mag;
    cv::pow(dest, 2.0, mag);
    cv::imshow("Mag", mag);
}
int main()
{

    float wavelength;
    float orientation;
    float gaussvar;
    float phaseoffset;
    float aspectratio;
    Size s=Size(21,21);

    Mat src,src1;
    src=imread(argv[1],0);
    cv::resize(src,src1,Size(256,256));
     src1.convertTo(src_f, CV_32F, 1.0/255, 0);
     if (!kernel_size%2)
        {
            kernel_size+=1;
        }
         cv::namedWindow("Process window", 1);
         cv::createTrackbar("Sigma", "Process window", &pos_sigma, kernel_size, Process);
         cv::createTrackbar("Lambda", "Process window", &pos_lm, 100, Process);
         cv::createTrackbar("Theta", "Process window", &pos_th, 180, Process);
         cv::createTrackbar("Psi", "Process window", &pos_psi, 360, Process);
         Process(0,0);
         cv::waitKey(0);

/*
      wavelength  = 40.0f;
      orientation = (float)M_PI/4.0f;
       gaussvar    = 20.0f;
       phaseoffset = M_PI/2;
       aspectratio = 0.5f;
       for(int k=0;k<10;k++)
       for (orientation=0.0f; orientation<=2*(float)M_PI; orientation+=2*(float)M_PI / 8.0f)
       {
          CreateGaborFilterImage("2. Orientation is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
       }
*/
       wavelength  = 40.0f;
       orientation = (float)0;//M_PI/4.0f;
       gaussvar    = 20.0f;
       phaseoffset = 0.0f;
       aspectratio = 0.5f;
       for (wavelength=5.0f; wavelength<=100.0f; wavelength+=5.0f)
       {
           CreateGaborFilterImage("1. Wavelength is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
       }


       wavelength  = 40.0f;
       orientation = (float)0;//M_PI/4.0f;
       gaussvar    = 20.0f;
       phaseoffset = 0.0f;
       aspectratio = 0.5f;
       for (orientation=0.0f; orientation<=2*(float)M_PI; orientation+=2*(float)M_PI / 8.0f)
       {
          CreateGaborFilterImage("2. Orientation is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
       }


       wavelength  = 40.0f;
       orientation = (float)0;//M_PI/4.0f;
       gaussvar    = 20.0f;
       phaseoffset = 0.0f;
       aspectratio = 0.5f;
       for (gaussvar=10.0f; gaussvar<=60; gaussvar+=5)
       {
           CreateGaborFilterImage("3. Gaussvariance is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
       }



       wavelength  = 40.0f;
       orientation = (float)0;//M_PI/4.0f;
       gaussvar    = 20.0f;
       phaseoffset = 0.0f;
       aspectratio = 0.5f;
       for (phaseoffset = 0.0f; phaseoffset<=2*(float)M_PI; phaseoffset+=2*(float)M_PI / 16.0f)
       {
            CreateGaborFilterImage("4. Phase offset is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
       }



       wavelength  = 40.0f;
       orientation = (float)0;//M_PI/4.0f;
       gaussvar    = 20.0f;
       phaseoffset = 0.0f;
       aspectratio = 0.5f;
       for (aspectratio = 0.1f; aspectratio<=1.0f; aspectratio+=.05f)
       {
           CreateGaborFilterImage("5. Aspect ratio is changed.", wavelength, orientation, phaseoffset, gaussvar, aspectratio);
       }

    cv::waitKey(0);
    return 0;
}
