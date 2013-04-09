/*compatible
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.android.gesture;

import static com.googlecode.javacv.cpp.opencv_core.CV_RGB;
import static com.googlecode.javacv.cpp.opencv_core.cvCreateImage;
import static com.googlecode.javacv.cpp.opencv_core.cvLine;
import static com.googlecode.javacv.cpp.opencv_core.cvPoint;
import static com.googlecode.javacv.cpp.opencv_core.cvResetImageROI;
import static com.googlecode.javacv.cpp.opencv_core.cvSetImageROI;
import static com.googlecode.javacv.cpp.opencv_core.cvSize;
import Settings.Settings;
import au.com.bytecode.opencsv.CSVReader;
import java.util.Vector;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import com.android.gesture.*;
import com.android.main.Log;
import com.android.opencv.Classifier;
import com.googlecode.javacv.cpp.opencv_core.CvPoint;
import com.googlecode.javacv.cpp.opencv_core.CvRect;
import com.googlecode.javacv.cpp.opencv_core.IplImage;

import com.googlecode.javacv.cpp.opencv_core.IplImage;
import java.util.Vector;
import com.googlecode.javacv.cpp.opencv_core.*;
import com.googlecode.javacv.cpp.opencv_imgproc.*;
import static com.googlecode.javacv.cpp.opencv_imgproc.*;

import com.googlecode.javacpp.*;
import com.googlecode.javacv.cpp.opencv_video.*;
import static com.googlecode.javacv.cpp.opencv_video.*;
import com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_imgproc.*;
import  com.googlecode.javacv.cpp.opencv_imgproc.*;
import static com.googlecode.javacpp.Loader.*;
import static com.googlecode.javacv.cpp.opencv_video.*;
import com.googlecode.javacv.cpp.opencv_video.*;
import static com.googlecode.javacv.cpp.opencv_highgui.cvSaveImage;
import com.googlecode.javacv.cpp.opencv_legacy.*;
import static com.googlecode.javacv.cpp.opencv_legacy.*;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.List;




import java.io.File;

/**
 *
 * @author pi19404
 */
//class to capture the candidate template points,pre process points and call the gesture
//recognizer methods
public class ShapeDataCapture {
	//boolean variable indicating if gesture has started or not
    public boolean drawingStarted=false;
    
    public Classifier classifier;
    
    public IplImage cimage;
    //class to store set of data points
    public DataVector candidate=new DataVector();

    
    public int WIDTH=320;
    public int HEIGHT=240;
    
    
    //class containing methods for $1 gesture recognizer
    public PUtils dollar2 ;
              
	String name = "";
	public double score = 0;
	boolean ok = false;;
	
   //settings class contains path where the templates are stored
   Settings set;
   //variable to indicate gesture state start,dragged,released(0,1,2)
   int state;
   
   public ShapeDataCapture()
   {
	   
   }
   //constructor for the data capture class
   public ShapeDataCapture(Settings set) throws Exception
   {
        //initialize dollar template matching
	    
        dollar2=new PUtils(set.TEMPLATE_DIR);
        dollar2.setActive(true);
        try {
			classifier=new Classifier(set);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.e("EE","exception in creating classifier"+e.toString());
			throw e;			
		}
        Log.e("EE","complete initialization of data capture panel");
   }
   
   public boolean save_image=false;
   int k=0;
   
   
   Rectangle B;

   

   
   List<String[]> list;
   
   public IplImage tmp_image;
   public IplImage cimage1;
    //method provided for pointer dragged function which adds the co-ordinate and time
    //to candidate template list
	public void pointerDragged(int x, int y)
	{                                  
		candidate.addPoint(x, y,System.currentTimeMillis());
	}
        
    //method provided to be called when the gesture has started,previous candidate points are clared
	public void pointerPressed(int x, int y)
	{
          
		candidate.clear();
		candidate.addPoint(x, y,System.currentTimeMillis());
	}
	
