// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


#include "Cam3D.h"

int main(int argc, char* argv[])
{
	Cam3D cam;

	if (cam.init() != 0)
		return 0;

	int frame = 0;
	for (;; frame++)
	{
		cam.isAttack();

		cv::waitKey(30);
	}

	cam.stop();

	return 0;
}