/********************************************************************************
** Form generated from reading UI file 'gridWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDWINDOW_H
#define UI_GRIDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include "unittextbox2.h"

QT_BEGIN_NAMESPACE

class Ui_gridWindow
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBoxV;
    QSpinBox *rowsSpin;
    QLabel *label_4;
    QSpinBox *columnsSpin;
    QLabel *label_5;
    QLabel *heightLbl;
    QLabel *z0Lbl;
    UnitTextBox2 *deltaHUnitBox;
    UnitTextBox2 *deltaVUnitBox;
    QLabel *label_1;
    QLabel *Label_2;
    UnitTextBox2 *lengthUnitBox;
    QLabel *label_2;
    QRadioButton *radioButtonV;
    QRadioButton *radioButtonH;
    QGroupBox *groupBoxH;
    QSpinBox *rowsSpin_2;
    QLabel *label_9;
    QSpinBox *columnsSpin_2;
    QLabel *label_10;
    QLabel *z0Lbl_3;
    UnitTextBox2 *deltaHXUnitBox;
    UnitTextBox2 *deltaHYUnitBox;
    QLabel *label_11;
    QLabel *Label_4;
    UnitTextBox2 *lengthXUnitBox;
    QLabel *label_12;
    QLabel *label_13;
    UnitTextBox2 *lengthYUnitBox;

    void setupUi(QDialog *gridWindow)
    {
        if (gridWindow->objectName().isEmpty())
            gridWindow->setObjectName(QStringLiteral("gridWindow"));
        gridWindow->setWindowModality(Qt::WindowModal);
        gridWindow->resize(400, 400);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(gridWindow->sizePolicy().hasHeightForWidth());
        gridWindow->setSizePolicy(sizePolicy);
        gridWindow->setMinimumSize(QSize(400, 400));
        gridWindow->setMaximumSize(QSize(400, 550));
        buttonBox = new QDialogButtonBox(gridWindow);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(310, 20, 81, 241));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBoxV = new QGroupBox(gridWindow);
        groupBoxV->setObjectName(QStringLiteral("groupBoxV"));
        groupBoxV->setGeometry(QRect(10, 60, 281, 331));
        rowsSpin = new QSpinBox(groupBoxV);
        rowsSpin->setObjectName(QStringLiteral("rowsSpin"));
        rowsSpin->setGeometry(QRect(110, 20, 42, 22));
        rowsSpin->setMinimum(1);
        label_4 = new QLabel(groupBoxV);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 20, 81, 20));
        columnsSpin = new QSpinBox(groupBoxV);
        columnsSpin->setObjectName(QStringLiteral("columnsSpin"));
        columnsSpin->setGeometry(QRect(110, 50, 42, 22));
        columnsSpin->setMinimum(1);
        label_5 = new QLabel(groupBoxV);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 50, 101, 16));
        heightLbl = new QLabel(groupBoxV);
        heightLbl->setObjectName(QStringLiteral("heightLbl"));
        heightLbl->setGeometry(QRect(10, 120, 261, 16));
        z0Lbl = new QLabel(groupBoxV);
        z0Lbl->setObjectName(QStringLiteral("z0Lbl"));
        z0Lbl->setGeometry(QRect(10, 90, 261, 16));
        deltaHUnitBox = new UnitTextBox2(groupBoxV);
        deltaHUnitBox->setObjectName(QStringLiteral("deltaHUnitBox"));
        deltaHUnitBox->setGeometry(QRect(140, 240, 133, 20));
        deltaVUnitBox = new UnitTextBox2(groupBoxV);
        deltaVUnitBox->setObjectName(QStringLiteral("deltaVUnitBox"));
        deltaVUnitBox->setGeometry(QRect(140, 300, 133, 20));
        label_1 = new QLabel(groupBoxV);
        label_1->setObjectName(QStringLiteral("label_1"));
        label_1->setGeometry(QRect(10, 220, 261, 16));
        Label_2 = new QLabel(groupBoxV);
        Label_2->setObjectName(QStringLiteral("Label_2"));
        Label_2->setGeometry(QRect(10, 280, 261, 16));
        lengthUnitBox = new UnitTextBox2(groupBoxV);
        lengthUnitBox->setObjectName(QStringLiteral("lengthUnitBox"));
        lengthUnitBox->setGeometry(QRect(140, 180, 133, 20));
        label_2 = new QLabel(groupBoxV);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 160, 261, 16));
        radioButtonV = new QRadioButton(gridWindow);
        radioButtonV->setObjectName(QStringLiteral("radioButtonV"));
        radioButtonV->setGeometry(QRect(20, 25, 101, 21));
        radioButtonV->setChecked(true);
        radioButtonH = new QRadioButton(gridWindow);
        radioButtonH->setObjectName(QStringLiteral("radioButtonH"));
        radioButtonH->setGeometry(QRect(130, 26, 111, 20));
        groupBoxH = new QGroupBox(gridWindow);
        groupBoxH->setObjectName(QStringLiteral("groupBoxH"));
        groupBoxH->setGeometry(QRect(10, 60, 281, 331));
        rowsSpin_2 = new QSpinBox(groupBoxH);
        rowsSpin_2->setObjectName(QStringLiteral("rowsSpin_2"));
        rowsSpin_2->setGeometry(QRect(110, 20, 42, 22));
        rowsSpin_2->setMinimum(1);
        label_9 = new QLabel(groupBoxH);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 20, 81, 20));
        columnsSpin_2 = new QSpinBox(groupBoxH);
        columnsSpin_2->setObjectName(QStringLiteral("columnsSpin_2"));
        columnsSpin_2->setGeometry(QRect(110, 50, 42, 22));
        columnsSpin_2->setMinimum(1);
        label_10 = new QLabel(groupBoxH);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 50, 101, 16));
        z0Lbl_3 = new QLabel(groupBoxH);
        z0Lbl_3->setObjectName(QStringLiteral("z0Lbl_3"));
        z0Lbl_3->setGeometry(QRect(10, 90, 261, 16));
        deltaHXUnitBox = new UnitTextBox2(groupBoxH);
        deltaHXUnitBox->setObjectName(QStringLiteral("deltaHXUnitBox"));
        deltaHXUnitBox->setGeometry(QRect(140, 250, 133, 20));
        deltaHYUnitBox = new UnitTextBox2(groupBoxH);
        deltaHYUnitBox->setObjectName(QStringLiteral("deltaHYUnitBox"));
        deltaHYUnitBox->setGeometry(QRect(140, 300, 133, 20));
        label_11 = new QLabel(groupBoxH);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 230, 261, 16));
        Label_4 = new QLabel(groupBoxH);
        Label_4->setObjectName(QStringLiteral("Label_4"));
        Label_4->setGeometry(QRect(10, 280, 261, 16));
        lengthXUnitBox = new UnitTextBox2(groupBoxH);
        lengthXUnitBox->setObjectName(QStringLiteral("lengthXUnitBox"));
        lengthXUnitBox->setGeometry(QRect(140, 150, 133, 20));
        label_12 = new QLabel(groupBoxH);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 130, 261, 16));
        label_13 = new QLabel(groupBoxH);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 180, 261, 16));
        lengthYUnitBox = new UnitTextBox2(groupBoxH);
        lengthYUnitBox->setObjectName(QStringLiteral("lengthYUnitBox"));
        lengthYUnitBox->setGeometry(QRect(140, 200, 133, 20));
        QWidget::setTabOrder(rowsSpin, columnsSpin);
        QWidget::setTabOrder(columnsSpin, lengthUnitBox);
        QWidget::setTabOrder(lengthUnitBox, deltaHUnitBox);
        QWidget::setTabOrder(deltaHUnitBox, deltaVUnitBox);

        retranslateUi(gridWindow);
        QObject::connect(buttonBox, SIGNAL(accepted()), gridWindow, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), gridWindow, SLOT(reject()));
        QObject::connect(columnsSpin, SIGNAL(valueChanged(int)), columnsSpin_2, SLOT(setValue(int)));
        QObject::connect(columnsSpin_2, SIGNAL(valueChanged(int)), columnsSpin, SLOT(setValue(int)));
        QObject::connect(rowsSpin, SIGNAL(valueChanged(int)), rowsSpin_2, SLOT(setValue(int)));
        QObject::connect(rowsSpin_2, SIGNAL(valueChanged(int)), rowsSpin, SLOT(setValue(int)));
        QObject::connect(deltaHUnitBox, SIGNAL(textChanged(QString)), deltaHXUnitBox, SLOT(setText(QString)));
        QObject::connect(deltaHXUnitBox, SIGNAL(textChanged(QString)), deltaHUnitBox, SLOT(setText(QString)));
        QObject::connect(lengthUnitBox, SIGNAL(textChanged(QString)), lengthXUnitBox, SLOT(setText(QString)));
        QObject::connect(lengthXUnitBox, SIGNAL(textChanged(QString)), lengthUnitBox, SLOT(setText(QString)));

        QMetaObject::connectSlotsByName(gridWindow);
    } // setupUi

    void retranslateUi(QDialog *gridWindow)
    {
        gridWindow->setWindowTitle(QApplication::translate("gridWindow", "Array configuration dialog", 0));
        groupBoxV->setTitle(QApplication::translate("gridWindow", "2D Vertical array configuration", 0));
        label_4->setText(QApplication::translate("gridWindow", "Number of rows", 0));
        label_5->setText(QApplication::translate("gridWindow", "Number of columns", 0));
        heightLbl->setText(QApplication::translate("gridWindow", "Height of blocks: ", 0));
        z0Lbl->setText(QApplication::translate("gridWindow", "Bottom elevation of starting block:", 0));
        label_1->setText(QApplication::translate("gridWindow", "Total bottom elevation change in horizontal direction", 0));
        Label_2->setText(QApplication::translate("gridWindow", "Total bottom elevation change in vertical direction", 0));
        lengthUnitBox->setText(QString());
        label_2->setText(QApplication::translate("gridWindow", "Horizontal distance between cell grids", 0));
        radioButtonV->setText(QApplication::translate("gridWindow", "Vertical 2D array", 0));
        radioButtonH->setText(QApplication::translate("gridWindow", "Horizontal 2D array", 0));
        groupBoxH->setTitle(QApplication::translate("gridWindow", "2D Horizontal array configuration", 0));
        label_9->setText(QApplication::translate("gridWindow", "Number of rows", 0));
        label_10->setText(QApplication::translate("gridWindow", "Number of columns", 0));
        z0Lbl_3->setText(QApplication::translate("gridWindow", "Bottom elevation of starting block:", 0));
        label_11->setText(QApplication::translate("gridWindow", "Total bottom elevation change in x direction", 0));
        Label_4->setText(QApplication::translate("gridWindow", "Total bottom elevation change in y direction", 0));
        lengthXUnitBox->setText(QString());
        label_12->setText(QApplication::translate("gridWindow", "Horizontal distance between cell in x direction", 0));
        label_13->setText(QApplication::translate("gridWindow", "Horizontal distance between cell in y direction", 0));
        lengthYUnitBox->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class gridWindow: public Ui_gridWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDWINDOW_H
