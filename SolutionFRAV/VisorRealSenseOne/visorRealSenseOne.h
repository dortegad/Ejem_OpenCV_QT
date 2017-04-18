#ifndef EJEM_OPENCV_QT_H
#define EJEM_OPENCV_QT_H

#include <QtWidgets/QMainWindow>
#include "ui_visorRealSenseOne.h"


//OPENCV
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

//STD
#include <conio.h>

#include "Cam3D.h"


class VisorRealSenseOne : public QMainWindow
{
	Q_OBJECT

public slots:
	int view();
	void stop();

public:
	VisorRealSenseOne(QWidget *parent = 0);
	~VisorRealSenseOne();
private:
	Ui::VisorRealSenseOneClass ui;
	bool stopStream;
	Cam3D *cam;

	void showImg(const cv::Mat &imagen);
	int stream();
};

#endif // EJEM_OPENCV_QT_H
