/********************************************************************************
** Form generated from reading UI file 'ejem_opencv_qt.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EJEM_OPENCV_QT_H
#define UI_EJEM_OPENCV_QT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Ejem_OpenCV_QTClass
{
public:
    QWidget *centralWidget;
    QPushButton *pBShowImg;
    QLabel *lImg_1;
    QLabel *lImg_2;
    QLabel *lImg_3;
    QGroupBox *groupBox;
    QRadioButton *rBViewReal;
    QRadioButton *rBViewAdjustDepth;
    QRadioButton *rBViewAdjustRGB;
    QPushButton *pBShowImg_4;
    QGroupBox *groupBox_2;
    QPushButton *pBShowImg_3;
    QPushButton *pBShowImg_2;
    QLineEdit *lEfileName;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *lImg_4;
    QRadioButton *rBRealSizeImg_1;
    QRadioButton *rBRealSizeImg_2;
    QRadioButton *rBRealSizeImg_3;
    QRadioButton *rBRealSizeImg_4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Ejem_OpenCV_QTClass)
    {
        if (Ejem_OpenCV_QTClass->objectName().isEmpty())
            Ejem_OpenCV_QTClass->setObjectName(QStringLiteral("Ejem_OpenCV_QTClass"));
        Ejem_OpenCV_QTClass->resize(1495, 589);
        centralWidget = new QWidget(Ejem_OpenCV_QTClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pBShowImg = new QPushButton(centralWidget);
        pBShowImg->setObjectName(QStringLiteral("pBShowImg"));
        pBShowImg->setGeometry(QRect(20, 399, 91, 30));
        lImg_1 = new QLabel(centralWidget);
        lImg_1->setObjectName(QStringLiteral("lImg_1"));
        lImg_1->setGeometry(QRect(20, 51, 361, 311));
        lImg_1->setStyleSheet(QStringLiteral("background-color: rgb(218, 213, 255);"));
        lImg_2 = new QLabel(centralWidget);
        lImg_2->setObjectName(QStringLiteral("lImg_2"));
        lImg_2->setGeometry(QRect(386, 51, 361, 311));
        lImg_2->setStyleSheet(QStringLiteral("background-color: rgb(218, 213, 255);"));
        lImg_3 = new QLabel(centralWidget);
        lImg_3->setObjectName(QStringLiteral("lImg_3"));
        lImg_3->setGeometry(QRect(752, 51, 361, 311));
        lImg_3->setStyleSheet(QStringLiteral("background-color: rgb(218, 213, 255);"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 2, 271, 41));
        rBViewReal = new QRadioButton(groupBox);
        rBViewReal->setObjectName(QStringLiteral("rBViewReal"));
        rBViewReal->setGeometry(QRect(20, 19, 61, 20));
        rBViewReal->setChecked(true);
        rBViewAdjustDepth = new QRadioButton(groupBox);
        rBViewAdjustDepth->setObjectName(QStringLiteral("rBViewAdjustDepth"));
        rBViewAdjustDepth->setGeometry(QRect(80, 20, 91, 17));
        rBViewAdjustDepth->setChecked(false);
        rBViewAdjustRGB = new QRadioButton(groupBox);
        rBViewAdjustRGB->setObjectName(QStringLiteral("rBViewAdjustRGB"));
        rBViewAdjustRGB->setGeometry(QRect(177, 20, 91, 17));
        pBShowImg_4 = new QPushButton(centralWidget);
        pBShowImg_4->setObjectName(QStringLiteral("pBShowImg_4"));
        pBShowImg_4->setGeometry(QRect(120, 400, 91, 30));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 440, 641, 80));
        pBShowImg_3 = new QPushButton(groupBox_2);
        pBShowImg_3->setObjectName(QStringLiteral("pBShowImg_3"));
        pBShowImg_3->setGeometry(QRect(110, 43, 91, 30));
        pBShowImg_2 = new QPushButton(groupBox_2);
        pBShowImg_2->setObjectName(QStringLiteral("pBShowImg_2"));
        pBShowImg_2->setGeometry(QRect(10, 43, 91, 30));
        lEfileName = new QLineEdit(groupBox_2);
        lEfileName->setObjectName(QStringLiteral("lEfileName"));
        lEfileName->setGeometry(QRect(60, 18, 541, 20));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(12, 22, 60, 16));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(600, 17, 21, 23));
        lImg_4 = new QLabel(centralWidget);
        lImg_4->setObjectName(QStringLiteral("lImg_4"));
        lImg_4->setGeometry(QRect(1118, 50, 361, 311));
        lImg_4->setStyleSheet(QStringLiteral("background-color: rgb(218, 213, 255);"));
        rBRealSizeImg_1 = new QRadioButton(centralWidget);
        rBRealSizeImg_1->setObjectName(QStringLiteral("rBRealSizeImg_1"));
        rBRealSizeImg_1->setGeometry(QRect(20, 362, 71, 20));
        rBRealSizeImg_1->setChecked(false);
        rBRealSizeImg_1->setAutoExclusive(false);
        rBRealSizeImg_2 = new QRadioButton(centralWidget);
        rBRealSizeImg_2->setObjectName(QStringLiteral("rBRealSizeImg_2"));
        rBRealSizeImg_2->setGeometry(QRect(388, 362, 71, 20));
        rBRealSizeImg_2->setChecked(false);
        rBRealSizeImg_2->setAutoExclusive(false);
        rBRealSizeImg_3 = new QRadioButton(centralWidget);
        rBRealSizeImg_3->setObjectName(QStringLiteral("rBRealSizeImg_3"));
        rBRealSizeImg_3->setGeometry(QRect(751, 362, 71, 20));
        rBRealSizeImg_3->setChecked(false);
        rBRealSizeImg_3->setAutoExclusive(false);
        rBRealSizeImg_4 = new QRadioButton(centralWidget);
        rBRealSizeImg_4->setObjectName(QStringLiteral("rBRealSizeImg_4"));
        rBRealSizeImg_4->setGeometry(QRect(1119, 362, 71, 20));
        rBRealSizeImg_4->setChecked(false);
        rBRealSizeImg_4->setAutoExclusive(false);
        Ejem_OpenCV_QTClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Ejem_OpenCV_QTClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1495, 21));
        Ejem_OpenCV_QTClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Ejem_OpenCV_QTClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        Ejem_OpenCV_QTClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Ejem_OpenCV_QTClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        Ejem_OpenCV_QTClass->setStatusBar(statusBar);

        retranslateUi(Ejem_OpenCV_QTClass);
        QObject::connect(pBShowImg, SIGNAL(clicked()), Ejem_OpenCV_QTClass, SLOT(view()));
        QObject::connect(pBShowImg_2, SIGNAL(clicked()), Ejem_OpenCV_QTClass, SLOT(record()));
        QObject::connect(pBShowImg_3, SIGNAL(clicked()), Ejem_OpenCV_QTClass, SLOT(play()));
        QObject::connect(pBShowImg_4, SIGNAL(clicked()), Ejem_OpenCV_QTClass, SLOT(stop()));
        QObject::connect(pushButton, SIGNAL(clicked()), Ejem_OpenCV_QTClass, SLOT(selectFile()));

        QMetaObject::connectSlotsByName(Ejem_OpenCV_QTClass);
    } // setupUi

    void retranslateUi(QMainWindow *Ejem_OpenCV_QTClass)
    {
        Ejem_OpenCV_QTClass->setWindowTitle(QApplication::translate("Ejem_OpenCV_QTClass", "FRAV view", 0));
        pBShowImg->setText(QApplication::translate("Ejem_OpenCV_QTClass", "View", 0));
        lImg_1->setText(QString());
        lImg_2->setText(QString());
        lImg_3->setText(QString());
        groupBox->setTitle(QApplication::translate("Ejem_OpenCV_QTClass", "View", 0));
        rBViewReal->setText(QApplication::translate("Ejem_OpenCV_QTClass", "Real", 0));
        rBViewAdjustDepth->setText(QApplication::translate("Ejem_OpenCV_QTClass", "Adjust Depth", 0));
        rBViewAdjustRGB->setText(QApplication::translate("Ejem_OpenCV_QTClass", "Adjust RGB", 0));
        pBShowImg_4->setText(QApplication::translate("Ejem_OpenCV_QTClass", "Stop", 0));
        groupBox_2->setTitle(QApplication::translate("Ejem_OpenCV_QTClass", "File", 0));
        pBShowImg_3->setText(QApplication::translate("Ejem_OpenCV_QTClass", "Play", 0));
        pBShowImg_2->setText(QApplication::translate("Ejem_OpenCV_QTClass", "Record", 0));
        lEfileName->setText(QApplication::translate("Ejem_OpenCV_QTClass", "D:\\newfile.rssdk", 0));
        label->setText(QApplication::translate("Ejem_OpenCV_QTClass", "file name", 0));
        pushButton->setText(QApplication::translate("Ejem_OpenCV_QTClass", "...", 0));
        lImg_4->setText(QString());
        rBRealSizeImg_1->setText(QApplication::translate("Ejem_OpenCV_QTClass", "Real size", 0));
        rBRealSizeImg_2->setText(QApplication::translate("Ejem_OpenCV_QTClass", "Real size", 0));
        rBRealSizeImg_3->setText(QApplication::translate("Ejem_OpenCV_QTClass", "Real size", 0));
        rBRealSizeImg_4->setText(QApplication::translate("Ejem_OpenCV_QTClass", "Real size", 0));
    } // retranslateUi

};

namespace Ui {
    class Ejem_OpenCV_QTClass: public Ui_Ejem_OpenCV_QTClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EJEM_OPENCV_QT_H
