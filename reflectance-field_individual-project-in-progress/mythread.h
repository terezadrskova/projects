#ifndef MYTHREAD_H
#define MYTHREAD_H

//#include <mainwindow.h>
#include <QtCore>
#include <QString>


class MainWindow;
class MyThread : public QThread
{
private:
    MainWindow *win;
    bool stop;
public:
    QString name;
    int number;
    MyThread(MainWindow *window);
    //overwriting public function run
    void run();

    void threadRelighting(int start, int end);


};

#endif // MYTHREAD_H
