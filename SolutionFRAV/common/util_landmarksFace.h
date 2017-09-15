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

class Util_LandmarksFace
{
	
private: static dlib::frontal_face_detector detector;
private: static dlib::shape_predictor sp;

public: static bool init();
public: static void facePoints(cv::Mat &img, 
							  std::vector<cv::Point2f> &points);
};

