/*compatible
  Please feel free to use/modify this class. 
  If you give me credit by keeping this information or
  by sending me an email before using it or by reporting bugs , i will be happy.
  Email : gtiwari333@gmail.com,
  Blog : http://ganeshtiwaridotcomdotnp.blogspot.com/ 
 */
package com.android.gesture;


import java.io.Serializable;
import java.util.Arrays;

import com.android.gesture.Point;



/**
 * 
 * @author Ganesh
 * 
 */
public class RawFeature implements Serializable{
	private static final long serialVersionUID = -7355364771484569103L;
	double[] curTime;
	Point[] drawPoint;

	public RawFeature() {
		super();
	}

	public RawFeature(double[] curTime, Point[] drawPoint) {
		super();
		this.curTime = curTime;
		this.drawPoint = drawPoint;
	}

	public double[] getCurTime() {
		return curTime;
	}

	public void setCurTime(double[] curTime) {
		this.curTime = curTime;
	}

	public Point[] getDrawPoint() {
		return drawPoint;
	}

	public void setDrawPoint(Point[] drawPoint) {
		this.drawPoint = drawPoint;
	}

	public String getModelType() {
		return "Raw";
	}

	
	public void showParameters() {
		this.toString();

	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + Arrays.hashCode(curTime);
		result = prime * result + Arrays.hashCode(drawPoint);
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		RawFeature other = (RawFeature) obj;
		if (!Arrays.equals(curTime, other.curTime))
			return false;
		if (!Arrays.equals(drawPoint, other.drawPoint))
			return false;
		return true;
	}

	@Override
	public String toString() {
		return "RawFeature [curTime=" + Arrays.toString(curTime)
				+ ", drawPoint=" + Arrays.toString(drawPoint) + "]";
	}

	@Override
	public Object clone() throws CloneNotSupportedException {
		double[] curTimeC = curTime.clone();
		Point[] drawPointC = drawPoint.clone();
		RawFeature rfc = new RawFeature(curTimeC, drawPointC);
		return rfc;
	}

}
