#ifndef ODWT_H
#define ODWT_H
#include "common.hpp"


void dwt1D(Mat &A, Mat &Lo, Mat &Hi,vector<float> &approx,vector<float> &detail);

//function to perform single level wavelet reconstruction
void idwt2D(vector<Mat> output,Mat &Lo,Mat &Hi,Mat &x,Size s)
{

    vector <Mat> tmp;
    tmp.resize (4);



        //upsample the columns
        for(int k=0;k<4;k++)
        {
            //perform transpose
            cv::transpose (output[k],output[k]);
            //upsample the rows
            int cols=2*output[0].cols+Lo.cols-1;
           tmp[k]=Mat(output[k].rows,cols,output[k].type());
            for(int i=0;i<output[k].rows;i++)
            {
                      float *ptr=output[k].ptr<float>(i);
                       float *ptr1=tmp[k].ptr<float>(i);
                       int l=0;
                       for(int j=0;j<tmp[k].cols;j++)
                       {
                           if(j<(int)(Lo.cols/2))
                           {
                               ptr1[j]=0;
                           }
                           else
                           {
                               ptr1[j]=ptr[l];
                               ptr1[j+1]=0;
                               l=l+1;
                               j=j+1;
                           }
                       }
        }
            //apply low pass reconstruction filter
            if(k==0||k==2)
            {
                filter2D(tmp[k],tmp[k],tmp[k].depth (),Lo,Point(0,0),0,BORDER_CONSTANT);

             }
            else if(k==1||k==3)
            {
                filter2D(tmp[k],tmp[k],tmp[k].depth (),Hi,Point(0,0),0,BORDER_CONSTANT);
            }


            //perform transpose of matrix
            cv::transpose (tmp[k],tmp[k]);

            //perform upsampling on the row
            cols=2*tmp[k].cols+Lo.cols-1;
           Mat temp=Mat(tmp[k].rows,cols,tmp[k].type());
            for(int i=0;i<tmp[k].rows;i++)
            {
                      float *ptr=tmp[k].ptr<float>(i);
                       float *ptr1=temp.ptr<float>(i);
                       int l=0;
                       for(int j=0;j<temp.cols;j++)
                       {
                           if(j<(int)(Lo.cols/2))
                           {
                               ptr1[j]=0;
                           }
                           else
                           {
                               ptr1[j]=ptr[l];
                               ptr1[j+1]=0;
                               l=l+1;
                               j=j+1;
                           }
                       }
        }
            temp.copyTo (tmp[k]);

            if(k==0||k==1)
            {
                filter2D(tmp[k],tmp[k],tmp[k].depth (),Lo,Point(0,0),0,BORDER_CONSTANT);
             }
            else if(k==2||k==3)
            {
                filter2D(tmp[k],tmp[k],tmp[k].depth (),Hi,Point(0,0),0,BORDER_CONSTANT);
            }




        }


        cv::add (tmp[0],tmp[1],x);
        cv::add (x,tmp[2],x);
        cv::add (x,tmp[3],x);

        //removing the border pixels
        if(s.width!=0 && s.height!=0)
        {
            x=x(Rect(0,0,s.width,s.height));
        }
        else
        {
            x=x(Rect(0,0,x.rows-1,x.cols-1));
        }






}

