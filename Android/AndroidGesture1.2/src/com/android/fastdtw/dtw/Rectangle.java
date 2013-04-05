package com.android.fastdtw.dtw;

public class Rectangle
{
	public double X, Y, Width, Height;

	public Rectangle(double x, double y, double width, double height) // constructor
	{
		this.X = x;
		this.Y = y;
		this.Width = width;
		this.Height = height;
	}
	
	public void copy(Rectangle src)
	{
		X = src.X;
		Y = src.Y;
		Width = src.Width;
		Height = src.Height;			
	}
}
