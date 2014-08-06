#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"
#include "mythread.h"

#include <QDebug>
#include <QString>
#include <QtCore/qmath.h>
#include <QMutex>

#include <iostream>
#include <cmath>

#include <cv.h>
#include <highgui.h>

using namespace cv;

MyThread::MyThread(MainWindow *window) : QThread(window),
    win(window)

{
}

void MyThread::run()
{
    int start, end;

    start = this->number*16;
    end = this->number*16+16;

    if(this->number<15){
        threadRelighting(start,end);
    }
    else {
        threadRelighting(start,250);
    }


}

void MyThread::threadRelighting(int start, int end){
    Mat tempImg;
    Mat channel[3];
    QMutex mutex;

    // BUG - doesn't work for last two images
    for(int i=start; i<end; i++){
        //qDebug() << "i: " << i << this->number << "start: " << start << "end: " << end;

        //correcting GAMMA to 2.2
        win->arrayOfRFImages[i].convertTo(tempImg,CV_8UC1);
        tempImg =  win->removeGamma(tempImg, GAMMA);

        // converting from 0-255 range to 0-1 range
        tempImg = win->convertToFloatingPoint(tempImg);

        // ------------ START OF COMPUTATION FOR EACH IMAGE -------------------------------

        // *** coloring the image ***

        // split image into R G B channels
        split(tempImg, channel);

        bool cellIsRelighted = false;
        float scalar = 255/((win->finalVoronoiColors[i][0]+win->finalVoronoiColors[i][1]+win->finalVoronoiColors[i][2])/3);

        // re relighting
        for(int x=0; x<win->light.size(); x++){
            //qDebug() << "i: " << i << "x: " << x;
            if(win->light[x]->cellToRelit==i){
                cellIsRelighted = true;
                // bug - colours from the class gave the same colour as first light

                qDebug() << "old: " << win->finalVoronoiIntensities[i][0] << "new : " << win->light[x]->lightIntensity;
                channel[2] *= (win->colorOfLights[x][0]/scalar)*win->light[x]->lightIntensity*win->lightIntensities[i][0];
                channel[1] *= (win->colorOfLights[x][1]/scalar)*win->light[x]->lightIntensity*win->lightIntensities[i][1];
                channel[0] *= (win->colorOfLights[x][2]/scalar)*win->light[x]->lightIntensity*win->lightIntensities[i][2];
            }
        }

        // ----------------------  painting with light -----------------------
        for(int x=0; x<win->storedCells.size(); x++){
            if(win->storedCells[x]==i){
                channel[2] *= (1/scalar)*50*win->lightIntensities[i][0];
                channel[1] *= (1/scalar)*50*win->lightIntensities[i][1];
                channel[0] *= (1/scalar)*50*win->lightIntensities[i][2];
             }
        }


        // multiplying each channel with corresponding final voronoi cell colour and intensity
        if(cellIsRelighted==false){
            channel[2] *= win->finalVoronoiColors[i][0]*win->finalVoronoiIntensities[i][0]*win->lightIntensities[i][0];
            channel[1] *= win->finalVoronoiColors[i][1]*win->finalVoronoiIntensities[i][1]*win->lightIntensities[i][1];
            channel[0] *= win->finalVoronoiColors[i][2]*win->finalVoronoiIntensities[i][2]*win->lightIntensities[i][2];
        }

        //Merging red and green channels
        merge(channel,3,tempImg);

        // if its a first image
        if(i==0){
            mutex.lock();
            win->relightedImg = win->finalLightStageIntensities[i]*tempImg;
            win->sumOfIntensities += win->finalLightStageIntensities[i];
            mutex.unlock();
        }
        // if it is not a second image - add weight of the new image to the previous one
        else if(i!=0){
            mutex.lock();
            win->relightedImg += win->finalLightStageIntensities[i]*tempImg;
            win->sumOfIntensities += win->finalLightStageIntensities[i];


            //QString testPath = QString("/homes/td613/Documents/individual project/images/test/test-image%1.png").arg(i);
            //const char* tpath = testPath.toStdString().c_str();
            //imwrite(tpath, win->relightedImg);
            mutex.unlock();
        }

        // ------------ END OF COMPUTATION FOR EACH IMAGE -------------------------------

        /*
        // change back from 0-1 range to 0-255 range
        temp *= 255;

        // convert it for the right format to add gamma
        temp.convertTo(tempImg,CV_8UC1);

        // add gamma to the image
        temp = win->addGamma(tempImg, GAMMA);

        // to cut off bug intensities over 1
        if(win->finalLightStageIntensities[i]>1){
            win->finalLightStageIntensities[i] = 0.75;
        }

        QString pathname = QString("/homes/td613/Documents/individual project/images/coloured/relighted-image%1.png").arg(i);
        const char* path = pathname.toStdString().c_str();
        imwrite(path, tempImg);
        */

    }
}
