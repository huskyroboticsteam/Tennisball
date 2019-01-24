# How to load a Tensorflow model using OpenCV
# Jean Vitor de Paulo Blog - https://jeanvitor.com/tensorflow-object-detecion-opencv/

import cv2

# Load a model imported from Tensorflow
model_dir = "./data/final_models/"
graph_path = model_dir + "frozen_inference_graph.pb"
pbtxt_path = model_dir + "graph.pbtxt"
tensorflowNet = cv2.dnn.readNetFromTensorflow(graph_path, pbtxt_path)

# Input image
image_path = "/home/evan/Downloads/multiple.jpg"
img = cv2.imread(image_path)
rows, cols, channels = img.shape

# Use the given image as input, which needs to be blob(s).
tensorflowNet.setInput(cv2.dnn.blobFromImage(img, size=(300, 300), swapRB=True, crop=False))

# Runs a forward pass to compute the net output
networkOutput = tensorflowNet.forward()

# Loop on the outputs
for detection in networkOutput[0,0]:
    score = float(detection[2])
    if score > 0.2:
        left = detection[3] * cols
        top = detection[4] * rows
        right = detection[5] * cols
        bottom = detection[6] * rows
        #draw a red rectangle around detected objects
        cv2.rectangle(img, (int(left), int(top)), (int(right), int(bottom)), (0, 0, 255), thickness=2)

# Show the image with a rectagle surrounding the detected objects
cv2.imshow('DO NOT CLOSE THIS WINDOW', img)
cv2.waitKey()
cv2.destroyAllWindows()
