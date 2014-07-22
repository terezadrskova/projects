#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <QPixmap>
#include <QString>
#include <QtCore/qmath.h>

#include <iostream>
#include <cmath>

#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;


/* ----------------------------------------------------------------
 *                  FLIP X AXIS
 *-----------------------------------------------------------------*/
int MainWindow::flipXAxis(int phi){
    int diff;

    qDebug() << "first phi" << phi;
    if(phi>512){
        diff = phi-512;
        phi = phi-2*diff;
    }
    else if (phi<512){
        diff = 512-phi;
        phi = phi+2*diff;
    }
    qDebug() << "second phi" << phi;
    return phi;
}

/* ----------------------------------------------------------------
 *                  FLIP Y AXIS
 *-----------------------------------------------------------------*/
int MainWindow::flipYAxis(int theta){
    int diff;
    if(theta>256){
        diff = theta-256;
        theta = theta-2*diff;
    }
    else if (theta<256){
        diff = 256-theta;
        theta = theta+2*diff;
    }
    return theta;
}

/* ----------------------------------------------------------------
 *                   CONVERT 0-255 RANGE TO 0-1 RANGE
 *-----------------------------------------------------------------*/
Mat MainWindow::convertToFloatingPoint(Mat &image)
{
   //read in a image
   Mat result;

   // convert from PNG - CV_8U format to floating point CV_32F
   image.convertTo(result,CV_32F);

   // normalise the matrix from 0 to 1
   normalize(result,result,0,1,CV_MINMAX);

   return result;
}

/* ----------------------------------------------------------------
 *                  FIND NEAREST VORONOI CELL
 *-----------------------------------------------------------------*/
int MainWindow::findNearestCell(Point2f &point){
    for( int i = 0; i < NUMBEROFLIGHTSOURCES; i++ )
    {
        if(sphericalCoordinatesPX[i][0] == point.x && sphericalCoordinatesPX[i][1] == point.y){
            return i;
        }
    }
    return -1;
}

int MainWindow::closestLightSource( Point2f whereClicked){
    float smallest = 500.0;
    int index = -1;
    for( int i = 0; i <light.size(); i++ )
    {
        float newValue = qAbs(light[i]->pos.x - whereClicked.x)+qAbs(light[i]->pos.y - whereClicked.y);
        if(newValue < smallest){
            smallest = newValue;
            index = i;
        }
    }
    return index;
}

