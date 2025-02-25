#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "imagewindow.h"
#include "ui_imagewindow.h"

#include "camerawindow.h"
#include "ui_camerawindow.h"

#include <iostream>
#include<opencv2/core.hpp>

#include <QFileDialog>
#include <QMessageBox>
#include <QMovie>
#include <QImage>
#include <QDebug>
#include <QTimer>

#include "Spinnaker.h"
#include "SpinGenApi/SpinnakerGenApi.h"

#include <exception>

using namespace Spinnaker;
using namespace Spinnaker::GenApi;
using namespace Spinnaker::GenICam;
using namespace cv;

using namespace cv;
using namespace std;



QImage MatToQImage2(const cv::Mat& mat);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->contrastSlider->setVisible(false);
    ui->focusSlider->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loadImageButton_clicked()
{
    isButtonPressed = false;

    QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "C:/Users/uif87690/Pictures/Saved Pictures", tr("Images (*.png *.jpg *.jpeg *.gif)"));
    if(QString::compare(filename,QString()) != 0)
    {
        QImage image;

        bool valid = image.load(filename);

        if(valid){

            QImage scaledImage = image.scaled(ui->loadImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            mainImage = scaledImage;
            ui->loadImageLabel->setAlignment(Qt::AlignCenter);
            ui->loadImageLabel->setPixmap(QPixmap::fromImage(scaledImage));


        }
        else {
            QMessageBox::critical(nullptr, "Error", "Image invalid.");
        }

    }
}

void MainWindow::on_cameraButton_clicked()    
{
    camerawindow* Camera = new camerawindow();
    Camera->isWebCam = true;
    Camera->show();
}

void MainWindow::on_ExternalCameraButton_clicked()
{
    camerawindow* Camera = new camerawindow();
    Camera->isWebCam = false;
    Camera->show();

}



QImage MatToQImage2(const cv::Mat& mat)
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




void MainWindow::on_editButton_clicked()
{
    isButtonPressed = false;
    isCameraOn = false;
    if(mainImage.isNull()){
        QMessageBox::critical(nullptr, "Error", "No photo selected.");
        return;
    }
    imageWindow ImageWindow(*this);

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



void MainWindow::on_algButton1_clicked()
{
    algName = "BlackAndWhite";
    imageWindow ImageWindow(*this);

    ImageWindow.loadImage(mainImage);

    ImageWindow.ui->horizontalSlider->setVisible(false);
    ImageWindow.ui->horizontalSlider_2->setVisible(false);

    ImageWindow.ui->labelSlider1->setVisible(false);
    ImageWindow.ui->labelSlider2->setVisible(false);

    ImageWindow.setModal(true);
    ImageWindow.exec();
}



void MainWindow::on_invertButton_clicked()
{
    algName = "ImageInversion";
    imageWindow ImageWindow(*this);


    ImageWindow.loadImage(mainImage);


    ImageWindow.ui->horizontalSlider->setVisible(false);
    ImageWindow.ui->horizontalSlider_2->setVisible(false);

    ImageWindow.ui->labelSlider1->setVisible(false);
    ImageWindow.ui->labelSlider2->setVisible(false);


    ImageWindow.setModal(true);
    ImageWindow.exec();

}


void MainWindow::on_binarizeButton_clicked()
{
    algName = "ImageBinarization";
    imageWindow ImageWindow(*this);

    ImageWindow.loadImage(mainImage);

    ImageWindow.ui->horizontalSlider->setVisible(true);
    ImageWindow.ui->horizontalSlider_2->setVisible(false);

    ImageWindow.ui->labelSlider1->setVisible(true);
    ImageWindow.ui->labelSlider2->setVisible(false);

    ImageWindow.setModal(true);
    ImageWindow.exec();
}


void MainWindow::on_contrastButton_clicked()
{
    algName = "ImageChangeContrast";
    imageWindow ImageWindow(*this);

    ImageWindow.loadImage(mainImage);

    ImageWindow.ui->horizontalSlider->setVisible(true);
    ImageWindow.ui->horizontalSlider_2->setVisible(false);

    ImageWindow.ui->labelSlider1->setVisible(true);
    ImageWindow.ui->labelSlider2->setVisible(false);

    ImageWindow.setModal(true);
    ImageWindow.exec();

}


void MainWindow::on_noiseButton_clicked()
{
    algName = "ImageNoiseReduction";
    imageWindow ImageWindow(*this);


    ImageWindow.loadImage(mainImage);

    ImageWindow.ui->horizontalSlider->setVisible(true);
    ImageWindow.ui->horizontalSlider_2->setVisible(false);

    ImageWindow.ui->labelSlider1->setVisible(true);
    ImageWindow.ui->labelSlider2->setVisible(false);

    ImageWindow.ui->horizontalSlider->setValue(10);

    ImageWindow.setModal(true);
    ImageWindow.exec();
}


void MainWindow::on_windowButton_clicked()
{
    algName = "ImageWindowOperation";
    imageWindow ImageWindow(*this);


    ImageWindow.loadImage(mainImage);

    ImageWindow.ui->horizontalSlider->setVisible(true);
    ImageWindow.ui->horizontalSlider_2->setVisible(true);

    ImageWindow.ui->labelSlider1->setVisible(true);
    ImageWindow.ui->labelSlider2->setVisible(true);

    ImageWindow.setModal(true);
    ImageWindow.exec();
}





