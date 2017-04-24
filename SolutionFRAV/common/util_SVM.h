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

public: static void balance(std::vector<std::string> &filesA, 
							std::vector<std::string> &filesB);
};

