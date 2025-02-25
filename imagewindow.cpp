#include "imagewindow.h"
#include "ui_imagewindow.h"

#include "Algorithms.h"
#include "Algorithms/BlackAndWhite.cpp"
#include "Algorithms/ImageInversion.cpp"
#include "Algorithms/ImageBinarization.cpp"
#include "Algorithms/ImageChangeContrast.cpp"
#include "Algorithms/ImageChangeBrightness.cpp"
#include "Algorithms/ImageColorCorrection.cpp"
#include "Algorithms/ImageNoiseReduction.cpp"
#include "Algorithms/ImageWindowOperation.cpp"
#include "Algorithms/ImageLineDetection.cpp"
#include "Algorithms/WatershedAlgorithm.cpp"
#include "Algorithms/SemanticSegmentation.cpp"
#include "Algorithms/FaceDetection.cpp"
#include "Algorithms/CarDetection.cpp"
#include "Algorithms/PedestrianDetection.cpp"
#include<QTimer>
#include<QMessageBox>
#include<QFileDialog>
#include<QSpinBox>
#include<QRubberBand>
#include<QMouseEvent>
#include<QPainter>
#include<QMenuBar>

#include <iostream>



imageWindow::imageWindow(MainWindow& mw,QWidget *parent) :
    QDialog(parent), MW(mw) ,
    ui(new Ui::imageWindow)
{
    this->auxImage = MW.mainImage;
    ui->setupUi(this);
    connect(ui->cropButton, &QPushButton::clicked, this, &imageWindow::cropImage);
}

imageWindow::~imageWindow()
{
    delete ui;
}


void imageWindow::loadImage(QImage image)
{

        undoImage = modifiedImage;
        modifiedImage = image;
        image = image.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imageLabel->setAlignment(Qt::AlignCenter);
        ui->imageLabel->setPixmap(QPixmap::fromImage(image));

}

QImage imageWindow::circleImage(QImage img)
{
    int diameter = qMin(img.width(), img.height());
    QImage croppedImage(diameter, diameter, QImage::Format_ARGB32);
    croppedImage.fill(Qt::transparent);

    QPainter painter(&croppedImage);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, diameter, diameter);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.drawImage((diameter - img.width()) / 2, (diameter - img.height()) / 2, img);
    painter.end();

    return croppedImage;
}
QImage imageWindow::elipseImage(QImage img)
{
    int width = img.width();
    int height = img.height();

    QImage croppedImage(width, height, QImage::Format_ARGB32);
    croppedImage.fill(Qt::transparent);

    QPainter painter(&croppedImage);
    painter.setRenderHint(QPainter::Antialiasing);

    // The ellipse will fit inside the rectangle with this size
    QRectF ellipseRect(0, 0, width, height);

    // Draw an ellipse in the center of the image
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(ellipseRect);

    // Set the composition mode so that the ellipse is used as a mask
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);

    // Draw the original image inside the ellipse
    painter.drawImage((width - img.width()) / 2, (height - img.height()) / 2, img);

    painter.end();

    // Crop the image to the ellipse
    QImage finalImage = croppedImage.copy(ellipseRect.toRect());

    return finalImage;
}


QImage imageWindow::squareImage(QImage img){
    int cropSize = qMin(img.width(), img.height()); // Crop to a square
    QRect cropRect((img.width() - cropSize) / 2, (img.height() - cropSize) / 2, cropSize, cropSize);
    QImage croppedImage = img.copy(cropRect);
    return croppedImage;
}


