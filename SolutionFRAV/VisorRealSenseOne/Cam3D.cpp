#include "Cam3D.h"

#include "pxcprojection.h"

//---------------------------------------------------------------------------------------
Cam3D::Cam3D()
{
}

//---------------------------------------------------------------------------------------
Cam3D::~Cam3D()
{
}

//---------------------------------------------------------------------------------------
int Cam3D::init()
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

	return 0;
}

//---------------------------------------------------------------------------------------
void Cam3D::convertPXCImageToOpenCVMat(PXCImage *inImg,
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
void Cam3D::createMapDepth(PXCProjection *projection,
							PXCImage *depth,
							std::vector<PXCPoint3DF32> &mapDepth)
{
	PXCImage::ImageData ddata;
	if (PXC_STATUS_NO_ERROR > depth->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_DEPTH, &ddata))
		return;

	float dwidth = (float)depth->QueryInfo().width;
	float dheight = (float)depth->QueryInfo().height;
	int imgSize = dwidth*dheight;

	PXCPointF32 *uvmap = new PXCPointF32[imgSize];
	projection->QueryInvUVMap(depth, uvmap);
	for (int i = 0; i < imgSize; i++)
	{
		if (uvmap[i].x < 0)
			continue;
		PXCPoint3DF32 point;
		point.x = uvmap[i].x * dwidth;
		point.y = uvmap[i].y * dheight;
		if ((point.x < dwidth) && (point.y < dheight) && (point.x > 0) && (point.y > 0))
		{
			//std::cout << (int)point.x << " - " << (int)point.y << std::endl;
			point.z = (float)(((pxcI16*)(ddata.planes[0] + (int)point.y * ddata.pitches[0]))[(int)point.x]);
			mapDepth.push_back(point);
		}
	}
	depth->ReleaseAccess(&ddata);
	delete uvmap;

	/*
	int dsize = dcords.size();
	PXCPoint3DF32 invP3D = { -1.f, -1.f, 0.f };
	std::vector<PXCPoint3DF32> dcordsAbs(dsize, invP3D);


	for (int i = 0; i < dsize; i++) {
	if (dcords[i].x < 0)
	continue;
	dcordsAbs[i].x = dcords[i].x * dwidth;
	dcordsAbs[i].y = dcords[i].y * dheight;
	dcordsAbs[i].z = (float)(((pxcI16*)(ddata.planes[0] + (int)dcordsAbs[i].y * ddata.pitches[0]))[(int)dcordsAbs[i].x]);
	}
	depth->ReleaseAccess(&ddata);
	mapDepth = dcordsAbs;
	*/
}

