#include <string>
#include <vector>

class Util_FravAttack
{
public: static int filesAttack(std::vector<std::string> &files,
							const std::string &attack,
							std::vector<std::string> &filesAttack);

public: static int filesNoAttack(std::vector<std::string> &files,
								const std::string &attack,
								std::vector<std::string> &filesAttack);

public: static int readSamplesFiles(const std::string &fileImgs,
									std::vector<std::string> &filesDepth,
									std::vector<std::string> &filesRGB,
									std::vector<std::string> &filesIR);

public: static void infoFileUser(const std::string &file, std::string &user);
public: static void infoFile(const std::string &file,
								std::string &user,
								std::string &attack,
								std::string &frame);

public: static int readSamplesFiles(const std::string &fileImgs,
									std::vector<std::string> &files);
};

