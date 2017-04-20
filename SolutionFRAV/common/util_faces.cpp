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

	cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(imgGray, imgGray);

	//-- Detect faces
	Util_Faces::face_cascade.detectMultiScale(imgGray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

	if (faces.size() > 0)
	{
		rect = faces[0];
		return true;
	}
	else
		return false;
}