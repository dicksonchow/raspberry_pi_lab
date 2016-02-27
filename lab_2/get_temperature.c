#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wiringPi.h>
#include <maxdetect.h>

#define LED_PIN 	0
#define	RHT03_PIN	4

void cleanup(int sig)
{
        printf("Detect Interrupt Signal.\n");
        pinMode(LED_PIN, OUTPUT);
        digitalWrite(0, HIGH);
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

int main (void)
{
	int temp, rh;
	int newTemp, newRh;
	char* str[80];

	temp = rh = newTemp = newRh = 0 ;

	if (catch_signal(SIGINT, cleanup) == -1){
                fprintf(stderr, "Failed to map the handler");
                exit(2);
        }

	if (wiringPiSetup() == -1){
                fprintf(stderr, "Failed to setup wiringPi...\n");
                return -1;
        }

	piHiPri(55);
	pinMode(LED_PIN, OUTPUT);

	for (;;)
	{
		delay (100);

		if (!readRHT03 (RHT03_PIN, &newTemp, &newRh))
			continue;

		if ((temp != newTemp) || (rh != newRh)){
			temp = newTemp;
			rh   = newRh;

			if ((temp & 0x8000) != 0){
				temp &= 0x7FFF ;
				temp = -temp ;
			}

			if (temp / 10.0 > 18){
				//sprintf(str, "The temperature is %5.1f.\nThe relative humidity is %5.1f%%\n", temp / 10.0, rh / 10.0);
				//char *arg[] = {"/usr/bin/espeak", &str, NULL};
				//execv("/usr/bin/espeak", arg);
				printf("The temperature is %5.1f.\nThe relative humidity is %5.1f%%\n", temp / 10.0, rh / 10.0);
				digitalWrite(LED_PIN, LOW);
				delay(500);
			}
			else
				printf("It is too cold\n");
			
		}
	}

	return 0 ;
}
