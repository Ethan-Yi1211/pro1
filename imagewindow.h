#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include"mainwindow.h"


#include <QDialog>
#include <QRubberBand>

namespace Ui {
class imageWindow;
}

class imageWindow : public QDialog
{
    Q_OBJECT

public:
    explicit imageWindow(MainWindow& mw, QWidget *parent = nullptr);
    ~imageWindow();

    void loadImage(QImage image);

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    QImage circleImage(QImage);
    QImage squareImage(QImage);
    QImage elipseImage(QImage);

    QPoint m_start; // Declare the starting point for cropping
    QPoint m_end;   // Declare the ending point for cropping
    QPoint mousePos;
    bool m_cropping = false;
    bool comboBoxCrop = false;
    QRubberBand* m_rubberBand;

    MainWindow& MW;

    QImage modifiedImage;

    QImage auxImage;
    QImage undoImage;

    QImage originalImage;



private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_algButton1_clicked();

    void on_invertButton_clicked();

    void on_binarizeButton_clicked();

    void on_contrastButton_clicked();

    void on_noiseButton_clicked();

    void on_saveButton_clicked();

    void cropImage();

    void on_refreshButton_clicked();

    void on_windowButton_clicked();


    void on_brightnessButton_clicked();

    void on_pushButton_clicked();

    void on_horizontalSlider_3_valueChanged(int value);

    void on_lineButton_clicked();

    void on_comboBox_activated(int index);

    void on_undoButton_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

public:
    Ui::imageWindow *ui;
};

#endif // IMAGEWINDOW_H
