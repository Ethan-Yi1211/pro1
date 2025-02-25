#include "Algorithms.h"


class ImageWindowOperation : public Algorithms {
public:
    imageWindow* IW;

    ImageWindowOperation(imageWindow &IW){
        this->IW = &IW;
        this->qImage = IW.auxImage.copy();
        this->image = QImageToMat(qImage).clone();
    };

    void start(){
        QImage qImage;
        cv::Mat image_modified = image.clone();
        int a =IW->ui->horizontalSlider->value();
        int b =IW->ui->horizontalSlider_2->value();

        int L = 255, i = 0, j = 0;
            if (a > b) {
                int aux = a;
                a = b;
                b = aux;
            }
            else if (a == b) {
                a--;
            }

            Algorithms alg(qImage);
            qImage = alg.MatToQImage(image_modified);
            Algorithms alg2(qImage);
            image_modified = alg.QImageToMat(alg2.BlackAndWhite());
        //cvtColor(image_modified, image_modified, cv::COLOR_RGBA2GRAY);

            for (i = 0; i < image_modified.rows; i++) {
                for (j = 0; j < image_modified.cols * image_modified.channels(); j++) {

                    int r = (int)image_modified.at<uchar>(i, j);
                    int channel = j%image_modified.channels();

                    if(channel != 3){
                    if (a <= r and r <= b) {		// a < r < b
                        image_modified.at<uchar>(i, j) = 0;

                    }
                    else {
                        image_modified.at<uchar>(i, j) = L-1;
                    } }

                }
            }

        qImage = MatToQImage(image_modified);

        IW->ui->horizontalSlider->setVisible(true);
        IW->ui->horizontalSlider_2->setVisible(true);
        IW->ui->horizontalSlider_3->setVisible(false);

        IW->ui->labelSlider1->setVisible(true);
        IW->ui->labelSlider2->setVisible(true);
        IW->ui->labelSlider3->setVisible(false);
        IW->loadImage(qImage);
    }
};
