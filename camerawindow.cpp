#include "camerawindow.h"
#include "ui_camerawindow.h"


#include "imagewindow.h"
#include "ui_imagewindow.h"

#include"Algorithms/objectdetection.h"
#include"Algorithms/DNN.h"

#include <QDebug>




QImage MatToQImage(const cv::Mat& mat);
QImage FaceDetection(cv::Mat mat, cv::Point* tl, cv::Point* br, bool&);
QImage CarDetection(cv::Mat mat, cv::Point* tl, cv::Point* br, bool&);

camerawindow::camerawindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camerawindow)
{
    ui->setupUi(this);
    ui->horizontalSlider->setVisible(false);
    ui->label->setVisible(false);

}

camerawindow::~camerawindow()
{
    delete ui;
}

QImage MatToQImage(const cv::Mat& mat)
{
    QImage image;
    if (mat.channels() == 1)
    {
        image = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        image = image.convertToFormat(QImage::Format_ARGB32); // convert to ARGB32
    }
    else if (mat.channels() == 3)
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        image = QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
        image = image.convertToFormat(QImage::Format_ARGB32); // convert to ARGB32
    }
    else if (mat.channels() == 4)
    {
        image = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
    }
    return image.copy();
}

void camerawindow::loadVideo(){

    qDebug() << "Opening camera...";
    isCameraOn = true;
    // Initialize the camera system
    SystemPtr system = System::GetInstance();

    // Retrieve list of cameras from the system
    CameraList camList = system->GetCameras();

    // Check if any cameras are connected
    if (camList.GetSize() == 0) {
        QMessageBox::critical(nullptr, "Error", "No cameras found.");
        return;
    }

    // Open the first camera
    CameraPtr cam = camList.GetByIndex(0);
    cam->Init();



    // Start acquisition
    cam->BeginAcquisition();
    qDebug() << "Camera open.";
    ui->loadingLabel->setVisible(false);

    int frameCounter = 0;
    QElapsedTimer timer;
    timer.start();
    cv::Point tlFace[15], brFace[15], tlCar[15], brCar[15]; //store the coords of the rectangle
    ObjectDetection objectDetection;

    ImageProcessor processor;
    processor.SetColorProcessing(SPINNAKER_COLOR_PROCESSING_ALGORITHM_HQ_LINEAR);

    // Main loop to retrieve and display the frames
    while (isCameraOn) {

        // Retrieve a frame from the camera
        ImagePtr img = cam->GetNextImage();
        ImagePtr convertedImage = processor.Convert(img, PixelFormat_BGR8);

        // Convert img to OpenCV format
        Mat cvimg(convertedImage->GetHeight(), convertedImage->GetWidth(), CV_8UC3, convertedImage->GetData(), convertedImage->GetStride());

        //Aplicam modificarile
        if (contrastValue != 0 || brightnessValue != 0 || saturationValue != 0 || hueValue != -100 || sharpnessValue != 0) {
            Mat adjustedImg;
            cvimg.convertTo(adjustedImg, -1, 1.0 + contrastValue/100.0, brightnessValue);
            cv::cvtColor(adjustedImg, adjustedImg, cv::COLOR_BGR2HSV);
            cv::Mat hsv_channels[3];
            cv::split(adjustedImg, hsv_channels);
            hsv_channels[0] += (hueValue + 100) * 1.5; //from[-100,100] to [0,360]
            hsv_channels[1] += saturationValue;
            hsv_channels[2] += sharpnessValue;
            cv::merge(hsv_channels, 3, adjustedImg);
            cv::cvtColor(adjustedImg, adjustedImg, cv::COLOR_HSV2BGR);
            cvimg = adjustedImg;
        }

        QImage imgQ; //The image displayed

        frameCounter++;
        if(timer.elapsed() >= 1000){
            std::string myStr = "FPS: " + std::to_string(frameCounter);
            QString valueInString = QString::fromStdString(myStr);
            ui->fpsLabel->setText(valueInString);

            timer.restart();
            frameCounter = 0;
        }

        //qDebug() << "Initial Resolution: " << QString::number(cvimg.size().width) + "x" + QString::number(cvimg.size().height);
        //1280 1025

        cv::Mat resizedImage;
        if(faceDetect || carDetect || pedestrianDetect || busDetect || bikeDetect){
            cv::resize(cvimg, resizedImage, cv::Size(cvimg.cols / 3, cvimg.rows / 3), cv::INTER_LINEAR); //"426 341"
            imgQ = MatToQImage(objectDetection.getDetection(resizedImage,faceDetect,carDetect,pedestrianDetect,busDetect,bikeDetect));
        }
        else
            imgQ = MatToQImage(cvimg);

        QImage scaledImage = imgQ.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        mainImage = scaledImage;
        ui->imageLabel->setAlignment(Qt::AlignCenter);
        ui->imageLabel->setPixmap(QPixmap::fromImage(scaledImage));

        mainImage = imgQ.copy();
        // Release the frame from the camera
        img->Release();

        QCoreApplication::processEvents();

    }
    qDebug() << "Closing camera...";
    // Stop acquisition and release the camera
    cam->EndAcquisition();
    cam->DeInit();
    cam = nullptr;
    camList.Clear();
    system->ReleaseInstance();
    qDebug() << "Camera closed.";
}

