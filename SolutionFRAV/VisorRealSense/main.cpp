#include "visorRealSense.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VisorRealSense w;
	w.show();
	return a.exec();
}
