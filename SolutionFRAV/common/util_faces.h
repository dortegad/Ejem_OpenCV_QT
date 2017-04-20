#include "opencv2\objdetect.hpp"

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


class Util_Faces
{
private: static cv::CascadeClassifier face_cascade;

public: static bool init();
public: static bool detectFace(const cv::Mat &img, cv::Rect &rect);
};

