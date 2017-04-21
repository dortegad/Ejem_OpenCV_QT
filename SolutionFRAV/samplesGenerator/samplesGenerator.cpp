// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

/*
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

#define DLIB_JPEG_SUPPORT
*/


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <util_faces.h>
#include <util_depth.h>
#include <util_LBP_Dlib.h>


//------------------------------------------------------------------------------
void readFiles(const std::string &fileImgs,
				std::vector<std::string> &filesDepth,
				std::vector<std::string> &filesRGB, 
				std::vector<std::string> &filesIR)
{
	std::ifstream imgsFile;
	imgsFile.open(fileImgs.c_str());

	filesDepth.clear();
	filesRGB.clear();
	filesIR.clear();

	while (imgsFile.good())
	{
		std::string sSamples;
		if (getline(imgsFile, sSamples))
		{
			std::stringstream line;
			line << sSamples.c_str();

			std::string fileDepth;
			getline(line, fileDepth, ';');
			std::string fileRGB;
			getline(line, fileRGB, ';');
			std::string fileIR;
			getline(line, fileIR, ';');

			std::cout << fileDepth.c_str() << std::endl;
			std::cout << fileRGB.c_str() << std::endl;
			std::cout << fileIR.c_str() << std::endl;
			std::cout << std::endl;

			filesDepth.push_back(fileDepth);
			filesRGB.push_back(fileRGB);
			filesIR.push_back(fileIR);
		}
	}

	imgsFile.close();
}

//------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	std::string fileImgs = "E:\\DB_FINAL\\RS\\VISIBLE\\verifyFiles.txt";
	std::vector<std::string> filesDepth;
	std::vector<std::string> filesRGB;
	std::vector<std::string> filesIR;

	readFiles(fileImgs, filesDepth, filesRGB, filesIR);

	Util_Faces::init();

	int numFiles = filesDepth.size();
	for (int i = 0; i < numFiles; i++)
	{
		std::string fileRGB = filesRGB[i];
			
		cv::Mat img = cv::imread(fileRGB);

		cv::Rect rectFace;
		if (!Util_Faces::detectFace(img, rectFace))
			continue;

		img = img(rectFace);
		cv::resize(img, img, cv::Size(100, 100));
		cv::imshow("imagen", img);

		cv::Mat imgDepth;
		std::string fileDepth = filesDepth[i];
		cv::FileStorage fs2(fileDepth, cv::FileStorage::READ);
		fs2["imgfaceDepth"] >> imgDepth;
		fs2.release();

		imgDepth = imgDepth(rectFace);
		cv::resize(imgDepth, imgDepth, cv::Size(100, 100));

		cv::Mat_<double> imgDepthNorm;
		Util_Depth::normalize(imgDepth, imgDepthNorm);

		cv::Mat visulaNormalicedFace;
		imgDepthNorm.convertTo(visulaNormalicedFace, CV_8UC1);
		cv::equalizeHist(visulaNormalicedFace, visulaNormalicedFace);
		cv::imshow("imagenDepth", visulaNormalicedFace);
		
		cv::Mat_<double> featuresLBP_RGB;
		Util_LBP_Dlib::LBP_RGB(img, featuresLBP_RGB);

		cv::Mat_<double> featuresLBP_Depth;
		Util_LBP_Dlib::LBP_Depth(imgDepthNorm, featuresLBP_Depth);
	}

	return 0;
}

