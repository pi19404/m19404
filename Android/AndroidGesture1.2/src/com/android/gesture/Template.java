package com.android.gesture;
import java.util.Vector;

import com.android.main.Log;


//class which holds the template points
public class Template
{
	String Name;
	Vector Points;
	PUtils utils=new PUtils();
	Template(String name, Vector points1) 
	{
        Vector points=new Vector(points1.size());
        for(int k=0;k<points1.size();k++)
        {
            points.addElement(points1.elementAt(k));
        }

        
		this.Name = name;		
		
		points= utils.Resample(points, PUtils.NumPoints);			
        //apply pre processing on template
        points= utils.ScaleDimTo(points, PUtils.SquareSize, utils.OneDThreshold);        
        points =utils.TranslateToOrigin(points);        
        

        
		this.Points = points;
	}
        
    public Vector getPoints()
    {
            return Points;
    }
}
