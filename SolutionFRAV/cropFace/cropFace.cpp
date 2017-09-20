// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <util_faces.h>
#include <util_depth.h>
#include <util_LBP_Dlib.h>
#include <util_LBP_CV.h>
#include <util_fravAttack.h>
#include <util_landmarksFace.h>
#include <util_draw.h>
#include <util_files.h>


//------------------------------------------------------------------------------
void imgsFaceZones(cv::Mat imgFace, 
					std::vector<cv::Point2f> points,
					cv::Mat &imgEyeLeft,
					cv::Mat &imgEyeRight,
					cv::Mat &imgMouth,
					cv::Mat &imgNoise,
					cv::Mat &imgForehead, 
					cv::Mat &imgHearLeft, 
					cv::Mat &imgHearRight,
					cv::Mat &imgDropFace,
					cv::Mat &imgBorderFace)
{
	//Check rect is into image
	cv::Rect imgRect(0, 0, imgFace.cols, imgFace.rows);

	//eye left 37 38 39 40 41 42
	std::vector<cv::Point2f> eyeLeft;
	for (int i = 36; i < 42; i++)
		eyeLeft.push_back(points[i]);
	//Util_Draw::boundingImg(imgFace, eyeLeft, imgEyeLeft);
	cv::Rect rectEyeLeft = cv::boundingRect(eyeLeft);
	rectEyeLeft.x = rectEyeLeft.x - (rectEyeLeft.width / 3);
	rectEyeLeft.y = rectEyeLeft.y - (rectEyeLeft.height / 2);
	rectEyeLeft.width = rectEyeLeft.width + ((rectEyeLeft.width / 3)*2);
	rectEyeLeft.height = rectEyeLeft.height + ((rectEyeLeft.height / 2)*2);
	imgEyeLeft = imgFace(rectEyeLeft & imgRect);
	//cv::imshow("LEFT EYE", imgEyeLeft);

	//eye right 43 44 45 46 47 48
	std::vector<cv::Point2f> eyeRight;
	for (int i = 42; i < 48; i++)
		eyeRight.push_back(points[i]);
	//Util_Draw::boundingImg(imgFace, eyeRight, imgEyeRight);
	cv::Rect rectEyeRight = cv::boundingRect(eyeRight);
	rectEyeRight.x = rectEyeRight.x - (rectEyeRight.width / 3);
	rectEyeRight.y = rectEyeRight.y - (rectEyeRight.height / 2);
	rectEyeRight.width = rectEyeRight.width + ((rectEyeRight.width / 3) * 2);
	rectEyeRight.height = rectEyeRight.height + ((rectEyeRight.height / 2) * 2);
	imgEyeRight = imgFace(rectEyeRight & imgRect);
	//cv::imshow("RIGHT EYE", imgEyeRight);

	//mouth 49 - 68
	std::vector<cv::Point2f> mouth;
	for (int i = 48; i < 68; i++)
		mouth.push_back(points[i]);
	Util_Draw::boundingImg(imgFace, mouth, imgMouth);
	//cv::imshow("MOUTH", imgMouth);

	//moise 28 - 36
	std::vector<cv::Point2f> noise;
	for (int i = 27; i < 36; i++)
		noise.push_back(points[i]);
	noise.push_back(points[39]);
	noise.push_back(points[42]);
	Util_Draw::boundingImg(imgFace, noise, imgNoise);
	//cv::imshow("NOISE", imgNoise);

	int half = points[8].y - points[36].y;
	int y = points[36].y - half;
	int x = points[0].x;
	int width = points[16].x - points[0].x;
	int heigth = points[19].y - y;
	cv::Rect foreheadRect(x, y, width, heigth);
	imgForehead = imgFace(foreheadRect & imgRect);
	//cv::imshow("forehead", imgForehead);

	width = points[36].x - points[0].x;
	y = points[17].y;
	x = points[0].x - width;
	heigth = points[3].y - points[17].y;
	cv::Rect hearLeftRect(x, y, width*1.5, heigth);
	imgHearLeft = imgFace(hearLeftRect & imgRect);
	//cv::imshow("hear left", imgHearLeft);

	y = points[26].y;
	x = points[26].x;
	heigth = points[3].y - points[17].y;
	cv::Rect hearRigthRect(x, y, width*1.5, heigth);
	imgHearRight = imgFace(hearRigthRect & imgRect);
	//cv::imshow("hear rigth", imgHearRight);

	Util_Draw::boundingImg(imgFace, points, imgDropFace);
	//cv::imshow("face", imgDropFace);

	std::vector<cv::Point2f> pointsConvexHull;
	cv::convexHull(points, pointsConvexHull, false);
	cv::Mat_<uchar> mask(imgFace.rows, imgFace.cols, (uchar)0);
	//drawFilledPoly(mask, pointsConvexHull, cv::Scalar(255));
	Util_Draw::drawPoly(mask, pointsConvexHull, cv::Scalar(255), 80);
	//cv::imshow("mask", mask);
	imgFace.copyTo(imgBorderFace, mask);
	//cv::imshow("border face", imgBorderFace);

	cv::waitKey();
}


