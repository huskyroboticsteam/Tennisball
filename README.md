# Tennisball
## Code for tennis ball detection (Work in progress)
 This repo contains the tennis ball detection dataset and code. A bit about this repository:
- `src` contains source code files. *Please note that `live_video.cpp` and `still_image.cpp`
   are demo programs for testing the models.* The only files you should really need for
   integrating this code are `detector.hpp` and `detector.cpp`.
- `data` contains the training and testing datasets and the final exported models. Please note
   that all `*_shuffled.csv` and `*.tfrecord` files are automatically generated; you should not
   need to do anything with them.
- `data/training` contains the training dataset, including the CSV with bounding boxes and the
   TFRecord files used during training.
- `data/testing` contains the testing dataset, including the CSV with bounding boxes and the
    TFRecord files used during evaluation.
- `data/final_models` contains the exported neural network in both binary and plain text
format. You will need both `frozen_inference_graph.pb` and `graph.pbtxt` in this directory to
use the detector.
-`images` contains the photos used in the dataset.
-`training` contains training pipeline config files.
-`scripts` contains scripts for setting up the repository, preparing data, training, and
exporting models.

### Setup
#### Installing Object Detection API models
1. Enter project directory
2. **From the project directory**, run `scripts/install_models.sh`. This script will clone the Object Detection API from GitHub, copy it to the project directory, and then patch the model_main.py file with the modified version found in `scripts/`.
#### Installing libraries
OpenCV must be installed for this repository to work. Please build at least OpenCV 3.4.3 from
source.

### Training setup
Please note that you probably shouldn't need to do this step. The models should have already
been trained. However, if it becomes absolutely necessary, follow these steps to run training.

#### Preparing data
1. Enter project directory
2. **From the project directory**, run `scripts/shuffle_data.sh`. This should shuffle the data
   in the CSV and output it to a separate CSV file in the data directories.
3. **From the project directory**, run `scripts/make_tfrecords.sh`. This should save the data
   from the images and CSV files into .tfrecord files used for training. 

#### Running training job
1. Enter project directory
2. **From the project directory**, run `scripts/install_models.sh`. This should clone the
   latest Object Detection API, and also install a starter model based on the COCO Dataset from
   the Tensorflow Object Detection Zoo.
3. **From the project directory**, run `scripts/run_training.sh`. This script will call the
   Object Detection API's model_main.py. Please note that this will take a VERY long time to
   complete, and should probably be run on a computer with an NVidia GPU and CUDA installed to
   speed up the process.

### Compiling demo programs
1. Enter the project directory
2. Run `cmake .`
3. Run `make`
4. Executables should have been created. Please note that these are just demo programs for
   testing the models. 
   
## Detector API
Include `detector.hpp` in your program to use the detector. Please note that all classes and
methods are in the `tb` namespace.

### The Detector Class
Construct one of these to use the detector. You can construct a `Detector` like so:
```c++
tb::Detector(std::string binaryGraphPath, std::string graphPbtxtPath)
```
where `binaryGraphPath` is the path to the aforementioned `frozen_inference_graph.pb`, and
`graphPbtxtPath` is the path to the aforementioned `graph.pbtxt`, both represented as strings.

Once you have a `Detector`, you may call one of these methods on it to run the detection:
```c++
std::vector<tb::Detection> performDetection(cv::Mat image);
std::vector<tb::Detection> performDetection(cv::Mat image, float confidenceThreshold);
```
where `image` is an OpenCV `Mat` representing the image you would like to run the detection on,
and `confidenceThreshold`, if supplied, is a value between 0 and 1 representing the minimum
confidence value of detections to be returned. If this is not supplied, it defaults to `0.2`.
These functions return a `vector` of `Detection` objects, which brings us to...

### The Detection Class
These are returned by the Detector to represent an object that was detected. You should likely
never need to construct one of these objects, as they are constructed by the Detector, but if
you wish to do so, they may be constructed like so:
```c++
tb::Detection(int left, int right, int top, int bottom, float confidence)
```
where left, right, top, and bottom are integers representing the left, right, top, and bottom
coordinates of the bounding box, respectively; and where confidence is a float between 0 and 1
representing the confidence value of this detection.

`Detection` objects have the following methods:
```c++
float getConfidence();
float getConfidencePct();
```
The former returns the confidence value for this Detection, the latter returns this value as a
percentage (i.e. multiplied by `100`).

```c++
int getBBoxLeft();
int getBBoxRight();
int getBBoxTop();
int getBBoxBottom();
int getBBoxWidth();
int getBBoxHeight();
```
These methods return the bounding box left, right, top, bottom, width, and height,
respectively.

```c++
cv::Rect2i getBBoxRect();
```
Gets the bounding box of this `Detection` as an OpenCV Rect.

```c++
cv::Point2f getBBoxCenter();
```
Gets the center of the bounding box as an OpenCV Point.

## Contact
Please contact us on Slack in the `#tennisballdetection` channel if you have any questions.
