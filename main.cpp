#include "mainwindow.h"
#include<opencv2/core.hpp>
#include <QCoreApplication>
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>

using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
