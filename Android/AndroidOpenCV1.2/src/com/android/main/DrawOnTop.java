package com.android.main;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.view.View;
import com.android.main.R;


public class DrawOnTop extends View {
	Bitmap mBitmap;
	Paint mPaintBlack;
	Paint mPaintYellow;
	byte[] mYUVData;
	int[] mRGBData;
	int mImageWidth, mImageHeight;
	int[] mGrayHistogram;
	double[] mGrayCDF;
	int mState;
	
	
	static final int STATE_ORIGINAL = 0;
	static final int STATE_PROCESSED = 1;

    public DrawOnTop(Context context) {
        super(context);
        
        mPaintBlack = new Paint();
        mPaintBlack.setStyle(Paint.Style.FILL);
        mPaintBlack.setColor(Color.BLACK);
        mPaintBlack.setTextSize(25);
        
        mPaintYellow = new Paint();
        mPaintYellow.setStyle(Paint.Style.FILL);
        mPaintYellow.setColor(Color.YELLOW);
        mPaintYellow.setTextSize(25);
        
        mBitmap = null;
        mYUVData = null;
        mRGBData = null;
        mGrayHistogram = new int[256];
        mGrayCDF = new double[256];
        mState = STATE_ORIGINAL;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (mBitmap != null)
        {
        	int canvasWidth = canvas.getWidth();
        	int canvasHeight = canvas.getHeight();
        	int newImageWidth = canvasWidth;
        	int newImageHeight = canvasHeight;
        	int marginWidth = (canvasWidth - newImageWidth)/2;
        	int marginHeight = (canvasHeight-newImageHeight)/2;
        	        	
        	// Convert from YUV to RGB
        	
        	
        	// Draw bitmap
        	mBitmap.setPixels(mRGBData, 0, mImageWidth, 0, 0, mImageWidth, mImageHeight);
        	
        	canvas.drawBitmap(mBitmap,0,0, mPaintBlack);
        	
        	Rect src = new Rect(0, 0, mImageWidth, mImageHeight);
        	Rect dst = new Rect(0, 0, canvasWidth, canvasHeight);//new Rect(marginWidth, 0, canvasWidth-marginWidth, canvasHeight-marginHeight);
        	canvas.drawBitmap(mBitmap, src, dst, mPaintBlack);
        	
        	// Draw label
        	String imageStateStr;
       		imageStateStr = "Demo Application 1";
        	canvas.drawText(imageStateStr, marginWidth+10-1, 30-1, mPaintBlack);
        	canvas.drawText(imageStateStr, marginWidth+10+1, 30-1, mPaintBlack);
        	canvas.drawText(imageStateStr, marginWidth+10+1, 30+1, mPaintBlack);
        	canvas.drawText(imageStateStr, marginWidth+10-1, 30+1, mPaintBlack);
        	canvas.drawText(imageStateStr, marginWidth+10, 30, mPaintYellow);
        	
        } // end if statement
        
        super.onDraw(canvas);
        
    } // end onDraw method

    }
