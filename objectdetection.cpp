#include "objectdetection.h"

ObjectDetection::ObjectDetection()
{
    faceCascade.load("C:/opencv4.1.1/opencv-4.x/opencv-4.x/data/haarcascades/haarcascade_frontalface_default.xml");
    carCascade.load("C:/opencv4.1.1/opencv-4.x/opencv-4.x/data/haarcascades/cars.xml");
    pedestrianCascade.load("C:/opencv4.1.1/opencv-4.x/opencv-4.x/data/haarcascades/pedestrian.xml");
    bikeCascade.load("C:/opencv4.1.1/opencv-4.x/opencv-4.x/data/haarcascades/two_wheeler.xml");
    busCascade.load("C:/opencv4.1.1/opencv-4.x/opencv-4.x/data/haarcascades/Bus_front.xml");


}

cv::Mat ObjectDetection::getDetection(cv::Mat img, bool face, bool car, bool pedestrian, bool bus, bool bike){
    cv::Mat mainImage;
    cv::cvtColor(img, mainImage, cv::COLOR_BGRA2BGR);
    cv::cvtColor(img, img, cv::COLOR_BGRA2BGR);
    std::vector<cv::Rect> faces, cars, pedestrians, buses, bikes;

    if(face){
        faceCascade.detectMultiScale(img,faces,1.2,10);
        for(int i = 0; i<faces.size(); i++){
            cv::rectangle(mainImage,faces[i].tl(),faces[i].br(), cv::Scalar(255, 0, 0), 2);
            cv::Point textPosition(faces[i].tl().x, faces[i].tl().y - 10);  // Position above the rectangle
            cv::putText(mainImage, "Face", textPosition, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 0, 0), 2);
        }
    }
    if(car){
        carCascade.detectMultiScale(img,cars,1.05, 4 , 0, cv::Size(10,10));
        for(int i = 0; i<cars.size(); i++){
            cv::rectangle(mainImage,cars[i].tl(),cars[i].br(), cv::Scalar(0, 255, 0) , 2);
            cv::Point textPosition(cars[i].tl().x, cars[i].tl().y - 10);  // Position above the rectangle
            cv::putText(mainImage, "Car", textPosition, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 255, 0) , 2);
        }
    }
    if(pedestrian){
        pedestrianCascade.detectMultiScale(img,pedestrians,1.01, 4);
        for(int i = 0; i<pedestrians.size(); i++){
            cv::rectangle(mainImage,pedestrians[i].tl(),pedestrians[i].br(), cv::Scalar(0, 0, 255) , 2);
            cv::Point textPosition(pedestrians[i].tl().x, pedestrians[i].tl().y - 10);  // Position above the rectangle
            cv::putText(mainImage, "Pedestrian", textPosition, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255) , 2);
        }
    }
    if(bus){
        busCascade.detectMultiScale(img,buses,1.05, 4);
        for(int i = 0; i<buses.size(); i++){
            cv::rectangle(mainImage,buses[i].tl(),buses[i].br(), cv::Scalar(255, 255, 0) , 2);
            cv::Point textPosition(buses[i].tl().x, buses[i].tl().y - 10);  // Position above the rectangle
            cv::putText(mainImage, "Bus", textPosition, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 0) , 2);
        }
    }
    if(bike){
        bikeCascade.detectMultiScale(img,bikes,1.05, 4);
        for(int i = 0; i<bikes.size(); i++){
            cv::rectangle(mainImage,bikes[i].tl(),bikes[i].br(), cv::Scalar(255, 255, 0) , 2);
            cv::Point textPosition(bikes[i].tl().x, bikes[i].tl().y - 10);  // Position above the rectangle
            cv::putText(mainImage, "Bike", textPosition, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 0) , 2);
        }
    }
    return mainImage;

}
