///*
// * To change this template, choose Tools | Templates
// * and open the template in the editor.
// */
//package dtw;
//
//
//import dtw.dtw.TimeWarpInfo;
//import dtw.timeseries.TimeSeries;
//import dtw.util.DistanceFunction;
//import dtw.util.DistanceFunctionFactory;
//import java.io.BufferedReader;
//import java.io.File;
//import java.io.FileReader;
//import java.io.FileWriter;
//import java.util.Enumeration;
//import java.util.Vector;
//
//
///** This class performs contains functions for template matching.
// *   contain methods for template normalization and comparision.
// *   compatible with both android and desktop
// * @author pi19404
// */
//public class PUtils {
//
//	
//    DistanceFunction distFn;
//    String name;		//base directory containing template configuration filter 
//    String test;
//	public static int NumPoints = 64; //NUMBER OF RESAMPLED POINTS
//    int NumTemplates = 30;    //MAX NUMBER OF TEMPLATES
//
//    //normalized dimensions size
//    public static double SquareSize = 250.0; 
//    
//    //normalized half diagonal size
//	double HalfDiagonal = 0.5 * Math.sqrt(250.0 * 250.0 + 250.0 * 250.0);
//	
//	
//	/**
//	 * funcation that computes the pathLength given the vector of input points
//	 * @param points
//	 * @return
//	 */
//	public  double PathLength(Vector points)
//	{
//            
//		double length = 0;
//		for (int i = 1; i < points.size(); i++)
//		{
//			//length += Distance((Point) points[i - 1], (Point) points[i]);
//			length += Distance((Point) points.elementAt(i - 1), (Point) points.elementAt(i));
//		}
//		return length;
//	}
//
//	
//	/**
//	 * function that computes the distance between two set of points
//	 * @param path1
//	 * @param path2
//	 * @return
//	 */
//	
//	public  double PathDistance(Vector path1, Vector path2)
//	{            
//		double distance = 0;
//		for (int i = 0; i < path1.size(); i++)
//		{
//			distance += Distance((Point) path1.elementAt(i), (Point) path2.elementAt(i));
//		}
//		return distance / path1.size();
//	}
//
//        	
//	/**
//	 * function that computes the distance between two points
//	 * @param p1
//	 * @param p2
//	 * @return
//	 */
//	public  double Distance(Point p1, Point p2)
//	{
//		double dx = p2.x - p1.x;
//		double dy = p2.y - p1.y;
//		return Math.sqrt(dx * dx + dy * dy);
//	}
//        
//	/**
//	 * function that computes the centroid of given points
//	 * @param points
//	 * @return
//	 */
//	public static Point Centroid(Vector points)
//	{
//		double xsum = 0.0;
//		double ysum = 0.0;
//		
//		Enumeration e = points.elements();
//		
////		foreach (Point p in points)
//		while (e.hasMoreElements())
//		{
//			Point p = (Point)e.nextElement();
//			xsum += p.x;
//			ysum += p.y;
//		}
//		return new Point(xsum / points.size(), ysum / points.size());
//	}
//        
//
//	/**
//	 * function than translates the centroid of points to origin
//	 * @param points
//	 * @return
//	 */
//	public  Vector TranslateToOrigin(Vector points)
//	{
//		Point c = Centroid(points);
//		Vector newpoints = new Vector(points.size());
//		for (int i = 0; i < points.size(); i++)
//		{
//			Point p = (Point)points.elementAt(i);
//			double qx = p.x - c.x;
//			double qy = p.y - c.y;
//			newpoints.addElement(new Point(qx, qy));
//		}
//		return newpoints;
//	}	
//                
//	/**
//	 * function that scales the set of points between boundaries in 2D plane
//	 * @param points
//	 * @return
//	 */
//	public  Vector Scale(Vector points)
//	{
//		double minX = Double.POSITIVE_INFINITY, maxX = Double.NEGATIVE_INFINITY, minY = Double.POSITIVE_INFINITY, maxY = Double.NEGATIVE_INFINITY;
//                
//		Vector newpoints = new Vector(points.size());
//		for (int i = 0; i < points.size(); i++)
//		{
//                    Point p = (Point)points.elementAt(i);
//                    minX = Math.min(minX, p.x);
//                    minY = Math.min(minY, p.y);
//                    maxX = Math.max(maxX, p.x);
//                    maxY = Math.max(maxY, p.y);
//                }
//                 double s=Math.max(maxX - minX, maxY - minY);
//                 for (int i = 0; i < points.size(); i++)
//		{
//                    
//            Point p = (Point)points.elementAt(i);
//			double qx = (p.x - minX)/s;
//			double qy = (p.y - minY)/s;
//			newpoints.addElement(new Point(qx, qy));
//		}
//		return newpoints;
//	}
//        
// 
//
//public class Template
//{
//	String Name;
//	Vector Points;
//
//	
//	//variable indicating the number of sample points
//		
//
//	
//	Template(String name, Vector points,int mode) 
//	{
//		this.Name = name;
//		this.Points = Resample(points,NumPoints);
//	}
//        
//        public Vector getPoints()
//        {
//            return Points;
//        }
//}
//
///**
// * function than resamples the points to fixed sequence
// * @param points
// * @param n
// * @return
// */
//	public   Vector Resample(Vector points, int n)
//	{		
//		double I = PathLength(points) / (n - 1); // interval length
//		double D = 0.0;
//		
//		Vector srcPts = new Vector(points.size());
//		for (int i = 0; i < points.size(); i++)
//			srcPts.addElement(points.elementAt(i));
//		
//		Vector dstPts = new Vector(n);
//		dstPts.addElement(srcPts.elementAt(0));	//assumes that srcPts.size() > 0
//		
//		for (int i = 1; i < srcPts.size(); i++)
//		{
//			Point pt1 = (Point) srcPts.elementAt(i - 1);
//			Point pt2 = (Point) srcPts.elementAt(i);
//
//			double d = Distance(pt1, pt2);
//			if ((D + d) >= I)
//			{
//				double qx = pt1.x + ((I - D) / d) * (pt2.x - pt1.x);
//				double qy = pt1.y + ((I - D) / d) * (pt2.y - pt1.y);
//				Point q = new Point(qx, qy);
//				dstPts.addElement(q); // append new point 'q'
//				srcPts.insertElementAt(q, i); // insert 'q' at position i in points s.t. 'q' will be the next i
//				D = 0.0;
//			}
//			else
//			{
//				D += d;
//			}
//		}
//		// somtimes we fall a rounding-error short of adding the last point, so add it if so
//		if (dstPts.size() == n - 1)
//		{
//			dstPts.addElement(srcPts.elementAt(srcPts.size() - 1));
//		}
//
//		return dstPts;
//	}
//
//    protected Result result = new Result("no gesture", 0, -1);
//	
//	protected boolean active = false;
//        
//	protected int x, y;
//	protected int state;
//		
//	protected int _key = -1;
//	double AngleRange = 30.0;
//	double AnglePrecision = 2.0;
//	public static double Phi = 0.5 * (-1.0 + Math.sqrt(5.0)); // Golden Ratio
//	
//	public Point centroid = new Point(0, 0);
//	
//	public Rectangle boundingBox = new Rectangle(0, 0, 0, 0);
//	
//	int bounds[] = { 0, 0, 0, 0 };
//	protected boolean gesture = true; 
//	public Vector points = new Vector(1000);
//        
//        
//    
//	public static Vector RotateToZero(Vector points)
//	{	
//    		return RotateToZero(points, null, null);	
//    }
//
//	
//	public static Vector RotateToZero(Vector points, Point centroid, Rectangle boundingBox)
//	{
//		Point c = Centroid(points);
//		Point first = (Point)points.elementAt(0);
//	
//		double theta = Math.atan2(c.y - first.y, c.x - first.x);
//		
//		if (centroid != null)
//			centroid.copy(c);
//		
//		if (boundingBox != null)
//			BoundingBox(points, boundingBox);
//		
//		return RotateBy(points, -theta);
//	}		
//	
//	public static Vector RotateBy(Vector points, double theta)
//	{
//		return RotateByRadians(points, theta);
//	}
//	
//	// rotate the points by the given radians about their centroid
//	public static Vector RotateByRadians(Vector points, double radians)
//	{
//		Vector newPoints = new Vector(points.size());
//		Point c = Centroid(points);
//
//		double _cos = Math.cos(radians);
//		double _sin = Math.sin(radians);
//
//		double cx = c.x;
//		double cy = c.y;
//
//		for (int i = 0; i < points.size(); i++)
//		{
//			Point p = (Point) points.elementAt(i);
//
//			double dx = p.x - cx;
//			double dy = p.y - cy;
//
//			newPoints.addElement(
//				new Point(	(dx * _cos) - (dy * _sin) + cx, 
//							(dx * _sin) + (dy * _cos) + cy )
//							);
//		}
//		return newPoints;
//	}
//    Vector Templates = new Vector(NumTemplates);
//
//    /**
//     * read template points from data file
//     * @param file
//     * @return
//     * @throws Exception
//     */
//  public  String[] CSVRead(String file ) throws Exception {
//
//  BufferedReader CSVFile = new BufferedReader(new FileReader(file));
//
//  String dataRow = CSVFile.readLine(); 
//  String[] dataArray = null;
//  while (dataRow != null){
//   dataArray = dataRow.split(",");
//dataRow = CSVFile.readLine(); // Read next line of data.
//
//  }
//  // Close the file once all data has been read.
//  CSVFile.close();
//
//  // End the printout with a blank line.
//  return dataArray;
//
// } //main()
// // CSVRead
//  
//  /**
//   * function that write points to data file
//   * @param fname
//   * @param points
//   */
//public void CSVWrite(String fname,Vector points)         
//{
//    try{
//     //   Log.e("","-"+name+"/"+fname);
//     File dir = new File(name+"/"+fname);
//     String name1="";
//     if(!dir.exists())
//     {
//         if(!dir.mkdir())
//             return;
//     }
//     String[] chld = dir.list();
//     //if(chld.length>0)
//     {
//         name1="a"+(chld.length+1)+".csv";
//         FileWriter writer = new FileWriter(dir.getAbsolutePath()+"/"+name1);
//         for(int i=0;i<points.size();i=i+2)
//         {
//             
//             Point a=(Point)points.elementAt(i);
//             String a1=""+(int)a.x;
//             writer.append(a1);
//             writer.append(",");
//             a1=""+(int)a.y;
//             writer.append(a1);
//             if(i!=points.size()-2)
//             writer.append(",");
//         }
//         writer.close();
//         
//     }
//
//    }
//    catch(Exception e)
//    {
//        
//    }
//}
//     
///**
// * function that lists the files in directory
// * @param name
// */
//public  void dirlist1(String name)
//        {
//            try{
//                
//                   
//            File dir = new File(name);
//            
//            String[] chld = dir.list();
//            for(int i=0;i<chld.length;i++)
//            {
//                 
//                File x=new File(dir.getAbsolutePath()+"/"+chld[i]);
//                if(x.isFile() && x.getName().substring(x.getName().lastIndexOf(".")).equals(".csv"))
//                {
//
//                  String data[]=CSVRead(dir.getAbsolutePath()+"/"+chld[i]);
//                  
//                 int d[]=new int[data.length];
//                 for (int k = 0; k < data.length;k++)
//		{
//                       d[k]=Integer.parseInt(data[k]);
//			
//		}
//                 
//                 Templates.addElement(loadTemplate(dir.getName(), d));
//                 //int d1[]=new int[d.length];
//                 //copyReversed(d,d1);
//                 //Templates.addElement(loadTemplate(dir.getName(), d));
//                }
//                if(x.isDirectory())
//                {
//                    //Log.e("-d-",""+dir.getAbsolutePath()+"/"+chld[i]);
//                    dirlist(dir.getAbsolutePath()+"/"+chld[i]);
//                
//                }
//            }
//            }
//            catch(Exception e)
//            {
//                
//            }
//        }
//	
///**
// * function to copy reflection of path
// * @param dst
// * @param src
// */
//      public void copyReversed(int dst[], int src[])
//	{		
//		for (int i = 0; i < src.length/2; i++)
//		{
//			dst[ i*2 ] = src[src.length - i*2 - 2];  
//			dst[ i*2 + 1 ] = src[src.length - i*2 + 1 - 2];  
//	
//		}
//	
///*		for (int i = 0; i < dst.length/2; i++)
//			System.out.println(dst[ i*2 ] + ", " + dst[ i*2 + 1 ] + "\t" + 
//								src[ i*2 ] + ", " + src[ i*2 + 1 ]);		
//*/	}
//                
//	public  void dirlist(String name)
//        {
//            try{
//                
//                   
//            File dir = new File(name);
//            
//            String[] chld = dir.list();
//            for(int i=0;i<chld.length;i++)
//            {
//                 
//                File x=new File(dir.getAbsolutePath()+"/"+chld[i]);
//
//                if(x.isFile() && x.getName().substring(x.getName().lastIndexOf(".")).equals(".csv"))
//                {
//                  //                 Log.e("1-f-",""+dir.getAbsolutePath()+"/"+chld[i]+":"+x.getName().substring(x.getName().lastIndexOf(".")).equals(".csv"));
//                  String data[]=CSVRead(dir.getAbsolutePath()+"/"+chld[i]);
//                   //Log.e("2-f-",""+dir.getAbsolutePath()+"/"+chld[i]+":"+x.getName().substring(x.getName().lastIndexOf(".")).equals(".csv"));                 
//                 int d[]=new int[data.length];
//                 for (int k = 0; k < data.length;k++)
//		{
//                    //Log.e("",":"+data[k]);
//                       d[k]=(int)Double.parseDouble(data[k]);
//			
//		}
//                  
//                 Templates.addElement(loadTemplate(dir.getName(), d));
//                  // Log.e("3-f-",""+dir.getAbsolutePath()+"/"+chld[i]+":"+x.getName().substring(x.getName().lastIndexOf(".")).equals(".csv"));
//                 //int d1[]=new int[d.length];
//                 //copyReversed(d,d1);
//                 //Templates.addElement(loadTemplate(dir.getName(), d));
//                }
//                if(x.isDirectory())
//                {
//                    //Log.e("-d-",""+dir.getAbsolutePath()+"/"+chld[i]);
//                    dirlist(dir.getAbsolutePath()+"/"+chld[i]);
//                
//                }
//            }
//            }
//            catch(Exception e)
//            {
//                
//            }
//        }
//        //int mm=0;
//	
//	public static Vector ScaleToSquare1(Vector points, double size,double size1)
//{
//return ScaleToSquare1(points, size,size1, null);
//}				
//
//public static Vector ScaleToSquare1(Vector points, double size,double size1, Rectangle boundingBox)
//{
//Rectangle B = BoundingBox(points);
//Vector newpoints = new Vector(points.size());
//for (int i = 0; i < points.size(); i++)
//{
//	Point p = (Point)points.elementAt(i);
//	double qx = p.x * (size / B.Width);
//	double qy = p.y * (size1 / B.Height);
//	newpoints.addElement(new Point(qx, qy));
//}
//
//if (boundingBox != null) //this will probably not be used as we are more interested in the pre-rotated bounding box -> see RotateToZero
//	boundingBox.copy(B);
//
//return newpoints;
//}	
//
//public static Vector TranslateToOrigin1(Vector points,int x1,int y1)
//{
//Point c = Centroid(points);
//Vector newpoints = new Vector(points.size());
//for (int i = 0; i < points.size(); i++)
//{
//	Point p = (Point)points.elementAt(i);
//	double qx = p.x -c.x+ x1;
//	double qy = p.y -c.y+ y1;
//	newpoints.addElement(new Point(qx, qy));
//}
//return newpoints;
//}
//
//    public  PUtils(int m,int m1,String direct)
//    {
//        mode=m1;
//        distFn = DistanceFunctionFactory.getDistFnByName("EuclideanDistance"); 
//        //loadTemplatesDefault();
//        name=direct+"/"+"data";
//        test=direct+"/"+"testdata";
//        dirlist(name);
//    }
//
//        public int getTemplateSize()
//        {
//            return Templates.size();
//        }
//        
//        public String getTemplateName(int i)
//        {
//            return ((Template)Templates.elementAt(i)).Name;
//        }
//        public Template getTemplate(int i)
//        {
//            return ((Template)Templates.elementAt(i));
//        } 
//        Template loadTemplate(String name, int[] array)
//	{
//		return new Template(name, loadArray(array),mode);
//	}
// 
//	Vector loadArray(int[] array)
//	{
//		Vector v = new Vector(array.length/2);
//		for (int i = 0; i < array.length; i+= 2)
//		{
//			Point p = new Point(array[i], array[i+1]);
//			v.addElement(p);
//		}
//		//Log.e("lllll ",""+v.size());
//	//	System.out.println(v.size() + " " + array.length);
//	
//		return v;
//	}
//	
//	
// 	int AddTemplate(String name, Vector points)
//	{
//		Templates.addElement(new Template(name, points,mode));
//		return Templates.size();
//	}
//	
//	int DeleteUserTemplates()
//	{
//		for (int i = Templates.size()-NumTemplates; i > 0; i--)
//		{
//			Templates.removeElementAt(Templates.size()-1);
//		}
//		
//		return Templates.size();
//	}
//        
//        	public static Vector ScaleToSquare(Vector points, double size)
//	{
//		return ScaleToSquare(points, size, null);
//	}				
//        	
//	public static Rectangle BoundingBox(Vector points)
//	{
//		double minX = Double.MAX_VALUE;
//		double maxX = Double.MIN_VALUE;
//		double minY = Double.MAX_VALUE;
//		double maxY = Double.MIN_VALUE;
//	
//		Enumeration e = points.elements();
//		
////		foreach (Point p in points)
//		while (e.hasMoreElements())
//		{
//			Point p = (Point)e.nextElement();
//		
//			if (p.x < minX)
//				minX = p.x;
//			if (p.x > maxX)
//				maxX = p.x;
//		
//			if (p.y < minY)
//				minY = p.y;
//			if (p.y > maxY)
//				maxY = p.y;
//		}
//	
//		return new Rectangle(minX, minY, maxX - minX, maxY - minY);
//	}
//        
//        	public static void BoundingBox(Vector points, Rectangle dst)
//	{
//		double minX = Double.MAX_VALUE;
//		double maxX = Double.MIN_VALUE;
//		double minY = Double.MAX_VALUE;
//		double maxY = Double.MIN_VALUE;
//	
//		Enumeration e = points.elements();
//		
////		foreach (Point p in points)
//		while (e.hasMoreElements())
//		{
//			Point p = (Point)e.nextElement();
//		
//			if (p.x < minX)
//				minX = p.x;
//			if (p.x > maxX)
//				maxX = p.x;
//		
//			if (p.y < minY)
//				minY = p.y;
//			if (p.y > maxY)
//				maxY = p.y;
//		}
//	
//		dst.X = minX;
//		dst.Y = minY;
//		dst.Width = maxX - minX;
//		dst.Height = maxY - minY;
//	}	
//                
//	public static Vector ScaleToSquare(Vector points, double size, Rectangle boundingBox)
//	{
//		Rectangle B = BoundingBox(points);
//		Vector newpoints = new Vector(points.size());
//		for (int i = 0; i < points.size(); i++)
//		{
//			Point p = (Point)points.elementAt(i);
//			double qx = p.x * (size / B.Width);
//			double qy = p.y * (size / B.Height);
//			newpoints.addElement(new Point(qx, qy));
//		}
//		
//		if (boundingBox != null) //this will probably not be used as we are more interested in the pre-rotated bounding box -> see RotateToZero
//			boundingBox.copy(B);
//		
//		return newpoints;
//	}
//               public int   mode=0;
//               double OneDThreshold = 0.25; // customize to desired gesture set (usually 0.20 - 0.35)
//               double StartAngleIndex = (NumPoints / 8); // eighth of gesture length
//               double AngleSimilarityThreshold = Deg2Rad(30.0);
//               
//public Result Recognize()
//	{
//  double a2=0;
//  //System.out.println("Creating time series");
//  TimeSeries template=new TimeSeries();
//  TimeSeries example=new TimeSeries();
//  double length1=0;
//  double length=0;
//                
//            if (points.isEmpty() || points.size()<10) //the recognizer will crash if we try to process an empty set of points...
//            return new Result("No match.", 0.0,0);
//        
//                Vector pointsx,pointsx1;
//                
//		 pointsx = Resample(points, NumPoints);		
//                 
//                 Point StartUnitVector=null;
//                 Point startv=null ;
//                 double radiansx = IndicativeAngle(pointsx);
//                 pointsx= ScaleDimTo(pointsx, SquareSize, OneDThreshold);
//                 pointsx =TranslateToOrigin(pointsx);
//                //points = dollar2.ScaleToSquare1(points, image.width()/2+1,image.height()/2+1);
//                //points = dollar2.TranslateToOrigin(points);
//                //points = dollar2.TranslateToOrigin1(points,(int)image.width()/2,(int)image.height()/2);
//           
//
//                 StartUnitVector = CalcStartUnitVector(pointsx,(int)StartAngleIndex);
//                 example=new TimeSeries(pointsx);
//                 length=PathLength(pointsx);
//                
//		double b = Double.POSITIVE_INFINITY; // best score
//		int u = -1; // best index
// 
//                double b1 = Double.POSITIVE_INFINITY; // best score
//		double d=Double.POSITIVE_INFINITY;	
//                double d1=Double.POSITIVE_INFINITY;
//                int s=0;
//    
//                
//		for (int i = 0; i < Templates.size(); i++) // for each point-cloud template
//		{
//                    Template x=(Template)Templates.elementAt(i);
//                    Vector points1=new Vector(x.Points.size());
//                    for(int k=0;k<x.Points.size();k++)
//                    {
//                        points1.addElement(x.Points.elementAt(k));
//                    }
//                    double radians = IndicativeAngle(points1);
//                   // points1= RotateBy(points1, -radians);
//                    points1= ScaleDimTo(points1, SquareSize, OneDThreshold);
//                   // pointsx = RotateBy(pointsx, +radians); // restore
//                    points1 =TranslateToOrigin(points1);
//                    
//                     startv = CalcStartUnitVector(points1,(int)StartAngleIndex);                
//                     template=new TimeSeries(points1);
//                     
//
//                
//                            a2=PathLength(points1);
//                            if(AngleBetweenUnitVectors(startv, StartUnitVector) <= AngleSimilarityThreshold)
//                            {
//                                System.out.println(AngleSimilarityThreshold+"  computing DTW"+AngleBetweenUnitVectors(startv, StartUnitVector)+":");
//                                //d1 = DistanceAtBestAngle(pointsx,new Template(((Template)Templates.elementAt(i)).Name,points1,mode), -AngleRange, AngleRange, AnglePrecision);
//                                final TimeWarpInfo info = dtw.dtw.FastDTW.getWarpInfoBetween(template, example,30, distFn);
//                                d=info.getDistance();
//                                
//                                
//                            }
//                            else
//                            {
//                                System.out.println(AngleSimilarityThreshold+"  computing DTW1"+AngleBetweenUnitVectors(startv, StartUnitVector)+":");
//                                d=Double.POSITIVE_INFINITY;
//                            }
//
//			if (d < b)
//			{
//                                length1=PathLength(points1);
//				b = d; // new best distance
//				u = i; // template index
//                                b1=d1;
//			}
//                        System.out.println("calculating the score "+d+":"+b+":"+u+":"+i);
//		}
//                
//                double score = 1 ;
//                Template x=null;
//                if(u!=-1)
//                {
//                x=(Template)Templates.elementAt(u);                
//                 //score=0;
//                }
//                //b=Math.round(b);
//                double a1=0;
/////                if(mode==2)
//                {
//                    System.out.println("calculating the score "+u);
//                 //    if(u!=-1)
//                     //Log.e("scored    ",""+x.Name+":"+"scored N   :"+b1+"   Score dtw  :"+(b*HalfDiagonal/(length*length1) )+":"+(b*HalfDiagonal/(length1) )+"  :f1"+(1.0 - (b1 / HalfDiagonal))+"  :f2: "+b+":"+HalfDiagonal);
//                     score = 1.0 - (b*HalfDiagonal/(length*length1) );
//                     System.out.println("calculating the score- "+score);
//                }
//                if(u!=-1)
//                //   Log.e("template--->",""+x.Name+":"+(b*HalfDiagonal )+":"+score+":"+u+":"+a1);
//                System.out.println("returning result"+((Template)Templates.elementAt(u)).Name);
//		return  (u == -1) ? new Result("No match.", 0.0,u) : new Result(((Template)Templates.elementAt(u)).Name, score, u);
//                
//               //return (u == -1) ? new Result("No match.", 0.0,u) : new Result(x.Name,b,u);
//	}
// 	public  double DistanceAtBestAngle(Vector points, Template T, double a, double b, double threshold)
//	{
//		double Phi = 3.1412;
//	
//		double x1 = Phi * a + (1.0 - Phi) * b;
//		double f1 = DistanceAtAngle(points, T, x1);
//		double x2 = (1.0 - Phi) * a + Phi * b;
//		double f2 = DistanceAtAngle(points, T, x2);
//		
//		while (Math.abs(b - a) > threshold)
//		{
//			if (f1 < f2)
//			{
//				b = x2;
//				x2 = x1;
//				f2 = f1;
//				x1 = Phi * a + (1.0 - Phi) * b;
//				f1 = DistanceAtAngle(points, T, x1);
//			}
//			else
//			{
//				a = x1;
//				x1 = x2;
//				f1 = f2;
//				x2 = (1.0 - Phi) * a + Phi * b;
//				f2 = DistanceAtAngle(points, T, x2);
//			}
//		}
//		return Math.min(f1, f2);
//	}
//        
//        	public  double DistanceAtAngle(Vector points, Template T, double theta)
//	{
//		Vector newpoints = RotateBy(points, theta);
//		return PathDistance(newpoints, T.Points);
//	}		
//
//    	public void pointerDragged(int x, int y)
//	{
//		addPoint(x, y);
//	}
//        
//        	
//	public void clear()
//	{
//		points.removeAllElements();
//		result.Name = "";
//		result.Score = 0;
//		result.Index = -1;
//	}
//        
//
//	
//
//	
//	public String getName()
//	{		
//		return result.Name;
//	}
//	
//	public double getScore()
//	{
//		return result.Score;
//	}
//
//	public int getIndex()
//	{
//		return result.Index;
//	}
//
//	public void setActive(boolean state)
//	{
//		active = state;
//	}
//	
//	public boolean getActive()
//	{
//		return active;
//	}	
//	
//	public void pointerPressed(int x, int y)
//	{
//		clear();
//	}
//	
//	public void pointerReleased(int x, int y)
//	{
//                points.addElement(new Point(x, y));
//                result=Recognize();
//                int t=points.size();
//                
//	}
//        
//	public void addPoint(int x, int y)
//	{
//		if (!active)
//			return;
//		
//		points.addElement(new Point(x, y));
////		System.out.println(x + " " + y + " " + points.size());
//	}
//        
//        public double OptimalCosineDistance(Vector v1,Vector v2) // for Protractor
//{
//	double a = 0.0;
//	double b = 0.0;
//	for (int i = 0; i < v1.size(); i =i+1)
//	{
//                Point v11=(Point)v1.elementAt(i);
//                Point v22=(Point)v1.elementAt(i);
//		a += v11.x * v22.x + v11.y*v22.y;
//                b += v11.x * v22.y - v11.y * v22.x;
//	}
//	double angle = Math.atan(b / a);
//	return Math.acos(a * Math.cos(angle) + b * Math.sin(angle));
//}
//        
//        public double IndicativeAngle(Vector points)
//{
//	Point c = Centroid(points);
//        Point a=(Point)points.elementAt(0);
//	return Math.atan2(c.y - a.y, c.x - a.x);
//}
//        
//        public Vector ScaleDimTo(Vector points, double size, double oneDratio) // scales bbox uniformly for 1D, non-uniformly for 2D
//{
//	Rectangle B = BoundingBox(points);
//	boolean uniformly = Math.min(B.Width / B.Height, B.Height / B.Width) <= oneDratio; // 1D or 2D gesture test
//	Vector newPoints = new Vector(points.size());
//	for (int i = 0; i < points.size(); i++)
//	{
//                Point i1=(Point)points.elementAt(i);
//		double qx = uniformly ? i1.x * (size / Math.max(B.Width, B.Height)) : i1.x * (size / B.Width);
//		double qy = uniformly ? i1.y * (size / Math.max(B.Width, B.Height)) : i1.y * (size / B.Height);
//                newPoints.addElement(new Point(qx, qy));
//		
//	}
//	return newPoints;
//}
//        
//public Vector RotateBy1(Vector points, double radians) // rotates points around centroid
//{
//	Point c = Centroid(points);
//	double cos1 = Math.cos(radians);
//	double sin1 = Math.sin(radians);
//
//	Vector newPoints = new Vector(points.size());
//	for (int i = 0; i < points.size(); i++)
//	{
//                 Point i1=(Point)points.elementAt(i);
//		double qx = (i1.x - c.x) * cos1 - (i1.y - c.y) * sin1 + c.x;
//		double qy = (i1.x - c.x) * sin1 + (i1.y - c.y) * cos1 + c.y;
//		newPoints.addElement(new Point(qx, qy));
//	}
//	return newPoints;
//}
//
//public Point CalcStartUnitVector(Vector points, int index) // start angle from points[0] to points[index] normalized as a unit vector
//{
//        Point i1=(Point)points.elementAt(index);
//        Point i2=(Point)points.elementAt(0);
//	Point v = new Point(i1.x - i2.x, i1.y - i2.y);
//	double len = Math.sqrt(v.x * v.x + v.y * v.y);
//	return new Point(v.x / len, v.y / len);
//}
//
//public double AngleBetweenUnitVectors(Point v1, Point v2) // gives acute angle between unit vectors from (0,0) to v1, and (0,0) to v2
//{
//	double n = (v1.x * v2.x + v1.y * v2.y);
//	if (n < -1.0 || n > +1.0)
//		n = Round(n, 5); // fix: JavaScript rounding bug that can occur so that -1 <= n <= +1
//	return Math.acos(n); // arc cosine of the vector dot product
//}
//
//public double Round(double n,int d) { 
//    double d1 = Math.pow(10,d); 
//    return Math.round(n*d1)/d1; 
//} // round 'n' to 'd' decimals
//double Deg2Rad(double d) { return (d * Math.PI / 180.0); }
//double Rad2Deg(double r) { return (r * 180.0 / Math.PI); }
//}
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.android.fastdtw.dtw;


