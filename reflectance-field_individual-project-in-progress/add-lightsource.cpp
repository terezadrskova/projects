#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"

#include <QDebug>
#include <iostream>

#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;


/* ----------------------------------------------------------------
 *                   ADD LIGHTSOURCE
 *-----------------------------------------------------------------*/

//const QPoint & QMouseEvent::pos () const
