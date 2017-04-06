#include "visorRealSenseOne.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VisorRealSenseOne w;
	w.show();
	return a.exec();
}
