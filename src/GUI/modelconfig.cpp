#include "modelconfig.h"
#include "GWidget.h"
#include "node.h"
#include "edge.h"
#include "entity.h"



modelConfig::modelConfig()
{

}


modelConfig::~modelConfig()
{
}

modelConfig::modelConfig(const modelConfig &M)
{
//	Nodes = M.Nodes;
//	Edges = M.Edges;

}


modelConfig modelConfig::operator=(const modelConfig &M)
{
//	Nodes = M.Nodes;
//	/Edges = M.Edges;
	return *this;
}

modelConfig::modelConfig(GraphWidget &gw)
{
}
modelConfig::modelConfig(GraphWidget *gw)
{
	this->gw = gw;
}
QList<modelItem> modelConfig::nodes()
{
	QList<modelItem> r;
	for each (Node* i in gw->Nodes())
		r.append(modelItem(*i));
	return r;
}
QList<modelItem> modelConfig::edges()
{
	QList<modelItem> r;
	for each (Edge* i in gw->Edges())
		r.append(modelItem(*i));
	return r;
}
QList<modelItem> modelConfig::entities()
{
	QList<modelItem> r;
	for each (Entity* i in gw->Entities)
		r.append(modelItem(*i));
	return r;
}

modelItem::modelItem(Node &node)
{
//	ID = node.ID;
	GUI = node.GUI;
//	itemType = node.itemType;
	particleInitialCondition = node.particleInitialCondition;
	constituentInitialCondition = node.constituentInitialCondition;
	x = node.x();
	y = node.y();
	width = node.Width();
	height = node.Height();
	name = node.Name();
	props = node.props.list;
	mProp objectType;

	for each (Edge* e in node.edges())
		edgeList.append(e->Name());
}
Node::Node(modelItem &item, GraphWidget *gwidget)
{
//	Node(gwidget, item.objectType.ObjectType, item.name, item.ID, item.x, item.y, item.width, item.height);

}
modelItem::modelItem(Edge &edge)
{
	sourceNode = edge.sourceNode()->Name();
	destNode = edge.destNode()->Name();
	GUI  = edge.GUI;
//	propList = edge.propList;
	objectType = edge.ObjectType();
	name = edge.Name();
//	id = edge.ID();
	sourcePoint = edge.sourcePoint;
	destPoint = edge.destPoint;
	arrowSize = edge.arrowSize;
	props = edge.props.list;
}
modelItem::modelItem(Entity &entity)
{
//	EntityType Type;
	objectType = entity.objectType;
	props = entity.props.list;
	name = entity.name;
}
Node* modelItem::node(GraphWidget* gw) const
{
	Node* n = new Node(gw, objectType.ObjectType, name, ID, x, y, width, height);
	n->GUI = GUI;
	n->particleInitialCondition = particleInitialCondition;
	n->constituentInitialCondition = constituentInitialCondition;
	n->props.list = props;
	n->objectType = objectType;
	return n;
}
Edge* modelItem::edge(GraphWidget* gw) const
{
	Edge* e = new Edge(gw->node(sourceNode), gw->node(destNode), gw);
	e->GUI = "Connector";
	e->objectType = objectType;
//	e->id = id;
	e->name = name;
	e->sourcePoint = sourcePoint;
	e->destPoint = destPoint;
	e->arrowSize = arrowSize;
	e->props.list = props;

	gw->node(sourceNode)->edgeList.append(e);
	gw->node(destNode)->edgeList.append(e);
	return e;
}

Entity* modelItem::entity(GraphWidget* gw) const
{
	Entity* e = new Entity(objectType.ObjectType, name, gw);
	e->props.list = props;
	return e;
}