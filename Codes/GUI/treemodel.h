#pragma once


#include <QModelIndex>
#include <QVariant>
#include <QAbstractItemModel>
//#include <QModelIndex>
//#include "QVariant.h"
//#include "mProplist.h"
//#include "treeitem.h"
//#include "GWidget.h"
class GraphWidget;
#include "node.h";
//class Node;
//class Edge;
//class TreeItem;
#include "qdebug.h"
#include "treeitem.h"

template <class T>
class Branch;
class TreeModel : public QAbstractItemModel 
{
    Q_OBJECT

public:
	explicit TreeModel(GraphWidget *parent);
//	explicit TreeModel(const QString &data, QObject *parent = 0);
	~TreeModel();
	GraphWidget *Parent;
	QVariant data(const QModelIndex &index, int role) const ;
	Qt::ItemFlags flags(const QModelIndex &index) const;// { return Qt::ItemIsEnabled; };
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;// { return QString("Header section(%1)").arg(section); };
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const ;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const; // { return 1; };
	bool hasChildren(const QModelIndex & parent = QModelIndex()) const;
	TreeItem *itemFromIndex(const QModelIndex &index) const;
	void prepareToAddFromMenu(const QString  name);
	void update() { emit reset; };
	void addChildFromMenu(const QString name, QModelIndex *parentIndex);
	void deleteNode(int row, QModelIndex &parent, QString name);
	void deleteNode(Node *node);
	void deleteEdge(int row, QModelIndex &parent, QString name);
	void deleteEdge(QString edgeName);
	void deleteEdge(Edge *edge);
	void deleteEntity(int row, QModelIndex &parent, QString type, QString name);
	void deleteEntity(Entity *entity);
	void add(Node *node);
	void addWell(Node *node);
	void addTracer(Node *node);
	void add(Edge *edge);
	void add(Entity *entity);
	void refresh(QString part = "All");
//	void collapsAll(const TreeItem *);
//	TreeItem *root() const 
//	{
//		return rootItem;
//	}
#ifdef WQV	
	TreeItem *rootItem, *settings, *blocks, *connectors, *evapotranspiration, *waterQuality, *constituent, *buildUp, *extrenalFlux, *particle,
		 *projectSetting, *climateSetting, *solverSetting, *reactions, *reaction, *reactionNetwork, *reactionParameter, *GA, *MCMC, *inverseModeling, *parameter,
		 *observed, *wells, *tracers;
#endif
#ifdef GWA
	TreeItem *rootItem, *settings, *blocks, *connectors, *evapotranspiration, *waterQuality, *constituent, *buildUp, *extrenalFlux, *particle,
		*globalSettings, *solver, *reactions, *reaction, *reactionNetwork, *reactionParameter, *GA, *MCMC, *inverseModeling, *parameter,
		*observed, *wells, *tracers;
#endif

	QString TreeModel::singularform(QString name) const;
	TreeItem * entityParentItemfromType(QString type) const;
public slots:

private:
	void setupModelData(const QStringList &lines, TreeItem *parent);
	//QModelIndex *root;
//	Branch<Node>* nodes;
	//, edges, entities;
//	QList<TreeItem> tempItems;

};