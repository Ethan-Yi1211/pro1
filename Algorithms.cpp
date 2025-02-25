#include "Algorithms.h"

#include <iostream>
#include <QCoreApplication>
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>

using namespace std;
using namespace cv;

Mat Algorithms::QImageToMat(const QImage& image)
{
    cv::Mat mat;
    switch (image.format())
    {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
            mat = cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
            //cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
            break;
        case QImage::Format_RGB888:
            mat = cv::Mat(image.height(), image.width(), CV_8UC3, const_cast<uchar*>(image.bits()), image.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
            break;
        default:
            mat = cv::Mat();
            break;
    }
    return mat.clone();
}

QImage Algorithms::MatToQImage(const cv::Mat& mat)
{
    QImage image;
    if (mat.channels() == 1)
    {
        image = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    }
    else if (mat.channels() == 3)
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        image = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    }
    else if (mat.channels() == 4)
    {
        image = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);

    }
    return image.copy();
}




Algorithms::Algorithms(QImage image){
    this->qImage = image.copy();
    this->image = QImageToMat(image).clone();
}

Algorithms::Algorithms(imageWindow& IW)
{
    this->IW = &IW;
}

Algorithms::Algorithms(){

}

QImage Algorithms::BlackAndWhite(){
    QImage qImage;
    Mat image_modified = image.clone();


    for (int i = 0; i < image_modified.rows; i++) {
        for (int j = 0; j < image_modified.cols * image_modified.channels(); j += image_modified.channels()) {
            // calculate average of RGB values
            int average = (image_modified.at<uchar>(i, j) +
                           image_modified.at<uchar>(i, j + 1) +
                           image_modified.at<uchar>(i, j + 2)) / 3;

            // set all channels to the average value
            image_modified.at<uchar>(i, j) = average;
            image_modified.at<uchar>(i, j + 1) = average;
            image_modified.at<uchar>(i, j + 2) = average;
        }
    }

    qImage = MatToQImage(image_modified);
    return qImage;
}

QImage Algorithms::ImageInversion(){
    QImage qImage;
    Mat image_modified = image.clone();

    int L = 255;

    for (int i = 0; i < image_modified.rows; i++) {
        for (int j = 0; j < image_modified.cols * image_modified.channels(); j++) {

            int r = (int)image_modified.at<uchar>(i, j);

            image_modified.at<uchar>(i, j) = L-1-r;
        }
    }


    qImage = MatToQImage(image_modified);
    return qImage;
}

QImage Algorithms::ImageBinarization(int pos){
    QImage qImage;
    Mat image_modified = image.clone();

    int L = 255;

    Algorithms alg(qImage);
    qImage = alg.MatToQImage(image_modified);
    Algorithms alg2(qImage);
    image_modified = alg.QImageToMat(alg2.BlackAndWhite());
    //cvtColor(image_modified, image_modified, cv::COLOR_RGBA2GRAY);

        for (int i = 0; i < image_modified.rows; i++) {
            for (int j = 0; j < image_modified.cols * image_modified.channels(); j++) {

                int channel = j%image_modified.channels();
                int r = (int)image_modified.at<uchar>(i, j);

                if(channel != 3){
                    if (r <= pos) {		                //0 < r < a
                        image_modified.at<uchar>(i, j) = 0;
                    }

                    else if (r > pos) {		            // b < r
                        image_modified.at<uchar>(i, j) = L - 1;
                    }
                }
            }
        }

    qImage = MatToQImage(image_modified);
    return qImage;
}

QImage Algorithms::ImageChangeContrast(int a){
    QImage qImage;
    Mat image_modified = image.clone();

    /*
    int L = 255, i = 0, j = 0, Sa=30, Sb=200;
        if (a > b) {
            int aux = a;
            a = b;
            b = aux;
        }
        else if (a == b) {
            a--;
        }
        int m = Sa / a;
        int n = (Sb - Sa) / (b - a);
        int p = (L - 1 - Sb) / (L - 1 - b);



        for (i = 0; i < image_modified.rows; i++) {
            for (j = 0; j < image_modified.cols * image_modified.channels(); j++) {

                int channel = j%image_modified.channels();
                int r = (int)image_modified.at<uchar>(i, j);
                if(channel != 3){
                    if (r <= a) {		                //0 < r < a
                        image_modified.at<uchar>(i, j) = m * r;
                    }

                    else if (a <= r and r <= b) {		// a < r < b
                        image_modified.at<uchar>(i, j) = n * (r - a) + Sa;
                    }

                    else if (r >= b) {		            // b < r
                        image_modified.at<uchar>(i, j) = p * (r - b) + Sb;
                    }
                }
            }
        }
    */

    a-=37;
    float contrast = 2.0 *(a/255.0) + 0.5;
    //cvtColor(image_modified, image_modified, COLOR_BGR2RGB);

    image_modified = image_modified*contrast;
    qImage = MatToQImage(image_modified);

    return qImage;
}

