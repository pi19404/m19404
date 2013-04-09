/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Settings;

import android.util.Log;




/**
 *
 * @author pi19404
 */
public class Settings {
    public  String HOME_DIR="/home/pi19404/NetBeansProjects/object_detection/";
    public  String TEMPLATE_DIR="/home/pi19404/NetBeansProjects/object_detection/";
    public  String HMM_DIR="/home/pi19404/NetBeansProjects/object_detection/";
    public  String SVM_DIR="/media/UBUNTU/SOFTWARES/software/java/";
    public  String SVM_MODEL="/media/UBUNTU/SOFTWARES/software/java/bac/t1.model";//SVM_DIR+"bac/t1.model";
    public  String SVM_SCALE="/media/UBUNTU/SOFTWARES/software/java/bac/t1.range";//SVM_DIR+"bac/t1.range";
    
    public static boolean isAndroid() {
        try {
            Class.forName("android.app.Activity");
            return true;
        } catch(ClassNotFoundException e) {
            return false;
        }
    }
    
    public Settings()
    {
    	
    }
    
    public  Settings(String dir,String dir1)
    {
    	HOME_DIR=dir;
    	TEMPLATE_DIR=dir;
    	SVM_DIR=dir1;
    	SVM_MODEL=SVM_DIR+"/t1.model";
    	SVM_SCALE=SVM_DIR+"/t1.range";
        Log.e("DIR",HOME_DIR+":"+TEMPLATE_DIR+":"+SVM_DIR+":"+SVM_MODEL+":"+SVM_SCALE);
    }
    
}

