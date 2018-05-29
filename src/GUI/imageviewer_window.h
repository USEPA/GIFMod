#pragma once

#include <QWidget>
#include "ui_imageviewer_window.h"
#include "imageviewer.h"

class imageviewer_window : public QWidget
{
	Q_OBJECT

public:
	imageviewer_window(QWidget *parent = Q_NULLPTR);
	~imageviewer_window();

	void setimage(QString image_filename);

private:
	Ui::imageviewer_window ui;
	ImageViewer *image;

private slots:
	void close(); 
};
