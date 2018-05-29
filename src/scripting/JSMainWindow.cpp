//#include "JSMainWindow.h"
//#include "mainwindow.h"
//
//JSMainWindow::JSMainWindow(): Scriptable()
//{	
//	dukglue_register_global(ScriptingEngine::instance()->context(), this, Names::mainWindow::name());
//	registerPrototype();
//}
//
//JSMainWindow::~JSMainWindow(){}
//
//void JSMainWindow::zoomAll()
//{
//    IoC::resolveByName<MainWindow>(Names::mainWindow::name())->on_actionZoom_All_triggered();
//}
//void JSMainWindow::zoomIn()
//{
//	auto ptr = IoC::resolveByName<MainWindow>(Names::mainWindow::name());
//    ptr->on_actionZoom_In_triggered();
//}
//void JSMainWindow::zoomOut()
//{
//    IoC::resolveByName<MainWindow>(Names::mainWindow::name())->on_actionZoom_Out_triggered();
//}
//void JSMainWindow::addPond(){
//   IoC::resolveByName<MainWindow>(Names::mainWindow::name())->on_actionAdd_Pond_triggered();
//}
//
//void JSMainWindow::addSoil(){
//   IoC::resolveByName<MainWindow>(Names::mainWindow::name())->on_actionAdd_Soil_triggered();
//}
//
//void JSMainWindow::registerPrototype()
//{
//    dukglue_register_method(ScriptingEngine::instance()->context(), &JSMainWindow::zoomAll, "zoomAll");
//    dukglue_register_method(ScriptingEngine::instance()->context(), &JSMainWindow::zoomIn, "zoomIn");
//    dukglue_register_method(ScriptingEngine::instance()->context(), &JSMainWindow::zoomOut, "zoomOut");
//    dukglue_register_method(ScriptingEngine::instance()->context(), &JSMainWindow::addPond, "addPond");
//    dukglue_register_method(ScriptingEngine::instance()->context(), &JSMainWindow::addSoil, "addSoil");
//}
