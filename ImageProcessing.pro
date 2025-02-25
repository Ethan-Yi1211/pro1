QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# OpenCV 4.10.0 配置
win32:CONFIG(release, debug|release): LIBS += -LD:/OpenCV/opencv/build/x64/vc16/lib/ -lopencv_world4100
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/OpenCV/opencv/build/x64/vc16/lib/ -lopencv_world4100d
else:unix: LIBS += -LD:/OpenCV/opencv/build/x64/vc16/lib/ -lopencv_world4100

INCLUDEPATH += D:/OpenCV/opencv/build/include
DEPENDPATH += D:/OpenCV/opencv/build/include

# FLIR Spinnaker 配置
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Program Files/FLIR Systems/Spinnaker/lib64/vs2015/' -lSpinnaker_v140
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Program Files/FLIR Systems/Spinnaker/lib64/vs2015/' -lSpinnaker_v140d
else:unix: LIBS += -L$$PWD/'../../../../Program Files/FLIR Systems/Spinnaker/lib64/vs2015/' -lSpinnaker_v140

INCLUDEPATH += $$PWD/'../../../../Program Files/FLIR Systems/Spinnaker/include'
DEPENDPATH += $$PWD/'../../../../Program Files/FLIR Systems/Spinnaker/include'

# 资源文件
RESOURCES += resources.qrc

SOURCES += \
    Algorithms.cpp \
    Algorithms/BlackAndWhite.cpp \
    Algorithms/CarDetection.cpp \
    Algorithms/DNN.cpp \
    Algorithms/FaceDetection.cpp \
    Algorithms/ImageBinarization.cpp \
    Algorithms/ImageChangeBrightness.cpp \
    Algorithms/ImageChangeContrast.cpp \
    Algorithms/ImageColorCorrection.cpp \
    Algorithms/ImageInversion.cpp \
    Algorithms/ImageLineDetection.cpp \
    Algorithms/ImageNoiseReduction.cpp \
    Algorithms/ImageWindowOperation.cpp \
    Algorithms/PedestrianDetection.cpp \
    Algorithms/SemanticSegmentation.cpp \
    Algorithms/WatershedAlgorithm.cpp \
    Algorithms/objectdetection.cpp \
    camerawindow.cpp \
    imagewindow.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Algorithms.h \
    Algorithms/DNN.h \
    Algorithms/objectdetection.h \
    Resources.h \
    camerawindow.h \
    imagewindow.h \
    mainwindow.h

FORMS += \
    camerawindow.ui \
    imagewindow.ui \
    mainwindow.ui

# 部署规则
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Algorithms/haarcascade_frontalface_default.xml \
    Algorithms/input/DenseNet_121.caffemodel \
    Algorithms/input/DenseNet_121.prototxt \
    Resources/haarcascade_frontalface_default.xml \
    haarcascade_frontalface_default.xml