void camerawindow::loadWebcam()
{
    if(externalVideo){

        qDebug() << "Opening webcam...";
        isCameraOn = true;


        VideoCapture cap(filePathOfVideo);

        if (!cap.isOpened()){
            QMessageBox::warning(this, "Camera Error", "Unable to open webcam.");
            return;
        }
        qDebug() << "Video ok.";
        ui->loadingLabel->setVisible(false);

        int frameCounter = 0;
        QElapsedTimer timer;
        timer.start();
        cv::Point tlFace[15], brFace[15], tlCar[15], brCar[15]; //store the coords of the rectangle
        ObjectDetection objectDetection;

        while(isCameraOn){

            Mat frame;
            QImage img;

            cap.read(frame);

            //Aplicam modificarile
            if (contrastValue != 0 || brightnessValue != 0 || saturationValue != 0 || hueValue != -100 || sharpnessValue != 0) {
                Mat adjustedImg;
                frame.convertTo(adjustedImg, -1, 1.0 + contrastValue/100.0, brightnessValue);
                cv::cvtColor(adjustedImg, adjustedImg, cv::COLOR_BGR2HSV);
                cv::Mat hsv_channels[3];
                cv::split(adjustedImg, hsv_channels);
                hsv_channels[0] += (hueValue + 100) * 1.8; //from[-100,100] to [0,360]
                hsv_channels[1] += saturationValue;
                hsv_channels[2] += sharpnessValue;
                cv::merge(hsv_channels, 3, adjustedImg);
                cv::cvtColor(adjustedImg, adjustedImg, cv::COLOR_HSV2BGR);
                frame = adjustedImg;
            }

            frameCounter++;
            if(timer.elapsed() >= 1000){
                std::string myStr = "FPS: " + std::to_string(frameCounter);
                QString valueInString = QString::fromStdString(myStr);
                ui->fpsLabel->setText(valueInString);

                timer.restart();
                frameCounter = 0;
            }
            if(dnnDetectbool){
                img = MatToQImage(dnnDetection(frame));
            }


            if(faceDetect || carDetect || pedestrianDetect || busDetect || bikeDetect)
                img = MatToQImage(objectDetection.getDetection(frame,faceDetect,carDetect,pedestrianDetect,busDetect,bikeDetect));
            else
                img = MatToQImage(frame);
            // Convert the adjusted frame to a QImage

            QImage scaledImage = img.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            mainImage = scaledImage;
            ui->imageLabel->setAlignment(Qt::AlignCenter);
            ui->imageLabel->setPixmap(QPixmap::fromImage(scaledImage));


            mainImage = img.copy();


            QCoreApplication::processEvents();
        }
        qDebug() << "Webcam closing...";
        cap.release();
        qDebug() << "Webcam closed.";
    }
    else{
        qDebug() << "Opening webcam...";
        isCameraOn = true;

        VideoCapture cap(0);

        if (!cap.isOpened()){
            QMessageBox::warning(this, "Camera Error", "Unable to open webcam.");
            return;
        }
        qDebug() << "Camera open.";
        ui->loadingLabel->setVisible(false);

        int frameCounter = 0;
        QElapsedTimer timer;
        timer.start();
        cv::Point tlFace[15], brFace[15], tlCar[15], brCar[15]; //store the coords of the rectangle
        ObjectDetection objectDetection;

        while(isCameraOn){

            Mat frame;
            QImage img;

            cap.read(frame);

            //Aplicam modificarile
            if (contrastValue != 0 || brightnessValue != 0 || saturationValue != 0 || hueValue != -100 || sharpnessValue != 0) {
                Mat adjustedImg;
                frame.convertTo(adjustedImg, -1, 1.0 + contrastValue/100.0, brightnessValue);
                cv::cvtColor(adjustedImg, adjustedImg, cv::COLOR_BGR2HSV);
                cv::Mat hsv_channels[3];
                cv::split(adjustedImg, hsv_channels);
                hsv_channels[0] += (hueValue + 100) * 1.8; //from[-100,100] to [0,360]
                hsv_channels[1] += saturationValue;
                hsv_channels[2] += sharpnessValue;
                cv::merge(hsv_channels, 3, adjustedImg);
                cv::cvtColor(adjustedImg, adjustedImg, cv::COLOR_HSV2BGR);
                frame = adjustedImg;
            }

            frameCounter++;
            if(timer.elapsed() >= 1000){
                std::string myStr = "FPS: " + std::to_string(frameCounter);
                QString valueInString = QString::fromStdString(myStr);
                ui->fpsLabel->setText(valueInString);

                timer.restart();
                frameCounter = 0;
            }
            if(dnnDetectbool){
                img = MatToQImage(dnnDetection(frame));
            }


            if(faceDetect || carDetect || pedestrianDetect || busDetect || bikeDetect)
                img = MatToQImage(objectDetection.getDetection(frame,faceDetect,carDetect,pedestrianDetect,busDetect,bikeDetect));
            else
                img = MatToQImage(frame);
            // Convert the adjusted frame to a QImage

            QImage scaledImage = img.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            mainImage = scaledImage;
            ui->imageLabel->setAlignment(Qt::AlignCenter);
            ui->imageLabel->setPixmap(QPixmap::fromImage(scaledImage));


            mainImage = img.copy();


            QCoreApplication::processEvents();
        }
        qDebug() << "Webcam closing...";
        cap.release();
        qDebug() << "Webcam closed.";
    }
}



