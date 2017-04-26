#include "util_SVM.h"

#include <iostream>
#include <fstream>

#include "opencv2\ml\ml.hpp"

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
int Util_SVM::balance(std::vector<std::string> &filesA, std::vector<std::string> &filesB)
{
	int numFilesA = filesA.size();
	int numFilesB = filesB.size();

	if (numFilesA > numFilesB)
	{
		std::vector<std::string> newFilesA(filesA.begin(), filesA.begin() + numFilesB);
		filesA = newFilesA;
		return newFilesA.size();
	}
	else
	{
		std::vector<std::string> newFilesB(filesB.begin(), filesB.begin() + numFilesA);
		filesB = newFilesB;
		return newFilesB.size();
	}
}

//------------------------------------------------------------------------------
bool Util_SVM::groupTestTrain(const std::vector<std::string> &filesA, 
							 const std::vector<std::string> &filesB,
							 std::vector<std::string> &filesA_train,
							 std::vector<std::string> &filesA_test, 
							 std::vector<std::string> &filesB_train, 
							 std::vector<std::string> &filesB_test)
{
	std::vector<std::string> filesA_temp = filesA;
	std::vector<std::string> filesB_temp = filesB;

	int numSampleFiles = Util_SVM::balance(filesA_temp, filesB_temp);

	int numSamplesFilesForTrain = numSampleFiles / 3;
	if (numSamplesFilesForTrain == 0)
		return false;

	filesA_train = std::vector<std::string>(filesA_temp.begin(), filesA_temp.begin() + numSamplesFilesForTrain);
	filesA_test = std::vector<std::string>(filesA_temp.begin() + numSamplesFilesForTrain, filesA_temp.end());

	filesB_train = std::vector<std::string>(filesB_temp.begin(), filesB_temp.begin() + numSamplesFilesForTrain);
	filesB_test = std::vector<std::string>(filesB_temp.begin() + numSamplesFilesForTrain, filesB_temp.end());

	return true;
}

//------------------------------------------------------------------------------
void Util_SVM::train(std::vector<std::string> &files_classA_train,
					std::vector<std::string> &files_classB_train,
					const std::string &svmFile)
{
	cv::Mat data_A;
	cv::Mat labelS_A;
	Util_SVM::filesDIR(files_classA_train, 0, data_A, labelS_A);

	cv::Mat data_B;
	cv::Mat labelS_B;
	Util_SVM::filesDIR(files_classB_train, 1, data_B, labelS_B);

	cv::Mat labelsMat;
	cv::Mat dataMat;
	cv::vconcat(labelS_A, labelS_B, labelsMat);
	cv::vconcat(data_A, data_B, dataMat);

	cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setKernel(cv::ml::SVM::LINEAR);
	svm->setGamma(3);

	cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(dataMat, cv::ml::ROW_SAMPLE, labelsMat);

	svm->train(tData);

	svm->save(svmFile);

	svm.release();
}

//------------------------------------------------------------------------------
void Util_SVM::test(std::vector<std::string> &files_classA_test,
						std::vector<std::string> &files_classB_test,
						const std::string &svmFile,
						const std::string &resultFile)
{
	cv::Mat data_A;
	cv::Mat labelS_A;
	Util_SVM::filesDIR(files_classA_test, 0, data_A, labelS_A);

	cv::Mat data_B;
	cv::Mat labelS_B;
	Util_SVM::filesDIR(files_classB_test, 1, data_B, labelS_B);

	cv::Mat labelsMat;
	cv::Mat dataMat;
	cv::vconcat(labelS_A, labelS_B, labelsMat);
	cv::vconcat(data_A, data_B, dataMat);

	cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::load(svmFile);

	std::ofstream file(resultFile.c_str(), std::ofstream::out);
	int numSamples = dataMat.rows;
	for (int i = 0; i < numSamples; i++)
	{
		cv::Mat sample = dataMat.row(i);

		float result = svm->predict(sample, cv::noArray(), cv::ml::StatModel::RAW_OUTPUT);

		int preditClass = svm->predict(sample, cv::noArray());

		float confidence = 1.0 / (1.0 + exp(-result));

		std::cout << labelsMat.at<__int32>(i, 0) << " - " << result << " - " << confidence << " - " << preditClass << std::endl;

		file << labelsMat.at<__int32>(i, 0) << ";" << result << ";" << confidence << ";" << preditClass << std::endl;
	}
	dataMat.release();
	labelsMat.release();

	file.close();
}