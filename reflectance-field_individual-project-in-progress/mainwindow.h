#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QString>
#include <QList>

#include <cv.h>
#include <highgui.h>


using namespace cv;

// CONTANSTS
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
    Mat relighting();

    // gamma
    Mat addGamma(Mat& img, double gamma);
    Mat removeGamma(Mat& img, double gamma);

    // image manipulation


    ~MainWindow();
    
private slots:
    void on_actionLoad_Reflectance_Field_Images_triggered();
    void on_actionLoad_Light_Map_image_triggered();
    void on_Voronoi_Diagram_Button_clicked();
    void on_gammaButtton_clicked();
    void on_relightingButton_clicked();

    void on_updateBtn_clicked();

    void on_slider_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    float* lightProbePFM;

    Mat lightMap;
    Mat resultImage;
    Mat resultLightMap;
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
};

#endif // MAINWINDOW_H
