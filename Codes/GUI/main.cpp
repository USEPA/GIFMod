
#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "qdebug.h"

#ifdef GIFMOD
int main(int argc, char *argv[])
{
	QApplication GUI(argc, argv);
//	QString modelfilename = "C:\\Users\\Sassan\\Documents\\GIFModers Projects\\inflow.GIFMod";
//	QString q = argv[0];
//	if (argc>1) modelfilename = argv[1];
//	MainWindow mainGUIwindow(0, "Versatile Water Quality Modeling System", "GIFModers", "GIFMod", "GIFModGUIPropList.csv", q+"  "+ modelfilename.replace("\\","/"));
	MainWindow mainGUIwindow(0, "Green Infrastructure Flexible Modeling System", "GIFMod", "GIFM", "GIFModGUIPropList.csv");
	mainGUIwindow.show();
	mainGUIwindow.afterInit();
	QEventLoop loop;
	return GUI.exec();
}
#endif

#ifdef GWA
int main(int argc, char *argv[])
{

	QApplication GUI(argc, argv);
	//	QString modelfilename = "C:\\Users\\Sassan\\Documents\\GIFModers Projects\\inflow.GIFMod";
	//	QString q = argv[0];
	//	if (argc>1) modelfilename = argv[1];
	//	MainWindow mainGUIwindow(0, "Versatile Water Quality Modeling System", "GIFModers", "GIFMod", "GIFModGUI/PropList.csv", q+"  "+ modelfilename.replace("\\","/"));
	MainWindow mainGUIwindow(0, "Ground Water Age Modeling System", "GWA", "GWA", "GWAGUIPropList.csv");
	mainGUIwindow.show();
	QEventLoop loop;
	return GUI.exec();
}
#endif