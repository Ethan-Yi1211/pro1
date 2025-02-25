#include "Algorithms.h"


class ImageInversion : public Algorithms {
public:
    imageWindow* IW;

    ImageInversion(imageWindow &IW){
        this->IW = &IW;
        this->qImage = IW.auxImage.copy();
        this->image = QImageToMat(qImage).clone();
    };

    void start(){
        QImage qImage;
        cv::Mat image_modified = image.clone();

        int L = 255;

        for (int i = 0; i < image_modified.rows; i++) {
            for (int j = 0; j < image_modified.cols * image_modified.channels(); j++) {

                int r = (int)image_modified.at<uchar>(i, j);

                image_modified.at<uchar>(i, j) = L-1-r;
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
