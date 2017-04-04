#include "ejem_opencv_qt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Ejem_OpenCV_QT w;
	w.show();
	return a.exec();
}
