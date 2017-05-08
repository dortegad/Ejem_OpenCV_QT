#include "CamF200.h"

#include "pxcprojection.h"
#include "util_faces.h"
#include "util_LBP_CV.h"

//---------------------------------------------------------------------------------------
CamF200::CamF200()
{
}

//---------------------------------------------------------------------------------------
CamF200::~CamF200()
{
}

//---------------------------------------------------------------------------------------
int CamF200::init()
{
	if (!Util_Faces::init())
		return 0;

	//svm_depth = cv::ml::SVM::load("svm_1_2_3_4_5_depth.svm");   //PROFUNDIDAD TODOS LOS ATAQUES (ENTREANDO ANTES)
	svm_depth = cv::ml::SVM::load("svm_1_depth.svm"); // PROFUNDIDAD ATAQUE 1 (ENTRENADO ANTES)

	svm_rgb = cv::ml::SVM::load("svm_attack_01.svm"); //RGB ATAQUE 1 (ENTRENADO_NUEVO)
	//svm_rgb = cv::ml::SVM::load("svm_1_2_3_4_5_rgb.svm"); //RGB TODOS LOS ATAQUES (ENTRENADO ANTES)
	//svm_rgb = cv::ml::SVM::load("svm_1_rgb.svm"); //RGB ATAQUE 1 (ENTRENADO ANTES)

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


	// Sets file recording or playback
	//PXCCaptureManager *cm = pp->QueryCaptureManager();
	//cm->SetFileName(cmdl.m_recordedFile, cmdl.m_bRecord);
	//if (cmdl.m_sdname)
	//cm->FilterByDeviceInfo(cmdl.m_sdname, 0, 0);
	
	int ancho = 640; //640  1920;
	int alto = 480; //480 1080;

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
cv::Mat CamF200::captureAdjustColor(PXCCapture::Device *device,
									const PXCCapture::Sample *sample)
{
	cv::Mat cvSample;
	if (sample->depth)
	{
		PXCProjection *projection = device->CreateProjection();

		PXCImage *imgDepthToColor = projection->CreateDepthImageMappedToColor(sample->depth, sample->color);
		cv::Mat *img_depth = new cv::Mat();
		this->convertPXCImageToOpenCVMat(imgDepthToColor, img_depth);

		//delete img_depth;
		cvSample = img_depth->clone();

		imgDepthToColor->Release();
		projection->Release();
	}

	return cvSample;
}

//---------------------------------------------------------------------------------------
cv::Mat CamF200::captureReal(PXCCapture::Device *device,
						  const PXCCapture::Sample *sample)
{
	cv::Mat cvSample;
	cv::Mat *img = new cv::Mat();
	this->convertPXCImageToOpenCVMat(sample->color, img);
	cvSample = img->clone();
	delete img;
	return cvSample;
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

	frameRGB = this->captureReal(device, sample);
	frameDepth = this->captureAdjustColor(device, sample);

	//Releases lock so pipeline can process next frame 
	pp->ReleaseFrame();
}

//---------------------------------------------------------------------------------------
int CamF200::isAttack()
{
	cv::Mat frameRGB;
	cv::Mat frameDepth;
	this->capture(frameRGB, frameDepth);
	//cv::imshow("frame RGB", frame);

	cv::Rect rectFace;
	if (Util_Faces::detectFace(frameRGB, rectFace))
	{
		cv::Mat imgFace = frameRGB(rectFace);
		cv::resize(imgFace, imgFace, cv::Size(100, 100));
		cv::imshow("face RGB", imgFace);
		cv::Mat_<double> featuresRGB;
		Util_LBP_CV::LBP_RGB(imgFace, featuresRGB);

		cv::Mat_<float> sampleRGB = featuresRGB;
		float result = svm_rgb->predict(sampleRGB, cv::noArray(), cv::ml::StatModel::RAW_OUTPUT);
		//int preditClass = svm_rgb->predict(sampleRGB, cv::noArray());
		//float confidence = 1.0 / (1.0 + exp(-result));
		//std::cout << "RGB" << result << " - " << confidence << " - " << preditClass << std::endl;
		if (result > 0.8)
			std::cout << "rgb ATTACK" << std::endl;
		else
			std::cout << "rgb BONA FIDE" << std::endl;


		cv::Mat imgFaceDepth = frameDepth(rectFace);
		cv::resize(imgFaceDepth, imgFaceDepth, cv::Size(100, 100));
		//this->imshowDepth("face depthImage", matFaceDepth, capture);
		cv::Mat_<double> featureDepth;
		Util_LBP_CV::LBP_Depth(imgFaceDepth, featureDepth);

		cv::Mat_<float> sampleDepth = featureDepth;
		result = svm_depth->predict(sampleDepth, cv::noArray(), cv::ml::StatModel::RAW_OUTPUT);
		//preditClass = svm_depth->predict(sampleDepth, cv::noArray());
		//confidence = 1.0 / (1.0 + exp(-result));
		//std::cout << "DEPTH" << result << " - " << confidence << " - " << preditClass << std::endl;
		if (result < -2.0)
			std::cout << "depth BONA FIDE" << std::endl;
		else
			std::cout << "depth ATTACK" << std::endl;
	}

	return 0;
}

//---------------------------------------------------------------------------------------
int CamF200::stop()
{
	pp->Release();
	return 0;
}
