#pragma once
#include <QMainWindow>
#include <QPaintEvent>
#include <QtGui>
#include <QtCore>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QTreeView.h>
#include "treemodel.h"
#include <qprogressbar>
#include "qtableview.h"
#include "runtimeWindow.h"
#include "btc.h"
#include "results.h"
#include <QDesktopWidget>

class GraphWidget;
class CMedium;
class CMediumSet;
class logWindow;
class CGWA;

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
//	QString applicationName = "Green InfraStructure Modeling";

	QGraphicsView *view;
	GraphWidget *mainGraphWidget;
	QGraphicsScene *mainGraphicsScene;
	QMainWindow *mainW;
	QDockWidget *dockWidget;
	QDockWidget *dockProp;
	QWidget *dockWidgetContents;
	QWidget *dockPropContents;
	QVBoxLayout *verticalLayout;
	QDockWidget *dockProjectExplorer;
	QWidget *projectExplorerContents;
	QVBoxLayout *verticalLayout_1;
	QVBoxLayout *verticalLayout_2;
	QTableView *tableProp;
	QTreeView *projectExplorer;
	//	QTableWidget *tableProp;
	//	QStandardItemModel *propModel;

	//	PropModel *propModel;
	//	ProjModel *projModel;
	TreeModel *projModel;

	explicit MainWindow(QWidget *parent, QString applicationName, QString shortName, QString extension, QString metafilename, QString modeltoLoad = "");
	void addToRecentFiles(QString fileName, bool addToFile=true);
	~MainWindow();
	QStringList recentFiles;

#ifdef GIFMOD
	void forwardRun(CMediumSet *model, runtimeWindow *runtimewindow);
	void inverseRun(CMediumSet *model, runtimeWindow* progress);
#endif
#ifdef GWA
	void forwardRun(CGWA *model, runtimeWindow *runtimewindow);
	void inverseRun(CGWA *model, runtimeWindow* progress);
#endif

	//void forwardRun(CMedium *model) { forwardRun(model, new runtimeWindow); };
	void setModelFileName(const QString &fileName);
	QString modelPathname();
	QString applicationName, fileExtension, metafilename, applicationShortName;
	logWindow *log;
	void MainWindow::centerWidget();
	Ui::MainWindow *ui;
	runtimeWindow * rtw = 0;
	QList<QMap<QString, QVariant>>* dataMap;
	QList<QMap<QString, QVariant>>* dataHash;
	void afterInit() {
		readRecentFilesList();
	}
	void readRecentFilesList();
protected:
	void paintEvent(QPaintEvent *e);

public slots:

	void Mouse_Pos(int, int, QString);

	void on_dockWidget_dockLocationChanged(const Qt::DockWidgetArea &area);
	
private slots:

	void on_actionAdd_Pond_triggered();

	void on_actionAdd_Soil_triggered();

	void on_actionAdd_Catchment_Area_triggered();

	void on_actionAdd_Darcy_Block_triggered();

	void on_actionAdd_Stora_ge_triggered();

	void on_actionAdd_St_ream_triggered();

	//    void on_actionAdd_Interface_triggered();

	void on_actionAdd_Connector_triggered(bool checked);

	void on_action_Select_triggered(bool checked);

	void on_action_Pan_triggered(bool checked);

	void on_actionProperty_Box_triggered(bool checked);


	void MainWindow::updateToolbarToggles(Operation_Modes OMode = Operation_Modes::NormalMode);
	void on_action_New_triggered();
	void on_actionNew_from_template_triggered();

	void on_action_Open_triggered();

	void on_action_Save_triggered();
	void on_actionSave_As_triggered();
	bool saveModel(QString &);
	void on_actionZoom_In_triggered();

	void on_actionZoom_Out_triggered();

	void on_actionZoom_All_triggered();

	void on_actionE_xit_triggered();

	void on_action_Undo_triggered();

	void on_action_Redo_triggered();

	void on_actionClear_Reaction_Network_triggered();

	void on_actionLoad_Reaction_Network_triggered();

	void on_actionSave_Reaction_Network_triggered();

	void on_actionReactions_triggered();

	//	void Mouse_Pos();

	void on_projectExplorer_clicked(const QModelIndex &index);

	void on_projectExplorer_customContextMenuRequested(const QPoint &pos);
	
	void on_actionmenuRecent_triggered();// QString fileName);

//	void on_tableProp_customContextMenuRequested(const QPoint &pos);

	void tablePropShowContextMenu(const QPoint&);
	
	void addParameter(QAction*);

	QModelIndex addParameterIndex(const QModelIndex &index = QModelIndex());
