#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"


#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <QString>
#include <QtCore/qmath.h>

#include <iostream>
#include <cmath>

#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

/* ----------------------------------------------------------------
 *                       RF IMAGES LOADING
 *-----------------------------------------------------------------*/

void MainWindow::loadRFImages()
{
    // creating Dialog window to open RF image
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(trUtf8("Image file (*.png *.ppm *.jpg)"));

    if (dialog.exec()){
        rfImagesPath = dialog.selectedFiles();
    }

    // define the size of vector to avoid out of range error
    arrayOfRFImages.resize(rfImagesPath.size());

    Mat tempImg;
    // processing of every single RF image within a vector
    for (int i = 0; i < rfImagesPath.size(); ++i){

        //qDebug() << "File" << i << " is " << rfImagesPath.at(i).toLocal8Bit().constData() << endl;

        // change type to constant char* which is needed for convertToFloatingPoint function
        const char* imageName = rfImagesPath.at(i).toLocal8Bit().constData();
        Mat image = imread(imageName, CV_LOAD_IMAGE_COLOR);

        arrayOfRFImages[i] = image;
    }

    // show that upload was successful
    if(!rfImagesPath.isEmpty()){
        QMessageBox::information(this, tr("Success"), "Upload was successful!");
    }
    // if no images has been uploaded - show an error message
    else {
         QMessageBox::warning(this, tr("Warning"), "You have not uploaded any files!");
    }

    qDebug() << "loading RF images - DONE" << endl;
}

/* ----------------------------------------------------------------
 *                   LIGHT PROBE IMAGE LOADING
 *-----------------------------------------------------------------*/

void MainWindow::loadLightMap()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(trUtf8("Light Map file (*.ppm *.pfm)"));

    if (dialog.exec()){
        lightMapPath = dialog.selectedFiles();
    }


    if(lightMapPath.isEmpty()) {
         QMessageBox::warning(this, tr("Filename"), "You havn't uploaded any files!");
         return;
    }

    QMessageBox::information(this, tr("Filename"), "Upload was successful");

    qDebug() << "Light probe path is: " << lightMapPath.at(0) << endl;

    // only one file can be uploaded at the time (using ExistingFile instead of ExistingFiles above)
    // changing pathname to constant char*
    lightMapPathname = lightMapPath.at(0);
    const char * imageName = lightMapPath.at(0).toLocal8Bit().constData();

    unsigned int mapLatitude = 1024;
    unsigned int mapLongtitude = 512;
    unsigned int mapComponenets = 3;
    lightProbePFM = loadPFM(imageName, mapLatitude, mapLongtitude, mapComponenets);

    qDebug() << "loading lightmap - DONE" << endl;
}
