#include "filebutton.h"
#include "qfiledialog.h"

FileButton::FileButton(QWidget *parent)
	: QPushButton(parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(on_clicked()));
}

FileButton::~FileButton()
{

}

void FileButton::on_clicked()
{
	filename = QFileDialog::getOpenFileName(this,
		tr("Time Series"), "", tr("Text (*.txt);;csv (*.csv);;All Files (*)"));
	this->setText(filename);

}
