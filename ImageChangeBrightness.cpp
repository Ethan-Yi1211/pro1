#include "Algorithms.h"

class ImageChangeBrightness : public Algorithms {
public:
    imageWindow* IW;

    ImageChangeBrightness(imageWindow &IW){
        this->IW = &IW;
        this->qImage = IW.auxImage.copy();
        this->image = QImageToMat(qImage).clone();
    };

    void start(){
        QImage qImage;
        int a = IW->ui->horizontalSlider->value();
        Algorithms alg(this->qImage);



        //qImage = MatToQImage(image_modified);

        IW->ui->horizontalSlider->setVisible(true);
        IW->ui->horizontalSlider_2->setVisible(false);
        IW->ui->horizontalSlider_3->setVisible(false);

        IW->ui->labelSlider1->setVisible(true);
        IW->ui->labelSlider2->setVisible(false);
        IW->ui->labelSlider3->setVisible(false);
        IW->loadImage(alg.ImageChangeBrightness(a));
    }
};
