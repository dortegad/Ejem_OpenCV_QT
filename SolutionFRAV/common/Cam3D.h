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

enum ImgType { RGB, DEPTH, IR };
enum ImgAdjustType { REAL, ADJUST_RGB, ADJUST_DEPTH };

class Cam3D
{
private:
	PXCSenseManager *pp;
	void convertPXCImageToOpenCVMat(PXCImage *inImg, cv::Mat *outImg);
	void createMapDepth(PXCProjection *projection,
						PXCImage *depth,
						std::vector<PXCPoint3DF32> &mapDepth);
	cv::Mat captureAdjustColor(PXCCapture::Device *device, 
								const PXCCapture::Sample *sample, 
								ImgType imgType = RGB);
	cv::Mat captureReal(PXCCapture::Device *device, 
						const PXCCapture::Sample *sample, 
						ImgType imgType = RGB);
	cv::Mat captureAdjustDepth(PXCCapture::Device *device, 
							const PXCCapture::Sample *sample,
							ImgType imgType = RGB);

public:
	Cam3D();
	~Cam3D();
	int init();
	int isAttack();
	int stop();

	cv::Mat capture(ImgType imgType = RGB,
			ImgAdjustType imgAdjustType = ADJUST_RGB);
};

