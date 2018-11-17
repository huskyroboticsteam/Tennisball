#include "utils.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;
using namespace util;

// window titles
const String filteredTitle = "Live (Filtered)";
const String regularTitle = "Live";

// trackbar titles
const String hueLowTitle = "Hue Low";
const String hueHighTitle = "Hue High";
const String satLowTitle = "Sat Low";
const String satHighTitle = "Sat High";
const String valLowTitle = "Val Low";
const String valHighTitle = "Val High";

// maximum values for the sliders
const int maxBlur = 50;

// variables used to hold the values of the trackbars
int blurStrength = 0;

//// FUNCTION PROTOTYPES ////
void show(Mat image);
Mat morphed_img(Mat mask);
void showFiltered(Mat image, bool morph);
void setupMainTrackbars();
/////////////////////////////

int main() {
  Mat frame; // define a Mat to be used for the frames coming from the camera

  VideoCapture cap;        // Initialize VideoCapture, this will be used for the camera
  int deviceID = 0;        // 0 = open default camera
  int apiID = cv::CAP_ANY; // 0 = autodetect default API

  cap.open(deviceID + apiID); // open selected camera using selected API
  if(!cap.isOpened())         // check if we succeeded
  {
    cerr << "ERROR! Unable to open camera\n"; // if not, print an error
    return -1;                                // and exit the program
  }

  //--- GRAB AND WRITE LOOP
  cout << "Start grabbing" << endl << "Press any key to terminate" << endl;
  bool setup = false; // used so that we perform setup of stuff like trackbars only once
  while(true) {

    cap.read(frame);  // wait for a new frame from camera and store it into 'frame'
    if(frame.empty()) // check if we succeeded
    {
      cerr << "ERROR! blank frame grabbed\n";
      break;
    }

    show(frame);                // display the original frame
    showFiltered(frame, false); // display the results of filtering on the frame

    if(!setup) {                                 // if it hasn't been performed yet,
      util::setupFilterTrackbars(filteredTitle); //  perform first time setup on filtering trackbars
      setupMainTrackbars();                      // sets up the "blur" trackbar on the main window
      setup = true;                              // set setup to true so we don't do this again
    }

    if(waitKey(5) >= 0) // wait 5ms for a key to be pressed
      break;            // if key was pressed, break the while loop
  }
  return 0; // end program, the camera will be deinitialized automatically in VideoCapture destructor
}

void show(Mat image) {
  // apply gaussian blur to image
  GaussianBlur(image, image, Size(blurStrength * 2 + 1, blurStrength * 2 + 1), 0, 0);
  imshow(regularTitle, image); // display image
}

Mat morphed_img(Mat mask) {
  Mat se21 = getStructuringElement(MORPH_RECT, Size(21, 21));
  Mat se11 = getStructuringElement(MORPH_RECT, Size(11, 11));

  morphologyEx(mask, mask, MORPH_CLOSE, se21);
  morphologyEx(mask, mask, MORPH_OPEN, se11);

  GaussianBlur(mask, mask, Size(15, 15), 0, 0);
  return (mask);
}

void showFiltered(Mat image, bool morph) {
  Scalar lowHSV = util::getLowHSV(filteredTitle);
  Scalar highHSV = util::getHighHSV(filteredTitle);
  Mat filtered = util::thresholdImage(image, lowHSV, highHSV); // perform threshold filtering on the image
  if(morph) {                                                  // if the morph parameter is true
    Mat morphed = morphed_img(filtered);                       // morph the image
    imshow(filteredTitle, morphed);                            // show the morphed image
  } else {                                                     // otherwise
    imshow(filteredTitle, filtered);                           // just show the filtered image
  }
}

void setupMainTrackbars() {
  createTrackbar("Blur", regularTitle, &blurStrength, maxBlur); // set up blur trackbar
}
