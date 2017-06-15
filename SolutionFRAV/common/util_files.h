#include <string>
#include <vector>

class Util_Files
{
public: static int filesDIR(const std::string &dirPath, std::vector<std::string> &files, const std::string &extension);
public: static bool createDIR(const std::string &dirPath);
};