void imageWindow::on_horizontalSlider_valueChanged(int value)
{   Algorithms alg(modifiedImage);

    if(MW.algName == "ImageBinarization"){

        ImageBinarization* imageBinarization2 = new ImageBinarization(*this);
        imageBinarization2->start();

        std::string myStr = "Value: " + std::to_string(value);  //changes the value in real time
        QString valueInString = QString::fromStdString(myStr);
        ui->labelSlider1->setText(valueInString);
    }
    else if (MW.algName == "ImageChangeContrast" ){

        ImageChangeContrast imageChangeContrast(*this);
        imageChangeContrast.start();
        //loadImage(imageChangeContrast.getImage(value));

        std::string myStr = "Value: " + std::to_string(value);
        QString valueInString = QString::fromStdString(myStr);
        ui->labelSlider1->setText(valueInString);
    }
    else if (MW.algName== "ImageNoiseReduction"){

        ImageNoiseReduction imageNoiseReduction(*this);
        imageNoiseReduction.start();
        //modifiedImage = alg.ImageNoiseReduction(value,ui->horizontalSlider_2->value());
        //loadImage(alg.ImageNoiseReduction(value));

        std::string myStr = "Value: " + std::to_string(value);
        QString valueInString = QString::fromStdString(myStr);
        ui->labelSlider1->setText(valueInString);
    }
    else if (MW.algName == "ImageWindowOperation"){

        ImageWindowOperation imageWindowOperation(*this);
        imageWindowOperation.start();
        //modifiedImage = alg.ImageWindowOperation(value,ui->horizontalSlider_2->value());
        //loadImage(alg.ImageWindowOperation(value,ui->horizontalSlider_2->value()));

        std::string myStr = "Value: " + std::to_string(value);
        QString valueInString = QString::fromStdString(myStr);
        ui->labelSlider1->setText(valueInString);
    }
    else if (MW.algName == "ImageChangeBrightness"){

        ImageChangeBrightness imageChangeBrightness(*this);
        imageChangeBrightness.start();
        //loadImage(alg.ImageChangeBrightness(value));

        std::string myStr = "Value: " + std::to_string(value);
        QString valueInString = QString::fromStdString(myStr);
        ui->labelSlider1->setText(valueInString);
    }
    else if (MW.algName == "ImageColorCorrection"){

        ImageColorCorrection imageColorCorrection(*this);
        imageColorCorrection.start();
        //loadImage(alg.ImageColorCorrection(value, ui->horizontalSlider_2->value(),ui->horizontalSlider_3->value()));

        std::string myStr = "Red: " + std::to_string(value);
        QString valueInString = QString::fromStdString(myStr);
        ui->labelSlider1->setText(valueInString);
    }

}




void imageWindow::on_horizontalSlider_2_valueChanged(int value)
{


    if(MW.algName == "ImageWindowOperation"){

        ImageWindowOperation imageWindowOperation(*this);
        imageWindowOperation.start();


        std::string myStr = "Value: " + std::to_string(value);
        QString valueInString = QString::fromStdString(myStr);
        ui->labelSlider2->setText(valueInString);
        ui->labelSlider2->setVisible(true);
    }
    else if (MW.algName == "ImageColorCorrection"){

        ImageColorCorrection imageColorCorrection(*this);
        imageColorCorrection.start();
        //loadImage(alg.ImageColorCorrection(ui->horizontalSlider->value(), value,ui->horizontalSlider_3->value()));

        std::string myStr = "Green: " + std::to_string(value);
        QString valueInString = QString::fromStdString(myStr);
        ui->labelSlider2->setText(valueInString);
    }


}

void imageWindow::on_horizontalSlider_3_valueChanged(int value)
{
    if(MW.algName == "ImageColorCorrection"){

        ImageColorCorrection imageColorCorrection(*this);
        imageColorCorrection.start();
        //loadImage(alg.ImageColorCorrection(ui->horizontalSlider->value(), ui->horizontalSlider_2->value(),value));

        std::string myStr = "Blue: " + std::to_string(value);
        QString valueInString = QString::fromStdString(myStr);
        ui->labelSlider3->setText(valueInString);
    }
}

void imageWindow::on_algButton1_clicked() //black and white
{
    auxImage = modifiedImage;
    BlackAndWhite blackAndWhite(*this);
    blackAndWhite.start();

}


void imageWindow::on_invertButton_clicked()
{
    auxImage = modifiedImage;
    ImageInversion imageInversion(*this);
    imageInversion.start();

}


void imageWindow::on_binarizeButton_clicked()
{

     auxImage = modifiedImage;
    MW.algName = "ImageBinarization";
    ImageBinarization imageBinarization(*this);
    ui->horizontalSlider->setValue(100);
    imageBinarization.start();


}


void imageWindow::on_contrastButton_clicked()
{
    auxImage = modifiedImage;
    ui->horizontalSlider->setValue(100);
    MW.algName = "ImageChangeContrast";
    ImageChangeContrast imageChangeContrast(*this);
    imageChangeContrast.start();

}


