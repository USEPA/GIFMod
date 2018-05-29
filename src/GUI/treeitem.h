#pragma once

#include <QList>
#include <QVariant>
class Node;
class Edge;
class Entity;
class GraphWidget;
//! [0]

class TreeItem
{
public:
	enum Type{ Root, Branch, Item, NodesBranch, EdgesBranch, WellsBranch, TracersBranch, NodeItem, EdgeItem, EntityItem, WaterQualityBranch, 
		SettingsBranch, ReactionsBranch, ReactionNetworkItem, InverseModelingBranch, ControlBranch, GAItem, MCMCItem };
	explicit TreeItem(const QString name, GraphWidget *gWidget, Type type);//, TreeItem *parent);
	explicit TreeItem(Node *node);// , TreeItem *parent);
	explicit TreeItem(Edge *edge);//, TreeItem *parent);
	explicit TreeItem(Entity *entity);//, TreeItem *parent);
	//	explicit TreeItem(const Type &type, GraphWidget &gWidget, TreeItem *parent = 0);
	~TreeItem();
	Type type;
	//void appendChild(TreeItem *child);

	TreeItem *child(int row);
	QString itemType() const;
	int childCount() ;
	int columnCount() const;
	int row() const;
	QVariant data(int column) const;
//	int row() const;
//	QList <TreeItem*> Children() ;
	Node *node;
	Edge* edge;
	Entity* entity;
	TreeItem *parent();

	QString Name() const;
	GraphWidget *gWidget;
//	QList<TreeItem*> children;
	void addChild(TreeItem *item, int index=-1);
	void addChild(QList<TreeItem *> items);
	void deleteChild(QString name);
	int indexOf(TreeItem *item) const;
	int indexOf(QString name) const;

private:
	QList<TreeItem*> childItems;
	TreeItem *parentItem;
	QList<QVariant> m_itemData;

	//	TreeItem *m_parentItem;
//	QString type;
//	Node* node;
//	Edge* edge;
	QString name;
};
//! [0]
