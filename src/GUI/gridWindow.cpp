#ifdef GIFMOD
#include "gridWindow.h"
#include "ui_gridWindow.h"
#include "qtableview.h"
#include "node.h"
#include "GWidget.h"
#include "UnitTextBox.h"
#include <QDoubleValidator>


gridWindow::gridWindow(QWidget *parent, XString z0, QString type, bool canChangeType, XString length, XString width, XString height) :
    QDialog(parent),
    ui(new Ui::gridWindow)
{
	//this->z0 = z0;
	this->height = height;
	ui->setupUi(this);
	//type = "horizontal";
	if (type.toLower().contains("h"))
	{
		ui->radioButtonH->setChecked(true);
		ui->groupBoxV->hide();
//		ui->groupBoxH->show();
	}
	else
	{
		ui->radioButtonV->setChecked(true);
//		ui->groupBoxV->show();
		ui->groupBoxH->hide();

	}
	QString text = ui->heightLbl->text() + height.toStringUnit();
	ui->heightLbl->setText(ui->heightLbl->text() + height.toStringUnit());// text);
	text = ui->z0Lbl->text() + z0.toStringUnit();
	ui->z0Lbl->setText(ui->z0Lbl->text() + z0.toStringUnit());// text);
	validator = new QDoubleValidator(this);
	ui->deltaHUnitBox->setValidator(validator);
	ui->deltaVUnitBox->setValidator(validator);
	ui->deltaHXUnitBox->setValidator(validator);
	ui->deltaHYUnitBox->setValidator(validator);
	QDoubleValidator * validator2 = new QDoubleValidator(this);
	validator2->setBottom(0);
	ui->lengthUnitBox->setValidator(validator2);
	ui->lengthXUnitBox->setValidator(validator2);
	ui->lengthYUnitBox->setValidator(validator2);
	
	XString dh;
	dh = height;
	dh.setNum(0); 
	
	ui->deltaHUnitBox->setXString(dh);
	ui->deltaVUnitBox->setXString(dh);
	ui->lengthUnitBox->setXString(length);
	
	
	ui->deltaHXUnitBox->setXString(dh);
	ui->deltaHYUnitBox->setXString(dh);
	ui->lengthXUnitBox->setXString(length);
	ui->lengthYUnitBox->setXString(width);

	r0 = length;
	r0.setNum(0);
	ui->r0UnitBox->setXString(r0);

	//	ui->deltaHUnitBox->setUnitsList(z0.unitsList);
//	ui->deltaHUnitBox->setUnit(z0.unit);
	if (!canChangeType)
	{
		ui->radioButtonH->setEnabled(false);
		ui->radioButtonV->setEnabled(false);
		ui->radioButtonR->setEnabled(false);
	}
	updateR0Label(); 
	

}

void gridWindow::on_columnsSpin_valueChanged(int columns)
{

}

void gridWindow::on_rowsSpin_valueChanged(int rows)
{
	ui->deltaVUnitBox->setText(QString::number(-(rows - 1)*height.toFloat(ui->deltaVUnitBox->toXString().unit)));
}

void gridWindow::accept()
{
	accepted = true;
}

void gridWindow::reject()
{
	accepted = false;
}

gridWindow::~gridWindow()
{
	delete validator;
}

QMap<QString, XString> gridWindow::getGridInfo()
{
	if (accepted)
		return results;
	return QMap < QString, XString > ();
}
	
void gridWindow::on_buttonBox_clicked(QAbstractButton * button)
{
	//qDebug() << button->text();
	if (button->text() == "OK")
	{
		accepted = true;
		results["Number of Columns"] = ui->columnsSpin->text();
		results["Number of Rows"] = ui->rowsSpin->text();
		if (ui->radioButtonR->isChecked())
		{
			results["Radially symmetrical array"] = '1';
			results["delta H"] = ui->deltaHUnitBox->value();
			results["delta V"] = ui->deltaVUnitBox->value();
			results["length"] = ui->lengthUnitBox->value();
			results["r0"] = ui->r0UnitBox->value();
		}
		else if (ui->radioButtonV->isChecked())
		{
			results["2D Vertical"] = '1';
			results["delta H"] = ui->deltaHUnitBox->value();
			results["delta V"] = ui->deltaVUnitBox->value();
			results["length"] = ui->lengthUnitBox->value();
		}
		else
		{
			results["2D Horizontal"] = '1';
			results["delta HX"] = ui->deltaHXUnitBox->value();
			results["delta HY"] = ui->deltaHYUnitBox->value();
			results["lengthX"] = ui->lengthXUnitBox->value();
			results["lengthY"] = ui->lengthYUnitBox->value();
		}

	}
	if (button->text() == "Cancel")
	{
		accepted = false;
	}
	hide();

}

void gridWindow::closeEvent(QCloseEvent *event)
{
	reject();
	hide();
}

void gridWindow::updateR0Label()
{
	bool stat = ui->radioButtonR->isChecked();
	if (stat)
	{
		ui->r0frame->show();
		//ui->r0UnitBox->show();
		//ui->label_10->show();
	}
	else
	{
		ui->r0frame->hide();
		//ui->r0UnitBox->hide();
		//ui->label_10->hide();
	}

}

void gridWindow::on_radioButtonH_clicked(bool checked)
{
	if (checked)
	{
		ui->groupBoxH->show();
	}
}
void gridWindow::on_radioButtonV_clicked(bool checked)
{
	updateR0Label();
}
void gridWindow::on_radioButtonH_toggled(bool checked)
{
	if (checked)
	{
		ui->groupBoxV->hide();
		ui->groupBoxH->show();
	}
	else
	{
		ui->groupBoxH->hide();
		ui->groupBoxV->show();
	}
	updateR0Label();
}
void gridWindow::on_radioButtonR_toggled(bool checked)
{
	if (checked || ui->radioButtonV->isChecked())
	{
		ui->groupBoxH->hide();
		ui->groupBoxV->show();
	}
	else
	{
		ui->groupBoxV->hide();
		ui->groupBoxH->show();
	}
	updateR0Label();
}

void gridWindow::on_radioButtonV_toggled(bool checked)
{
	if (checked || ui->radioButtonR->isChecked())
	{
		ui->groupBoxH->hide();
		ui->groupBoxV->show();
	}
	else
	{
		ui->groupBoxV->hide();
		ui->groupBoxH->show();
	}
	updateR0Label();
}
#endif
