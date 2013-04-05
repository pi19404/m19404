//
package com.android.gesture;


import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Enumeration;
import java.util.Vector;

import com.android.main.Log;


/** This class performs contains functions for template matching.
 *   contain methods for template normalization and comparision.
 *   compatible with both android and desktop
 * @author pi19404
 */
public class PUtils {
    

   	int NumTemplates = 32;
	
   	//number of resampled points
   	public static int NumPoints = 16;

    // customize to desired gesture set (usually 0.20 - 0.35)
    double OneDThreshold = 0.25; 
   	//scaling size
   	public static double SquareSize = 250.0;
   	
    double StartAngleIndex = (NumPoints / 8); // eighth of gesture length
    //andgle similarity thresholds
    double AngleSimilarityThreshold = Deg2Rad(30.0);
    
	//binding box for points
	public Rectangle boundingBox = new Rectangle(0, 0, 0, 0);

	//used to normalize scores
	double HalfDiagonal = 0.5 * Math.sqrt(SquareSize*SquareSize+SquareSize*SquareSize);

	public Point centroid = new Point(0, 0);
	
	double AngleRange = 15.0;
	double AnglePrecision = 2.0;
	
	public static double Phi = 0.5 * (-1.0 + Math.sqrt(5.0)); // Golden Ratio

	
	int bounds[] = { 0, 0, 0, 0 };
	
	//name of template directory
	String name;
    String test;


    public PUtils()
    {
    	
    }
    
    //utility functions
    public double Round(double n,int d) { 
        double d1 = Math.pow(10,d); 
        return Math.round(n*d1)/d1; 
    } // round 'n' to 'd' decimals
    double Deg2Rad(double d) { return (d * Math.PI / 180.0); }
    double Rad2Deg(double r) { return (r * 180.0 / Math.PI); }

    
	//method to compute the path length
	public  double PathLength(Vector points)
	{
		double length = 0;
		for (int i = 1; i < points.size(); i++)
		{	
			length += Distance((Point) points.elementAt(i - 1), (Point) points.elementAt(i));
		}
		return length;
	}

	//method to compute the distance between two points
	public  double Distance(Point p1, Point p2)
	{
		double dx = p2.x - p1.x;
		double dy = p2.y - p1.y;
		return Math.sqrt(dx * dx + dy * dy);
	}
	
    //method to resample the set of points 
	public  Vector Resample(Vector points, int n)
	{		
		//compute the interval length
		double I = PathLength(points) / (n - 1); // interval length
		double D = 0.0;
		
		//make a copy of vector of data points
		Vector srcPts = new Vector(points.size());
		for (int i = 0; i < points.size(); i++)
			srcPts.addElement(points.elementAt(i));
		
		//allocate a vector for resampled points
		Vector dstPts = new Vector(n);
		dstPts.addElement(srcPts.elementAt(0));	
		
		for (int i = 1; i < srcPts.size(); i++)
		{
			Point pt1 = (Point) srcPts.elementAt(i - 1);
			Point pt2 = (Point) srcPts.elementAt(i);

			double d = Distance(pt1, pt2);
			if ((D + d) >= I)
			{
				double qx = pt1.x + (I  / (D+d)) * (pt2.x - pt1.x);
				double qy = pt1.y + (I / (D+d)) * (pt2.y - pt1.y);
				Point q = new Point(qx, qy);
				dstPts.addElement(q); 
				srcPts.insertElementAt(q, i); 
				D = 0.0;
			}
			else
			{
				D += d;
			}
		}
		// somtimes we fall a rounding-error short of adding the last point, so add it if so
		if (dstPts.size() == n - 1)
		{
			dstPts.addElement(srcPts.elementAt(srcPts.size() - 1));
		}
		
		return dstPts;
	}
	

	
	// computes the 'distance' between two point paths by summing their corresponding point distances.
	// assumes that each path has been resampled to the same number of points at the same distance apart.
	public  double PathDistance(Vector path1, Vector path2)
	{            		
		double distance = 0;
		for (int i = 0; i < path1.size(); i++)
		{
			distance += Distance((Point) path1.elementAt(i), (Point) path2.elementAt(i));
		}
		return distance / path1.size();
	}

