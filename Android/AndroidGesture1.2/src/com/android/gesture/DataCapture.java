/*compatible
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.android.gesture;

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



import java.io.File;

/**
 *
 * @author pi19404
 */
//class to capture the candidate template points,pre process points and call the gesture
//recognizer methods
public class DataCapture {
	//boolean variable indicating if gesture has started or not
    public boolean drawingStarted=false;
    
    //class to store set of data points
    public DataVector candidate=new DataVector();

    
    int WIDTH=320;
    int HEIGHT=240;
    
    
    //class containing methods for $1 gesture recognizer
    public PUtils dollar2 ;
              
	String name = "";
	public double score = 0;
	boolean ok = false;;
	
   //settings class contains path where the templates are stored
   Settings set;
   //variable to indicate gesture state start,dragged,released(0,1,2)
   int state;
   
   public DataCapture()
   {
	   
   }
   //constructor for the data capture class
   public DataCapture(Settings set)
   {
        //initialize dollar template matching
	    
        dollar2=new PUtils(set.TEMPLATE_DIR);
        dollar2.setActive(true);
   }
   
   public boolean save_image=false;
   int k=0;
   
   
   Rectangle B;

   

   
   List<String[]> list;
   
   
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
                    
			//compute the path length of raw features
			
            double L=dollar2.PathLength(candidate.getPoints());
            
            //minimum thresholds for path length is defined as 100 to ignore invalid gestures
            if(L>100)
            {            	  
                   try {
                	   	
                		    dollar2.points=candidate.getPoints();
                			
                	        dollar2.pointerReleased(x,y);
                
                                                       
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
