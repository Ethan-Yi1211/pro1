#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


#include <QCoreApplication>
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QImage mainImage;

    QString algName;

    bool isButtonPressed = false;
    bool isCameraOn = false;

private slots:


    void on_loadImageButton_clicked();

    void on_algButton1_clicked();

    void on_invertButton_clicked();

    void on_binarizeButton_clicked();

    void on_contrastButton_clicked();

    void on_noiseButton_clicked();

    void on_windowButton_clicked();

    void on_cameraButton_clicked();

    void on_editButton_clicked();

    void on_ExternalCameraButton_clicked();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