	//method to compute the bounding box.
	public static Rectangle BoundingBox(Vector points)
	{
		double minX = Double.MAX_VALUE;
		double maxX = Double.MIN_VALUE;
		double minY = Double.MAX_VALUE;
		double maxY = Double.MIN_VALUE;
	
		Enumeration e = points.elements();
		
//		foreach (Point p in points)
		while (e.hasMoreElements())
		{
			Point p = (Point)e.nextElement();
		
			if (p.x < minX)
				minX = p.x;
			if (p.x > maxX)
				maxX = p.x;
		
			if (p.y < minY)
				minY = p.y;
			if (p.y > maxY)
				maxY = p.y;
		}
	
		return new Rectangle(minX, minY, maxX - minX, maxY - minY);
	}
	
	//method to compute the bounding box.
	public static void BoundingBox(Vector points, Rectangle dst)
	{
		double minX = Double.MAX_VALUE;
		double maxX = Double.MIN_VALUE;
		double minY = Double.MAX_VALUE;
		double maxY = Double.MIN_VALUE;
	
		Enumeration e = points.elements();
		
//		foreach (Point p in points)
		while (e.hasMoreElements())
		{
			Point p = (Point)e.nextElement();
		
			if (p.x < minX)
				minX = p.x;
			if (p.x > maxX)
				maxX = p.x;
		
			if (p.y < minY)
				minY = p.y;
			if (p.y > maxY)
				maxY = p.y;
		}
	
		dst.X = minX;
		dst.Y = minY;
		dst.Width = maxX - minX;
		dst.Height = maxY - minY;
	}	
    
//another method of scaling
public Vector ScaleDimTo(Vector points, double size, double oneDratio) // scales bbox uniformly for 1D, non-uniformly for 2D
{
Rectangle B = BoundingBox(points);
boolean uniformly = Math.min(B.Width / B.Height, B.Height / B.Width) <= oneDratio; // 1D or 2D gesture test
Vector newPoints = new Vector(points.size());
for (int i = 0; i < points.size(); i++)
{
	Point i1=(Point)points.elementAt(i);
	double qx = uniformly ? i1.x * (size / Math.max(B.Width, B.Height)) : i1.x * (size / B.Width);
	double qy = uniformly ? i1.y * (size / Math.max(B.Width, B.Height)) : i1.y * (size / B.Height);
    newPoints.addElement(new Point(qx, qy));
	
}
return newPoints;
}
    
    //method to scale the co-ordinate locations to specified bounds.
	public static Vector ScaleToSquare1(Vector points, double size,double size1)
	{
	return ScaleToSquare1(points, size,size1, null);
	}				

	public static Vector ScaleToSquare1(Vector points, double size,double size1, Rectangle boundingBox)
	{
	Rectangle B = BoundingBox(points);
	Vector newpoints = new Vector(points.size());
	for (int i = 0; i < points.size(); i++)
	{
		Point p = (Point)points.elementAt(i);
		double qx = p.x * (size / B.Width);
		double qy = p.y * (size1 / B.Height);
		newpoints.addElement(new Point(qx, qy));
	}

	if (boundingBox != null) //this will probably not be used as we are more interested in the pre-rotated bounding box -> see RotateToZero
		boundingBox.copy(B);

	return newpoints;
	}	

	
        
	//method to compute the centroid of the points given
	public static Point Centroid(Vector points)
	{
		double xsum = 0.0;
		double ysum = 0.0;
		
		Enumeration e = points.elements();
		
		while (e.hasMoreElements())
		{
			Point p = (Point)e.nextElement();
			xsum += p.x;
			ysum += p.y;
		}
		return new Point(xsum / points.size(), ysum / points.size());
	}
        
