// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "CamF200.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

int main(int argc, char* argv[])
{
	CamF200 cam;

	if (cam.load() != 0)
		return 0;

	if (cam.init() != 0)
		return 0;

	int frame = 0;
	for (;; frame++)
	{
		cam.isAttack();

		cv::waitKey();
	}

	cam.stop();

	return 0;
}