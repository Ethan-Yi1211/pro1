#include "Resources.h"
#include <opencv2/objdetect.hpp>
#include<vector>

cv::Mat QImageToMatCar(const QImage& image)
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
QImage MatToQImageCar(const cv::Mat& mat)
{
    QImage image;
    if (mat.channels() == 1)
    {
        image = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        image = image.convertToFormat(QImage::Format_ARGB32); // convert to ARGB32
    }
    else if (mat.channels() == 3)
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        image = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
        image = image.convertToFormat(QImage::Format_ARGB32); // convert to ARGB32
    }
    else if (mat.channels() == 4)
    {
        image = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
    }
    return image.copy();
}


class CarDetection {
public:
    cv::Mat mainImage;
    QImage mainQImage;

    CarDetection(cv::Mat mainImage){
        this->mainImage = mainImage;
    }
    CarDetection(QImage mainQ){
        this->mainQImage = mainQ;
        this->mainImage = QImageToMatCar(mainQImage);
    }

    QImage GetImage(){
        cv::cvtColor(mainImage, mainImage, cv::COLOR_BGRA2BGR);
        cv::CascadeClassifier faceCascade;
        faceCascade.load("C:/opencv4.1.1/opencv-4.x/opencv-4.x/data/haarcascades/cars.xml");

        if(faceCascade.empty()){
           qDebug() << "XMl file not loaded";
        }

        std::vector<cv::Rect> faces;
        faceCascade.detectMultiScale(mainImage,faces,1.05, 4 , 0, cv::Size(10,10));

        for(int i = 0; i<faces.size(); i++){
            cv::rectangle(mainImage,faces[i].tl(),faces[i].br(), cv::Scalar(255 - i * 25,i*25,i*25), 2);
            cv::Point textPosition(faces[i].tl().x, faces[i].tl().y - 10);  // Position above the rectangle
            cv::putText(mainImage, "Car", textPosition, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255 - i * 25, i * 25, i * 25), 2);
            qDebug() << i;
        }

        return MatToQImageCar(mainImage);
    }
};
