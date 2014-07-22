#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadPPM.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include <QPixmap>
#include <QString>
#include <QtCore/qmath.h>

#include <iostream>
#include <cmath>

#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;


/* ----------------------------------------------------------------
 *                EXTRACT FILE AND CONVERT TO 2D ARRAY
 *-----------------------------------------------------------------*/
void MainWindow::extractFile(const QString &pathName, bool isDirection)
{
    QFile inputFile(pathName);

    qDebug() << "Filepath: " << pathName;

     // Light Direction file couldn't be open
    if (!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "There has been an error while loading the file!";
        return;
     }


    // extract file
    QTextStream in(&inputFile);
    int i = 0;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        //splitting string into list of strings to seperate each light direction
        QRegExp rx("(\\ )"); //RegEx for ' '
        QStringList directionsList = line.split(rx);
        // extracking each token an inserting it into 2D array
        if (isDirection)
        {
            lightDirections[i][0] = directionsList.at(1).toFloat();
            lightDirections[i][1] = directionsList.at(2).toFloat();
            lightDirections[i][2] = directionsList.at(3).toFloat();
            if(i==252)
            {
              qDebug() << "Light Directions loaded successfully" << endl;
            }

        }


        // change light intensities
        else
        {

            lightIntensities[i][0] = directionsList.at(1).toFloat();
            lightIntensities[i][1] = directionsList.at(2).toFloat();
            lightIntensities[i][2] = directionsList.at(3).toFloat();
            if(i==252)
            {
              qDebug() << "Light Intensities loaded successfully";
            }
        }

        // got ot next line
        i++;
        inputFile.close();
     }

    return;
}

