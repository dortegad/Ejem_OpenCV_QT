#include "opencv2\objdetect.hpp"

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

class Util_Morphing
{
private: static void calculateDelaunayTriangles(cv::Rect rect, 
	std::vector<cv::Point2f> &points, 
	std::vector< std::vector<int> > &delaunayTri);
private: static void constrainPoint(cv::Point2f &p, cv::Size sz);
private: static void similarityTransform(std::vector<cv::Point2f>& inPoints,
	std::vector<cv::Point2f>& outPoints,
	cv::Mat &tform);
private: static void warpTriangle(cv::Mat &img1,
	cv::Mat &img2,
	std::vector<cv::Point2f> t1,
	std::vector<cv::Point2f> t2);
private: static void applyAffineTransform(cv::Mat &warpImage,
	cv::Mat &src,
	std::vector<cv::Point2f> &srcTri,
	std::vector<cv::Point2f> &dstTri);

public: static int morphingCrop(cv::Mat &img1,
	cv::Mat &img2,
	cv::Mat &outputImg_1,
	cv::Mat &outputImg_2);
};