import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Enumeration;
import java.util.Vector;

import com.android.fastdtw.dtw.dtw.TimeWarpInfo;
import com.android.fastdtw.dtw.timeseries.TimeSeries;
import com.android.fastdtw.dtw.util.DistanceFunction;
import com.android.fastdtw.dtw.util.DistanceFunctionFactory;


/** This class performs contains functions for template matching.
 *   contain methods for template normalization and comparision.
 *   compatible with both android and desktop
 * @author pi19404
 */
public class PUtils {
    
    	public static double SquareSize = 250.0;
	double HalfDiagonal = 0.5 * Math.sqrt(250.0 * 250.0 + 250.0 * 250.0);
	public  double PathLength(Vector points)
	{
		double length = 0;
		for (int i = 1; i < points.size(); i++)
		{
			//length += Distance((Point) points[i - 1], (Point) points[i]);
			length += Distance((Point) points.elementAt(i - 1), (Point) points.elementAt(i));
		}
		return length;
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

        	
	public  double Distance(Point p1, Point p2)
	{
		double dx = p2.x - p1.x;
		double dy = p2.y - p1.y;
		return Math.sqrt(dx * dx + dy * dy);
	}
        
        	// compute the centroid of the points given
	public static Point Centroid(Vector points)
	{
		double xsum = 0.0;
		double ysum = 0.0;
		
		Enumeration e = points.elements();
		
//		foreach (Point p in points)
		while (e.hasMoreElements())
		{
			Point p = (Point)e.nextElement();
			xsum += p.x;
			ysum += p.y;
		}
		return new Point(xsum / points.size(), ysum / points.size());
	}
        
			
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
                
        public  Vector Scale(Vector points)
	{
		double minX = Double.POSITIVE_INFINITY, maxX = Double.NEGATIVE_INFINITY, minY = Double.POSITIVE_INFINITY, maxY = Double.NEGATIVE_INFINITY;
                
		Vector newpoints = new Vector(points.size());
		for (int i = 0; i < points.size(); i++)
		{
                    Point p = (Point)points.elementAt(i);
                    minX = Math.min(minX, p.x);
                    minY = Math.min(minY, p.y);
                    maxX = Math.max(maxX, p.x);
                    maxY = Math.max(maxY, p.y);
                }
                 double s=Math.max(maxX - minX, maxY - minY);
                 for (int i = 0; i < points.size(); i++)
		{
                    
                        Point p = (Point)points.elementAt(i);
			double qx = (p.x - minX)/s;
			double qy = (p.y - minY)/s;
			newpoints.addElement(new Point(qx, qy));
		}
		return newpoints;
	}
        
 

public class Template
{
	String Name;
	Vector Points;

	Template(String name, Vector points,int mode) 
	{
		this.Name = name;
		this.Points = Resample(points,NumPoints);
//                if(mode==0)
//                {
//		this.Points = Utils.RotateToZero(this.Points);
//                this.Points = Utils.ScaleToSquare(this.Points, Recognizer.SquareSize);
//                }
//                else if(mode==1)
//                {
//		this.Points = Scale(this.Points);
//                }
//                else if(mode==2)
//                {
//                    double radians = IndicativeAngle(this.Points);
//                    this.Points = RotateBy(this.Points, -radians);
//                    this.Points = ScaleDimTo(this.Points, SquareSize, OneDThreshold);
//                }
//		this.Points = TranslateToOrigin(this.Points);		
	}
        
        public Vector getPoints()
        {
            return Points;
        }
}
    
	public  Vector Resample(Vector points, int n)
	{		
		double I = PathLength(points) / (n - 1); // interval length
		double D = 0.0;
		
		Vector srcPts = new Vector(points.size());
		for (int i = 0; i < points.size(); i++)
			srcPts.addElement(points.elementAt(i));
		
		Vector dstPts = new Vector(n);
		dstPts.addElement(srcPts.elementAt(0));	//assumes that srcPts.size() > 0
		
		for (int i = 1; i < srcPts.size(); i++)
		{
			Point pt1 = (Point) srcPts.elementAt(i - 1);
			Point pt2 = (Point) srcPts.elementAt(i);

			double d = Distance(pt1, pt2);
			if ((D + d) >= I)
			{
				double qx = pt1.x + ((I - D) / d) * (pt2.x - pt1.x);
				double qy = pt1.y + ((I - D) / d) * (pt2.y - pt1.y);
				Point q = new Point(qx, qy);
				dstPts.addElement(q); // append new point 'q'
				srcPts.insertElementAt(q, i); // insert 'q' at position i in points s.t. 'q' will be the next i
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

    public double CloudDistance(Vector pts1, Vector pts2, int start,int index1)
{
	// ***** for debugging for debugging for debugging for debugging for debugging 
	if (pts1.size() != pts2.size())
	{
		//Log.e("   ",pts1.size() + ", " + pts2.size());
	}
	// ***** for debugging for debugging for debugging for debugging for debugging 
	int s=Math.min(pts1.size(),pts2.size());
        s=pts1.size();
	Vector matched = new Vector(s); // bool[n] array
        
	for (int k = 0; k < s; k++)
		matched.addElement(false);
        int c=0;
	double sum = 0;
	int i = start;
        int index =-1;
	do
	{
		index = -1;
		double min = Double.POSITIVE_INFINITY;
		for (int j = 0; j < matched.size(); j++)
		{
                    
			if ((matched.elementAt(j).equals(false)))
			{
                            Point pt1 = (Point) pts1.elementAt(i);
                            Point pt2 = (Point) pts2.elementAt(j);
				double d = Distance(pt1,pt2);
				if (d < min)
				{
					min = d;
					index = j;
				}
			}
		}
             //  if(index!=-1)
               // {
		matched.remove(index);
                matched.insertElementAt(true,index);                
              //  }
              // else
              // {
               //    Log.e(":::",""+start+":"+index);
              // }
               double ff=i;
		double weight = 1 - (Math.abs((ff - start + s)%s) / s);
		sum += weight * min;
//                Log.e("DDDDD",""+i+":"+sum+":"+weight+":"+index+":"+start+":"+c);
		i = (int)Math.abs((ff + 1)%s);
                
	} while (i != start);
        index1=index;
	return sum;
}
	int NumTemplates = 16;
	public static int NumPoints = 32;
        protected Result result = new Result("no gesture", 0, -1);
	
	protected boolean active = false;
        
        protected int x, y;
	protected int state;
		
	protected int _key = -1;
	double AngleRange = 30.0;
	double AnglePrecision = 2.0;
	public static double Phi = 0.5 * (-1.0 + Math.sqrt(5.0)); // Golden Ratio
	
	public Point centroid = new Point(0, 0);
	public Rectangle boundingBox = new Rectangle(0, 0, 0, 0);
	int bounds[] = { 0, 0, 0, 0 };
	protected boolean gesture = true; 
	public Vector points = new Vector(1000);
        
        
    public double GreedyCloudMatch(Vector points, Vector P,int xindex)
{
	double e = 0.50;
	int step =(int) Math.floor(Math.pow(points.size(), 1 - e));
	double min = Double.POSITIVE_INFINITY;
        int c=0;
        int index=0,tindex=0;
	for (int i = 0; i < points.size(); i += step)
	{
		double d1 = CloudDistance(points, P, i,index);
		double d2 = CloudDistance(P, points, i,tindex);
                if(d1<=d2)
                {
                    if(d1<min)
                    {
                       min=d1;
                       xindex=index;
                    }
                }
                if(d1>d2)
                {
                    if(d2<min)
                    {
                       min=d2;
                       xindex=tindex;
                    }
                }
                    
                
		//min = Math.min(min, Math.min(d1, d2)); // min3
                //c=c+1;
	}
       ;
        //Log.e("DDD",""+step+":"+min+":"+points.size());
	return min;
}
    
    	public static Vector RotateToZero(Vector points)
	{	return RotateToZero(points, null, null);	}

	
	public static Vector RotateToZero(Vector points, Point centroid, Rectangle boundingBox)
	{
		Point c = Centroid(points);
		Point first = (Point)points.elementAt(0);
		//double theta = Trigonometric.atan2(c.Y - first.Y, c.X - first.X);
		double theta = Math.atan2(c.y - first.y, c.x - first.x);
		
		if (centroid != null)
			centroid.copy(c);
		
		if (boundingBox != null)
			BoundingBox(points, boundingBox);
		
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
        
    Vector Templates = new Vector(NumTemplates);

public Vector CSVRead_train(String fname)         
{
    try{
//        Log.e("","-"+name+"/"+fname);
     
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
         
public void CSVWrite(String fname,Vector points)         
{
    try{
//        Log.e("","-"+name+"/"+fname);
     File dir = new File(name+"/"+fname);
     String name1="";
     if(!dir.exists())
     {
         if(!dir.mkdir())
             return;
     }
     String[] chld = dir.list();
     //if(chld.length>0)
     {
         name1="a"+(chld.length+1)+".csv";
         FileWriter writer = new FileWriter(dir.getAbsolutePath()+"/"+name1);
         for(int i=0;i<points.size();i=i+2)
         {
             
             Point a=(Point)points.elementAt(i);
             String a1=""+(int)a.x;
             writer.append(a1);
             writer.append(",");
             a1=""+(int)a.y;
             writer.append(a1);
             if(i!=points.size()-2)
             writer.append(",");
         }
         writer.close();
         
     }

    }
    catch(Exception e)
    {
        
    }
}

public void CSVWrite_train(String fname,Vector points)         
{
    try{
//        Log.e("","-"+name+"/"+fname);
     {

         FileWriter writer = new FileWriter(fname);
         for(int i=0;i<points.size();i=i+2)
         {
             
             Point a=(Point)points.elementAt(i);
             String a1=""+(int)a.x;
             writer.append(a1);
             writer.append(",");
             a1=""+(int)a.y;
             writer.append(a1);
             if(i!=points.size()-2)
             writer.append(",");
         }
         writer.close();
         
     }

    }
    catch(Exception e)
    {
        
    }
}

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
//                    Log.e("-f-",""+dir.getAbsolutePath()+"/"+chld[i]+":"+x.getName().substring(x.getName().lastIndexOf(".")));
                  String data[]=CSVRead(dir.getAbsolutePath()+"/"+chld[i]);
                  
                 int d[]=new int[data.length];
                 for (int k = 0; k < data.length;k++)
		{
                       d[k]=Integer.parseInt(data[k]);
			
		}
                 
                 Templates.addElement(loadTemplate(dir.getName(), d));
                 //int d1[]=new int[d.length];
                 //copyReversed(d,d1);
                 //Templates.addElement(loadTemplate(dir.getName(), d));
                }
                if(x.isDirectory())
                {
                    //Log.e("-d-",""+dir.getAbsolutePath()+"/"+chld[i]);
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
	
/*		for (int i = 0; i < dst.length/2; i++)
			System.out.println(dst[ i*2 ] + ", " + dst[ i*2 + 1 ] + "\t" + 
								src[ i*2 ] + ", " + src[ i*2 + 1 ]);		
*/	}
                
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
                  //                 Log.e("1-f-",""+dir.getAbsolutePath()+"/"+chld[i]+":"+x.getName().substring(x.getName().lastIndexOf(".")).equals(".csv"));
                  String data[]=CSVRead(dir.getAbsolutePath()+"/"+chld[i]);
                   //Log.e("2-f-",""+dir.getAbsolutePath()+"/"+chld[i]+":"+x.getName().substring(x.getName().lastIndexOf(".")).equals(".csv"));                 
                 int d[]=new int[data.length];
                 for (int k = 0; k < data.length;k++)
		{
                    //Log.e("",":"+data[k]);
                       d[k]=(int)Double.parseDouble(data[k]);
			
		}
                  
                 Templates.addElement(loadTemplate(dir.getName(), d));
                  // Log.e("3-f-",""+dir.getAbsolutePath()+"/"+chld[i]+":"+x.getName().substring(x.getName().lastIndexOf(".")).equals(".csv"));
                 //int d1[]=new int[d.length];
                 //copyReversed(d,d1);
                 //Templates.addElement(loadTemplate(dir.getName(), d));
                }
                if(x.isDirectory())
                {
                    //Log.e("-d-",""+dir.getAbsolutePath()+"/"+chld[i]);
                    dirlist(dir.getAbsolutePath()+"/"+chld[i]);
                
                }
            }
            }
            catch(Exception e)
            {
                
            }
        }
        //int mm=0;
    public  PUtils(int m,int m1,String dir)
    {
        mode=m1;
        distFn = DistanceFunctionFactory.getDistFnByName("EuclideanDistance"); 
        name=dir+"/"+"data";
        test=dir+"/"+"testdata";
        //loadTemplatesDefault(); 
        dirlist(name);
    }
        DistanceFunction distFn;
        String name;
        String test;

        public int getTemplateSize()
        {
            return Templates.size();
        }
        
        public String getTemplateName(int i)
        {
            return ((PUtils.Template)Templates.elementAt(i)).Name;
        }
        public PUtils.Template getTemplate(int i)
        {
            return ((PUtils.Template)Templates.elementAt(i));
        } 
        PUtils.Template loadTemplate(String name, int[] array)
	{
		return new PUtils.Template(name, loadArray(array),mode);
	}
 
	Vector loadArray(int[] array)
	{
		Vector v = new Vector(array.length/2);
		for (int i = 0; i < array.length; i+= 2)
		{
			Point p = new Point(array[i], array[i+1]);
			v.addElement(p);
		}
		//Log.e("lllll ",""+v.size());
	//	System.out.println(v.size() + " " + array.length);
	
		return v;
	}
	
 	int AddTemplate(String name, Vector points)
	{
		Templates.addElement(new PUtils.Template(name, points,mode));
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
               public int   mode=0;
               double OneDThreshold = 0.25; // customize to desired gesture set (usually 0.20 - 0.35)
               double StartAngleIndex = (NumPoints / 8); // eighth of gesture length
               double AngleSimilarityThreshold = Deg2Rad(30.0);
public Result Recognize()
	{
  double a2=0;
  TimeSeries template=new TimeSeries();
  TimeSeries example=new TimeSeries();
  double length1=0;
  double length=0;
 //if(mode==1)
    //  return new Result("no-matching ", 0, 0);
                
            if (points.isEmpty() || points.size()<10) //the recognizer will crash if we try to process an empty set of points...
            return new Result("No match.", 0.0,0);
            //    Log.e("31---->",":"+Templates.size()+":"+points.size());        
                Vector pointsx,pointsx1;
		pointsx = Resample(points, NumPoints);		
              //  Log.e("32---->",":"+Templates.size()+":"+points.size());
                 Point StartUnitVector=null;
                 Point startv=null ;
                if(mode==1)
                {
                pointsx = Scale(pointsx);
                 pointsx = ScaleToSquare(pointsx, SquareSize);
                pointsx =TranslateToOrigin(pointsx);
                }
                if(mode==0)
                {
                pointsx = RotateToZero(pointsx, centroid, boundingBox);                    
                pointsx = ScaleToSquare(pointsx, SquareSize);
                pointsx =TranslateToOrigin(pointsx);
                }
                if(mode==2)
                {
                    double radians = IndicativeAngle(pointsx);
                    //pointsx= RotateBy(pointsx, -radians);
                    pointsx= ScaleDimTo(pointsx, SquareSize, OneDThreshold);
                    //pointsx = RotateBy(pointsx, +radians); // restore
                    pointsx =TranslateToOrigin(pointsx);
                     StartUnitVector = CalcStartUnitVector(pointsx,(int)StartAngleIndex);
                     example=new TimeSeries(pointsx);
                     length=PathLength(pointsx);
                }
		//
                //
            //     Log.e("33---->",":"+Templates.size()+":"+points.size());        
		
            //     Log.e("34---->",":"+Templates.size()+":"+points.size());        
                                                
		double b = Double.POSITIVE_INFINITY; // best score
		int u = -1; // best index
 
                double b1 = Double.POSITIVE_INFINITY; // best score
		double d=Double.POSITIVE_INFINITY;	
                double d1=Double.POSITIVE_INFINITY;
               int s=0;
		for (int i = 0; i < Templates.size(); i++) // for each point-cloud template
		{
                    PUtils.Template x=(PUtils.Template)Templates.elementAt(i);
                    Vector points1=new Vector(x.Points.size());
                    for(int k=0;k<x.Points.size();k++)
                    {
                        points1.addElement(x.Points.elementAt(k));
                    }
                    //points1= Resample(points1, NumPoints);	
       
                    
                    
                if(mode==1)
                {
                points1 = Scale(points1);
                points1 = ScaleToSquare(points1, SquareSize);
                points1 =TranslateToOrigin(points1);
                }
                if(mode==0)
                {
                points1 = RotateToZero(points1, centroid, boundingBox);                    
                points1 = ScaleToSquare(points1, SquareSize);
                points1 =TranslateToOrigin(points1);
                }
                if(mode==2)
                {
                    double radians = IndicativeAngle(points1);
                   // points1= RotateBy(points1, -radians);
                    points1= ScaleDimTo(points1, SquareSize, OneDThreshold);
                   // pointsx = RotateBy(pointsx, +radians); // restore
                    points1 =TranslateToOrigin(points1);
                    
                     startv = CalcStartUnitVector(points1,(int)StartAngleIndex);
                     template=new TimeSeries(points1);
                     
                }
                
                    a2=PathLength(points1);
                      //Log.e("2---->",":"+points.size()+":"+points1.size());
                        if(mode==1)
			d = GreedyCloudMatch(pointsx,points1,s);
                        if(mode==0)
                        d = DistanceAtBestAngle(pointsx,new PUtils.Template(((PUtils.Template)Templates.elementAt(i)).Name,points1,mode), -AngleRange, AngleRange, AnglePrecision);
                        else if(mode==2)
                        {
                            if(AngleBetweenUnitVectors(startv, StartUnitVector) <= AngleSimilarityThreshold||(1==1))
                            {
                               d1 = DistanceAtBestAngle(pointsx,new PUtils.Template(((PUtils.Template)Templates.elementAt(i)).Name,points1,mode), -AngleRange, AngleRange, AnglePrecision);
                               //TimeWarpInfo info = dtw.dtw.FastDTW.getWarpInfoBetween(template, example,20, distFn);
                               //info=dtw.dtw.DTW.getWarpInfoBetween(template, example, distFn);
                               //d=info.getDistance();
                               d=d1;
                            }
                            else
                            {
                                d=Double.POSITIVE_INFINITY;
                            }
                        }
			if (d < b)
			{
                                length1=PathLength(points1);
				b = d; // new best distance
				u = i; // template index
                                b1=d1;
			}
		}
                
                double score = 1 ;
                 PUtils.Template x=null;
                if(u!=-1)
                {
                x=(PUtils.Template)Templates.elementAt(u);                
                 //score=0;
                }
                //b=Math.round(b);
                double a1=0;
                if(mode==1)
                {
                a1=PathLength(pointsx);
                a1=Math.max(a1,a2);
                score=1-(b/a1);
                score = 1.0 - (b / HalfDiagonal);
                }
                else if (mode==0)
                {
                     score = 1.0 - (b / HalfDiagonal);
                }
                else if(mode==2)
                {
                     //if(u!=-1)
                     //Log.e("scored    ",""+x.Name+":"+"scored N   :"+b1+"   Score dtw  :"+(b*HalfDiagonal/(length*length1) )+":"+(b*HalfDiagonal/(length1) )+"  :f1"+(1.0 - (b1 / HalfDiagonal))+"  :f2: "+b+":"+HalfDiagonal);
                     score = 1.0 - (b*HalfDiagonal/(length*length1) );
                     
                }
                //if(u!=-1)
//                   Log.e("template--->",""+x.Name+":"+(b*HalfDiagonal )+":"+score+":"+u+":"+a1);
		return  (u == -1) ? new Result("No match.", 0.0,u) : new Result(((PUtils.Template)Templates.elementAt(u)).Name, score, u);
                
               //return (u == -1) ? new Result("No match.", 0.0,u) : new Result(x.Name,b,u);
	}

 	public  double DistanceAtBestAngle(Vector points, PUtils.Template T, double a, double b, double threshold)
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
        
        	public  double DistanceAtAngle(Vector points, PUtils.Template T, double theta)
	{
		Vector newpoints = RotateBy(points, theta);
                TimeSeries template=new TimeSeries();
                TimeSeries example=new TimeSeries();
                
                template=new TimeSeries(points);
                example=new TimeSeries(T.Points);
                TimeWarpInfo info = com.android.fastdtw.dtw.dtw.FastDTW.getWarpInfoBetween(template, example,20, distFn);                
		//return PathDistance(newpoints, T.Points);
                return info.getDistance();
	}		

    	public void pointerDragged(int x, int y)
	{
		addPoint(x, y);
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
	
	public void pointerReleased(int x, int y)
	{
                points.addElement(new Point(x, y));
		result=Recognize();
                int t=points.size();
                //Log.e("-----",""+points.size());
	}
        
        public void addPoint(int x, int y)
	{
		if (!active)
			return;
		
		points.addElement(new Point(x, y));
//		System.out.println(x + " " + y + " " + points.size());
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

public double Round(double n,int d) { 
    double d1 = Math.pow(10,d); 
    return Math.round(n*d1)/d1; 
} // round 'n' to 'd' decimals
double Deg2Rad(double d) { return (d * Math.PI / 180.0); }
double Rad2Deg(double r) { return (r * 180.0 / Math.PI); }
}
