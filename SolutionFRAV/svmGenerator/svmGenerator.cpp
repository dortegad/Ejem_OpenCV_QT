// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\ml\ml.hpp"

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
#include <util_SVM.h>

//------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	std::vector <std::string> files;
	Util_Files::filesDIR("D:\\DESCRIPTORS_PRUEBA\\" , files);
	
	std::string svmFile = "D:\\DESCRIPTORS_PRUEBA\\svm.svm";

	std::vector<std::string> filesAttack_00;
	int numFilesAttack_00 = Util_FravAttack::filesAttack(files, "00", filesAttack_00);

	std::vector<std::string> filesAttack_04;
	int numFilesAttack_04 = Util_FravAttack::filesAttack(files, "04", filesAttack_04);

	Util_SVM::balance(filesAttack_00, filesAttack_04);

	cv::Mat data_00;
	cv::Mat labelS_00;
	Util_SVM::filesDIR(filesAttack_00, 0, data_00, labelS_00);

	cv::Mat data_04;
	cv::Mat labelS_04;
	Util_SVM::filesDIR(filesAttack_04, 1, data_04, labelS_04);

	cv::Mat labelsMat;
	cv::Mat dataMat;
	cv::vconcat(labelS_00, labelS_04, labelsMat);
	cv::vconcat(data_00, data_04, dataMat);

	cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setKernel(cv::ml::SVM::LINEAR);
	svm->setGamma(3);

	cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(dataMat, cv::ml::ROW_SAMPLE, labelsMat);

	svm->train(tData);

	svm->save(svmFile);

	svm.release();

	std::getchar();


	return 0;
}

