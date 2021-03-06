#include "util_files.h"
#include <iostream>


#include <Windows.h>
//cambiado
//------------------------------------------------------------------------------
int Util_Files::filesDIR(const std::string &dir, std::vector<std::string> &files, const std::string &extension)
{
	std::string dirPath = dir;
	if (dirPath[dirPath.length()] != '\\')
		dirPath = dirPath + "\\";
	
	std::string dirFind = dirPath +  extension;
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

//------------------------------------------------------------------------------
bool Util_Files::createDIR(const std::string &dirPath)
{
	return CreateDirectoryA(dirPath.c_str(),NULL);
}

//-----------------------------------------------------------------------------
std::string Util_Files::fileName(const std::string &dirPath)
{
	std::string fileName = dirPath.substr(dirPath.find_last_of("\\") + 1, dirPath.size() - dirPath.find_last_of("\\") - 1 - 4);
	return fileName;
}