	//mthod to translate the centroid of set of points to origin
	public  Vector TranslateToOrigin(Vector points)
	{
		Point c = Centroid(points);
		Vector newpoints = new Vector(points.size());
		for (int i = 0; i < points.size(); i++)
		{
			Point p = (Point)points.elementAt(i);
			double qx = p.x - c.x;
			double qy = p.y - c.y;
			newpoints.addElement(new Point(qx, qy));
		}
		return newpoints;
	}	


	public static Vector RotateToZero(Vector points)
	{	
		return RotateToZero(points, null, null);	
    }

	
	public static Vector RotateToZero(Vector points, Point centroid, Rectangle boundingBox)
	{
		Point c = Centroid(points);
		Point first = (Point)points.elementAt(0);
		//angle between first element and centrood
		double theta = Math.atan2(c.y - first.y, c.x - first.x);
		
		if (centroid != null)
			centroid.copy(c);
		
		if (boundingBox != null)
			BoundingBox(points, boundingBox);
		//rotate points such that this line at 0 deg
		return RotateBy(points, -theta);
	}		
	
	public static Vector RotateBy(Vector points, double theta)
	{
		return RotateByRadians(points, theta);
	}
	
	// rotate the points by the given radians about their centroid
	public static Vector RotateByRadians(Vector points, double radians)
	{
		Vector newPoints = new Vector(points.size());
		Point c = Centroid(points);

		double _cos = Math.cos(radians);
		double _sin = Math.sin(radians);

		double cx = c.x;
		double cy = c.y;

		for (int i = 0; i < points.size(); i++)
		{
			Point p = (Point) points.elementAt(i);

			double dx = p.x - cx;
			double dy = p.y - cy;

			newPoints.addElement(
				new Point(	(dx * _cos) - (dy * _sin) + cx, 
							(dx * _sin) + (dy * _cos) + cy )
							);
		}
		return newPoints;
	}

	
	public Point CalcStartUnitVector(Vector points, int index) // start angle from points[0] to points[index] normalized as a unit vector
	{
	    Point i1=(Point)points.elementAt(index);
	    Point i2=(Point)points.elementAt(0);
		Point v = new Point(i1.x - i2.x, i1.y - i2.y);
		double len = Math.sqrt(v.x * v.x + v.y * v.y);
		return new Point(v.x / len, v.y / len);
	}

	public double AngleBetweenUnitVectors(Point v1, Point v2) // gives acute angle between unit vectors from (0,0) to v1, and (0,0) to v2
	{
		double n = (v1.x * v2.x + v1.y * v2.y);
		if (n < -1.0 || n > +1.0)
			n = Round(n, 5); // fix: JavaScript rounding bug that can occur so that -1 <= n <= +1
		return Math.acos(n); // arc cosine of the vector dot product
	}        
 

    public Result result = new Result("no gesture", 0, -1);
	
	protected boolean active = false;
        
        protected int x, y;
	protected int state;
		
	protected int _key = -1;
	
	protected boolean gesture = true; 
	public Vector points = new Vector(1000);
        
        
    
    
        
Vector Templates = new Vector(NumTemplates);


//method to read contents of csv template files into integer array
public Vector CSVRead_train(String fname)         
{
    try{        
     
    	Vector points=new Vector();
    	BufferedReader CSVFile = new BufferedReader(new FileReader(fname));

    	String dataRow = CSVFile.readLine(); 
    	String[] dataArray = null;
    	while (dataRow != null){
      
    		dataArray = dataRow.split(",");
    		dataRow = CSVFile.readLine(); // Read next line of data.
    	}
         String rr=new String();
        int d[]=new int[dataArray.length];
        for (int k = 0; k < dataArray.length;k=k+2)
		{
                     
                       d[k]=(int)Integer.parseInt(dataArray[k]);
                
                       d[k+1]=(int)Integer.parseInt(dataArray[k+1]);
                       Point a=new Point();
                       a.x=(int)d[k];
                       a.y=(int)d[k+1];
                       rr=rr+":"+a.x+":"+a.y;
                       points.add(a);
		}

                    //System.out.println(""+":"+rr);
                    return points;
    
    }
    catch(Exception e)
    {
        return null;
    }
}

    
	//method to read contents of csv file into a string array
    public  String[] CSVRead(String file ) throws Exception {

  BufferedReader CSVFile = new BufferedReader(new FileReader(file));

  String dataRow = CSVFile.readLine(); 
  String[] dataArray = null;
  while (dataRow != null){
   dataArray = dataRow.split(",");
dataRow = CSVFile.readLine(); // Read next line of data.

  }
  // Close the file once all data has been read.
  CSVFile.close();

  // End the printout with a blank line.
  return dataArray;

 } //main()
 // CSVRead
         

