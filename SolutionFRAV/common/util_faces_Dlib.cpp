#include "util_faces_Dlib.h"

dlib::frontal_face_detector Util_FacesDlib::detector;
dlib::image_window Util_FacesDlib::win;

#include <iostream>


//------------------------------------------------------------------------------
bool Util_Faces_Dlib::init()
{
	Util_FacesDlib::detector = dlib::get_frontal_face_detector();
	
	return true;
}

//------------------------------------------------------------------------------
bool Util_Faces_Dlib::detectFace(const cv::Mat &img, cv::Rect &rect)
{
	/*
	std::cout << "processing image " << fileRGB << std::endl;
	dlib::array2d<unsigned char> img;
	dlib::load_image(img, fileRGB.c_str());

	dlib::pyramid_up(img);

	// Now tell the face detector to give us a list of bounding boxes
	// around all the faces it can find in the image.
	std::vector<dlib::rectangle> dets = detector(img);

	std::cout << "Number of faces detected: " << dets.size() << std::endl;
	// Now we show the image on the screen and the face detections as
	// red overlay boxes.
	win.clear_overlay();
	win.set_image(img);
	win.add_overlay(dets, dlib::rgb_pixel(255, 0, 0));

	std::cout << "Hit enter to process the next image..." << std::endl;
	std::cin.get();
	
	if (faces.size() > 0)
	{
		rect = faces[0];
		return true;
	}
	else
		return false;
	*/
	retunr true;
}


//------------------------------------------------------------------------------

bool dlibDetectFace()
{

}