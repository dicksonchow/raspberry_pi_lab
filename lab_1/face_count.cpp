#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <iostream>
#include <stdlib.h>
#include <wiringPi.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <mysql++.h>
#include "userclass.h"

#define LED_PIN 0
#define MOTION_PIN 2

using namespace cv;
using namespace std;

void cleanup(int sig)
{
        cout << "Detect Interrupt Signal." << endl;
        pinMode(0, OUTPUT);
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


int main(int argc, const char *argv[]) {
	if (catch_signal(SIGINT, cleanup) == -1){
                cerr << "Failed to map the handler" << endl;
                exit(2);
        }
	// Get the path to your CSV:
	string fn_haar = "/usr/share/faces_meta/haarcascade_frontalface_alt.xml";
	int deviceId = 0;
	// These vectors hold the images and corresponding labels:
	vector<Mat> images;
	vector<int> labels;

	CascadeClassifier haar_cascade;
	haar_cascade.load(fn_haar);
	// Get a handle to the Video device:
	VideoCapture cap(deviceId);
	// Check if we can use this device at all:
	if (!cap.isOpened()) {
		cerr << "Capture Device ID " << deviceId << " cannot be opened." << endl;
		return -1;
	}

	if (wiringPiSetup() == -1){
		cerr << "Failed to setup wiringPi..." << endl;
		return -1;
	}
	//pinMode(LED_PIN, OUTPUT);
	// Holds the current frame from the Video device:
	Mat frame;
	//Mat background = imread("/usr/share/faces_meta/door_locked.jpg", CV_LOAD_IMAGE_COLOR);

	for(;;)
	{
		cap >> frame;
		// Clone the current frame:
		Mat original = frame.clone();
		// Convert the current frame to grayscale:
		Mat gray;
		cvtColor(original, gray, CV_BGR2GRAY);
		// Find the faces in the frame:
		vector< Rect_<int> > faces;
		haar_cascade.detectMultiScale(gray, faces);

		cout << "Count some faces: " << faces.sizes() << endl;

	}
	return 0;
}
