#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wiringPi.h>
#include <maxdetect.h>
#include <sys/wait.h>

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

void tell_tempture(float temp, float rh)
{
	char str[80];
	int pid_status = 0;

	pinMode(LED_PIN, OUTPUT);

	pid_t pid = fork();

	if (pid == -1){
		fprintf(stderr, "Fork failed. Program exited.\n");
		exit(1);
	}

	if (!pid){
		/* This is the child process. */
		sprintf(str, "The temperature is %5.1f degree celsius.\nThe relative humidity is %5.1f%%.\nThe light bulb will light up for a while.\n", temp / 10.0, rh / 10.0);
		execl("/usr/bin/espeak", "/usr/bin/espeak", str);
	}
	else{
		/* This is the parent process */
		if (waitpid(pid, &pid_status, 0) == -1)
			exit(1);
		digitalWrite(LED_PIN, LOW);
		delay(500);
		return;
	}
}

int main (int argc, char** argv)
{
	int temp, rh, newTemp, newRh, threshold;

	temp = rh = newTemp = newRh = 0 ;

	if (argc != 2){
		fprintf(stderr, "Usage: %s [temp]\n", argv[0]);
		exit(1);
	}

	threshold = atoi(argv[1]);

	if (catch_signal(SIGINT, cleanup) == -1){
                fprintf(stderr, "Failed to map the handler");
                exit(2);
        }

	if (wiringPiSetup() == -1){
                fprintf(stderr, "Failed to setup wiringPi...\n");
                return -1;
        }

	piHiPri(55);

	for (;;)
	{
		delay (500);
		
		if (!readRHT03 (RHT03_PIN, &newTemp, &newRh))
			continue;

		printf("The temperature is %5.1f. The relative humidity is %5.1f%%.\n", temp / 10.0, rh / 10.0);
		if ((temp != newTemp) || (rh != newRh)){
			temp = newTemp;
			rh   = newRh;

			if ((temp & 0x8000) != 0){
				temp &= 0x7FFF ;
				temp = -temp ;
			}

			if (temp / 10.0 > threshold)
				tell_tempture(temp, rh);
			
		}
	}

	return 0 ;
}
