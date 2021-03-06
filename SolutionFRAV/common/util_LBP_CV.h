#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include "opencv2\objdetect\objdetect.hpp"


class Util_LBP_CV
{
public: static int LBP_RGB(cv::Mat &img, cv::Mat &features);
public: static int LBP_Depth(cv::Mat &imgDepth, cv::Mat &features);
public:	static int LBP_RGB_Eyes(cv::Mat &imgRGB, cv::Mat &features);
public:	static int LBP_RGB_Hair(cv::Mat &imgRGB, cv::Mat &features);
public:	static int LBP_RGB_Eyes_Hair(cv::Mat &imgRGB, cv::Mat &features);
};

