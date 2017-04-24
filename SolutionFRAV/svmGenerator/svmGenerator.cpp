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
#include <util_files.h>
#include <util_fravAttack.h>


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
	std::vector <std::string> files;
	Util_Files::filesDIR("D:\\DESCRIPTORS_PRUEBA\\" , files);

	std::vector<std::string> filesAttack_00;
	int numFilesAttack_00 = Util_FravAttack::filesAttack(files, "00", filesAttack_00);

	std::vector<std::string> filesAttack_04;
	int numFilesAttack_04 = Util_FravAttack::filesAttack(files, "04", filesAttack_04);

	cv::Mat_<double> matSampleS;
	for (std::vector<std::string>::iterator it = filesAttack_00.begin(); it != filesAttack_00.end(); it++)
	{
		cv::Mat matSample;
		std::string fileSample = *it;
		cv::FileStorage fs2(fileSample, cv::FileStorage::READ);
		fs2["descriptor"] >> matSample;
		fs2.release();

		if (matSampleS.rows == 0)
			matSampleS = matSample;
		else
			cv::vconcat(matSampleS, matSample, matSampleS);

		//std::cout << matSample << std::endl;
	}

	std::getchar();

	


	/*
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

		cv::Mat imgDepth;
		std::string fileDepth = filesDepth[i];
		cv::FileStorage fs2(fileDepth, cv::FileStorage::READ);
		fs2["imgfaceDepth"] >> imgDepth;
		fs2.release();
		imgDepth = imgDepth(rectFace);
		cv::resize(imgDepth, imgDepth, cv::Size(100, 100));

		//cv::Mat_<double> featuresLBP_RGB;
		//Util_LBP_Dlib::LBP_RGB(img, featuresLBP_RGB);
		
		//cv::Mat_<double> featuresLBP_Depth;
		//Util_LBP_Dlib::LBP_Depth(imgDepth, featuresLBP_Depth);

		//cv::Mat_<double> featuresLBP_RGB;
		//Util_LBP_CV::LBP_RGB(img, featuresLBP_RGB);

		//cv::Mat_<double> featuresLBP_Depth;
		//Util_LBP_CV::LBP_Depth(imgDepth, featuresLBP_Depth);
	}
	*/

	return 0;
}

