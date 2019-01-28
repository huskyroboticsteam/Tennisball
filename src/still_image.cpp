#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {
    string graphPath = "./data/final_models/frozen_inference_graph.pb";
    string graphPbtxt = "./data/final_models/graph.pbtxt";
    cv::dnn::Net network = cv::dnn::readNetFromTensorflow(graphPath, graphPbtxt);

    string classNames[] = { "", "TennisBall" };

    string imagePath = "./images/1.jpg";
    Mat frame = imread(imagePath);

    network.setInput(cv::dnn::blobFromImage(frame, 1.0, Size(300, 300), Scalar(), true, false));
    Mat output = network.forward();

    cout << output.size << endl;
    Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

    for(int i = 0; i < detectionMat.rows; i++) {
        float confidence = detectionMat.at<float>(i, 2);
        cout << "confidence: " << confidence << endl;

        if(confidence > 0.2) {
            int objectClass = static_cast<int>(detectionMat.at<float>(i, 1));
            cout << "object class: " << objectClass << endl;

            int left = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
            int bottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
            int right = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
            int top = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

            cout << "[" << left << ", " << right << "] x [" << bottom << ", " << top << "]" << endl;

            ostringstream ss;
            ss << confidence * 100 << "%";
            string conf(ss.str());

            Rect object((int)left, (int)bottom, (int)(right - left), (int)(top - bottom));

            rectangle(frame, object, Scalar(0, 255, 0), 2);
            String label = classNames[objectClass] + ": " + conf;
            int baseLine = 0;
            Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
            rectangle(frame,
                      Rect(Point(left, bottom - labelSize.height),
                           Size(labelSize.width, labelSize.height + baseLine)),
                      Scalar(0, 255, 0), CV_FILLED);
            putText(frame, label, Point(left, bottom), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
        }
    }
    imshow("image", frame);
    waitKey(0);
}
