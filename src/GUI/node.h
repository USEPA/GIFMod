#pragma once
#include <QGraphicsItem>
#include "PropModel.h"
#include "PropList.h"
#include "XString.h"
#include "enums.h"

//#include "GWidget.h"
//using namespace std;

class Edge;
class GraphWidget;
class modelItem;

//QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
//QT_END_NAMESPACE

struct ParticleInitialConditionItem
{
	QString Particle, Model, Value;
};
struct ConstituentInitialConditionItem
{
	QString Constituent, Particle, Model, Value;
};

struct NutrientHalfSaturationConstantItem
{
	QString Constituent, Value;
};

class Node : public QGraphicsItem
{
	//Q_OBJECT

public:
	Node(GraphWidget *gwidget, QString _type = "Not Defined", QString _name = "No Name", int _ID = -1, int _xx = 0, int _yy = 0, int _width = 200, int _height = 100);	
	Node(modelItem &item, GraphWidget *gwidget);
	Node(const Node &);
	~Node() {
		delete model;
		delete particleInitialConditions;
		delete constituentInitialConditions;
		delete NutrientHalfSaturationConstants;

	}
	objectColor color;
	Node operator=(const Node &);
	void addEdge(Edge *edge);

	QList<Edge *> edges() const { return edgeList; };

	mPropList getmList(const mProp &_filter) const;
	mPropList getmList(const QList<mProp>_filter) const;

	enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE { return Type; }

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	mPropList mpropList()  const;
    QString Name() const { return name; }
	bool setName(const QString &Name);
	void setBold(const bool _Bold = true);

	bool setObjectType(const QString &);
	bool setObjectSubType(const QString &);
	mProp ObjectType() const{ return objectType; };
	int Width() const { return width; };
	int Height() const { return height; };
	void setWidth(const int &Width) 
	{ 
		width = Width; update(); 
	};
	void setHeight(const int &Height) { 
		height = Height; update(); 
	};
	mProp Filter() const { return ObjectType(); };
	QList<mProp> Filter(const QList<Node*> nodes) const { 
		QList<mProp> objectTypes;
        foreach (Node* n , nodes)
			objectTypes.append(n->ObjectType());
		return objectTypes; };


	QVariant getProp(const QString &propName, const int role = Qt::DisplayRole) const;
	QVariant getProp(const QString &propName, const QList<Node*> nodes, const int role = Qt::DisplayRole) const;
	XString getValue(const QString &propName) const;
	bool setProp(const QString &propName, const QVariant &Value, const int role = Qt::EditRole) ;
	bool setValue(const QString &propName, const XString &Value);
	mPropList *mList() const;
	corners corner(const int x, const int y);
	edgesides edge(const int x, const int y);
	QModelIndex index(QModelIndex &parent) const { return model->index(0, 0, parent); };
	void update(bool fast = false);
	QString g(QString experimentName = "") const;
	QString cg(QString experimentName = "") const;
	QString planthsc(QString experimentName = "") const;
    QString newNodeName(const QString name, QList<Node*> &nodes) const;
	QList<ParticleInitialConditionItem> &particleInitialCondition(QString experimentName = "") const;
	QList<ParticleInitialConditionItem> &particleInitialCondition(QString experimentName = "");
	QList<ConstituentInitialConditionItem> &constituentInitialCondition(QString experimentName = "") const;
	QList<ConstituentInitialConditionItem> &constituentInitialCondition(QString experimentName = "");
	QList<NutrientHalfSaturationConstantItem> &NutrientHalfSaturationConstant(QString experimentName = "") const;
	QList<NutrientHalfSaturationConstantItem> &NutrientHalfSaturationConstant(QString experimentName = "");
	QMap<QString, QVariant> compact() const;
	static Node* unCompact(QMap<QString, QVariant>, GraphWidget *gwidget, bool oldVersion = false);
	static Node* unCompact10(QMap<QString, QVariant>, GraphWidget *gwidget);
	QStringList codes() const;
	QMap<QString, condition> variableNameConditions() const;
	bool errorDetected() const { return (errors.count()>0) ? true : false; }
	QStringList variableNames() const;
	QString variableName(QString code) const;
	int experimentID();
	QString experimentName() const;
	void copyProps(QString sourceExperiment, QString destExperiment);

	XString val(const QString & code) const;

	GraphWidget *parent;
	QString GUI;
	Object_Types itemType;
	QMap<QString, QList<ParticleInitialConditionItem>>* particleInitialConditions;
	QMap<QString, QList<NutrientHalfSaturationConstantItem>>* NutrientHalfSaturationConstants;
	QMap<QString, QList<ConstituentInitialConditionItem>>* constituentInitialConditions;
	PropList<Node> props;
	mProp objectType;
	QList<Edge *> edgeList;
	PropModel<Node> *model;
	QMap<QString, QString> warnings, errors;
	int minH = 30, minW = 40;
	bool oldVersionLoad = false;
	bool isPorous() {
		QStringList Porous;
		Porous << "Soil" << "Darcy" << "Storage";// << "" << "";
		if (Porous.contains(ObjectType().ObjectType))
			return true;
		else 
			return false;
	}
	QString middleText = "";
	void changed();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

signals:
	void hoverMoveEvent(QGraphicsSceneHoverEvent * event);

//public slots:
//	void contextMenuClicked(QAction*);

private:
	int width, height;
	QString name;

	QPointF newPos;
//	GraphWidget *graph;
//	QMap<QString, QString> warnings, errors;
	bool bold = false;


};
