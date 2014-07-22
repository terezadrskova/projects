#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <iostream>

#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;


void clamp(Mat& mat, Point2f lowerBound, Point2f upperBound) {
    vector<Mat> matc;
    split(mat, matc);
    min(max(matc[0], lowerBound.x), upperBound.x, matc[0]);
    min(max(matc[1], lowerBound.y), upperBound.y, matc[1]);
    merge(matc, mat);
}
