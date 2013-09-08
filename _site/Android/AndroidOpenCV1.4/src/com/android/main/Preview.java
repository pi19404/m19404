package com.android.main;

import java.io.IOException;

import android.content.Context;
import android.graphics.Bitmap;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import com.android.main.R;

import com.android.opencv.ProcessImage.*;


public class Preview extends SurfaceView implements SurfaceHolder.Callback {
    SurfaceHolder mHolder;
    Camera mCamera;
    DrawOnTop mDrawOnTop;
    boolean mFinished;

    OpenCVProcess opencvp;
    
    Preview(Context context, DrawOnTop drawOnTop) {
        super(context);
        opencvp=new OpenCVProcess();
        mDrawOnTop = drawOnTop;
        mFinished = false;

        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed.
        mHolder = getHolder();
        mHolder.addCallback(this);
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public void surfaceCreated(SurfaceHolder holder) {
        Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
        int cameraCount = Camera.getNumberOfCameras();

	    for ( int camIdx = 0; camIdx < cameraCount; camIdx++ ) {
	        Camera.getCameraInfo( camIdx, cameraInfo );
	        if ( cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT  ) {
	            try {
	            	if(mCamera==null)
	            	{
	            	mCamera = Camera.open( camIdx );
	                break;
	            	}
	            }
            catch (RuntimeException e) {
            	return;
            }
	        }
	    }
        try {
           mCamera.setPreviewDisplay(holder);
           
           // Preview callback used whenever new viewfinder frame is available
           mCamera.setPreviewCallback(new PreviewCallback() {
        	  public void onPreviewFrame(byte[] data, Camera camera)
        	  {
        		  if ( (mDrawOnTop == null) || mFinished )
        			  return;
        		  
        		  if (mDrawOnTop.mBitmap == null)
        		  {
        			  // Initialize the draw-on-top companion
        			  Camera.Parameters params = camera.getParameters();
        			  mDrawOnTop.mImageWidth = params.getPreviewSize().width;
        			  mDrawOnTop.mImageHeight = params.getPreviewSize().height;
        			  mDrawOnTop.mBitmap = Bitmap.createBitmap(mDrawOnTop.mImageWidth, mDrawOnTop.mImageHeight, Bitmap.Config.RGB_565);
        			  mDrawOnTop.mRGBData = new int[mDrawOnTop.mImageWidth * mDrawOnTop.mImageHeight]; 
        			  mDrawOnTop.mYUVData = new byte[data.length];        			  
        		  }
        		  
        		  // Pass YUV data to draw-on-top companion
        		  int[] rgba = new int [data.length];
        		  System.arraycopy(data, 0, mDrawOnTop.mYUVData, 0, data.length);
        		  if(opencvp.run(mDrawOnTop.mImageWidth, mDrawOnTop.mImageHeight, mDrawOnTop.mYUVData, mDrawOnTop.mRGBData)==0)
        			  Log.e("EEE","Error in exeucting opencv routine");
    			  mDrawOnTop.invalidate();
        	  }
           });
           
	        // Define on touch listener
           this.setOnTouchListener(new OnTouchListener() {
	   			public boolean onTouch(View v, MotionEvent event)
	   			{
	   				if (mDrawOnTop.mState == DrawOnTop.STATE_ORIGINAL)
	   				{
	   					mDrawOnTop.mState = DrawOnTop.STATE_PROCESSED;
	   				}
	   				else if (mDrawOnTop.mState == DrawOnTop.STATE_PROCESSED)
	   				{
	   					mDrawOnTop.mState = DrawOnTop.STATE_ORIGINAL;
	   				}
	   				return false;
	   			}
	   		});
        } 
        catch (IOException exception) {
            mCamera.release();
            mCamera = null;
        }
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        // Surface will be destroyed when we return, so stop the preview.
        // Because the CameraDevice object is not a shared resource, it's very
        // important to release it when the activity is paused.
    	mFinished = true;
    	mCamera.setPreviewCallback(null);
        mCamera.stopPreview();
        mCamera.release();
        mCamera = null;
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        // Now that the size is known, set up the camera parameters and begin
        // the preview.
        Camera.Parameters parameters = mCamera.getParameters();
        parameters.setPreviewSize(640,480);
        parameters.setPreviewFrameRate(30);
        //parameters.setSceneMode(Camera.Parameters.SCENE_MODE_NIGHT);
        //parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
        mCamera.setParameters(parameters);
        mCamera.startPreview();
    }
}
