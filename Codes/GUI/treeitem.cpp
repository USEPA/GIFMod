#include <QStringList>

#include "treeitem.h"
#include "node.h"
#include "edge.h"
#include "entity.h"
#include "GWidget.h"

TreeItem::TreeItem(const QString name, GraphWidget *gWidget, Type type)
{
	this->type = type;
	this->name = name;
	this->gWidget = gWidget;
}

TreeItem::TreeItem(Node *node)//, TreeItem *parent)
{
	this->type = Type::NodeItem;
	this->node = node;
	this->gWidget = node->parent;
}
TreeItem::TreeItem(Edge *edge)//, TreeItem *parent)
{
	this->type = Type::EdgeItem;
	this->edge = edge;
	this->gWidget = edge->parent;
}
TreeItem::TreeItem(Entity *entity)//, TreeItem *parent)
{
	this->type = Type::EntityItem;
	this->entity = entity;
	this->gWidget = entity->parent;
}
void TreeItem::addChild(TreeItem *item, int index)
{
	if (index == -1)
		childItems.append(item);
	else
		childItems.insert(index, item);
	item->parentItem = this;
}
void TreeItem::addChild(QList<TreeItem *> items)
{
	childItems.append(items);
	for each (TreeItem *item in items)
		item->parentItem = this;
}

void TreeItem::deleteChild(QString name)
{
	for each (TreeItem *child in childItems)
		if (child->Name() == name)
		{
			childItems.removeAll(child);
			return;
		}
}

int TreeItem::indexOf(TreeItem *item) const
{
	return childItems.indexOf(item);
}
int TreeItem::indexOf(QString name) const
{
	for each (TreeItem * item in childItems)
		if (item->Name() == name) return indexOf(item);
	return -1;
}
//! [0]
QString TreeItem::Name() const
{
	switch (type){
	case Type::NodeItem:
		if (node) return node->Name();
	case Type::EdgeItem:
		if (edge) return edge->Name();
	case Type::EntityItem:
		if (entity) return entity->Name();
	default:
		return name;
	}
	return "X";
}
/*QList <TreeItem*> TreeItem::Children() 
{
	if (type == Type::NodesBranch)
		if (children.count() != gWidget->Nodes().count())
		{
			qDeleteAll(children);
			children.clear();
			for each (Node* node in gWidget->Nodes())
				new TreeItem(node, this);
		}
	
	if (type == Type::WellsBranch)
		if (children.count() != gWidget->nodesByType("Well").count())
		{
			qDeleteAll(children);
			children.clear();
			for each (Node* tracer in gWidget->nodesByType("Well"))
				new TreeItem(tracer, this);
		}
	if (type == Type::TracersBranch)
		if (children.count() != gWidget->nodesByType("Tracer").count())
		{
			qDeleteAll(children);
			children.clear();
			for each (Node* tracer in gWidget->nodesByType("Tracer"))
				new TreeItem(tracer, this);
		}
	if (type == Type::EdgesBranch)
		if (children.count() != gWidget->Edges().count())
		{
			qDeleteAll(children);
			children.clear();
			for each (Edge* edge in gWidget->Edges())
				new TreeItem(edge, this);
		}
/*	if (type == Type::Branch)
		if (children.count() != gWidget->Edges().count())
		{
			qDeleteAll(children);
			children.clear();
			for each (Entity *entity in gWidget->Entities)
				if (entity->ObjectType().ObjectType == Name()) new TreeItem(entity, this);
		}
	return children;
}*/
//! [1]
TreeItem::~TreeItem()
{
	qDeleteAll(childItems);
}
//! [1]

//! [2]
/*void TreeItem::appendChild(TreeItem *item)
{
	children.append(item);
//	item->parent = this;
}*/
//! [2]

//! [3]
TreeItem *TreeItem::child(int row)
{
	return childItems.value(row);
}
//! [3]

//! [4]
int TreeItem::childCount() 
{
	return childItems.count();
}
//! [4]

//! [5]
int TreeItem::columnCount() const
{
	return 1;
	if (name == "Root") return 1;
	return m_itemData.count();
}
//! [5]
QString TreeItem::itemType() const
{
	if (type == NodeItem) return node->ObjectType().ObjectType;
	return Name();
}
//! [6]
QVariant TreeItem::data(int column) const
{
	return Name();
}
TreeItem *TreeItem::parent()
{
	return parentItem;
}
/*int TreeItem::row() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}*/
//! [8]
int TreeItem::row() const
{
	if (parentItem)
		return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
	return 0;
}