	//method to be called when the gesture has terminated
	public void pointerReleased(int x, int y)
	{
		

        
        Point p1,p2;
        

        
			//compute the path length of raw features
			
            double L=dollar2.PathLength(candidate.getPoints());
            
            //minimum thresholds for path length is defined as 100 to ignore invalid gestures
            if(L>100)
            {            	  
            	
            	Vector<Point> points=candidate.getPoints();
            	
            	int f=10;
            	tmp_image=cvCreateImage(cvSize(WIDTH,HEIGHT),8,4);
                if(cimage==null )
                    cimage=cvCreateImage(cvSize(32*f,24*f),8,4);
            	
                
            	//B = dollar2.BoundingBox(points);
            	//dollar2.points=points;
                points=dollar2.Resample(points,PUtils.NumPoints);
                points = PUtils.ScaleToSquare1(points,tmp_image.width()/2,tmp_image.height()/2);
                points = dollar2.TranslateToOrigin(points);
                points = PUtils.TranslateToOrigin1(points,(int)tmp_image.width()/2,(int)tmp_image.height()/2);
                B = PUtils.BoundingBox(points);
                
                Log.e("EE","xxx"+B.Width+":"+B.Height+":"+WIDTH+":"+HEIGHT);
                CvRect rect1=new CvRect((int)B.X-4,(int)B.Y-4,(int)B.Width+8,(int)B.Height+8);
                
                
                
                
                cvZero(tmp_image);
                cvZero(cimage);
                String s=new String();
                for (int i = 0; i < points.size()-1; i++)
                {

             	   	p1 = (Point)points.elementAt(i);
             	   	p2 = (Point)points.elementAt(i+1);
             	  // 	s=s+"-"+(int)p1.x+":"+(int)p1.y;
                    CvPoint po=cvPoint((int)p1.x,(int)p1.y);
                    CvPoint po1=cvPoint((int)p2.x,(int)p2.y);
                    //cvCircle(tmp_image,po1,3,CV_RGB(255,255,255),3,8,0);
                    cvLine(tmp_image,po1,po,CV_RGB(0,255,0),3,8,0);    
                }
                //Log.e("EE",s);
                cvSetImageROI(tmp_image,rect1);
                cvResize(tmp_image,cimage);
                cvResetImageROI(tmp_image);

                cimage1=cvCreateImage(cvSize(32*f,24*f),8,3);
                cvCvtColor(cimage,cimage1,CV_BGRA2BGR);
                   try {
                	   	
                	   classifier.extract_feature(cimage1);
                	   if(classifier.probx>0.5)
                	   {
                		   boolean flag=false;
                		   if(classifier.ll==1 && classifier.probx>0.85)
                			   flag=true;
                		   if(classifier.ll==2 && classifier.probx>0.6)
                			   flag=true;
                		   if(classifier.ll==3 && classifier.probx>0.85)
                			   flag=true;
                		   if(classifier.ll==10 && classifier.probx>0.6)
                			   flag=true;
                		   if(classifier.ll==9 && classifier.probx>0.6)
                			   flag=true;
                		   
                		   if(flag==true)
                			   dollar2.result=new Result("Gesture"+classifier.ll,classifier.probx,-1);
                		   else
                			   dollar2.result=new Result("no gesture"+classifier.ll,classifier.probx,-1);
                	   }
                	   else
                	   {
                		   dollar2.result=new Result("no gesture", 0, -1);
                	   }
                	        
                
                                                       
                   } catch (Exception ex) {
          Log.e("SEVER EXCEPTION",ex.toString());
        }
                   
            }
         
	}
        
        
        
        
        String recGest="no gesture";
        public String getName()
        {
            return recGest;
        }
        
    //flag indicates whether to capture data points or not
    //flag set to false call pointer released
    public void On(boolean flag,int x,int y)
    {
    	
        if (drawingStarted==true&&flag==true) {
        	
        state = 2;
        pointerDragged(x,y);	        
        }        
        else if(drawingStarted==false && flag==true )
        {        
        	
        drawingStarted = true;
        state = 1;
        dollar2.result = new Result("no gesture", 0, -1);
        pointerPressed(x,y);	
        }        
        else if(drawingStarted==true && flag==false)
        {
            drawingStarted = false;
            state = 0;            
            pointerReleased(x,y);
            

        }
        
        
    }
        public boolean last=false;
        
	
        
	
	
 
   
    
    
}
