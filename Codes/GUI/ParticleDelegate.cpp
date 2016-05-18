#include "particledelegate.h"
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
#include "ParticleWindow.h"
//#include "qobject.h"

QWidget *ParticleDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
//	if (index.column() == 0) return QStyledItemDelegate::createEditor(parent, option, index);

	QString ParticleDelegateType = index.data(TypeRole).toString();

	if (ParticleDelegateType.contains("PushButton"))
	{
		QPushButton *editor = new QPushButton(parent);
		editor->setText(index.data(DefaultValuesListRole).toStringList()[0]);

		return editor;
	}
	if (ParticleDelegateType.contains("UnitBox"))
	{
		QStringList QL = index.data(UnitsListRole).toStringList();
		UnitTextBox *editor = new UnitTextBox(option, parent);
		editor->setUnitsList(index.data(UnitsListRole).toStringList());
		return editor;
	}
	if (ParticleDelegateType.contains("ComboBox"))
	{
		QComboBox *editor = new QComboBox(parent);
		editor->setFrame(false);
		QStringList DefaultValues = index.data(DefaultValuesListRole).toStringList();

		editor->addItems(DefaultValues);
		if (index.data(InputMethodRole).toString() == "Input") editor->setEditable(true);
		if (index.data(InputMethodRole).toString() == "Select") editor->setEditable(false);
		return editor;
	}
	if (ParticleDelegateType.contains("CheckBox"))
	{
		QCheckBox *editor = new QCheckBox(parent);
		return editor;
	}
	if (ParticleDelegateType.contains("Browser"))
	{
		QLineEdit *editor = new QLineEdit(parent);
		return editor;
	}
	if (ParticleDelegateType.contains("ListBox"))
	{
		QComboBox *editor = new QComboBox(parent);
		QStringList DefaultValues = index.data(DefaultValuesListRole).toStringList();

		editor->addItems(DefaultValues);
		editor->setAutoCompletion(true);
		return editor;
	}
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void ParticleDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
	if (index.column() == 0) QStyledItemDelegate::setEditorData(editor, index);
	QString ParticleDelegateType = index.data(TypeRole).toString();
	if (ParticleDelegateType.contains("UnitBox"))
	{
		UnitTextBox *textBox = static_cast<UnitTextBox*>(editor);
	
		textBox->setXString(index.model()->data(index, XStringEditRole).toStringList());
		return;
	}
	if (ParticleDelegateType.contains("ComboBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
	}
	if (ParticleDelegateType.contains("CheckBox"))
	{
		QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
		checkBox->setCheckState((index.model()->data(index, Qt::CheckStateRole).toBool()) ? Qt::Checked : Qt::Unchecked);
	}
	if (ParticleDelegateType.contains("Browser"))
	{
//		QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
//		lineEdit->setText("fileName");
		QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
		lineEdit->setText(index.model()->data(index, Qt::EditRole).toString());

//		QComboBox *comboBox = static_cast<QComboBox*>(editor);
//		comboBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());

	//	fileType = mList.List[index.row() - offset].inputMethod;
		/*
		QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
		QString fileName =
			QFileDialog::getOpenFileName(0, tr("Select File"),
			QDir::currentPath(),
			mList.List[index.row() - offset].inputMethod);
	//	if (fileName.isEmpty())
		//	fileName = mList.List[index.row() - offset].Value;
		lineEdit->setText(fileName);
		// *comboBox = static_cast<QComboBox*>(editor);
		//comboBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());

*/
	}
	if (ParticleDelegateType.contains("ListBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
	}
	if (ParticleDelegateType.contains("PushButton"))
	{
		QPushButton *pushButton = static_cast<QPushButton*>(editor); 
//		pushButton->setText("text");
//		QAction *openAct;
//		openAct = new QAction( (index.data(DefaultValuesListRole).toStringList()[0], parent);
		//openAct->setShortcuts(QKeySequence::Open);
		//openAct->setStatusTip(tr("Open an existing file"));
		if (index.data(DefaultValuesListRole).toStringList()[1].contains("Particle"))
			QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(openParticleInitialCondition()));
		if (index.data(DefaultValuesListRole).toStringList()[1].contains("Constituent"))
			QObject::connect(pushButton, SIGNAL(triggered()), this, SLOT(openConstituentInitialCondition()));
		//connect(openAct, SIGNAL(triggered()), this, SLOT(openConstituentInitialCondition()));

	//	pushButton->addAction(openAct);
	}

	QStyledItemDelegate::setEditorData(editor, index);
}

void ParticleDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
	if (index.column() == 0) QStyledItemDelegate::setModelData(editor, model, index);
//	QString Property = index.model()->data(index.sibling(index.row(), index.column() - 1), Qt::EditRole).toString();
	QString Property = model->data(index.sibling(index.row(), 0)).toString();
	QString ParticleDelegateType = index.data(TypeRole).toString();
	if (ParticleDelegateType.contains("UnitBox"))
	{
		UnitTextBox *textBox = static_cast<UnitTextBox*>(editor);
		QStringList QL = textBox->list();
		if (model->data(index, XStringEditRole).toStringList() != textBox->list())
			model->setData(index, textBox->list(), XStringEditRole);
		return;
	}
	if (ParticleDelegateType.contains("ComboBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		if (model->data(index, Qt::EditRole).toString() != comboBox->currentText())
			model->setData(index, comboBox->currentText(), Qt::EditRole);
		return;
	}
	if (ParticleDelegateType.contains("CheckBox"))
	{
		QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
		if (model->data(index, Qt::CheckStateRole).toBool() != checkBox->checkState())
			model->setData(index, (checkBox->checkState()) ? 1 : 0, Qt::EditRole);
		return;
	}
	if (ParticleDelegateType.contains("Browser"))
	{
		QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
		model->setData(index, lineEdit->text(), Qt::EditRole);
	}
	if (ParticleDelegateType.contains("ListBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		if (model->data(index, Qt::EditRole).toString() != comboBox->currentData().toString())
			model->setData(index, comboBox->currentData().toString(), Qt::EditRole);
		return;
	}
	//QString a = model->data(index, Qt::EditRole).toString();
	//QString b = editor->index.data().toString();
	//if (model->data(index, Qt::EditRole) != index.data()) 
		QStyledItemDelegate::setModelData(editor, model, index);
}

void ParticleDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	editor->setGeometry(option.rect);
	return;
}

void ParticleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
	const QModelIndex &index) const
{
	QString ParticleDelegateType = index.data(TypeRole).toString();
	if (ParticleDelegateType.contains("BarTextBox") && index.column() == 1)
	{
		int progress = index.data().toFloat() * 100;

		QStyleOptionProgressBar progressBarOption;
		QRect rect = option.rect;
		rect.setHeight(rect.height() /2);
		progressBarOption.rect = rect;
		progressBarOption.minimum = 0;
		progressBarOption.maximum = 100;
		progressBarOption.progress = progress;
//		progressBarOption.text = QString::number(progress / 100.0);
		progressBarOption.textVisible = true;

		QApplication::style()->drawControl(QStyle::CE_ProgressBar,
			&progressBarOption, painter);
	}
	else
		QStyledItemDelegate::paint(painter, option, index);
}
