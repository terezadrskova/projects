#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"
#include "lightmapwidget.h"
#include "dragwidget.h"
#include "colordialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include <QPixmap>
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
    blue(255)
{
    ui->setupUi(this);

    // initilise light source icon - correcting transparency
    lightImg = imread("/homes/td613/Documents/individual project/images/light.png", -1);

    // lightmap
    QLabel *label_lightmap = new QLabel;
    if(lightMapPathname!=""){
        const char* pfmLightMapPath = lightMapPathname.toStdString().c_str();
        LoadPFMAndSavePPM(pfmLightMapPath, "/homes/td613/Documents/individual project/images/light-map.ppm");
        QPixmap lightmap("/homes/td613/Documents/individual project/images/light-map.ppm");
        ui->label_lightmap->setPixmap(lightmap);
        ui->label_lightmap->setScaledContents(true);
    }

    // voronoi
    QLabel *label_voronoi = new QLabel;
    QPixmap voronoiImage(voronoiPathname);
    ui->label_voronoi->setPixmap(voronoiImage);
    ui->label_voronoi->setScaledContents(true);


    // relighted image
    QLabel *label_pix_2 = new QLabel;
    QPixmap rfImage("/homes/td613/Documents/individual project/images/grace-cathedral/FINAL-RELIGHTED-image.png");
    ui->label_pix_2->setPixmap(rfImage);
    ui->label_pix_2->setScaledContents(true);

}

MainWindow::~MainWindow()
{
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
    // make image lighter - add Gamma
    if(value!=0){
        lightmapTmp *= qPow(2,value);
    }
    //Mat mask,dst;
    //inRange(lightmapTmp,Scalar(0),Scalar(1),mask); // Create a mask image in the range 300 and 400
    //lightmapTmp.copyTo(dst,mask);
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


void MainWindow::on_spbIntensity_editingFinished()
{
    if(activeLightSource>=0 && activeLightSource<=numberOfLights){
        light[activeLightSource]->lightIntensity = ui->spbIntensity->value();
        qDebug() << "----------------- relighting() change value in spin box----------------------------";
        relighting();
    }
}

void MainWindow::on_colorPicker_clicked()
{
    setColor();
}

