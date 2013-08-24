#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtGui>
#include <QWidget>

class Window : public QWidget
{
    Q_OBJECT
public:
 //  MyWidget();

protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:

};

#endif // MYWIDGET_H
