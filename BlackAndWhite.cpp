#include "Algorithms.h"


class BlackAndWhite : public Algorithms {
public:
    imageWindow* IW;

    BlackAndWhite(imageWindow &IW){
        this->IW = &IW;
        this->qImage = IW.modifiedImage.copy();
        this->image = QImageToMat(IW.modifiedImage).clone();
    };

    void start(){
        QImage qImage;
        cv::Mat image_modified = image.clone();


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

        IW->ui->horizontalSlider->setVisible(false);
        IW->ui->horizontalSlider_2->setVisible(false);
        IW->ui->horizontalSlider_3->setVisible(false);

        IW->ui->labelSlider1->setVisible(false);
        IW->ui->labelSlider2->setVisible(false);
        IW->ui->labelSlider3->setVisible(false);
        IW->loadImage(qImage);
    }
};
