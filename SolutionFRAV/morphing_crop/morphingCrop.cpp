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

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iomanip>

#include "util_landmarksFace.h"
#include "util_morphing.h"
#include "util_files.h"
#include "util_draw.h"

using namespace cv;
using namespace std;

//---------------------------------------------------------------------------------------------
//PARA GENERAR LOS MORPHING DE AS IMAGENES DE LA DGP
int main(int argc, char** argv)
{
	std::string dir = "c:\\FOTOS CHIP";
	std::string dirDestino = "c:\\FOTOS CHIP MOR\\";
	
	Util_LandmarksFace::init();

	std::vector<std::string> files;
	int numFiles = Util_Files::filesDIR(dir, files, "*.jpg");
	for (int file1 = 0; file1 < numFiles; file1++)
	{
		std::string fileName1 = files[file1];
		std::string fileName1Dest = Util_Files::fileName(fileName1);

		Mat img1 = imread(fileName1);
		//cv::imshow("img1", img1);
		for (int file2 = 0; file2 < numFiles; file2++)
		{
			std::string fileName2 = files[file2];
			std::string fileName2Dest = Util_Files::fileName(fileName2);

			std::cout << fileName1 << std::endl;
			std::cout << fileName2 << std::endl;

			Mat img2 = imread(fileName2);
			//cv::imshow("img2", img2);
			//cv::waitKey();

			cv::Mat imgOut1;
			cv::Mat imgOut2;
			int result = Util_Morphing::morphingCrop(img1, img2, imgOut1, imgOut2);
			if (result == 0)
			{
				std::stringstream fileOutName1;
				fileOutName1 << dirDestino << fileName1Dest << "_" << fileName2Dest << "_1.jpg";
				cv::imwrite(fileOutName1.str(),imgOut1);
				//cv::imshow(fileOutName1.str(), imgOut1);

				std::stringstream fileOutName2;
				fileOutName2 << dirDestino << fileName1Dest << "_" << fileName2Dest << "_2.jpg";
				cv::imwrite(fileOutName2.str(), imgOut2);
				//cv::imshow(fileOutName2.str(), imgOut2);

				cv::waitKey();
				
				imgOut1.release();
				imgOut2.release();
				img2.release();
			}
			else
				std::cout << "No realizado morphing" << std::endl;
		}
		img1.release();
	}
}

