#include "DNN.h"



using namespace std;
using namespace cv;
using namespace dnn;


Mat dnnDetection(Mat image){
    std::vector<std::string> class_names;
    qDebug()<<"test";
    ifstream ifs(string("C:/Users/uif87690/Documents/ImageProcessing/Resources/object_detection_classes_coco.txt").c_str());

    string line;
    while (getline(ifs, line))
    {
        class_names.push_back(line);
    }


    // load the neural network model
    auto model = readNet("C:/Users/uif87690/Documents/ImageProcessing/Resources/frozen_inference_graph.pb",
        "C:/Users/uif87690/Documents/ImageProcessing/Resources/ssd_mobilenet_v2_coco_2018_03_29.pbtxt.txt", "TensorFlow");


    qDebug()<<"test1";
    // read the image from disk
    //create blob from image
    Mat blob = blobFromImage(image, 1.0, Size(300, 300), Scalar(127.5, 127.5, 127.5), true, false);
    //create blob from image
    model.setInput(blob);
    //forward pass through the model to carry out the detection
    Mat output = model.forward();
    Mat detectionMat(output.size[2], output.size[3], CV_32F, output.ptr<float>());

    for (int i = 0; i < detectionMat.rows; i++) {
        int class_id = detectionMat.at<float>(i, 1);
        float confidence = detectionMat.at<float>(i, 2);

        // Check if the detection is of good quality
        if (confidence > 0.4) {
            int box_x = static_cast<int>(detectionMat.at<float>(i, 3) * image.cols);
            int box_y = static_cast<int>(detectionMat.at<float>(i, 4) * image.rows);
            int box_width = static_cast<int>(detectionMat.at<float>(i, 5) * image.cols - box_x);
            int box_height = static_cast<int>(detectionMat.at<float>(i, 6) * image.rows - box_y);
            rectangle(image, Point(box_x, box_y), Point(box_x + box_width, box_y + box_height), Scalar(255, 255, 255), 2);
            putText(image, class_names[class_id - 1].c_str(), Point(box_x, box_y - 5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1);
        }
    }
    return image;
//    imshow("image", image);
//    imwrite("image_result.jpg", image);
//    waitKey(0);
//    destroyAllWindows();

}
