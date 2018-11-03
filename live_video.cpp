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
  //printf("%d\n",lowH);
}

void onHighHueChange(int newVal, void* userdata){
  
}

void onLowSatChange(int newVal, void* userdata){
  
}

void onHighSatChange(int newVal, void* userdata){
  
}

void onLowValChange(int newVal, void* userdata){
  
}

void onHighValChange(int newVal, void* userdata){
  
}

void setupTrackbars(String windowName){
  createTrackbar("Hue Low",windowName,&lowH,maxHue,onLowHueChange);
  createTrackbar("Hue High",windowName,&highH,maxHue,onHighHueChange);
  createTrackbar("Sat Low",windowName,&lowS,maxVal,onLowSatChange);
  createTrackbar("Sat High",windowName,&highS,maxVal,onHighSatChange);
  createTrackbar("Val Low",windowName,&lowV,maxVal,onLowValChange);
  createTrackbar("Val High",windowName,&highV,maxVal,onHighValChange);
}

