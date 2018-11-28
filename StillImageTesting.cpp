#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "utils.hpp"

using namespace cv;
using namespace std;

const string mainTitle = "Display window";
const string filterTitle = "Threshold results";

int main(int argc, char **argv) {
  string imageName = "";
  if(argc > 1) {
    imageName = argv[1];
  }

  Mat image;
  image = imread(imageName, IMREAD_COLOR);

  if(image.empty()) {
    cout << "Could not open or find the image" << std::endl;
    return -1;
  }

  namedWindow(mainTitle, WINDOW_AUTOSIZE);
  namedWindow(filterTitle);
  util::setupFilterTrackbars(filterTitle);

  while(true) {
    imshow(mainTitle, image);
    Mat thresh = util::thresholdImage(image, util::getLowHSV(filterTitle), util::getHighHSV(filterTitle));

    vector<Vec3f> circles = util::findCircles(thresh);
    for(size_t i = 0; i<circles.size(); i++){
      Vec3f c = circles[i];
      cout << "Found circle at (" << c[0] << "," << c[1] << ") with radius " << c[2] << endl;
      rectangle(image, util::centerSquare(c[0],c[1],c[2]), Scalar(255,0,0), 2);
    }

    imshow(filterTitle, thresh);

    if(waitKey(5) >= 0) {
      break;
    }
  }
  return 0;
}
