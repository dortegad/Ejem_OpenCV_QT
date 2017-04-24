#include "util_fravAttack.h"
#include <iostream>


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