void imageWindow::on_noiseButton_clicked()
{

    auxImage = modifiedImage;
    MW.algName = "ImageNoiseReduction";
    ImageNoiseReduction imageNoiseReduction(*this);
    ui->horizontalSlider->setValue(10);
    imageNoiseReduction.start();

}

void imageWindow::on_windowButton_clicked()
{
    auxImage = modifiedImage;
    MW.algName = "ImageWindowOperation";
    ImageWindowOperation imageWindowOperation(*this);
    ui->horizontalSlider->setValue(100);
    ui->horizontalSlider_2->setValue(100);
    imageWindowOperation.start();
}


void imageWindow::cropImage(){
    m_cropping = true;
}

void imageWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_cropping) {
        m_cropping = true;

        QPoint pos = ui->imageLabel->mapFrom(this, event->pos());
        QSize imageSize = modifiedImage.size();
        QSize labelSize = ui->imageLabel->size();
        QSize scaledImageSize = ui->imageLabel->pixmap().size();
        mousePos =ui->imageLabel->mapFrom(this, event->pos());

        int dw = (labelSize.width() - scaledImageSize.width()) / 2;
        int dh = (labelSize.height() - scaledImageSize.height()) / 2;
        m_start = QPoint((pos.x() - dw) * imageSize.width() / scaledImageSize.width(),
                          (pos.y() - dh) * imageSize.height() / scaledImageSize.height());

        m_rubberBand = new QRubberBand(QRubberBand::Rectangle, ui->imageLabel);
        m_rubberBand->setGeometry(QRect(mousePos, QSize()));
        m_rubberBand->show();
    }
}

void imageWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((m_cropping && ui->comboBox->currentIndex() == 1) || (m_cropping && ui->comboBox->currentIndex() == 2)) { //if circle or square is selected
        QPoint pos = ui->imageLabel->mapFrom(this, event->pos());
        int size = qMin(qAbs(mousePos.x() - pos.x()), qAbs(mousePos.y() - pos.y()));
        QRect squareRect(mousePos, QSize(size, size));
        m_rubberBand->setGeometry(squareRect.normalized());
    }
    else if (m_cropping){
        QPoint pos = ui->imageLabel->mapFrom(this, event->pos());
        m_rubberBand->setGeometry(QRect(mousePos, pos).normalized());
    }
}

void imageWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_cropping && ui->comboBox->currentIndex() == 0) { //rectangle selected
        m_cropping = false;
        m_rubberBand->hide();

        QPoint pos = ui->imageLabel->mapFrom(this, event->pos());
        QSize imageSize = modifiedImage.size();
        QSize labelSize = ui->imageLabel->size();
        QSize scaledImageSize = ui->imageLabel->pixmap().size();

        int dw = (labelSize.width() - scaledImageSize.width()) / 2;
        int dh = (labelSize.height() - scaledImageSize.height()) / 2;
        m_end = QPoint((pos.x() - dw) * imageSize.width() / scaledImageSize.width(),
                        (pos.y() - dh) * imageSize.height() / scaledImageSize.height());

        QRect cropRect(m_start, m_end);

        QImage croppedImage = modifiedImage.copy(cropRect);
        loadImage(croppedImage);
    }
    else if(m_cropping && ui->comboBox->currentIndex() == 2){ //square selected
        m_cropping = false;
        m_rubberBand->hide();

        QPoint pos = ui->imageLabel->mapFrom(this, event->pos());

        QSize imageSize = modifiedImage.size();
        QSize labelSize = ui->imageLabel->size();
        QSize scaledImageSize = ui->imageLabel->pixmap().size();

        int dw = (labelSize.width() - scaledImageSize.width()) / 2;
        int dh = (labelSize.height() - scaledImageSize.height()) / 2;
        m_end = QPoint((pos.x() - dw) * imageSize.width() / scaledImageSize.width(),
                        (pos.y() - dh) * imageSize.height() / scaledImageSize.height());

        int squareSize = qMin(qAbs(m_end.x() - m_start.x()), qAbs(m_end.y() - m_start.y()));
        m_end = m_start + QPoint(squareSize, squareSize); // Adjust m_end to make square

        QRect cropRect(m_start, m_end);
        QImage croppedImage = modifiedImage.copy(cropRect);
        loadImage(croppedImage);
    }
    else if(m_cropping && ui->comboBox->currentIndex() == 1){//circle
        m_cropping = false;
        m_rubberBand->hide();

        QPoint pos = ui->imageLabel->mapFrom(this, event->pos());

        QSize imageSize = modifiedImage.size();
        QSize labelSize = ui->imageLabel->size();
        QSize scaledImageSize = ui->imageLabel->pixmap().size();

        int dw = (labelSize.width() - scaledImageSize.width()) / 2;
        int dh = (labelSize.height() - scaledImageSize.height()) / 2;
        m_end = QPoint((pos.x() - dw) * imageSize.width() / scaledImageSize.width(),
                        (pos.y() - dh) * imageSize.height() / scaledImageSize.height());

        int squareSize = qMin(qAbs(m_end.x() - m_start.x()), qAbs(m_end.y() - m_start.y()));
        m_end = m_start + QPoint(squareSize, squareSize); // Adjust m_end to make square

        QRect cropRect(m_start, m_end);
        QImage croppedImage = modifiedImage.copy(cropRect);
        loadImage(circleImage(croppedImage));
    }
    else if(m_cropping && ui->comboBox->currentIndex() == 3){ // elipsa
        m_cropping = false;
        m_rubberBand->hide();

        QPoint pos = ui->imageLabel->mapFrom(this, event->pos());
        QSize imageSize = modifiedImage.size();
        QSize labelSize = ui->imageLabel->size();
        QSize scaledImageSize = ui->imageLabel->pixmap().size();

        int dw = (labelSize.width() - scaledImageSize.width()) / 2;
        int dh = (labelSize.height() - scaledImageSize.height()) / 2;
        m_end = QPoint((pos.x() - dw) * imageSize.width() / scaledImageSize.width(),
                        (pos.y() - dh) * imageSize.height() / scaledImageSize.height());

        QRect cropRect(m_start, m_end);

        QImage croppedImage = modifiedImage.copy(cropRect);
        loadImage(elipseImage(croppedImage));
    }
}




