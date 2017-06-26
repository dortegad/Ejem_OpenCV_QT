#include "util_eyes.h"

cv::CascadeClassifier Util_Eyes::eyes_cascade;

//------------------------------------------------------------------------------
bool Util_Eyes::init()
{
	std::string eyes_cascade_name = "haarcascade_eye.xml";

	if (!Util_Eyes::eyes_cascade.load(eyes_cascade_name))
		return false;
	else
		return true;
}

//------------------------------------------------------------------------------
bool Util_Eyes::detectEyes(const cv::Mat &img, cv::Rect &rectEye1, cv::Rect &rectEye2)
{
	cv::Mat imgGray;

	//float invertRatioCols = img / ratio;
	float newCols = 300.0;//img.cols*invertRatio;
	float newRows = 300.0;// img.rows*invertRatio;
	float invertRatioCols = img.cols / newCols;
	float invertRatioRows = img.rows / newRows;
	cv::resize(img, imgGray, cv::Size(newCols, newRows));
	cv::cvtColor(imgGray, imgGray, cv::COLOR_BGR2GRAY);
	//cv::equalizeHist(imgGray, imgGray);
	//cv::imshow("prueba", imgGray);
	//cv::waitKey();

	//-- Detect faces
	std::vector<cv::Rect> eyes;
	Util_Eyes::eyes_cascade.detectMultiScale(imgGray, eyes , 1.3, 4, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(50, 50));

	if (eyes.size() < 2)
		return false;

	cv::Rect eye1 = eyes[0];	
	cv::Rect eye2 = eyes[1];
	rectEye1 = cv::Rect(eye1.x*invertRatioCols, eye1.y*invertRatioRows, eye1.width*invertRatioCols, eye1.height*invertRatioRows);
	rectEye2 = cv::Rect(eye2.x*invertRatioCols, eye2.y*invertRatioRows, eye2.width*invertRatioCols, eye2.height*invertRatioRows);
	return true;
}