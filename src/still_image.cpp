#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {
  string graph_path = "./data/final_models/frozen_inference_graph.pb";
  string graph_pbtxt = "./data/final_models/graph.pbtxt";
  cv::dnn::Net network =
      cv::dnn::readNetFromTensorflow(graph_path, graph_pbtxt);

  string classNames[] = {"TennisBall"};

  string image_path = "./images/1.jpg";
  cv::Mat frame = cv::imread(image_path);

  network.setInput(cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300),
                                          cv::Scalar(), true, false));
  cv::Mat output = network.forward();

  cout << output.size << endl;
  cv::Mat detectionMat(output.size[2], output.size[3], CV_32F,
                       output.ptr<float>());

  for (int i = 0; i < detectionMat.rows; i++) {
    float confidence = detectionMat.at<float>(i, 2);
    cout << "confidence: " << confidence << endl;

    if (confidence > 0.2) {
      size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));

      int xLeftBottom =
          static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
      int yLeftBottom =
          static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
      int xRightTop =
          static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
      int yRightTop =
          static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

      cout << "[" << xLeftBottom << ", " << xRightTop << "] x [" << yLeftBottom << ", " << yRightTop << "]" << endl;

      ostringstream ss;
      ss << confidence*100 << "%";
      string conf(ss.str());

      cv::Rect object((int)xLeftBottom, (int)yLeftBottom,
                      (int)(xRightTop - xLeftBottom),
                      (int)(yRightTop - yLeftBottom));

      rectangle(frame, object, Scalar(0, 255, 0), 2);
      String label = classNames[objectClass] + ": " + conf;
      int baseLine = 0;
      Size labelSize =
          getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
      rectangle(frame,
                Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),
                     Size(labelSize.width, labelSize.height + baseLine)),
                Scalar(0, 255, 0), CV_FILLED);
      putText(frame, label, Point(xLeftBottom, yLeftBottom),
              FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
    }
  }
  imshow("image", frame);
  waitKey(0);
}
