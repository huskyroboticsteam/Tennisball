#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>

using std::string;
namespace util {

  // trackbar titles
  const std::string hueLowTitle = "Hue Low";
  const std::string hueHighTitle = "Hue High";
  const std::string satLowTitle = "Sat Low";
  const std::string satHighTitle = "Sat High";
  const std::string valLowTitle = "Val Low";
  const std::string valHighTitle = "Val High";

  // maximum values for the sliders
  const int maxHue = 180;
  const int maxSat = 255;
  const int maxVal = 255;
  // const int maxBlur = 50;

  // int blurStrength = 0;

  cv::Mat toGrayscale(cv::Mat image) {
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_RGB2GRAY);
    return grayImage;
  }

  cv::Mat toBGR(cv::Mat image) {
    cv::Mat bgrImage;
    cv::cvtColor(image, bgrImage, cv::COLOR_RGB2BGR);
    return bgrImage;
  }

  cv::Mat toHSV(cv::Mat image) {
    cv::Mat hsvImage;
    cvtColor(image, hsvImage, CV_RGB2HSV);
    return hsvImage;
  }

  cv::Mat thresholdImage(cv::Mat image, cv::Scalar lowHSV, cv::Scalar highHSV) {
    cv::Mat thresh(image.rows, image.cols, CV_8UC1);
    cv::Mat hsv = toHSV(image);
    cv::inRange(hsv, lowHSV, highHSV, thresh);
    return thresh;
  }


  int getLowHue(string windowTitle) {
    return cv::getTrackbarPos(hueLowTitle, windowTitle);
  }

  int getLowSat(string windowTitle) {
    return cv::getTrackbarPos(satLowTitle, windowTitle);
  }

  int getLowVal(string windowTitle) {
    return cv::getTrackbarPos(valLowTitle, windowTitle);
  }

  int getHighHue(string windowTitle) {
    return cv::getTrackbarPos(hueHighTitle, windowTitle);
  }

  int getHighSat(string windowTitle) {
    return cv::getTrackbarPos(satHighTitle, windowTitle);
  }

  int getHighVal(string windowTitle) {
    return cv::getTrackbarPos(valHighTitle, windowTitle);
  }

  cv::Scalar getLowHSV(std::string windowTitle) {
    int hLow = util::getLowHue(windowTitle);
    int sLow = util::getLowSat(windowTitle);
    int vLow = util::getLowVal(windowTitle);
    return cv::Scalar(hLow, sLow, vLow);
  }

  cv::Scalar getHighHSV(std::string windowTitle) {
    int hHigh = util::getHighHue(windowTitle);
    int sHigh = util::getHighSat(windowTitle);
    int vHigh = util::getHighVal(windowTitle);
    return cv::Scalar(hHigh, sHigh, vHigh);
  }


  // Trackbar callbacks
  // these are called when the associated trackbars change.
  // their parameters aren't actually used but they're needed in order for OpenCV to recognize them as
  //  valid callback functions so don't remove them.
  void onLowHueChange(int newVal, void *userdata) {
    string *windowTitle = (string *)userdata;
    int highH = util::getHighHue(*windowTitle);
    int lowH = newVal;
    if(highH < lowH) { // if high slider is less than low slider
                       // set the high slider to 1 above the low slider if possible
      cv::setTrackbarPos(hueHighTitle, *windowTitle, std::min(lowH + 1, maxHue));
    }
  }

  void onHighHueChange(int newVal, void *userdata) {
    string *windowTitle = (string *)userdata;
    int highH = newVal;
    int lowH = util::getLowHue(*windowTitle);
    if(lowH > highH) { // if low slider is higher than low slider
                       // set the low slider to 1 below the high slider if possible
      cv::setTrackbarPos(hueLowTitle, *windowTitle, std::max(highH - 1, 0));
    }
  }

  void onLowSatChange(int newVal, void *userdata) {
    string *windowTitle = (string *)userdata;
    int lowS = newVal;
    int highS = util::getHighSat(*windowTitle);
    if(highS < lowS) { // if low slider is higher than low slider
                       // set the low slider to 1 below the high slider if possible
      cv::setTrackbarPos(satHighTitle, *windowTitle, std::min(lowS + 1, maxSat));
    }
  }

  void onHighSatChange(int newVal, void *userdata) {
    string *windowTitle = (string *)userdata;
    int lowS = util::getLowSat(*windowTitle);
    int highS = newVal;
    if(lowS > highS) { // if low slider is higher than low slider
                       // set the low slider to 1 below the high slider if possible
      cv::setTrackbarPos(satLowTitle, *windowTitle, std::max(highS - 1, 0));
    }
  }

  void onLowValChange(int newVal, void *userdata) {
    string *windowTitle = (string *)userdata;
    int lowV = newVal;
    int highV = util::getHighVal(*windowTitle);
    if(highV < lowV) { // if low slider is higher than low slider
                       // set the low slider to 1 below the high slider if possible
      cv::setTrackbarPos(valHighTitle, *windowTitle, std::min(lowV + 1, maxVal));
    }
  }

  void onHighValChange(int newVal, void *userdata) {
    string *windowTitle = (string *)userdata;
    int lowV = util::getLowVal(*windowTitle);
    int highV = newVal;
    if(lowV > highV) { // if low slider is higher than low slider
                       // set the low slider to 1 below the high slider if possible
      cv::setTrackbarPos(valLowTitle, *windowTitle, std::max(highV - 1, 0));
    }
  }

  void setupFilterTrackbars(string windowTitle) {
    cv::createTrackbar(hueLowTitle, windowTitle, NULL, maxHue, onLowHueChange, &windowTitle); // set up low hue trackbar
    cv::createTrackbar(hueHighTitle, windowTitle, NULL, maxHue, onHighHueChange, &windowTitle); // set up high hue trackbar
    cv::createTrackbar(satLowTitle, windowTitle, NULL, maxVal, onLowSatChange, &windowTitle); // set up low sat trackbar
    cv::createTrackbar(satHighTitle, windowTitle, NULL, maxVal, onHighSatChange, &windowTitle); // set up high sat trackbar
    cv::createTrackbar(valLowTitle, windowTitle, NULL, maxVal, onLowValChange, &windowTitle); // set up low val trackbar
    cv::createTrackbar(valHighTitle, windowTitle, NULL, maxVal, onHighValChange, &windowTitle); // set up high val trackbar
  }

  std::vector<cv::Vec3f> findCircles(cv::Mat image) {
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT, 1, image.rows / 16, 200, 100, 0, 0);
    return circles;
  }

  cv::Rect centerSquare(int x, int y, int r) {
    int x1 = x - r;
    int y1 = y - r;
    int h = r * 2;
    int w = r * 2;
    return cv::Rect(x1, y1, w, h);
  }

} // namespace util
