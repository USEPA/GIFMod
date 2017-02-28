#pragma once
#include <QGraphicsItem>
#include "PropModel.h"
#include "Proplist.h"
#include "XString.h"
#include "node.h"

class GraphWidget;
class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode, GraphWidget *_parent = 0);
	Edge(const Edge &);
	Edge Edge::operator=(const Edge &);
	~Edge() {
		delete model;
	}
	objectColor color;
	mPropList getmList(const mProp &_filter = '*') const;
	bool setObjectType(const QString &type);
	mProp Filter() const;
	int dist(const QPointF point);
	QString updateSubType();
	void adjust();
	bool bold = false;
	void setBold(const bool _Bold = true);
	enum { Type = UserType + 2 };
	int type() const Q_DECL_OVERRIDE{ return Type; };
	void setName(const QString& Name){ name = Name; }
//	void setID(const QString& ID){ id = ID; };
	mPropList *mList() const;
	QVariant getProp(const QString &propName, const int role = Qt::DisplayRole) const;
	XString getValue(const QString &propName) const;
	bool setProp(const QString &propName, const QVariant &Value, const int role = Qt::EditRole);
	bool setValue(const QString &propName, const XString &Value);
	void update(bool fast = false);

	Node *sourceNode() const;
    Node *destNode() const;
	QString GUI;
	Object_Types itemType;
//	mPropList propList;
	GraphWidget *parent;
	mProp ObjectType() const { return objectType; };
	QString Name() const { 
		return name; };
//	QString ID() const { return id; };
	QPointF sourcePoint;
	QPointF destPoint;
	qreal arrowSize;
	PropList<Edge> props;
	mProp objectType;
	QString name; // , id;
	QMap<QString, QVariant> compact() const;
	static Edge* unCompact(QMap<QString, QVariant>, GraphWidget *gwidget, bool oldVersionLoad = false);
	static Edge* unCompact10(QMap<QString, QVariant>, GraphWidget *gwidget);
	QStringList codes() const;
	QMap<QString, condition> variableNameConditions() const;

	XString val(const QString & code) const;

	PropModel<Edge> *model;

	QMap<QString, QString> warnings, errors;
	bool errorDetected() const { return (errors.count()) ? true : false; }
	QStringList Edge::variableNames() const;
	QString variableName(QString code) const;
	void copyProps(Node *node, QString arrayDirection, QString connectorDirection, bool copyLength = true);
	bool avoidCrossObjects = true;// false;
	QString experimentName() const;
	void copyProps(QString sourceExperiment, QString destExperiment);
protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
signals:

private:
    Node *source, *dest;
	int sourceID, destID;
//	PropList<Edge> props;
	void changed();
	bool intersects(double x, double y);
};
/*
bool twoLineIntersect(QPoint p1, QPoint p2, QLine l2){
	return twoLineIntersect(QLine(p1, p2), l2);
}

bool twoLineIntersect(QLine l1, QPoint p1, QPoint p2){
	return twoLineIntersect(l1, QLine(p1, p2));
}
bool twoLineIntersect(QPoint p1, QPoint p2, QPoint P1, QPoint P2){
	return twoLineIntersect(QLine(p1, p2), QLine(P1, P2));
}
bool twoLineIntersect(double x1, double y1, double x2, double y2, double X1, double Y1, double X2, double Y2){
	return twoLineIntersect(QPoint(x1, y1), QPoint(x2, y2), QPoint(X1, Y1), QPoint(X2, Y2));
}
bool twoLineIntersect(QLine l1, QLine l2){
	if (!l1.dx || !l2.dx)
	{

	}
	else
	{
		double s1 = l1.dy / l1.dx;
		double s2 = l2.dy / l2.dx;
		if (s1 != s2)
		{
			double x = (l1.x1*s1 - l2.x1*s2 + l2.y1 - l1.y1) / (s1 - s2);
			if (between(x, l1.x1, l1.x2) && between(x, l2.x1, l2.x2))
				return true;
			else return false;
		}
	}
}
bool between(double x, double a, double b)
{
	if (a <= b)
	{
		if (x >= a && x <= b)
			return true;
		else return false;
	}
	else
	{
		if (x >= b && x <= a)
			return true;
		else return false;
	}
}
*/



