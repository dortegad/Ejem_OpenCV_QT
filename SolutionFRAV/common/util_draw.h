#include "opencv2\objdetect.hpp"

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


class Util_Draw
{
public: static void drawPoints(cv::Mat &img, 
				std::vector<cv::Point2f> &points,
				cv::Scalar color);
public: static void drawPoly(cv::Mat &img,
	std::vector<cv::Point2f> &points,
	cv::Scalar color,
	int width = 1);
public: static void drawFilledPoly(cv::Mat &img,
	std::vector<cv::Point2f> &points,
	cv::Scalar color);
public: static void boundingImg(cv::Mat &img,
	std::vector<cv::Point2f> &points,
	cv::Mat &subImg);
};

