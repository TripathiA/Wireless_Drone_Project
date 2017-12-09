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

#define THRESHOLD_STRENGTH 61


MyThread::MyThread()
{
    gui = NULL;
    f = open("/home/aastha/Desktop/Wireless/log.txt", O_WRONLY|O_CREAT);
}

void MyThread::startSystem()
{
        start();
}

double MyThread::check_signal_strength(int sec)
{
	FILE *fp;
	int status;
	char strengthString[4];
	time_t start;
	time_t now;
	int64_t sum;
	int64_t count;

	start = time(0);
	now = time(0);
	sum = 0;
	count = 0;
	
	while (now - start < sec) {
		fp = popen("iwconfig wlp1s0 | grep Signal | cut -d\"=\" -f3|cut -d\" \" -f1", "r");
		if (fp == NULL) {
			perror("popen");
			exit(1);
		}

		if (fgets(strengthString, 4, fp) == NULL) {
			printf("can't get signal strength!\n");
			exit(1);
		}

		sum += atoi(strengthString);
		count += 1;

		if (pclose(fp) < 0) {
			perror("pclose");
			exit(1);
		}

		now = time(0);
	}
	double signal_strength = -1 * (sum/count);
	return signal_strength;
}

void MyThread::move(double init_ss)
{
	double curr_ss = init_ss;
	// i - 73 - forward
	// k - 75 - backwards
	// j - 74 - left
	// l - 76 - right
	// q - 81 - up
	// a - 65 - down
	// u - 85 - clokwise
	// o - 79 - counterclockwise
	int moves[] = {73,75,74,76};
	int neg_move = 0;
	for(int i = 0 ;i < 4; i++)
	{
		if(i%2 == 0) neg_move = moves[i+1];
		else neg_move = moves[i-1];
		
		printf("MY :: trying moving %d at %f\n",moves[i],time(0));

		QKeyEvent *event = new QKeyEvent(QKeyEvent::KeyPress, moves[i], Qt::NoModifier);
        	gui->keyPressEvent(event);
        	usleep(100000);
        	QKeyEvent *release = new QKeyEvent(QKeyEvent::KeyRelease, moves[i], Qt::NoModifier);
        	gui->keyReleaseEvent(release);

		double signal_strength = check_signal_strength(10);
		printf("MY :: Average Signal Strength: %f dbm at %f\n", signal_strength,time(0));

		if(init_ss - signal_strength  >= 2 || signal_strength <= THRESHOLD_STRENGTH )
		{
			printf("MY :: got better\n");
			if(signal_strength <= THRESHOLD_STRENGTH)
			{
				printf("MY :: now better\n");
				return;
			}
			else
			{
				printf("MY :: making more better moving %d \n",moves[i]);
				while(signal_strength > THRESHOLD_STRENGTH)
				{
					QKeyEvent *event = new QKeyEvent(QKeyEvent::KeyPress, moves[i], Qt::NoModifier);
                			gui->keyPressEvent(event);
                			usleep(100000);
                			QKeyEvent *release = new QKeyEvent(QKeyEvent::KeyRelease, moves[i], Qt::NoModifier);
                			gui->keyReleaseEvent(release);
					double signal_strength = check_signal_strength(5);
                			printf("MY :: Average Signal Strength: %f dbm at %f\n", signal_strength,time(0));
				}
				return;
			}
		}
		else
		{
			printf("MY :: got worse moving %d\n",neg_move);
			QKeyEvent *event = new QKeyEvent(QKeyEvent::KeyPress, neg_move, Qt::NoModifier);
                	gui->keyPressEvent(event);
                	usleep(100000);
                	QKeyEvent *release = new QKeyEvent(QKeyEvent::KeyRelease, neg_move, Qt::NoModifier);
                	gui->keyReleaseEvent(release);

		}
	}
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


    while (1) {

	double signal_strength = check_signal_strength(10);
	printf("MY :: Average Signal Strength :: run: %f dbm\n", signal_strength);


	if(signal_strength > THRESHOLD_STRENGTH)
	{
		printf("MY :: SS DROPPED\n");
		move(signal_strength);
	}
    }

   // while(i < 5)
   // {
   //     time_t now = time(0);
   //     while((now-start) < diff) now = time(0);
   //     snprintf(nowbuff, 16, "%lu ", now);
   //     snprintf(startbuff, 16, "%lu\n", start);
   //     printf("%s", nowbuff);
   //     printf("%s", startbuff);
   //     QKeyEvent *event = new QKeyEvent(QKeyEvent::KeyPress, 73, Qt::NoModifier);
   //     gui->keyPressEvent(event);
   //     usleep(100000);
   //     QKeyEvent *release = new QKeyEvent(QKeyEvent::KeyRelease, 73, Qt::NoModifier);
   //     gui->keyReleaseEvent(release);
   //     start = now;
   //     i++;
   //     diff = 5;
   // }

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
