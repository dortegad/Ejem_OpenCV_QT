#include "util_LBP_CV.h"


#include "opencv2\face.hpp"

#include "util_depth.h"
#include "util_eyes.h"

#include <algorithm>    // std::max

//------------------------------------------------------------------------------
int Util_LBP_CV::LBP_RGB(cv::Mat &img, cv::Mat &features)
{	
	cv::cvtColor(img, img, CV_BGR2GRAY);
	std::vector<cv::Mat> images;
	std::vector<int> labels;
	images.push_back(img); labels.push_back(0);

	
	cv::Ptr<cv::face::LBPHFaceRecognizer> model2 = cv::face::createLBPHFaceRecognizer();
	model2->train(images, labels);
	std::vector<cv::Mat> histogramsS = model2->getHistograms();
	features =  histogramsS[0];

	return histogramsS[0].cols;
}

//------------------------------------------------------------------------------
int Util_LBP_CV::LBP_Depth(cv::Mat &imgDepth, cv::Mat &features)
{
	cv::Mat_<double> imgDepthNorm;
	Util_Depth::normalize(imgDepth, imgDepthNorm);

	cv::Mat depthGrayNormalicedFace;
	imgDepthNorm.convertTo(depthGrayNormalicedFace, CV_8UC1);
	cv::equalizeHist(depthGrayNormalicedFace, depthGrayNormalicedFace);

	std::vector<cv::Mat> images;
	std::vector<int> labels;
	images.push_back(depthGrayNormalicedFace); labels.push_back(0);

	cv::Ptr<cv::face::LBPHFaceRecognizer> model2 = cv::face::createLBPHFaceRecognizer();
	model2->train(images, labels);
	std::vector<cv::Mat> histogramsS = model2->getHistograms();
	features = histogramsS[0];

	return histogramsS[0].cols;
}

//------------------------------------------------------------------------------
int Util_LBP_CV::LBP_RGB_Eyes(cv::Mat &imgRGB, cv::Mat &features)
{
	cv::Rect eye1;
	cv::Rect eye2;
	bool detect = Util_Eyes::detectEyes(imgRGB, eye1, eye2);
	if (detect)
	{
		cv::Size sizeEyes(50, 50);
		cv::Mat eyeImg1 = imgRGB(eye1);
		cv::Mat eyeImg2 = imgRGB(eye2);
		//cv::imshow("eye 1", eyeImg1);
		//cv::imshow("eye 2", eyeImg2);
		cv::resize(eyeImg1, eyeImg1, sizeEyes);
		cv::resize(eyeImg2, eyeImg2, sizeEyes);
		cv::Mat_<double> featuresEye1;
		cv::Mat_<double> featuresEye2;
		Util_LBP_CV::LBP_RGB(eyeImg1, featuresEye1);
		Util_LBP_CV::LBP_RGB(eyeImg2, featuresEye2);

		cv::Mat fea;
		cv::hconcat(featuresEye1, featuresEye2, fea);
		features = fea;

		//cv::rectangle(imgRGB, eye1, cv::Scalar(0, 0, 255), 2);
		//cv::rectangle(imgRGB, eye2, cv::Scalar(0, 0, 255), 2);
		//cv::imshow("eye 1", eyeImg1);
		//cv::imshow("eye 2", eyeImg2);
	}
	//cv::imshow("eyes", imgRGB);
	//cv::waitKey();

	return features.cols;
}

//------------------------------------------------------------------------------
int Util_LBP_CV::LBP_RGB_Hair(cv::Mat &imgRGB, cv::Mat &features)
{
	cv::Rect eye1;
	cv::Rect eye2;
	bool detect = Util_Eyes::detectEyes(imgRGB, eye1, eye2);
	if (detect)
	{
		int topEyes = std::min(eye1.y, eye2.y);
		int leftEyes = std::min(eye1.x, eye2.x);
		int rightEyes = std::max(eye1.x + eye1.width, eye2.x + eye2.width);
		cv::Rect rectHair(cv::Point2f(leftEyes, 0), cv::Point2f(rightEyes, topEyes));
		cv::Size sizeHair(100, 50);
		cv::Mat hairImg = imgRGB(rectHair);
		//cv::imshow("hair", hairImg);
		cv::resize(hairImg, hairImg, sizeHair);
		Util_LBP_CV::LBP_RGB(hairImg, features);

		//cv::rectangle(imgRGB, rectHair, cv::Scalar(0, 0, 255), 2);
		//cv::imshow("hair", hairImg);
	}
	//cv::imshow("seg hair", imgRGB);
	//cv::waitKey();

	return features.cols;
}

//------------------------------------------------------------------------------
int Util_LBP_CV::LBP_RGB_Eyes_Hair(cv::Mat &imgRGB, cv::Mat &features)
{
	cv::Rect eye1;
	cv::Rect eye2;
	bool detect = Util_Eyes::detectEyes(imgRGB, eye1, eye2);
	if (detect)
	{
		cv::Size sizeEyes(50, 50);
		cv::Mat eyeImg1 = imgRGB(eye1);
		cv::Mat eyeImg2 = imgRGB(eye2);
		//cv::imshow("eye 1", eyeImg1);
		//cv::imshow("eye 2", eyeImg2);
		cv::resize(eyeImg1, eyeImg1, sizeEyes);
		cv::resize(eyeImg2, eyeImg2, sizeEyes);
		cv::Mat_<double> featuresEye1;
		cv::Mat_<double> featuresEye2;
		Util_LBP_CV::LBP_RGB(eyeImg1, featuresEye1);
		Util_LBP_CV::LBP_RGB(eyeImg2, featuresEye2);

		cv::Mat eyesFeatures;
		cv::hconcat(featuresEye1, featuresEye2, eyesFeatures);

		int topEyes = std::min(eye1.y, eye2.y);
		int leftEyes = std::min(eye1.x, eye2.x);
		int rightEyes = std::max(eye1.x + eye1.width, eye2.x + eye2.width);
		cv::Rect rectHair(cv::Point2f(leftEyes, 0), cv::Point2f(rightEyes, topEyes));
		cv::Size sizeHair(100, 50);
		cv::Mat hairImg = imgRGB(rectHair);
		cv::resize(hairImg, hairImg, sizeHair);
		cv::Mat_<double> hairFeatures;
		Util_LBP_CV::LBP_RGB(hairImg, hairFeatures);

		cv::Mat allFeatures;
		cv::hconcat(eyesFeatures, hairFeatures, allFeatures);
		features = allFeatures;
	}

	return features.cols;
}