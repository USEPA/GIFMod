/********************************************************************************
** Form generated from reading UI file 'helpWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPWINDOW_H
#define UI_HELPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_helpWindow
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *back;
    QPushButton *forward;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *save;
    QTextEdit *textEdit;

    void setupUi(QDialog *helpWindow)
    {
        if (helpWindow->objectName().isEmpty())
            helpWindow->setObjectName(QStringLiteral("helpWindow"));
        helpWindow->resize(617, 163);
        helpWindow->setLayoutDirection(Qt::LeftToRight);
        helpWindow->setAutoFillBackground(false);
        verticalLayout = new QVBoxLayout(helpWindow);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        back = new QPushButton(helpWindow);
        back->setObjectName(QStringLiteral("back"));

        horizontalLayout->addWidget(back);

        forward = new QPushButton(helpWindow);
        forward->setObjectName(QStringLiteral("forward"));

        horizontalLayout->addWidget(forward);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        save = new QPushButton(helpWindow);
        save->setObjectName(QStringLiteral("save"));

        horizontalLayout->addWidget(save);


        verticalLayout->addLayout(horizontalLayout);

        textEdit = new QTextEdit(helpWindow);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setEnabled(true);

        verticalLayout->addWidget(textEdit);


        retranslateUi(helpWindow);

        QMetaObject::connectSlotsByName(helpWindow);
    } // setupUi

    void retranslateUi(QDialog *helpWindow)
    {
        helpWindow->setWindowTitle(QApplication::translate("helpWindow", "Help", 0));
        back->setText(QApplication::translate("helpWindow", "<", 0));
        forward->setText(QApplication::translate("helpWindow", ">", 0));
        save->setText(QApplication::translate("helpWindow", "Save", 0));
    } // retranslateUi

};

namespace Ui {
    class helpWindow: public Ui_helpWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPWINDOW_H
