// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\video\video.hpp"


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
#include <vector>


#include <util_faces.h>
#include <util_eyes.h>
#include <util_depth.h>
#include <util_LBP_Dlib.h>
#include <util_LBP_CV.h>
#include <util_fravAttack.h>

//------------------------------------------------------------------------------
bool sample(cv::Mat &imgRGB,
	cv::Mat &imgDepth,
	cv::Mat_<double> &features,
	const std::string &samplesType,
	bool segFace = true)
{
	if (segFace)
	{
		cv::Rect rectFace;
		if (!Util_Faces::detectFace(imgRGB, rectFace))
			return false;

		imgRGB = imgRGB(rectFace);
		if (!imgDepth.empty())
			imgDepth = imgDepth(rectFace);
	}

	if (samplesType == "LBP_RGB_HAIR")
	{
		return (Util_LBP_CV::LBP_RGB_Hair(imgRGB, features) != 0);
	}
	else if (samplesType == "LBP_RGB_EYES")
	{
		return (Util_LBP_CV::LBP_RGB_Eyes(imgRGB,features) != 0);
	}
	else
	{
		cv::resize(imgRGB, imgRGB, cv::Size(100, 100));
		if (!imgDepth.empty())
			cv::resize(imgDepth, imgDepth, cv::Size(100, 100));

		if (samplesType == "DLIB_LBP_RGB")
		{
			Util_LBP_Dlib::LBP_RGB(imgRGB, features);
		}
		else if (samplesType == "DLIB_LBP_DEPTH")
		{
			Util_LBP_Dlib::LBP_Depth(imgDepth, features);
		}
		else if (samplesType == "LBP_RGB")
		{
			Util_LBP_CV::LBP_RGB(imgRGB, features);
		}
		else if (samplesType == "LBP_DEPTH")
		{
			Util_LBP_CV::LBP_Depth(imgDepth, features);
		}
		else
		{
			return false;
		}
	}

	return true;
}


//------------------------------------------------------------------------------
void writeSample(cv::Mat_<double> &features, 
				const std::string &outDirName,
				const std::string &user,
				const std::string &attack,
				const std::string &frame)
{
	std::stringstream outFileName;
	outFileName << outDirName << "\\" << user << "_" << attack << "_" << frame << ".des";

	std::cout << outFileName.str() << std::endl;

	cv::FileStorage fileDes(outFileName.str().c_str(), cv::FileStorage::WRITE);
	fileDes << "descriptor" << features;
	fileDes.release();

	//std::cout << features << std::endl;
}

/*PARA COPIAR UNOS FICHERO A UN DIRECTORIO
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
std::string fileImgs = "E:\\MORPH\\files_morph_one_per_user.txt";
std::string outDirName = "E:\\MORPH\\FRAV_MORPH_ONE_PER_USER";

std::vector<std::string> files;
int numFiles = Util_FravAttack::readSamplesFiles(fileImgs, files);

for (int i = 0; i < numFiles; i++)
{
	std::string fileRGB = files[i];
	cv::Mat imgRGB = cv::imread(fileRGB);

	std::string fileName = fileRGB.substr(fileRGB.length() - 16, fileRGB.length());
	std::stringstream fileOut;
	fileOut << outDirName << "\\" << fileName;
	std::cout << fileOut.str().c_str() << std::endl;

	cv::imwrite(fileOut.str().c_str(), imgRGB);
}

std::getchar();
}
*/

/* PARA VER UN VIDEO
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	cv::VideoCapture cap;
	cap.open("C:\\r.avi");

	while (true)
	{
		cv::Mat img;
		cap >> img;
		cv::imshow("video", img);
		cv::waitKey();
	}
}
*/

/*
//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	std::string fileImgs = argv[1];// "E:\\DB_FINAL\\RS\\VISIBLE\\verifyFiles.txt";
	std::string outDirName = argv[2]; //"E:\\DB_FINAL\\RS\\VISIBLE\\DESCRIPTORS\\LBP_RGB_NEW";
	std::string samplesType = argv[3];

	std::vector<std::string> filesDepth;
	std::vector<std::string> filesRGB;
	std::vector<std::string> filesIR;
	int numFiles = Util_FravAttack::readSamplesFiles(fileImgs, filesDepth, filesRGB, filesIR);

	Util_Faces::init();
	Util_Eyes::init();

	for (int i = 0; i < numFiles; i++)
	{
		std::string fileRGB = filesRGB[i];

		cv::Mat imgRGB = cv::imread(fileRGB);

		cv::Mat imgDepth;
		std::string fileDepth = filesDepth[i];
		cv::FileStorage fs2(fileDepth, cv::FileStorage::READ);
		fs2["imgfaceDepth"] >> imgDepth;
		fs2.release();

		cv::Mat_<double> features;
		if (sample(imgRGB, imgDepth, features, samplesType))
		{
			std::string user;
			std::string attack;
			std::string frame;
			Util_FravAttack::infoFile(fileRGB, user, attack, frame);

			//writeSample(features, outDirName, user, attack, frame);
		}
	}

	//std::getchar();

	return 0;
}
*/



//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	std::string fileImgs = argv[1];// "E:\\DB_FINAL\\RS\\VISIBLE\\verifyFiles.txt";
	std::string outDirName = argv[2]; //"E:\\DB_FINAL\\RS\\VISIBLE\\DESCRIPTORS\\LBP_RGB_NEW";

	std::vector<std::string> files;
	int numFiles = Util_FravAttack::readSamplesFiles(fileImgs, files);

	Util_Faces::init();
	Util_Eyes::init();

	for (int i = 0; i < numFiles; i++)
	{
		std::string file = files[i];
		cv::Mat img = cv::imread(file);

		cv::Mat_<double> features;
		if (sample(img, cv::Mat(), features, "LBP_RGB_HAIR",true))
		{
			//std::string user;
			//Util_FravAttack::infoFileUser(file, user);

			//writeSample(features, outDirName, user, "00", "00");
		}
	}

	std::getchar();

	return 0;
}

