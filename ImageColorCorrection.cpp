#include "Algorithms.h"


class ImageColorCorrection : public Algorithms {
public:
    imageWindow* IW;

    ImageColorCorrection(imageWindow &IW){
        this->IW = &IW;
        this->qImage = IW.auxImage.copy();
        this->image = QImageToMat(qImage).clone();
    };

    void start(){
        QImage qImage;
        cv::Mat image_modified = image.clone();
        int r = IW->ui->horizontalSlider->value();
        int g = IW->ui->horizontalSlider_2->value();
        int b = IW->ui->horizontalSlider_3->value();


        std::vector<cv::Mat> channels;
        split(image_modified, channels);
        r-=125;     g-=125;     b-=125;


        channels[0] += b;  // Blue channel
        channels[1] += g;  // Green channel
        channels[2] += r;  // Red channel


        merge(channels, image_modified);

        qImage = MatToQImage(image_modified);

        IW->ui->horizontalSlider->setVisible(true);
        IW->ui->horizontalSlider_2->setVisible(true);
        IW->ui->horizontalSlider_3->setVisible(true);

        IW->ui->labelSlider1->setVisible(true);
        IW->ui->labelSlider2->setVisible(true);
        IW->ui->labelSlider3->setVisible(true);
        IW->loadImage(qImage);
    }
};
