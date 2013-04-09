package libsvm;

/** Class for computing histogram of oriented gradients for color images **/


import com.googlecode.javacpp.FloatPointer;
import com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_imgproc.*;
import com.googlecode.javacv.cpp.opencv_imgproc.*;
import java.lang.Math.*;
import java.nio.FloatBuffer;
import java.nio.DoubleBuffer;

public class HOG3_fast {

   	 float pi = (float)3.1416;
	 
         /** number of windows along x and y dircetions **/
	 public int nwin_x=3; 
	 public int nwin_y=3;
         
         /** number of histogram bins **/
	 public int B=12; //set here the number of histogram bins

         /** gradients along x and y directions **/
	 CvMat hx;
	 CvMat hy;
	 
         /** storage for descriptors **/
	 CvMat H,H2; 
         
         /** storing floating point image **/
	 IplImage Im1 ; 
         
         
	 int step_x,C,L;
	 int step_y;
	 int first=0;
	
         /** gradients along x and y directions **/
	 IplImage grad_xr,grad_yu;
	 
         /** magnitude and orientation of gradients **/
	 IplImage magnitude;
	 IplImage orientation;
	 
	 IplImage magnitude1;
	 IplImage orientation1;
	 
	 IplImage I1,I2,I3,I4,I5,I6;
	 
	 CvMat angles2;
	 CvMat magnit2;
	 
	 CvMat v_angles, v_magnit,h1,h2   ;
	 CvMat v_angles1,v_magnit1;
         
         
	 long start,end,time;

         /** function returns maximum of 3 input float **/
         float maximum(float x, float y, float z) {
	     float max = x; /* assume x is the largest */

	     if (y > max) { /* if y is larger than max, assign y to max */
	         max = y;
	     } /* end if */

	     if (z > max) { /* if z is larger than max, assign z to max */
	         max = z;
	     } /* end if */

	     return max; /* max is the largest value */
	 }
	 
