#include "opencv2\objdetect.hpp"

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


class Util_LBP_Dlib
{
public: static int LBP_RGB(cv::Mat &img, cv::Mat &features);
public: static int LBP_Depth(cv::Mat &imgDepth, cv::Mat &features);
};

