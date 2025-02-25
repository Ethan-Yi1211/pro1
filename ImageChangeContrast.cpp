#include "Algorithms.h"


class ImageChangeContrast : public Algorithms {
public:
    imageWindow* IW;


    ImageChangeContrast(imageWindow &IW){
        this->IW = &IW;
        this->qImage = IW.auxImage.copy();
        this->image = QImageToMat(qImage).clone();
    };

    void start(){
        QImage qImage;
        cv::Mat image_modified = image.clone();
        int a = IW->ui->horizontalSlider->value();
        a-=37;
        float contrast = 2.0 *(a/255.0) + 0.5;
        //cvtColor(image_modified, image_modified, COLOR_BGR2RGB);

        image_modified = image_modified*contrast;

        qImage = MatToQImage(image_modified);

        IW->ui->horizontalSlider->setVisible(true);
        IW->ui->horizontalSlider_2->setVisible(false);
        IW->ui->horizontalSlider_3->setVisible(false);

        IW->ui->labelSlider1->setVisible(true);
        IW->ui->labelSlider2->setVisible(false);
        IW->ui->labelSlider3->setVisible(false);
        IW->loadImage(qImage);
    }
    QImage getImage(int a){
        QImage qImage;
        cv::Mat image_modified = image.clone();

        a-=37;
        float contrast = 2.0 *(a/255.0) + 0.5;
        //cvtColor(image_modified, image_modified, COLOR_BGR2RGB);

        image_modified = image_modified*contrast;
        qImage = MatToQImage(image_modified);

        return qImage;
    }
};
