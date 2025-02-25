#include "Algorithms.h"
#include<vector>

class ImageBinarization : public Algorithms {
public:
    imageWindow* IW;
    int pos;

    ImageBinarization(imageWindow &IW){
        this->IW = &IW;
        this->qImage = IW.auxImage.copy();
        this->image = QImageToMat(qImage).clone();
    };

    void start(){
        //QImage qImage;
        cv::Mat image_modified = image.clone();
        int pos = IW->ui->horizontalSlider->value();

        int L = 255;

        Algorithms alg(qImage);
        qImage = alg.MatToQImage(image_modified);
        Algorithms alg2(qImage);
        image_modified = alg.QImageToMat(alg2.BlackAndWhite());
        //qImage = alg2.ImageBinarization(pos);

            for (int i = 0; i < image_modified.rows; i++) {
                for (int j = 0; j < image_modified.cols * image_modified.channels(); j++) {

                    int channel = j%image_modified.channels();
                    int r = (int)image_modified.at<uchar>(i, j);

                    if(channel != 3){
                        if (r <= pos) {		                //0 < r < a
                            image_modified.at<uchar>(i, j) = 0;
                        }

                        else if (r > pos) {		            // b < r
                            image_modified.at<uchar>(i, j) = L - 1;
                        }
                    }
                }
            }

        qImage = MatToQImage(image_modified);

        IW->ui->horizontalSlider->setVisible(true);
        IW->ui->horizontalSlider_2->setVisible(false);
        IW->ui->horizontalSlider_3->setVisible(false);

        IW->ui->labelSlider1->setVisible(true);
        IW->ui->labelSlider2->setVisible(false);
        IW->ui->labelSlider3->setVisible(false);
        IW->modifiedImage = qImage;
        IW->loadImage(qImage);
    }


};