void imageWindow::on_saveButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.bmp)");

        if (!filePath.isEmpty()) {
            // save the image to the selected file path
            modifiedImage.save(filePath);
    }
}


void imageWindow::on_refreshButton_clicked()
{
    loadImage(MW.mainImage);
    auxImage=MW.mainImage;
}

void imageWindow::on_undoButton_clicked()
{
    loadImage(undoImage);
}


void imageWindow::on_brightnessButton_clicked()
{

    auxImage = modifiedImage;
    MW.algName = "ImageChangeBrightness";

    ImageChangeBrightness imageChangeBrightness(*this);
    ui->horizontalSlider->setValue(125);
    imageChangeBrightness.start();
}


void imageWindow::on_pushButton_clicked() //color correction
{

    auxImage = modifiedImage;
    MW.algName = "ImageColorCorrection";
    ImageColorCorrection imageColorCorrection(*this);
    ui->horizontalSlider->setValue(125);
    ui->horizontalSlider_2->setValue(125);
    ui->horizontalSlider_3->setValue(125);
    imageColorCorrection.start();
}


void imageWindow::on_lineButton_clicked()
{

}


void imageWindow::on_comboBox_activated(int index)
{

    comboBoxCrop = true;

}






void imageWindow::on_comboBox_2_currentIndexChanged(int index)
{
    if(ui->comboBox_2->currentIndex() == 1) //Image Segmentation with Watershed Algorithm
    {
        WatershedAlgorithm img(modifiedImage);
        loadImage(img.GetImage());
    }
    else if(ui->comboBox_2->currentIndex() == 2){//Canny edge detect
        auxImage = modifiedImage;
        MW.algName = "ImageLineDetection";
        ImageLineDetection* imageLineDetection = new ImageLineDetection(*this);
        imageLineDetection->start();
    }
    else if(ui->comboBox_2->currentIndex() == 3){ //semantic seg
        SemanticSegmentation img(modifiedImage);
        loadImage(img.GetImage());
    }
    else if(ui->comboBox_2->currentIndex() == 4){ //face detect seg
        FaceDetection img(modifiedImage);
        loadImage(img.GetImage());
    }
    else if(ui->comboBox_2->currentIndex() == 5){ //car detect seg
        CarDetection img(modifiedImage);
        loadImage(img.GetImage());
    }
    else if(ui->comboBox_2->currentIndex() == 6){ //pedestrian detect seg
        PedestrianDetection img(modifiedImage);
        loadImage(img.GetImage());
    }
}

