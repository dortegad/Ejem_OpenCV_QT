#include "util_draw.h"

//------------------------------------------------------------------------------
void Util_Draw::drawPoints(cv::Mat &img, 
				std::vector<cv::Point> &points,
				cv::Scalar color)
{
	int numPoints = points.size();
	for (int i = 0; i < numPoints; i++)
	{
		cv::Point2f point = points[i];
		cv::Rect rectPoint(point.x - 1, point.y - 1, 3, 3);
		cv::rectangle(img, rectPoint, color);
	}
}

//------------------------------------------------------------------------------
void Util_Draw::drawPoly(cv::Mat &img,
	std::vector<cv::Point> &points,
	cv::Scalar color,
	int width)
{

	std::vector<std::vector<cv::Point> > vectorPoints;
	vectorPoints.push_back(points);
	cv::drawContours(img, vectorPoints, 0, color, width, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
}

//------------------------------------------------------------------------------
void Util_Draw::drawFilledPoly(cv::Mat &img,
	std::vector<cv::Point> &points,
	cv::Scalar color)
{

	std::vector<std::vector<cv::Point> > vectorPoints;
	vectorPoints.push_back(points);
	cv::drawContours(img, vectorPoints, 0, color, CV_FILLED, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
}

//------------------------------------------------------------------------------
void Util_Draw::boundingImg(cv::Mat &img,
	std::vector<cv::Point> &points,
	cv::Mat &subImg)
{
	cv::Rect rect = cv::boundingRect(points);
	subImg = img(rect);
}