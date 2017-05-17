#include "CamF200.h"

#include "pxcprojection.h"
#include "util_faces.h"
#include "util_LBP_CV.h"
#include <algorithm>

//---------------------------------------------------------------------------------------
CamF200::CamF200()
{
}

//---------------------------------------------------------------------------------------
CamF200::~CamF200()
{
}

//---------------------------------------------------------------------------------------
int CamF200::load()
{
	if (!Util_Faces::init())
		return -1;

	svm_depth_attack_01 = cv::ml::SVM::load(".\\SVM_LBP_DEPTH_NEW\\svm_attack_01.svm");
	svm_depth_attack_02 = cv::ml::SVM::load(".\\SVM_LBP_DEPTH_NEW\\svm_attack_02.svm");
	svm_depth_attack_03 = cv::ml::SVM::load(".\\SVM_LBP_DEPTH_NEW\\svm_attack_03.svm");
	svm_depth_attack_04 = cv::ml::SVM::load(".\\SVM_LBP_DEPTH_NEW\\svm_attack_04.svm");
	svm_depth_attack_05 = cv::ml::SVM::load(".\\SVM_LBP_DEPTH_NEW\\svm_attack_05.svm");

	//svm_rgb_attack_01 = cv::ml::SVM::load("svm_1_rgb.svm");

	svm_rgb_attack_01 = cv::ml::SVM::load(".\\SVM_LBP_RGB_NEW\\svm_attack_01.svm");
	svm_rgb_attack_02 = cv::ml::SVM::load(".\\SVM_LBP_RGB_NEW\\svm_attack_02.svm");
	svm_rgb_attack_03 = cv::ml::SVM::load(".\\SVM_LBP_RGB_NEW\\svm_attack_03.svm");
	svm_rgb_attack_04 = cv::ml::SVM::load(".\\SVM_LBP_RGB_NEW\\svm_attack_04.svm");
	svm_rgb_attack_05 = cv::ml::SVM::load(".\\SVM_LBP_RGB_NEW\\svm_attack_05.svm");

	return 0;
}

//---------------------------------------------------------------------------------------
int CamF200::init()
{
	// Creates an instance of the PXCSenseManager 
	pp = PXCSenseManager::CreateInstance();
	if (!pp) {
		wprintf_s(L"Unable to create the SenseManager\n");
		return NULL;
	}

	// Optional steps to send feedback to Intel Corporation to understand how often each SDK sample is used.
	PXCMetadata * md = pp->QuerySession()->QueryInstance<PXCMetadata>();
	if (md)
	{
		pxcCHAR sample_name[] = L"Camera Viewer";
		md->AttachBuffer(PXCSessionService::FEEDBACK_SAMPLE_INFO, (pxcBYTE*)sample_name, sizeof(sample_name));
	}


	pp->EnableStream(PXCCapture::STREAM_TYPE_COLOR, 1920, 1080);// , 30.0F);
	pp->EnableStream(PXCCapture::STREAM_TYPE_IR, 640, 480);// , 30.0F);
	pp->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 640, 480);// , 30.0F);

	if (pp == NULL)
		return -1;

	// Initializes the pipeline
	pxcStatus sts;
	sts = pp->Init();
	if (sts<PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to locate any video stream(s)\n");
		pp->Release();
		return sts;
	}

	// Reset all properties
	device = pp->QueryCaptureManager()->QueryDevice();
	device->ResetProperties(PXCCapture::STREAM_TYPE_ANY);

	return 0;
}

