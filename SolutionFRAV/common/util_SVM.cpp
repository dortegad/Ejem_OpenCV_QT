#include "util_SVM.h"
#include <iostream>


//------------------------------------------------------------------------------
void Util_SVM::filesDIR(std::vector<std::string> &sampleFileS, 
						int label, 
						cv::Mat &dataMat,
						cv::Mat &labelMat)
{
	int numFiles = sampleFileS.size();
	
	cv::Mat matSample;
	cv::FileStorage fs(sampleFileS[0], cv::FileStorage::READ);
	fs["descriptor"] >> matSample;
	fs.release();
	int sampleSize = matSample.cols;
	matSample.release();
	
	labelMat = cv::Mat(numFiles, 1, CV_32SC1, cv::Scalar(label));
	dataMat = cv::Mat(numFiles, sampleSize, CV_32FC1);
	int pos = 0;
	for (std::vector<std::string>::iterator it = sampleFileS.begin(); it != sampleFileS.end(); it++)
	{
		std::string fileSample = *it;
		cv::FileStorage fs2(fileSample, cv::FileStorage::READ);
		fs2["descriptor"] >> matSample;
		fs2.release();

		matSample.copyTo(dataMat.row(pos));
		matSample.release();

		//std::cout << matSample << std::endl;
		pos++;
	}
}


//------------------------------------------------------------------------------
void Util_SVM::balance(std::vector<std::string> &filesA, std::vector<std::string> &filesB)
{
	int numFilesA = filesA.size();
	int numFilesB = filesB.size();

	if (numFilesA > numFilesB)
	{
		std::vector<std::string> newFilesA(filesA.begin(), filesA.begin() + numFilesB);
		filesA = newFilesA;
	}
	else
	{
		std::vector<std::string> newFilesB(filesB.begin(), filesB.begin() + numFilesA);
		filesB = newFilesB;
	}
}