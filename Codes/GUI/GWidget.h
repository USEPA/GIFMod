#pragma once

#include "QGraphicsView.h"
#include <enums.h>
//#include "modelconfig.h"
#include "mProp.h"
#include <qstandarditemmodel.h>
#include "qtableview.h"
#include "logwindow.h"
#include "qcalendarwidget.h"
#include "mainwindow.h"
#include "qcombobox.h"
#include "PropModel.h"
#include "XString.h"

//#include "helpWindow.h"

//class MainWindow;
class helpWindow;
class QCPGraph;
class Ray;
class Node;
class Edge;
class Entity;
class Process;
class Delegate;
class mPropList;
class QTreeView;
class TreeModel;
class CMedium;
class CMediumSet;
class CGWA;
class Results;
struct plotformat;

//class logWindow;


struct scatterPlotData
{
	QString name;
	QVector<double> t;
	QVector<double> y;
};

struct propertyItem
{
	QString objectType;
	QString objectName;
	QString description;
	QString propertyName;
	XString value;
	QString valueType;
	QString minRange = "-#inf", maxRange = "#inf";
};

struct modelBlocks
{
	QString name, type, subType;
	QList<propertyItem> properties;
};

struct modelConnectors
{
	QString name, type, subType;
	QString start, end;
	QList<propertyItem> properties;
};

struct command
{
	QString name;
	QString description;
	QList<modelBlocks> blocks;
	QList<modelConnectors> connectors;
	QList<propertyItem> properties;
	bool askUser = true;
};


class GraphWidget : public QGraphicsView
{
	Q_OBJECT

public:
	GraphWidget(QWidget *parent, QString applicationShortName, QString metafilename, logWindow *log, MainWindow* mainWindow);
	MainWindow* mainWindow;
	QWidget* parent;
	QString applicationShortName = "";
	//	void refresh();
//	vector<Node*> Nodes;
	int node_id, edge_id;
	void itemMoved();
	QGraphicsScene *MainGraphicsScene;
//	QList<QGraphicsScene> UndoScenes;
	int undo_counter;
	void undo();
	void redo();
	bool trackingUndo = false;
	void deselectAll() const;
	void deleteSelected();
	void update();
//	void PropsPopulate(const Node *node, QTableView *tableProp);
	void PropsPopulate(Node *node, QStandardItemModel *propModel);
	int _x, _y;
	Operation_Modes setMode(Operation_Modes OMode = Operation_Modes::NormalMode, bool back = false);
	Operation_Modes setMode(int i);
	Operation_Modes GraphWidget::setModeCursor(void);
	QList<Node *> Nodes() const;
	QList<Edge *> Edges() const;
	QList<Entity *> Entities;
	QList<Process *> Processes;

	QList<Entity *> entitiesByType(const QString &type) const;
	QList<Node *> nodesByType(const QString &type) const;
	QStringList nodeNames() const;
	QStringList nodeNames(const QString &type) const;
	QStringList edgeNames() const;

	Node* node(const QString &name) const;
	Edge* edge(const QString &name) const;
	Entity* entity(const QString &name, const QString &type ="*") const;
	Process * process(const QString &name) const;

	void deleteNode(const QString &name) {deleteNode(node(name));}
	void deleteNode(Node *node);
	void deleteEdge(const QString &name) { deleteEdge(edge(name)); }
	void deleteEdge(Edge *edge);
	void deleteEntity(const QString &name) { deleteEntity(entity(name)); }
	void deleteEntity(Entity *entity);
	void deleteProcess(const QString &name) { deleteProcess(process(name)); }
	void deleteProcess(Process *process);

	QStringList EntityNames(const QString &type) const;
	QTreeView *projectExplorer;
	void setProjExp(QTreeView *p){ projectExplorer = p; };
	//void setProjExpModel(QAbstractItemModel *p){ projectExplorer = p; };

	mPropList *mList;
	QTableView *tableProp;
//	PropModel *propModel;
	QAbstractItemModel *propModel() const { return tableProp->model(); };
	//PropModel *propModel() const { return static_cast<PropModel*>(tableProp->model()); };
	//	QList<modelConfig> undo_list;
	QList<QList<QMap<QString, QVariant>>> undolist;
	//modelConfig active_model_config;
	Node *Node1; // , *Node2;
	Ray *tempRay;
	mProp ModelSpace;
	Delegate *mDelegate;
	Entity* entityByName(const QString &name) const;
	QStringList inflowFileNames;

