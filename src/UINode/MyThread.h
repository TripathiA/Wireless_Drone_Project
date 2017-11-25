#ifndef MYTHREAD_H
#define MYTHREAD_H
#endif

#include <QThread>
#include <QMutex>

class tum_ardrone_gui;

class MyThread : public QThread
{
   Q_OBJECT

public:
   	tum_ardrone_gui* gui;
   	int f;

    MyThread(void);
    void isRunning();
    void startSystem();

protected:
   virtual void run();

// signals:
//    void signalValueUpdated(QString);

// private:
//     bool isRunning;

};
