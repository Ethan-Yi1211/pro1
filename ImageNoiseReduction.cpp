#include "Algorithms.h"


class ImageNoiseReduction : public Algorithms {
public:
    imageWindow* IW;

    ImageNoiseReduction(imageWindow &IW){
        this->IW = &IW;
        this->qImage = IW.auxImage.copy();
        this->image = QImageToMat(qImage).clone();
    };

    void start(){
        QImage qImage;
        cv::Mat image_modified = image.clone();
        int a =IW->ui->horizontalSlider->value();
        int max = a/9;
        if(max%2==0)
            max++;

        //GaussianBlur(image_modified, image_modified, Size(max,max),0,0);
        medianBlur(image_modified, image_modified, max);

        qImage = MatToQImage(image_modified);

        IW->ui->horizontalSlider->setVisible(true);
        IW->ui->horizontalSlider_2->setVisible(false);
        IW->ui->horizontalSlider_3->setVisible(false);

        IW->ui->labelSlider1->setVisible(true);
        IW->ui->labelSlider2->setVisible(false);
        IW->ui->labelSlider3->setVisible(false);
        IW->loadImage(qImage);
    }
};
