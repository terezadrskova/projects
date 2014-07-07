#ifndef VORONOIDIAGRAM_H
#define VORONOIDIAGRAM_H

#include <cv.h>
#include <highgui.h>
#include <QString>

using namespace cv;

class voronoiDiagram : public QMainWindow
{
public:
    //standard functions go here
    //void createVoronoi(QString &pathname, MainWindow &win);
    void drawSubdivPoint(Mat& img, Point2f fp, Scalar color);
    void drawSubdiv(Mat& img, Subdiv2D& subdiv, Scalar delaunayColor);
    void locatePoint(Mat& img, Subdiv2D& subdiv, Point2f fp, Scalar activeColor);
    void paintVoronoi(Mat& img, Subdiv2D& subdiv);

    voronoiDiagram();
private slots:
    // triggered functions go here

private:
    //attributes go here
};


#endif // VORONOIDIAGRAM_H
