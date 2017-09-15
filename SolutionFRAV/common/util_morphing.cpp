#include "util_morphing.h"


#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

#include "util_landmarksFace.h"
#include "util_draw.h"


//----------------------------------------------------------------------------------------------
// Calculate Delaunay triangles for set of points
// Returns the vector of indices of 3 points for each triangle
void Util_Morphing::calculateDelaunayTriangles(cv::Rect rect, 
		std::vector<cv::Point2f> &points, 
		std::vector< std::vector<int> > &delaunayTri)
{
	// Create an instance of Subdiv2D
	cv::Subdiv2D subdiv(rect);

	// Insert points into subdiv
	for (std::vector<cv::Point2f>::iterator it = points.begin(); it != points.end(); it++)
		subdiv.insert(*it);

	std::vector<cv::Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	std::vector<cv::Point2f> pt(3);
	std::vector<int> ind(3);

	for (size_t i = 0; i < triangleList.size(); i++)
	{
		cv::Vec6f t = triangleList[i];
		pt[0] = cv::Point2f(t[0], t[1]);
		pt[1] = cv::Point2f(t[2], t[3]);
		pt[2] = cv::Point2f(t[4], t[5]);

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
// Constrains points to be inside boundary
void Util_Morphing::constrainPoint(cv::Point2f &p, cv::Size sz)
{
	p.x = std::min(std::max((double)p.x, 0.0), (double)(sz.width - 1));
	p.y = std::min(std::max((double)p.y, 0.0), (double)(sz.height - 1));

}

//-------------------------------------------------------------------------------------------------
// Compute similarity transform given two pairs of corresponding points.
// OpenCV requires 3 pairs of corresponding points.
// We are faking the third one.
void Util_Morphing::similarityTransform(std::vector<cv::Point2f>& inPoints, 
	std::vector<cv::Point2f>& outPoints, 
	cv::Mat &tform)
{

	double s60 = sin(60 * M_PI / 180.0);
	double c60 = cos(60 * M_PI / 180.0);

	std::vector <cv::Point2f> inPts = inPoints;
	std::vector <cv::Point2f> outPts = outPoints;

	inPts.push_back(cv::Point2f(0, 0));
	outPts.push_back(cv::Point2f(0, 0));


	inPts[2].x = c60 * (inPts[0].x - inPts[1].x) - s60 * (inPts[0].y - inPts[1].y) + inPts[1].x;
	inPts[2].y = s60 * (inPts[0].x - inPts[1].x) + c60 * (inPts[0].y - inPts[1].y) + inPts[1].y;

	outPts[2].x = c60 * (outPts[0].x - outPts[1].x) - s60 * (outPts[0].y - outPts[1].y) + outPts[1].x;
	outPts[2].y = s60 * (outPts[0].x - outPts[1].x) + c60 * (outPts[0].y - outPts[1].y) + outPts[1].y;


	tform = cv::estimateRigidTransform(inPts, outPts, false);
}

//----------------------------------------------------------------------------------------------
// Apply affine transform calculated using srcTri and dstTri to src
void Util_Morphing::applyAffineTransform(cv::Mat &warpImage, 
	cv::Mat &src, 
	std::vector<cv::Point2f> &srcTri, 
	std::vector<cv::Point2f> &dstTri)
{
	// Given a pair of triangles, find the affine transform.
	cv::Mat warpMat = getAffineTransform(srcTri, dstTri);
	
	// Apply the Affine Transform just found to the src image
	warpAffine(src, warpImage, warpMat, warpImage.size(), cv::INTER_LINEAR, cv::BORDER_REFLECT_101);
}

//----------------------------------------------------------------------------------------------
// Warps and alpha blends triangular regions from img1 and img2 to img
void Util_Morphing::warpTriangle(cv::Mat &img1, 
	cv::Mat &img2, 
	std::vector<cv::Point2f> t1, 
	std::vector<cv::Point2f> t2)
{
	// Find bounding rectangle for each triangle
	cv::Rect r1 = boundingRect(t1);
	cv::Rect r2 = boundingRect(t2);

	// Offset points by left top corner of the respective rectangles
	std::vector<cv::Point2f> t1Rect, t2Rect;
	std::vector<cv::Point> t2RectInt;
	for (int i = 0; i < 3; i++)
	{
		//tRect.push_back( Point2f( t[i].x - r.x, t[i].y -  r.y) );
		t2RectInt.push_back(cv::Point((int)(t2[i].x - r2.x), (int)(t2[i].y - r2.y))); // for fillConvexPoly

		t1Rect.push_back(cv::Point2f(t1[i].x - r1.x, t1[i].y - r1.y));
		t2Rect.push_back(cv::Point2f(t2[i].x - r2.x, t2[i].y - r2.y));
	}

	// Get mask by filling triangle
	cv::Mat mask = cv::Mat::zeros(r2.height, r2.width, CV_32FC3);
	fillConvexPoly(mask, t2RectInt, cv::Scalar(1.0, 1.0, 1.0), 16, 0);

	// Apply warpImage to small rectangular patches
	cv::Mat img1Rect, img2Rect;
	img1(r1).copyTo(img1Rect);

	cv::Mat warpImage = cv::Mat::zeros(r2.height, r2.width, img1Rect.type());

	applyAffineTransform(warpImage, img1Rect, t1Rect, t2Rect);

	// Copy triangular region of the rectangular patch to the output image
	multiply(warpImage, mask, warpImage);
	multiply(img2(r2), cv::Scalar(1.0, 1.0, 1.0) - mask, img2(r2));
	img2(r2) = img2(r2) + warpImage;

}


//----------------------------------------------------------------------------------------------
int Util_Morphing::morphingCrop(cv::Mat &img1, 
							cv::Mat &img2, 
							cv::Mat &outputImg_1,
							cv::Mat &outputImg_2)
{
	int w = 600;
	int h = 800;

	std::vector<std::vector<cv::Point2f> > allPoints;

	std::vector<cv::Mat> images;

	//Mat img1 = imread(filename1);
	//Mat img2 = imread(filename2);

	//Read points
	std::vector<cv::Point2f> points1;// = readPoints(filename1 + ".txt");
	std::vector<cv::Point2f> points2;// = readPoints(filename2 + ".txt");
	Util_LandmarksFace::facePoints(img1, points1);
	Util_LandmarksFace::facePoints(img2, points2);
	allPoints.push_back(points1);
	allPoints.push_back(points2);

	img1.convertTo(img1, CV_32FC3, 1 / 255.0);
	img2.convertTo(img2, CV_32FC3, 1 / 255.0);
	images.push_back(img1);
	images.push_back(img2);

	int numImages = images.size();

	// Eye corners
	std::vector<cv::Point2f> eyecornerDst, eyecornerSrc;
	eyecornerDst.push_back(cv::Point2f(0.3*w, h / 2));
	eyecornerDst.push_back(cv::Point2f(0.7*w, h / 2));

	eyecornerSrc.push_back(cv::Point2f(0, 0));
	eyecornerSrc.push_back(cv::Point2f(0, 0));

	// Space for normalized images and points.
	std::vector <cv::Mat> imagesNorm;
	std::vector < std::vector <cv::Point2f> > pointsNorm;

	// Space for average landmark points
	std::vector <cv::Point2f> pointsAvg(allPoints[0].size());

	// 8 Boundary points for Delaunay Triangulation
	std::vector <cv::Point2f> boundaryPts;
	boundaryPts.push_back(cv::Point2f(0, 0));
	boundaryPts.push_back(cv::Point2f(w / 2, 0));
	boundaryPts.push_back(cv::Point2f(w - 1, 0));
	boundaryPts.push_back(cv::Point2f(w - 1, h / 2));
	boundaryPts.push_back(cv::Point2f(w - 1, h - 1));
	boundaryPts.push_back(cv::Point2f(w / 2, h - 1));
	boundaryPts.push_back(cv::Point2f(0, h - 1));
	boundaryPts.push_back(cv::Point2f(0, h / 2));

	// Warp images and trasnform landmarks to output coordinate system,
	// and find average of transformed landmarks.

	for (size_t i = 0; i < images.size(); i++)
	{

		std::vector <cv::Point2f> points = allPoints[i];

		// The corners of the eyes are the landmarks number 36 and 45
		eyecornerSrc[0] = allPoints[i][36];
		eyecornerSrc[1] = allPoints[i][45];

		// Calculate similarity transform
		cv::Mat tform;
		similarityTransform(eyecornerSrc, eyecornerDst, tform);

		// Apply similarity transform to input image and landmarks
		cv::Mat img = cv::Mat::zeros(h, w, CV_32FC3);
		cv::warpAffine(images[i], img, tform, img.size());
		cv::transform(points, points, tform);

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
	cv::Rect rect(0, 0, w, h);
	std::vector< std::vector<int> > dt;
	Util_Morphing::calculateDelaunayTriangles(rect, pointsAvg, dt);

	// Space for output image
	cv::Mat output = cv::Mat::zeros(h, w, CV_32FC3);
	cv::Size size(w, h);


	// Warp input images to average image landmarks
	std::vector <cv::Mat> outputImgs;
	for (size_t i = 0; i < numImages; i++)
	{
		cv::Mat img = cv::Mat::zeros(h, w, CV_32FC3);
		// Transform triangles one by one
		for (size_t j = 0; j < dt.size(); j++)
		{
			// Input and output points corresponding to jth triangle
			std::vector<cv::Point2f> tin, tout;
			for (int k = 0; k < 3; k++)
			{
				cv::Point2f pIn = pointsNorm[i][dt[j][k]];
				constrainPoint(pIn, size);

				cv::Point2f pOut = pointsAvg[dt[j][k]];
				constrainPoint(pOut, size);

				tin.push_back(pIn);
				tout.push_back(pOut);
			}

			warpTriangle(imagesNorm[i], img, tin, tout);
		}

		// Add image intensities for averaging
		output = output + img;

		img.convertTo(img, CV_8UC3, 255.0);
		outputImgs.push_back(img);
	}

	// Divide by numImages to get average
	cv::Mat outputImg;
	output = output / (double)numImages;
	output.convertTo(outputImg, CV_8UC3, 255.0);

	std::vector <cv::Point2f> faceMorphingPoints;
	Util_LandmarksFace::facePoints(outputImg, faceMorphingPoints);
	std::vector<cv::Point2f> faceMorphingPointsConvexHull;
	cv::convexHull(faceMorphingPoints, faceMorphingPointsConvexHull, false);
	cv::Mat_<uchar> mask(output.rows, output.cols, (uchar)0);
	Util_Draw::drawFilledPoly(mask, faceMorphingPointsConvexHull, cv::Scalar(255));

	for (int i = 0; i < outputImgs.size(); i++)
	{
		outputImg.copyTo(outputImgs[i], mask);
	}

	outputImg_1 = outputImgs[0];
	outputImg_2 = outputImgs[1];


	return 0;
}