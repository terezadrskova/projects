#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"
#include "lightmapwidget.h"
#include "dragwidget.h"
#include "colordialog.h"
#include "mythread.h"

#include <QGLShaderProgram>

#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include <QPixmap>
#include <QThread>

#include <QString>
#include <QtCore/qmath.h>
#include <QSlider>
#include <QSpinBox>
#include <QTabWidget>
#include <QMouseEvent>

#include <iostream>
#include <cmath>

#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    rect(0, 0, 1024, 512),
    subdiv(rect),
    numberOfLights(0),
    activeLightSource(-1),
    red(255),
    green(255),
    blue(255),
    sumOfIntensities(0),
    myThread1(this),
    myThread2(this),
    myThread3(this),
    myThread4(this),
    myThread5(this),
    myThread6(this),
    myThread7(this),
    myThread8(this),
    myThread9(this),
    myThread10(this),
    myThread11(this),
    myThread12(this),
    myThread13(this),
    myThread14(this),
    myThread15(this),
    myThread16(this),
    isPaintingOn(false),
    m_nInitialX(0),
    m_nInitialY(0),
    m_nFinalX(0),
    m_nFinalY(0),
    m_nbMousePressed(false)
{
    ui->setupUi(this);

    //--------------------- INITIALISING THREADS ------------------------
    myThread1.name = "thread1";
    myThread1.number = 0;

    myThread2.name = "thread2";
    myThread2.number = 1;

    myThread3.name = "thread3";
    myThread3.number = 2;

    myThread4.name = "thread4";
    myThread4.number = 3;

    myThread5.name = "thread5";
    myThread5.number = 4;

    myThread6.name = "thread6";
    myThread6.number = 5;

    myThread7.name = "thread7";
    myThread7.number = 6;

    myThread8.name = "thread8";
    myThread8.number = 7;

    myThread9.name = "thread9";
    myThread9.number = 8;

    myThread10.name = "thread10";
    myThread10.number = 9;

    myThread11.name = "thread11";
    myThread11.number = 10;

    myThread12.name = "thread12";
    myThread12.number = 11;

    myThread13.name = "thread13";
    myThread13.number = 12;

    myThread14.name = "thread14";
    myThread14.number = 13;

    myThread15.name = "thread15";
    myThread15.number = 14;

    myThread16.name = "thread16";
    myThread16.number = 15;
    //---------------------- END OF THREADS ----------------------------

    // ------------------- PAINTING section -----------------------------
    //ui->buttonOnOff->setStyleSheet("color: rgb(204, 59, 59)");


    // initilise light source icon - correcting transparency
    lightImg = imread("/homes/td613/Documents/individual project/images/light.png", -1);

    // lightmap
   // QLabel *label_lightmap = new QLabel;
    if(lightMapPathname!=""){
        const char* pfmLightMapPath = lightMapPathname.toStdString().c_str();
        LoadPFMAndSavePPM(pfmLightMapPath, "/homes/td613/Documents/individual project/images/light-map.ppm");
        QPixmap lightmap("/homes/td613/Documents/individual project/images/light-map.ppm");
        ui->label_lightmap->setPixmap(lightmap);
        ui->label_lightmap->setScaledContents(true);
    }

    // voronoi
    //QLabel *label_voronoi = new QLabel;
    QPixmap voronoiImage(voronoiPathname);
    ui->label_voronoi->setPixmap(voronoiImage);
    ui->label_voronoi->setScaledContents(true);


    // relighted image
    //QLabel *label_pix_2 = new QLabel;
    QPixmap rfImage("/homes/td613/Documents/individual project/images/grace-cathedral/FINAL-RELIGHTED-image.png");
    ui->label_pix_2->setPixmap(rfImage);
    ui->label_pix_2->setScaledContents(true);

}

MainWindow::~MainWindow()
{
    delete lightProbePFM;
    delete ui;
}


/* ----------------------------------------------------------------
 *                  TRIGGERED FUNCTIONS
 *-----------------------------------------------------------------*/

