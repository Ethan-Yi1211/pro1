#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp> //model antrenat pt detectare fata
#include<vector>

#include "imagewindow.h"
#include"ui_imagewindow.h"

#include <QCoreApplication>
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>



class Algorithms {
public:
    imageWindow* IW;

    cv::Mat image;
    QImage qImage;
    QImage modifiedQImage;

    Algorithms(QImage image);
    Algorithms(imageWindow& IW);
    Algorithms();

    cv::Mat QImageToMat(const QImage& image);
    QImage MatToQImage(const cv::Mat& mat);

    QImage BlackAndWhite();
    QImage ImageBinarization(int pos);
    QImage ImageInversion();

    //QImage ImageChangeContrast(int a, int b);
    QImage ImageChangeContrast(int a);

    QImage ImageChangeBrightness(int a);

    QImage ImageNoiseReduction(int a);

    QImage ImageWindowOperation(int a, int b);

    QImage ImageColorCorrection(int r, int g, int b);

    QImage ImageHistogram();




private:


};

#endif // ALGORITHMS_H
