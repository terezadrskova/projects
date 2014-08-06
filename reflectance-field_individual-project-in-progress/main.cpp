#include "mainwindow.h"
#include "loadPPM.h"

#include <QApplication>
#include <GL/glut.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include <QBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <dragwidget.h>






int main(int argc, char *argv[])
{

    glutInit(&argc, argv);

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Reflectance Field Relighting");

    w.show();
    return a.exec();


}
