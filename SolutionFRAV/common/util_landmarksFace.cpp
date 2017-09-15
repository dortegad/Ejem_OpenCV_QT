#include "util_landmarksFace.h"

dlib::frontal_face_detector Util_LandmarksFace::detector;
dlib::shape_predictor Util_LandmarksFace::sp;

//------------------------------------------------------------------------------
bool Util_LandmarksFace::init()
{
		// We need a face detector.  We will use this to get bounding boxes for
	// each face in an image.
	Util_LandmarksFace::detector = dlib::get_frontal_face_detector();
	// And we also need a shape_predictor.  This is the tool that will predict face
	// landmark positions given an image and face bounding box.  Here we are just
	// loading the model from the shape_predictor_68_face_landmarks.dat file you gave
	// as a command line argument.
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> Util_LandmarksFace::sp;

	return true;
}



// ----------------------------------------------------------------------------------------
void Util_LandmarksFace::facePoints(cv::Mat &img, std::vector<cv::Point2f> &points)
{
	try
	{
		// This example takes in a shape model file and then a list of images to
		// process.  We will take these filenames in as command line arguments.
		// Dlib comes with example images in the examples/faces folder so give
		// those as arguments to this program.
		//if (argc == 1)
		//{
		//	cout << "Call this program like this:" << endl;
		//	cout << "./face_landmark_detection_ex shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
		//	cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
		//	cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
		//	return 0;
		//}

		float inv = 3;
		cv::Mat imgMat;
		cv::resize(img, imgMat, cv::Size(img.cols / inv, img.rows / inv));
		dlib::cv_image<dlib::bgr_pixel> img(imgMat);

		// Now tell the face detector to give us a list of bounding boxes
		// around all the faces in the image.
		std::vector<dlib::rectangle> dets = Util_LandmarksFace::detector(img);
		//cout << "Number of faces detected: " << dets.size() << endl;

		std::vector<dlib::full_object_detection> shapes;
		for (unsigned long j = 0; j < dets.size(); ++j)
		{
			dlib::full_object_detection shape = Util_LandmarksFace::sp(img, dets[j]);
			shapes.push_back(shape);

			const dlib::full_object_detection& d = shapes[0];
			for (unsigned long k = 0; k < shape.num_parts(); ++k)
			{
				//std::cout << shape.part(k).x() << " " << shape.part(k).y() << endl;
				double x = shape.part(k).x()*inv;
				double y = shape.part(k).y()*inv;
				points.push_back(cv::Point2f(x, y));
			}
		}
	}
	catch (std::exception& e)
	{
		std::cout << "\nexception thrown!" << std::endl;
		std::cout << e.what() << std::endl;
	}
}