#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"

#include <QDebug>
#include <QString>
#include <QtCore/qmath.h>
#include <QThread>

#include <iostream>
#include <cmath>

#include <cv.h>
#include <highgui.h>

using namespace cv;

void MainWindow::relighting(){

    createVoronoiDiagram();

    // start threads - first thread goes first - need to create relighteImg
    // before other threads work on it
    myThread1.start();
    if (myThread1.wait()){
        myThread2.start();
        myThread3.start();
        myThread4.start();
        myThread5.start();
        myThread6.start();
        myThread7.start();
        myThread8.start();
        myThread9.start();
        myThread10.start();
        myThread11.start();
        myThread12.start();
        myThread13.start();
        myThread14.start();
        myThread15.start();
        myThread16.start();
    }

    myThread2.wait();
    myThread3.wait();
    myThread4.wait();
    myThread5.wait();
    myThread6.wait();
    myThread7.wait();
    myThread8.wait();
    myThread9.wait();
    myThread10.wait();
    myThread11.wait();
    myThread12.wait();
    myThread13.wait();
    myThread14.wait();
    myThread15.wait();
    myThread16.wait();

    myThread1.quit();
    myThread2.quit();
    myThread3.quit();
    myThread4.quit();
    myThread5.quit();
    myThread6.quit();
    myThread7.quit();
    myThread8.quit();
    myThread9.quit();
    myThread10.quit();
    myThread11.quit();
    myThread12.quit();
    myThread13.quit();
    myThread14.quit();
    myThread15.quit();
    myThread16.quit();


    qDebug() << "THREAD IS FINISHED";

    //relightedImg /= sumOfIntensities;
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
