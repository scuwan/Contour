/********************************************************************************
** Form generated from reading UI file 'contor.ui'
**
** Created by: Qt User Interface Compiler version 5.9.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTOR_H
#define UI_CONTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContorClass
{
public:
    QAction *actionOpen_grid_file;
    QAction *actionGradientIndex;
    QAction *actionConformityIndex;
    QWidget *centralWidget;
    QPushButton *btnDraw;
    QCheckBox *checkBoxContor;
    QCheckBox *checkBoxImage;
    QComboBox *comboBox;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ContorClass)
    {
        if (ContorClass->objectName().isEmpty())
            ContorClass->setObjectName(QStringLiteral("ContorClass"));
        ContorClass->resize(205, 251);
        actionOpen_grid_file = new QAction(ContorClass);
        actionOpen_grid_file->setObjectName(QStringLiteral("actionOpen_grid_file"));
        actionGradientIndex = new QAction(ContorClass);
        actionGradientIndex->setObjectName(QStringLiteral("actionGradientIndex"));
        actionConformityIndex = new QAction(ContorClass);
        actionConformityIndex->setObjectName(QStringLiteral("actionConformityIndex"));
        centralWidget = new QWidget(ContorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btnDraw = new QPushButton(centralWidget);
        btnDraw->setObjectName(QStringLiteral("btnDraw"));
        btnDraw->setGeometry(QRect(60, 140, 75, 23));
        checkBoxContor = new QCheckBox(centralWidget);
        checkBoxContor->setObjectName(QStringLiteral("checkBoxContor"));
        checkBoxContor->setGeometry(QRect(80, 30, 71, 16));
        checkBoxImage = new QCheckBox(centralWidget);
        checkBoxImage->setObjectName(QStringLiteral("checkBoxImage"));
        checkBoxImage->setGeometry(QRect(80, 60, 71, 16));
        comboBox = new QComboBox(centralWidget);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(60, 100, 73, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 105, 54, 12));
        ContorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ContorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 205, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        ContorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(ContorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ContorClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menu->addAction(actionOpen_grid_file);
        menu_2->addAction(actionGradientIndex);
        menu_2->addAction(actionConformityIndex);

        retranslateUi(ContorClass);

        QMetaObject::connectSlotsByName(ContorClass);
    } // setupUi

    void retranslateUi(QMainWindow *ContorClass)
    {
        ContorClass->setWindowTitle(QApplication::translate("ContorClass", "Contor", Q_NULLPTR));
        actionOpen_grid_file->setText(QApplication::translate("ContorClass", "open grid file", Q_NULLPTR));
        actionGradientIndex->setText(QApplication::translate("ContorClass", "GradientIndex", Q_NULLPTR));
        actionConformityIndex->setText(QApplication::translate("ContorClass", "ConformityIndex", Q_NULLPTR));
        btnDraw->setText(QApplication::translate("ContorClass", "\347\273\230\345\233\276", Q_NULLPTR));
        checkBoxContor->setText(QApplication::translate("ContorClass", "\347\255\211\351\253\230\347\272\277", Q_NULLPTR));
        checkBoxImage->setText(QApplication::translate("ContorClass", "\345\233\276\345\203\217", Q_NULLPTR));
        label->setText(QApplication::translate("ContorClass", "\345\261\202\357\274\210Z\357\274\211:", Q_NULLPTR));
        menu->setTitle(QApplication::translate("ContorClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("ContorClass", "\350\256\241\347\256\227", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ContorClass: public Ui_ContorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTOR_H
