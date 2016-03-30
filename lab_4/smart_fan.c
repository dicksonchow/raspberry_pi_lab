#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wiringPi.h>
#include <maxdetect.h>
#include <sys/wait.h>

#define RELAY_PIN 	1
#define	RHT03_PIN	4

void cleanup(int sig)
{
        printf("Detect Interrupt Signal.\n");
        pinMode(RELAY_PIN, OUTPUT);
        digitalWrite(RELAY_PIN, LOW);
        exit(0);
}

int catch_signal(int sig, void (*handler)(int))
{
        struct sigaction action;
        action.sa_handler = handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        return sigaction (sig, &action, NULL);
}

int main (int argc, char** argv)
{
	double threshold;
	int temp, rh; 

	temp = rh = threshold = 250 ;

	if (argc != 2){
		fprintf(stderr, "Usage: sudo %s [temperature]\n", argv[0]);
		fprintf(stderr, "\t[temperature] - threshold temperature to turn on the fan\n");
		fprintf(stderr, "\nExample: sudo %s 18\n", argv[0]);
		fprintf(stderr, "This program will take the first command line argument as a threshold temperature, 18 degree Celsius in the above example.\nWhenever the room temperature is higher than the threshold temperature you supplied to the program, the fan will start rotating.\nHave Fun!\n");
		return 1;
	}

	if (catch_signal(SIGINT, cleanup) == -1){
                fprintf(stderr, "Failed to map the handler");
                return 1;
        }

	if (wiringPiSetup() == -1){
                fprintf(stderr, "Failed to setup wiringPi...\n");
                return 1;
        }

	threshold = atof(argv[1]);
	printf("Threshold temperature: %5.1f degree\n", threshold);
	piHiPri(55);
	pinMode(RELAY_PIN, OUTPUT);

	for (;;)
	{
		delay (500);
		
		if (!readRHT03 (RHT03_PIN, &temp, &rh))
			continue;

		printf("The temperature is %5.1f. The relative humidity is %5.1f%%.\n", temp / 10.0, rh / 10.0);

		if ((temp & 0x8000) != 0){
			temp &= 0x7FFF ;
			temp = -temp ;
		}

		(temp / 10.0 > threshold)
			? digitalWrite(RELAY_PIN, HIGH) : digitalWrite(RELAY_PIN, LOW);
			
	}

	return 0 ;
}
