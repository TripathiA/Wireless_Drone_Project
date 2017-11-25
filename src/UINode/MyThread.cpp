#include "MyThread.h"
#include <QString>
#include <time.h>
#include <QtGui/QWidget>
#include <QApplication>
#include "tum_ardrone_gui.h"
#include <fcntl.h>
#include <unistd.h>
#include <sstream>
#include <string>

MyThread::MyThread()
{
    gui = NULL;
    f = open("/home/aastha/Desktop/Wireless/log.txt", O_WRONLY|O_CREAT);
}

void MyThread::startSystem()
{
        start();
}


void MyThread::run()
{
    qDebug("Thread id inside run %d",(int)QThread::currentThreadId());

    close(1);
    dup(f);
    time_t start = time(0);

    std::stringstream ss;      
    char nowbuff[16];
    char startbuff[16];

    int i = 0;
    int diff = 10;
    while(i < 5)
    {
        time_t now = time(0);
        while((now-start) < diff) now = time(0);
        snprintf(nowbuff, 16, "%lu ", now);
        snprintf(startbuff, 16, "%lu\n", start);
        printf("%s", nowbuff);
        printf("%s", startbuff);
        QKeyEvent *event = new QKeyEvent(QKeyEvent::KeyPress, 73, Qt::NoModifier);
        gui->keyPressEvent(event);
        sleep(1);
        QKeyEvent *release = new QKeyEvent(QKeyEvent::KeyRelease, 73, Qt::NoModifier);
        gui->keyReleaseEvent(release);
        start = now;
        i++;
        diff = 5;
    }

    printf("done\n");
    close(f);
    // static int value=0; //If this is not static, then it is reset to 0 every time this function is called.
    // isRunning = 1;
    // while(isRunning == 1)
    // {
    //     QString string = QString("value: %1").arg(value++);
    //     sleep(1/1000); //If this isn't here, the counter increments way too fast and dies, or something; the app freezes, anyway.

    //     emit signalValueUpdated(string);       
    // }            
}

void MyThread::isRunning()
{
    printf("RUNNING THREAD\n");
}