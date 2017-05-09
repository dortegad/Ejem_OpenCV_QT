#pragma once

//REALSENSE
#include "pxcsensemanager.h"
#include "pxcmetadata.h"
#include "util_cmdline.h"
#include "util_render.h"

//OPENCV
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\ml\ml.hpp"

class CamF200
{
private:
	cv::Ptr<cv::ml::SVM> svm_rgb_attack_01;
	cv::Ptr<cv::ml::SVM> svm_rgb_attack_02;
	cv::Ptr<cv::ml::SVM> svm_rgb_attack_03;
	cv::Ptr<cv::ml::SVM> svm_rgb_attack_04;
	cv::Ptr<cv::ml::SVM> svm_rgb_attack_05;

	cv::Ptr<cv::ml::SVM> svm_depth_attack_01;
	cv::Ptr<cv::ml::SVM> svm_depth_attack_02;
	cv::Ptr<cv::ml::SVM> svm_depth_attack_03;
	cv::Ptr<cv::ml::SVM> svm_depth_attack_04;
	cv::Ptr<cv::ml::SVM> svm_depth_attack_05;

	PXCSenseManager *pp;
	PXCCapture::Device *device;

	void convertPXCImageToOpenCVMat(PXCImage *inImg, cv::Mat *outImg);
	void capture(cv::Mat & frameRGB, cv::Mat &frameDepth);
	float evalue(cv::Ptr<cv::ml::SVM> svm, cv::Mat & features, float umbral, const std::string &msg);
public:
	CamF200();
	~CamF200();
	int init();
	int isAttack();
	int stop();
};

