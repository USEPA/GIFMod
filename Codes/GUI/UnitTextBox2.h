#pragma once
#include "qlineedit.h"
#include "qcombobox.h"
#include "qlistview.h"
#include "XString.h"
#include <QDoubleValidator>
#include "qvalidator.h"
#include "qevent.h"

class UnitTextBox2 :
	public QWidget
{	
public:
	UnitTextBox2() {
	};
	UnitTextBox2(QWidget * parent) :QWidget(parent)
	{
		this->parent = parent;
		initialize();
	};
	UnitTextBox2(const QStyleOptionViewItem &option, QWidget * parent = 0) :QWidget(parent)
	{
		this->parent = parent;
		initialize();
		Rect = option.rect;
	};
	void show(){
		unitBox->show();
		textBox->show();
	}
	~UnitTextBox2(){
	};
	void initialize(){
		textBox = new QLineEdit(parent);
		unitBox = new QComboBox(textBox);
		validator = new QDoubleValidator(textBox);
	}
	void setValidator(const QValidator *v)
	{
		textBox->setValidator(v);
	}
	void paintEvent(QPaintEvent * event)
	{
		textBox->setGeometry(Rect);
		QFont QF = font(); QF.setPointSize(QF.pointSize() - 1);
		unitBox->setFont(QF);
		unitBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
		int w = (unitBox->rect().width() < rect().width() / 4) ? unitBox->rect().width() : rect().width() / 4;
		unitBox->setGeometry(rect().width() - unitBox->rect().width(), 0, w, rect().height());
	};
	void setXString(const XString &X)
	{ 
		setText(X.toQString()); 
		setUnitsList(X.unitsList); 
		setUnit(X.unit);
		defaultUnit = X.defaultUnit;

	};
	XString toXString() const{
		return XString(list());
	}
	XString value() const{
		return toXString();
	}
	void setText(const QString &T){ textBox->setText(T); };
	void setUnit(const QString &U){ unitBox->setCurrentText(U); };
	void setUnitsList(const QStringList &L){ for (int i = 0; unitBox->count(); i++) unitBox->clear(); unitBox->addItems(L); };
	QString text() const { return textBox->text(); };
	QString unit() const { return unitBox->currentText(); };
	QStringList units() const { QStringList R; for (int i = 0; i < unitBox->count(); i++) R.append(unitBox->itemText(i)); return R; };
	QStringList list() const { return QStringList() << text() << unit() << units() << defaultUnit; };
	void setGeometry(const QRect &R) {
		Rect = R;};
	QRect rect() const { return Rect; };
	QString defaultUnit;
	QWidget *parent;
private:
	QComboBox *unitBox;
	QLineEdit *textBox;
	QRect Rect;
	QDoubleValidator *validator;

protected:
	void focusInEvent(QFocusEvent * e){
		int i = 0;
	}
	void focusOutEvent(QFocusEvent * e){
		int i = 0;
		e->ignore();
	}
public slots:

void finishEditing(){
	emit(closeEditor(this));
}

signals :
		void editingFinished();
		void closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint = QAbstractItemDelegate::NoHint	);
	};

