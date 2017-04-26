#include <string>
#include <vector>

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

class Util_SVM
{
public: static void filesDIR(std::vector<std::string> &sampleFileS, 
							int label, 
							cv::Mat &data, 
							cv::Mat &labelS);

public: static int balance(std::vector<std::string> &filesA, 
							std::vector<std::string> &filesB);


public: static bool groupTestTrain(const std::vector<std::string> &filesA,
								   const std::vector<std::string> &filesB,
									std::vector<std::string> &filesA_train,
									std::vector<std::string> &filesA_test,
									std::vector<std::string> &filesB_train,
									std::vector<std::string> &filesB_test);

public: static void Util_SVM::train(std::vector<std::string> &files_classA_train,
									std::vector<std::string> &files_classB_train,
									const std::string &svmFile);

public: static void Util_SVM::test(std::vector<std::string> &files_classA_test,
									std::vector<std::string> &files_classB_test,
									const std::string &svmFile,
									const std::string &resultFile);
};