         public HOG3_fast()
         {
             
         }
         /** function to compute HOG of input image **/
	 public void compute(IplImage Im,FloatPointer descriptors)
	 {
		 
		 
		 if(first==0)
		 {
			L=Im.height();
			C=Im.width();
			step_x=(int) Math.floor(C/(nwin_x+1));
			step_y=(int) Math.floor(L/(nwin_y+1));
	    
                         H = cvCreateMat(nwin_x*nwin_y*B,1, CV_32FC3);
                         Im1=cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F,3);
		 
                         
                        hx=cvCreateMat(1,3,CV_32F); 
                        FloatPointer hxf=hx.data_fl();
		 hxf.put(0, -1);
		 hxf.put(1, 0);
		 hxf.put(2, 1);
		 hy=cvCreateMat(3,1,CV_32F);
		 FloatPointer hyf=hy.data_fl();
		 hyf.put(0, 1);
		 hyf.put(1, 0);
		 hyf.put(2, -1);
		
		  grad_xr = cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F, 3);
		  grad_yu = cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F, 3);
			
		 
		  magnitude=cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F,3);
		  orientation=cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F,3);
		  magnitude1=cvCreateImage(cvSize(C,L),IPL_DEPTH_32F,1);
		  orientation1=cvCreateImage(cvSize(C,L),IPL_DEPTH_32F,1);
		  I1=cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F,1);
		  I2=cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F,1);
		  I3=cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F,1);
		  I4=cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F,1);
		   I5=cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F,1);
		   I6=cvCreateImage(cvGetSize(Im),IPL_DEPTH_32F,1);
		  
		  first=1;
		 }
                 /** creating floating point image **/
		 cvConvertScale(Im,Im1,1.0,0.0); 
		 //start= System.currentTimeMillis();
                 
                 /** computing gradient along x and y directions **/
		 cvFilter2D(Im1, grad_xr,hx,cvPoint(1,0));
		 cvFilter2D(Im1, grad_yu,hy,cvPoint(-1,-1));
                 
		 //end = System.currentTimeMillis();
		 //time = end - start;
		// Log.e("EE","time taken to copute derivatives is "+time+"clock sec");
		 //convert from cartesian to polar co-ordinates
		 //start= System.currentTimeMillis();
                 
                 /** cartesian to polar transformation **/
		 cvCartToPolar(grad_xr, grad_yu, magnitude, orientation,0);
                 /** normalization of orientation **/
		 cvSubS(orientation,cvScalar(pi,pi,pi,0),orientation,null);
		 //end = System.currentTimeMillis();
		 //time = end - start;
		// Log.e("EE","time taken to convert to polar derivatives is "+time+"clock sec");
                 
                 
                 /** finding orientation of dominant gradients in all three channels **/
		 cvSplit( orientation, I1, I2, I3, null );
		 cvSplit( magnitude, I4, I5, I6, null);

		 
		 //FloatBuffer objectPointsBuf = orientation1.getFloatBuffer();
		 start= System.currentTimeMillis();
		 FloatBuffer I4b = I4.getFloatBuffer();
		 FloatBuffer I5b = I5.getFloatBuffer();
		 FloatBuffer I6b = I6.getFloatBuffer();
                 
		 FloatBuffer I1b = I1.getFloatBuffer();
		 FloatBuffer I2b = I2.getFloatBuffer();
		 FloatBuffer I3b = I3.getFloatBuffer();
                 
                   int i1=0;
                 while(i1<I4b.capacity())
                     //for(int i=0;i<orientation1.height();i++)
		 {
			 

			    {
                              //  int index=i*orientation1.height()+j;
                                
			    	float pt;
                                
			    	float pt1=I4b.get(i1);
			    	float pt2=I5b.get(i1);
			    	float pt3=I6b.get(i1);
			    	float max = pt1; 
                                //System.out.println(" :: "+pt1);
			    	if (pt2 > max) { /* if y is larger than max, assign y to max */
			            I4b.put(i1,pt2);
			            
			            I1b.put(i1,I2b.get(i1));
			        } /* end if */
			        //consider only H and S channels.
			        if (pt3 > max) { /* if z is larger than max, assign z to max */
			        	I4b.put(i1,pt3);
			        	I1b.put(i1,I3b.get(i1));

			        }
			        
			        pt=I1b.get(i1);
			        if(pt>0)
			        {

			            if(pt>pi && (pt-pi <0.001))
			                pt=0;
			            else if(pt<pi && (pt+pi<0.001))
			                pt=0;
			            else
			                pt=pt;
			            if(pt>0)
			                pt=(float) (-pt+pi);

			            pt=-pt;
			            
			        }
			        else if(pt<0)
			        {
			            if(pt>pi && (pt-pi <0.001))
			                pt=0;
			            else if(pt<pi && (pt+pi<0.001))
			                pt=0;
			            else
			                pt=pt;
			            if(pt<0)
			                pt= (pt+pi);


			        }
			        I1b.put(i1,pt);
			       
			    }
			    
			    
			 
			i1++; 
		 }
		 cvCopy(I4,magnitude1,null);
		 cvCopy(I1,orientation1,null);

                
                CvSize size=cvGetSize(magnitude1);
                
                 /** allocating histogram bins and integral images **/
                  IplImage bins[]=new IplImage[B];
                  IplImage integrals[]=new IplImage[B];
                 
                  for (int i = 0; i < B ; i++) {
                    bins[i] = cvCreateImage(cvGetSize(magnitude1), IPL_DEPTH_32F,1);
                    cvSetZero(bins[i]);
                    }
                  for (int i = 0; i < B ; i++) {
                integrals[i] = cvCreateImage(cvSize(size.width()+1,size.height()+1),IPL_DEPTH_64F,1);
                cvZero(integrals[i]);
                }


  int x, y;
  FloatBuffer ptr1b=orientation1.getFloatBuffer();
  FloatBuffer ptr2b=magnitude1.getFloatBuffer();
   FloatBuffer []ptrs=new FloatBuffer[B];
  int index=0;
  float temp_gradient, temp_magnitude;
  
  /** construct orientation images for each bin **/
  for (y = 0; y <size.height(); y++) {
                

             
              for (int i = 0; i < B ;i++){
                  ptrs[i] = bins[i].getFloatBuffer();
              }
              
              for (x = 0; x <size.width(); x++) {
                    index=x+y*size.width();
                    //ptr1b.position(y*size.width());
                    //ptr2b.position(y*size.width());
                    temp_gradient=ptr1b.get(index);
                    temp_magnitude=ptr2b.get(index);
                    
               
                  if ( temp_gradient <= -pi+((2*pi)/B)) {
                      //ptrs[0].position(y*size.width());
                      ptrs[0].put(index,temp_magnitude);
                      //ptr1[x]=100;
                  
                  }
                  else if ( temp_gradient <= -pi+((4*pi)/B)) {
                      // ptrs[1].position(y*size.width());
                      ptrs[1].put(index,temp_magnitude);
                      //ptr1[x]=100;
                  }
                  else if (temp_gradient <= -pi+((6*pi)/B)) {
                     //  ptrs[2].position(y*size.width());
                     ptrs[2].put(index,temp_magnitude);
                     // ptr1[x]=100;
                  }
                  else if (  temp_gradient <= -pi+((8*pi)/B)) {
                   //ptrs[3].position(y*size.width());
                      ptrs[3].put(index,temp_magnitude);
                     // ptr1[x]=100;
                  }
                  else if (temp_gradient <= -pi+((10*pi)/B)) {
                     //  ptrs[4].position(y*size.width());
                      ptrs[4].put(index,temp_magnitude);
                     // ptr1[x]=100;
                  }
                  else if (temp_gradient <= -pi+((12*pi)/B)) {
                    //   ptrs[5].position(y*size.width());
                      ptrs[5].put(index,temp_magnitude);
                     // ptr1[x]=100;
                  }
                  else if (temp_gradient <= -pi+((14*pi)/B)) {
                    //   ptrs[6].position(y*size.width());
                     ptrs[6].put(index,temp_magnitude);
                      //ptr1[x]=100;
                  }
                  else if (temp_gradient <= -pi+((16*pi)/B)) {
                    //   ptrs[7].position(y*size.width());
                     ptrs[7].put(index,temp_magnitude);
                      //ptr1[x]=100;
                  }
                  else if (temp_gradient <= -pi+((18*pi)/B)) {
                    //   ptrs[7].position(y*size.width());
                     ptrs[8].put(index,temp_magnitude);
                      //ptr1[x]=100;
                  }
                  else if (temp_gradient <= -pi+((20*pi)/B)) {
                    //   ptrs[7].position(y*size.width());
                     ptrs[9].put(index,temp_magnitude);
                      //ptr1[x]=100;
                  }                                    
                  else if (temp_gradient <= -pi+((22*pi)/B)) {
                    //   ptrs[7].position(y*size.width());
                     ptrs[10].put(index,temp_magnitude);
                      //ptr1[x]=100;
                  }                  
                  else {
                    //   ptrs[8].position(y*size.width());
                     ptrs[11].put(index,temp_magnitude);
                      //ptr1[x]=100;
                  }
                  //index=index+1;

              }
  }
                
                  /** compute integral image for each orientation image **/
                  for (int i = 0; i <B ; i++){
                    cvIntegral(bins[i], integrals[i],null,null);
                    //Log.e("EE","EE ---->"+integrals[i].getDoubleBuffer().get(1000));
                    }

                        for (int i = 0; i <B ; i++){
                        cvReleaseImage(bins[i]);
                        }
  
                        
 
                CvMat vector_block=cvCreateMat(1,B,CV_32FC1);
                int startcol = 0;

                /** calculate HOG over input windows **/
		 for(int n=0;n<nwin_x;n++)
		 {
		     for(int m=0;m<nwin_y;m++)
		     {
		   
		//cvGetCols(window_feature_vector, vector_block,startcol, startcol + B);
                        vector_block=calculateHOG_rect(cvRect((m*step_x),(n*step_y),2*step_x,2*step_y), integrals, 4);
                
		 for(int j=0;j<vector_block.cols();j++)
                 {
                    //Log.e("EEA "+j,","+(float)vector_block.get(j));
                    descriptors.put(startcol+j,(float)vector_block.get(j));

                 }      
                  startcol += B;  
		     }
		 
		 
		     }

	
	 }
        

         /** function to calculate HOG of image **/
       CvMat calculateHOG_rect(CvRect cell,IplImage integrals[], int normalization) {

     CvMat hog_cell=cvCreateMat(1,B,CV_32FC1);;

     for (int i = 0; i < B ; i++){
         
         IplImage a1=integrals[i];
         //Log.e("EE","EE ---->"+integrals[i].getDoubleBuffer().get(1000));
         DoubleBuffer da1=integrals[i].getDoubleBuffer();
         //da1.position(cell.y()*a1.width());
         double a =da1.get((cell.y()+0)*a1.width()+cell.x());
         
         
         //da1.position((cell.y()+cell.height())*a1.width());
         double b = da1.get((cell.y()+cell.height())*a1.width()+cell.x()+cell.width());
         //Log.e("EE",""+(cell.y()+cell.height())+";;;"+a1.width());
      
        // da1.position(cell.y()*a1.width());
         double c = da1.get((cell.y()+0)*a1.width()+cell.x()+cell.width());

          //da1.position((cell.y()+cell.height())*a1.width());
          double d = da1.get((cell.y()+cell.height())*a1.width()+cell.x()+0);
          
         /** obtain the histogram count for the block **/
          double f=(float)((a+b)-(c+d));
          hog_cell.put(i, f);
          //hog_cell->data.fl)[i] = (a+b)-(c+d);
     }
         
         cvNormalize(hog_cell, hog_cell, 1, 0, 4,null);
         return hog_cell;
       }
         
}
