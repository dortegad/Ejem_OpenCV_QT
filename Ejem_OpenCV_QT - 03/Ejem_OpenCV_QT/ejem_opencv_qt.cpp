//Comentario
#include "ejem_opencv_qt.h"
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
const std::string Ejem_OpenCV_QT::WINNAME_RGB = "real image RGB";
const std::string Ejem_OpenCV_QT::WINNAME_Depth = "real image Depth";
const std::string Ejem_OpenCV_QT::WINNAME_IR = "real image IR";
const std::string Ejem_OpenCV_QT::WINNAME_FLIR = "real image FLIR";

//--------------------------------------------------------------------------------------------------------------
Ejem_OpenCV_QT::Ejem_OpenCV_QT(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

//--------------------------------------------------------------------------------------------------------------
Ejem_OpenCV_QT::~Ejem_OpenCV_QT()
{

}

//--------------------------------------------------------------------------------------------------------------
void Ejem_OpenCV_QT::showImg_1(const cv::Mat &imagen)
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
void Ejem_OpenCV_QT::showImg_2(const cv::Mat &imagen)
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
void Ejem_OpenCV_QT::showImg_3(const cv::Mat &imagen)
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
void Ejem_OpenCV_QT::showImg_4(const cv::Mat &imagen)
{
	if (ui.rBRealSizeImg_4->isChecked())
		cv::imshow(WINNAME_FLIR, imagen);
	else
		cv::destroyWindow(WINNAME_FLIR);

	cv::Mat imgMostrar;
	if (imagen.type() == CV_8UC1)
		cv::cvtColor(imagen, imgMostrar, cv::COLOR_GRAY2RGB);
	else
		cv::cvtColor(imagen, imgMostrar, cv::COLOR_BGR2RGB);

	QImage imagenQT = QImage((const unsigned char *)imgMostrar.data, imgMostrar.cols, imgMostrar.rows, imgMostrar.step, QImage::Format_RGB888);
	ui.lImg_4->setPixmap(QPixmap::fromImage(imagenQT));
	ui.lImg_4->setScaledContents(true);
	ui.lImg_4->repaint();
	qApp->processEvents();
	imgMostrar.release();
}

//--------------------------------------------------------------------------------------------------------------
void Ejem_OpenCV_QT::ConvertPXCImageToOpenCVMat(PXCImage *inImg,
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
void Ejem_OpenCV_QT::createMapDepth(PXCProjection *projection,
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
	for (unsigned int i = 0; i < imgSize; i++)
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
bool Ejem_OpenCV_QT::showSampleAdjustColor(PXCCapture::Device *device,
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
bool Ejem_OpenCV_QT::showSampleReal(PXCCapture::Device *device,
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
bool Ejem_OpenCV_QT::showSampleAdjustDepth(PXCCapture::Device *device,
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
PXCSenseManager * Ejem_OpenCV_QT::setup()
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
std::list <std::string> ficheros;
bool hiloTermica = false;
SOCKET sock;
struct sockaddr_in local;

//--------------------------------------------------------------------------------------------------------------
int openSocket()
{
	WSADATA wsa;

	//char Buffer[1024];
	char nombre_archivo[20];
	//Inicializamos
	WSAStartup(MAKEWORD(2, 0), &wsa);
	//Creamos el socket
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == (SOCKET)-1)
	{
		std::cout << "error en el socket" << std::endl;
		return -1;
	}
	//defnimos direcci&#243;n por defecto, ipv4 y el puerto 9999
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(9999);
	//asociamos el socket al puerto
	if (bind(sock, (SOCKADDR*)&local, sizeof(local)) == -1)
	{
		std::cout << "error en el bind" << std::endl;
		return -1;
	}
	//ponemos el socket a la escucha
	if (listen(sock, 1) == -1)
	{
		//int listen(int sockfd, int backlog), backlog pone en la cola 
		//los clientes conectados, en este caso es 1
		std::cout << "error en el listen" << std::endl;
		return -1;
	}


	hiloTermica = true;

	std::cout << "\nEsperando conexion ..." << std::endl;

	return 0;
}

//--------------------------------------------------------------------------------------------------------------
int closeSocket()
{
	closesocket(sock); //cierra el socket
	WSACleanup(); // cierra los recursos de la DLL abierto por WSAStartup
	MessageBeep(MB_ICONEXCLAMATION); // beep del PC
	std::cout << "Desconectado...\n" << std::endl;

	return 0;
}

//--------------------------------------------------------------------------------------------------------------
DWORD WINAPI MyThreadSocket(LPVOID lpParam)
{
	//hay una conexion entrante y la aceptamos
	int len = sizeof(struct sockaddr);
	sock = accept(sock, (sockaddr*)&local, &len);


	//recibe el tama�o del buffer
	int numFichero = 0;
	while (true)
	{
		int inicio;
		if ((len = recv(sock, (char*)&inicio, sizeof(inicio), 0)) == -1)
		{
			//std::cout << "error inico" << std::endl;
			return -1;
		}
		if (ntohl(inicio) != 999999)
		{
			continue;
		}
		std::cout << "incio" << std::endl;

		int size;
		if ((len = recv(sock, (char*)&size, sizeof(size), 0)) == -1)
		{
			//std::cout << "error al recibir buffer size" << std::endl;
			return -1;
		}
		std::cout << "Buffer size " << ntohl(size) << std::endl;

		if ((ntohl(size) < 0) || (ntohl(size) > 600000))
			continue;

		FILE *archivo;
		std::stringstream fname;
		fname << "d:\\file_" << numFichero << ".jpg";
		if ((archivo = std::fopen(fname.str().c_str(), "ab")) == NULL)
		{
			//std::cout << "Error al intenter de abrir el archivo" << std::endl;
			return -1;
		}

		long int porcentaje = 0;
		long int contador = 0;

		__int32 totalbyte = ntohl(size);
		//std::cout << "Buffer size " << totalbyte << std::endl;

		int bufferSize = totalbyte;//2048;
		char *Buffer = new char[totalbyte];

		while ((len != 0) && (contador < totalbyte)) //mientras estemos conectados con el otro pc
		{
			//recibimos los datos que envie
			if ((len = recv(sock, Buffer, bufferSize, 0)) == -1)
			{
				//std::cout << "\nError al recibir el byte!!" << std::endl;
				std::fclose(archivo); //cierra el archivo
				return -1;
			}
			else if (len > 0) //si seguimos conectados
			{
				fwrite(Buffer, 1, len, archivo);
				contador += len;
			}
			//std::cout << len << std::endl;
		}
		std::fclose(archivo); //cierra el archivo

		ficheros.push_back(fname.str().c_str());

		numFichero++;

		//std::cout << "\nFichero recibido con exito!!\n" << std::endl;
	}
}


//--------------------------------------------------------------------------------------------------------------
int Ejem_OpenCV_QT::stream(PXCSenseManager *pp)
{
	stopStream = false;

	/*
	//TERMICA : Abrimos el socket y lanzamos el hilo de escucha
	openSocket();
	DWORD myThreadSocketID;
	hThreadSocket = CreateThread(NULL,               // default security attributes
								0,                  // use default stack size  
								MyThreadSocket,     // thread function name
								NULL,				// argument to thread function 
								0,                  // use default creation flags 
								&myThreadSocketID);
	*/


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

		//Termica
		termica();

		qApp->processEvents();

		//Releases lock so pipeline can process next frame 
		pp->ReleaseFrame();

		if (_kbhit()) { // Break loop
			int c = _getch() & 255;
			if (c == 27 || c == 'q' || c == 'Q') break; // ESC|q|Q for Exit
		}
	}

	wprintf_s(L"Exiting\n");

	/*
	//TERMICA : Cerramos el socket, paramos el hilo y borramos los ficheros temporales pendientes
	ResumeThread(hThreadSocket);
	closeSocket();
	deleteTempFiles();
	*/

	return 0;
}

//--------------------------------------------------------------------------------------------------------------
void Ejem_OpenCV_QT::deleteTempFiles()
{
	while (!ficheros.empty())
	{
		std::string fichero = ficheros.front();
		ficheros.pop_front();

		cv::Mat img = cv::imread(fichero.c_str());
		showImg_4(img);
		img.release();

		if (remove(fichero.c_str()) != 0)
			perror("Error deleting file");
		else
			puts("File successfully deleted");
	}
}

//--------------------------------------------------------------------------------------------------------------
void Ejem_OpenCV_QT::termica()
{
	if (ficheros.size() > 0)
	{
		std::string fichero = ficheros.front();
		ficheros.pop_front();

		cv::Mat img = cv::imread(fichero.c_str());
		showImg_4(img);
		img.release();

		if (remove(fichero.c_str()) != 0)
			perror("Error deleting file");
		else
			puts("File successfully deleted");
	}
}

//--------------------------------------------------------------------------------------------------------------
int Ejem_OpenCV_QT::view()
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
int Ejem_OpenCV_QT::record()
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
int Ejem_OpenCV_QT::play() 
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
void Ejem_OpenCV_QT::stop()
{
	/*
	//TERMICA : Cerramos el socket, paramos el hilo y borramos los ficheros temporales pendientes
	ResumeThread(hThreadSocket);
	closeSocket();
	deleteTempFiles();

	stopStream = true;
	*/
}

//--------------------------------------------------------------------------------------------------------------
void Ejem_OpenCV_QT::selectFile()
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