    //method to read the template csv files ,directory name is template name
public  void dirlist1(String name)
        {
			
            try{
                
                   
            File dir = new File(name);
            
            String[] chld = dir.list();
            for(int i=0;i<chld.length;i++)
            {
                 
                File x=new File(dir.getAbsolutePath()+"/"+chld[i]);
                if(x.isFile() && x.getName().substring(x.getName().lastIndexOf(".")).equals(".csv"))
                {
                 String data[]=CSVRead(dir.getAbsolutePath()+"/"+chld[i]);                  
                 int d[]=new int[data.length];
                 for (int k = 0; k < data.length;k++)
                 {
                       d[k]=Integer.parseInt(data[k]);
			
                 }                 
                 Templates.addElement(loadTemplate(dir.getName(),d));
                }
                if(x.isDirectory())
                {                  
                    dirlist(dir.getAbsolutePath()+"/"+chld[i]);
                
                }
            }
            }
            catch(Exception e)
            {
                
            }
        }
	   
      public void copyReversed(int dst[], int src[])
	{		
		for (int i = 0; i < src.length/2; i++)
		{
			dst[ i*2 ] = src[src.length - i*2 - 2];  
			dst[ i*2 + 1 ] = src[src.length - i*2 + 1 - 2];  
	
		}
	
	}
                
	public  void dirlist(String name)
        {
            try{
                
            	
            File dir = new File(name);
            
            String[] chld = dir.list();
            for(int i=0;i<chld.length;i++)
            {
                 
            	
                File x=new File(dir.getAbsolutePath()+"/"+chld[i]);

                if(x.isFile() && x.getName().substring(x.getName().lastIndexOf(".")).equals(".csv"))
                {
         
                  String data[]=CSVRead(dir.getAbsolutePath()+"/"+chld[i]);
         
                 int d[]=new int[data.length];
                 for (int k = 0; k < data.length;k++)
                 {
                       d[k]=(int)Double.parseDouble(data[k]);
			
                 }
                  
                 Templates.addElement(loadTemplate(dir.getName(), d));
                }
                if(x.isDirectory())
                {
                    dirlist(dir.getAbsolutePath()+"/"+chld[i]);
                
                }
            }
            }
            catch(Exception e)
            {
                
            }
        }

	
	//constructor for putils class
    public  PUtils(String dir)
    {
        
        
        name=dir+"/"+"data";
        test=dir+"/"+"testdata";
        //loadTemplatesDefault(); 
        dirlist(name);
    }
        
        
    	//get the size of template list
        public int getTemplateSize()
        {
            return Templates.size();
        }
        
        //get the template name 
        public String getTemplateName(int i)
        {
            return ((Template)Templates.elementAt(i)).Name;
        }
        //get the template at specifid index
        public Template getTemplate(int i)
        {
            return ((Template)Templates.elementAt(i));
        } 
      
        //load the template from specified integer array
        Template loadTemplate(String name, int[] array)
        {
        	return new Template(name, loadArray(array));
        }
 
        Vector loadArray(int[] array)
        {
        	Vector v = new Vector(array.length/2);
        	for (int i = 0; i < array.length; i+= 2)
        	{
        		Point p = new Point(array[i], array[i+1]);
        		v.addElement(p);
        	}
		return v;
	}
	
    //add the template to template list
 	int AddTemplate(String name, Vector points)
	{
		Templates.addElement(new Template(name, points));
		return Templates.size();
	}
	