//	void addParameter(QString variableName = "");

	void openRXNWindow();


	void addProjectExplorerTreeItem(TreeModel *model = 0, const QString name = "", const QModelIndex = QModelIndex());

	void removeProjectExplorerNodeItem(QString name = "", const QModelIndex = QModelIndex()); // TreeModel *model = 0, const QString name = "", const QModelIndex = QModelIndex());
	void removeProjectExplorerEdgeItem(QString name = "", const QModelIndex = QModelIndex()); // TreeModel *model = 0, const QString name = "", const QModelIndex = QModelIndex());
	void removeProjectExplorerEntityItem(QString name = "", const QModelIndex = QModelIndex()); // TreeModel *model = 0, const QString name = "", const QModelIndex = QModelIndex());
	void plotObservationData(CBTC data = CBTC(), QString name = "");
	void plotControllerData(CBTC data = CBTC(), QString name = "");
	void plotModeledData(CBTC modeled = CBTC(), CBTC observed = CBTC(), QString name = "");
	void plotModeledDataDot(CBTC modeled = CBTC(), CBTC observed = CBTC(), QString name = "");
	void plotAgreementPlotData(CBTC observation = CBTC(), CBTC modeled = CBTC(), QString name = "");
	void plotPercentileData(percentileData data = percentileData(), QString name = "");
	void plotAllPercentileData(vector<percentileData> data = vector<percentileData>(), QString name = "");
	void plotPriorHistogram(CBTC histogram = CBTC(), QString name = "");
	void plotPosteriorHistogram(CBTC histogram = CBTC(), QString name = "");
	void plotRealization(CBTCSet data = CBTCSet(), QString name = "");
	void plotNoiseRealization(CBTCSet data = CBTCSet(), QString name = "");
	void plotRealizationPercentile(CBTCSet data = CBTCSet(), QString name = "");
	void plotNoiseRealizationPercentile(CBTCSet data = CBTCSet(), QString name = "");
	void plotTimeSeries(QAction*, CBTC data = CBTC(), QString name = "", bool convertXtoTime = true, bool reset = false);
	void showLocalSensitivityMatrix();
	void showGlobalSensitivityMatrix();
	void showCorrelationMatrix();
	void showHelp(int code = -1, QString variableName = "");
	void on_actionRun_Model_triggered();
	void on_actionRun_Inverse_Model_triggered();
	void on_actionRun_Model_from_Script_triggered();

	void on_action_Hydraulic_Outputs_triggered();
	void on_actionlogWindow_triggered();
	void on_actionExport_to_Script_Language_triggered();
	
	void on_actionAdd_Well_triggered();
	void on_actionAdd_Tracer_triggered();

	void on_actionAbout_triggered();
	void on_actioncolorCodedResults_triggered();
	void on_actioncolorCodeStorage_triggered();
	void on_actioncolorCodeHead_triggered();
	void on_actioncolorCodeMoistureContent_triggered();
	void on_actioncolorCodeWaterDepth_triggered();
	void on_actioncolorCodeEvaporationRate_triggered();
	void on_actionColorCodeConnectorFlow_triggered();
	void on_actionColorCodeConnectorVelocity_triggered();
	void on_actionColorCodeConnectorArea_triggered();
	void on_actionColorCodeConnectorVaporExchangeEate_triggered();


	void on_actionProjectSettings_triggered();
	void on_actionClimateSettings_triggered();
	void on_actionSolverSettings_triggered();

	void on_actionNewExperiment_triggered();
	void on_actionCopyFromCurrentExperiment_triggered();
	void addExperiment(QString sourceExperiment = "");
	void on_actionremoveCurrentExperiment_triggered();
	
	void on_actionShowRuntimeWindow_triggered();
	void on_actionRecent_triggered();
	void on_actionReset_colors_triggered();
	void menuWaterQuality_hovered();
	void updateAction(QAction *a, QString particleConstituent, QString p, QString c="", QString phase="get from action");
	void waterQualityPostProcessing_clicked();
	void menuWaterQuality_triggered();
	void on_actionContact_Us_triggered();
	void on_actionContact_Us_hovered(); 
	void removeFromRecentList(QAction*);

	void writeRecentFilesList();

	bool loadModel(QString modelfilename);

	void gwidgetChanged();
	void MainWindow::closeEvent(QCloseEvent *event);
	void getNumber(double initial = -1);
	void recentItem()
	{
		int i = 0;
	}

private:
//	QString modelFilename = "";
	int newBlockX(){
//		if (newBlockCounter > 6) newBlockCounter = 0;
		return 650 + 90 * newBlockCounter;
	}
	int newBlockY(){
		if (newBlockCounter > 6) newBlockCounter = 0;
		return 750 + 70 * ++newBlockCounter;
	};
	int newBlockCounter = 0;

signals:


};