void camerawindow::on_startButton_clicked()
{
    if(isWebCam){
        if(isCameraOn){
            isCameraOn = false;
            ui->startButton->setText("Open camera");
        }else{
            ui->startButton->setText("Close camera");
            loadWebcam();
        }
    }
    else{
        if(isCameraOn){
            isCameraOn = false;
            ui->startButton->setText("Open camera");
        }else{
            ui->startButton->setText("Close camera");
            loadVideo();
        }
    }

}

void camerawindow::closeEvent(QCloseEvent *event)
{
    isCameraOn = false;
    QWidget::closeEvent(event);
}


void camerawindow::on_horizontalSlider_valueChanged(int value)
{
    if(ui->contrastButton->isChecked()) {
        contrastValue = value;

        QString labelText = "Value: " + QString::number(value);
        ui->label->setText(labelText);
    } else if(ui->brightnessButton->isChecked()) {
        brightnessValue = value;

        QString labelText = "Value: " + QString::number(value);
        ui->label->setText(labelText);
    } else if(ui->saturationButton->isChecked()) {
        saturationValue = value;

        QString labelText = "Value: " + QString::number(value);
        ui->label->setText(labelText);
    } else if(ui->hueButton->isChecked()) {
        hueValue = value;

        QString labelText = "Value: " + QString::number(value);
        ui->label->setText(labelText);
    } else if(ui->sharpnessButton->isChecked()) {
        sharpnessValue = value;

        QString labelText = "Value: " + QString::number(value);
        ui->label->setText(labelText);
    }else return;
}


