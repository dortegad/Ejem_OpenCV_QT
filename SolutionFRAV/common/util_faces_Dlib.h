#include "opencv2\objdetect.hpp"

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>

class Util_Faces_Dlib
{
private: static dlib::frontal_face_detector Util_FacesDlib::detector;
private: static dlib::image_window Util_FacesDlib::win;

public: static bool init();
public: static bool detectFace(const cv::Mat &img, cv::Rect &rect);
};

