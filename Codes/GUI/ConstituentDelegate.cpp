#ifdef GIFMOD
#include "Constituentdelegate.h"
#include <qcombobox.h>
#include <qdebug.h>
#include <QFileDialog>
#include <Qlineedit>
#include "qcheckbox.h"
#include "UnitTextBox.h"
#include "qapplication.h"
#include "qpushbutton.h"
#include "qaction.h"
#include "GWidget.h"
#include "node.h"
#include "ConstituentWindow.h"
//#include "qobject.h"

QWidget *ConstituentDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{

	QString ConstituentDelegateType = index.data(TypeRole).toString();

	if (ConstituentDelegateType.contains("PushButton"))
	{
		QPushButton *editor = new QPushButton(parent);
		editor->setText(index.data(DefaultValuesListRole).toStringList()[0]);

		return editor;
	}
	if (ConstituentDelegateType.contains("UnitBox"))
	{
		QStringList QL = index.data(UnitsListRole).toStringList();
		UnitTextBox *editor = new UnitTextBox(option, parent);
		editor->setUnitsList(index.data(UnitsListRole).toStringList());
		return editor;
	}
	if (ConstituentDelegateType.contains("ComboBox"))
	{
		QComboBox *editor = new QComboBox(parent);
		editor->setFrame(false);
		QStringList DefaultValues = index.data(DefaultValuesListRole).toStringList();

		editor->addItems(DefaultValues);
		if (index.data(InputMethodRole).toString() == "Input") editor->setEditable(true);
		if (index.data(InputMethodRole).toString() == "Select") editor->setEditable(false);
		return editor;
	}
	if (ConstituentDelegateType.contains("CheckBox"))
	{
		QCheckBox *editor = new QCheckBox(parent);
		return editor;
	}
	if (ConstituentDelegateType.contains("Browser"))
	{
		QLineEdit *editor = new QLineEdit(parent);
		return editor;
	}
	if (ConstituentDelegateType.contains("ListBox"))
	{
		QComboBox *editor = new QComboBox(parent);
		QStringList DefaultValues = index.data(DefaultValuesListRole).toStringList();

		editor->addItems(DefaultValues);
		editor->setAutoCompletion(true);
		return editor;
	}
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void ConstituentDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
	if (index.column() == 0) QStyledItemDelegate::setEditorData(editor, index);
	QString ConstituentDelegateType = index.data(TypeRole).toString();
	if (ConstituentDelegateType.contains("UnitBox"))
	{
		UnitTextBox *textBox = static_cast<UnitTextBox*>(editor);
	
		textBox->setXString(index.model()->data(index, XStringEditRole).toStringList());
		return;
	}
	if (ConstituentDelegateType.contains("ComboBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
	}
	if (ConstituentDelegateType.contains("CheckBox"))
	{
		QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
		checkBox->setCheckState((index.model()->data(index, Qt::CheckStateRole).toBool()) ? Qt::Checked : Qt::Unchecked);
	}
	if (ConstituentDelegateType.contains("Browser"))
	{
		QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
		lineEdit->setText(index.model()->data(index, Qt::EditRole).toString());

	}
	if (ConstituentDelegateType.contains("ListBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
	}
	if (ConstituentDelegateType.contains("PushButton"))
	{
		QPushButton *pushButton = static_cast<QPushButton*>(editor); 
		if (index.data(DefaultValuesListRole).toStringList()[1].contains("Constituent"))
			QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(openConstituentInitialCondition()));
		if (index.data(DefaultValuesListRole).toStringList()[1].contains("Constituent"))
			QObject::connect(pushButton, SIGNAL(triggered()), this, SLOT(openConstituentInitialCondition()));
	}

	QStyledItemDelegate::setEditorData(editor, index);
}

void ConstituentDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
	if (index.column() == 0) QStyledItemDelegate::setModelData(editor, model, index);
	QString Property = model->data(index.sibling(index.row(), 0)).toString();
	QString ConstituentDelegateType = index.data(TypeRole).toString();
	if (ConstituentDelegateType.contains("UnitBox"))
	{
		UnitTextBox *textBox = static_cast<UnitTextBox*>(editor);
		QStringList QL = textBox->list();
		if (model->data(index, XStringEditRole).toStringList() != textBox->list())
			model->setData(index, textBox->list(), XStringEditRole);
		return;
	}
	if (ConstituentDelegateType.contains("ComboBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		if (model->data(index, Qt::EditRole).toString() != comboBox->currentText())
			model->setData(index, comboBox->currentText(), Qt::EditRole);
		return;
	}
	if (ConstituentDelegateType.contains("CheckBox"))
	{
		QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
		if (model->data(index, Qt::CheckStateRole).toBool() != checkBox->checkState())
			model->setData(index, (checkBox->checkState()) ? 1 : 0, Qt::EditRole);
		return;
	}
	if (ConstituentDelegateType.contains("Browser"))
	{
		QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
		model->setData(index, lineEdit->text(), Qt::EditRole);
	}
	if (ConstituentDelegateType.contains("ListBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		if (model->data(index, Qt::EditRole).toString() != comboBox->currentData().toString())
			model->setData(index, comboBox->currentData().toString(), Qt::EditRole);
		return;
	}
		QStyledItemDelegate::setModelData(editor, model, index);
}

void ConstituentDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	editor->setGeometry(option.rect);
	return;
}
#endif