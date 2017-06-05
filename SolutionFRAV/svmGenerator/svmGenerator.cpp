// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


#include <iostream>
#include <sstream>
#include <string>

#include <util_faces.h>
#include <util_depth.h>
#include <util_LBP_Dlib.h>
#include <util_LBP_CV.h>
#include <util_files.h>
#include <util_fravAttack.h>
#include <util_SVM.h>

//------------------------------------------------------------------------------
void trainTestSVM(std::vector<std::string> &files00, std::vector<std::string> &files01,
				  const std::string &svmFile, std::string &resultFile)
{
	std::vector<std::string> files00_train;
	std::vector<std::string> files00_test;
	std::vector<std::string> files01_train;
	std::vector<std::string> files01_test;
	if (Util_SVM::groupTestTrain(files00, files01,
								files00_train, files00_test,
								files01_train, files01_test))
	{
		Util_SVM::train(files00_train, files01_train, svmFile);

		Util_SVM::test(files00_test, files01_test, svmFile, resultFile);
	}
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	if (argc < 2)
		return 0;

	std::string descriptorDIR = argv[1];// "D:\\DESCRIPTORS_PRUEBA\\";
	std::vector <std::string> files;

	Util_Files::filesDIR(descriptorDIR, files);

	std::vector<std::string> filesAttack_00;
	Util_FravAttack::filesAttack(files, "00", filesAttack_00);

	//std::vector<std::string> filesAttack_01;
	//Util_FravAttack::filesAttack(files, "01", filesAttack_01);

	//std::vector<std::string> filesAttack_02;
	//Util_FravAttack::filesAttack(files, "02", filesAttack_02);

	//std::vector<std::string> filesAttack_03;
	//Util_FravAttack::filesAttack(files, "03", filesAttack_03);

	//std::vector<std::string> filesAttack_04;
	//Util_FravAttack::filesAttack(files, "04", filesAttack_04);

	//std::vector<std::string> filesAttack_05;
	//Util_FravAttack::filesAttack(files, "05", filesAttack_05);

	std::vector<std::string> filesAttack_06;
	Util_FravAttack::filesAttack(files, "06", filesAttack_06);

	//std::vector<std::string> files_No_Attack_00;
	//Util_FravAttack::filesNoAttack(files, "00", files_No_Attack_00);

	//std::cout << "attack 01" << std::endl;
	//trainTestSVM(filesAttack_00, filesAttack_01, descriptorDIR + "svm_attack_01.svm", descriptorDIR + "result_attack_01.txt");
	//std::cout << "attack 02" << std::endl;
	//trainTestSVM(filesAttack_00, filesAttack_02, descriptorDIR + "svm_attack_02.svm", descriptorDIR + "result_attack_02.txt");
	//std::cout << "attack 03" << std::endl;
	//trainTestSVM(filesAttack_00, filesAttack_03, descriptorDIR + "svm_attack_03.svm", descriptorDIR + "result_attack_03.txt");
	//std::cout << "attack 04" << std::endl;
	//trainTestSVM(filesAttack_00, filesAttack_04, descriptorDIR + "svm_attack_04.svm", descriptorDIR + "result_attack_04.txt");
	//std::cout << "attack 05" << std::endl;
	//trainTestSVM(filesAttack_00, filesAttack_05, descriptorDIR + "svm_attack_05.svm", descriptorDIR + "result_attack_05.txt");
	std::cout << "attack 06" << std::endl;
	trainTestSVM(filesAttack_00, filesAttack_06, descriptorDIR + "svm_attack_06.svm", descriptorDIR + "result_attack_06.txt");
	//std::cout << "all attacks" << std::endl;
	//trainTestSVM(filesAttack_00, files_No_Attack_00, descriptorDIR + "svm_attack_All.svm", descriptorDIR + "result_attack_All.txt");

	
	std::getchar();

	return 0;
}

