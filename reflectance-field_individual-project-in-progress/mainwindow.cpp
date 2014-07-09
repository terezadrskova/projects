#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include <QPixmap>
#include <QString>
#include <QtCore/qmath.h>
#include <QSlider>

#include <iostream>
#include <cmath>

#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLabel *label_pix = new QLabel;
    QPixmap lightmap(lightMapPathname);
    ui->label_pix->setPixmap(lightmap);
    ui->label_pix->setScaledContents(true);

    QLabel *label_pix_2 = new QLabel;
    QPixmap rfImage("/homes/td613/Documents/individual project/images/grace-cathedral/FINAL-RELIGHTED-image.png");
    ui->label_pix_2->setPixmap(rfImage);
    ui->label_pix_2->setScaledContents(true);

    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setValue(1);
    slider->setRange(0,1);

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

void MainWindow::on_Voronoi_Diagram_Button_clicked()
{
   createVoronoiDiagram();
}



void MainWindow::on_gammaButtton_clicked()
{
    LoadPPMAndSavePFM("/homes/td613/Documents/individual project/images/red-green-blue.ppm", "/homes/td613/Documents/individual project/images/red-green-blue.pfm");

}

void MainWindow::on_relightingButton_clicked()
{
    Mat relightedImg = relighting();
    imwrite( "/homes/td613/Documents/individual project/images/relightedImage.png", relightedImg);

    qDebug() << "Done";
}


void MainWindow::on_updateBtn_clicked()
{
    qDebug() << "button clicked";
}


void MainWindow::on_slider_valueChanged(int value)
{
    if(value==0){
        const char* pfmLightMapPath = lightMapPathname.toStdString().c_str();
        LoadPFMAndSavePPM(pfmLightMapPath, "/homes/td613/Documents/individual project/images/light-map.ppm");
        QPixmap newlightmap("/homes/td613/Documents/individual project/images/light-map.ppm");
        ui->label_pix->setPixmap(newlightmap);
    }
    else {
        QPixmap voronoiImage(voronoiPathname);
        ui->label_pix->setPixmap(voronoiImage);
    }
}