//---------------------------------------------------------------------------------------
void CamF200::convertPXCImageToOpenCVMat(PXCImage *inImg,
	cv::Mat *outImg)
{
	int cvDataType;
	int cvDataWidth;

	PXCImage::ImageData data;
	inImg->AcquireAccess(PXCImage::ACCESS_READ, &data);
	PXCImage::ImageInfo imgInfo = inImg->QueryInfo();

	switch (data.format) {
		/* STREAM_TYPE_COLOR */
	case PXCImage::PIXEL_FORMAT_YUY2: /* YUY2 image  */
	case PXCImage::PIXEL_FORMAT_NV12: /* NV12 image */
		//throw(0); // Not implemented
		inImg->ReleaseAccess(&data);
		inImg->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB24, &data);
		cvDataType = CV_8UC3;
		cvDataWidth = 3;
		break;
	case PXCImage::PIXEL_FORMAT_RGB32: /* BGRA layout on a little-endian machine */
		cvDataType = CV_8UC4;
		cvDataWidth = 4;
		break;
	case PXCImage::PIXEL_FORMAT_RGB24: /* BGR layout on a little-endian machine */
		cvDataType = CV_8UC3;
		cvDataWidth = 3;
		break;
	case PXCImage::PIXEL_FORMAT_Y8:  /* 8-Bit Gray Image, or IR 8-bit */
		cvDataType = CV_8U;
		cvDataWidth = 1;
		break;

		/* STREAM_TYPE_DEPTH */
	case PXCImage::PIXEL_FORMAT_DEPTH: /* 16-bit unsigned integer with precision mm. */
	case PXCImage::PIXEL_FORMAT_DEPTH_RAW: /* 16-bit unsigned integer with device specific precision (call device->QueryDepthUnit()) */
		//cvDataType = CV_16U;
		//cvDataWidth = 2;
		inImg->ReleaseAccess(&data);
		inImg->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_DEPTH_F32, &data);
		cvDataType = CV_32F;
		cvDataWidth = 4;
		break;
	case PXCImage::PIXEL_FORMAT_DEPTH_F32: /* 32-bit float-point with precision mm. */
		cvDataType = CV_32F;
		cvDataWidth = 4;
		break;

		/* STREAM_TYPE_IR */
	case PXCImage::PIXEL_FORMAT_Y16:          /* 16-Bit Gray Image */
		cvDataType = CV_16U;
		cvDataWidth = 2;
		break;
	case PXCImage::PIXEL_FORMAT_Y8_IR_RELATIVE:    /* Relative IR Image */
		cvDataType = CV_8U;
		cvDataWidth = 1;
		break;
	}

	// suppose that no other planes
	if (data.planes[1] != NULL)
		throw(0); // not implemented

	// suppose that no sub pixel padding needed
	if (data.pitches[0] % cvDataWidth != 0)
		throw(0); // not implemented

	outImg->create(imgInfo.height, data.pitches[0] / cvDataWidth, cvDataType);

	memcpy(outImg->data, data.planes[0], imgInfo.height*imgInfo.width*cvDataWidth*sizeof(pxcBYTE));

	inImg->ReleaseAccess(&data);
}


//---------------------------------------------------------------------------------------
void CamF200::capture(cv::Mat &frameRGB, cv::Mat &frameDepth)
{
	//Waits until new frame is available and locks it for application processing 
	pxcStatus sts;
	sts = pp->AcquireFrame(true);
	//std::cout << sts << std::endl;

	if (sts<PXC_STATUS_NO_ERROR) {
		if (sts == PXC_STATUS_STREAM_CONFIG_CHANGED) {
			wprintf_s(L"Stream configuration was changed, re-initilizing\n");
			pp->Close();
		}
		//return -1;
	}

	//Render streams, unless -noRender is selected
	const PXCCapture::Sample *sample = pp->QuerySample();

	//Capturamos la imagen RGB
	cv::Mat *img = new cv::Mat();
	this->convertPXCImageToOpenCVMat(sample->color, img);
	frameRGB = img->clone();
	delete img;

	//Capturamos la imagen Depth
	if (sample->depth)
	{
		PXCProjection *projection = device->CreateProjection();

		PXCImage *imgDepthToColor = projection->CreateDepthImageMappedToColor(sample->depth, sample->color);
		cv::Mat *img_depth = new cv::Mat();
		this->convertPXCImageToOpenCVMat(imgDepthToColor, img_depth);

		frameDepth = img_depth->clone();

		imgDepthToColor->Release();
		projection->Release();
		delete img_depth;
	}

	//Releases lock so pipeline can process next frame 
	pp->ReleaseFrame();
}


//---------------------------------------------------------------------------------------
float CamF200::evalue(cv::Ptr<cv::ml::SVM> svm, 
					  cv::Mat &features,
					  float umbral,
					  const std::string &msg)
{
	cv::Mat_<float> sample = features;
	float result = svm->predict(sample, cv::noArray(), cv::ml::StatModel::RAW_OUTPUT);
	int preditClass = svm->predict(sample, cv::noArray());
	float confidence = 1.0 / (1.0 + exp(-result));
	/*std::cout << msg << " : " << result << " - " << confidence << " - " << preditClass << std::endl;
	if (confidence > umbral)
		std::cout << msg << " = BONA FIDE" << std::endl;
	else
		std::cout << msg << " = ATTTACK" << std::endl;
	*/
	return confidence;
}

