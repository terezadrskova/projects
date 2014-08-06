#ifndef LIGHTAREA_H
#define LIGHTAREA_H

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <highgui.h>
#include <iostream>

#include <QColorDialog>
#include <QLabel>
#include <QColor>

using namespace cv;

class LightArea
{
private:

public:
    // to be accessed from the main window
    Point2f pos;
    QColor color;
    float width;
    float height;
    CvRect box;
    bool drawing_box;


    LightArea();
    //void my_mouse_callback(int event);
    void draw_box( IplImage* img, CvRect rect );
};

#endif // LIGHTAREA_H
