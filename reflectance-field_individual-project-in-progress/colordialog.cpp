#include "colordialog.h"

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QString>

#include <cv.h>
#include <highgui.h>

using namespace cv;

ColorDialog::ColorDialog(QWidget *parent)
    : QDialog(parent),
    green(255),
    blue(255),
    red(255)
{
    //button = new QPushButton("Select Color", this);
    //connect(button, SIGNAL(clicked()), this, SLOT(setcolor()));

    //colorLabel =new QLabel(this);
    //layout = new QVBoxLayout(this);
    //layout->addWidget(button);
    //layout->addWidget(colorLabel);
    //setLayout(layout);
}

ColorDialog::~ColorDialog()
{
// No need to delete any object that got a parent that is properly deleted.
}
