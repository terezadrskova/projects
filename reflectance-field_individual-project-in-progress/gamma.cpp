#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"

#include <QDebug>
#include <iostream>

#include <cv.h>
#include <highgui.h>

using namespace cv;


/* ----------------------------------------------------------------
 *                   ADD GAMMA
 *-----------------------------------------------------------------*/
Mat MainWindow::addGamma(Mat& img, double gamma) {
 double inverse_gamma = 1.0 / gamma;

 Mat lut_matrix(1, 256, CV_8U );
 uchar * ptr = lut_matrix.ptr();
 for( int i = 0; i < 256; i++ )
   ptr[i] = (int)( pow( (double) i / 255.0, inverse_gamma ) * 255.0 );

 Mat result;
 LUT( img, lut_matrix, result );

 return result;
}


/* ----------------------------------------------------------------
 *                   REMOVE GAMMA
 *-----------------------------------------------------------------*/
Mat MainWindow::removeGamma(Mat& img, double gamma) {

 Mat lut_matrix(1, 256, CV_8UC1 );
 uchar * ptr = lut_matrix.ptr();
 for( int i = 0; i < 256; i++ )
   ptr[i] = (int)( pow( (double) i / 255.0, gamma ) * 255.0 );

 Mat result;
 LUT( img, lut_matrix, result );

 return result;
}