//----------------------------------------------------------------------------------------------
int main_frav(int argc, char** argv)
{
	/*
	std::string filename1("E:\\DB_FINAL\\SONY\\crop\\from\\users\\USUARIO_007_02.JPG");
	std::string filename2("E:\\DB_FINAL\\SONY\\crop\\from\\users\\USUARIO_018.JPG");
	Mat img1 = imread(filename1);
	Mat img2 = imread(filename2);
	cv::Mat imgOut1;
	cv::Mat imgOut2;
	Util_Morphing::morphingCrop(img1, img2, imgOut1, imgOut2);
	imshow("image_1", imgOut1);
	imshow("image_2", imgOut2);
	cv::waitKey();
	*/

	Util_LandmarksFace::init();

	std::string dir = /*argv[1];//*/ "E:\\DB_FINAL\\SONY\\crop\\from\\users";

	std::string baseFich = "E:\\DB_FINAL\\SONY\\crop\\from\\users\\USUARIO_";
	std::string extFich = ".JPG";

	std::string fichA;// = /*argv[2];//*/ "E:\\DB_FINAL\\SONY\\crop\\from\\users\\USUARIO_000.JPG";
	std::string fichB;// = /*argv[3];//*/ "E:\\DB_FINAL\\SONY\\crop\\from\\users\\USUARIO_000.JPG";
	std::string outDirName = /*argv[4]; //*/"C:\\NEW_MORPHING\\";

	while (true)
	{
		bool retry = true;
		std::ifstream file;
		do{
			file.open("c:\\NEW_MORPHING\\actual.txt");
			retry = !file.is_open();
		} while (retry);
		std::string actual;
		file >> fichA;
		file >> fichB;
		file.close();

		std::cout << fichA << std::endl;
		std::cout << fichB << std::endl;

		int numFichA = std::atoi(fichA.c_str());
		int numFichB = std::atoi(fichB.c_str());


		int sigA = numFichA;
		int sigB = numFichB + 1;
		if (sigB >= 186)
		{
			sigB = 0;
			sigA = numFichA + 1;

			if (sigA >= 186)
				return 0;
		}

		retry = true;
		std::ofstream fileout;
		do{
			fileout.open("c:\\NEW_MORPHING\\actual.txt");
			retry = !fileout.is_open();
		} while (retry);
		fileout << sigA << std::endl;
		fileout << sigB << std::endl;
		fileout.close();

		char stringNumFichA[3];
		char stringNumFichB[3];
		sprintf(stringNumFichA, "%03d", numFichA);
		sprintf(stringNumFichB, "%03d", numFichB);

		std::stringstream nameFichA;
		std::stringstream nameFichB;
		nameFichA << baseFich << stringNumFichA << extFich;
		nameFichB << baseFich << stringNumFichB << extFich;

		cv::Mat img_01 = cv::imread(nameFichA.str());
		cv::Mat img_02 = cv::imread(nameFichB.str());
		//imshow("image 01", img_01);
		//imshow("image 02", img_02);
		cv::Mat imgMorphing_01;
		cv::Mat imgMorphing_02;
		Util_Morphing::morphingCrop(img_01, img_02, imgMorphing_01, imgMorphing_02);
		//imshow("image morp 01", imgMorphing_01);
		//imshow("image mopr 02", imgMorphing_02);

		std::stringstream outDirUserA;
		outDirUserA << outDirName << "\\" << "USER_" << stringNumFichA;
		Util_Files::createDIR(outDirUserA.str().c_str());

		std::stringstream nameMorpFich_01;
		std::stringstream nameMorpFich_02;
		nameMorpFich_01 << outDirUserA.str() << "\\USER_" << stringNumFichA << "_" << stringNumFichB << "_01.jpg";
		nameMorpFich_02 << outDirUserA.str() << "\\USER_" << stringNumFichA << "_" << stringNumFichB << "_02.jpg";
	
		cv::imwrite(nameMorpFich_01.str(), imgMorphing_01);
		cv::imwrite(nameMorpFich_02.str(), imgMorphing_02);

		//cv::waitKey(4);
	}
	return 0;
	
	/*
	Util_LandmarksFace::init();

	std::vector<std::string> files;
	int numFiles = Util_Files::filesDIR(dir, files, "*.jpg");

	char numberUserB[3];
	char numberUserA[3];
	bool findFirstA = false;
	bool findFirstB = false;
	for (int i = 0; i<numFiles; i++)
	{
		if (!findFirstA)
		{
			if (files[i] == firstA)
			{
				findFirstA = true;
			}
			else
			{
				continue;
			}
		}

		
		sprintf(numberUserA, "%03d", i);
		std::stringstream outDirUserA;
		outDirUserA << outDirName << "\\" << "USER_" << numberUserA;
		Util_Files::createDIR(outDirUserA.str().c_str());


		for (int j = 0; j<numFiles; j++)
		{
			if (!findFirstB)
			{
				if (files[j] == firstB)
				{
					findFirstB = true;
				}
				else
				{
					continue;
				}
			}
			cv::Mat img_01 = cv::imread(files[i]);
			cv::Mat img_02 = cv::imread(files[j]);
			cv::Mat imgMorphing_01;
			cv::Mat imgMorphing_02;
			Util_Morphing::morphingCrop(img_01, img_02, imgMorphing_01,imgMorphing_02);
			imshow("image 01", imgMorphing_01);
			imshow("image 02", imgMorphing_02);
			//cv::waitKey();

			sprintf(numberUserB, "%03d", j);
			std::stringstream fileName_01;
			std::stringstream fileName_02;
			fileName_01 << outDirName.c_str() << "\\USER_" << numberUserA << "\\USER_" << numberUserA << "_" << numberUserB << "_01.jpg";
			fileName_02 << outDirName.c_str() << "\\USER_" << numberUserA << "\\USER_" << numberUserA << "_" << numberUserB << "_02.jpg";
			//cv::imwrite(fileName_01.str(), imgMorphing_01);
			//cv::imwrite(fileName_02.str(), imgMorphing_02);

			std::cout << fileName_01.str().c_str() << std::endl;
			std::cout << fileName_02.str().c_str() << std::endl;

			std::stringstream file_user_A;
			std::stringstream file_user_B;
			file_user_A << "E:\\DB_FINAL\\SONY\\crop\\from\\users\\USUARIO_" << numberUserA << ".JPG";
			int sig = 0;// j + 1;
			if (j+1 < numFiles)
				sig = j+1;
			sprintf(numberUserB, "%03d", sig);
			file_user_B << "E:\\DB_FINAL\\SONY\\crop\\from\\users\\USUARIO_" << numberUserB << ".JPG";
			std::ofstream fileout("c:\\NEW_MORPHING\\actual.txt");
			fileout << file_user_A.str().c_str() << std::endl;
			fileout << file_user_B.str().c_str() << std::endl;
			fileout.close();

			firstB = file_user_B.str().c_str();

			std::cout << file_user_A.str().c_str() << std::endl;
			std::cout << file_user_B.str().c_str() << std::endl;
		}
	}

	
	return 0;
	*/
}