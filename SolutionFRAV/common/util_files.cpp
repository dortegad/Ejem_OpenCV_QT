#include "util_files.h"
#include <iostream>


#include <Windows.h>

//------------------------------------------------------------------------------
int Util_Files::filesDIR(const std::string &dirPath, std::vector<std::string> &files)
{
	std::string dirFind = dirPath +  "*.des";
	std::wstring stemp(dirFind.begin(), dirFind.end());

	HANDLE hFind;
	WIN32_FIND_DATA data;

	hFind = FindFirstFile(stemp.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			std::wstring wfileName = data.cFileName;
			std::string fileName(wfileName.begin(), wfileName.end());
			std::string filePathName = dirPath + fileName;
			//std::cout << filePathName.c_str() << std::endl;
			files.push_back(filePathName);
		} while (FindNextFile(hFind, &data));
		FindClose(hFind);
	}
	
	return files.size();
}