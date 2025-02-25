#include "Algorithms.h"

cv::Mat QImageToMat4(const QImage& image)
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
QImage MatToQImage4(const cv::Mat& mat)
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


class SemanticSegmentation {
public:
    cv::Mat mainImage;
    QImage mainQImage;

    SemanticSegmentation(cv::Mat mainImage){
        this->mainImage = mainImage;
    }
    SemanticSegmentation(QImage mainQ){
        this->mainQImage = mainQ;
        this->mainImage = QImageToMat4(mainQImage);
    }

    QImage GetImage(){
        cv::cvtColor(mainImage, mainImage, cv::COLOR_BGRA2BGR);
        cv::Mat res, element;
        cv::pyrMeanShiftFiltering(mainImage, res, 20, 45, 3);

        return MatToQImage4(res);
    }
};
