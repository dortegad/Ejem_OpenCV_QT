#ifndef EJEM_VISOR_ATTACK_H
#define EJEM_VISOR_ATTACK_H

#include <QtWidgets/QMainWindow>
#include "ui_visorAttack.h"


//OPENCV
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

//STD
#include <conio.h>

#include "Cam3D.h"


class VisorAttack : public QMainWindow
{
	Q_OBJECT

public slots:
	int view();
	void stop();

public:
	VisorAttack(QWidget *parent = 0);
	~VisorAttack();
private:
	Ui::VisorAttackClass ui;
	bool stopStream;
	Cam3D *cam;

	void showImg(const cv::Mat &imagen);
	int stream();
};

#endif
