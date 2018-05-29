#include "imageviewer_window.h"
#include "imageviewer.h"

imageviewer_window::imageviewer_window(QWidget *parent)
	: QWidget(parent)
{
	image = new ImageViewer(true);
	image->setObjectName(QStringLiteral("image"));
	ui.setupUi(this);
	ui.verticalLayout->addWidget(image,0);
	connect(image, SIGNAL(close), this, SLOT(close()));
	
}



imageviewer_window::~imageviewer_window()
{
}

void imageviewer_window::setimage(QString image_filename)
{
    image->loadFile(image_filename, true);
}

void imageviewer_window::close()
{
	this->destroy();
}