//computing 2D DWT and storing the coefficients in LL,LH,HL and HH matrices
void dwt2D(Mat &A,Mat &Lo,Mat &Hi,vector<Mat> &output)
{
    output.resize (4);
    Mat temp1,temp2;


    //applyping 1D low pass filtering to the row of the matrix
     filter2D(A,temp1,A.depth (),Lo,Point(0,0),0,BORDER_REFLECT);



     temp2=Mat(temp1.rows,ceil((float)temp1.cols/2),temp1.type ());
     //downsample the rows by 2
        for(int i=0;i<temp1.rows;i++)
        {
         int k=0;
        float * ptr=temp1.ptr<float> (i);
        float * ptr1=temp2.ptr<float> (i);
        for(int j=0;j<temp1.cols;j=j+2)
        {
            ptr1[k]=ptr[j];
            k=k+1;
        }

        }

        //downsampling rows and applying the low and high pass filters to the columns of the matrix

        //trasposing the matrix
        Mat temp3;
        cv::transpose (temp2,temp3);

        Mat temp4,temp5;
        //applying the low pass filter to columns of output
        filter2D(temp3,temp4,temp2.depth (),Lo,Point(0,0),0,BORDER_REFLECT);
        //applying high pass filter to columns of the output
        filter2D(temp3,temp5,temp3.depth (),Hi,Point(0,0),0,BORDER_REFLECT);

        //downsampling

        output[0]=Mat(temp4.rows,ceil((float)temp4.cols/2),temp4.type());
        output[1]=Mat(temp4.rows,ceil((float)temp4.cols/2),temp4.type());
        for(int i=0;i<temp4.rows;i++)
        {
        float * ptr1=temp4.ptr<float> (i);
        float * ptr2=temp5.ptr<float> (i);
        float * ptr3=output[0].ptr<float> (i);
        float * ptr4=output[1].ptr<float> (i);
        int k=0;
        for(int j=0;j<temp4.cols;j=j+2)
        {
            ptr3[k]=ptr1[j];
            ptr4[k]=ptr2[j];
            k=k+1;
        }
        }

        cv::transpose (output[0],output[0]);
        cv::transpose (output[1],output[1]);

        //similarily computing HH and HL


        //applyping 1D low pass filtering to the row of the matrix
         filter2D(A,temp1,A.depth (),Hi,Point(0,0),0,BORDER_REFLECT);

         temp2=Mat(temp1.rows,ceil((float)temp1.cols/2),temp1.type ());
         //downsample the rows by 2
            for(int i=0;i<temp1.rows;i++)
            {
             int k=0;
            float * ptr=temp1.ptr<float> (i);
            float * ptr1=temp2.ptr<float> (i);
            for(int j=0;j<temp1.cols;j=j+2)
            {
                ptr1[k]=ptr[j];
                k=k+1;
            }

            }
            //downsampling rows and applying the low and high pass filters to the columns of the matrix

            //trasposing the matrix
            temp3.setTo (cv::Scalar::all (0));
            cv::transpose (temp2,temp3);


            //applying the low pass filter to columns of output
            filter2D(temp3,temp4,temp2.depth (),Lo,Point(0,0),0,BORDER_REFLECT);
            //applying high pass filter to columns of the output
            filter2D(temp3,temp5,temp3.depth (),Hi,Point(0,0),0,BORDER_REFLECT);

            //downsampling
            output[2]=Mat(temp4.rows,ceil((float)temp4.cols/2),temp4.type());
            output[3]=Mat(temp4.rows,ceil((float)temp4.cols/2),temp4.type());

            for(int i=0;i<temp4.rows;i++)
            {
            float * ptr1=temp4.ptr<float> (i);
            float * ptr2=temp5.ptr<float> (i);
            float * ptr3=output[2].ptr<float> (i);
            float * ptr4=output[3].ptr<float> (i);
            int k=0;
            for(int j=0;j<temp4.cols;j=j+2)
            {
                ptr3[k]=ptr1[j];
                ptr4[k]=ptr2[j];
                k=k+1;
            }
            }

            cv::transpose (output[2],output[2]);
            cv::transpose (output[3],output[3]);



}




void idwt1D(vector<float> &approx,vector<float> &detail,Mat &Lo,Mat &Hi,Mat &x,int l=0)
{
    //perform didactic upsampling
    int rows=2*approx.size ()+Lo.cols-1;
    Mat A=Mat(1,rows,CV_32FC(1));
    A.setTo (cv::Scalar::all (0));
    float *ptr=A.ptr<float>();
    int j=0;
    for(int i=0;i<rows;i++)
    {
        if(i<(int)(Lo.cols/2))
        {
            ptr[i]=0;
        }
        else
        {
        ptr[i]=approx[j];
        ptr[i+1]=0;
        i=i+1;
        j=j+1;
        }


    }

    Mat temp1;
    filter2D(A,temp1,A.depth (),Lo,Point(0,0),0,BORDER_CONSTANT);

    rows=2*detail.size ()+Hi.cols-1;
    Mat B=Mat(1,rows,CV_32FC(1));
    B.setTo (cv::Scalar::all (0));
    ptr=B.ptr<float>();
    j=0;
    for(int i=0;i<rows;i++)
    {
        if(i<(int)(Hi.cols/2))
        {
            ptr[i]=0;
        }
    else
        {
        ptr[i]=detail[j];
        ptr[i+1]=0;
        i=i+1;
        j=j+1;

        }
    }

    Mat temp2;
    filter2D(B,temp2,B.depth (),Hi,Point(0,0),0,BORDER_CONSTANT);


     float *aptr=temp1.ptr<float>();
     float *bptr=temp2.ptr<float>();


     int size=(l!=0)?l:2*approx.size();
     x=Mat(1,size,A.type ());
     ptr=x.ptr<float>();
     for(int i=0;i<x.cols;i++)
     {
         ptr[i]=aptr[i]+bptr[i];

     }


}

