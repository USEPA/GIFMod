#pragma once

#include <QWidget>
#include <qdialog.h>
#include "ui_slndetailswindow.h"

class runtimeWindow; 
class slndetailswindow : public QDialog
{
	Q_OBJECT

public:
	slndetailswindow(runtimeWindow *parent = Q_NULLPTR);
	runtimeWindow *parent; 
	~slndetailswindow();
	void append(QString s)
	{
		ui.textEdit->append(s);
	}
private slots:
	void close_window();

private:
	Ui::slndetailswindow ui;
};
