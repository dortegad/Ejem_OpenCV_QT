/*
* Copyright (c) 2016 Satya Mallick <spmallick@learnopencv.com>
* All rights reserved. No warranty, explicit or implicit, provided.
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <vector>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>
#include <dlib/opencv.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iomanip>

#define _USE_MATH_DEFINES
#include <math.h>

#include "util_files.h"

using namespace cv;
using namespace std;

//-------------------------------------------------------------------------------------------------
// Compute similarity transform given two pairs of corresponding points.
// OpenCV requires 3 pairs of corresponding points.
// We are faking the third one.
void similarityTransform(std::vector<cv::Point2f>& inPoints, std::vector<cv::Point2f>& outPoints, cv::Mat &tform)
{

	double s60 = sin(60 * M_PI / 180.0);
	double c60 = cos(60 * M_PI / 180.0);

	vector <Point2f> inPts = inPoints;
	vector <Point2f> outPts = outPoints;

	inPts.push_back(cv::Point2f(0, 0));
	outPts.push_back(cv::Point2f(0, 0));


	inPts[2].x = c60 * (inPts[0].x - inPts[1].x) - s60 * (inPts[0].y - inPts[1].y) + inPts[1].x;
	inPts[2].y = s60 * (inPts[0].x - inPts[1].x) + c60 * (inPts[0].y - inPts[1].y) + inPts[1].y;

	outPts[2].x = c60 * (outPts[0].x - outPts[1].x) - s60 * (outPts[0].y - outPts[1].y) + outPts[1].x;
	outPts[2].y = s60 * (outPts[0].x - outPts[1].x) + c60 * (outPts[0].y - outPts[1].y) + outPts[1].y;


	tform = cv::estimateRigidTransform(inPts, outPts, false);
}


//----------------------------------------------------------------------------------------------
// Read points from list of text file
void readPoints(vector<string> pointsFileNames, vector<vector<Point2f> > &pointsVec){

	for (size_t i = 0; i < pointsFileNames.size(); i++)
	{
		vector<Point2f> points;
		ifstream ifs(pointsFileNames[i]);
		float x, y;
		while (ifs >> x >> y)
			points.push_back(Point2f((float)x, (float)y));

		pointsVec.push_back(points);
	}

}


//----------------------------------------------------------------------------------------------
// Calculate Delaunay triangles for set of points
// Returns the vector of indices of 3 points for each triangle
static void calculateDelaunayTriangles(Rect rect, vector<Point2f> &points, vector< vector<int> > &delaunayTri){

	// Create an instance of Subdiv2D
	Subdiv2D subdiv(rect);

	// Insert points into subdiv
	for (vector<Point2f>::iterator it = points.begin(); it != points.end(); it++)
		subdiv.insert(*it);

	vector<Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	vector<Point2f> pt(3);
	vector<int> ind(3);

	for (size_t i = 0; i < triangleList.size(); i++)
	{
		Vec6f t = triangleList[i];
		pt[0] = Point2f(t[0], t[1]);
		pt[1] = Point2f(t[2], t[3]);
		pt[2] = Point2f(t[4], t[5]);

		if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])){
			for (int j = 0; j < 3; j++)
				for (size_t k = 0; k < points.size(); k++)
					if (abs(pt[j].x - points[k].x) < 1.0 && abs(pt[j].y - points[k].y) < 1)
						ind[j] = k;

			delaunayTri.push_back(ind);
		}
	}

}

//----------------------------------------------------------------------------------------------
// Apply affine transform calculated using srcTri and dstTri to src
void applyAffineTransform(Mat &warpImage, Mat &src, vector<Point2f> &srcTri, vector<Point2f> &dstTri)
{
	// Given a pair of triangles, find the affine transform.
	Mat warpMat = getAffineTransform(srcTri, dstTri);

	// Apply the Affine Transform just found to the src image
	warpAffine(src, warpImage, warpMat, warpImage.size(), INTER_LINEAR, BORDER_REFLECT_101);
}


//----------------------------------------------------------------------------------------------
// Warps and alpha blends triangular regions from img1 and img2 to img
void warpTriangle(Mat &img1, Mat &img2, vector<Point2f> t1, vector<Point2f> t2)
{
	// Find bounding rectangle for each triangle
	Rect r1 = boundingRect(t1);
	Rect r2 = boundingRect(t2);

	// Offset points by left top corner of the respective rectangles
	vector<Point2f> t1Rect, t2Rect;
	vector<Point> t2RectInt;
	for (int i = 0; i < 3; i++)
	{
		//tRect.push_back( Point2f( t[i].x - r.x, t[i].y -  r.y) );
		t2RectInt.push_back(Point((int)(t2[i].x - r2.x), (int)(t2[i].y - r2.y))); // for fillConvexPoly

		t1Rect.push_back(Point2f(t1[i].x - r1.x, t1[i].y - r1.y));
		t2Rect.push_back(Point2f(t2[i].x - r2.x, t2[i].y - r2.y));
	}

	// Get mask by filling triangle
	Mat mask = Mat::zeros(r2.height, r2.width, CV_32FC3);
	fillConvexPoly(mask, t2RectInt, Scalar(1.0, 1.0, 1.0), 16, 0);

	// Apply warpImage to small rectangular patches
	Mat img1Rect, img2Rect;
	img1(r1).copyTo(img1Rect);

	Mat warpImage = Mat::zeros(r2.height, r2.width, img1Rect.type());

	applyAffineTransform(warpImage, img1Rect, t1Rect, t2Rect);

	// Copy triangular region of the rectangular patch to the output image
	multiply(warpImage, mask, warpImage);
	multiply(img2(r2), Scalar(1.0, 1.0, 1.0) - mask, img2(r2));
	img2(r2) = img2(r2) + warpImage;

}

//----------------------------------------------------------------------------------------------
// Constrains points to be inside boundary
void constrainPoint(Point2f &p, Size sz)
{
	p.x = min(max((double)p.x, 0.0), (double)(sz.width - 1));
	p.y = min(max((double)p.y, 0.0), (double)(sz.height - 1));

}


dlib::frontal_face_detector detector;
dlib::shape_predictor sp;
// ----------------------------------------------------------------------------------------
void facePoints(cv::Mat &img,
	std::vector<cv::Point2f> &points)
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

		// We need a face detector.  We will use this to get bounding boxes for
		// each face in an image.
		//dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
		// And we also need a shape_predictor.  This is the tool that will predict face
		// landmark positions given an image and face bounding box.  Here we are just
		// loading the model from the shape_predictor_68_face_landmarks.dat file you gave
		// as a command line argument.
		//dlib::shape_predictor sp;
		//dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> sp;


		//dlib::image_window win, win_faces;
		// Loop over all the images provided on the command line.
		//for (int i = 2; i < argc; ++i)
		//{
		//	std::string filename(argv[i]);
		//	cout << "processing image " << filename << endl;
		//array2d<rgb_pixel> img;
		//load_image(img, argv[i]);
		//	cv::Mat imgMat = cv::imread(argv[i]);

		float inv = 3;
		cv::Mat imgMat;
		cv::resize(img, imgMat, cv::Size(img.cols/inv,img.rows/inv));
		dlib::cv_image<dlib::bgr_pixel> img(imgMat);

		// Now tell the face detector to give us a list of bounding boxes
		// around all the faces in the image.
		std::vector<dlib::rectangle> dets = detector(img);
		cout << "Number of faces detected: " << dets.size() << endl;

		// Now we will go ask the shape_predictor to tell us the pose of
		// each face we detected.

		//size_t lastindex = filename.find_last_of(".");
		//string basename = filename.substr(0, lastindex);

		std::vector<dlib::full_object_detection> shapes;
		for (unsigned long j = 0; j < dets.size(); ++j)
		{
			dlib::full_object_detection shape = sp(img, dets[j]);
			shapes.push_back(shape);

			//std::stringstream points_filename;
			//std::ofstream ofs;

			//if (j == 0)
			//{
			//	points_filename << basename << ".txt";
			//}
			//else
			//{
			//	points_filename << basename << "_" << j << ".txt";
			//}

			//ofs.open(points_filename.str().c_str());
			const dlib::full_object_detection& d = shapes[0];
			for (unsigned long k = 0; k < shape.num_parts(); ++k)
			{
				std::cout << shape.part(k).x() << " " << shape.part(k).y() << endl;
				double x = shape.part(k).x()*inv;
				double y = shape.part(k).y()*inv;
				points.push_back(cv::Point2f(x,y));
			}
			//ofs.close();
			//render_face(imgMat, shape);

		}

		//	cv::imshow("image", imgMat);
		//	cv::waitKey(0);
		//}
		//points.push_back(Point2f(0, 0));
		//points.push_back(Point2f(imgMat.cols / 2, 0));
		//points.push_back(Point2f(imgMat.cols - 1, 0));
		//points.push_back(Point2f(imgMat.cols - 1, imgMat.rows / 2));
		//points.push_back(Point2f(imgMat.cols - 1, imgMat.rows - 1));
		//points.push_back(Point2f(imgMat.cols / 2, imgMat.rows - 1));
		//points.push_back(Point2f(0, imgMat.rows - 1));
		//points.push_back(Point2f(0, imgMat.rows / 2));
	}
	catch (exception& e)
	{
		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
	}
}

//----------------------------------------------------------------------------------------------
int morphing(const std::string filename1, const std::string filename2, cv::Mat &outputImg)
{
	int w = 600;
	int h = 800;

	vector<vector<Point2f> > allPoints;
	
	vector<Mat> images;
	
	Mat img1 = imread(filename1);
	Mat img2 = imread(filename2);

	//Read points
	std::vector<cv::Point2f> points1;// = readPoints(filename1 + ".txt");
	std::vector<cv::Point2f> points2;// = readPoints(filename2 + ".txt");
	facePoints(img1, points1);
	facePoints(img2, points2);
	allPoints.push_back(points1);
	allPoints.push_back(points2);

	img1.convertTo(img1, CV_32FC3, 1 / 255.0);
	img2.convertTo(img2, CV_32FC3, 1 / 255.0);
	images.push_back(img1);
	images.push_back(img2);

	int numImages = images.size();

	// Eye corners
	vector<Point2f> eyecornerDst, eyecornerSrc;
	eyecornerDst.push_back(Point2f(0.3*w, h/2));
	eyecornerDst.push_back(Point2f(0.7*w, h/2));

	eyecornerSrc.push_back(Point2f(0, 0));
	eyecornerSrc.push_back(Point2f(0, 0));

	// Space for normalized images and points.
	vector <Mat> imagesNorm;
	vector < vector <Point2f> > pointsNorm;

	// Space for average landmark points
	vector <Point2f> pointsAvg(allPoints[0].size());

	// 8 Boundary points for Delaunay Triangulation
	vector <Point2f> boundaryPts;
	boundaryPts.push_back(Point2f(0, 0));
	boundaryPts.push_back(Point2f(w / 2, 0));
	boundaryPts.push_back(Point2f(w - 1, 0));
	boundaryPts.push_back(Point2f(w - 1, h / 2));
	boundaryPts.push_back(Point2f(w - 1, h - 1));
	boundaryPts.push_back(Point2f(w / 2, h - 1));
	boundaryPts.push_back(Point2f(0, h - 1));
	boundaryPts.push_back(Point2f(0, h / 2));

	// Warp images and trasnform landmarks to output coordinate system,
	// and find average of transformed landmarks.

	for (size_t i = 0; i < images.size(); i++)
	{

		vector <Point2f> points = allPoints[i];

		// The corners of the eyes are the landmarks number 36 and 45
		eyecornerSrc[0] = allPoints[i][36];
		eyecornerSrc[1] = allPoints[i][45];

		// Calculate similarity transform
		Mat tform;
		similarityTransform(eyecornerSrc, eyecornerDst, tform);

		// Apply similarity transform to input image and landmarks
		Mat img = Mat::zeros(h, w, CV_32FC3);
		warpAffine(images[i], img, tform, img.size());
		transform(points, points, tform);

		// Calculate average landmark locations
		for (size_t j = 0; j < points.size(); j++)
		{
			pointsAvg[j] += points[j] * (1.0 / numImages);
		}

		// Append boundary points. Will be used in Delaunay Triangulation
		for (size_t j = 0; j < boundaryPts.size(); j++)
		{
			points.push_back(boundaryPts[j]);
		}

		pointsNorm.push_back(points);
		imagesNorm.push_back(img);
	}

	// Append boundary points to average points.
	for (size_t j = 0; j < boundaryPts.size(); j++)
	{
		pointsAvg.push_back(boundaryPts[j]);
	}

	// Calculate Delaunay triangles
	Rect rect(0, 0, w, h);
	vector< vector<int> > dt;
	calculateDelaunayTriangles(rect, pointsAvg, dt);

	// Space for output image
	Mat output = Mat::zeros(h, w, CV_32FC3);
	Size size(w, h);

	// Warp input images to average image landmarks

	for (size_t i = 0; i < numImages; i++)
	{
		Mat img = Mat::zeros(h, w, CV_32FC3);
		// Transform triangles one by one
		for (size_t j = 0; j < dt.size(); j++)
		{
			// Input and output points corresponding to jth triangle
			vector<Point2f> tin, tout;
			for (int k = 0; k < 3; k++)
			{
				Point2f pIn = pointsNorm[i][dt[j][k]];
				constrainPoint(pIn, size);

				Point2f pOut = pointsAvg[dt[j][k]];
				constrainPoint(pOut, size);

				tin.push_back(pIn);
				tout.push_back(pOut);
			}

			warpTriangle(imagesNorm[i], img, tin, tout);
		}

		// Add image intensities for averaging
		output = output + img;

	}

	// Divide by numImages to get average
	output = output / (double)numImages;
	output.convertTo(outputImg, CV_8UC3,255.0);

	return 0;
}


//----------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	// We need a face detector.  We will use this to get bounding boxes for
	// each face in an image.
	detector = dlib::get_frontal_face_detector();
	// And we also need a shape_predictor.  This is the tool that will predict face
	// landmark positions given an image and face bounding box.  Here we are just
	// loading the model from the shape_predictor_68_face_landmarks.dat file you gave
	// as a command line argument.
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> sp;

	//string filename1(".\\mophingData\\hillary_clinton.jpg");
	//string filename2(".\\mophingData\\ted_cruz.jpg");

	//string filename1("D:\\MORPH\\PRUEBAS\\DGP-POLICIA\\07816337.jpg");
	//string filename2("D:\\MORPH\\PRUEBAS\\DGP-POLICIA\\52873470.jpg");


	//string filename1("F:\\IMPRIMIR\\CAMISETAS\\FRENTE_01.jpg");
	//string filename2("F:\\IMPRIMIR\\CAMISETAS\\FRENTE_02.jpg");

	/*
	std::string filename1("E:\\DB_FINAL\\SONY\\crop\\from\\users");
	std::string filename2("E:\\DB_FINAL\\SONY\\crop\\from\\users\\USUARIO_001.jpg");
	cv::Mat img;
	morphing(filename1, filename2,img);
	imshow("image", img);
	cv::waitKey();
	*/

	std::string dir = "E:\\MORPH\\USERS\\";
	std::string outDirName = "E:\\MORPH\\FRAV_MORPH_ONE_PER_USER";

	std::vector<std::string> files;
	int numFiles = Util_Files::filesDIR(dir, files,"*.jpg");

	for (int i=0; i<numFiles; i++)
	{
		for (int j=0; j<numFiles; j++)
		{
			cv::Mat img;
			morphing(files[i],files[j],img);
			imshow("image", img);
			cv::waitKey();
		}
		/*
		std::string fileRGB = files[i];
		cv::Mat imgRGB = cv::imread(fileRGB);

		std::string fileName = fileRGB.substr(fileRGB.length() - 16, fileRGB.length());
		std::stringstream fileOut;
		fileOut << outDirName << "\\" << fileName;
		std::cout << fileOut.str().c_str() << std::endl;

		cv::imwrite(fileOut.str().c_str(), imgRGB);
		*/
	}
}