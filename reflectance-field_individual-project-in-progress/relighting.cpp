#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"

#include <QDebug>
#include <QString>
#include <QtCore/qmath.h>

#include <iostream>
#include <cmath>

#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

void MainWindow::relighting(){

    int size = arrayOfRFImages.size();
    Mat tempImg, relightedImg;
    Mat channel[3];

    createVoronoiDiagram();

    for(int i=0; i<size; i++){
        //correcting GAMMA to 2.2
        arrayOfRFImages[i].convertTo(tempImg,CV_8UC1);
        tempImg =  removeGamma(tempImg, GAMMA);

        // converting from 0-255 range to 0-1 range
        tempImg = convertToFloatingPoint(tempImg);


        // ------------ START OF COMPUTATION FOR EACH IMAGE -------------------------------

        // *** coloring the image ***

        // split image into R G B channels
        split(tempImg, channel);

        // multiplying each channel with corresponding final voronoi cell colour and intensity
        channel[2] *= finalVoronoiColors[i][0]*finalVoronoiIntensities[i][0]*lightIntensities[i][0];
        channel[1] *= finalVoronoiColors[i][1]*finalVoronoiIntensities[i][1]*lightIntensities[i][1];
        channel[0] *= finalVoronoiColors[i][2]*finalVoronoiIntensities[i][2]*lightIntensities[i][2];

        // re relighting
        for(int x=0; x<light.size(); x++){
            if(light[x]->cellToRelit==i){
                qDebug() << "changing lighting of cell: " << i << "R: " << light[x]->redC
                         << "G: " << light[x]->greenC
                         << "B: " << light[x]->blueC;
                channel[2] *= (light[x]->redC/255)*light[x]->lightIntensity;
                channel[1] *= (light[x]->greenC/255)*light[x]->lightIntensity;
                channel[0] *= (light[x]->blueC/255)*light[x]->lightIntensity;
            }
        }

        //Merging red and green channels
        merge(channel,3,tempImg);

        // if its a first image
        if(i==0){
            relightedImg = tempImg;
        }
        // if it is not a second image - add weight of the new image to the previous one
        else if(i!=0){
            relightedImg += finalLightStageIntensities[i]*tempImg;
        }

        // ------------ END OF COMPUTATION FOR EACH IMAGE -------------------------------

        // a!!! fter this and before the end of the loop it can be deleted - code only for testing
        // change back from 0-1 range to 0-255 range
        tempImg *= 255;

        // convert it for the right format to add gamma
        tempImg.convertTo(tempImg,CV_8UC1);

        // add gamma to the image
        tempImg = addGamma(tempImg, GAMMA);

        // to cut off bug intensities over 1
        if(finalLightStageIntensities[i]>1){
            finalLightStageIntensities[i] = 0.75;
        }

        QString pathname = QString("/homes/td613/Documents/individual project/images/coloured/relighted-image%1.png").arg(i);
        const char* path = pathname.toStdString().c_str();
        imwrite(path, tempImg);

    }

    relightedImg /= NUMBEROFLIGHTSOURCES;

    relightedImg *= 255;

    // convert it for the right format to add gamma
    relightedImg.convertTo(relightedImg,CV_8UC1);

    // add gamma to the image
    relightedImg = addGamma(relightedImg, GAMMA);

    // convert to format which can be displayed by using OpenCV imwrite
    relightedImg.convertTo(relightedImg,CV_8UC1);

    // decrease contrast (scalar made image too bright - but in correct range)
    relightedImg.convertTo(relightedImg, -1, 0.7, 0);

    imwrite( "/homes/td613/Documents/individual project/images/grace-cathedral/FINAL-RELIGHTED-image.png", relightedImg);
    qDebug() << "DONE WITH RELIGHTING";

    // update RF image
    QPixmap rfImage("/homes/td613/Documents/individual project/images/grace-cathedral/FINAL-RELIGHTED-image.png");
    ui->label_pix_2->setPixmap(rfImage);

    // update lightmap
    if(lightMapPathname!=""){
        const char* pfmLightMapPath = lightMapPathname.toStdString().c_str();
        LoadPFMAndSavePPM(pfmLightMapPath, "/homes/td613/Documents/individual project/images/light-map.ppm");
        Mat lightMapTmp = imread("/homes/td613/Documents/individual project/images/light-map.ppm");
        // make image lighter - add Gamma 2.2
        lightMapTmp = addGamma(lightMapTmp, GAMMA);
        imwrite( "/homes/td613/Documents/individual project/images/light-map.ppm", lightMapTmp);
        QPixmap newLightmap("/homes/td613/Documents/individual project/images/light-map.ppm");
        ui->label_lightmap->setPixmap(newLightmap);
        ui->label_lightmap->setScaledContents(true);
    }

    // update voronoi
    QPixmap newVoronoiImage(voronoiPathname);
    ui->label_voronoi->setPixmap(newVoronoiImage);
    ui->label_voronoi->setScaledContents(true);
}