	int DeleteUserTemplates()
	{
		for (int i = Templates.size()-NumTemplates; i > 0; i--)
		{
			Templates.removeElementAt(Templates.size()-1);
		}
		
		return Templates.size();
	}
        
    public static Vector ScaleToSquare(Vector points, double size)
	{
		return ScaleToSquare(points, size, null);
	}
    
	
        
	            
	public static Vector ScaleToSquare(Vector points, double size, Rectangle boundingBox)
	{
		Rectangle B = BoundingBox(points);
		Vector newpoints = new Vector(points.size());
		for (int i = 0; i < points.size(); i++)
		{
			Point p = (Point)points.elementAt(i);
			double qx = p.x * (size / B.Width);
			double qy = p.y * (size / B.Height);
			newpoints.addElement(new Point(qx, qy));
		}
		
		if (boundingBox != null) //this will probably not be used as we are more interested in the pre-rotated bounding box -> see RotateToZero
			boundingBox.copy(B);
		
		return newpoints;
	}
	
               

	//method which implements recognising routine.
public Result Recognize()
	{
  double a2=0;
  double length1=0;
  double length=0;
  
  
  //atleast NumPoints should be present for match
  if (points.isEmpty() || points.size()<10) 
  return new Result("No match.", 0.0,0);
        
  Vector pointsx,pointsx1;
  
  //resampling the candidate points
  
  pointsx = Resample(points, NumPoints);		
  
  
  Point StartUnitVector=null;
  Point startv=null ;
  
  double radians = IndicativeAngle(pointsx);
  
  //rescaling the candidate points
  pointsx= ScaleDimTo(pointsx, SquareSize, OneDThreshold);
  //translating centroid to origin
  pointsx =TranslateToOrigin(pointsx);
  //computing start unit vector
  StartUnitVector = CalcStartUnitVector(pointsx,(int)StartAngleIndex);
  //computing the path length
  length=PathLength(pointsx);
                
                                                
  double best_score = Double.POSITIVE_INFINITY; // best score
  int best_index = -1; // best index
 
  double tmp_score = Double.POSITIVE_INFINITY; // best score
  double temp_index=Double.POSITIVE_INFINITY;
		
  double d1=Double.POSITIVE_INFINITY;
  int s=0;
       
		for (int i = 0; i < Templates.size(); i++) // for each point-cloud template
		{
                    Template x=(Template)Templates.elementAt(i);
                    
                    Vector points1=new Vector(x.Points.size());
                    for(int k=0;k<x.Points.size();k++)
                    {
                        points1.addElement(x.Points.elementAt(k));
                    }
                    
                    startv = CalcStartUnitVector(points1,(int)StartAngleIndex);
                    //a2=PathLength(points1);
                    
                    if(AngleBetweenUnitVectors(startv, StartUnitVector) <= AngleSimilarityThreshold)
                    {
                        tmp_score = DistanceAtBestAngle(pointsx,new Template(((Template)Templates.elementAt(i)).Name,points1), -AngleRange, AngleRange, AnglePrecision);
                    
                        
                    }
                    else
                    {
                                tmp_score=Double.POSITIVE_INFINITY;
                    }
                    //Log.e("EE",x.Name+":"+tmp_score);
                    //recording the best score
                    
                    if (tmp_score < best_score)
                    {
                                length1=PathLength(points1);//length of best temlate
                                best_score = tmp_score; // new best distance
                                best_index = i; // template index
                                //b1=d1;
                    }
		}
                
                double score = 1 ;
                 Template x=null;
                if(best_index!=-1)
                {
                x=(Template)Templates.elementAt(best_index);                
                }
                double a1=0;
                score = 1.0 - (best_score*HalfDiagonal/(length*length1) );
                     
                
                return  (best_index == -1) ? new Result("No match.", 0.0,best_index) : new Result(((Template)Templates.elementAt(best_index)).Name, score, best_index);
                
               
	}

