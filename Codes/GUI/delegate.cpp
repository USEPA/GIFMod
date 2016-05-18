#include "delegate.h"
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
#include "qdatetime.h"
#include "qevent.h"
#include "qlistwidget.h"
//#include "logwindow.h"
#include "qtextedit.h"

#ifdef WQV

#include "node.h"
#include "ParticleWindow.h"
#include "ConstituentWindow.h"
#include "ExchangeParametersWindow.h"
#include "observedcombobox.h"
#include "qmessagebox.h"
#include "entity.h"
#endif


QWidget *Delegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
	this->parent->endEditingDelegate();

	if (index.column() == 0) return QStyledItemDelegate::createEditor(parent, option, index);

	QString delegateType = index.data(TypeRole).toString();
	if (delegateType.toLower().contains("date"))
	{
		QCalendarWidget *editor = new QCalendarWidget(0);
		//(option.rect.left());
		return editor;
	}
	if (delegateType.contains("PushButton"))
	{
		QPushButton *editor = new QPushButton(parent);
		editor->setText(index.data().toString());
		return editor;
	}
	if (delegateType.contains("UnitBox"))
	{
		UnitTextBox *editor = new UnitTextBox(option, parent);
		QStringList QL = index.data(UnitsListRole).toStringList();
		editor->setUnitsList(index.data(UnitsListRole).toStringList());
		return editor;
	}
	if (delegateType.contains("MultiComboBox"))
	{
		QListWidget *editor = new QListWidget(parent);
		QStringList allItems = index.data(DefaultValuesListRole).toStringList();
		for each (QString item in allItems)
		{
			QListWidgetItem* wi = new QListWidgetItem(item);
			//wi.setText(item);
			editor->addItem(wi);
		}
		editor->setSelectionMode(QAbstractItemView::MultiSelection);
		return editor;
	}
#ifdef WQV
	if (delegateType.contains("ObservedComboBox"))
	{
		ObservedComboBox *editor = new ObservedComboBox(parent);
		editor->setFrame(false);
		QStringList DefaultValues = index.data(DefaultValuesListRole).toStringList();

		editor->addItems(DefaultValues);
		if (index.data(InputMethodRole).toString() == "Input") editor->setEditable(true);
		if (index.data(InputMethodRole).toString() == "Select") editor->setEditable(false);
		return editor;
	}
#endif
	if (delegateType.contains("ComboBox"))
	{
		QComboBox *editor = new QComboBox(parent);
		editor->setFrame(false);
		QStringList DefaultValues = index.data(DefaultValuesListRole).toStringList();

		editor->addItems(DefaultValues);
		if (index.data(InputMethodRole).toString() == "Input") editor->setEditable(true);
		if (index.data(InputMethodRole).toString() == "Select") editor->setEditable(false);
		return editor;
	}
	if (delegateType.contains("CheckBox"))
	{
		QCheckBox *editor = new QCheckBox(parent);
		return editor;
	}
	if (delegateType.contains("DirBrowser"))
	{
		QPushButton *editor = new QPushButton(parent);
		editor->setText(index.data().toString());
		return editor;
	}
	if (delegateType.contains("Browser"))
	{
		QPushButton *editor = new QPushButton(parent);
		editor->setText(index.data().toString());
		return editor;
	}
	if (delegateType.contains("ListBox"))
	{
		QComboBox *editor = new QComboBox(parent);
		//connect(editor, SIGNAL(clicked()), this, SLOT(browserClicked()));
		QStringList DefaultValues = index.data(DefaultValuesListRole).toStringList();

		editor->addItems(DefaultValues);
		editor->setAutoCompletion(true);
		return editor;
	}
	if (delegateType.contains("Memo"))
	{
		QTextEdit* editor = new QTextEdit(parent);
		editor->setWordWrapMode(QTextOption::NoWrap);
		return editor;

	}

	return QStyledItemDelegate::createEditor(parent, option, index);
}

