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

	//Load configuration files
	if (cam.load() != 0)
		return 0;

	//Start camera
	if (cam.init() != 0)
		return 0;

	//Capture frames and detect attacks
	int frame = 0;
	for (;; frame++)
	{
		float result = cam.isAttack();

		std::cout << "RESULT = " << result;
		if (result > 0.28)
			std::cout << " = BONA FIDE" << std::endl;
		else
			std::cout << " = ATTACK" << std::endl;

		std::getchar();
	}

	//Close comera stream
	cam.stop();

	return 0;
}