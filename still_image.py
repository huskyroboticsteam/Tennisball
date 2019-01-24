import cv2

def main():
    # Load a model imported from Tensorflow
    model_dir = "./data/final_models/"
    graph_path = model_dir + "frozen_inference_graph.pb"
    pbtxt_path = model_dir + "graph.pbtxt"
    tensorflow_net = cv2.dnn.readNetFromTensorflow(graph_path, pbtxt_path)

    # Input image
    image_path = "/home/evan/Downloads/multiple.jpg"
    img = cv2.imread(image_path)
    rows, cols, channels = img.shape

    # Use the given image as input, which needs to be blob(s).
    tensorflow_net.setInput(cv2.dnn.blobFromImage(img, size=(300, 300), swapRB=True, crop=False))

    # Runs a forward pass to compute the net output
    output = tensorflow_net.forward()

    # Loop on the outputs
    for detection in output[0, 0]:
        score = float(detection[2])
        if score > 0.2:
            left = round(detection[3] * cols)
            top = round(detection[4] * rows)
            right = round(detection[5] * cols)
            bottom = round(detection[6] * rows)
            centerx = (right-left)/2
            centery = (bottom-top)/2
            print("Detected TennisBall with bbox [{}, {}]x[{}, {}]; center: ({}, {}); confidence: {}%".format(left, right, top, bottom, centerx, centery, score*100))
            #draw a red rectangle around detected objects
            cv2.rectangle(img, (int(left), int(top)), (int(right), int(bottom)), (0, 0, 255), thickness=2)

    # Show the image with a rectagle surrounding the detected objects
    cv2.imshow('DO NOT CLOSE THIS WINDOW', img)
    cv2.waitKey()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
