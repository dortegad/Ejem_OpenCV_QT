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


//------------------------------------------------------------------------------
void imgsFaceZones(cv::Mat imgFace, std::vector<cv::Point2f> points)
{
	//eye left 37 38 39 40 41 42
	std::vector<cv::Point2f> eyeLeft;
	for (int i = 36; i < 42; i++)
		eyeLeft.push_back(points[i]);
	cv::Mat imgEyeLeft;
	Util_Draw::boundingImg(imgFace, eyeLeft, imgEyeLeft);
	cv::imshow("LEFT EYE", imgEyeLeft);

	//eye right 43 44 45 46 47 48
	std::vector<cv::Point2f> eyeRight;
	for (int i = 42; i < 48; i++)
		eyeRight.push_back(points[i]);
	cv::Mat imgEyeRight;
	Util_Draw::boundingImg(imgFace, eyeRight, imgEyeRight);
	cv::imshow("RIGHT EYE", imgEyeRight);

	//mouth 49 - 68
	std::vector<cv::Point2f> mouth;
	for (int i = 48; i < 68; i++)
		mouth.push_back(points[i]);
	cv::Mat imgMouth;
	Util_Draw::boundingImg(imgFace, mouth, imgMouth);
	cv::imshow("MOUTH", imgMouth);

	//moise 28 - 36
	std::vector<cv::Point2f> noise;
	for (int i = 27; i < 36; i++)
		noise.push_back(points[i]);
	noise.push_back(points[39]);
	noise.push_back(points[42]);
	cv::Mat imgNoise;
	Util_Draw::boundingImg(imgFace, noise, imgNoise);
	cv::imshow("NOISE", imgNoise);


	int half = points[8].y - points[36].y;
	int y = points[36].y - half;
	int x = points[0].x;
	int width = points[16].x - points[0].x;
	int heigth = points[19].y - y;
	cv::Rect foreheadRect(x, y, width, heigth);
	cv::Mat imgForehead = imgFace(foreheadRect);
	cv::imshow("forehead", imgForehead);

	width = points[36].x - points[0].x;
	y = points[17].y;
	x = points[0].x - width;
	heigth = points[3].y - points[17].y;
	cv::Rect hearLeftRect(x, y, width*1.5, heigth);
	cv::Mat imgHearLeft = imgFace(hearLeftRect);
	cv::imshow("hear left", imgHearLeft);

	y = points[26].y;
	x = points[26].x;
	heigth = points[3].y - points[17].y;
	cv::Rect hearRigthRect(x, y, width*1.5, heigth);
	cv::Mat imgHearRigth = imgFace(hearRigthRect);
	cv::imshow("hear rigth", imgHearRigth);
}


//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	//std::string fileImgs = "E:\\MORPH\\SCAN\\SCAN_USUARIOS_GENUINOS_MORPHING_CARNET.txt";
	//std::string fileImgs = "E:\\MORPH\\SCAN\\SCAN_USUARIOS_MORPHING_CARNET.txt";
	//std::string fileImgs = "C:\\Users\\frav\\Desktop\\morphing_completo.txt";
	std::string fileImgs = "C:\\NEW_MORPHING\\USER_000\\files.txt";

	Util_LandmarksFace::init();

	std::vector<std::string> files;
	int numFiles = Util_FravAttack::readSamplesFiles(fileImgs, files);

	//Util_Faces::init();

	for (int i = 0; i < numFiles; i++)
	{
		std::string file = files[i];
		cv::Mat img = cv::imread(file);

		std::vector<cv::Point2f> points;
		Util_LandmarksFace::facePoints(img, points);

		imgsFaceZones(img,points);

		//mitad = 9.x - 37.x
		//x frente = 37.x - mitad
		//y frente = 1.y 
		//ancho = 17.x - 1.x
		//alto = mitad
		
		/*
		int half = points[8].y - points[36].y;
		int y = points[36].y - half;
		int x = points[0].x;
		int width = points[16].x - points[0].x;
		int heigth = points[19].y - y;
		cv::Rect foreheadRect(x, y, width, heigth);
		cv::Mat imgForehead = img(foreheadRect);
		cv::imshow("forehead", imgForehead);

		int width= points[36].x - points[0].x;
		int y = points[17].y;
		int x = points[0].x - width;
		int heigth = points[3].y - points[17].y;
		cv::Rect foreheadRect(x, y, width*1.5, heigth);
		cv::Mat imgForehead = img(foreheadRect);
		cv::imshow("hear", imgForehead);
		
		
		int y = points[26].y;
		int x = points[26].x;
		int heigth = points[3].y - points[17].y;
		cv::Rect foreheadRect(x, y, width*1.5, heigth);
		cv::Mat imgForehead = img(foreheadRect);
		cv::imshow("hear", imgForehead);
		*/


		std::vector<cv::Point2f> pointsConvexHull;
		cv::convexHull(points, pointsConvexHull, false);

		cv::Mat imgToShow = img.clone();
		Util_Draw::drawPoints(imgToShow, points, cv::Scalar(0,0,255));

		Util_Draw::drawPoints(imgToShow, pointsConvexHull, cv::Scalar(255, 255, 255));

		Util_Draw::drawPoly(imgToShow, pointsConvexHull, cv::Scalar(255, 255, 255));

		cv::Mat_<uchar> mask(img.rows,img.cols,(uchar)0); 
		//drawFilledPoly(mask, pointsConvexHull, cv::Scalar(255));
		Util_Draw::drawPoly(mask, pointsConvexHull, cv::Scalar(255), 80);
		//cv::imshow("mask", mask);
		cv::Mat imgDropFace;
		img.copyTo(imgDropFace, mask);
		cv::imshow("drop face", imgDropFace);

		cv::Rect rectFace = cv::boundingRect(points);
		cv::Mat imgDrop = img(rectFace);
		cv::imshow("drop", imgDrop);

		cv::imshow("img", imgToShow);
		cv::waitKey();

	}

	return 0;
}

