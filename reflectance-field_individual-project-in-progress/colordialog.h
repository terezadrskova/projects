#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <highgui.h>
#include <iostream>

#include <QDialog>
#include <QPushButton>
#include <QColorDialog>
#include <QLabel>
#include <QVBoxLayout>

using namespace cv;

class ColorDialog : public QDialog
{
    Q_OBJECT

public:
    ColorDialog(QWidget *parent = 0);
    QColor color;
    int green;
    int blue;
    int red;
    ~ColorDialog();
private slots:


private:
    QPushButton *button;
    QLabel *colorLabel;
    QVBoxLayout *layout;

};


#endif // QCOLORDIALOG_H