//---------------------------------------------------------------------------------------
cv::Mat Cam3D::captureAdjustColor(PXCCapture::Device *device,
									const PXCCapture::Sample *sample,
									ImgType imgType)
{
	//if (sample) {
	//if (sample->depth && !renderd.RenderFrame(sample->depth)) break;
	//if (sample->color && !renderc.RenderFrame(sample->color)) break;
	//if (sample->ir    && !renderi.RenderFrame(sample->ir))    break;
	//if (sample->right    && !renderr.RenderFrame(sample->right)) break;
	//if (sample->left    && !renderl.RenderFrame(sample->left)) break;
	//}

	cv::Mat cvSample;
	if (imgType == RGB)
	{
		if (sample->color)
		{
			//PXCProjection *projection = device->CreateProjection();
			//PXCImage *imgColorToDepth = projection->CreateColorImageMappedToDepth(sample->depth, sample->color);

			cv::Mat *img = new cv::Mat();
			this->convertPXCImageToOpenCVMat(sample->color, img);
			cvSample = img->clone();
			delete img;

			//imgColorToDepth->Release();
			//projection->Release();
		}
	}
	else
	{
		if (sample->depth)
		{
			PXCProjection *projection = device->CreateProjection();

			/*
			//Se calcula la proyeccion para los puntos IR
			cv::Mat *img_ir = new cv::Mat();
			this->convertPXCImageToOpenCVMat(sample->ir, img_ir);
			cv::Mat_<uchar> new_img_ir = cv::Mat::zeros(img_ir->rows, img_ir->cols, img_ir->type());

			std::vector<PXCPoint3DF32> mapDepth;
			this->createMapDepth(projection, sample->depth, mapDepth);
			if (mapDepth.size() > 0)
			{
				std::vector<PXCPointF32> ccords;
				PXCPointF32 invP = { -1.f, -1.f };
				ccords.resize(mapDepth.size(), invP);
				projection->MapDepthToColor(mapDepth.size(), &mapDepth[0], &ccords[0]);

				for (unsigned int i = 0; i < ccords.size(); i++)
				{
					if ((ccords[i].y > 0) && (ccords[i].x > 0) && (ccords[i].y < img_ir->rows) && (ccords[i].x < img_ir->cols))
						new_img_ir(ccords[i].y, ccords[i].x) = img_ir->at<uchar>(mapDepth[i].y, mapDepth[i].x);
				}
			}
			*/

			PXCImage *imgDepthToColor = projection->CreateDepthImageMappedToColor(sample->depth, sample->color);
			cv::Mat *img_depth = new cv::Mat();
			this->convertPXCImageToOpenCVMat(imgDepthToColor, img_depth);

			double min;
			double max;
			cv::minMaxIdx(*img_depth, &min, &max);
			cv::Mat adjMap;
			float scale = 255 / (max - min);
			img_depth->convertTo(adjMap, CV_8UC1, scale, -min*scale);
			cv::equalizeHist(adjMap, adjMap);

			
			//cv::Mat recorte = (*img_depth)(cv::Rect(100, 100, 300, 300)).clone();
			//recorte.convertTo(recorte, CV_8UC1, scale, -min*scale);
			//cv::equalizeHist(recorte, recorte);
			//cv::imshow("prueba", recorte);

			
			//PXCImage::ImageData colorData;
			//sample->depth->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_DEPTH_F32, &colorData);
			//int height = sample->depth->QueryInfo().height;
			//int width = sample->depth->QueryInfo().width;
			// image planes are in data.planes[0-3] with pitch data.pitches[0-3]
			//QImage colorImage(colorData.planes[0], width, height, QImage::Format_RGB32);
			//sample->color->ReleaseAccess(&colorData);
			//ui.lImg_2->setPixmap(QPixmap::fromImage(colorImage));
			//ui.lImg_2->setScaledContents(true);
			//ui.lImg_2->repaint();

			if (imgType == DEPTH)
				cvSample = adjMap.clone();
			//else
			//	cvSample = new_img_ir.clone();

			delete img_depth;

			imgDepthToColor->Release();
			projection->Release();
		}
	}

	//if (sample->ir)
	//{
	//cv::Mat *img_ir = new cv::Mat();
	///ConvertPXCImageToOpenCVMat(sample->ir, img_ir);
	//this->showImg_3(*img_ir);
	//delete img_ir;
	//}


	
	//const PXCCapture::Sample *sample = pp->QuerySample();
	//PXCImage::ImageData colorData;
	//sample->color->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB32, &colorData);
	//int height = sample->color->QueryInfo().height;
	//int width = sample->color->QueryInfo().width;
	// image planes are in data.planes[0-3] with pitch data.pitches[0-3]
	//QImage colorImage(colorData.planes[0], width, height, QImage::Format_RGB32);
	//sample->color->ReleaseAccess(&colorData);
	//ui.lImg->setPixmap(QPixmap::fromImage(colorImage));
	//ui.lImg->setScaledContents(true);
	//ui.lImg->repaint();

	return cvSample;
}