void Delegate::setEditorData(QWidget *editor,
	const QModelIndex &index) const
{
	parent->startEditingDelegate(index.data(VariableNameRole).toString());
	if (index.column() == 0) QStyledItemDelegate::setEditorData(editor, index);
	QString delegateType = index.data(TypeRole).toString();
	if (delegateType.toLower().contains("date"))
	{
		qint64 currentDate = index.model()->data(index, Qt::EditRole).toDouble();
		currentDate += QDate(1900, 1, 1).toJulianDay();
		QDate date = QDate::fromJulianDay(currentDate);

		QCalendarWidget *calendar = static_cast<QCalendarWidget*>(editor);
		calendar->setSelectedDate(date);
		parent->delegateDatePicked(calendar, index);
		QObject::connect(calendar, SIGNAL(selectionChanged()), parent, SLOT(delegateDatePicked()));
		return;
	}


	if (delegateType.contains("UnitBox"))
	{
		UnitTextBox *textBox = static_cast<UnitTextBox*>(editor);
		textBox->setXString(index.model()->data(index, XStringEditRole).toStringList());
		textBox->show();
		return;
	}
	if (delegateType.contains("MultiComboBox"))
	{
		QListWidget *list = static_cast<QListWidget*>(editor);
		QStringList selectedList = index.model()->data(index, Qt::EditRole).toString().split(':');
		for (int i = 0; i < list->count(); i++)
		{
			if (selectedList.contains(list->item(i)->text()))
				list->item(i)->setSelected(true);
			else
				list->item(i)->setSelected(false);
		}
		return;
	}
#ifdef WQV
	if (delegateType.contains("ObservedComboBox"))
	{
		ObservedComboBox *comboBox = static_cast<ObservedComboBox*>(editor);
		QString txt = index.model()->data(index, Qt::EditRole).toString();
		if (comboBox->findText(txt) == -1) {
			comboBox->insertSeparator(comboBox->count());
			comboBox->addItem(txt);
		}
		comboBox->setCurrentText(txt);
		return;
	}
#endif
	if (delegateType.contains("ComboBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
		return;
	}
	if (delegateType.contains("CheckBox"))
	{
		QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
		checkBox->setCheckState((index.model()->data(index, Qt::CheckStateRole).toBool()) ? Qt::Checked : Qt::Unchecked);
		return;
	}
	if (delegateType.contains("DirBrowser"))
	{
		QPushButton *pushButton = static_cast<QPushButton*>(editor);
		pushButton->setText(index.data().toString());
		index.model()->data(index, saveIndex);
		QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(dirBrowserClicked()));
		return;
	}
	if (delegateType.contains("Browser"))
	{
		QPushButton *pushButton = static_cast<QPushButton*>(editor);
		pushButton->setText(index.data().toString());
		index.model()->data(index, saveIndex);
		QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(browserClicked()));
		return;
	}
	if (delegateType.contains("ListBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		comboBox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
		return;
	}
	if (delegateType.contains("PushButton"))
	{
		QPushButton *pushButton = static_cast<QPushButton*>(editor);
		pushButton->setText(index.data().toString());
		if (index.data(DefaultValuesListRole).toStringList()[1].contains("Particle"))
			QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(openParticleInitialCondition()));
		if (index.data(DefaultValuesListRole).toStringList()[1].contains("Constituent"))
			QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(openConstituentInitialCondition()));
		if (index.data(DefaultValuesListRole).toStringList()[1].contains("exchange parameters"))
			QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(openAqueousExchangeParameters()));
		
		return;
	}
	if (delegateType.contains("Memo"))
	{
		QTextEdit *memo = static_cast<QTextEdit*>(editor);
		memo->setText(index.model()->data(index, Qt::EditRole).toString());
		return;
	}

	QStyledItemDelegate::setEditorData(editor, index);
}

void Delegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
	parent->endEditingDelegate();
	if (index.column() == 0) QStyledItemDelegate::setModelData(editor, model, index);
	QString Property = model->data(index.sibling(index.row(), 0)).toString();
	
	QString delegateType = index.data(TypeRole).toString();
	if (delegateType.toLower().contains("date"))
	{
		return;
	}



	if (delegateType.contains("UnitBox"))
	{
		UnitTextBox *textBox = static_cast<UnitTextBox*>(editor);
		QStringList QL = textBox->list();
		if (model->data(index, XStringEditRole).toStringList() != textBox->list())
			model->setData(index, textBox->list(), XStringEditRole);
	//	delete editor;
		return;
	}
	if (delegateType.contains("MultiComboBox"))
	{
		QListWidget *list = static_cast<QListWidget*>(editor);
		QStringList selectedList;
		//list->selectAll();
		for (int i = 0; i < list->count(); i++)
			if (list->item(i)->isSelected())
				selectedList.append(list->item(i)->text());
		QString newValue = selectedList.join(':');
		if (model->data(index, Qt::EditRole).toString() != newValue)
			model->setData(index, newValue, Qt::EditRole);
		return;
	}
#ifdef WQV
	if (delegateType.contains("ObservedComboBox"))
	{
		ObservedComboBox *comboBox = static_cast<ObservedComboBox*>(editor);
		if (model->data(index, Qt::EditRole).toString() != comboBox->currentText())
			model->setData(index, comboBox->currentText(), Qt::EditRole);
		return;
	}
#endif
	if (delegateType.contains("ComboBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		if (model->data(index, Qt::EditRole).toString() != comboBox->currentText())
			model->setData(index, comboBox->currentText(), Qt::EditRole);
		return;
	}
	if (delegateType.contains("CheckBox"))
	{
		QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
		if (model->data(index, Qt::CheckStateRole).toBool() != checkBox->checkState())
			model->setData(index, (checkBox->checkState()) ? 1 : 0, Qt::EditRole);
		return;
	}
	if (delegateType.contains("DirBrowser"))
	{
		return;
	}
	if (delegateType.contains("Browser"))
	{
		return;
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		qDebug() << comboBox->currentText();
		model->setData(index, comboBox->currentText(), Qt::EditRole);
		return;
	}
	if (delegateType.contains("ListBox"))
	{
		QComboBox *comboBox = static_cast<QComboBox*>(editor);
		if (model->data(index, Qt::EditRole).toString() != comboBox->currentData().toString())
			model->setData(index, comboBox->currentData().toString(), Qt::EditRole);
		return;
	}	
	if (delegateType.contains("PushButton"))
	{
		if (index.data(DefaultValuesListRole).toStringList()[1].contains("Particle")){
			return;
			for each(Node *n in parent->Nodes())
				if (n->isSelected())
					model->setData(index, n->g(), Qt::EditRole);
		}
		if (index.data(DefaultValuesListRole).toStringList()[1].contains("Constituent"))
			return;
		for each(Node *n in parent->Nodes())
			if (n->isSelected())
				model->setData(index, n->cg(), Qt::EditRole);
		return;
	}
	if (delegateType.contains("Memo"))
	{
		QTextEdit *memo = static_cast<QTextEdit*>(editor);
		if (index.model()->data(index, Qt::EditRole).toString() != memo->toPlainText())
			model->setData(index, memo->toPlainText(), Qt::EditRole);
		return;
	}
	if (delegateType.toLower().contains("text"))
	{
		QLineEdit *textBox = static_cast<QLineEdit*>(editor);

		if (model->data(index, Qt::EditRole).toString() != textBox->text())
			model->setData(index, textBox->text(), Qt::EditRole);
//		parent->setFocus(); // tableProp->setFocus();
//		QWidget * ed = QStyledItemDelegate::createEditor(parent, editor->rect(), index.sibling(index.row(), 0);
//		ed->setFocus();
		return;
	}
	QStyledItemDelegate::setModelData(editor, model, index);
}

void Delegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	parent->endEditingDelegate();
	QString delegateType = index.data(TypeRole).toString();

	if (delegateType.toLower().contains("date"))
		return;
	if (delegateType.toLower().contains("multicombobox"))
	{
		QRect tallerRect = option.rect;
		tallerRect.setHeight(option.rect.height() * 5);
		tallerRect.setTop(option.rect.top() + option.rect.height());
		editor->setGeometry(tallerRect);
		return;
	}
	if (delegateType.toLower().contains("memo"))
	{
		QRect bigerRect = option.rect;
		bigerRect.setHeight(option.rect.height() * 5);
		bigerRect.setTop(option.rect.top() + option.rect.height());
		bigerRect.setLeft(option.rect.left() - 100);

		editor->setGeometry(bigerRect);
		return;
	}

	editor->setGeometry(option.rect);
	return;
}

void Delegate::browserClicked()
{
	QString file;
	if (parent->propModel()->data(QModelIndex(), loadIndexandInputMethodRole) == "Input")
	{
		QString a;
		QStringList b;
		a = parent->modelPathname();
		b = parent->propModel()->data(QModelIndex(), loadIndexandDefaultValuesListRole).toStringList();

		QString defaultFile = parent->modelPathname() + "/" + parent->propModel()->data(QModelIndex(), loadIndexandDefaultValuesListRole).toStringList()[0];
		file = fileDialog->getSaveFileName(
			qApp->activeWindow(),
			tr("Select the File"),
			parent->modelPathname() + "/" + parent->propModel()->data(QModelIndex(), loadIndexandDefaultValuesListRole).toStringList()[0],
			tr("(*.txt *.csv)"));
	}
	if (parent->propModel()->data(QModelIndex(), loadIndexandInputMethodRole) == "Select")
		file = fileDialog->getOpenFileName(
		qApp->activeWindow(),
		tr("Select the File"),
		parent->modelPathname(),
		tr("(*.txt *.csv)"));
	file = relativePathFilename(file, parent->modelPathname());
	parent->propModel()->setData(QModelIndex(), file, loadIndex);
}
void Delegate::dirBrowserClicked()
{
	QString Dir = fileDialog->getExistingDirectory(
		qApp->activeWindow(),
		tr("Select the Working Directory"),
		parent->modelPathname());
	Dir = relativePathFilename(Dir, parent->modelPathname());

	parent->propModel()->setData(QModelIndex(), Dir, loadIndex);
}
void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
	const QModelIndex &index) const
{
	QString delegateType = index.data(TypeRole).toString();
	if (delegateType.contains("BarTextBox") && index.column() == 1)
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


void Delegate::openParticleInitialCondition()
{
#ifdef WQV
	for each(Node *n in parent->Nodes())
		if (n->isSelected())
		{
			QString experimentName = n->parent->experimentName();
			if (experimentName != "Global" || (n->parent->experiments->count() == 2))
			{
				new ParticleWindow(parent->parent, parent, n, experimentName);
				return;
			}
			vector<QVariant> gValues;
			for (int i = 0; i < n->parent->experimentsList().count(); i++)
				gValues.push_back(n->g(parent->experimentsList()[i]));
			multiValues gMultiValues(gValues);
			if (gMultiValues.sameValues())
			{
				new ParticleWindow(parent->parent, parent, n, experimentName);
				return;
			}
			//				particleInitialConditions->operator[](experimentName) = particleInitialConditions->operator[](parent->firstExperimentName());
			else
			{
				QString message = "You are changing the initial concentration for all the experiments. \nPlease choose whether you would like to start from the scratch or from one of the previously defined experiments.";
				QMessageBox msg;
				msg.setText(message);
				msg.setIcon(QMessageBox::Question);
				msg.setWindowTitle("Particle initial condition");
				QList <QAbstractButton *> buttons;
				QAbstractButton * emptybtn = msg.addButton("Empty initial condition", QMessageBox::AcceptRole);
				for each (QString experiment in n->parent->experimentsList())
					buttons.push_back(msg.addButton(QString("Copy from %1").arg(experiment), QMessageBox::AcceptRole));
				QPushButton * cancel = msg.addButton("Cancel", QMessageBox::RejectRole);
				msg.setDefaultButton(cancel);
				msg.exec();
				if (msg.clickedButton() == cancel)
					return;

				QList<ParticleInitialConditionItem> copyFrom;
				if (msg.clickedButton() != emptybtn)
				{
					QAbstractButton * clicked = msg.clickedButton();

					int index = buttons.indexOf(clicked);
					QStringList lst = n->parent->experimentsList();
					copyFrom = n->particleInitialCondition(n->parent->experimentsList()[index]);
				}
				for each (QString experiment in n->parent->experimentsList())
					n->particleInitialConditions->operator[](experiment) = copyFrom;

				new ParticleWindow(parent->parent, parent, n, experimentName);
			}
		}

#endif
}
void Delegate::openConstituentInitialCondition()
{
#ifdef WQV
	for each(Node *n in parent->Nodes())
		if (n->isSelected())
		{
			QString experimentName = n->parent->experimentName();
			if (experimentName != "Global" || (n->parent->experiments->count() == 2))
			{
				new ConstituentWindow(parent->parent, parent, n, experimentName);
				return;
			}
			vector<QVariant> gValues;
			for (int i = 0; i < n->parent->experimentsList().count(); i++)
				gValues.push_back(n->g(parent->experimentsList()[i]));
			multiValues gMultiValues(gValues);
			if (gMultiValues.sameValues())
			{
				new ConstituentWindow(parent->parent, parent, n, experimentName);
				return;
			}
			//				particleInitialConditions->operator[](experimentName) = particleInitialConditions->operator[](parent->firstExperimentName());
			else
			{
				QString message = "You are changing the initial concentration for all the experiments. \nPlease choose whether you would like to start from the scratch or from one of the previously defined experiments.";
				QMessageBox msg;
				msg.setText(message);
				msg.setIcon(QMessageBox::Question);
				msg.setWindowTitle("Constituent initial condition");
				QList <QAbstractButton *> buttons;
				QAbstractButton * emptybtn = msg.addButton("Empty initial condition", QMessageBox::AcceptRole);
				for each (QString experiment in n->parent->experimentsList())
					buttons.push_back(msg.addButton(QString("Copy from %1").arg(experiment), QMessageBox::AcceptRole));
				QPushButton * cancel = msg.addButton("Cancel", QMessageBox::RejectRole);
				msg.setDefaultButton(cancel);
				msg.exec();
				if (msg.clickedButton() == cancel)
					return;

				QList<ConstituentInitialConditionItem> copyFrom;
				if (msg.clickedButton() != emptybtn)
				{
					QAbstractButton * clicked = msg.clickedButton();

					int index = buttons.indexOf(clicked);
					QStringList lst = n->parent->experimentsList();
					copyFrom = n->constituentInitialCondition(n->parent->experimentsList()[index]);
				}
				for each (QString experiment in n->parent->experimentsList())
					n->constituentInitialConditions->operator[](experiment) = copyFrom;

				new ConstituentWindow(parent->parent, parent, n, experimentName);
			}
		}

#endif
}

void Delegate::openAqueousExchangeParameters()
{
#ifdef WQV
	for each(Entity *e in parent->entitiesByType("Constituent"))
		if (e->isSelected())
		{
			new ExchangeParametersWindow(parent->parent, parent, e);
			return;
		}

#endif
}
void Delegate::browserCheck(QString _fileName)
{
	if (!_fileName.contains("Browse ...")) return;
	QString file;
	if (_fileName.contains("Browse ..."))
	{
		fileDialog = new QFileDialog();
		file = fileDialog->getOpenFileName(
			qApp->activeWindow(),
			tr("Select the File"),
			QDir::currentPath(),
			tr("Time Series Input Files (*.txt *.csv)"));
	}
	else file = _fileName;
	if (file.isEmpty()) return;
	QString fileName = file;
	if (!parent->inflowFileNames.contains(fileName)) parent->inflowFileNames.append(fileName);
	parent->propModel()->setData(QModelIndex(), fileName, loadIndex);
	qDebug() << "set to saved index." << fileName;
	delete fileDialog;
	return;
}

