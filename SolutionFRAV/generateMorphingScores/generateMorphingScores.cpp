// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\ml\ml.hpp"


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
#include <iostream>


#include <util_faces.h>
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
	else if(samplesType == "LBP_DEPTH")
	{
		Util_LBP_CV::LBP_Depth(imgDepth, features);
	}
	else
	{
		return false;
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

//------------------------------------------------------------------------------
/*int main_paraFicheroDeRealSense(int argc, char *argv[])
{
	std::string fileImgs = argv[1];// "E:\\DB_FINAL\\RS\\VISIBLE\\verifyFiles.txt";
	std::string outDirName = argv[2]; //"E:\\DB_FINAL\\RS\\VISIBLE\\DESCRIPTORS\\LBP_RGB_NEW";
	std::string samplesType = argv[3];

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
		if (sample(imgRGB, imgDepth, features, samplesType))
		{
			std::string user;
			std::string attack;
			std::string frame;
			Util_FravAttack::infoFile(fileRGB, user, attack, frame);

			writeSample(features, outDirName, user, attack, frame);
		}
	}

	std::getchar();

	return 0;
}
*/

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	std::string fileImgs = /*argv[1];*/ "D:\\MORPH\\files_for_verify_morph_scores.txt";
	std::string resultFile = /*argv[1];*/ "D:\\MORPH\\files_for_verify_morph_scores_COGNITED_AND_SVM_LBP_MORPHING.txt";
	//std::string fileImgs = "D:\\MORPH\\RESULTADOS\\RS_faces_viola_files_scores_000_016_SOLO_USARIOS_NO_ATAQUES.txt";
	//std::string resultFile = "D:\\MORPH\\RESULTADOS\\RS_faces_viola_files_scores_000_016_SOLO_USARIOS_NO_ATAQUES_COGNITEG_SVM_LBP_MORPHING.txt";
	std::string svmFile = /*argv[2];*/ ".\\SVM_LBP_RGB\\svm_attack_06.svm";

	std::vector<std::string> files;
	int numFiles = Util_FravAttack::readSamplesFiles(fileImgs, files);

	Util_Faces::init();
	std::ofstream outFile(resultFile.c_str(), std::ofstream::out);

	for (int i = 0; i < numFiles; i++)
	{
		std::string file = files[i];
		std::stringstream line;
		line << file.c_str();
		getline(line, file, ',');
		getline(line, file, ',');
		getline(line, file, ',');
		getline(line, file, ',');

		std::cout << file << std::endl;
		cv::Mat img = cv::imread(file);

		cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::load(svmFile);
		cv::Mat_<double> features;
		if (sample(img, cv::Mat(), features, "LBP_RGB",true))
		{
			float result = svm->predict(features, cv::noArray(), cv::ml::StatModel::RAW_OUTPUT);
			float confidence = 1.0 / (1.0 + exp(-result));

			std::cout << file << "  " << confidence << std::endl << std::endl;

			outFile << line.str().c_str() << "," << confidence << std::endl;

			//writeSample(features, outDirName, user, "00", "00");
		}
	}
	outFile.close();
	std::getchar();

	return 0;
}

