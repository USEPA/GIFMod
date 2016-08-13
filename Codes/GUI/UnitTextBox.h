#pragma once
#include "qlineedit.h"
#include "qcombobox.h"
#include "qlistview.h"
#include "XString.h"
#include <QDoubleValidator>
class UnitTextBox :
	public QWidget
{	
public:
	UnitTextBox() {};
	UnitTextBox(const QStyleOptionViewItem &option, QWidget * parent = 0) :QWidget(parent)
	{
		textBox = new QLineEdit(this);
		unitBox = new QComboBox(textBox);
		validator = new QDoubleValidator(this);
		textBox->setValidator(validator);
		Rect = option.rect;
		unitBox->show();
		textBox->show();
		this->show();

		//		QObject::connect(textBox, SIGNAL(textChanged(const QString & text)), this, SIGNAL(editingFinished()));
	};
	~UnitTextBox(){};
	void paintEvent(QPaintEvent * event)
	{
		setGeometry(rect());
		textBox->resize(rect().width(), rect().height());
		QFont QF = font(); QF.setPointSize(QF.pointSize() - 1);
		unitBox->setFont(QF);
		unitBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		int w = (unitBox->rect().width() < rect().width() / 2) ? unitBox->rect().width() : rect().width() / 2;
		unitBox->setGeometry(rect().width() - unitBox->rect().width(), 0, w, rect().height());
	};
	//XString value() const { return XString(text(), unit(), units()); };
	void setXString(const XString &X)
	{ 
		setText(X.toQString()); 
		setUnitsList(X.unitsList); 
		setUnit(X.unit);
		defaultUnit = X.defaultUnit;

	};
	void setText(const QString &T){ textBox->setText(T); };
	void setUnit(const QString &U){ unitBox->setCurrentText(U); };
	void setUnitsList(const QStringList &L){ for (int i = 0; unitBox->count(); i++) unitBox->clear(); unitBox->addItems(L); };
	QString text() const { return textBox->text(); };
	QString unit() const { return unitBox->currentText(); };
	QStringList units() const { QStringList R; for (int i = 0; i < unitBox->count(); i++) R.append(unitBox->itemText(i)); return R; };
	QStringList list() const { return QStringList() << text() << unit() << units() << defaultUnit; };
	void setGeometry(const QRect &R) {Rect = R;};
	QRect rect() const { return Rect; };
	QString defaultUnit;
private:
	QComboBox *unitBox;
	QLineEdit *textBox;
	QRect Rect;
	QDoubleValidator *validator;
public slots:
signals:
/*	
	void	cursorPositionChanged(int old, int new1);
	void	editingFinished();
	void	returnPressed();
	void	selectionChanged();
	void	textChanged(const QString & text);
	void	textEdited(const QString & text);
	*/
	};

