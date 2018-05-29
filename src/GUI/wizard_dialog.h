#ifndef WIZARD_DIALOG_H
#define WIZARD_DIALOG_H

#include <QWidget>
#include <qdialog.h>
#include "ui_wizard_dialog.h"
#include "Wizard/Wizard_Script_Reader.h"
#include "qlabel.h"
#include "qcombobox.h"
#include "qdatetimeedit.h"
#include "qlineedit.h"
#include "qformlayout.h"
#include "qcheckbox.h"
#include "UnitTextBox3.h"
#include "mProplist.h"
#include "qspinbox.h"
#include "Command.h"
#include "filebutton.h"
#include "imageviewer.h"

class MainWindow; 

class Wizard_Dialog : public QDialog
{
	Q_OBJECT

public:
	Wizard_Dialog(QString *template_selected, MainWindow *parent = 0);
	~Wizard_Dialog();
	void setup_form();
	MainWindow *_parent; 
	void set_wiz_parameters();
	QString _template_selected;
	void save_script(QList<CCommand> commands);
private:
	Ui::Wizard_Dialog ui;
	Wizard_Script_Reader wiz;
	QTabWidget *tabWidget;
	ImageViewer *image;
	QMap<QString, QLabel*> labels;
	QMap<QString, QLineEdit*> textedits;
	QMap<QString, UnitTextBox3*> unittextedits;
	QMap<QString, QComboBox*>  comboboxes;
	QMap<QString, QDateTimeEdit*>  datetimepickers;
	QMap<QString, FileButton*>  filetextedits;
	QMap<QString, QSpinBox*>  spinboxes;
	QMap<QString, QCheckBox*>  checkboxes;
	QMap<QString, QWidget*> tabs;
	QFormLayout *formLayout;
	mPropList *mproplist;
	QList<CCommand> commands;
	
private slots:
	void on_next_clicked();
	void on_ok_clicked();
	void on_create_script_clicked();
	void on_tab_changed();
	void on_cancel_clicked();
	void on_image_double_click();
	
};

#endif // WIZARD_DIALOG_H