	//golden section scan algorithm
 	public  double DistanceAtBestAngle(Vector points, Template T, double a, double b, double threshold)
	{
		//double Phi = Recognizer.Phi;
 		double Phix = 0.5 * (-1.0 + Math.sqrt(5.0)); // Golden Ratio
		double x1 = Phi * a + (1.0 - Phi) * b;
		double f1 = DistanceAtAngle(points, T, x1);
		double x2 = (1.0 - Phi) * a + Phi * b;
		double f2 = DistanceAtAngle(points, T, x2);
		
		while (Math.abs(b - a) > threshold)
		{
			if (f1 < f2)
			{
				b = x2;
				x2 = x1;
				f2 = f1;
				x1 = Phix * a + (1.0 - Phix) * b;
				f1 = DistanceAtAngle(points, T, x1);
			}
			else
			{
				a = x1;
				x1 = x2;
				f1 = f2;
				x2 = (1.0 - Phix) * a + Phix * b;
				f2 = DistanceAtAngle(points, T, x2);
			}
		}
		return Math.min(f1, f2);
	}
        
   	public  double DistanceAtAngle(Vector points, Template T, double theta)
	{
		Vector newpoints = RotateBy(points, theta);
		return PathDistance(newpoints, T.Points);        
	}		

        
        	
	public void clear()
	{
		points.removeAllElements();
		result.Name = "";
		result.Score = 0;
		result.Index = -1;
	}
        

	

	
	public String getName()
	{		
		return result.Name;
	}
	
	public double getScore()
	{
		return result.Score;
	}

	public int getIndex()
	{
		return result.Index;
	}

	public void setActive(boolean state)
	{
		active = state;
	}
	
	public boolean getActive()
	{
		return active;
	}	
	
	public void pointerPressed(int x, int y)
	{
		clear();
	}

   	public void pointerDragged(int x, int y)
	{
		addPoint(x, y);
	}
   	
	public void pointerReleased(int x, int y)
	{
		
        //        points.addElement(new Point(x, y));
                result=Recognize();
                int t=points.size();

	}
        
	public void addPoint(int x, int y)
	{	
		points.addElement(new Point(x, y));
	}
        
public double OptimalCosineDistance(Vector v1,Vector v2) // for Protractor
{
	double a = 0.0;
	double b = 0.0;
	for (int i = 0; i < v1.size(); i =i+1)
	{
                Point v11=(Point)v1.elementAt(i);
                Point v22=(Point)v1.elementAt(i);
		a += v11.x * v22.x + v11.y*v22.y;
                b += v11.x * v22.y - v11.y * v22.x;
	}
	double angle = Math.atan(b / a);
	return Math.acos(a * Math.cos(angle) + b * Math.sin(angle));
}
        
public double IndicativeAngle(Vector points)
{
	Point c = Centroid(points);
        Point a=(Point)points.elementAt(0);
	return Math.atan2(c.y - a.y, c.x - a.x);
}
        


public static Vector TranslateToOrigin1(Vector points,int x1,int y1)
{
Point c = Centroid(points);
Vector newpoints = new Vector(points.size());
for (int i = 0; i < points.size(); i++)
{
	Point p = (Point)points.elementAt(i);
	double qx = p.x -c.x+ x1;
	double qy = p.y -c.y+ y1;
	newpoints.addElement(new Point(qx, qy));
}
return newpoints;
}


        
public Vector RotateBy1(Vector points, double radians) // rotates points around centroid
{
	Point c = Centroid(points);
	double cos1 = Math.cos(radians);
	double sin1 = Math.sin(radians);

	Vector newPoints = new Vector(points.size());
	for (int i = 0; i < points.size(); i++)
	{
                 Point i1=(Point)points.elementAt(i);
		double qx = (i1.x - c.x) * cos1 - (i1.y - c.y) * sin1 + c.x;
		double qy = (i1.x - c.x) * sin1 + (i1.y - c.y) * cos1 + c.y;
		newPoints.addElement(new Point(qx, qy));
	}
	return newPoints;
}



}
