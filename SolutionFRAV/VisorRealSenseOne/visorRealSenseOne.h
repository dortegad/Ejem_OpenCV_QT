#ifndef EJEM_OPENCV_QT_H
#define EJEM_OPENCV_QT_H

#include <QtWidgets/QMainWindow>
#include "ui_visorRealSenseOne.h"


//OPENCV
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

//REALSENSE
#include "pxcsensemanager.h"
#include "pxcmetadata.h"
#include "util_cmdline.h"
#include "util_render.h"

//STD
#include <conio.h>


class VisorRealSenseOne : public QMainWindow
{
	Q_OBJECT

public slots:
	int view();
	int record();
	int play();
	void stop();
	void selectFile();

public:
	VisorRealSenseOne(QWidget *parent = 0);
	~VisorRealSenseOne();

	static const std::string WINNAME_RGB;
	static const std::string WINNAME_Depth;
	static const std::string WINNAME_IR;
	static const std::string WINNAME_FLIR;

private:
	Ui::VisorRealSenseOneClass ui;
	bool stopStream;
 	HANDLE hThreadSocket;

	void showImg_1(const cv::Mat &imagen);
	void createMapDepth(PXCProjection *projection,
						PXCImage *depth,
						std::vector<PXCPoint3DF32> &mapDepth);
	void ConvertPXCImageToOpenCVMat(PXCImage *inImg,
									cv::Mat *outImg);
	bool showSampleAdjustColor(PXCCapture::Device *device,
							  const PXCCapture::Sample *sample);
	bool showSampleReal(PXCCapture::Device *device,
						const PXCCapture::Sample *sample);
	bool showSampleAdjustDepth(PXCCapture::Device *device,
							   const PXCCapture::Sample *sample);
	PXCSenseManager *setup();
	int stream(PXCSenseManager *pp);
};

#endif // EJEM_OPENCV_QT_H
