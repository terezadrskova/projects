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
 *                   CALCULATE DIRECTIONS
 *-----------------------------------------------------------------*/
void MainWindow::calculateDirections()
{

    double x, y, z, r, theta, phi;
    if(lightDirections[0][0]==0){
        qDebug() << "Please load light directions!";
        return;
    }


    // going through the file of light directions - stored as 2D double array
    for(int i=0; i<253; i++){

        x = lightDirections[i][0];
        y = lightDirections[i][1];
        z = lightDirections[i][2];

        r = qSqrt(x*x + y*y + z*z);


        // PHI has to be in a range 0-2PI
        phi = qAtan2(x,z)+PI;
        phi = fmod(phi,2*PI);
        calculatedDirections[i][0] = phi;
        phi = (phi/(2*PI))*1024;

        // flip X axis - didn't work when function was called
        int diff;
        if(phi>512){
            diff = phi-512;
            phi = phi-2*diff;
        }
        else if (phi<512){
            diff = 512-phi;
            phi = phi+2*diff;
        }
        sphericalCoordinatesPX[i][0] = phi;


        // THETA has to be in a range 0-PI
        theta = qAcos(y/r)+PI;
        theta = fmod(theta,PI);
        calculatedDirections[i][1] = theta;
        theta = (theta/PI)*512;

        // flip Y axis
        theta = flipYAxis(theta);
        sphericalCoordinatesPX[i][1] = theta;
    }
}

