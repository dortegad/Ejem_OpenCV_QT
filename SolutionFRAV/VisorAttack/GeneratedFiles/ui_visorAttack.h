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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
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
    QPushButton *pBShowImg_4;
    QPushButton *pBShowImg_5;
    QLabel *lAttack;
    QLabel *lBonaFide;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VisorAttackClass)
    {
        if (VisorAttackClass->objectName().isEmpty())
            VisorAttackClass->setObjectName(QStringLiteral("VisorAttackClass"));
        VisorAttackClass->resize(1024, 786);
        centralWidget = new QWidget(VisorAttackClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pBShowImg = new QPushButton(centralWidget);
        pBShowImg->setObjectName(QStringLiteral("pBShowImg"));
        pBShowImg->setGeometry(QRect(60, 20, 101, 51));
        lImg_1 = new QLabel(centralWidget);
        lImg_1->setObjectName(QStringLiteral("lImg_1"));
        lImg_1->setGeometry(QRect(10, 160, 1001, 571));
        lImg_1->setStyleSheet(QStringLiteral("background-color: rgb(218, 213, 255);"));
        pBShowImg_4 = new QPushButton(centralWidget);
        pBShowImg_4->setObjectName(QStringLiteral("pBShowImg_4"));
        pBShowImg_4->setGeometry(QRect(170, 20, 101, 51));
        pBShowImg_5 = new QPushButton(centralWidget);
        pBShowImg_5->setObjectName(QStringLiteral("pBShowImg_5"));
        pBShowImg_5->setGeometry(QRect(280, 20, 101, 51));
        lAttack = new QLabel(centralWidget);
        lAttack->setObjectName(QStringLiteral("lAttack"));
        lAttack->setGeometry(QRect(500, 30, 331, 101));
        QFont font;
        font.setPointSize(50);
        font.setBold(true);
        font.setWeight(75);
        lAttack->setFont(font);
        lAttack->setStyleSheet(QStringLiteral("color: rgb(255, 0, 0);"));
        lBonaFide = new QLabel(centralWidget);
        lBonaFide->setObjectName(QStringLiteral("lBonaFide"));
        lBonaFide->setGeometry(QRect(440, 30, 491, 101));
        lBonaFide->setFont(font);
        VisorAttackClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VisorAttackClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 26));
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
        QObject::connect(pBShowImg_5, SIGNAL(clicked()), VisorAttackClass, SLOT(isAttack()));

        QMetaObject::connectSlotsByName(VisorAttackClass);
    } // setupUi

    void retranslateUi(QMainWindow *VisorAttackClass)
    {
        VisorAttackClass->setWindowTitle(QApplication::translate("VisorAttackClass", "FRAV view", 0));
        pBShowImg->setText(QApplication::translate("VisorAttackClass", "View", 0));
        lImg_1->setText(QString());
        pBShowImg_4->setText(QApplication::translate("VisorAttackClass", "Stop", 0));
        pBShowImg_5->setText(QApplication::translate("VisorAttackClass", "is attack", 0));
        lAttack->setText(QApplication::translate("VisorAttackClass", "ATTACK", 0));
        lBonaFide->setText(QApplication::translate("VisorAttackClass", "BONA FIDE", 0));
    } // retranslateUi

};

namespace Ui {
    class VisorAttackClass: public Ui_VisorAttackClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISORATTACK_H
