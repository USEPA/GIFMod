/********************************************************************************
** Form generated from reading UI file 'ObservedConstituentWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBSERVEDCONSTITUENTWINDOW_H
#define UI_OBSERVEDCONSTITUENTWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ObservedConstituentWindow
{
public:
    QAction *action_Remove;
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QDialogButtonBox *buttonBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *ObservedConstituentWindow)
    {
        if (ObservedConstituentWindow->objectName().isEmpty())
            ObservedConstituentWindow->setObjectName(QStringLiteral("ObservedConstituentWindow"));
        ObservedConstituentWindow->setWindowModality(Qt::WindowModal);
        ObservedConstituentWindow->resize(714, 170);
        action_Remove = new QAction(ObservedConstituentWindow);
        action_Remove->setObjectName(QStringLiteral("action_Remove"));
        horizontalLayout_4 = new QHBoxLayout(ObservedConstituentWindow);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        groupBox = new QGroupBox(ObservedConstituentWindow);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setAutoFillBackground(false);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        buttonBox = new QDialogButtonBox(groupBox);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        horizontalLayout->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));

        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        verticalLayout->addLayout(verticalLayout_2);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        horizontalLayout_4->addWidget(groupBox);


        retranslateUi(ObservedConstituentWindow);

        QMetaObject::connectSlotsByName(ObservedConstituentWindow);
    } // setupUi

    void retranslateUi(QDialog *ObservedConstituentWindow)
    {
        ObservedConstituentWindow->setWindowTitle(QApplication::translate("ObservedConstituentWindow", "Dialog", 0));
        action_Remove->setText(QApplication::translate("ObservedConstituentWindow", "&Remove", 0));
        groupBox->setTitle(QApplication::translate("ObservedConstituentWindow", "Constituent Concentration State Variable", 0));
    } // retranslateUi

};

namespace Ui {
    class ObservedConstituentWindow: public Ui_ObservedConstituentWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBSERVEDCONSTITUENTWINDOW_H
