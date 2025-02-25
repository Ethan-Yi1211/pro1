#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include"mainwindow.h"
#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"

#include <opencv2/objdetect.hpp>
#include<vector>

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QMovie>
#include <QImage>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace cv;

using namespace cv;
using namespace std;

namespace Ui {
class camerawindow;
}

class camerawindow : public QWidget
{
    Q_OBJECT

public:
    explicit camerawindow(QWidget *parent = nullptr);
    ~camerawindow();
    void loadVideo();
    void loadWebcam();

    //QImage MatToQImage(const cv::Mat& mat);
    QImage mainImage;
    bool isCameraOn = false;
    bool isWebCam; //false = e webcam
    bool faceDetect = false;
    bool carDetect = false;
    bool pedestrianDetect = false;
    bool busDetect = false;
    bool bikeDetect=false;
    bool dnnDetectbool=false;
    bool externalVideo = false;
    std::string filePathOfVideo;
    void closeEvent(QCloseEvent *event);

    int contrastValue = 0;
    int brightnessValue = 0;
    int saturationValue = 0;
    int hueValue = -100;
    int sharpnessValue = 0;


private slots:
    void on_startButton_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_contrastButton_clicked();

    void on_brightnessButton_clicked();

    void on_saturationButton_clicked();

    void on_hueButton_clicked();

    void on_sharpnessButton_clicked();

    void on_resetButton_clicked();

    void on_editImgButton_clicked();

    void on_faceCheckBox_stateChanged(int arg1);

    void on_carCheckBox_stateChanged(int arg1);

    void on_pedestrianCheckBox_stateChanged(int arg1);

    void on_busCheckBox_stateChanged(int arg1);

    void on_bikeCheckBox_stateChanged(int arg1);


    void on_checkBox_stateChanged(int arg1);

    void on_loadVideoButton_clicked();

private:
    Ui::camerawindow *ui;
};

#endif // CAMERAWINDOW_H
