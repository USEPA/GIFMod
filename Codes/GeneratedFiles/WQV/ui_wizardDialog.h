/********************************************************************************
** Form generated from reading UI file 'wizardDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIZARDDIALOG_H
#define UI_WIZARDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_wizardDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *wizardDialog)
    {
        if (wizardDialog->objectName().isEmpty())
            wizardDialog->setObjectName(QStringLiteral("wizardDialog"));
        wizardDialog->resize(731, 615);
        horizontalLayout = new QHBoxLayout(wizardDialog);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(wizardDialog);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        tabWidget = new QTabWidget(wizardDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(wizardDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(wizardDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), wizardDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), wizardDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(wizardDialog);
    } // setupUi

    void retranslateUi(QDialog *wizardDialog)
    {
        wizardDialog->setWindowTitle(QApplication::translate("wizardDialog", "Dialog", 0));
        label->setText(QApplication::translate("wizardDialog", "TextLabel", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("wizardDialog", "Tab 1", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("wizardDialog", "Tab 2", 0));
    } // retranslateUi

};

namespace Ui {
    class wizardDialog: public Ui_wizardDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZARDDIALOG_H
