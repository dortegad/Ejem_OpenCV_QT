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
#include <util_LBP_CV.h>
#include <util_fravAttack.h>

//------------------------------------------------------------------------------
bool sample(cv::Mat &imgRGB, 
			cv::Mat &imgDepth, 
			cv::Mat_<double> &features)
{
	cv::Rect rectFace;
	if (!Util_Faces::detectFace(imgRGB, rectFace))
		return false;

	imgRGB = imgRGB(rectFace);
	cv::resize(imgRGB, imgRGB, cv::Size(100, 100));

	imgDepth = imgDepth(rectFace);
	cv::resize(imgDepth, imgDepth, cv::Size(100, 100));

	/*
	Util_LBP_Dlib::LBP_RGB(imgRGB, features);

	Util_LBP_Dlib::LBP_Depth(imgDepth, features);

	*/
	Util_LBP_CV::LBP_RGB(imgRGB, features);
	
	/*
	Util_LBP_CV::LBP_Depth(imgDepth, features);
	*/

	return true;
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	std::string fileImgs = argv[1];// "E:\\DB_FINAL\\RS\\VISIBLE\\verifyFiles.txt";
	std::string outDirName = argv[2]; //"E:\\DB_FINAL\\RS\\VISIBLE\\DESCRIPTORS\\LBP_RGB_NEW";

	std::vector<std::string> filesDepth;
	std::vector<std::string> filesRGB;
	std::vector<std::string> filesIR;
	int numFiles = Util_FravAttack::readSamplesFiles(fileImgs, filesDepth, filesRGB, filesIR);

	Util_Faces::init();

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
		if (sample(imgRGB, imgDepth, features))
		{
			std::string user;
			std::string attack;
			std::string frame;
			Util_FravAttack::infoFile(fileRGB, user, attack, frame);
			std::stringstream outFileName;
			outFileName << outDirName << "\\" << user << "_" << attack << "_" << frame << ".des";

			std::cout << outFileName.str() << std::endl;

			cv::FileStorage fileDes(outFileName.str().c_str(), cv::FileStorage::WRITE);
			fileDes << "imgfaceDepth" << features;
			fileDes.release();

			//std::cout << features << std::endl;
		}
	}

	std::getchar();

	return 0;
}

