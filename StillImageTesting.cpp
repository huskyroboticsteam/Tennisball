#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    String imageName = "";
    if(argc > 1){
        imageName = argv[1];
    }

    Mat image;
    image = imread(imageName, IMREAD_COLOR);

    if( image.empty() ) {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE ); 

    imshow( "Display window", image );

    waitKey(0);
    return 0;
}