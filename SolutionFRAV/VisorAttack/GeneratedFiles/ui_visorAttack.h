/********************************************************************************
** Form generated from reading UI file 'visorAttack.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISORATTACK_H
#define UI_VISORATTACK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VisorAttackClass
{
public:
    QWidget *centralWidget;
    QPushButton *pBShowImg;
    QLabel *lImg_1;
    QGroupBox *groupBox;
    QSplitter *splitter;
    QRadioButton *rBViewReal;
    QRadioButton *rBViewAdjustDepth;
    QRadioButton *rBViewAdjustRGB;
    QPushButton *pBShowImg_4;
    QSplitter *splitter_2;
    QRadioButton *rBRealSizeImg_1;
    QGroupBox *groupBox_3;
    QSplitter *splitter_3;
    QRadioButton *rBRGB;
    QRadioButton *rBDepth;
    QRadioButton *rBIR;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VisorAttackClass)
    {
        if (VisorAttackClass->objectName().isEmpty())
            VisorAttackClass->setObjectName(QStringLiteral("VisorAttackClass"));
        VisorAttackClass->resize(1028, 1293);
        centralWidget = new QWidget(VisorAttackClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pBShowImg = new QPushButton(centralWidget);
        pBShowImg->setObjectName(QStringLiteral("pBShowImg"));
        pBShowImg->setGeometry(QRect(10, 1100, 101, 51));
        lImg_1 = new QLabel(centralWidget);
        lImg_1->setObjectName(QStringLiteral("lImg_1"));
        lImg_1->setGeometry(QRect(10, 200, 1001, 891));
        lImg_1->setStyleSheet(QStringLiteral("background-color: rgb(218, 213, 255);"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 110, 631, 101));
        splitter = new QSplitter(groupBox);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setGeometry(QRect(40, 50, 492, 37));
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
        pBShowImg_4->setGeometry(QRect(120, 1100, 101, 51));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setGeometry(QRect(810, 150, 161, 37));
        splitter_2->setOrientation(Qt::Horizontal);
        rBRealSizeImg_1 = new QRadioButton(splitter_2);
        rBRealSizeImg_1->setObjectName(QStringLiteral("rBRealSizeImg_1"));
        rBRealSizeImg_1->setChecked(false);
        rBRealSizeImg_1->setAutoExclusive(false);
        splitter_2->addWidget(rBRealSizeImg_1);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 10, 461, 101));
        splitter_3 = new QSplitter(groupBox_3);
        splitter_3->setObjectName(QStringLiteral("splitter_3"));
        splitter_3->setGeometry(QRect(10, 40, 492, 37));
        splitter_3->setOrientation(Qt::Horizontal);
        rBRGB = new QRadioButton(splitter_3);
        rBRGB->setObjectName(QStringLiteral("rBRGB"));
        rBRGB->setChecked(true);
        splitter_3->addWidget(rBRGB);
        rBDepth = new QRadioButton(splitter_3);
        rBDepth->setObjectName(QStringLiteral("rBDepth"));
        rBDepth->setChecked(false);
        splitter_3->addWidget(rBDepth);
        rBIR = new QRadioButton(splitter_3);
        rBIR->setObjectName(QStringLiteral("rBIR"));
        splitter_3->addWidget(rBIR);
        VisorAttackClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VisorAttackClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1028, 47));
        VisorAttackClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VisorAttackClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        VisorAttackClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(VisorAttackClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        VisorAttackClass->setStatusBar(statusBar);

        retranslateUi(VisorAttackClass);
        QObject::connect(pBShowImg, SIGNAL(clicked()), VisorAttackClass, SLOT(view()));
        QObject::connect(pBShowImg_4, SIGNAL(clicked()), VisorAttackClass, SLOT(stop()));

        QMetaObject::connectSlotsByName(VisorAttackClass);
    } // setupUi

    void retranslateUi(QMainWindow *VisorAttackClass)
    {
        VisorAttackClass->setWindowTitle(QApplication::translate("VisorAttackClass", "FRAV view", 0));
        pBShowImg->setText(QApplication::translate("VisorAttackClass", "View", 0));
        lImg_1->setText(QString());
        groupBox->setTitle(QApplication::translate("VisorAttackClass", "Adjust", 0));
        rBViewReal->setText(QApplication::translate("VisorAttackClass", "Real", 0));
        rBViewAdjustDepth->setText(QApplication::translate("VisorAttackClass", "Adjust Depth", 0));
        rBViewAdjustRGB->setText(QApplication::translate("VisorAttackClass", "Adjust RGB", 0));
        pBShowImg_4->setText(QApplication::translate("VisorAttackClass", "Stop", 0));
        rBRealSizeImg_1->setText(QApplication::translate("VisorAttackClass", "Real size", 0));
        groupBox_3->setTitle(QApplication::translate("VisorAttackClass", "View", 0));
        rBRGB->setText(QApplication::translate("VisorAttackClass", "RGB", 0));
        rBDepth->setText(QApplication::translate("VisorAttackClass", "Depth", 0));
        rBIR->setText(QApplication::translate("VisorAttackClass", "IR", 0));
    } // retranslateUi

};

namespace Ui {
    class VisorAttackClass: public Ui_VisorAttackClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISORATTACK_H