void camerawindow::on_contrastButton_clicked()
{
    ui->brightnessButton->setChecked(false);
    ui->saturationButton->setChecked(false);
    ui->hueButton->setChecked(false);
    ui->sharpnessButton->setChecked(false);
    ui->horizontalSlider->setVisible(true);
    ui->label->setVisible(true);

    ui->horizontalSlider->setValue(contrastValue);
}


void camerawindow::on_brightnessButton_clicked()
{
    ui->contrastButton->setChecked(false);
    ui->saturationButton->setChecked(false);
    ui->hueButton->setChecked(false);
    ui->sharpnessButton->setChecked(false);
    ui->horizontalSlider->setVisible(true);
    ui->label->setVisible(true);

    ui->horizontalSlider->setValue(brightnessValue);
}


void camerawindow::on_saturationButton_clicked()
{
    ui->contrastButton->setChecked(false);
    ui->brightnessButton->setChecked(false);
    ui->hueButton->setChecked(false);
    ui->sharpnessButton->setChecked(false);
    ui->horizontalSlider->setVisible(true);
    ui->label->setVisible(true);

    ui->horizontalSlider->setValue(saturationValue);
}


void camerawindow::on_hueButton_clicked()
{
    ui->contrastButton->setChecked(false);
    ui->saturationButton->setChecked(false);
    ui->brightnessButton->setChecked(false);
    ui->sharpnessButton->setChecked(false);
    ui->horizontalSlider->setVisible(true);
    ui->label->setVisible(true);

    ui->horizontalSlider->setValue(hueValue);
}


void camerawindow::on_sharpnessButton_clicked()
{
    ui->contrastButton->setChecked(false);
    ui->saturationButton->setChecked(false);
    ui->hueButton->setChecked(false);
    ui->brightnessButton->setChecked(false);
    ui->horizontalSlider->setVisible(true);
    ui->label->setVisible(true);

    ui->horizontalSlider->setValue(sharpnessValue);
}


void camerawindow::on_resetButton_clicked()
{
    contrastValue = 0;
    brightnessValue = 0;
    saturationValue = 0;
    hueValue = -100;
    sharpnessValue = 0;

    ui->horizontalSlider->setVisible(false);
    ui->label->setVisible(false);
}


void camerawindow::on_editImgButton_clicked()
{
    MainWindow MW;
    MW.mainImage = mainImage;
    imageWindow ImageWindow(MW);

    if(mainImage.isNull()){
        QMessageBox::critical(nullptr, "Error", "No photo selected.");
        return;
    }

    ImageWindow.ui->horizontalSlider->setVisible(false);
    ImageWindow.ui->horizontalSlider_2->setVisible(false);
    ImageWindow.ui->horizontalSlider_3->setVisible(false);

    ImageWindow.ui->labelSlider1->setVisible(false);
    ImageWindow.ui->labelSlider2->setVisible(false);
    ImageWindow.ui->labelSlider3->setVisible(false);

    ImageWindow.loadImage(mainImage);

    ImageWindow.setModal(true);
    ImageWindow.exec();
}



void camerawindow::on_faceCheckBox_stateChanged(int arg1)
{
    if(faceDetect){
        faceDetect = false;
    }
    else faceDetect = true;
}


void camerawindow::on_carCheckBox_stateChanged(int arg1)
{
    if(carDetect){
        carDetect = false;
    }
    else carDetect = true;
}


void camerawindow::on_pedestrianCheckBox_stateChanged(int arg1)
{
    if(pedestrianDetect){
        pedestrianDetect = false;
    }
    else pedestrianDetect = true;
}


void camerawindow::on_busCheckBox_stateChanged(int arg1)
{
    if(busDetect){
        busDetect = false;
    }
    else busDetect = true;
}


void camerawindow::on_bikeCheckBox_stateChanged(int arg1)
{
    if(bikeDetect){
        bikeDetect = false;
    }
    else bikeDetect = true;
}



void camerawindow::on_checkBox_stateChanged(int arg1)
{
    if(dnnDetectbool){
        dnnDetectbool = false;
    }
    else dnnDetectbool = true;
}


void camerawindow::on_loadVideoButton_clicked()
{
    externalVideo = true;
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select Video File", "", "Video Files (*.mp4 *.avi)");
    filePathOfVideo = filePath.toStdString();
}

