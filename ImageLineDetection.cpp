#include "Algorithms.h"


class ImageLineDetection : public Algorithms {
public:
    imageWindow* IW;

    ImageLineDetection(imageWindow &IW){
        this->IW = &IW;
        this->qImage = IW.auxImage.copy();
        this->image = QImageToMat(qImage).clone();
    };

    void start(){
        QImage qImage;

        // Convert the image to grayscale
        cv::Mat image_modified = image.clone();

        cv::Mat gray;
        cv::cvtColor(image_modified, gray, cv::COLOR_BGR2GRAY);

        // Apply edge detection

        cv::Mat edges;
        cv::Canny(gray, edges, 50, 150);

        // Apply the Hough transform

        std::vector<cv::Vec2f> lines;
        cv::HoughLines(edges, lines, 1, CV_PI/180, 200);

        // Draw the detected lines on the original image
        for (size_t i = 0; i < lines.size(); i++) {
            float rho = lines[i][0], theta = lines[i][1];
            double a = std::cos(theta), b = std::sin(theta);
            double x0 = a*rho, y0 = b*rho;
            cv::Point pt1(cvRound(x0 + 1000*(-b)), cvRound(y0 + 1000*(a)));
            cv::Point pt2(cvRound(x0 - 1000*(-b)), cvRound(y0 - 1000*(a)));
            cv::line(image_modified, pt1, pt2, cv::Scalar(0,0,255), 2);
        }

        qImage = MatToQImage(edges);

        IW->ui->horizontalSlider->setVisible(false);
        IW->ui->horizontalSlider_2->setVisible(false);
        IW->ui->horizontalSlider_3->setVisible(false);

        IW->ui->labelSlider1->setVisible(false);
        IW->ui->labelSlider2->setVisible(false);
        IW->ui->labelSlider3->setVisible(false);
        IW->loadImage(qImage);
    }
};
