/********************************************************************************
** Form generated from reading UI file 'mainwindowGWA.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOWGWA_H
#define UI_MAINWINDOWGWA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_New;
    QAction *action_Open;
    QAction *action_Save;
    QAction *actionSave_As;
    QAction *actionE_xit;
    QAction *action_Undo;
    QAction *action_redo;
    QAction *action_Cut;
    QAction *action_Copy;
    QAction *action_Paste;
    QAction *action_Select;
    QAction *actionSelect_All;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *action_Pan;
    QAction *actionContact_Us;
    QAction *actionAdd_Pond;
    QAction *actionAdd_Soil;
    QAction *actionAdd_Connector;
    QAction *actionZoom_All;
    QAction *actionlogWindow;
    QAction *actionAdd_Catchment_Area;
    QAction *actionAdd_Darcy_Block;
    QAction *actionAdd_Stora_ge;
    QAction *actionAdd_St_ream;
    QAction *actionDeterministic;
    QAction *actionBayesian;
    QAction *actionSoil;
    QAction *actionDarcy;
    QAction *actionStream;
    QAction *actionSoil_2;
    QAction *actionDarcy_2;
    QAction *actionCatchment_2;
    QAction *actionStream_2;
    QAction *actionSolver;
    QAction *actionParticulate_Phase;
    QAction *actionConstituents;
    QAction *actionReactions;
    QAction *actionRun_Model;
    QAction *actionLoad_Reaction_Network;
    QAction *actionSave_Reaction_Network;
    QAction *actionClear_Reaction_Network;
    QAction *actionRun_Model_from_Script;
    QAction *action_Hydraulic_Outputs;
    QAction *actionExport_to_Script_Language;
    QAction *actionAbout;
    QAction *actionAdd_Well;
    QAction *actionAdd_Tracer;
    QAction *actionRun_Inverse_Model;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menu_Help;
    QMenu *menu_Edit;
    QMenu *menu_Window;
    QMenu *menu_Model;
    QMenu *menuAdd_Block;
    QMenu *menuArray;
    QMenu *menuGrid;
    QMenu *menu_Post_Processing;
    QMenu *menuParameter_Estimation;
    QMenu *menuPreferences;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;
    QToolBar *toolBar_3;
    QToolBar *toolBar_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setWindowModality(Qt::NonModal);
        MainWindow->resize(1177, 791);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMouseTracking(true);
        QIcon icon;
        icon.addFile(QStringLiteral("Icons/GWA.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setWindowOpacity(1);
        MainWindow->setToolTipDuration(1);
        MainWindow->setIconSize(QSize(30, 30));
        MainWindow->setDocumentMode(false);
        MainWindow->setTabShape(QTabWidget::Rounded);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        action_New = new QAction(MainWindow);
        action_New->setObjectName(QStringLiteral("action_New"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("Icons/Document-Blank-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_New->setIcon(icon1);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QStringLiteral("action_Open"));
        QIcon icon2;
        icon2.addFile(QStringLiteral("Icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QStringLiteral("Icons/open.ico"), QSize(), QIcon::Normal, QIcon::On);
        action_Open->setIcon(icon2);
        action_Save = new QAction(MainWindow);
        action_Save->setObjectName(QStringLiteral("action_Save"));
        QIcon icon3;
        icon3.addFile(QStringLiteral("Icons/Save.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Save->setIcon(icon3);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionE_xit = new QAction(MainWindow);
        actionE_xit->setObjectName(QStringLiteral("actionE_xit"));
        action_Undo = new QAction(MainWindow);
        action_Undo->setObjectName(QStringLiteral("action_Undo"));
        QIcon icon4;
        icon4.addFile(QStringLiteral("Icons/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Undo->setIcon(icon4);
        action_redo = new QAction(MainWindow);
        action_redo->setObjectName(QStringLiteral("action_redo"));
        QIcon icon5;
        icon5.addFile(QStringLiteral("Icons/1439255778_Redo.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_redo->setIcon(icon5);
        action_Cut = new QAction(MainWindow);
        action_Cut->setObjectName(QStringLiteral("action_Cut"));
        QIcon icon6;
        icon6.addFile(QStringLiteral("Icons/cut-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Cut->setIcon(icon6);
        action_Copy = new QAction(MainWindow);
        action_Copy->setObjectName(QStringLiteral("action_Copy"));
        QIcon icon7;
        icon7.addFile(QStringLiteral("Icons/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Copy->setIcon(icon7);
        action_Paste = new QAction(MainWindow);
        action_Paste->setObjectName(QStringLiteral("action_Paste"));
        QIcon icon8;
        icon8.addFile(QStringLiteral("Icons/Paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Paste->setIcon(icon8);
        action_Select = new QAction(MainWindow);
        action_Select->setObjectName(QStringLiteral("action_Select"));
        action_Select->setCheckable(true);
        action_Select->setChecked(true);
        QIcon icon9;
        icon9.addFile(QStringLiteral("Icons/arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Select->setIcon(icon9);
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QStringLiteral("actionSelect_All"));
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName(QStringLiteral("actionZoom_In"));
        QIcon icon10;
        icon10.addFile(QStringLiteral("Icons/zoom_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_In->setIcon(icon10);
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QStringLiteral("actionZoom_Out"));
        QIcon icon11;
        icon11.addFile(QStringLiteral("Icons/zoom_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Out->setIcon(icon11);
        action_Pan = new QAction(MainWindow);
        action_Pan->setObjectName(QStringLiteral("action_Pan"));
        action_Pan->setCheckable(true);
        QIcon icon12;
        icon12.addFile(QStringLiteral("Icons/pan-icon.gif"), QSize(), QIcon::Normal, QIcon::Off);
        action_Pan->setIcon(icon12);
        actionContact_Us = new QAction(MainWindow);
        actionContact_Us->setObjectName(QStringLiteral("actionContact_Us"));
        actionAdd_Pond = new QAction(MainWindow);
        actionAdd_Pond->setObjectName(QStringLiteral("actionAdd_Pond"));
        QIcon icon13;
        icon13.addFile(QStringLiteral("Icons/well.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Pond->setIcon(icon13);
        actionAdd_Soil = new QAction(MainWindow);
        actionAdd_Soil->setObjectName(QStringLiteral("actionAdd_Soil"));
        QIcon icon14;
        icon14.addFile(QStringLiteral("Icons/atom.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Soil->setIcon(icon14);
        actionAdd_Connector = new QAction(MainWindow);
        actionAdd_Connector->setObjectName(QStringLiteral("actionAdd_Connector"));
        actionAdd_Connector->setCheckable(true);
        QIcon icon15;
        icon15.addFile(QStringLiteral("Icons/draw-connector.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Connector->setIcon(icon15);
        actionZoom_All = new QAction(MainWindow);
        actionZoom_All->setObjectName(QStringLiteral("actionZoom_All"));
        QIcon icon16;
        icon16.addFile(QStringLiteral("Icons/Full_screen_view.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_All->setIcon(icon16);
        actionlogWindow = new QAction(MainWindow);
        actionlogWindow->setObjectName(QStringLiteral("actionlogWindow"));
        actionlogWindow->setCheckable(true);
        QIcon icon17;
        icon17.addFile(QStringLiteral("Icons/log_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionlogWindow->setIcon(icon17);
        actionAdd_Catchment_Area = new QAction(MainWindow);
        actionAdd_Catchment_Area->setObjectName(QStringLiteral("actionAdd_Catchment_Area"));
        QIcon icon18;
        icon18.addFile(QStringLiteral("Icons/catchment.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Catchment_Area->setIcon(icon18);
        actionAdd_Darcy_Block = new QAction(MainWindow);
        actionAdd_Darcy_Block->setObjectName(QStringLiteral("actionAdd_Darcy_Block"));
        QIcon icon19;
        icon19.addFile(QStringLiteral("Icons/Darcy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Darcy_Block->setIcon(icon19);
        actionAdd_Stora_ge = new QAction(MainWindow);
        actionAdd_Stora_ge->setObjectName(QStringLiteral("actionAdd_Stora_ge"));
        QIcon icon20;
        icon20.addFile(QStringLiteral("Icons/Storage.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Stora_ge->setIcon(icon20);
        actionAdd_St_ream = new QAction(MainWindow);
        actionAdd_St_ream->setObjectName(QStringLiteral("actionAdd_St_ream"));
        QIcon icon21;
        icon21.addFile(QStringLiteral("Icons/Stream.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_St_ream->setIcon(icon21);
        actionDeterministic = new QAction(MainWindow);
        actionDeterministic->setObjectName(QStringLiteral("actionDeterministic"));
        actionBayesian = new QAction(MainWindow);
        actionBayesian->setObjectName(QStringLiteral("actionBayesian"));
        actionSoil = new QAction(MainWindow);
        actionSoil->setObjectName(QStringLiteral("actionSoil"));
        actionDarcy = new QAction(MainWindow);
        actionDarcy->setObjectName(QStringLiteral("actionDarcy"));
        actionStream = new QAction(MainWindow);
        actionStream->setObjectName(QStringLiteral("actionStream"));
        actionSoil_2 = new QAction(MainWindow);
        actionSoil_2->setObjectName(QStringLiteral("actionSoil_2"));
        actionDarcy_2 = new QAction(MainWindow);
        actionDarcy_2->setObjectName(QStringLiteral("actionDarcy_2"));
        actionCatchment_2 = new QAction(MainWindow);
        actionCatchment_2->setObjectName(QStringLiteral("actionCatchment_2"));
        actionStream_2 = new QAction(MainWindow);
        actionStream_2->setObjectName(QStringLiteral("actionStream_2"));
        actionSolver = new QAction(MainWindow);
        actionSolver->setObjectName(QStringLiteral("actionSolver"));
        actionParticulate_Phase = new QAction(MainWindow);
        actionParticulate_Phase->setObjectName(QStringLiteral("actionParticulate_Phase"));
        actionConstituents = new QAction(MainWindow);
        actionConstituents->setObjectName(QStringLiteral("actionConstituents"));
        actionReactions = new QAction(MainWindow);
        actionReactions->setObjectName(QStringLiteral("actionReactions"));
        actionRun_Model = new QAction(MainWindow);
        actionRun_Model->setObjectName(QStringLiteral("actionRun_Model"));
        QIcon icon22;
        icon22.addFile(QStringLiteral("Icons/gear_run.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon22.addFile(QStringLiteral("Icons/gear_run.png"), QSize(), QIcon::Normal, QIcon::On);
        actionRun_Model->setIcon(icon22);
        actionLoad_Reaction_Network = new QAction(MainWindow);
        actionLoad_Reaction_Network->setObjectName(QStringLiteral("actionLoad_Reaction_Network"));
        actionSave_Reaction_Network = new QAction(MainWindow);
        actionSave_Reaction_Network->setObjectName(QStringLiteral("actionSave_Reaction_Network"));
        actionClear_Reaction_Network = new QAction(MainWindow);
        actionClear_Reaction_Network->setObjectName(QStringLiteral("actionClear_Reaction_Network"));
        actionRun_Model_from_Script = new QAction(MainWindow);
        actionRun_Model_from_Script->setObjectName(QStringLiteral("actionRun_Model_from_Script"));
        QIcon icon23;
        icon23.addFile(QStringLiteral("Icons/runScript.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun_Model_from_Script->setIcon(icon23);
        action_Hydraulic_Outputs = new QAction(MainWindow);
        action_Hydraulic_Outputs->setObjectName(QStringLiteral("action_Hydraulic_Outputs"));
        actionExport_to_Script_Language = new QAction(MainWindow);
        actionExport_to_Script_Language->setObjectName(QStringLiteral("actionExport_to_Script_Language"));
        QIcon icon24;
        icon24.addFile(QStringLiteral("Icons/generate script.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExport_to_Script_Language->setIcon(icon24);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAdd_Well = new QAction(MainWindow);
        actionAdd_Well->setObjectName(QStringLiteral("actionAdd_Well"));
        actionAdd_Well->setIcon(icon13);
        actionAdd_Tracer = new QAction(MainWindow);
        actionAdd_Tracer->setObjectName(QStringLiteral("actionAdd_Tracer"));
        actionAdd_Tracer->setIcon(icon14);
        actionRun_Inverse_Model = new QAction(MainWindow);
        actionRun_Inverse_Model->setObjectName(QStringLiteral("actionRun_Inverse_Model"));
        QIcon icon25;
        icon25.addFile(QStringLiteral("Icons/backwards-running.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun_Inverse_Model->setIcon(icon25);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1177, 21));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QStringLiteral("menu_Help"));
        menu_Edit = new QMenu(menuBar);
        menu_Edit->setObjectName(QStringLiteral("menu_Edit"));
        menu_Window = new QMenu(menuBar);
        menu_Window->setObjectName(QStringLiteral("menu_Window"));
        menu_Model = new QMenu(menuBar);
        menu_Model->setObjectName(QStringLiteral("menu_Model"));
        menuAdd_Block = new QMenu(menu_Model);
        menuAdd_Block->setObjectName(QStringLiteral("menuAdd_Block"));
        menuArray = new QMenu(menu_Model);
        menuArray->setObjectName(QStringLiteral("menuArray"));
        menuGrid = new QMenu(menu_Model);
        menuGrid->setObjectName(QStringLiteral("menuGrid"));
        menu_Post_Processing = new QMenu(menuBar);
        menu_Post_Processing->setObjectName(QStringLiteral("menu_Post_Processing"));
        menuParameter_Estimation = new QMenu(menuBar);
        menuParameter_Estimation->setObjectName(QStringLiteral("menuParameter_Estimation"));
        menuPreferences = new QMenu(menuBar);
        menuPreferences->setObjectName(QStringLiteral("menuPreferences"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMouseTracking(true);
        mainToolBar->setWindowOpacity(1);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(MainWindow);
        toolBar_2->setObjectName(QStringLiteral("toolBar_2"));
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar_2);
        toolBar_3 = new QToolBar(MainWindow);
        toolBar_3->setObjectName(QStringLiteral("toolBar_3"));
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar_3);
        toolBar_4 = new QToolBar(MainWindow);
        toolBar_4->setObjectName(QStringLiteral("toolBar_4"));
        toolBar_4->setEnabled(true);
        MainWindow->addToolBar(Qt::LeftToolBarArea, toolBar_4);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_Edit->menuAction());
        menuBar->addAction(menu_Window->menuAction());
        menuBar->addAction(menu_Model->menuAction());
        menuBar->addAction(menu_Post_Processing->menuAction());
        menuBar->addAction(menuParameter_Estimation->menuAction());
        menuBar->addAction(menuPreferences->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menu_File->addAction(action_New);
        menu_File->addAction(action_Open);
        menu_File->addAction(action_Save);
        menu_File->addAction(actionSave_As);
        menu_File->addSeparator();
        menu_File->addAction(actionE_xit);
        menu_Help->addAction(actionContact_Us);
        menu_Help->addAction(actionAbout);
        menu_Edit->addAction(action_Undo);
        menu_Edit->addAction(action_redo);
        menu_Edit->addSeparator();
        menu_Edit->addAction(actionSelect_All);
        menu_Edit->addSeparator();
        menu_Edit->addAction(action_Cut);
        menu_Edit->addAction(action_Copy);
        menu_Edit->addAction(action_Paste);
        menu_Window->addAction(actionZoom_In);
        menu_Window->addAction(actionZoom_Out);
        menu_Window->addAction(actionZoom_All);
        menu_Window->addSeparator();
        menu_Window->addAction(action_Select);
        menu_Window->addAction(action_Pan);
        menu_Window->addSeparator();
        menu_Model->addAction(menuAdd_Block->menuAction());
        menu_Model->addAction(actionAdd_Connector);
        menu_Model->addAction(menuArray->menuAction());
        menu_Model->addAction(menuGrid->menuAction());
        menu_Model->addAction(actionParticulate_Phase);
        menu_Model->addAction(actionConstituents);
        menu_Model->addAction(actionReactions);
        menu_Model->addSeparator();
        menu_Model->addAction(actionRun_Model);
        menu_Model->addAction(actionExport_to_Script_Language);
        menu_Model->addAction(actionlogWindow);
        menuAdd_Block->addAction(actionAdd_Pond);
        menuAdd_Block->addAction(actionAdd_Soil);
        menuAdd_Block->addAction(actionAdd_Catchment_Area);
        menuAdd_Block->addAction(actionAdd_Darcy_Block);
        menuAdd_Block->addAction(actionAdd_Stora_ge);
        menuAdd_Block->addAction(actionAdd_St_ream);
        menuArray->addAction(actionSoil);
        menuArray->addAction(actionDarcy);
        menuArray->addAction(actionStream);
        menuGrid->addAction(actionSoil_2);
        menuGrid->addAction(actionDarcy_2);
        menuGrid->addAction(actionCatchment_2);
        menuGrid->addAction(actionStream_2);
        menu_Post_Processing->addAction(action_Hydraulic_Outputs);
        menuParameter_Estimation->addAction(actionDeterministic);
        menuParameter_Estimation->addAction(actionBayesian);
        menuPreferences->addAction(actionSolver);
        mainToolBar->addAction(action_New);
        mainToolBar->addAction(action_Open);
        mainToolBar->addAction(action_Save);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_Undo);
        mainToolBar->addAction(action_redo);
        mainToolBar->addSeparator();
        mainToolBar->addAction(action_Cut);
        mainToolBar->addAction(action_Copy);
        mainToolBar->addAction(action_Paste);
        toolBar->addAction(actionAdd_Well);
        toolBar->addSeparator();
        toolBar->addAction(actionAdd_Tracer);
        toolBar_2->addAction(actionZoom_In);
        toolBar_2->addAction(actionZoom_Out);
        toolBar_2->addAction(actionZoom_All);
        toolBar_4->addAction(actionRun_Model);
        toolBar_4->addAction(actionRun_Inverse_Model);
        toolBar_4->addAction(actionlogWindow);
        toolBar_4->addSeparator();
        toolBar_4->addAction(actionExport_to_Script_Language);
        toolBar_4->addAction(actionRun_Model_from_Script);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Versatile Water Quality Modeling (WQVers)", 0));
        action_New->setText(QApplication::translate("MainWindow", "&New", 0));
        action_Open->setText(QApplication::translate("MainWindow", "&Open", 0));
        action_Open->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        action_Save->setText(QApplication::translate("MainWindow", "&Save", 0));
        action_Save->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save &As", 0));
        actionE_xit->setText(QApplication::translate("MainWindow", "E&xit", 0));
        action_Undo->setText(QApplication::translate("MainWindow", "&Undo", 0));
        action_Undo->setShortcut(QApplication::translate("MainWindow", "Ctrl+Z", 0));
        action_redo->setText(QApplication::translate("MainWindow", "&Redo", 0));
        action_Cut->setText(QApplication::translate("MainWindow", "C&ut", 0));
        action_Copy->setText(QApplication::translate("MainWindow", "&Copy", 0));
        action_Copy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0));
        action_Paste->setText(QApplication::translate("MainWindow", "&Paste", 0));
        action_Paste->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", 0));
        action_Select->setText(QApplication::translate("MainWindow", "&Select", 0));
        actionSelect_All->setText(QApplication::translate("MainWindow", "Select &All", 0));
        actionZoom_In->setText(QApplication::translate("MainWindow", "Zoom &In", 0));
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom &Out", 0));
        action_Pan->setText(QApplication::translate("MainWindow", "&Pan", 0));
        actionContact_Us->setText(QApplication::translate("MainWindow", "Contact &Us", 0));
        actionAdd_Pond->setText(QApplication::translate("MainWindow", "Add &Pond", 0));
        actionAdd_Soil->setText(QApplication::translate("MainWindow", "Add &Soil", 0));
        actionAdd_Connector->setText(QApplication::translate("MainWindow", "Interface", 0));
        actionZoom_All->setText(QApplication::translate("MainWindow", "Zoom &All", 0));
        actionlogWindow->setText(QApplication::translate("MainWindow", "Open Log Window", 0));
        actionlogWindow->setShortcut(QApplication::translate("MainWindow", "Ctrl+L", 0));
        actionAdd_Catchment_Area->setText(QApplication::translate("MainWindow", "Add Catchment", 0));
        actionAdd_Darcy_Block->setText(QApplication::translate("MainWindow", "Add &Darcy Block", 0));
        actionAdd_Stora_ge->setText(QApplication::translate("MainWindow", "Add Stora&ge", 0));
        actionAdd_St_ream->setText(QApplication::translate("MainWindow", "Add St&ream", 0));
        actionDeterministic->setText(QApplication::translate("MainWindow", "Deterministic", 0));
        actionBayesian->setText(QApplication::translate("MainWindow", "Bayesian", 0));
        actionSoil->setText(QApplication::translate("MainWindow", "Soil", 0));
        actionDarcy->setText(QApplication::translate("MainWindow", "Darcy", 0));
        actionStream->setText(QApplication::translate("MainWindow", "Stream", 0));
        actionSoil_2->setText(QApplication::translate("MainWindow", "Soil", 0));
        actionDarcy_2->setText(QApplication::translate("MainWindow", "Darcy", 0));
        actionCatchment_2->setText(QApplication::translate("MainWindow", "Catchment", 0));
        actionStream_2->setText(QApplication::translate("MainWindow", "Stream", 0));
        actionSolver->setText(QApplication::translate("MainWindow", "Solver Options", 0));
        actionParticulate_Phase->setText(QApplication::translate("MainWindow", "Particulate Phase", 0));
        actionConstituents->setText(QApplication::translate("MainWindow", "Constituents", 0));
        actionReactions->setText(QApplication::translate("MainWindow", "Reactions", 0));
        actionRun_Model->setText(QApplication::translate("MainWindow", "Run Model", 0));
        actionRun_Model->setShortcut(QApplication::translate("MainWindow", "F5", 0));
        actionLoad_Reaction_Network->setText(QApplication::translate("MainWindow", "Load Reaction Network", 0));
        actionSave_Reaction_Network->setText(QApplication::translate("MainWindow", "Save Reaction Network", 0));
#ifndef QT_NO_TOOLTIP
        actionSave_Reaction_Network->setToolTip(QApplication::translate("MainWindow", "Save Reaction Network", 0));
#endif // QT_NO_TOOLTIP
        actionClear_Reaction_Network->setText(QApplication::translate("MainWindow", "Clear Reaction Network", 0));
        actionRun_Model_from_Script->setText(QApplication::translate("MainWindow", "Run Model from Script", 0));
        action_Hydraulic_Outputs->setText(QApplication::translate("MainWindow", "&Hydraulic Outputs", 0));
        actionExport_to_Script_Language->setText(QApplication::translate("MainWindow", "Export to &Script Language", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About WQVers", 0));
        actionAdd_Well->setText(QApplication::translate("MainWindow", "Add &Well", 0));
#ifndef QT_NO_TOOLTIP
        actionAdd_Well->setToolTip(QApplication::translate("MainWindow", "Add Well", 0));
#endif // QT_NO_TOOLTIP
        actionAdd_Tracer->setText(QApplication::translate("MainWindow", "Add &Tracer", 0));
#ifndef QT_NO_TOOLTIP
        actionAdd_Tracer->setToolTip(QApplication::translate("MainWindow", "Add Tracer", 0));
#endif // QT_NO_TOOLTIP
        actionRun_Inverse_Model->setText(QApplication::translate("MainWindow", "runInverse", 0));
        menu_File->setTitle(QApplication::translate("MainWindow", "&File", 0));
        menu_Help->setTitle(QApplication::translate("MainWindow", "&Help", 0));
        menu_Edit->setTitle(QApplication::translate("MainWindow", "&Edit", 0));
        menu_Window->setTitle(QApplication::translate("MainWindow", "&View", 0));
        menu_Model->setTitle(QApplication::translate("MainWindow", "&Model", 0));
        menuAdd_Block->setTitle(QApplication::translate("MainWindow", "Block", 0));
        menuArray->setTitle(QApplication::translate("MainWindow", "Array", 0));
        menuGrid->setTitle(QApplication::translate("MainWindow", "Grid", 0));
        menu_Post_Processing->setTitle(QApplication::translate("MainWindow", "&Post Processing", 0));
        menuParameter_Estimation->setTitle(QApplication::translate("MainWindow", "Parameter Estimation", 0));
        menuPreferences->setTitle(QApplication::translate("MainWindow", "Preferences", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
        toolBar_2->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", 0));
        toolBar_3->setWindowTitle(QApplication::translate("MainWindow", "toolBar_3", 0));
        toolBar_4->setWindowTitle(QApplication::translate("MainWindow", "toolBar_4", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOWGWA_H
