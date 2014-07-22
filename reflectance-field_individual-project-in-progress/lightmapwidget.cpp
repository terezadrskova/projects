#include "mainwindow.h"
#include "lightmapwidget.h"

lightMapWidget::lightMapWidget(QWidget *parent) :
    QMainWindow(parent)
{
}

void lightMapWidget::mousePressEvent(QMouseEvent *ev)
{
    emit MainWindow::mouse_Pressed();
}


void lightMapWidget::mouseMoveEvent(QMouseEvent *ev)
{
    this->x = ev->x();
    this->y = ev->y();
    emit MainWindow::mouse_Pos();
}

void lightMapWidget::leaveEvent(QEvent *ev)
{
    emit MainWindow::mouse_Left();
}