QImage Algorithms::ImageNoiseReduction(int a){
    QImage qImage2;
    Mat image_modified = image.clone();
    /*
    if (a > b) {
        int aux = a;
        a = b;
        b = aux;
    }
    else if (a == b) {
        a--;
    }

    int L = 255, i = 0, j = 0;
    int n = (L - 1) / (b - a);



        for (i = 0; i < image_modified.rows; i++) {
            for (j = 0; j < image_modified.cols * image_modified.channels(); j++) {

                int r = (int)image_modified.at<uchar>(i, j);
                int channel = j%image_modified.channels();

                if(channel != 3){
                    if (r <= a) {		                //0 < r < a
                        image_modified.at<uchar>(i, j) = 0;
                    }

                    else if (a <= r and r <= b) {		// a < r < b
                        image_modified.at<uchar>(i, j) = n * (r - a);
                    }

                    else if (r >= b) {		            // b < r
                        image_modified.at<uchar>(i, j) = L - 1;
                    }
                }

            }
        }



    */
    int max = a/9;
    if(max%2==0)
        max++;

    //GaussianBlur(image_modified, image_modified, Size(max,max),0,0);
    medianBlur(image_modified, image_modified, max);

    qImage2 = MatToQImage(image_modified);

    return qImage2;
}

QImage Algorithms::ImageWindowOperation(int a, int b){

    QImage qImage;
    Mat image_modified = image.clone();

    int L = 255, i = 0, j = 0;
        if (a > b) {
            int aux = a;
            a = b;
            b = aux;
        }
        else if (a == b) {
            a--;
        }

        Algorithms alg(qImage);
        qImage = alg.MatToQImage(image_modified);
        Algorithms alg2(qImage);
        image_modified = alg.QImageToMat(alg2.BlackAndWhite());
    //cvtColor(image_modified, image_modified, cv::COLOR_RGBA2GRAY);

        for (i = 0; i < image_modified.rows; i++) {
            for (j = 0; j < image_modified.cols * image_modified.channels(); j++) {

                int r = (int)image_modified.at<uchar>(i, j);
                int channel = j%image_modified.channels();

                if(channel != 3){
                if (a <= r and r <= b) {		// a < r < b
                    image_modified.at<uchar>(i, j) = 0;

                }
                else {
                    image_modified.at<uchar>(i, j) = L-1;
                } }

            }
        }

    qImage = MatToQImage(image_modified);
    return qImage;
}

QImage Algorithms::ImageHistogram(){

    QImage qImage;
    Mat image_modified = image.clone();
    int L = 255, i , j;
    float  Frecventa[261] = { 0 }, size,procent, procent2 = 0;

    size = image_modified.rows * image_modified.cols;
    Mat hist;

    hist = Mat::zeros(image_modified.size(), image_modified.type());
    hist.rows = 270;
    hist.cols = 270;


        for ( i = 0; i < image_modified.rows; i++) {
            for ( j = 0; j < image_modified.cols ; j++) {

                int r = (int)image_modified.at<uchar>(i, j);

                if (1) {
                    float g = (int)image_modified.at<uchar>(i, j+1);
                    float b = (int)image_modified.at<uchar>(i, j+2);
                    float i = (r + g + b) / 3;
                    Frecventa[r]++;
                    j += 2;
                }
                else {
                    Frecventa[r]++;
                }

            }
        }

        for (int i = 0; i < 255; i++) {
            procent = Frecventa[i] / size * 100;
            //cout << procent<<"%"<< " ";
            procent2 = (procent/100) * 255;

            for (j = 0; j < procent2; j++) {
                hist.at<uchar>(255-(j), i+10) = L - 1;
            }
        }

   qImage = MatToQImage(hist);
   return qImage;
}

QImage Algorithms::ImageChangeBrightness(int a){
    QImage qImage;
    Mat image_modified = image.clone() + Scalar(a-125,a-125,a-125);


    qImage = MatToQImage(image_modified);
    return qImage;
}

QImage Algorithms::ImageColorCorrection(int r, int g, int b){
    QImage qImage;
    Mat image_modified = image.clone();

    vector<Mat> channels;
    split(image_modified, channels);
    r-=125;     g-=125;     b-=125;


    channels[0] += b;  // Blue channel
    channels[1] += g;  // Green channel
    channels[2] += r;  // Red channel


    merge(channels, image_modified);

    qImage = MatToQImage(image_modified);
    return qImage;
}
