/********************************************************************************
** Form generated from reading UI file 'visorRealSense.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISORREALSENSE_H
#define UI_VISORREALSENSE_H

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
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VisorRealSenseClass
{
public:
    QWidget *centralWidget;
    QPushButton *pBShowImg;
    QLabel *lImg_1;
    QLabel *lImg_2;
    QLabel *lImg_3;
    QGroupBox *groupBox;
    QSplitter *splitter;
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
    QSplitter *splitter_2;
    QRadioButton *rBRealSizeImg_1;
    QRadioButton *rBRealSizeImg_2;
    QRadioButton *rBRealSizeImg_3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VisorRealSenseClass)
    {
        if (VisorRealSenseClass->objectName().isEmpty())
            VisorRealSenseClass->setObjectName(QStringLiteral("VisorRealSenseClass"));
        VisorRealSenseClass->resize(1200, 871);
        centralWidget = new QWidget(VisorRealSenseClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pBShowImg = new QPushButton(centralWidget);
        pBShowImg->setObjectName(QStringLiteral("pBShowImg"));
        pBShowImg->setGeometry(QRect(20, 529, 91, 30));
        lImg_1 = new QLabel(centralWidget);
        lImg_1->setObjectName(QStringLiteral("lImg_1"));
        lImg_1->setGeometry(QRect(20, 159, 361, 311));
        lImg_1->setStyleSheet(QStringLiteral("background-color: rgb(218, 213, 255);"));
        lImg_2 = new QLabel(centralWidget);
        lImg_2->setObjectName(QStringLiteral("lImg_2"));
        lImg_2->setGeometry(QRect(386, 159, 361, 311));
        lImg_2->setStyleSheet(QStringLiteral("background-color: rgb(218, 213, 255);"));
        lImg_3 = new QLabel(centralWidget);
        lImg_3->setObjectName(QStringLiteral("lImg_3"));
        lImg_3->setGeometry(QRect(752, 159, 361, 311));
        lImg_3->setStyleSheet(QStringLiteral("background-color: rgb(218, 213, 255);"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 20, 631, 101));
        splitter = new QSplitter(groupBox);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setGeometry(QRect(13, 30, 492, 37));
        splitter->setOrientation(Qt::Horizontal);
        rBViewReal = new QRadioButton(splitter);
        rBViewReal->setObjectName(QStringLiteral("rBViewReal"));
        rBViewReal->setChecked(true);
        splitter->addWidget(rBViewReal);
        rBViewAdjustDepth = new QRadioButton(splitter);
        rBViewAdjustDepth->setObjectName(QStringLiteral("rBViewAdjustDepth"));
        rBViewAdjustDepth->setChecked(false);
        splitter->addWidget(rBViewAdjustDepth);
        rBViewAdjustRGB = new QRadioButton(splitter);
        rBViewAdjustRGB->setObjectName(QStringLiteral("rBViewAdjustRGB"));
        splitter->addWidget(rBViewAdjustRGB);
        pBShowImg_4 = new QPushButton(centralWidget);
        pBShowImg_4->setObjectName(QStringLiteral("pBShowImg_4"));
        pBShowImg_4->setGeometry(QRect(120, 530, 91, 30));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 560, 961, 131));
        pBShowImg_3 = new QPushButton(groupBox_2);
        pBShowImg_3->setObjectName(QStringLiteral("pBShowImg_3"));
        pBShowImg_3->setGeometry(QRect(120, 70, 101, 41));
        pBShowImg_2 = new QPushButton(groupBox_2);
        pBShowImg_2->setObjectName(QStringLiteral("pBShowImg_2"));
        pBShowImg_2->setGeometry(QRect(10, 70, 101, 41));
        lEfileName = new QLineEdit(groupBox_2);
        lEfileName->setObjectName(QStringLiteral("lEfileName"));
        lEfileName->setGeometry(QRect(130, 30, 541, 20));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 30, 60, 16));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(680, 30, 21, 23));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setGeometry(QRect(30, 480, 1081, 37));
        splitter_2->setOrientation(Qt::Horizontal);
        rBRealSizeImg_1 = new QRadioButton(splitter_2);
        rBRealSizeImg_1->setObjectName(QStringLiteral("rBRealSizeImg_1"));
        rBRealSizeImg_1->setChecked(false);
        rBRealSizeImg_1->setAutoExclusive(false);
        splitter_2->addWidget(rBRealSizeImg_1);
        rBRealSizeImg_2 = new QRadioButton(splitter_2);
        rBRealSizeImg_2->setObjectName(QStringLiteral("rBRealSizeImg_2"));
        rBRealSizeImg_2->setChecked(false);
        rBRealSizeImg_2->setAutoExclusive(false);
        splitter_2->addWidget(rBRealSizeImg_2);
        rBRealSizeImg_3 = new QRadioButton(splitter_2);
        rBRealSizeImg_3->setObjectName(QStringLiteral("rBRealSizeImg_3"));
        rBRealSizeImg_3->setChecked(false);
        rBRealSizeImg_3->setAutoExclusive(false);
        splitter_2->addWidget(rBRealSizeImg_3);
        VisorRealSenseClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VisorRealSenseClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1200, 47));
        VisorRealSenseClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VisorRealSenseClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        VisorRealSenseClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(VisorRealSenseClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        VisorRealSenseClass->setStatusBar(statusBar);

        retranslateUi(VisorRealSenseClass);
        QObject::connect(pBShowImg, SIGNAL(clicked()), VisorRealSenseClass, SLOT(view()));
        QObject::connect(pBShowImg_2, SIGNAL(clicked()), VisorRealSenseClass, SLOT(record()));
        QObject::connect(pBShowImg_3, SIGNAL(clicked()), VisorRealSenseClass, SLOT(play()));
        QObject::connect(pBShowImg_4, SIGNAL(clicked()), VisorRealSenseClass, SLOT(stop()));
        QObject::connect(pushButton, SIGNAL(clicked()), VisorRealSenseClass, SLOT(selectFile()));

        QMetaObject::connectSlotsByName(VisorRealSenseClass);
    } // setupUi

    void retranslateUi(QMainWindow *VisorRealSenseClass)
    {
        VisorRealSenseClass->setWindowTitle(QApplication::translate("VisorRealSenseClass", "FRAV view", 0));
        pBShowImg->setText(QApplication::translate("VisorRealSenseClass", "View", 0));
        lImg_1->setText(QString());
        lImg_2->setText(QString());
        lImg_3->setText(QString());
        groupBox->setTitle(QApplication::translate("VisorRealSenseClass", "View", 0));
        rBViewReal->setText(QApplication::translate("VisorRealSenseClass", "Real", 0));
        rBViewAdjustDepth->setText(QApplication::translate("VisorRealSenseClass", "Adjust Depth", 0));
        rBViewAdjustRGB->setText(QApplication::translate("VisorRealSenseClass", "Adjust RGB", 0));
        pBShowImg_4->setText(QApplication::translate("VisorRealSenseClass", "Stop", 0));
        groupBox_2->setTitle(QApplication::translate("VisorRealSenseClass", "File", 0));
        pBShowImg_3->setText(QApplication::translate("VisorRealSenseClass", "Play", 0));
        pBShowImg_2->setText(QApplication::translate("VisorRealSenseClass", "Record", 0));
        lEfileName->setText(QApplication::translate("VisorRealSenseClass", "D:\\newfile.rssdk", 0));
        label->setText(QApplication::translate("VisorRealSenseClass", "file name", 0));
        pushButton->setText(QApplication::translate("VisorRealSenseClass", "...", 0));
        rBRealSizeImg_1->setText(QApplication::translate("VisorRealSenseClass", "Real size", 0));
        rBRealSizeImg_2->setText(QApplication::translate("VisorRealSenseClass", "Real size", 0));
        rBRealSizeImg_3->setText(QApplication::translate("VisorRealSenseClass", "Real size", 0));
    } // retranslateUi

};

namespace Ui {
    class VisorRealSenseClass: public Ui_VisorRealSenseClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISORREALSENSE_H
