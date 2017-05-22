//Comentario
#include "visorAttack.h"
#include "pxcsensemanager.h"
#include "pxcmetadata.h"
#include "util_cmdline.h"
#include "util_render.h"
#include <conio.h>
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qthread.h"

#include <iostream>
#include <fstream>
#include <winsock.h>
#include <windows.h>
#include <iostream>

#include "util_faces.h"

#pragma comment(lib, "Ws2_32.lib")


//--------------------------------------------------------------------------------------------------------------
VisorAttack::VisorAttack(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.lBonaFide->setVisible(false);
	ui.lAttack->setVisible(false);

	cam = new CamF200();
	cam->load();
}

//--------------------------------------------------------------------------------------------------------------
VisorAttack::~VisorAttack()
{
}

//--------------------------------------------------------------------------------------------------------------
void VisorAttack::showImg(const cv::Mat &imagen)
{
	//if (ui.rBRealSizeImg_1->isChecked())
	//	cv::imshow("RealSize", imagen);
	//else
	//	cv::destroyWindow("RealSize");

	cv::Mat imgMostrar = imagen;
	//if (imagen.type() == CV_8UC1)
	//	cv::cvtColor(imagen, imgMostrar, cv::COLOR_GRAY2RGB);
	//else
	cv::cvtColor(imagen, imgMostrar, cv::COLOR_BGR2RGB);

	QImage imagenQT = QImage((const unsigned char *)imgMostrar.data, imgMostrar.cols, imgMostrar.rows, imgMostrar.step, QImage::Format_RGB888);
	ui.lImg_1->setPixmap(QPixmap::fromImage(imagenQT));
	ui.lImg_1->setScaledContents(true);
	ui.lImg_1->repaint();
	qApp->processEvents();
	imgMostrar.release();
}

//--------------------------------------------------------------------------------------------------------------
int VisorAttack::stream()
{
	stopStream = false;
	std::vector<float> attackResults;
	int numFramesAttackVerify = 3;

	int framesShowResult = 0;

	while (!stopStream)
	{
		if (framesShowResult > 0)
			framesShowResult--;
		else
		{
			ui.lBonaFide->setVisible(false);
			ui.lAttack->setVisible(false);
		}

		cv::Mat rgbImg;
		cv::Mat depthImg;
		cam->capture(rgbImg, depthImg);

		cv::Rect rectFace;
		Util_Faces::detectFace(rgbImg, rectFace);

		if (verifyAttack)
		{
			float result = cam->isAttackFrame(rgbImg, depthImg);
			attackResults.push_back(result);
			if (attackResults.size() == numFramesAttackVerify)
			{ 
				std::vector<float>::iterator itMin = std::min_element(attackResults.begin(), attackResults.end());
				float minScore = *itMin;
				std::cout << "RESULT = " << minScore;
				if (minScore > 0.28)
				{
					ui.lBonaFide->setVisible(true);
					std::cout << " = BONA FIDE" << std::endl;

				}
				else
				{
					ui.lAttack->setVisible(true);
					std::cout << " = ATTACK" << std::endl;
				}
				framesShowResult = 10;
				attackResults.clear();
				verifyAttack = false;
			}
		}

		cv::rectangle(rgbImg, rectFace, cv::Scalar(0, 0, 255), 3);
		this->showImg(rgbImg);

		//qApp->processEvents();
	}

	return 0;
}

//--------------------------------------------------------------------------------------------------------------
int VisorAttack::view()
{
	cam->init();

	stream();

	return 0;
}

//--------------------------------------------------------------------------------------------------------------
void VisorAttack::stop()
{
	if (stopStream)
		return;

	stopStream = true;

	cam->stop();
	delete cam;
}

//--------------------------------------------------------------------------------------------------------------
void VisorAttack::isAttack()
{
	verifyAttack = true;
}