	QList<QMap<QString, QVariant>> compact() const;
	QList<QMap<QString, QVariant>> compactRXN() const;
	GraphWidget* unCompact(QList<QMap<QString, QVariant>>);//, QWidget *parent = 0);
	GraphWidget* unCompact10(QList<QMap<QString, QVariant>>);//, QWidget *parent = 0);
	void clear();
	void clearRXN();
	TreeModel *treeModel;
	void expandNode(const QModelIndex &parentIndex, bool expand);
/*	QString getInflowFileName(){
		return QFileDialog::getOpenFileName(
			qApp->activeWindow(),
			tr("Select the File"),
			QDir::currentPath(),
			tr("Time Series Input Files (*.txt *.csv)"));
	};
*/
	void copyProps(QString sourceExperiment, QString destExperiment);
#ifdef WQV
	CMediumSet* modelSet = 0;
	CMedium *model = 0;
	vector<Results *> resultsSet;
	bool wizard(QList<command>&commands);
#endif
#ifdef GWA
	CGWA *model = 0;
#endif
	Results *results = 0;
	helpWindow* help = 0;

	void deleteSolution(){
		if (model) delete model;
		if (modelSet) delete modelSet;
		if (results) delete results;
		if (resultsSet.size()) resultsSet.clear();
		model = 0;		results = 0;
		modelSet = 0;
	}
	QMap<QCPGraph *, plotformat> graphsClipboard; // scatterPlotsList;
	QString modelFilename = "";
	QString modelPathname() const;
	QString updateRelativePaths(QString oldPath, QString newPath);
	QString defaultDir() const;
	void log(QString text) const { 
		(*logW)(text); };
	logWindow *logW;
	void newError(QString message){
		logW->append(message);
	}
	QStringList functionList, PhysicalCharacteristicsList;
	// #Errors, #Warnings
	QStringList variableValuesHasError();
	bool hasChanged() {
		return changedState;
	};
	bool changedState = false;

	void startEditingDelegate(QString variableName){
		qDebug() << "start editing " << variableName;
		allowRunVariableName = variableName;
		allowRun = false;
	}
	void endEditingDelegate() {
		qDebug() << "end editing " << allowRunVariableName;
		allowRunVariableName = "";
		allowRun = true;
	}
	bool allowRun = true;
	QString allowRunVariableName;
	int experimentID();
	QString experimentName();
	QString firstExperimentName(){
		if (experiments->count() < 2)
			return QString();
		return experiments->itemText(1);
	}
	QStringList experimentsList() const {
		QStringList r;
		if (!experiments)
			return QStringList();
		for (int i = 1; i < experiments->count(); i++)
			r.append(experiments->itemText(i));
		return r;
	}
	QComboBox *experiments;
public slots:
	void shuffle();
	void zoomIn();
	void zoomOut();
	void add_to_undo_list();
	void settableProp(QTableView*_tableProp);
	//void setpropModel(PropModel *_propModel);
	void scaleView(qreal scaleFactor); 
	bool select(const QString &name, const QString type) const;
	void nodeContextMenuRequested(Node* ,QPointF pos);
	void edgeContextMenuRequested(Edge*, QPointF pos);
	void warning(QString){};
	QStringList script() const;
	QStringList s_get_params() const;
	QStringList s_get_observed() const;
	QStringList s_get_environmental_params() const;
	QStringList s_get_model_configuration() const;
	QStringList s_get_particle_types() const;
	QStringList s_get_constituents() const;
	QStringList s_get_reactions() const;
//	QStringList s_set_default_connector_expressions() const;
//	QStringList s_set_default_block_expressions() const;
	QStringList s_get_buildup() const;
	QStringList s_get_external_flux() const;
//	QStringList s_load_inflows() const;
	double minX() const;
	double minY() const;
	double maxX() const;
	double maxY() const;
	void sceneChanged();

	void gwChanged();
	void nodeChanged(Node*);
	void edgeChanged(Edge*);
	void entityChanged(Entity*);
	void delegateDatePicked(QCalendarWidget *calendar = 0, QModelIndex index = QModelIndex());
	//QComboBox* experiments;
	void experimentSelect(const QString &experimentName);
signals:
	void Mouse_Pos(int, int, QString);
	void changed();
	
protected:
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
#ifndef QT_NO_WHEELEVENT
	void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif
	void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;
//	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	

private:
	int timerId;
	Node *centerNode;
	Operation_Modes Operation_Mode;
	void draw_temp_edge(Node *source, QPoint point);
	Node *resizenode;
	corners resizecorner;
	bool sceneReady = false;
};
bool validInflowFile(QString file);
