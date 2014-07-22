#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <highgui.h>
#include <iostream>

#include <QColorDialog>
#include <QLabel>

using namespace cv;

class LightSource
{
public:
    int lightIntensity;
    int redC;
    int greenC;
    int blueC;
    Point2i pos;
    int cellToRelit;
    QList<QLabel*> lbLight;

    LightSource(int &_lightIntensity, int &_redC, int &_greenC, int &_blueC, Point2f &_pos, int &_cellToRelit,  QList<QLabel*> &_lbLight){
        lightIntensity = _lightIntensity;
        redC = _redC;
        greenC = _greenC;
        blueC = _blueC;
        pos = _pos;
        cellToRelit = _cellToRelit;
        lbLight = _lbLight;
    }
};

#endif // LIGHTSOURCE_H