void mldwt2D(vector<vector <Mat> >&components,Mat Lo,Mat Hi,Mat &x,Size s)
{
    int N=components.size ();

    for(int i=N-1;i>=0;i--)
    {
        if(i==0)
        idwt2D(components[i],Lo,Hi,x,s);
        else
            idwt2D(components[i],Lo,Hi,x,Size(0,0));
        if(i!=0)
        components[i-1][0]=x;


    }
}

//function to perform multi level wavelet decomposition
void mldwt2D(Mat A,Mat Lo,Mat Hi,int N,vector<vector <Mat> >&components)
{

    Mat temp;
    A.copyTo (temp);
    components.resize (N);
    for(int i=0;i<N;i++)
    {
        dwt2D(temp,Lo,Hi,components[i]);
        components[i][0].copyTo(temp);
    }

}

//passing the approximation and detail coeffcients for reconstruction
void mlidwt1D(vector<float>&approx,vector<vector<float> >&detail,Mat Lo,Mat Hi,Mat &x,int length)
{
    int N=approx.size ();
    Mat tmp;
    vector<float> app=approx;
    for(int i=N-1;i>=0;i--)
    {
        //calling IDWT routine
        int l=(i==0)?length:0;
        idwt1D(app,detail[i],Lo,Hi,tmp,l);
        app.resize (tmp.cols);
        float *ptr=tmp.ptr<float>();
        for(int j=0;j<tmp.cols;j++)
        {
            app[j]=ptr[j];
        }

    }

    tmp.copyTo (x);

}

//performs multiscale DWT computation
void mldwt1D(Mat &A, Mat &Lo, Mat &Hi,int N,vector<vector<float> >&approx,vector<vector<float> >&detail)
{

    approx.resize (N);
    detail.resize (N);
    Mat tmp_A;
    A.copyTo (tmp_A);
    for(int i=0;i<N;i++)
    {
        dwt1D(tmp_A,Lo,Hi,approx[i],detail[i]);
        int cols=approx[i].size();
        vector<float> x=approx[i];
        tmp_A=Mat(1,cols,tmp_A.type());
        tmp_A.setTo (cv::Scalar::all (0));
        float *ptr=tmp_A.ptr<float>(0);
        for(int j=0;j<cols;j++)
        {
            ptr[j]=approx[i][j];
            //cerr << approx[i][j] << ":";
        }
        //cerr << "#############" <<endl;
    }
}

void dwt1D(Mat &A, Mat &Lo, Mat &Hi,vector<float> &approx,vector<float> &detail)
{
      //perform symmetric extension of border pixels
      int lext=Lo.cols-1;

      Mat temp1,temp2;
       //performing 1D convolution
       filter2D(A,temp1,A.depth (),Lo,Point(0,0),0,BORDER_REFLECT);


     //perform 1D convolution
       filter2D(A,temp2,A.depth (),Hi,Point(0,0),0,BORDER_REFLECT);

       //downsamples and store the coefficients
       float *aptr=temp1.ptr<float>();
       for(int i=0;i<temp1.cols;i=i+2)
       {
           approx.push_back (aptr[i]);

       }

       std::cerr << endl;
       aptr=temp2.ptr<float>();
       for(int i=0;i<temp2.cols;i=i+2)
       {
           detail.push_back (aptr[i]);

       }







}

#endif // ODWT_H
