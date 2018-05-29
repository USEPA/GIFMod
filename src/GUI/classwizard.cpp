#ifdef GIFMOD


#include <QtWidgets>

#include "classwizard.h"
#include "UnitTextBox3.h"


//! [0] //! [1]
ClassWizard::ClassWizard(wizardData &data, QWidget *parent)
    : QWizard(parent)
{
	setWizardStyle(QWizard::ModernStyle);

	for (int i = 0; i < data.items.count(); i++)
	{
		if (data.items[i].type.toLower().contains("intro"))
			addPage(new IntroPage(data.items[i], this));
		if (data.items[i].type.toLower().contains("property"))
			addPage(new PropertyPage(data.items[i], this));
//		if (data.items[i].type.toLower().contains("output"))
//			addPage(new OutputFilesPage(data.items[i], this));
//		if (data.items[i].type.toLower().contains("conclusion"))
//			addPage(new ConclusionPage(data.items[i], this));

	}
//    addPage(new CodeStylePage);
//    addPage(new OutputFilesPage);
//    addPage(new ConclusionPage);
//! [0]

    setPixmap(QWizard::BannerPixmap, QPixmap("C:/Users/user/Dropbox/GitHub/GIFMod/GUI/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap("C:/Users/user/Dropbox/GitHub/GIFMod/GUI/images/background.png"));

    setWindowTitle(tr("Class Wizard"));
//! [2]
}
//! [1] //! [2]

//! [3]
void ClassWizard::accept()
//! [3] //! [4]
{
    QByteArray className = field("className").toByteArray();
    QByteArray baseClass = field("baseClass").toByteArray();
    QByteArray macroName = field("macroName").toByteArray();
    QByteArray baseInclude = field("baseInclude").toByteArray();

    QString outputDir = field("outputDir").toString();
    QString header = field("header").toString();
    QString implementation = field("implementation").toString();
//! [4]

    QByteArray block;

    if (field("comment").toBool()) {
        block += "/*\n";
        block += "    " + header.toLatin1() + '\n';
        block += "*/\n";
        block += '\n';
    }
    if (field("protect").toBool()) {
        block += "#ifndef " + macroName + '\n';
        block += "#define " + macroName + '\n';
        block += '\n';
    }
    if (field("includeBase").toBool()) {
        block += "#include " + baseInclude + '\n';
        block += '\n';
    }

    block += "class " + className;
    if (!baseClass.isEmpty())
        block += " : public " + baseClass;
    block += '\n';
    block += "{\n";

    /* qmake ignore Q_OBJECT */

    if (field("qobjectMacro").toBool()) {
        block += "    Q_OBJECT\n";
        block += '\n';
    }
    block += "public:\n";

    if (field("qobjectCtor").toBool()) {
        block += "    " + className + "(QObject *parent = 0);\n";
    } else if (field("qwidgetCtor").toBool()) {
        block += "    " + className + "(QWidget *parent = 0);\n";
    } else if (field("defaultCtor").toBool()) {
        block += "    " + className + "();\n";
        if (field("copyCtor").toBool()) {
            block += "    " + className + "(const " + className + " &other);\n";
            block += '\n';
            block += "    " + className + " &operator=" + "(const " + className
                     + " &other);\n";
        }
    }
    block += "};\n";

    if (field("protect").toBool()) {
        block += '\n';
        block += "#endif\n";
    }

    QFile headerFile(outputDir + '/' + header);
    if (!headerFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Simple Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(headerFile.fileName())
                             .arg(headerFile.errorString()));
        return;
    }
    headerFile.write(block);

    block.clear();

    if (field("comment").toBool()) {
        block += "/*\n";
        block += "    " + implementation.toLatin1() + '\n';
        block += "*/\n";
        block += '\n';
    }
    block += "#include \"" + header.toLatin1() + "\"\n";
    block += '\n';

    if (field("qobjectCtor").toBool()) {
        block += className + "::" + className + "(QObject *parent)\n";
        block += "    : " + baseClass + "(parent)\n";
        block += "{\n";
        block += "}\n";
    } else if (field("qwidgetCtor").toBool()) {
        block += className + "::" + className + "(QWidget *parent)\n";
        block += "    : " + baseClass + "(parent)\n";
        block += "{\n";
        block += "}\n";
    } else if (field("defaultCtor").toBool()) {
        block += className + "::" + className + "()\n";
        block += "{\n";
        block += "    // missing code\n";
        block += "}\n";

        if (field("copyCtor").toBool()) {
            block += "\n";
            block += className + "::" + className + "(const " + className
                     + " &other)\n";
            block += "{\n";
            block += "    *this = other;\n";
            block += "}\n";
            block += '\n';
            block += className + " &" + className + "::operator=(const "
                     + className + " &other)\n";
            block += "{\n";
            if (!baseClass.isEmpty())
                block += "    " + baseClass + "::operator=(other);\n";
            block += "    // missing code\n";
            block += "    return *this;\n";
            block += "}\n";
        }
    }

    QFile implementationFile(outputDir + '/' + implementation);
    if (!implementationFile.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(0, QObject::tr("Simple Wizard"),
                             QObject::tr("Cannot write file %1:\n%2")
                             .arg(implementationFile.fileName())
                             .arg(implementationFile.errorString()));
        return;
    }
    implementationFile.write(block);

//! [5]
    QDialog::accept();
//! [5] //! [6]
}
//! [6]

//! [7]
IntroPage::IntroPage(wizardDataItem data, QWidget *parent)
    : QWizardPage(parent)
{
	setTitle(data.name);
    if (data.watermark!="")
		setPixmap(QWizard::WatermarkPixmap, QPixmap(data.watermark));

    label = new QLabel(data.desc);
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}
//! [7]

//! [8] //! [9]
PropertyPage::PropertyPage(wizardDataItem data, QWidget *parent)
    : QWizardPage(parent)
{
//! [8]
    setTitle(data.name);
	setSubTitle(data.desc);

	for (int i = 0; i < data.groups.count(); i++)
	{
		//setPixmap(QWizard::LogoPixmap, QPixmap("C:/Users/user/Dropbox/GitHub/GIFMod/GUI/images/logo1.png"));

		//! [10]
		labels.append(QList<QLabel*>());
		lineEdits.append(QList<UnitTextBox*>());
		for (int j = 0; j < data.labels[i].count(); j++)
		{
			labels[i].append(new QLabel(data.labels[i][j]));
			lineEdits[i].append(new UnitTextBox(data.variables[i][j]));
			labels[i][j]->setBuddy(lineEdits[i][j]);

		/*	baseClassLabel = new QLabel(tr("B&ase class:"));
			baseClassLineEdit = new QLineEdit;
			baseClassLabel->setBuddy(baseClassLineEdit);*/
		}
	}
    //qobjectMacroCheckBox = new QCheckBox(tr("Generate Q_OBJECT &macro"));

//! [10]
   groupBox = new QGroupBox(tr("C&onstructor"));
//! [9]

    //qobjectCtorRadioButton = new QRadioButton(tr("&QObject-style constructor"));
    //qwidgetCtorRadioButton = new QRadioButton(tr("Q&Widget-style constructor"));
    //defaultCtorRadioButton = new QRadioButton(tr("&Default constructor"));
    //copyCtorCheckBox = new QCheckBox(tr("&Generate copy constructor and "
    //                                    "operator="));

    //defaultCtorRadioButton->setChecked(true);

    //connect(defaultCtorRadioButton, &QAbstractButton::toggled,
    //        copyCtorCheckBox, &QWidget::setEnabled);

//! [11] //! [12]
	for (int i = 0; i < data.groups.count(); i++)
	{
		for (int j = 0; j < data.labels[i].count(); j++)
			registerField(data.variables[i][j].toQString(), lineEdits[i][j]);
	}
//    registerField("baseClass", baseClassLineEdit);
 //   registerField("qobjectMacro", qobjectMacroCheckBox);
//! [11]
//    registerField("qobjectCtor", qobjectCtorRadioButton);
//    registerField("qwidgetCtor", qwidgetCtorRadioButton);
//    registerField("defaultCtor", defaultCtorRadioButton);
//    registerField("copyCtor", copyCtorCheckBox);

//    QVBoxLayout *groupBoxLayout = new QVBoxLayout;
//! [12]
//    groupBoxLayout->addWidget(qobjectCtorRadioButton);
//    groupBoxLayout->addWidget(qwidgetCtorRadioButton);
//    groupBoxLayout->addWidget(defaultCtorRadioButton);
//    groupBoxLayout->addWidget(copyCtorCheckBox);
//    groupBox->setLayout(groupBoxLayout);

	int rowCounter = 0;
	QGridLayout *layout = new QGridLayout;
	for (int i = 0; i < data.groups.count(); i++)
	{
		layout->addWidget(new QLabel(data.groups[i]));
		rowCounter++;
		for (int j = 0; j < data.labels[i].count(); j++)
		{
			layout->addWidget(labels[i][j], rowCounter, 0);
            UnitTextBox3 * a = new UnitTextBox3(XString("XString"), true, this);
			layout->addWidget(a, rowCounter, 1);
			
			a->show();
			a->repaint();
			layout->addWidget(new QLineEdit("text"), rowCounter, 2);

//			layout->addWidget(lineEdits[i][j], rowCounter, 2);
			rowCounter++;
		}
		rowCounter++;
	}
	groupBox->setLayout(layout);
	
//	layout->addWidget(classNameLabel, 0, 0);
//   layout->addWidget(classNameLineEdit, 0, 1);
//    layout->addWidget(baseClassLabel, 1, 0);
//    layout->addWidget(baseClassLineEdit, 1, 1);
//   layout->addWidget(qobjectMacroCheckBox, 2, 0, 1, 2);
//    layout->addWidget(groupBox, 3, 0, 1, 2);
    setLayout(layout);
//! [13]
//! [8]
}
//! [13]






//! [14]
CodeStylePage::CodeStylePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Code Style Options"));
    setSubTitle(tr("Choose the formatting of the generated code."));
    setPixmap(QWizard::LogoPixmap, QPixmap("C:/Users/user/Dropbox/GitHub/GIFMod/GUI/images/logo2.png"));

    commentCheckBox = new QCheckBox(tr("&Start generated files with a "
//! [14]
                                       "comment"));
    commentCheckBox->setChecked(true);

    protectCheckBox = new QCheckBox(tr("&Protect header file against multiple "
                                       "inclusions"));
    protectCheckBox->setChecked(true);

    macroNameLabel = new QLabel(tr("&Macro name:"));
    macroNameLineEdit = new QLineEdit;
    macroNameLabel->setBuddy(macroNameLineEdit);

    includeBaseCheckBox = new QCheckBox(tr("&Include base class definition"));
    baseIncludeLabel = new QLabel(tr("Base class include:"));
    baseIncludeLineEdit = new QLineEdit;
    baseIncludeLabel->setBuddy(baseIncludeLineEdit);

    connect(protectCheckBox, &QAbstractButton::toggled,
            macroNameLabel, &QWidget::setEnabled);
    connect(protectCheckBox, &QAbstractButton::toggled,
            macroNameLineEdit, &QWidget::setEnabled);
    connect(includeBaseCheckBox, &QAbstractButton::toggled,
            baseIncludeLabel, &QWidget::setEnabled);
    connect(includeBaseCheckBox, &QAbstractButton::toggled,
            baseIncludeLineEdit, &QWidget::setEnabled);

    registerField("comment", commentCheckBox);
    registerField("protect", protectCheckBox);
    registerField("macroName", macroNameLineEdit);
    registerField("includeBase", includeBaseCheckBox);
    registerField("baseInclude", baseIncludeLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->setColumnMinimumWidth(0, 20);
    layout->addWidget(commentCheckBox, 0, 0, 1, 3);
    layout->addWidget(protectCheckBox, 1, 0, 1, 3);
    layout->addWidget(macroNameLabel, 2, 1);
    layout->addWidget(macroNameLineEdit, 2, 2);
    layout->addWidget(includeBaseCheckBox, 3, 0, 1, 3);
    layout->addWidget(baseIncludeLabel, 4, 1);
    layout->addWidget(baseIncludeLineEdit, 4, 2);
//! [15]
    setLayout(layout);
}
//! [15]

//! [16]
void CodeStylePage::initializePage()
{
    QString className = field("className").toString();
    macroNameLineEdit->setText(className.toUpper() + "_H");

    QString baseClass = field("baseClass").toString();

    includeBaseCheckBox->setChecked(!baseClass.isEmpty());
    includeBaseCheckBox->setEnabled(!baseClass.isEmpty());
    baseIncludeLabel->setEnabled(!baseClass.isEmpty());
    baseIncludeLineEdit->setEnabled(!baseClass.isEmpty());

    if (baseClass.isEmpty()) {
        baseIncludeLineEdit->clear();
    } else if (QRegExp("Q[A-Z].*").exactMatch(baseClass)) {
        baseIncludeLineEdit->setText('<' + baseClass + '>');
    } else {
        baseIncludeLineEdit->setText('"' + baseClass.toLower() + ".h\"");
    }
}
//! [16]

OutputFilesPage::OutputFilesPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Output Files"));
    setSubTitle(tr("Specify where you want the wizard to put the generated "
                   "skeleton code."));
    setPixmap(QWizard::LogoPixmap, QPixmap("C:/Users/user/Dropbox/GitHub/GIFMod/GUI/images/logo3.png"));

    outputDirLabel = new QLabel(tr("&Output directory:"));
    outputDirLineEdit = new QLineEdit;
    outputDirLabel->setBuddy(outputDirLineEdit);

    headerLabel = new QLabel(tr("&Header file name:"));
    headerLineEdit = new QLineEdit;
    headerLabel->setBuddy(headerLineEdit);

    implementationLabel = new QLabel(tr("&Implementation file name:"));
    implementationLineEdit = new QLineEdit;
    implementationLabel->setBuddy(implementationLineEdit);

    registerField("outputDir*", outputDirLineEdit);
    registerField("header*", headerLineEdit);
    registerField("implementation*", implementationLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(outputDirLabel, 0, 0);
    layout->addWidget(outputDirLineEdit, 0, 1);
    layout->addWidget(headerLabel, 1, 0);
    layout->addWidget(headerLineEdit, 1, 1);
    layout->addWidget(implementationLabel, 2, 0);
    layout->addWidget(implementationLineEdit, 2, 1);
    setLayout(layout);
}

//! [17]
void OutputFilesPage::initializePage()
{
    QString className = field("className").toString();
    headerLineEdit->setText(className.toLower() + ".h");
    implementationLineEdit->setText(className.toLower() + ".cpp");
    outputDirLineEdit->setText(QDir::toNativeSeparators(QDir::tempPath()));
}
//! [17]

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Conclusion"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap("C:/Users/user/Dropbox/GitHub/GIFMod/GUI/images/watermark2.png"));

    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void ConclusionPage::initializePage()
{
    QString finishText = wizard()->buttonText(QWizard::FinishButton);
    finishText.remove('&');
    label->setText(tr("Click %1 to generate the class skeleton.")
                   .arg(finishText));
}
#endif
