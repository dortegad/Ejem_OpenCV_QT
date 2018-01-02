#include <string>
#include <vector>
//cambiado
class Util_Files
{
public: static int filesDIR(const std::string &dirPath, std::vector<std::string> &files, const std::string &extension);
public: static bool createDIR(const std::string &dirPath);
public: static std::string Util_Files::fileName(const std::string &dirPath);
};

