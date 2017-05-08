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
	cv::Ptr<cv::ml::SVM> svm_rgb;
	cv::Ptr<cv::ml::SVM> svm_depth;

	PXCSenseManager *pp;
	PXCCapture::Device *device;
	void convertPXCImageToOpenCVMat(PXCImage *inImg, cv::Mat *outImg);
	cv::Mat captureAdjustColor(PXCCapture::Device *device, 
								const PXCCapture::Sample *sample);
	cv::Mat captureReal(PXCCapture::Device *device, 
						const PXCCapture::Sample *sample);

	void capture(cv::Mat & frameRGB, cv::Mat &frameDepth);
public:
	CamF200();
	~CamF200();
	int init();
	int isAttack();
	int stop();
};

