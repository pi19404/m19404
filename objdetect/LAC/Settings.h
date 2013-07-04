#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtCore/QByteArray>
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/nonfree/nonfree.hpp"
//#include "common.h"
class Camera;

typedef QMap<QString, QVariant> ParametersMap; // Key, value
typedef QMap<QString, QString> ParametersType; // Key, type

typedef unsigned int uint;

// MACRO BEGIN

#define PARAMETER_GETTER_bool(PREFIX, NAME) \
    static bool get##PREFIX##_##NAME() {return parameters_.value(#PREFIX "/" #NAME).toBool();}
#define PARAMETER_GETTER_int(PREFIX, NAME) \
    static int get##PREFIX##_##NAME() {return parameters_.value(#PREFIX "/" #NAME).toInt();}
#define PARAMETER_GETTER_uint(PREFIX, NAME) \
    static uint get##PREFIX##_##NAME() {return parameters_.value(#PREFIX "/" #NAME).toUInt();}
#define PARAMETER_GETTER_float(PREFIX, NAME) \
    static float get##PREFIX##_##NAME() {return parameters_.value(#PREFIX "/" #NAME).toFloat();}
#define PARAMETER_GETTER_double(PREFIX, NAME) \
    static double get##PREFIX##_##NAME() {return parameters_.value(#PREFIX "/" #NAME).toDouble();}
#define PARAMETER_GETTER_QString(PREFIX, NAME) \
    static QString get##PREFIX##_##NAME() {return parameters_.value(#PREFIX "/" #NAME).toString();}

#define PARAMETER(PREFIX, NAME, TYPE, DEFAULT_VALUE) \
    public: \
        static QString k##PREFIX##_##NAME() {return QString(#PREFIX "/" #NAME);} \
        static TYPE default##PREFIX##_##NAME() {return DEFAULT_VALUE;} \
        static QString type##PREFIX##_##NAME() {return QString(#TYPE);} \
        PARAMETER_GETTER_##TYPE(PREFIX, NAME) \
        static void set##PREFIX##_##NAME(const TYPE & value) {parameters_[#PREFIX "/" #NAME] = value;} \
    private: \
        class Dummy##PREFIX##_##NAME { \
        public: \
            Dummy##PREFIX##_##NAME() { \
                defaultParameters_.insert(#PREFIX "/" #NAME, QVariant(DEFAULT_VALUE)); \
                parameters_.insert(#PREFIX "/" #NAME, DEFAULT_VALUE); \
                parametersType_.insert(#PREFIX "/" #NAME, #TYPE);} \
        }; \
        Dummy##PREFIX##_##NAME dummy##PREFIX##_##NAME;
// MACRO END

class Settings
{
    PARAMETER(Camera, deviceId, int, 0);
    PARAMETER(Camera, imageWidth, int, 640);
    PARAMETER(Camera, imageHeight, int, 480);
    PARAMETER(Camera, imageRate, int, 2); // Hz
    PARAMETER(Camera, videoFilePath, QString, "");


    PARAMETER(Path,path,QString,"/");


public:
    virtual ~Settings(){}

    static QString workingDirectory();
    static QString iniDefaultPath();
    static QString iniDefaultFileName() {return "config.ini";}

    static void loadSettings(const QString & fileName = QString(), QByteArray * windowGeometry = 0);
    static void saveSettings(const QString & fileName = QString(), const QByteArray & windowGeometry = QByteArray());

    static const ParametersMap & getDefaultParameters() {return defaultParameters_;}
    static const ParametersMap & getParameters() {return parameters_;}
    static const ParametersType & getParametersType() {return parametersType_;}
    static void setParameter(const QString & key, const QVariant & value) {if(parameters_.contains(key))parameters_[key] = value;}
    static void resetParameter(const QString & key) {if(defaultParameters_.contains(key)) parameters_.insert(key, defaultParameters_.value(key));}
    static QVariant getParameter(const QString & key) {return parameters_.value(key, QVariant());}



private:
    Settings(){}

private:
    static ParametersMap defaultParameters_;
    static ParametersMap parameters_;
    static ParametersType parametersType_;
    static Settings dummyInit_;
};


#endif /* SETTINGS_H_ */
