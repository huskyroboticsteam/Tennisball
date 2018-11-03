#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>
using namespace cv;
using namespace std;

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
const int maxHue = 180;
const int maxVal = 255;
const int maxBlur = 50;

// variables used to hold the values of the trackbars
int lowH, lowS, lowV = 0;
int highH, highS, highV = 50;
int blurStrength = 0;

//// FUNCTION PROTOTYPES ////
void show(Mat image);
void showGray(Mat image);
void showBGR(Mat image);
Mat morphed_img(Mat mask);
Mat threshold_image(Mat img);
void showFiltered(Mat image, bool morph);
void setupFilterTrackbars();
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

        if(!setup) {                // if it hasn't been performed yet,
            setupFilterTrackbars(); //  perform first time setup on filtering trackbars
            setupMainTrackbars();   // sets up the "blur" trackbar on the main window
            setup = true;           // set setup to true so we don't do this again
        }

        if(waitKey(5) >= 0) // wait 5ms for a key to be pressed
            break;          // if key was pressed, break the while loop
    }
    return 0; // end program, the camera will be deinitialized automatically in VideoCapture destructor
}

void show(Mat image) {
    // apply gaussian blur to image
    GaussianBlur(image, image, Size(blurStrength * 2 + 1, blurStrength * 2 + 1), 0, 0);
    imshow(regularTitle, image); // display image
}

void showGray(Mat image) {
    Mat gray_image;
    cvtColor(image, gray_image, COLOR_RGB2GRAY);
    imshow("Live (Grayscale)", gray_image);
}

void showBGR(Mat image) {
    Mat bgr_image;
    cvtColor(image, bgr_image, COLOR_RGB2BGR);
    imshow("Live (BGR)", bgr_image);
}

Mat morphed_img(Mat mask) {
    Mat se21 = getStructuringElement(MORPH_RECT, Size(21, 21));
    Mat se11 = getStructuringElement(MORPH_RECT, Size(11, 11));

    morphologyEx(mask, mask, MORPH_CLOSE, se21);
    morphologyEx(mask, mask, MORPH_OPEN, se11);

    GaussianBlur(mask, mask, Size(15, 15), 0, 0);
    return (mask);
}

Mat threshold_image(Mat img) {
    Mat hsv(img.rows, img.cols, CV_8UC3);    // create Mat to hold HSV image
    cvtColor(img, hsv, CV_RGB2HSV);          // convert image to HSV
    Mat thresh(img.rows, img.cols, CV_8UC1); // create Mat to hold threshold results
    Scalar lowHSV(lowH, lowS, lowV);         // define Scalar to hold lower limits for HSV filtering
    Scalar highHSV(highH, highS, highV);     // define Scalar to hold upper limits for HSV filtering
    inRange(hsv, lowHSV, highHSV, thresh);   // filter the image and put the results in thresh Mat
    return (thresh);
}

void showFiltered(Mat image, bool morph) {
    Mat filtered = threshold_image(image);   // perform threshold filtering on the image
    if(morph) {                              // if the morph parameter is true
        Mat morphed = morphed_img(filtered); // morph the image
        imshow(filteredTitle, morphed);      // show the morphed image
    } else {                                 // otherwise
        imshow(filteredTitle, filtered);     // just show the filtered image
    }
}

// Trackbar callbacks
// these are called when the associated trackbars change.
// their parameters aren't actually used but they're needed in order for OpenCV to recognize them as
//  valid callback functions so don't remove them.
void onLowHueChange(int newVal, void *userdata) {
    if(highH < lowH) { // if high slider is less than low slider
                       // set the high slider to 1 above the low slider if possible
        setTrackbarPos(hueHighTitle, filteredTitle, min(lowH + 1, maxHue));
    }
}

void onHighHueChange(int newVal, void *userdata) {
    if(lowH > highH) { // if low slider is higher than low slider
                       // set the low slider to 1 below the high slider if possible
        setTrackbarPos(hueLowTitle, filteredTitle, max(highH - 1, 0));
    }
}

void onLowSatChange(int newVal, void *userdata) {
    if(highS < lowS) { // if low slider is higher than low slider
                       // set the low slider to 1 below the high slider if possible
        setTrackbarPos(satHighTitle, filteredTitle, min(lowS + 1, maxVal));
    }
}

void onHighSatChange(int newVal, void *userdata) {
    if(lowS > highS) { // if low slider is higher than low slider
                       // set the low slider to 1 below the high slider if possible
        setTrackbarPos(satLowTitle, filteredTitle, max(highS - 1, 0));
    }
}

void onLowValChange(int newVal, void *userdata) {
    if(highV < lowV) { // if low slider is higher than low slider
                       // set the low slider to 1 below the high slider if possible
        setTrackbarPos(valHighTitle, filteredTitle, min(lowV + 1, maxVal));
    }
}

void onHighValChange(int newVal, void *userdata) {
    if(lowV > highV) { // if low slider is higher than low slider
                       // set the low slider to 1 below the high slider if possible
        setTrackbarPos(valLowTitle, filteredTitle, max(highV - 1, 0));
    }
}

void setupFilterTrackbars() {
    createTrackbar(hueLowTitle, filteredTitle, &lowH, maxHue, onLowHueChange);    // set up low hue trackbar
    createTrackbar(hueHighTitle, filteredTitle, &highH, maxHue, onHighHueChange); // set up high hue trackbar
    createTrackbar(satLowTitle, filteredTitle, &lowS, maxVal, onLowSatChange);    // set up low sat trackbar
    createTrackbar(satHighTitle, filteredTitle, &highS, maxVal, onHighSatChange); // set up high sat trackbar
    createTrackbar(valLowTitle, filteredTitle, &lowV, maxVal, onLowValChange);    // set up low val trackbar
    createTrackbar(valHighTitle, filteredTitle, &highV, maxVal, onHighValChange); // set up high val trackbar
}

void setupMainTrackbars() {
    createTrackbar("Blur", regularTitle, &blurStrength, maxBlur); // set up blur trackbar
}