//---------------------------------------------------------------------------------------
float CamF200::isAttackFrame()
{
	cv::Mat frameRGB;
	cv::Mat frameDepth;
	this->capture(frameRGB, frameDepth);
	//cv::imshow("frame RGB", frame);

	cv::Rect rectFace;
	if (!Util_Faces::detectFace(frameRGB, rectFace))
		return 0.0;

	cv::Mat imgFace = frameRGB(rectFace);
	cv::resize(imgFace, imgFace, cv::Size(100, 100));
	//cv::cvtColor(imgFace, imgFace, CV_BGR2RGB);
	cv::imshow("face RGB", imgFace);
	cv::Mat featuresRGB;
	Util_LBP_CV::LBP_RGB(imgFace, featuresRGB);

	//float score_attack_01_rgb = this->evalue(svm_rgb_attack_01, featuresRGB, 0.45, "RGB Attack 1");
	float score_attack_01_rgb = this->evalue(svm_rgb_attack_01, featuresRGB, 0.6, "RGB Attack 1");
	float score_attack_02_rgb = this->evalue(svm_rgb_attack_02, featuresRGB, 0.6, "RGB Attack 2");
	float score_attack_03_rgb = this->evalue(svm_rgb_attack_03, featuresRGB, 0.8, "RGB Attack 3");
	float score_attack_04_rgb = this->evalue(svm_rgb_attack_04, featuresRGB, 0.8, "RGB Attack 4");
	float score_attack_05_rgb = this->evalue(svm_rgb_attack_05, featuresRGB, 0.8, "RGB Attack 5");
	//float score_rgb = (score_attack_01_rgb +
	//	score_attack_02_rgb +
	//	score_attack_03_rgb +
	//	score_attack_04_rgb +
	//	score_attack_05_rgb) / 5;
	//std::cout << "RGB = " << score_rgb;
	//if (score_rgb > 0.6)
	//	std::cout << " = BONA FIDE" << std::endl;
	//else
	//	std::cout << " = ATTACK" << std::endl;

	cv::Mat imgFaceDepth = frameDepth(rectFace);
	cv::resize(imgFaceDepth, imgFaceDepth, cv::Size(100, 100));
	cv::Mat_<double> featureDepth;
	Util_LBP_CV::LBP_Depth(imgFaceDepth, featureDepth);

	float score_attack_01_depth = this->evalue(svm_depth_attack_01, featureDepth, 0.7, "DEPTH Attack 1");
	float score_attack_02_depth = this->evalue(svm_depth_attack_02, featureDepth, 0.7, "DEPTH Attack 2");
	float score_attack_03_depth = this->evalue(svm_depth_attack_03, featureDepth, 0.7, "DEPTH Attack 3");
	float score_attack_04_depth = this->evalue(svm_depth_attack_04, featureDepth, 0.7, "DEPTH Attack 4");
	float score_attack_05_depth = this->evalue(svm_depth_attack_05, featureDepth, 0.7, "DEPTH Attack 5");
	//float score_depth = (score_attack_01_depth +
	//	score_attack_02_depth +
	//	score_attack_03_depth +
	//	score_attack_04_depth +
	//	score_attack_05_depth) / 5;
	//std::cout << "DEPTH = " << score_depth;
	//if (score_depth > 0.6)
	//	std::cout << " = BONA FIDE" << std::endl;
	//else
	//	std::cout << " = ATTACK" << std::endl;

	std::vector<float> scoresFusion;
	scoresFusion.push_back(score_attack_01_rgb);
	scoresFusion.push_back(score_attack_02_rgb);
	scoresFusion.push_back(score_attack_03_rgb);
	scoresFusion.push_back(score_attack_04_rgb);
	scoresFusion.push_back(score_attack_05_rgb);
	scoresFusion.push_back(score_attack_01_depth);
	scoresFusion.push_back(score_attack_02_depth);
	scoresFusion.push_back(score_attack_03_depth);
	scoresFusion.push_back(score_attack_04_depth);
	//scoresFusion.push_back(score_attack_05_depth);
	std::vector<float>::iterator itMin = std::min_element(scoresFusion.begin(), scoresFusion.end());
	float minScore = *itMin;
	std::cout << minScore;
	if (minScore > 0.28)
		std::cout << " = BONA FIDE" << std::endl;
	else
		std::cout << " = ATTACK" << std::endl;

	return minScore;
}

//---------------------------------------------------------------------------------------
int CamF200::isAttack()
{
	int numFrames = 3;

	std::vector<float> scoreFrames;
	int i = 0;
	while (i < numFrames)
	{
		std::cout << "frame " << i << std::endl;
		float result = isAttackFrame();
		cv::waitKey(15);

		if (result > 0.0)
		{
			scoreFrames.push_back(result);
			i++;
		}
	}

	std::vector<float>::iterator itMin = std::min_element(scoreFrames.begin(), scoreFrames.end());
	float minScore = *itMin;
	std::cout << "RESULT = " << minScore;
	if (minScore > 0.28)
		std::cout << " = BONA FIDE" << std::endl;
	else
		std::cout << " = ATTACK" << std::endl;

	return 0;
}

//---------------------------------------------------------------------------------------
int CamF200::stop()
{
	pp->Release();
	return 0;
}