//---------------------------------------------------------------------------------------
cv::Mat Cam3D::captureReal(PXCCapture::Device *device,
						  const PXCCapture::Sample *sample,
						  ImgType imgType)
{
	cv::Mat cvSample;
	if (imgType == RGB)
	{
		if (sample->color)
		{
			cv::Mat *img = new cv::Mat();
			this->convertPXCImageToOpenCVMat(sample->color, img);
			cvSample = img->clone();
			delete img;
		}
	}
	else if (imgType == DEPTH)
	{
		if (sample->depth)
		{
			//Se calcula la proyeccion para los puntos IR
			cv::Mat *img_depth = new cv::Mat();
			this->convertPXCImageToOpenCVMat(sample->depth, img_depth);

			double min;
			double max;
			cv::minMaxIdx(*img_depth, &min, &max);
			cv::Mat adjMap;
			float scale = 255 / (max - min);
			img_depth->convertTo(adjMap, CV_8UC1, scale, -min*scale);
			cv::equalizeHist(adjMap, adjMap);

			cvSample = adjMap.clone();
			delete img_depth;
		}
	}
	else
	{
		if (sample->ir)
		{
			cv::Mat *img_ir = new cv::Mat();
			this->convertPXCImageToOpenCVMat(sample->ir, img_ir);
			cvSample = img_ir->clone();
			delete img_ir;
		}
	}
	return cvSample;
}

//---------------------------------------------------------------------------------------
cv::Mat Cam3D::captureAdjustDepth(PXCCapture::Device *device,
								const PXCCapture::Sample *sample,
								ImgType imgType)
{
	cv::Mat cvSample;
	if (imgType == RGB)
	{
		if (sample->color)
		{
			PXCProjection *projection = device->CreateProjection();
			PXCImage *imgColorToDepth = projection->CreateColorImageMappedToDepth(sample->depth, sample->color);

			cv::Mat *img = new cv::Mat();
			this->convertPXCImageToOpenCVMat(imgColorToDepth, img);
			//this->showImg_1(*img);
			cvSample = img->clone();
			delete img;

			imgColorToDepth->Release();
			projection->Release();
		}
	}
	else if (imgType == DEPTH)
	{
		if (sample->depth)
		{
			//Se calcula la proyeccion para los puntos IR
			cv::Mat *img_depth = new cv::Mat();
			this->convertPXCImageToOpenCVMat(sample->depth, img_depth);

			double min;
			double max;
			cv::minMaxIdx(*img_depth, &min, &max);
			cv::Mat adjMap;
			float scale = 255 / (max - min);
			img_depth->convertTo(adjMap, CV_8UC1, scale, -min*scale);
			cv::equalizeHist(adjMap, adjMap);

			//this->showImg_1(adjMap);
			cvSample = img_depth->clone();
			delete img_depth;
		}
	}
	else
	{
		if (sample->ir)
		{
			cv::Mat *img_ir = new cv::Mat();
			this->convertPXCImageToOpenCVMat(sample->ir, img_ir);
			//this->showImg_1(*img_ir);
			cvSample = img_ir->clone();
			delete img_ir;
		}
	}

	return cvSample;
}


//---------------------------------------------------------------------------------------
cv::Mat Cam3D::capture(ImgType imgType,
				   ImgAdjustType imgAdjustType)
{
	// Reset all properties
	PXCCapture::Device *device = pp->QueryCaptureManager()->QueryDevice();
	device->ResetProperties(PXCCapture::STREAM_TYPE_ANY);

	////Set mirror mode
	//if (cmdl.m_bMirror)
	//device->SetMirrorMode(PXCCapture::Device::MirrorMode::MIRROR_MODE_HORIZONTAL);
	//else
	//device->SetMirrorMode(PXCCapture::Device::MirrorMode::MIRROR_MODE_DISABLED);


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

	cv::Mat cvSample;
	if (imgAdjustType == REAL)
		cvSample = this->captureReal(device, sample, imgType);
	else if (imgAdjustType == ADJUST_DEPTH)
		cvSample = this->captureAdjustDepth(device, sample, imgType);
	else if (imgAdjustType == ADJUST_RGB)
		cvSample = this->captureAdjustColor(device, sample, imgType);

	//Releases lock so pipeline can process next frame 
	pp->ReleaseFrame();

	wprintf_s(L"Exiting\n");

	return cvSample;
}

//---------------------------------------------------------------------------------------
int Cam3D::isAttack()
{
	this->capture();

	return 0;
}

//---------------------------------------------------------------------------------------
int Cam3D::stop()
{
	pp->Release();
	return 0;
}
