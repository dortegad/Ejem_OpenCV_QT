//Comentario
#include "visorRealSense.h"
#include "pxcsensemanager.h"
#include "pxcmetadata.h"
#include "util_cmdline.h"
#include "util_render.h"
#include <conio.h>
#include "qfiledialog.h"
#include "qmessagebox.h"

#include <iostream>
#include <fstream>
#include <winsock.h>
#include <windows.h>
#include <iostream>

#include "pxcprojection.h"

#pragma comment(lib, "Ws2_32.lib")

//--------------------------------------------------------------------------------------------------------------
const std::string VisorRealSense::WINNAME_RGB = "real image RGB";
const std::string VisorRealSense::WINNAME_Depth = "real image Depth";
const std::string VisorRealSense::WINNAME_IR = "real image IR";
const std::string VisorRealSense::WINNAME_FLIR = "real image FLIR";

//--------------------------------------------------------------------------------------------------------------
VisorRealSense::VisorRealSense(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

//--------------------------------------------------------------------------------------------------------------
VisorRealSense::~VisorRealSense()
{

}

//--------------------------------------------------------------------------------------------------------------
void VisorRealSense::showImg_1(const cv::Mat &imagen)
{
	if (ui.rBRealSizeImg_1->isChecked())
		cv::imshow(WINNAME_RGB, imagen);
	else
		cv::destroyWindow(WINNAME_RGB);

	cv::Mat imgMostrar;
	if (imagen.type() == CV_8UC1)
		cv::cvtColor(imagen, imgMostrar, cv::COLOR_GRAY2RGB);
	else
		cv::cvtColor(imagen, imgMostrar, cv::COLOR_BGR2RGB);

	QImage imagenQT = QImage((const unsigned char *)imgMostrar.data, imgMostrar.cols, imgMostrar.rows, imgMostrar.step, QImage::Format_RGB888);
	ui.lImg_1->setPixmap(QPixmap::fromImage(imagenQT));
	ui.lImg_1->setScaledContents(true);
	ui.lImg_1->repaint();
	qApp->processEvents();
	imgMostrar.release();
}

//--------------------------------------------------------------------------------------------------------------
void VisorRealSense::showImg_2(const cv::Mat &imagen)
{
	if (ui.rBRealSizeImg_2->isChecked())
		cv::imshow(WINNAME_Depth, imagen);
	else
		cv::destroyWindow(WINNAME_Depth);

	cv::Mat imgMostrar;
	if (imagen.type() == CV_8UC1)
		cv::cvtColor(imagen, imgMostrar, cv::COLOR_GRAY2RGB);
	else
		cv::cvtColor(imagen, imgMostrar, cv::COLOR_BGR2RGB);

	QImage imagenQT = QImage((const unsigned char *)imgMostrar.data, imgMostrar.cols, imgMostrar.rows, imgMostrar.step, QImage::Format_RGB888);
	ui.lImg_2->setPixmap(QPixmap::fromImage(imagenQT));
	ui.lImg_2->setScaledContents(true);
	ui.lImg_2->repaint();
	qApp->processEvents();
	imgMostrar.release();
}

//--------------------------------------------------------------------------------------------------------------
void VisorRealSense::showImg_3(const cv::Mat &imagen)
{
	if (ui.rBRealSizeImg_3->isChecked())
		cv::imshow(WINNAME_IR, imagen);
	else
		cv::destroyWindow(WINNAME_IR);

	cv::Mat imgMostrar;
	if (imagen.type() == CV_8UC1)
		cv::cvtColor(imagen, imgMostrar, cv::COLOR_GRAY2RGB);
	else
		cv::cvtColor(imagen, imgMostrar, cv::COLOR_BGR2RGB);

	QImage imagenQT = QImage((const unsigned char *)imgMostrar.data, imgMostrar.cols, imgMostrar.rows, imgMostrar.step, QImage::Format_RGB888);
	ui.lImg_3->setPixmap(QPixmap::fromImage(imagenQT));
	ui.lImg_3->setScaledContents(true);
	ui.lImg_3->repaint();
	qApp->processEvents();
	imgMostrar.release();
}

//--------------------------------------------------------------------------------------------------------------
void VisorRealSense::ConvertPXCImageToOpenCVMat(PXCImage *inImg,
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

//--------------------------------------------------------------------------------------------------------------
void VisorRealSense::createMapDepth(PXCProjection *projection,
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

//--------------------------------------------------------------------------------------------------------------
bool VisorRealSense::showSampleAdjustColor(PXCCapture::Device *device,
											const PXCCapture::Sample *sample)
{
	 /*
	 if (sample) {
		 if (sample->depth && !renderd.RenderFrame(sample->depth)) break;
		 if (sample->color && !renderc.RenderFrame(sample->color)) break;
		 if (sample->ir    && !renderi.RenderFrame(sample->ir))    break;
		 if (sample->right    && !renderr.RenderFrame(sample->right)) break;
		 if (sample->left    && !renderl.RenderFrame(sample->left)) break;
	 }
	 */

	 if (sample->color)
	 {
		 //PXCProjection *projection = device->CreateProjection();
		 //PXCImage *imgColorToDepth = projection->CreateColorImageMappedToDepth(sample->depth, sample->color);

		 cv::Mat *img = new cv::Mat();
		 ConvertPXCImageToOpenCVMat(sample->color, img);
		 this->showImg_1(*img);
		 delete img;

		 //imgColorToDepth->Release();
		 //projection->Release();
	 }

	 if (sample->depth)
	 {
		 PXCProjection *projection = device->CreateProjection();

		 //Se calcula la proyeccion para los puntos IR
		 cv::Mat *img_ir = new cv::Mat();
		 ConvertPXCImageToOpenCVMat(sample->ir, img_ir);
		 cv::Mat_<uchar> new_img_ir = cv::Mat::zeros(img_ir->rows, img_ir->cols, img_ir->type());

		 std::vector<PXCPoint3DF32> mapDepth;
		 createMapDepth(projection,
			 sample->depth,
			 mapDepth);
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
			 this->showImg_3(new_img_ir);
			 //cv::imshow("prueba", new_img_ir);
		 }
		 
		 PXCImage *imgDepthToColor = projection->CreateDepthImageMappedToColor(sample->depth, sample->color);
		 cv::Mat *img_depth = new cv::Mat();
		 ConvertPXCImageToOpenCVMat(imgDepthToColor, img_depth);

		 double min;
		 double max;
		 cv::minMaxIdx(*img_depth, &min, &max);
		 cv::Mat adjMap;
		 float scale = 255 / (max - min);
		 img_depth->convertTo(adjMap, CV_8UC1, scale, -min*scale);
		 cv::equalizeHist(adjMap, adjMap);

		 /*
		 cv::Mat recorte = (*img_depth)(cv::Rect(100, 100, 300, 300)).clone();
		 recorte.convertTo(recorte, CV_8UC1, scale, -min*scale);
		 cv::equalizeHist(recorte, recorte);
		 cv::imshow("prueba", recorte);
		 */

		 /*
		 PXCImage::ImageData colorData;
		 sample->depth->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_DEPTH_F32, &colorData);
		 int height = sample->depth->QueryInfo().height;
		 int width = sample->depth->QueryInfo().width;
		 // image planes are in data.planes[0-3] with pitch data.pitches[0-3]
		 QImage colorImage(colorData.planes[0], width, height, QImage::Format_RGB32);
		 sample->color->ReleaseAccess(&colorData);
		 ui.lImg_2->setPixmap(QPixmap::fromImage(colorImage));
		 ui.lImg_2->setScaledContents(true);
		 ui.lImg_2->repaint();
		 */

		 this->showImg_2(adjMap);
		 delete img_depth;

		 imgDepthToColor->Release();
		 projection->Release();
	 }

	 /*
	 if (sample->ir)
	 {
		 cv::Mat *img_ir = new cv::Mat();
		 ConvertPXCImageToOpenCVMat(sample->ir, img_ir);
		 this->showImg_3(*img_ir);
		 delete img_ir;
	 }
	 */


	 /*
	 const PXCCapture::Sample *sample = pp->QuerySample();
	 PXCImage::ImageData colorData;
	 sample->color->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB32, &colorData);
	 int height = sample->color->QueryInfo().height;
	 int width = sample->color->QueryInfo().width;
	 // image planes are in data.planes[0-3] with pitch data.pitches[0-3]
	 QImage colorImage(colorData.planes[0], width, height, QImage::Format_RGB32);
	 sample->color->ReleaseAccess(&colorData);
	 ui.lImg->setPixmap(QPixmap::fromImage(colorImage));
	 ui.lImg->setScaledContents(true);
	 ui.lImg->repaint();
	 */

	 return true;
}

//--------------------------------------------------------------------------------------------------------------
bool VisorRealSense::showSampleReal(PXCCapture::Device *device,
									const PXCCapture::Sample *sample)
{
	if (sample->color)
	{
		cv::Mat *img = new cv::Mat();
		ConvertPXCImageToOpenCVMat(sample->color, img);
		this->showImg_1(*img);
		delete img;
	}

	if (sample->depth)
	{
		//Se calcula la proyeccion para los puntos IR
		cv::Mat *img_depth = new cv::Mat();
		ConvertPXCImageToOpenCVMat(sample->depth, img_depth);

		double min;
		double max;
		cv::minMaxIdx(*img_depth, &min, &max);
		cv::Mat adjMap;
		float scale = 255 / (max - min);
		img_depth->convertTo(adjMap, CV_8UC1, scale, -min*scale);
		cv::equalizeHist(adjMap, adjMap);

		this->showImg_2(adjMap);
		delete img_depth;
	}

	if (sample->ir)
	{
		cv::Mat *img_ir = new cv::Mat();
		ConvertPXCImageToOpenCVMat(sample->ir, img_ir);
		this->showImg_3(*img_ir);
		delete img_ir;
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------------
bool VisorRealSense::showSampleAdjustDepth(PXCCapture::Device *device,
										  const PXCCapture::Sample *sample)
{
	if (sample->color)
	{
		PXCProjection *projection = device->CreateProjection();
		PXCImage *imgColorToDepth = projection->CreateColorImageMappedToDepth(sample->depth, sample->color);

		cv::Mat *img = new cv::Mat();
		ConvertPXCImageToOpenCVMat(imgColorToDepth, img);
		this->showImg_1(*img);
		delete img;

		imgColorToDepth->Release();
		projection->Release();
	}

	if (sample->depth)
	{
		//Se calcula la proyeccion para los puntos IR
		cv::Mat *img_depth = new cv::Mat();
		ConvertPXCImageToOpenCVMat(sample->depth, img_depth);

		double min;
		double max;
		cv::minMaxIdx(*img_depth, &min, &max);
		cv::Mat adjMap;
		float scale = 255 / (max - min);
		img_depth->convertTo(adjMap, CV_8UC1, scale, -min*scale);
		cv::equalizeHist(adjMap, adjMap);

		this->showImg_2(adjMap);
		delete img_depth;
	}

	if (sample->ir)
	{
		cv::Mat *img_ir = new cv::Mat();
		ConvertPXCImageToOpenCVMat(sample->ir, img_ir);
		this->showImg_3(*img_ir);
		delete img_ir;
	}

	return true;
}

//--------------------------------------------------------------------------------------------------------------
PXCSenseManager * VisorRealSense::setup()
{
	// Creates an instance of the PXCSenseManager 
	PXCSenseManager *pp = PXCSenseManager::CreateInstance();
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

	pp->EnableStream(PXCCapture::STREAM_TYPE_COLOR, 640, 480, 30.0F);
	pp->EnableStream(PXCCapture::STREAM_TYPE_IR, 640, 480, 30.0F);
	pp->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, 640, 480, 30.0F);

	return pp;
}

//--------------------------------------------------------------------------------------------------------------
int VisorRealSense::stream(PXCSenseManager *pp)
{
	stopStream = false;

	// Initializes the pipeline
	pxcStatus sts;
	sts = pp->Init();
	if (sts<PXC_STATUS_NO_ERROR) {
		wprintf_s(L"Failed to locate any video stream(s)\n");
		pp->Release();
		return sts;
	}

	// Reset all properties
	PXCCapture::Device *device = pp->QueryCaptureManager()->QueryDevice();
	device->ResetProperties(PXCCapture::STREAM_TYPE_ANY);

	////Set mirror mode
	//if (cmdl.m_bMirror)
	//device->SetMirrorMode(PXCCapture::Device::MirrorMode::MIRROR_MODE_HORIZONTAL);
	//else
	//device->SetMirrorMode(PXCCapture::Device::MirrorMode::MIRROR_MODE_DISABLED);

	//Stream Data		
	while (!stopStream) //(int nframes = 0; nframes<cmdl.m_nframes; nframes++) {
	{
		//Waits until new frame is available and locks it for application processing 
		sts = pp->AcquireFrame(true);		
		//std::cout << sts << std::endl;

		if (sts<PXC_STATUS_NO_ERROR) {
			if (sts == PXC_STATUS_STREAM_CONFIG_CHANGED) {
				wprintf_s(L"Stream configuration was changed, re-initilizing\n");
				pp->Close();
			}
			break;
		}

		//Render streams, unless -noRender is selected 
		const PXCCapture::Sample *sample = pp->QuerySample();
		if (ui.rBViewReal->isChecked())
			showSampleReal(device, sample);
		else if (ui.rBViewAdjustDepth->isChecked())
			showSampleAdjustDepth(device, sample);
		else if (ui.rBViewAdjustRGB->isChecked())
			showSampleAdjustColor(device, sample);

		qApp->processEvents();

		//Releases lock so pipeline can process next frame 
		pp->ReleaseFrame();

		if (_kbhit()) { // Break loop
			int c = _getch() & 255;
			if (c == 27 || c == 'q' || c == 'Q') break; // ESC|q|Q for Exit
		}
	}

	wprintf_s(L"Exiting\n");

	return 0;
}

//--------------------------------------------------------------------------------------------------------------
int VisorRealSense::view()
{
	PXCSenseManager *pp = setup();
	if (pp == NULL)
		return -1;

	stream(pp);

	// Clean Up
	pp->Release();
	return 0;
}

//--------------------------------------------------------------------------------------------------------------
int VisorRealSense::record()
{
	PXCSenseManager *pp = setup();
	if (pp == NULL)
		return -1;

	//return (access(ui.lEfileName->text().toStdString().c_str(), F_OK) != -1);
	//DWORD       fileAttr;
	//fileAttr = GetFileAttributes(ui.lEfileName->text().toStdWString().c_str());
	//if (0xFFFFFFFF != fileAttr)
	//{
	//	QMessageBox messageBox(this);
	//	messageBox.setText("File exist. Override? ");
	//	messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	//	messageBox.setDefaultButton(QMessageBox::Cancel);
	//	int ret = messageBox.exec();
	//	if (ret == QMessageBox::Cancel)
	//	{
	//		pp->Release();
	//		return 0;
	//	}
	//	remove(ui.lEfileName->text().toStdString().c_str());
	//}

	// Set realtime=true and pause=false
	pxcCHAR *fileName = (pxcCHAR *)ui.lEfileName->text().unicode();
	pp->QueryCaptureManager()->SetFileName(fileName, true);
	pp->QueryCaptureManager()->SetRealtime(false);
	pp->QueryCaptureManager()->SetPause(false);

	stream(pp);

	// Clean Up
	pp->Release();
	return 0;
}

 //--------------------------------------------------------------------------------------------------------------
int VisorRealSense::play() 
{
	PXCSenseManager *pp = setup();
	if (pp == NULL)
		return -1;

	// Set realtime=true and pause=false
	pxcCHAR *fileName = (pxcCHAR *)ui.lEfileName->text().unicode();
	pp->QueryCaptureManager()->SetFileName(fileName, false);
	pp->QueryCaptureManager()->SetRealtime(false);
	pp->QueryCaptureManager()->SetPause(false);

	stream(pp);

	// Clean Up
	pp->Release();
	return 0;
}

//--------------------------------------------------------------------------------------------------------------
void VisorRealSense::stop()
{
	stopStream = true;
}

//--------------------------------------------------------------------------------------------------------------
void VisorRealSense::selectFile()
{
	QString selectedFilter;
	QString dirName = QFileDialog::getSaveFileName(this, tr("File"),
														ui.lEfileName->text(),
														tr("Images (*.rssdk)"),
														&selectedFilter,
														QFileDialog::DontConfirmOverwrite);
	if (dirName == "")
		return;
	ui.lEfileName->setText(dirName);
}



