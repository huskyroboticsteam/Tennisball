#include "detector.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace cv;
using namespace std;

const string WINDOW_TITLE = "Detection";
const string GRAPH_PATH = "./data/final_models/frozen_inference_graph.pb";
const string GRAPH_PBTXT = "./data/final_models/graph.pbtxt";

//set these as high as possible; the camera will be set to its maximum supported resolution
const int FRAME_WIDTH = 1280;
const int FRAME_HEIGHT = 720;

const bool USE_BLUR = true;
const int BLUR_SIZE = 1;

int main(int argc, char** argv) {
    int deviceID = 0; // 0 = open default camera
    if(argc > 1) {
        // second argument will be the device id
        string device = argv[1];
        deviceID = stoi(device);
    }

    Mat frame; // define a Mat to be used for the frames coming from the camera

    VideoCapture cap; // Initialize VideoCapture, this will be used for the camera

    int apiID = cv::CAP_ANY; // 0 = autodetect default API

	cout << "Opening camera..." << endl;
	cap.open(deviceID + apiID); // open selected camera using selected API
    if(!cap.isOpened())         // check if we succeeded
    {
        cerr << "ERROR! Unable to open camera\n"; // if not, print an error
        return -1;                                // and exit the program
    }

	cout << "Setting frame dimensions to " << FRAME_WIDTH << "x" << FRAME_HEIGHT << endl;
    cap.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    cout << "Frame dimensions set to " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << "x"
         << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

    tb::Detector detector(GRAPH_PATH, GRAPH_PBTXT);

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl << "Press any key to terminate" << endl;
    while(true) {

        cap.read(frame);  // wait for a new frame from camera and store it into 'frame'
        if(frame.empty()) // check if we succeeded
        {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        if(USE_BLUR) {
		  GaussianBlur(frame, frame, Size(BLUR_SIZE, BLUR_SIZE), 0);
        }

		frame.convertTo(frame, -1, 0.7, 0.9);

        vector<tb::Detection> detections = detector.performDetection(frame, 0.8);
		int i = 0;
        for(tb::Detection current : detections) {
		  cout << "(#" << i << " confidence: " << current.getConfidencePct() << "%) ";
            rectangle(frame, current.getBBoxRect(), Scalar(0, 255, 0), 2);
			drawMarker(frame, current.getBBoxCenter(), Scalar(0, 0, 255), MARKER_CROSS, 20, 2);
			i++;
        }
		if(i>0){
		  cout << endl;
		}

        imshow(WINDOW_TITLE, frame);

        if(waitKey(5) >= 0) // wait 5ms for a key to be pressed
            break;          // if key was pressed, break the while loop
    }
    return 0; // end program, the camera will be deinitialized automatically in VideoCapture destructor
}