//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	Util_LandmarksFace::init();

	std::string fileImgs = "C:\\NEW_MORPHING\\files.txt";
	std::string outDirName = "C:\\NEW_MORPHING_CROP";


	std::vector<std::string> files;
	int numFiles = Util_FravAttack::readSamplesFiles(fileImgs, files);

	std::stringstream outDirNoise;
	outDirNoise << outDirName << "\\" << "NOISE";
	Util_Files::createDIR(outDirNoise.str().c_str());
	std::stringstream outDirEyes;
	outDirEyes << outDirName << "\\" << "EYES";
	Util_Files::createDIR(outDirEyes.str().c_str());
	std::stringstream outDirMouth;
	outDirMouth << outDirName << "\\" << "MOUTH";
	Util_Files::createDIR(outDirMouth.str().c_str());
	std::stringstream outDirForehead;
	outDirForehead << outDirName << "\\" << "FOREHEAD";
	Util_Files::createDIR(outDirForehead.str().c_str());
	std::stringstream outDirHears;
	outDirHears << outDirName << "\\" << "HEARS";
	Util_Files::createDIR(outDirHears.str().c_str());
	std::stringstream outDirDropFace;
	outDirDropFace << outDirName << "\\" << "DROP_FACES";
	Util_Files::createDIR(outDirDropFace.str().c_str());
	std::stringstream outDirBorderFace;
	outDirBorderFace << outDirName << "\\" << "BORDER_FACES";
	Util_Files::createDIR(outDirBorderFace.str().c_str());

	char numUser[3];
	for (int i = 0; i < numFiles; i++)
	{
		std::string file = files[i];
		cv::Mat img = cv::imread(file);

		std::vector<cv::Point2f> points;
		Util_LandmarksFace::facePoints(img, points);

		cv::Mat imgEyeLeft;
		cv::Mat imgEyeRight;
		cv::Mat imgMouth;
		cv::Mat imgNoise;
		cv::Mat imgForehead;
		cv::Mat imgHearLeft;
		cv::Mat imgHearRight;
		cv::Mat imgDropFace;
		cv::Mat imgBorderFace;
		imgsFaceZones(img, points,
			imgEyeLeft,
			imgEyeRight,
			imgMouth,
			imgNoise,
			imgForehead,
			imgHearLeft,
			imgHearRight,
			imgDropFace,
			imgBorderFace);

		sprintf(numUser, "%03d", i);
		std::stringstream fileNoise;
		fileNoise << outDirNoise.str().c_str() << "\\" << "NOISE_" << numUser << ".jpg";

		std::stringstream fileEyeLeft;
		fileEyeLeft << outDirEyes.str().c_str() << "\\" << "EYE_LEFT_" << numUser << ".jpg";
		std::stringstream fileEyesRight;
		fileEyesRight << outDirEyes.str().c_str() << "\\" << "EYE_RIGHT_" << numUser << ".jpg";

		std::stringstream fileMouth;
		fileMouth << outDirMouth.str().c_str() << "\\" << "MOUTH_" << numUser << ".jpg";

		std::stringstream fileForehead;
		fileForehead << outDirForehead.str().c_str() << "\\" << "FOREHEAD_" << numUser << ".jpg";

		std::stringstream fileHearLeft;
		fileHearLeft << outDirHears.str().c_str() << "\\" << "HEAR_LEFT_" << numUser << ".jpg";
		std::stringstream fileHearRight;
		fileHearRight << outDirHears.str().c_str() << "\\" << "HEAR_RIGHT_" << numUser << ".jpg";

		std::stringstream fileDropFace;
		fileDropFace << outDirDropFace.str().c_str() << "\\" << "DROP_FACE_" << numUser << ".jpg";

		std::stringstream fileBorderFace;
		fileBorderFace << outDirBorderFace.str().c_str() << "\\" << "BORDER_FACE_" << numUser << ".jpg";

		cv::imwrite(fileNoise.str(), imgNoise);
		cv::imwrite(fileEyeLeft.str(), imgEyeLeft);
		cv::imwrite(fileEyesRight.str(), imgEyeRight);
		cv::imwrite(fileMouth.str(), imgMouth);
		cv::imwrite(fileHearLeft.str(), imgHearLeft);
		cv::imwrite(fileHearRight.str(), imgHearRight);
		cv::imwrite(fileForehead.str(), imgForehead);
		cv::imwrite(fileDropFace.str(), imgDropFace);
		cv::imwrite(fileBorderFace.str(), imgBorderFace);
	}
	delete[] numUser;
}

