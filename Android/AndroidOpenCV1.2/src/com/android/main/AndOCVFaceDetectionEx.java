package com.android.main;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.graphics.PixelFormat;
import android.os.Bundle;
import android.os.Environment;
import android.view.Menu;
import android.view.MenuItem;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import com.android.main.R;

public class AndOCVFaceDetectionEx extends Activity {
    private Preview mPreview;
    private DrawOnTop mDrawOnTop;
    public static String APPLICATION_DIR=Environment.getExternalStorageDirectory().getAbsolutePath()+"/Android";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Hide the window title and set full screen
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        Window W= getWindow();
        getWindow().setFormat(PixelFormat.TRANSLUCENT);
        W.requestFeature(Window.FEATURE_NO_TITLE); 
        W.addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);  
        //W.addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
        W.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        // Create our Preview view and set it as the content of our activity.
        // Create our DrawOnTop view.
        mDrawOnTop = new DrawOnTop(this);
        mPreview = new Preview(this, mDrawOnTop);
        setContentView(mPreview);
        addContentView(mDrawOnTop, new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
    }
    
	/**
	 * Set options menu.
	 */
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		menu.add("Reset Tracker");
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		
		if (item.getTitle().equals("Reset Tracker")) {
			mPreview.initTracker();
			
		}

		// Register all callbacks with preview				
		return true;
	}

	/**
	 * Set option menu close behavior.
	 */
	@Override
	public void onOptionsMenuClosed(Menu menu) {
		// TODO Auto-generated method stub
		super.onOptionsMenuClosed(menu);
	}


}