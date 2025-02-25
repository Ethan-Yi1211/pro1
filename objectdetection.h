#ifndef OBJECTDETECTION_H
#define OBJECTDETECTION_H

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp> //model antrenat pt detectare fata
#include<vector>

#include <QCoreApplication>
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>


class ObjectDetection
{
public:
    ObjectDetection();
    cv::Mat getDetection(cv:: Mat, bool face, bool car, bool pedestrian, bool bus, bool bike);

    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier carCascade;
    cv::CascadeClassifier pedestrianCascade;
    cv::CascadeClassifier busCascade;
    cv::CascadeClassifier bikeCascade;
};

#endif // OBJECTDETECTION_H
