package com.android.opencv;


/* comptible
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


import com.googlecode.javacpp.Loader;

import com.googlecode.javacpp.*;
import com.googlecode.javacpp.annotation.*;

import static com.googlecode.javacpp.Loader.*;
import static com.googlecode.javacv.cpp.opencv_core.*;
import java.util.StringTokenizer;

import static com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_features2d.*;
import static com.googlecode.javacpp.Loader.*;
/**
 *
 * @author pi19404
 */


@Properties({
@Platform(includepath=genericIncludepath,linkpath=genericLinkpath,include="OpenCVProcessImage.hpp"
, link={"opencv_calib3d","opencv_contrib","opencv_core","opencv_features2d","opencv_flann","opencv_highgui","opencv_imgproc","opencv_legacy","opencv_ml","opencv_nonfree","opencv_objdetect","opencv_video"} ),
@Platform(value="android", includepath=androidIncludepath, linkpath=androidLinkpath,include="opencv_support_class.h",
link={"opencv_core","opencv_flann","opencv_imgproc","opencv_ml","opencv_highgui","opencv_video","opencv_features2d","opencv_calib3d","opencv_objdetect","opencv_legacy","opencv_contrib","opencv_nonfree"} )})
//,/*"opencv_contrib@2.4",*/"opencv_calib3d@.2.4"
@Namespace("OpenCV")
public class ProcessImage {
    public static final String androidIncludepath    = "../include/";
    public static final String androidLinkpath       = "../libs/armeabi";
    public static final String genericIncludepath    = "/opt/local/include/";
    public static final String genericLinkpath       = "/usr/local/lib/:/usr/local/lib64/:/usr/lib:/opt/local/lib/:/opt/local/lib64/:";
    //../lib:	
    
    public static final int
    CV_MAJOR_VERSION    = 2,
    CV_MINOR_VERSION    = 4,
    CV_SUBMINOR_VERSION = 2;

    public static final String CV_VERSION = CV_MAJOR_VERSION + "." + CV_MINOR_VERSION + "." + CV_SUBMINOR_VERSION;
    
    static {
        if (load() != null) {
            String platformName = getPlatformName();
            if (platformName.equals("windows-x86")) {
                SetLibraryPath("C:/opencv/build/x86/vc10/bin/");
            } else if (platformName.equals("windows-x86_64")) {
                SetLibraryPath("/");
            }
        }
    }
    public static class OpenCVProcess extends Pointer {
        
    	static { 
    		Loader.load(); 
                
    		}
   

        public OpenCVProcess() { allocate(); }
        private native void allocate();
        public native void run(int width, int height, byte yuv[], int[] rgba);
        
        	
        


    }

    
    
           
    


    
 
    
}
