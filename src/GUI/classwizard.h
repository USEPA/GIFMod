#ifdef GIFMOD


#ifndef CLASSWIZARD_H
#define CLASSWIZARD_H

#include <QWizard>
#include "XString.h"
#include "UnitTextBox.h"

class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;

struct wizardDataItem
{
	QString name, desc, watermark="";
	// Introduction, Property, Conclusion, OutputFiles
	QString type;
	QStringList groups;
	QList<QStringList> labels;
	QList<QList<XString>> variables;
};

struct wizardData
{
	QList<wizardDataItem> items;
};

//! [0]
class ClassWizard : public QWizard
{
    Q_OBJECT

public:
    ClassWizard(wizardData &items, QWidget *parent = 0);

    void accept() Q_DECL_OVERRIDE;
	wizardData data;
	QMap<QString, XString> variablesAndValues;
};
//! [0]

//! [1]
class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);
	IntroPage(wizardDataItem data, QWidget *parent = 0);

private:
    QLabel *label;
};
//! [1]

//! [2]
class PropertyPage : public QWizardPage
{
    Q_OBJECT

public:
    PropertyPage(QWidget *parent = 0);
	PropertyPage(wizardDataItem data, QWidget *parent = 0);

private:
    QList<QList<QLabel*>> labels;
    QLabel *baseClassLabel;
    QList<QList<UnitTextBox*>> lineEdits;
    QLineEdit *baseClassLineEdit;
    QCheckBox *qobjectMacroCheckBox;
    QGroupBox *groupBox;
    QRadioButton *qobjectCtorRadioButton;
    QRadioButton *qwidgetCtorRadioButton;
    QRadioButton *defaultCtorRadioButton;
    QCheckBox *copyCtorCheckBox;
};
//! [2]

//! [3]
class CodeStylePage : public QWizardPage
{
    Q_OBJECT

public:
    CodeStylePage(QWidget *parent = 0);
	CodeStylePage(wizardDataItem data, QWidget *parent = 0);

protected:
    void initializePage() Q_DECL_OVERRIDE;

private:
    QCheckBox *commentCheckBox;
    QCheckBox *protectCheckBox;
    QCheckBox *includeBaseCheckBox;
    QLabel *macroNameLabel;
    QLabel *baseIncludeLabel;
    QLineEdit *macroNameLineEdit;
    QLineEdit *baseIncludeLineEdit;
};
//! [3]

class OutputFilesPage : public QWizardPage
{
    Q_OBJECT

public:
    OutputFilesPage(QWidget *parent = 0);

protected:
    void initializePage() Q_DECL_OVERRIDE;

private:
    QLabel *outputDirLabel;
    QLabel *headerLabel;
    QLabel *implementationLabel;
    QLineEdit *outputDirLineEdit;
    QLineEdit *headerLineEdit;
    QLineEdit *implementationLineEdit;
};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

protected:
    void initializePage() Q_DECL_OVERRIDE;

private:
    QLabel *label;
};

#endif
#endif //GIFMOD
