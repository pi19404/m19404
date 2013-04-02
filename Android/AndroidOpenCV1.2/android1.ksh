cp /media/UBUNTU/repository/m19404/Android/Android-jni/AndroidOpenCVProcessImage.hpp /opt/android-ndk-r7/platforms/android-14/arch-arm/usr/include/AndroidOpenCVProcessImage.hpp
export home1=`pwd`
cd $home1 
export JAVA_HOME=/opt/softwares-linux/jdk1.7.0_04/
JAR=$home1/libs/javacpp.jar
JAR1=$home1/libs/javacv.jar
SRC=$home1/src/com/android/opencv/ProcessImage.java
LIBS=$home1/libs/armeabi
$JAVA_HOME/bin/javac -cp $JAR -classpath bin/classes:build/classes:$JAR1:$JAR $SRC
$JAVA_HOME/bin/java -Dplatform.root=/opt/android-ndk-r7 -jar $JAR -properties android-arm -classpath $home1/src:$JAR1:$JAR:$home1/src/com/android/main com.android.opencv.ProcessImage -d libs/armeabi

