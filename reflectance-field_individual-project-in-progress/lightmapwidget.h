#ifndef LIGHTMAPWIDGET_H
#define LIGHTMAPWIDGET_H

#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class lightMapWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit lightMapWidget(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void leaveEvent(QEvent *ev);

    int x,y;
    
signals:
    
public slots:
    
};

#endif // LIGHTMAPWIDGET_H
