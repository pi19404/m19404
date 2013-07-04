/*
 * Copyright (C) 2011, Mathieu Labbe - IntRoLab - Universite de Sherbrooke
 */
#include "Settings.h"
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <stdio.h>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
#include <string>
using namespace std;
ParametersMap Settings::defaultParameters_;
ParametersMap Settings::parameters_;
ParametersType Settings::parametersType_;
Settings Settings::dummyInit_;

QString Settings::workingDirectory()
{
#ifdef WIN32
    return QString("%1/Documents/%2").arg(QDir::homePath()).arg(PROJECT_NAME);
#else
    return QString("%1").arg(QDir::currentPath());
#endif
}

QString Settings::iniDefaultPath()
{
#ifdef WIN32
    return QString("%1/Documents/%2/%3").arg(QDir::homePath()).arg(PROJECT_NAME).arg(Settings::iniDefaultFileName());
#else
    return QString("%1/%2_%3").arg(QDir::currentPath()).arg(PROJECT_PREFIX).arg(Settings::iniDefaultFileName());
#endif
}

void Settings::loadSettings(const QString & fileName, QByteArray * windowGeometry)
{

    QString path = fileName;
    if(fileName.isEmpty())
    {
        path = iniDefaultPath();
    }
    cerr << " Settings path is " << fileName.toStdString() << endl;
    QSettings ini(path, QSettings::IniFormat);
    for(ParametersMap::const_iterator iter = defaultParameters_.begin(); iter!=defaultParameters_.end(); ++iter)
    {
        const QString & key = iter.key();
        QVariant value = ini.value(key, QVariant());
        if(value.isValid())
        {
            setParameter(key, value);
        }
    }

    if(windowGeometry)
    {
        QVariant value = ini.value("windowGeometry", QVariant());
        if(value.isValid())
        {
            *windowGeometry = value.toByteArray();
        }
    }

    printf("Settings loaded from %s\n", path.toStdString().c_str());
}

void Settings::saveSettings(const QString & fileName, const QByteArray & windowGeometry)
{
    QString path = fileName;
    if(fileName.isEmpty())
    {
        path = iniDefaultPath();
    }
    QSettings ini(path, QSettings::IniFormat);
    for(ParametersMap::const_iterator iter = parameters_.begin(); iter!=parameters_.end(); ++iter)
    {
        QString type = Settings::getParametersType().value(iter.key());
        if(type.compare("float") == 0)
        {
            ini.setValue(iter.key(), QString::number(iter.value().toFloat(),'g',6));
        }
        else
        {
            ini.setValue(iter.key(), iter.value());
        }
    }
    if(!windowGeometry.isEmpty())
    {
        ini.setValue("windowGeometry", windowGeometry);
    }
    printf("Settings saved to %s\n", path.toStdString().c_str());
}

