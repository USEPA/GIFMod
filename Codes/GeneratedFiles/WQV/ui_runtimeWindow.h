/********************************************************************************
** Form generated from reading UI file 'runtimeWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RUNTIMEWINDOW_H
#define UI_RUNTIMEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qcustomplot.h>

QT_BEGIN_NAMESPACE

class Ui_runtimeWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_4;
    QLabel *topLabel;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLCDNumber *lcdNumber;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QPushButton *btnHide;
    QPushButton *btnStop;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QCustomPlot *customPlot;
    QCustomPlot *customPlot2;
    QVBoxLayout *verticalLayout_3;
    QProgressBar *progressBar2;
    QProgressBar *progressBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *runtimeWindow)
    {
        if (runtimeWindow->objectName().isEmpty())
            runtimeWindow->setObjectName(QStringLiteral("runtimeWindow"));
        runtimeWindow->resize(757, 662);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(runtimeWindow->sizePolicy().hasHeightForWidth());
        runtimeWindow->setSizePolicy(sizePolicy);
        runtimeWindow->setMinimumSize(QSize(270, 190));
        runtimeWindow->setMouseTracking(true);
        centralWidget = new QWidget(runtimeWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        topLabel = new QLabel(centralWidget);
        topLabel->setObjectName(QStringLiteral("topLabel"));
        QFont font;
        font.setPointSize(10);
        topLabel->setFont(font);
        topLabel->setMouseTracking(true);
        topLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(topLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(label);

        horizontalSpacer_3 = new QSpacerItem(5, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lcdNumber->sizePolicy().hasHeightForWidth());
        lcdNumber->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(lcdNumber);

        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        btnHide = new QPushButton(centralWidget);
        btnHide->setObjectName(QStringLiteral("btnHide"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnHide->sizePolicy().hasHeightForWidth());
        btnHide->setSizePolicy(sizePolicy3);
        btnHide->setMouseTracking(true);

        verticalLayout->addWidget(btnHide);

        btnStop = new QPushButton(centralWidget);
        btnStop->setObjectName(QStringLiteral("btnStop"));
        sizePolicy3.setHeightForWidth(btnStop->sizePolicy().hasHeightForWidth());
        btnStop->setSizePolicy(sizePolicy3);
        btnStop->setMouseTracking(true);

        verticalLayout->addWidget(btnStop);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        customPlot = new QCustomPlot(centralWidget);
        customPlot->setObjectName(QStringLiteral("customPlot"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(customPlot->sizePolicy().hasHeightForWidth());
        customPlot->setSizePolicy(sizePolicy4);

        verticalLayout_2->addWidget(customPlot);

        customPlot2 = new QCustomPlot(centralWidget);
        customPlot2->setObjectName(QStringLiteral("customPlot2"));
        sizePolicy4.setHeightForWidth(customPlot2->sizePolicy().hasHeightForWidth());
        customPlot2->setSizePolicy(sizePolicy4);

        verticalLayout_2->addWidget(customPlot2);


        horizontalLayout_3->addLayout(verticalLayout_2);


        verticalLayout_4->addLayout(horizontalLayout_3);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        progressBar2 = new QProgressBar(centralWidget);
        progressBar2->setObjectName(QStringLiteral("progressBar2"));
        progressBar2->setValue(0);

        verticalLayout_3->addWidget(progressBar2);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        verticalLayout_3->addWidget(progressBar);


        verticalLayout_4->addLayout(verticalLayout_3);


        gridLayout->addLayout(verticalLayout_4, 0, 0, 1, 1);

        runtimeWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(runtimeWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        runtimeWindow->setStatusBar(statusBar);

        retranslateUi(runtimeWindow);
        QObject::connect(btnHide, SIGNAL(clicked()), runtimeWindow, SLOT(hide()));

        QMetaObject::connectSlotsByName(runtimeWindow);
    } // setupUi

    void retranslateUi(QMainWindow *runtimeWindow)
    {
        runtimeWindow->setWindowTitle(QApplication::translate("runtimeWindow", "Plot Window", 0));
        topLabel->setText(QString());
        label->setText(QApplication::translate("runtimeWindow", "Epoch count", 0));
        btnHide->setText(QApplication::translate("runtimeWindow", "Hide", 0));
        btnStop->setText(QApplication::translate("runtimeWindow", "Stop running", 0));
    } // retranslateUi

};

namespace Ui {
    class runtimeWindow: public Ui_runtimeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RUNTIMEWINDOW_H
