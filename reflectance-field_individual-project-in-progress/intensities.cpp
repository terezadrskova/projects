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
 *                   CALCULATE INTENSITIES
 *-----------------------------------------------------------------*/
void MainWindow::calculateIntensities()
{

    double r, g, b;
    if(lightIntensities[0][0]==0){
        qDebug() << "Please load light intensities!";
        return;
    }

    // going through the file of light intensities - stored as 2D double array
    for(int i=0; i<NUMBEROFLIGHTSOURCES; i++){

        r = lightIntensities[i][0];
        g = lightIntensities[i][1];
        b = lightIntensities[i][2];

        finalLightStageIntensities[i] = (r+g+b)/3;

    }
}

/* ----------------------------------------------------------------
 *                   CALCULATE FINAL INTENSITIES
 *-----------------------------------------------------------------*/
void MainWindow::calculateFinalIntensities(Subdiv2D& subdiv){
    int nearestCell;

    for(int i=0; i<NUMBEROFLIGHTSOURCES; i++){
         voronoiIntensities[i][0] = 0;
         voronoiIntensities[i][1] = 0;
         voronoiIntensities[i][2] = 0;
         voronoiIntensities[i][3] = 0;
         finalVoronoiIntensities[i][0] = 0;
         finalVoronoiIntensities[i][1] = 0;
         finalVoronoiIntensities[i][2] = 0;
         finalVoronoiColors[i][0] = 0;
         finalVoronoiColors[i][1] = 0;
         finalVoronoiColors[i][2] = 0;
         averageTheta[i] = 0;
    }
    //red-green-blue.pfm  grace_lat_long.pfm
    const char* imageName = lightMapPathname.toStdString().c_str();
    unsigned int mapLatitude = 1024;
    unsigned int mapLongtitude = 512;
    unsigned int mapComponenets = 3;
    lightProbePFM = loadPFM(imageName, mapLatitude, mapLongtitude, mapComponenets);

    int i=0;

      for(int theta=0; theta<LONGTITUDE; theta++){
          for(int phi=0; phi<LATITUDE; phi++){
            for(int color=0; color<COLORCOMPONENTS; color++){

                Point2f fp(phi,theta);
                Point2f nearestCentroid;
                subdiv.findNearest(fp, &nearestCentroid);
                nearestCell = findNearestCell(nearestCentroid);

                //summing all theta of points from the same cell for the SOLID ANGLE
                float scaledTheta;
                float scalar = LONGTITUDE/PI;
                scaledTheta = theta/scalar;

                // change R
                if(color==0){
                    voronoiIntensities[nearestCell][0] += 1.0;
                    voronoiIntensities[nearestCell][1] += (lightProbePFM[i]*qSin(scaledTheta));
                }

                // change G
                else if(color==1){
                    voronoiIntensities[nearestCell][2] += (lightProbePFM[i]*qSin(scaledTheta));
                }

                // change B
                else if(color==2){
                    voronoiIntensities[nearestCell][3] += (lightProbePFM[i]*qSin(scaledTheta));
                }

                // next index
                i++;
            }
        }
    }

    float scalar;

    for(int i=0; i<NUMBEROFLIGHTSOURCES; i++){

        if(voronoiIntensities[i][0]!=0)
        {
            finalVoronoiIntensities[i][0] = voronoiIntensities[i][1]/voronoiIntensities[i][0];
            finalVoronoiIntensities[i][1] = voronoiIntensities[i][2]/voronoiIntensities[i][0];
            finalVoronoiIntensities[i][2] = voronoiIntensities[i][3]/voronoiIntensities[i][0];
        }

          // final cell intensity is the contrast added to the RF images
         finalCellIntensity[i] = (finalVoronoiIntensities[i][0]+finalVoronoiIntensities[i][1]+finalVoronoiIntensities[i][2])/3;

           //normalise values to get colour between 0-1
         scalar = (finalVoronoiIntensities[i][0] + finalVoronoiIntensities[i][1] + finalVoronoiIntensities[i][2])/13;

         finalVoronoiColors[i][0] = finalVoronoiIntensities[i][0]/scalar;
         finalVoronoiColors[i][1] = finalVoronoiIntensities[i][1]/scalar;
         finalVoronoiColors[i][2] = finalVoronoiIntensities[i][2]/scalar;

         //qDebug() << "final voronoi colours" << finalVoronoiColors[i][0] << finalVoronoiColors[i][1] << finalVoronoiColors[i][2];
     }
}


