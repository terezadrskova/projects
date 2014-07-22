#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lightsource.h"

#include <QMainWindow>
#include <QVector>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QString>
#include <QList>
#include <QColor>

#include <cv.h>
#include <highgui.h>
#include <iostream>

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

// CONTANTS
const QString LIGHTDIRECTIONSPATH = "/homes/td613/Documents/individual project/individual-project/editing-gui/config/light_directions.txt";
const QString LIGHTINTENSITIESPATH = "/homes/td613/Documents/individual project/individual-project/editing-gui/config/light_intensities.txt";
const double PI = 3.14159265359;

const int LATITUDE = 1024;
const int LONGTITUDE = 512;
const int COLORCOMPONENTS = 3;
const int NUMBEROFLIGHTSOURCES = 253;

const QString voronoiPathname = "/homes/td613/Documents/individual project/images/voronoi-image.png";
//const char * ppmLightMapPath = "/homes/td613/Documents/individual project/images/light-map.ppm";

const double GAMMA = 2.2;

//class LightSource;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);

    // math functions
    Mat convertToFloatingPoint(Mat &image);
    int flipXAxis(int theta);
    int flipYAxis(int phi);

    // directions and intensities
    void extractFile(const QString &pathName, bool isDirection);
    void calculateDirections();
    void calculateIntensities();
    void calculateFinalIntensities(Subdiv2D& subdiv);

    // voronoi diagram
    void drawSubdivPoint(Mat& img, Point2f fp, Scalar color);
    void drawSubdiv(Mat& img, Subdiv2D& subdiv, Scalar delaunayColor);
    void locatePoint(Mat& img, Subdiv2D& subdiv, Point2f fp, Scalar activeColor);
    void paintVoronoi(Mat& img, Subdiv2D& subdiv);
    int findNearestCell(Point2f &point);

    // loading images
    void loadRFImages();
    void loadLightMap();
    void createVoronoiDiagram();

    // relighting
    void relighting();

    // gamma
    Mat addGamma(Mat& img, double gamma);
    Mat removeGamma(Mat& img, double gamma);

    // lightsources and draggable
    Subdiv2D getSubdiv();
    int closestLightSource(Point2f whereClicked);

    // color picker
    void setColor();

    // public attributes so can be access by LightSource class
    Rect rect;
    Subdiv2D subdiv;
    QVector<LightSource*> light;
    int activeLightSource;

    ~MainWindow();
    
private slots:
    void on_actionLoad_Reflectance_Field_Images_triggered();
    void on_actionLoad_Light_Map_image_triggered();
    void on_relightingButton_clicked();
    void on_updateBtn_clicked();

    void on_sliderVoronoi_actionTriggered(int action);
    void on_spinBoxVoronoi_valueChanged(int arg1);

    void on_sliderLightmap_valueChanged(int value);
    void on_spinBoxLightmap_valueChanged(int arg1);

    void on_spbIntensity_editingFinished();

    void on_colorPicker_clicked();

    void on_pushButton_2_clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;

    float* lightProbePFM;
    int x,y,;

    Mat lightMap,resultImage,resultLightMap;
    QVector<Mat> arrayOfRFImages;

    float lightDirections[NUMBEROFLIGHTSOURCES][COLORCOMPONENTS];
    float lightIntensities[NUMBEROFLIGHTSOURCES][COLORCOMPONENTS];

    float calculatedDirections[NUMBEROFLIGHTSOURCES][2];
    float calculatedIntensities[NUMBEROFLIGHTSOURCES][COLORCOMPONENTS];

    int sphericalCoordinatesPX[NUMBEROFLIGHTSOURCES][2];
    float finalLightStageIntensities[NUMBEROFLIGHTSOURCES];
    float voronoiIntensities[NUMBEROFLIGHTSOURCES][4];
    float finalVoronoiIntensities[NUMBEROFLIGHTSOURCES][COLORCOMPONENTS];
    float finalCellIntensity[NUMBEROFLIGHTSOURCES];
    float finalVoronoiColors[NUMBEROFLIGHTSOURCES][COLORCOMPONENTS];
    float averageTheta[NUMBEROFLIGHTSOURCES];

    QStringList lightMapPath;
    QStringList rfImagesPath;
    QString lightMapPathname;

    // lightsources
    int numberOfLights;
    Mat lightImg;

   // color dialog
    QColor color;
    int red;
    int green;
    int blue;
};





#endif // MAINWINDOW_H
