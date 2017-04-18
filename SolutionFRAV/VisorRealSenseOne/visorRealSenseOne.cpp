//Comentario
#include "visorRealSenseOne.h"
#include "pxcsensemanager.h"
#include "pxcmetadata.h"
#include "util_cmdline.h"
#include "util_render.h"
#include <conio.h>
#include "qfiledialog.h"
#include "qmessagebox.h"

#include <iostream>
#include <fstream>
#include <winsock.h>
#include <windows.h>
#include <iostream>


#pragma comment(lib, "Ws2_32.lib")


//--------------------------------------------------------------------------------------------------------------
VisorRealSenseOne::VisorRealSenseOne(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

//--------------------------------------------------------------------------------------------------------------
VisorRealSenseOne::~VisorRealSenseOne()
{
}

//--------------------------------------------------------------------------------------------------------------
void VisorRealSenseOne::showImg(const cv::Mat &imagen)
{
	if (ui.rBRealSizeImg_1->isChecked())
		cv::imshow("RealSize", imagen);
	else
		cv::destroyWindow("RealSize");

	cv::Mat imgMostrar;
	if (imagen.type() == CV_8UC1)
		cv::cvtColor(imagen, imgMostrar, cv::COLOR_GRAY2RGB);
	else
		cv::cvtColor(imagen, imgMostrar, cv::COLOR_BGR2RGB);

	QImage imagenQT = QImage((const unsigned char *)imgMostrar.data, imgMostrar.cols, imgMostrar.rows, imgMostrar.step, QImage::Format_RGB888);
	ui.lImg_1->setPixmap(QPixmap::fromImage(imagenQT));
	ui.lImg_1->setScaledContents(true);
	ui.lImg_1->repaint();
	qApp->processEvents();
	imgMostrar.release();
}

//--------------------------------------------------------------------------------------------------------------
int VisorRealSenseOne::stream()
{
	stopStream = false;

	//Stream Data		
	while (!stopStream) //(int nframes = 0; nframes<cmdl.m_nframes; nframes++) {
	{
		ImgType imgType = RGB;
		ImgAdjustType adjustType = REAL;
	
		if (ui.rBViewReal->isChecked())
			adjustType = REAL;
		else if (ui.rBViewAdjustDepth->isChecked())
			adjustType = ADJUST_DEPTH;
		else if (ui.rBViewAdjustRGB->isChecked())
			adjustType = ADJUST_RGB;

		if (this->ui.rBRGB->isChecked())
			imgType = RGB;
		else if (this->ui.rBDepth->isChecked())
			imgType = DEPTH;
		else if (this->ui.rBIR->isChecked())
			imgType = IR;

		cv::Mat img = cam->capture(imgType, adjustType);
		this->showImg(img);

		qApp->processEvents();

		if (_kbhit()) { // Break loop
			int c = _getch() & 255;
			if (c == 27 || c == 'q' || c == 'Q') break; // ESC|q|Q for Exit
		}
	}

	wprintf_s(L"Exiting\n");

	return 0;
}

//--------------------------------------------------------------------------------------------------------------
int VisorRealSenseOne::view()
{
	cam = new Cam3D();
	cam->init();

	stream();

	return 0;
}

//--------------------------------------------------------------------------------------------------------------
void VisorRealSenseOne::stop()
{
	stopStream = true;

	cam->stop();
	delete cam;
}