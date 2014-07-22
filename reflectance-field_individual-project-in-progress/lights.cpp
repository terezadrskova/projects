#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lightsource.h"
#include "loadPPM.h"

#include <QtGui>
#include <QLabel>
#include <QList>
#include <QColorDialog>

#include <cv.h>
#include <highgui.h>

using namespace cv;

 void MainWindow::dragEnterEvent(QDragEnterEvent *event)
 {
     if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
         if (event->source() == this) {
             event->setDropAction(Qt::MoveAction);
             event->accept();
         } else {
             event->acceptProposedAction();
         }
     } else {
         event->ignore();
     }
 }

 void MainWindow::dragMoveEvent(QDragMoveEvent *event)
 {
     if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
         if (event->source() == this) {
             event->setDropAction(Qt::MoveAction);
             event->accept();
         } else {
             event->acceptProposedAction();
         }
     } else {
         event->ignore();
     }
 }

 void MainWindow::dropEvent(QDropEvent *event)
 {
     if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
         // change coordinates/nearestcell of the light source
         if(activeLightSource!=-1 && activeLightSource<light.size()){
             QPoint thisPoint(event->pos());
             Point2f cvPoint(thisPoint.x(),thisPoint.y());
             light[activeLightSource]->pos = cvPoint;
             Point2f nearestCell;
             subdiv.findNearest(cvPoint,&nearestCell);
             light[activeLightSource]->cellToRelit = findNearestCell(nearestCell);
             qDebug() << "cell to relit: " << light[activeLightSource]->cellToRelit;
             relighting();
         }

         QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
         QDataStream dataStream(&itemData, QIODevice::ReadOnly);

         QPixmap pixmap;
         QPoint offset;
         dataStream >> pixmap >> offset;

         QLabel *newIcon = new QLabel(this);
         newIcon->setPixmap(pixmap);
         newIcon->move(event->pos() - offset);
         newIcon->show();
         newIcon->setAttribute(Qt::WA_DeleteOnClose);

         if (event->source() == this) {
             event->setDropAction(Qt::MoveAction);
             event->accept();
         } else {
             event->acceptProposedAction();
         }
     } else {
         event->ignore();
     }
 }

 void MainWindow::mousePressEvent(QMouseEvent *event)
 {
     this->x = event->x();
     this->y = event->y();
     Point2f position(x,y);

     // drag and drop on the left click
     if (event->button() == Qt::LeftButton){
         int localX,localY;
         localX = (x-557)*1.538461538; // 65%
         localY = (y-166)*1.538461538; // 65%
         if(localX>1024 || localX<0 || localY>512 || localY<0){
             // return if you are not in the lightsource widget
             return;
         }

         // check which object is moving and save an index
         Point2f eventPos(event->x(),event->y());
         activeLightSource = closestLightSource(eventPos);

         QString activeLightTx = QString("Lightsource: %1").arg(activeLightSource);
         ui->activeLightLb->setText(activeLightTx);

         ui->spbIntensity->setValue(light[activeLightSource]->lightIntensity);


         QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
         if (!child)
             return;

         QPixmap pixmap = *child->pixmap();

         QByteArray itemData;
         QDataStream dataStream(&itemData, QIODevice::WriteOnly);
         dataStream << pixmap << QPoint(event->pos() - child->pos());

         QMimeData *mimeData = new QMimeData;
         mimeData->setData("application/x-dnditemdata", itemData);

         QDrag *drag = new QDrag(this);
         drag->setMimeData(mimeData);
         drag->setPixmap(pixmap);
         drag->setHotSpot(event->pos() - child->pos());

         QPixmap tempPixmap = pixmap;
         QPainter painter;
         painter.begin(&tempPixmap);
         painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
         painter.end();

         child->setPixmap(tempPixmap);

         if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
             child->close();
         else {
             child->show();
             child->setPixmap(pixmap);
         }
     }

     // ************ ADDING A LIGHT SOURCE THROUGH RIGHT CLICK **********************


     // create a new light source on right click
     if (event->button() == Qt::RightButton)
     {
        // convert values from MainWindow to local coordinates
        int localX,localY;
        localX = (x-557)*1.538461538; // 65%
        localY = (y-166)*1.538461538; // 65%
        Point2f localPoint(localX,localY);

        if(localX>1024 || localX<0 || localY>512 || localY<0){
            // return if you are not in the lightsource widget
            return;
        }

        qDebug() << "coordinates: " << localX << localY;
        qDebug() << "right mouse click - create a new lightsource";
        setAcceptDrops(true);



        // created lightsource is the closest one
        QString activeLightTx2 = QString("Lightsource: %1").arg(numberOfLights);
        ui->activeLightLb->setText(activeLightTx2);
        activeLightSource = numberOfLights;
        light.resize(numberOfLights);
        qDebug() << "number of lights" << numberOfLights;

        // -------------------- change COLOUR of the light source -------------------------
        int redCh = red;
        int greenCh = green;
        int blueCh = blue;

        qDebug() << "new light source " << red << green << blue;
        Mat channel[4];
        split(lightImg, channel);

        // multiplying each channel with corresponding colour
        channel[2] = red;
        channel[1] = green;
        channel[0] = blue;

        //Merging red and green channels
        merge(channel,4,lightImg);
        imwrite("/homes/td613/Documents/individual project/images/light-out.png", lightImg);

        // creates a new labal with lightsource
        QLabel *lbLight = new QLabel(this);
        lbLight->setPixmap(QPixmap("/homes/td613/Documents/individual project/images/light-out.png"));
        //lbLight->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        lbLight->move(x, y);
        lbLight->setAttribute(Qt::WA_DeleteOnClose);

        QList<QLabel*> listLight;
        listLight.append(lbLight);

        // display icon for the new lightsource
        QVBoxLayout *vbl = new QVBoxLayout;
        vbl->addWidget(lbLight);
        lbLight->show();

        Q_INIT_RESOURCE(draggableicons);


        // ----------- RELIGHTING OF THE CELL -------------

        // find cell to relight
        Point2f nearestCell;
        subdiv.findNearest(localPoint,&nearestCell);
        int cellForRelighting = findNearestCell(nearestCell);
        qDebug() << "cell to relit: " << cellForRelighting;

        // ----------------- change INTENSITY of the light source
        int intensity = 50;

        light.push_back(new LightSource(intensity, redCh, greenCh, blueCh, position,cellForRelighting, listLight));
        numberOfLights++;
     }
     qDebug() << "----------------- relighting() after mouse drag----------------------------";
     relighting();

 }

 void MainWindow::setColor()
 {
     color = QColorDialog::getColor(Qt::blue, this);
     if (color.isValid())
     {
         if(activeLightSource!=-1){
            light[activeLightSource]->redC = color.red();
            light[activeLightSource]->greenC = color.green();
            light[activeLightSource]->blueC = color.blue();
            qDebug() << "color name: " << color.name() << "red: " << light[activeLightSource]->redC
                  << "blue: " << light[activeLightSource]->blueC
                  << "green: " << light[activeLightSource]->greenC ;
         }
         red = color.red();
         green = color.green();
         blue = color.blue();
     }
 }

 /* ----------------------------------------------------------------
  *                  UPDATE COLOR
  *-----------------------------------------------------------------*/
 void MainWindow::on_pushButton_2_clicked()
 {
     qDebug() << "before the if statement";
     if(activeLightSource!=-1){
        // updating color values of the active light source
        qDebug() << "new RGB" << red << green << blue;
        light[activeLightSource]->redC = red;
        light[activeLightSource]->greenC = green;
        light[activeLightSource]->blueC = blue;

        // fourth channel is transparency
        Mat ch[4];
        split(lightImg, ch);

        // multiplying each channel with corresponding colour
        ch[2] = red;
        ch[1] = green;
        ch[0] = blue;

        //Merging red and green channels
        merge(ch,4,lightImg);
        imwrite("/homes/td613/Documents/individual project/images/new-light-out.png", lightImg);

        // updates label with a new pixmap colored image
        light[activeLightSource]->lbLight[0]->setPixmap(QPixmap("/homes/td613/Documents/individual project/images/new-light-out.png"));
     }
 }
