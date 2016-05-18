/********************************************************************************
** Form generated from reading UI file 'ConstituentWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSTITUENTWINDOW_H
#define UI_CONSTITUENTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConstituentWindow
{
public:
    QAction *action_Remove;
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *ConstituentWindow)
    {
        if (ConstituentWindow->objectName().isEmpty())
            ConstituentWindow->setObjectName(QStringLiteral("ConstituentWindow"));
        ConstituentWindow->setWindowModality(Qt::WindowModal);
        ConstituentWindow->resize(714, 775);
        action_Remove = new QAction(ConstituentWindow);
        action_Remove->setObjectName(QStringLiteral("action_Remove"));
        horizontalLayout_4 = new QHBoxLayout(ConstituentWindow);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        groupBox = new QGroupBox(ConstituentWindow);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setAutoFillBackground(false);
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(32, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));

        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout_3->addLayout(verticalLayout_2);


        horizontalLayout_4->addWidget(groupBox);


        retranslateUi(ConstituentWindow);

        QMetaObject::connectSlotsByName(ConstituentWindow);
    } // setupUi

    void retranslateUi(QDialog *ConstituentWindow)
    {
        ConstituentWindow->setWindowTitle(QApplication::translate("ConstituentWindow", "Dialog", 0));
        action_Remove->setText(QApplication::translate("ConstituentWindow", "&Remove", 0));
        groupBox->setTitle(QApplication::translate("ConstituentWindow", "Constituent Concentration Initial Condition", 0));
        pushButton->setText(QApplication::translate("ConstituentWindow", "&Remove", 0));
    } // retranslateUi

};

namespace Ui {
    class ConstituentWindow: public Ui_ConstituentWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSTITUENTWINDOW_H
