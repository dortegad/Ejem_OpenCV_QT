#include "util_fravAttack.h"

#include <iostream>
#include <fstream>
#include <sstream>


//------------------------------------------------------------------------------
int Util_FravAttack::filesAttack(std::vector<std::string> &files, 
								const std::string &attack, 
								std::vector<std::string> &filesAttack)
{
	std::string attackString = "_" + attack + "_";
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		std::string file = *it;
		std::size_t found = file.find(attackString);
		if (found != std::string::npos)
		{
			std::cout << file.c_str() << std::endl;
			filesAttack.push_back(file);
		}
	}
	
	return filesAttack.size();
}

//------------------------------------------------------------------------------
int Util_FravAttack::filesNoAttack(std::vector<std::string> &files,
									const std::string &attack,
									std::vector<std::string> &filesAttack)
{
	std::string attackString = "_" + attack + "_";
	for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
	{
		std::string file = *it;
		std::size_t found = file.find(attackString);
		if (found == std::string::npos)
		{
			std::cout << file.c_str() << std::endl;
			filesAttack.push_back(file);
		}
	}

	return filesAttack.size();
}

//------------------------------------------------------------------------------
int Util_FravAttack::readSamplesFiles(const std::string &fileImgs,
										std::vector<std::string> &filesDepth,
										std::vector<std::string> &filesRGB,
										std::vector<std::string> &filesIR)
{
	std::ifstream imgsFile;
	imgsFile.open(fileImgs.c_str());

	filesDepth.clear();
	filesRGB.clear();
	filesIR.clear();

	while (imgsFile.good())
	{
		std::string sSamples;
		if (getline(imgsFile, sSamples))
		{
			std::stringstream line;
			line << sSamples.c_str();

			std::string fileDepth;
			getline(line, fileDepth, ';');
			std::string fileRGB;
			getline(line, fileRGB, ';');
			std::string fileIR;
			getline(line, fileIR, ';');

			//std::cout << fileDepth.c_str() << std::endl;
			//std::cout << fileRGB.c_str() << std::endl;
			//std::cout << fileIR.c_str() << std::endl;
			//std::cout << std::endl;

			filesDepth.push_back(fileDepth);
			filesRGB.push_back(fileRGB);
			filesIR.push_back(fileIR);
		}
	}

	imgsFile.close();

	return filesDepth.size();
}

//------------------------------------------------------------------------------
void Util_FravAttack::infoFile(const std::string &file,
								std::string &user,
								std::string &attack,
								std::string &frame)
{
	user = file.substr(file.find("USER_")+5, 3);
	attack = "00";
	if (file.find("attack_") != std::string::npos)
		attack = file.substr(file.find("attack_")+7, 2);
	frame = file.substr(file.find("frame_")+6, 2);

	//std::cout << user << " - " << attack << " - " << frame << std::endl;
}