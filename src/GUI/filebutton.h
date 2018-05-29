#ifndef FILEBUTTON_H
#define FILEBUTTON_H

#include <QPushButton>

class FileButton : public QPushButton
{
	Q_OBJECT

public:
	FileButton(QWidget *parent);
	~FileButton();
	QString filename; 
private:
	
public slots:
	void on_clicked(); 
};

#endif // FILEBUTTON_H
