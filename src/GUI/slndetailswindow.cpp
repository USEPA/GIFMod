#include "slndetailswindow.h"
#include "runtimeWindow.h"

slndetailswindow::slndetailswindow(runtimeWindow *_parent)
	: QDialog(_parent)
{
	parent = _parent;
	ui.setupUi(this);
	connect(ui.pushClose, SIGNAL(clicked()), this, SLOT(close_window()));
}

slndetailswindow::~slndetailswindow()
{
    parent->sln_dtl_active = false;
}

void slndetailswindow::close_window()
{
	parent->sln_dtl_active = false;
	close(); 
}
