#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;

String filteredTitle = "Live (Filtered)";
String regularTitle = "Live";

String hueLowTitle = "Hue Low";
String hueHighTitle = "Hue High";
String satLowTitle = "Sat Low";
String satHighTitle = "Sat High";
String valLowTitle = "Val Low";
String valHighTitle = "Val High";

int lowH, lowS, lowV;
int highH, highS, highV;
int blurStrength=0;

const int maxHue = 180;
const int maxVal = 255;
const int maxBlur = 50;

void show(Mat image);
void showGray(Mat image);
void showBGR(Mat image);
Mat morphed_img(Mat mask);
Mat threshold_image(Mat img);
void showFiltered(Mat image, bool morph);
void setupTrackbars(String windowName);


int main(int, char**)
{
    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID + apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;
    bool setup = false;
    while(true)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        show(frame);

        //showGray(frame);

        //showBGR(frame);
        
        showFiltered(frame,false);

        if(!setup){
          setupTrackbars(filteredTitle);
          createTrackbar("Blur",regularTitle,&blurStrength,maxBlur);
          setup = true;
        }

        if (waitKey(5) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

void show(Mat image){
  GaussianBlur(image,image,Size(blurStrength*2+1,blurStrength*2+1),0,0);
  imshow(regularTitle,image);
}

void showGray(Mat image){
  Mat gray_image;
  cvtColor(image, gray_image, COLOR_RGB2GRAY);
  imshow("Live (Grayscale)",gray_image);
}

void showBGR(Mat image){
  Mat bgr_image;
  cvtColor(image, bgr_image, COLOR_RGB2BGR);
  imshow("Live (BGR)",bgr_image);
}

Mat morphed_img(Mat mask)
{
  Mat se21 = getStructuringElement(MORPH_RECT, Size(21, 21));
  Mat se11 = getStructuringElement(MORPH_RECT, Size(11, 11));

  morphologyEx(mask, mask, MORPH_CLOSE, se21);
  morphologyEx(mask, mask, MORPH_OPEN, se11);

  GaussianBlur(mask, mask, Size(15, 15), 0, 0);
  return(mask);
}

Mat threshold_image(Mat img)
{
  Mat hsv(img.rows, img.cols, CV_8UC3);
  cvtColor(img, hsv, CV_RGB2HSV);
  Mat thresh(img.rows, img.cols, CV_8UC1);
  Scalar high_HSV(lowH,lowS,lowV);
  Scalar low_HSV(highH,highS,highV);
  inRange(hsv, high_HSV, low_HSV, thresh);
  return(thresh);
}

void showFiltered(Mat image, bool morph){
  Mat mask = threshold_image(image);
  if(morph){
    Mat filtered = morphed_img(mask);
    imshow(filteredTitle,filtered);
  } else{
    imshow(filteredTitle,mask);
  }
}

void onLowHueChange(int newVal, void* userdata){
  if(highH < lowH){
    setTrackbarPos(hueHighTitle,filteredTitle,min(lowH+1,maxHue));
  }
}

void onHighHueChange(int newVal, void* userdata){
  if(lowH > highH){
    setTrackbarPos(hueLowTitle,filteredTitle,max(highH-1,0));
  }
}

void onLowSatChange(int newVal, void* userdata){
  if(highS < lowS){
    setTrackbarPos(satHighTitle,filteredTitle,min(lowS+1,maxVal));
  }
}

void onHighSatChange(int newVal, void* userdata){
  if(lowS > highS){
    setTrackbarPos(satLowTitle,filteredTitle,max(highS-1,0));
  }
}

void onLowValChange(int newVal, void* userdata){
  if(highV < lowV){
    setTrackbarPos(valHighTitle,filteredTitle,min(lowV+1,maxVal));
  }
}

void onHighValChange(int newVal, void* userdata){
  if(lowV > highV){
    setTrackbarPos(valLowTitle, filteredTitle, max(highV-1,0));
  }
}

void setupTrackbars(String windowName){
  createTrackbar(hueLowTitle,windowName,&lowH,maxHue,onLowHueChange);
  createTrackbar(hueHighTitle,windowName,&highH,maxHue,onHighHueChange);
  createTrackbar(satLowTitle,windowName,&lowS,maxVal,onLowSatChange);
  createTrackbar(satHighTitle,windowName,&highS,maxVal,onHighSatChange);
  createTrackbar(valLowTitle,windowName,&lowV,maxVal,onLowValChange);
  createTrackbar(valHighTitle,windowName,&highV,maxVal,onHighValChange);
}

