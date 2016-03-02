#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	// Check for valid command line arguments, print usage
	// if no arguments were given.
	if (argc != 2) {
		cerr << "usage: " << argv[0] << " </path/to/haar_cascade>" << endl;
		cerr << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << endl;
		return -1;
	}
	// Get the path to your CSV:
	string fn_haar = string(argv[1]);
	int deviceId = 0;
	// These vectors hold the images and corresponding labels:
	vector<Mat> images;
	vector<int> labels;
	
	CascadeClassifier haar_cascade;
	haar_cascade.load(fn_haar);
	// Get a handle to the Video device:
	VideoCapture cap(deviceId);
	// Check if we can use this device at all:
	if(!cap.isOpened()) {
		cerr << "Capture Device ID " << deviceId <<"  cannot be opened." << endl;
		return -1;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	// Holds the current frame from the Video device:
	Mat frame;
	for(;;) {
		cap >> frame;
		// Clone the current frame:
		Mat original = frame.clone();
		// Convert the current frame to grayscale:
		Mat gray;
		cvtColor(original, gray, CV_BGR2GRAY);
		// Find the faces in the frame:
		vector< Rect_<int> > faces;
		haar_cascade.detectMultiScale(gray, faces);
		// At this point you have the position of the faces in
		// faces. Now we'll get the faces, make a prediction and
		// annotate it in the video. Cool or what?
		for(int i = 0; i < faces.size(); i++) {
			// Process face by face:
			Rect face_i = faces[i];
			// Crop the face from the image. So simple with OpenCV C++:
			Mat face = gray(face_i);
			// And finally write all we've found out to the original image!
			// First of all draw a green rectangle around the detected face:
			rectangle(original, face_i, CV_RGB(0, 255,0), 1);
		}
		// Show the result:
		namedWindow("face_recognizer", WINDOW_AUTOSIZE);
		imshow("face_recognizer", original);
		// And display it:
		char key = (char) waitKey(20);
		// Exit this loop on escape:
		if(key == 27)
			break;
	}
	return 0;
}
