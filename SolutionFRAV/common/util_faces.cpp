#include "util_faces.h"

cv::CascadeClassifier Util_Faces::face_cascade;

//------------------------------------------------------------------------------
bool Util_Faces::init()
{
	std::string face_cascade_name = "haarcascade_frontalface_alt.xml";
	
	if (!Util_Faces::face_cascade.load(face_cascade_name))
		return false;
	else
		return true;
}

//------------------------------------------------------------------------------
bool Util_Faces::detectFace(const cv::Mat &img, cv::Rect &rect)
{
	std::vector<cv::Rect> faces;
	cv::Mat imgGray;

	float ratio = 5.0;
	float invertRatio = 1 / ratio;
	int newCols = img.cols*invertRatio;
	int newRows = img.rows*invertRatio;
	cv::resize(img, imgGray, cv::Size(newCols, newRows));
	cv::cvtColor(imgGray, imgGray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(imgGray, imgGray);

	//-- Detect faces
	Util_Faces::face_cascade.detectMultiScale(imgGray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE/*, cv::Size(newCols/5, newRows/5)*/);

	if (faces.size() == 0)
		return false;

	cv::Rect ajustRect = faces[0];	
	rect = cv::Rect(ajustRect.x*ratio, ajustRect.y*ratio, ajustRect.width*ratio, ajustRect.height*ratio);
	return true;
}