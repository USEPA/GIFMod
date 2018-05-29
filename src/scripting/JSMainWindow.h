//#pragma once
//
//#include "scriptingengine.h"
//
//
//class JSMainWindow : public Scriptable 
//{
//	
//public:
//	JSMainWindow();
//    ~JSMainWindow();
//
//    void zoomAll();
//    void zoomIn();
//    void zoomOut();
//    void addPond();
//    void addSoil();
//
//
////    void  on_actionAdd_Catchment_Area_triggered();
//
////    void  on_actionAdd_Darcy_Block_triggered();
//
////    void  on_actionAdd_Stora_ge_triggered();
//
////    void on_actionAdd_St_ream_triggered();
//
////    //    void on_actionAdd_Interface_triggered();
//
////    void on_actionAdd_Connector_triggered(bool checked);
//
////    void on_action_Select_triggered(bool checked);
//
////    void on_action_Pan_triggered(bool checked);
//
////    void on_actionProperty_Box_triggered(bool checked);
//
////    void MainWindow::updateToolbarToggles(Operation_Modes OMode = Operation_Modes::NormalMode);
////    void on_action_New_triggered();
//
////    void on_action_Open_triggered();
//
////    void on_action_Save_triggered();
////    void on_actionSave_As_triggered();
////    bool saveModel(QString &);
////    void on_actionZoom_In_triggered();
//
////    void on_actionZoom_Out_triggered();
//
////    void on_actionZoom_All_triggered();
//
////    void on_actionE_xit_triggered();
//
////    void on_action_Undo_triggered();
//
////    void on_action_Redo_triggered();
//
////    void on_actionClear_Reaction_Network_triggered();
//
////    void on_actionLoad_Reaction_Network_triggered();
//
////    void on_actionSave_Reaction_Network_triggered();
//
////    void on_actionReactions_triggered();
//
////    //	void Mouse_Pos();
//
////    void on_projectExplorer_clicked(const QModelIndex &index);
//
////    void on_projectExplorer_customContextMenuRequested(const QPoint &pos);
//
////    void on_actionmenuRecent_triggered();// QString fileName);
//
//////	void on_tableProp_customContextMenuRequested(const QPoint &pos);
//
////    void tablePropShowContextMenu(const QPoint&);
////    void addParameter(QAction*);
//
////    QModelIndex addParameterIndex(const QModelIndex &index = QModelIndex());
////    //	void addParameter(QString variableName = "");
//
////    void openRXNWindow();
////    void removeProjectExplorerNodeItem(QString name = "", const QModelIndex = QModelIndex()); // TreeModel *model = 0, const QString name = "", const QModelIndex = QModelIndex());
////    void removeProjectExplorerEdgeItem(QString name = "", const QModelIndex = QModelIndex()); // TreeModel *model = 0, const QString name = "", const QModelIndex = QModelIndex());
////    void removeProjectExplorerEntityItem(QString name = "", const QModelIndex = QModelIndex()); // TreeModel *model = 0, const QString name = "", const QModelIndex = QModelIndex());
////    void showLocalSensitivityMatrix();
////    void showGlobalSensitivityMatrix();
////    void showCorrelationMatrix();
////    void showHelp(int code = -1, QString variableName = "");
////    void on_actionRun_Model_triggered();
////    void on_actionRun_Inverse_Model_triggered();
////    void on_actionRun_Model_from_Script_triggered();
////    void on_action_Hydraulic_Outputs_triggered();
////    void on_actionlogWindow_triggered();
////    void on_actionExport_to_Script_Language_triggered();
////    void on_actionAdd_Well_triggered();
////    void on_actionAdd_Tracer_triggered();
////    void on_actionAbout_triggered();
////    void on_actioncolorCodedResults_triggered();
////    void on_actioncolorCodeStorage_triggered();
////    void on_actioncolorCodeHead_triggered();
////    void on_actioncolorCodeMoistureContent_triggered();
////    void on_actioncolorCodeWaterDepth_triggered();
////    void on_actioncolorCodeEvaporationRate_triggered();
////    void on_actionColorCodeConnectorFlow_triggered();
////    void on_actionColorCodeConnectorVelocity_triggered();
////    void on_actionColorCodeConnectorArea_triggered();
////    void on_actionColorCodeConnectorVaporExchangeEate_triggered();
////    void on_actionProjectSettings_triggered();
////    void on_actionClimateSettings_triggered();
////    void on_actionSolverSettings_triggered();
////    void on_actionNewExperiment_triggered();
////    void on_actionCopyFromCurrentExperiment_triggered();
////    void addExperiment(QString sourceExperiment = "");
////    void on_actionremoveCurrentExperiment_triggered();
////    void on_actionShowRuntimeWindow_triggered();
////    void on_actionRecent_triggered();
////    void on_actionReset_colors_triggered();
////    void menuWaterQuality_triggered();
////    void on_actionNew_from_template_triggered();
////    void on_actionCommandLine_triggered();
////    void waterQualityPostProcessing_clicked();
////    void on_actionContact_Us_triggered();
////    void on_actionContact_Us_hovered();
////    void removeFromRecentList(QAction*);
////    void writeRecentFilesList();
////    bool loadModel(QString modelfilename);
////    void on_actionAdd_Plant_Block_triggered();
//
//protected:
//	
//	// Inherited via ScriptableGlobalObject
//	virtual void registerPrototype() override;
//
//};
//
