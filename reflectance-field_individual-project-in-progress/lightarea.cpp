#include "lightarea.h"

#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <highgui.h>
#include <iostream>

#include <QColorDialog>
#include <QLabel>

using namespace cv;

LightArea::LightArea() :
    drawing_box(false),
    height(0),
    width(0)
{
}


void LightArea::draw_box( IplImage* img, CvRect rect ){
    //void rectangle(Mat& img, Rect rec, const Scalar& color, int thickness=1, int lineType=8, int shift=0 )
    cvRectangle( img, cvPoint(box.x, box.y), cvPoint(box.x+box.width,box.y+box.height),
                cvScalar(0xff,0x00,0x00) );
}

// Implement mouse callback
/*
void LightArea::my_mouse_callback(int event){

    switch( event ){
        case CV_EVENT_MOUSEMOVE:
            if( drawing_box ){
                box.width = x-box.x;
                box.height = y-box.y;
            }
            break;

        case CV_EVENT_RBUTTONDOWN:
            drawing_box = true;
            box = cvRect( x, y, 0, 0 );
            break;

        case CV_EVENT_RBUTTONUP:
            drawing_box = false;
            if( box.width < 0 ){
                box.x += box.width;
                box.width *= -1;
            }
            if( box.height < 0 ){
                box.y += box.height;
                box.height *= -1;
            }
            //draw_box( image, box );
            break;
    }
}

*/
