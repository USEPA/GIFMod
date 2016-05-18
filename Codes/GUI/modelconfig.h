#pragma once
//#include <node.h>
//#include <edge.h>
//#include <vector>
#include "qlist.h"
//#include "GWidget.h"

#include "node.h"
class Edge;

//using namespace std;
class GraphWidget;

class modelItem
{
public:
	modelItem(){};
	modelItem(Node &node);
	modelItem(Edge &edge);
	modelItem(Entity &entity);
//	modelItem(const modelItem&);
	Node* node(GraphWidget* gw = 0) const;
	Edge* edge(GraphWidget* gw = 0) const;
	Entity* entity(GraphWidget* gw = 0) const;

private:
	QString GUI;
	int ID;
	Object_Types itemType;
	QList<ParticleInitialConditionItem> particleInitialCondition;
	QList<ConstituentInitialConditionItem> constituentInitialCondition;
	int x, y, width, height;
	QString name;
	QMap<QString, XString> props;
	QList<QString> edgeList;
	mProp objectType;
	QString sourceNode, destNode;
	mPropList propList;
	int sourceID, destID;
	QPointF sourcePoint;
	QPointF destPoint;
	qreal arrowSize;
	QString id;
};
class modelConfig
{
public:
	modelConfig(GraphWidget &);
	modelConfig(GraphWidget *);
	modelConfig(const modelConfig &);
	modelConfig();
	~modelConfig();
	modelConfig modelConfig::operator=(const modelConfig &);
	QList<modelItem> nodes();
	QList<modelItem> edges();
	QList<modelItem> entities();
	GraphWidget *gw;
};

