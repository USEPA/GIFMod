/********************************************************************************
** Form generated from reading UI file 'plotwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTWINDOW_H
#define UI_PLOTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_plotWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QCustomPlot *customPlot;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *plotWindow)
    {
        if (plotWindow->objectName().isEmpty())
            plotWindow->setObjectName(QStringLiteral("plotWindow"));
        plotWindow->resize(548, 420);
        centralWidget = new QWidget(plotWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        customPlot = new QCustomPlot(centralWidget);
        customPlot->setObjectName(QStringLiteral("customPlot"));

        verticalLayout->addWidget(customPlot);

        plotWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(plotWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        plotWindow->setStatusBar(statusBar);

        retranslateUi(plotWindow);

        QMetaObject::connectSlotsByName(plotWindow);
    } // setupUi

    void retranslateUi(QMainWindow *plotWindow)
    {
        plotWindow->setWindowTitle(QApplication::translate("plotWindow", "Plot Window", 0));
    } // retranslateUi

};

namespace Ui {
    class plotWindow: public Ui_plotWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTWINDOW_H
