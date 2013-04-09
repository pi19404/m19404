package com.android.gesture;

import java.io.Serializable;

public class Point implements Serializable
{
	public double x, y;
	public Point()
        {
            x=0;y=0;
        }
        
        public double getX()
        {
            return x;
        }
        
        public double getY()
        {
            return y;
        }
                
	public Point(double x, double y)
	{	this.x = x; this.y = y;	}
	
	public void copy(Point src)
	{
		x = src.x;
		y = src.y;
	}	
}
