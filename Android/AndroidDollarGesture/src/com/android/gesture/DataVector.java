/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.android.gesture;


import java.util.Vector;


import com.android.gesture.Point;
import com.android.gesture.Result;

import java.util.Vector;



/**
 * compatible with android and desktop
 * @author pi19404
 */
//class to store the template points and time of capture of points
public class DataVector {
        //data type to store the relative time information of captured points        
        Vector<Double> rf_curTime = new Vector<Double>();
        //data type to store the 2D co-ordinates of captured points
        public Vector points = new Vector();
        
    	//adds co-ordinate information to 2D co-ordinate list
        public void addPoint(int x, int y)
        {
        	points.addElement(new Point(x, y));
	
        }	

        //adds co-ordinate and time information to 2D co-ordinate list
        public void addPoint(int x, int y,double curTime)
        {           
        	rf_curTime.addElement(curTime);
        	points.addElement(new Point(x, y));
		
        }
	

	
	//clears information about the gestures stored previously
	public void pointerPressed(int x, int y)
	{
		clear();
		addPoint(x, y);
	}
		
	//adds points to template
	public void pointerDragged(int x, int y)
	{
		addPoint(x, y);
	}
	
	
	//returns a vector of stored points
    public Vector getPoints()
    {
           return points;
     }
        
        public Vector<Double> get_time() {
		return rf_curTime;
	}

    //clear information about previously store poits
	public void clear()
	{
		points.removeAllElements();
		rf_curTime.removeAllElements();	
	}
	
}


