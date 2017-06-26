#include "opencv2\objdetect.hpp"

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


class Util_Eyes
{
private: static cv::CascadeClassifier eyes_cascade;

public: static bool init();
public: static bool detectEyes(const cv::Mat &img, cv::Rect &rectEye1, cv::Rect &rectEye2);
};

