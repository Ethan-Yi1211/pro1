#ifndef DNN_H
#define DNN_H

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/all_layers.hpp>

#include <iostream>
#include <fstream>

#include <QDebug>

cv::Mat dnnDetection(cv::Mat input);

#endif
