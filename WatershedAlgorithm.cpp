#include "Algorithms.h"

cv::Mat QImageToMat3(const QImage& image)
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
QImage MatToQImage3(const cv::Mat& mat)
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


class WatershedAlgorithm {
public:
    cv::Mat mainImage;
    QImage mainQImage;

    WatershedAlgorithm(cv::Mat mainImage){
        this->mainImage = mainImage;
    }
    WatershedAlgorithm(QImage mainQ){
        this->mainQImage = mainQ;
        this->mainImage = QImageToMat3(mainQImage);
    }

    QImage GetImage(){
        cv::Mat gray ;
        cv::cvtColor(mainImage,gray,cv::COLOR_BGR2GRAY);

        cv::Mat thresh;
        double thresh_val = cv::threshold(gray, thresh, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

        //# noise removal
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
        cv::Mat opening;
        cv::morphologyEx(thresh, opening, cv::MORPH_OPEN, kernel, cv::Point(-1, -1), 2);

        // sure background area
        cv::Mat sure_bg;
        cv::dilate(opening, sure_bg, kernel, cv::Point(-1, -1), 3, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());


        // Find sure foreground area
        cv::Mat dist_transform;
        cv::distanceTransform(opening, dist_transform, cv::DIST_L2, 5);

        double max_dist;
        cv::minMaxLoc(dist_transform, nullptr, &max_dist);
        cv::Mat sure_fg;
        cv::threshold(dist_transform, sure_fg, 0.1* max_dist, 255, 0);
        sure_fg.convertTo(sure_fg, CV_8UC1);

        // Find unknown region
        cv::Mat unknown;
        cv::subtract(sure_bg, sure_fg, unknown);

        // Marker labeling
        cv::Mat markers;
        cv::connectedComponents(sure_fg, markers);

        // Add 1 to all labels so that background is not 0 but 1
        markers = markers + 1;

        // Label the unknown region as 0
        markers.setTo(0, unknown);

        // Convert mainImage to 8-bit, 3-channel image
        cv::Mat mainImage_8uc3;
        cv::cvtColor(mainImage, mainImage_8uc3, cv::COLOR_BGR2RGB);

        //Apply watershed algorithm
        cv::watershed(mainImage_8uc3, markers);

        // Mark the boundary regions with red color
        cv::Mat mask;
        cv::compare(markers, -1, mask, cv::CMP_EQ);
        mainImage_8uc3.setTo(cv::Scalar(0, 0, 255), mask);
        cv::cvtColor(mainImage_8uc3, mainImage_8uc3, cv::COLOR_RGB2BGR);


        return (MatToQImage3(mainImage_8uc3));
    }
};
