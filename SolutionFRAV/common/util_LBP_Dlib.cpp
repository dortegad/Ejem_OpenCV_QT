#include "util_LBP_Dlib.h"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

#include "util_depth.h"

//------------------------------------------------------------------------------
int Util_LBP_Dlib::LBP_RGB(cv::Mat &img, cv::Mat &features)
{
	dlib::cv_image<dlib::rgb_pixel> faceRGB(img);
	std::vector<double> featuresRGBLBP;
	dlib::extract_uniform_lbp_descriptors<dlib::cv_image<dlib::rgb_pixel>, double>(faceRGB, featuresRGBLBP);

	cv::Mat matFeaturesRGB(1, featuresRGBLBP.size(), CV_64F);
	memcpy(matFeaturesRGB.data, featuresRGBLBP.data(), featuresRGBLBP.size()*sizeof(double));
	//std::cout << matFeaturesRGB << std::endl;
	features = matFeaturesRGB;

	return featuresRGBLBP.size();
}

//------------------------------------------------------------------------------
int Util_LBP_Dlib::LBP_Depth(cv::Mat &imgDepth, cv::Mat &features)
{
	cv::Mat_<double> imgDepthNorm;
	Util_Depth::normalize(imgDepth, imgDepthNorm);

	dlib::array2d<double> depthFace;
	dlib::assign_image(depthFace, dlib::cv_image<double>(imgDepthNorm));
	std::vector<double> featuresDepthLBP;
	dlib::extract_uniform_lbp_descriptors<dlib::array2d<double>, double>(depthFace, featuresDepthLBP);

	cv::Mat matFeatures(1, featuresDepthLBP.size(), CV_64F);
	memcpy(matFeatures.data, featuresDepthLBP.data(), featuresDepthLBP.size()*sizeof(double));
	//std::cout << matFeatures << std::endl;
	features = matFeatures;

	return featuresDepthLBP.size();
}