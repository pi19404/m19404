package com.android.opencv;

import java.io.IOException;

import libsvm.HOG3_fast;
import libsvm.feature_scale;
import libsvm.svm_predict;


import Settings.Settings;

import com.android.main.Log;
import com.googlecode.javacpp.FloatPointer;
import com.googlecode.javacv.cpp.opencv_core.IplImage;

import static com.googlecode.javacv.cpp.opencv_highgui.*;
import Settings.Settings;

import com.googlecode.javacpp.*;
import com.googlecode.javacv.cpp.opencv_video.*;
import static com.googlecode.javacv.cpp.opencv_video.*;
import com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_core.*;
import static com.googlecode.javacv.cpp.opencv_imgproc.*;
import  com.googlecode.javacv.cpp.opencv_imgproc.*;
import static com.googlecode.javacpp.Loader.*;
import com.googlecode.javacpp.annotation.Platform;
import com.googlecode.javacpp.annotation.Properties;
import com.googlecode.javacv.CanvasFrame;
import static com.googlecode.javacv.cpp.opencv_video.*;
import com.googlecode.javacv.cpp.opencv_video.*;

import com.googlecode.javacv.cpp.opencv_legacy.*;
import static com.googlecode.javacv.cpp.opencv_legacy.*;
import com.googlecode.javacv.cpp.opencv_core;
import com.googlecode.javacv.cpp.opencv_core.CvMat;
import com.googlecode.javacv.cpp.opencv_core.CvPoint;
import com.googlecode.javacv.cpp.opencv_core.CvRect;
import com.googlecode.javacv.cpp.opencv_core.IplImage;
import com.googlecode.javacv.cpp.opencv_features2d.DescriptorExtractor;
import com.googlecode.javacv.cpp.opencv_features2d.FastFeatureDetector;
import com.googlecode.javacv.cpp.opencv_features2d.FeatureDetector;
import com.googlecode.javacv.cpp.opencv_features2d.KeyPoint;
import com.googlecode.javacv.cpp.opencv_features2d.ORB;
import com.googlecode.javacv.cpp.opencv_nonfree.SIFT;
import  static com.googlecode.javacv.cpp.opencv_objdetect.*;
import  static com.googlecode.javacv.cpp.opencv_ml.*;
import  static com.googlecode.javacv.cpp.opencv_video.*;
import  com.googlecode.javacv.cpp.opencv_video.*;
import com.googlecode.javacv.JavaCvErrorCallback;

import  static com.googlecode.javacv.cpp.opencv_objdetect.*;
import  static com.googlecode.javacv.cpp.opencv_ml.*;
import  static com.googlecode.javacv.cpp.opencv_video.*;
import  com.googlecode.javacv.cpp.opencv_video.*;
import com.googlecode.javacv.JavaCvErrorCallback;

import  static com.googlecode.javacv.cpp.opencv_objdetect.*;
import  static com.googlecode.javacv.cpp.opencv_ml.*;
import  static com.googlecode.javacv.cpp.opencv_video.*;
import  com.googlecode.javacv.cpp.opencv_video.*;
import com.googlecode.javacv.JavaCvErrorCallback;

import  static com.googlecode.javacv.cpp.opencv_objdetect.*;
import  static com.googlecode.javacv.cpp.opencv_ml.*;
import  static com.googlecode.javacv.cpp.opencv_video.*;
import  static com.googlecode.javacv.cpp.opencv_nonfree.*;
import  com.googlecode.javacv.cpp.opencv_nonfree.*;
import  com.googlecode.javacv.cpp.opencv_features2d.*;
import  static com.googlecode.javacv.cpp.opencv_features2d.*;
import  com.googlecode.javacv.cpp.opencv_video.*;
import com.googlecode.javacv.JavaCvErrorCallback;

import java.nio.FloatBuffer;
import com.googlecode.javacpp.annotation.Platform;
import com.googlecode.javacpp.annotation.Properties;
import com.googlecode.javacv.Parallel;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.DoubleBuffer;
import java.util.Arrays;
import com.googlecode.javacv.cpp.opencv_features2d.FeatureDetector;
//import static com.googlecode.javacv.cpp.opencv_features2d.FeatureDetector;
import static com.googlecode.javacpp.Loader.*;
import static com.googlecode.javacv.cpp.opencv_core.*;
import com.googlecode.javacv.cpp.opencv_features2d.SimpleBlobDetector.Params;
import java.io.IOException;

import libsvm.feature_scale;
import libsvm.svm_predict;

import com.googlecode.javacv.cpp.opencv_objdetect.CvHaarClassifierCascade;

import java.util.logging.Level;
import java.util.logging.Logger;


 public class Classifier {
	 
HOG3_fast hog;    
svm_predict svm;
IplImage resize_image;
FloatPointer hog_features;
feature_scale main_scale;
Settings set;
public Classifier()
{
	Loader.load();
}

public Classifier(Settings set) throws Exception
{
    try{
    	//class to perform SVM prediction
    	svm=new svm_predict();
    	String model_path=set.SVM_MODEL;
    	Log.e("EE","creating svm model file "+model_path);
    	svm.load_model(model_path);
    	Log.e("EE","loading svm model file "+model_path);
    	//class to compute the HOG featurs
        hog=new HOG3_fast();
        Log.e("EE","creating hog descriptor");
        svm.set_probability();
        Log.e("EE","setting svm probability");
        //HOG features pointer
        
        Log.e("EE","allocating hog descriptor");
        //class for scaling object
        main_scale=new feature_scale(set.SVM_SCALE); 
        Log.e("EE","setting feature scaling"+set.SVM_SCALE);
        }
        catch(Exception e)
        {
            e.printStackTrace();
            Log.e("EE","error in classifier initialiation");
            throw e;
        }
}

    public double ll=0;
    public double probx=0;

    //method to extract SVM features
public void extract_feature(IplImage imgxx)
{
        
        
        hog_features= new FloatPointer(hog.nwin_x*hog.nwin_y*hog.B);
        try {
        hog.compute(imgxx, hog_features);
        hog_features.position(0);
        
        	//perform feature scaling
            main_scale.svm_scale(hog_features);
        } catch (Exception ex) {
            ex.printStackTrace();
            Log.e("EE","excption in SVM scaling"+ex.getLocalizedMessage());
        }
        try {
        	//perform prediction
            ll=svm.prediction(hog_features);
        } catch (Exception ex) {
            Log.e("EE","excption in SVM prediction"+ex.getMessage());
        }
        //output probability
        probx=svm.outputp;
}
    
}
