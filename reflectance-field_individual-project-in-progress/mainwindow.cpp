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

    QPixmap pix("/homes/td613/Documents/individual project/images/Gray_Image.jpg");
    ui->label_pix->setPixmap(pix);
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