void MainWindow::on_actionLoad_Reflectance_Field_Images_triggered(){
    loadRFImages();
}
void MainWindow::on_actionLoad_Light_Map_image_triggered(){
    loadLightMap();
}

void MainWindow::on_relightingButton_clicked()
{
    relighting();
    qDebug() << "Done";
}


void MainWindow::on_updateBtn_clicked()
{
    qDebug() << "button clicked";
}


void MainWindow::on_sliderVoronoi_actionTriggered(int action)
{
    const char * voronoiPathStr = voronoiPathname.toStdString().c_str();
    Mat voronoiTmp = imread(voronoiPathStr);
    // make image lighter - add Gamma
    if(action!=0){
        voronoiTmp *= qPow(2,action);
    }
    //Mat mask,dst;
    //inRange(voronoiTmp,Scalar(0),Scalar(1),mask); // Create a mask image in the range 300 and 400
    //voronoiTmp.copyTo(dst,mask);
    imwrite( "/homes/td613/Documents/individual project/images/voronoi-gamma.png", voronoiTmp);
    QPixmap gammaVoronoi("/homes/td613/Documents/individual project/images/voronoi-gamma.png");
    ui->label_voronoi->setPixmap(gammaVoronoi);
    ui->label_voronoi->setScaledContents(true);
}

void MainWindow::on_spinBoxVoronoi_valueChanged(int arg1)
{
    on_sliderVoronoi_actionTriggered(arg1);
}



void MainWindow::on_sliderLightmap_valueChanged(int value)
{
    const char* newLightMapPath = lightMapPathname.toStdString().c_str();
    LoadPFMAndSavePPM(newLightMapPath, "/homes/td613/Documents/individual project/images/lightmap-gamma.ppm");
    Mat lightmapTmp = imread("/homes/td613/Documents/individual project/images/lightmap-gamma.ppm");
    for(int y = 0; y < lightmapTmp.rows; y++){
       for( int x = 0; x < lightmapTmp.cols; x++ ){
           for( int c = 0; c < 3; c++ ){
                if( lightmapTmp.at<Vec3b>(y,x)[c] * qPow(2,value)>255){
                    lightmapTmp.at<Vec3b>(y,x)[c] = 255;
                    //saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
                }
                else{
                    lightmapTmp.at<Vec3b>(y,x)[c] *= qPow(2,value);
                }


            }
        }
    }
    /*
    if(value!=0){
        lightmapTmp *= qPow(2,value);
    }
    */

    imwrite( "/homes/td613/Documents/individual project/images/lightmap-gamma.png", lightmapTmp);
    QPixmap gammaLightmap("/homes/td613/Documents/individual project/images/lightmap-gamma.png");
    ui->label_lightmap->setPixmap(gammaLightmap);
    ui->label_lightmap->setScaledContents(true);
}

void MainWindow::on_spinBoxLightmap_valueChanged(int arg1)
{
    on_sliderLightmap_valueChanged(arg1);
}

Subdiv2D MainWindow::getSubdiv(){
    return subdiv;
}

void MainWindow::on_colorPicker_clicked()
{
    setColor();
}


void MainWindow::on_spbIntensity_editingFinished()
{

    if(activeLightSource>=0 && activeLightSource<=numberOfLights
            && light[activeLightSource]->lightIntensity != ui->spbIntensity->value()){
        light[activeLightSource]->lightIntensity = ui->spbIntensity->value();
        qDebug() << "----------------- relighting() change value in spin box----------------------------";
        relighting();
    }
}

/* ----------------------------------------------------------------
 *                  PAINTING FUNCTINS
 *-----------------------------------------------------------------*/
void MainWindow::on_buttonOnOff_clicked()
{
    if(isPaintingOn==false){
        isPaintingOn = true;
        ui->buttonOnOff->setText("ON");
        ui->buttonOnOff->setStyleSheet("color: #CC3B3B");
    }
    else if (isPaintingOn==true)  {
        isPaintingOn = false;
        ui->buttonOnOff->setText("OFF");
        ui->buttonOnOff->setStyleSheet("color: 0CB32E");
    }
}

