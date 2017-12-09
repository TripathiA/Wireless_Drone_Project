#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define MONITOR_PERIOD 5

int main()
{
	FILE *fp;
	int status;
	char strengthString[4];
	time_t start;
	time_t now;
	int64_t sum;
	int64_t count;

	while (1) {
		start = time(0);
		now = time(0);
		sum = 0;
		count = 0;
		while (now - start < MONITOR_PERIOD) {
			fp = popen("iwconfig wlo1 | grep Signal | cut -d\"=\" -f3|cut -d\" \" -f1", "r");
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

		printf("Average Signal Strength: %f dbm\n", 1.0 * sum / count);
	}

	return 0;
}
