#pragma once

#include "edge.h"
#include "node.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <GWidget.h>
#include <qdebug.h>
#include "PropModel.h"
#include "GWidget.h"

#include "qtableview.h"
#include "treemodel.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(Node *sourceNode, Node *destNode, GraphWidget *_parent)
    : arrowSize(10)
{
    setAcceptedMouseButtons(0);
    source = sourceNode;
    dest = destNode;
	model = new PropModel<Edge>(this);
	QList<Node*> list;
	for each (Edge *e in source->edgeList)
	{
		if (e->sourceNode() == source) list.append(e->destNode());
		if (e->destNode() == source) list.append(e->sourceNode());
	}
	if (list.contains(dest))
	{
		_parent->log(QString("Duplicate connector from %1 to %2.").arg(source->Name()).arg(dest->Name()));
		delete this;
		return;
	}
    source->addEdge(this);
    dest->addEdge(this);
    adjust();
	GUI = "Connector";
	itemType = Object_Types::Connector;
//	sourceID = source->ID;
//	destID = dest->ID;

	setFlag(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);
	setCacheMode(DeviceCoordinateCache);
	setZValue(1);
	parent = _parent;
	objectType = parent->ModelSpace; // mProp('*');
	objectType.GuiObject = "Connector";
	QList <mProp> QL;
	QL = (*parent->mList).List;
//	propList = (*parent->mList).filter(objectType);
//	for (int i = 0; i < propList.size(); i++)
		//		PropList.List[i].Value = QSplit (PropList.List[i].DefaultValues, ':')[0];
//		propList.List[i].Value = propList.List[i].DefaultValuesList()[0];

//	ObjectType.ObjectType = sourceNode->ObjectType.ObjectType;
//	mProp filter;
	//	filter = (parent->ModelSpace && ObjectType);
	//filter = (*parent).ModelSpace && ObjectType;
	objectType.ObjectType = (*parent->mList).filter(objectType).ObjectTypes()[0];
//	objectType.SubType = (*parent->mList).filter(objectType).SubTypes()[0];
	updateSubType();
//	props = new PropList<Edge>(this);
	props.parent = this;

	parent->MainGraphicsScene->addItem(this);
	name = QString("%1 - %2").arg(sourceNode->Name()).arg(destNode->Name());
	parent->treeModel->add(this);
	parent->log(QString("One %3 connector from %1 to %2 created.").arg(sourceNode->Name()).arg(destNode->Name()).arg(objectType.SubType));
	changed();
}

bool Edge::setObjectType(const QString &type)
{
	if (objectType.ObjectType == type) return false;
	else
	{
		objectType.ObjectType = type;
		objectType.SubType = '*';
	//	objectType.SubType = (*parent->mList).filter(Filter()).SubTypes()[0];
		//update();
		if (type == "Default")
			updateSubType();
		changed();
		return true;
	}
}

mProp Edge::Filter() const
{
//	return((*parent).ModelSpace & ObjectType);
	return objectType;
}

mPropList *Edge::mList() const 
{
	return parent->mList; 
};

Node *Edge::sourceNode() const
{
    return source;
}

Node *Edge::destNode() const
{
    return dest;
}

void Edge::setBold(const bool _Bold)
{
	bold = _Bold;
}

void Edge::adjust()
{
    if (!source || !dest)
        return;

	QLineF line(mapFromItem(source, source->Width() / 2, source->Height() / 2), mapFromItem(dest, dest->Width() / 2, dest->Height() / 2));
    qreal length = line.length();

	prepareGeometryChange();

	qreal Ox, Oy, Dx, Dy;
		
	if (abs(line.dx()) < 1)
	{
		Ox = 0;
		Dx = 0;
	}
	else
	{
		Ox = line.dx() / abs(line.dx())*min(int(source->Width() / 2), int(fabs(source->Height() / 2.0 * line.dx() / (line.dy()+0.5))));
		Dx = -line.dx() / abs(line.dx())*min(int(dest->Width() / 2), int(fabs(dest->Height() / 2.0 * line.dx() / (line.dy()+0.5))));
	}
	if (abs(line.dy()) < 1)
	{
		Oy = 0;
		Dy = 0;
	}
	else
	{
		Oy = line.dy() / abs(line.dy())*min(int(source->Height() / 2), int(fabs(source->Width()/ 2.0 * line.dy() / (line.dx()+0.5))));
		Dy = -line.dy() / abs(line.dy())*min(int(dest->Height() / 2), int(fabs(dest->Width() / 2.0 * line.dy() / (line.dx()+0.5))));
	}
		QPointF edgeOffsetSource(Ox, Oy);
		QPointF edgeOffsetDest(Dx, Dy);
		sourcePoint = line.p1() + edgeOffsetSource;
		destPoint = line.p2()  + edgeOffsetDest;
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 8.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

	// Draw the line itself
	if (isSelected())
		painter->setPen(QPen(Qt::green, (bold) ? 3 : 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	else if (errorDetected())
		painter->setPen(QPen(Qt::red, (bold) ? 3 : 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	else
		painter->setPen(QPen(Qt::black, (bold) ? 3 : 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	if (!avoidCrossObjects)
	{

		painter->drawLine(line);

		// Draw the arrows
		double angle = ::acos(line.dx() / line.length());
		if (line.dy() >= 0)
			angle = TwoPi - angle;
		QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
			cos(angle + Pi / 3) * arrowSize);
		QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
			cos(angle + Pi - Pi / 3) * arrowSize);
		QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
			cos(angle - Pi / 3) * arrowSize);
		QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
			cos(angle - Pi + Pi / 3) * arrowSize);
		if (isSelected())
			painter->setBrush(Qt::green);
		else
			painter->setBrush(Qt::black);
		painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
		painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
	}
	if (avoidCrossObjects)
	{

		painter->drawLine(line);

		// Draw the arrows
		double angle = ::acos(line.dx() / line.length());
		if (line.dy() >= 0)
			angle = TwoPi - angle;

		QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
			cos(angle + Pi / 3) * arrowSize);
		QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
			cos(angle + Pi - Pi / 3) * arrowSize);
		QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
			cos(angle - Pi / 3) * arrowSize);
		QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
			cos(angle - Pi + Pi / 3) * arrowSize);
		if (isSelected())
			painter->setBrush(Qt::green);
		else
			painter->setBrush(Qt::black);
		painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
		painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
	}
	if (isSelected())
	{
//		parent->propModel->setParentEdge(this);
		parent->tableProp->setModel(model);
		parent->tableProp->setFocus();
	}
}
int Edge::dist(const QPointF point)
{
	int x1 = sourcePoint.x();
	int y1 = sourcePoint.y();
	int x2 = destPoint.x();
	int y2 = destPoint.y();
	int x0 = point.x();
	int y0 = point.y();
	int dist = abs(x0*(y2 - y1) - y0*(x2 - x1) + x2*y1 - y2*x1) / sqrt((y2 - y1) ^ 2 + (x2 - x1) ^ 2);
	return dist;
}
mPropList Edge::getmList(const mProp &_filter) const
{
	static mProp filter;
	static mPropList r;
	if (filter %= _filter) return r;
	r = mList()->filter(_filter);
	filter = _filter;
	return r;// (*parent->mList).filter(Filter() & _filter);
}

Edge::Edge(const Edge &E)
{
	setName(E.Name());
//	setID(E.ID());
//	itemType = E.itemType; 
	GUI = E.GUI;
	source = E.source;
	dest = E.dest;
	sourceID = E.sourceID;
	destID = E.destID;
	sourcePoint = E.sourcePoint;
	destPoint = E.destPoint;
	arrowSize = E.arrowSize;
	parent = E.parent;
//	propList = E.propList;
	objectType = E.objectType;
	avoidCrossObjects = E.avoidCrossObjects;
	props.list = E.props.list;
}

Edge Edge::operator=(const Edge &E)
{
	setName(E.Name());
//	setID(E.ID());
//	itemType = E.itemType;
	GUI = E.GUI;
	source = E.source;
	dest = E.dest;
	sourceID = E.sourceID;
	destID = E.destID;
	sourcePoint = E.sourcePoint;
	destPoint = E.destPoint;
	arrowSize = E.arrowSize;
	parent = E.parent;
//	propList = E.propList;
	objectType = E.objectType;
	avoidCrossObjects = E.avoidCrossObjects;
	props.list = E.props.list;
	return *this;
}

QVariant Edge::getProp(const QString &propName, const int role) const
{
	mProp filter = Filter();
	filter.VariableName = propName;
	mProp mValue = mList()->filter(filter)[0];
	if (role == TypeRole) return mValue.Delegate;
	if (role == InputMethodRole) return mValue.inputMethod;
	if (role == DefaultValuesListRole) return mValue.DefaultValuesStringList(mList(), &Filter(), parent);
	if (role == VariableTypeRole) return mValue.VariableType;
	if (role == UnitRole) return getValue(propName).unit;
	if (role == defaultUnitRole) return getValue(propName).defaultUnit;
	if (role == UnitsListRole) return getValue(propName).unitsList;
	if (role == experimentDependentRole) return mValue.ExperimentDependent;
	if (role == differentValuesRole)
	{
		if (mValue.ExperimentDependent == "No" || parent->experimentID())
			return false;
		else
			return props.getPropMultiValues(propName, parent->experimentsList()).differentValues();
	}
	if (role == allUnitsRole){
		QStringList allUnits;
		allUnits.append(getValue(propName).unit);
		allUnits.append(getValue(propName).unitsList);
		allUnits.append(getValue(propName).defaultUnit);
		return allUnits;
	}
	if (role == DescriptionCodeRole) return mValue.DescriptionCode;

	if (role == Qt::ForegroundRole)
	{
		if (warnings.keys().contains(propName))	return QBrush((QColor(255, 111, 28)));
		if (errors.keys().contains(propName)) return QBrush(Qt::red);
		if (getProp(propName, differentValuesRole).toBool())
			return QBrush(Qt::gray);
	}

	if (role == Qt::FontRole) //TextColorRole)
	{
		QFont boldFont;
		boldFont.setBold(true);
		if (propName == "Name" || propName == "Type" || propName == "SubType") return boldFont;
		if (mValue.DefaultValuesStringList(0, 0, parent).indexOf(getValue(propName)) != -1) return boldFont;
		else return QFont();
	}
	if (role == Qt::CheckStateRole)
		if ((mValue.Delegate == "CheckBox") || (mValue.VariableType == "boolean"))
			if ((getValue(propName) != "0") && (getValue(propName).toLower() != "false") && (getValue(propName).toLower() != "no")) return Qt::Checked; else return Qt::Unchecked;

	if (role == fullFileNameRole) return getValue(propName);
	if (role == Qt::DisplayRole)// || role == Qt::ToolTipRole)
	{
		if (getProp(propName, differentValuesRole).toBool())
			return QVariant("Different values.");
		if (mValue.VariableType == "filename")
			return OnlyFilenames(getValue(propName));
		else if (mValue.Delegate == "CheckBox")
			return "";
		else return getValue(propName).toStringUnit();
	}
	if (role == Qt::ToolTipRole)
	{
		QString toolTip;
		if (errors[propName].size()) toolTip.append("\n Error: ").append(errors[propName]);
		if (warnings[propName].size()) toolTip.append("\n Warning: ").append(warnings[propName]);

		if (mValue.VariableType == "filename")
			return getValue(propName).append(toolTip);
		else if (mValue.Delegate == "CheckBox")
			return toolTip;
		else return (getValue(propName).toStringUnit() == " ...") ? QVariant() : getValue(propName).toStringUnit().remove("...").append(toolTip);
	}
	if (role == VariableNameToolTipRole) return (mValue.Description != "*") ? mValue.Description : QString("");

//	if (role == Qt::ToolTipRole) return (mValue.Description != "*") ? mValue.Description : (mValue.VariableName.size() >= 12) ? mValue.VariableName : QString("");
	if (role == Qt::EditRole){
		if (getProp(propName, differentValuesRole).toBool())
			return QVariant();
		return getValue(propName);
	}
	if (role == XStringEditRole){
		return getValue(propName).list();
	}
	return QVariant();
}

XString Edge::getValue(const QString& propName) const
{
	if (propName == "Name") return Name();
	if (propName == "Type") return ObjectType().ObjectType;
	if (propName == "SubType") return ObjectType().SubType;
	if (experimentName() == "Global" && !getProp(propName, differentValuesRole).toBool())
		return props.getProp(propName, parent->experimentsList()[0]);
	return props.getProp(propName, experimentName());
}
bool Edge::setProp(const QString &propName, const QVariant &Value, const int role)
{
	if (role == Qt::EditRole)
	{
		if (getProp(propName, VariableTypeRole).toString().toLower() == "filename" || getProp(propName, VariableTypeRole).toString().toLower() == "directory")
			return setValue(propName, relativePathFilename(Value.toString(), parent->modelPathname()));
		else return setValue(propName, Value.toString());
	}
	if (role == XStringEditRole) return setValue(propName, Value.toStringList());
	if (role == setParamRole)
	{
		QStringList valallUnits;
		valallUnits.append(Value.toString());
		valallUnits.append(getProp(propName, allUnitsRole).toStringList());

		return setValue(propName, valallUnits);
	}

	return false;
}

bool Edge::setValue(const QString &propName, const XString &Value)
{
	QString experiment = (getProp(propName, experimentDependentRole) == "Yes") ? experimentName() : "Global";
	bool r = props.setProp(propName, Value, experiment);
	if (r)
		changed();
	return r;
}

QString Edge::updateSubType()
{
	QString r;
	QStringList Porous;
	Porous << "Soil" << "Darcy" << "Storage";// << "" << "";
	if (Porous.contains(source->ObjectType().ObjectType) || Porous.contains(dest->ObjectType().ObjectType))
		r = "Porous";
	else
		r = "non-Porous";
	if (r != objectType.SubType)
	{
		objectType.SubType = r;
		changed();
		qDebug() << source->ObjectType().ObjectType << dest->ObjectType().ObjectType << objectType.SubType;
	}
	return objectType.SubType;

}

QMap<QString, QVariant> Edge::compact() const
{
	qDebug() << "Compacting: " << name << sourceNode()->Name() << destNode()->Name();

	QMap<QString, QVariant> r;
	r["Source Node"] = sourceNode()->Name();
	r["Dest Node"] = destNode()->Name();

	r["GUI"] = GUI;
	r["Name"] = name;
	r["Type"] = objectType.ObjectType;
	r["SubType"] = objectType.SubType;
	r["Arrow Size"] = arrowSize;
	r["Properties"] = props.compact();
	/*for each (QString key in props.list.keys())
		if (!key.contains("Node")) r[key] = props.list[key].compact();*/
	return r;
}

Edge* Edge::unCompact(QMap<QString, QVariant> n, GraphWidget *gwidget)
{
	qDebug() << "Loading: " << n["Name"].toString() << n["Source Node"].toString() << n["Dest Node"].toString(); 


	QString source = n["Source Node"].toString();
	QString dest = n["Dest Node"].toString();
	qDebug() << gwidget->node(source)->Name();
	qDebug() << gwidget->node(dest)->Name();
	Edge *edge = new Edge(gwidget->node(source), gwidget->node(dest), gwidget);

	if (!gwidget->Edges().contains(edge))
	{
		gwidget->log(QString("%1-%2 connector ignored.").arg(source).arg(dest));
		return nullptr;
	}
	edge->name = n["Name"].toString();
	edge->objectType.ObjectType = n["Type"].toString();
	edge->objectType.SubType = n["SubType"].toString();
	edge->updateSubType();
	edge->arrowSize = n["Arrow Size"].toInt();

	n.remove("Name");
	n.remove("GUI");
	n.remove("Type");
	n.remove("SubType");
	n.remove("Arrow Size");

	edge->props.list = PropList<Edge>::unCompact(n.value("Properties").toString());
	
	/*for each (QString key in n.keys())
		edge->props.list[key] = XString::unCompact(n[key].toString());*/
	return edge;
}
Edge* Edge::unCompact10(QMap<QString, QVariant> n, GraphWidget *gwidget)
{
	qDebug() << "FUNCTION CANCELED.";
	/*qDebug() << "Loading: " << n["Name"].toString() << n["Source Node"].toString() << n["Dest Node"].toString();


	QString source = n["Source Node"].toString();
	QString dest = n["Dest Node"].toString();
	qDebug() << gwidget->node(source)->Name();
	qDebug() << gwidget->node(dest)->Name();
	Edge *edge = new Edge(gwidget->node(source), gwidget->node(dest), gwidget);

	if (!gwidget->Edges().contains(edge))
	{
		gwidget->log(QString("%1-%2 connector ignored.").arg(source).arg(dest));
		return nullptr;
	}
	edge->name = n["Name"].toString();
	edge->objectType.ObjectType = n["Type"].toString();
	edge->objectType.SubType = n["SubType"].toString();
	edge->updateSubType();
	edge->arrowSize = n["Arrow Size"].toInt();

	n.remove("Name");
	n.remove("GUI");
	n.remove("Type");
	n.remove("SubType");
	n.remove("Arrow Size");

	for each (QString key in n.keys())
		edge->props.list[key] = XString::unCompactOld(n[key].toString());*/
	return &Edge(0,0,0);
}

QStringList Edge::codes() const
{
	QStringList r;
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode != "") r.append(mP.VariableCode);
	return r;
}
QMap<QString, condition> Edge::variableNameConditions() const
{
	QMap<QString, condition> r;
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode != "")
		{
			condition c;
			c.Condition = mP.Condition;
			c.error = mP.error;
			c.errorDesc = mP.errorDesc;
			if (c.Condition.count()) r[mP.VariableName] = c;
		}
	return r;
}
XString Edge::val(const QString & code) const
{
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode.toLower() == code.toLower())
		{
			XString r = getValue(mP.VariableName);
//			if (r.unitsList.count() >1) return r.convertTo(r.unitsList[0]);
			if (!r.toQString().isEmpty())
				if (!r.unit.isEmpty())
				{
					if (r.toDouble() == 0)
						return r;
					else
						return r.convertToDefaultUnit();
				}
			return r;
		}
	return XString();
}
void Edge::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QPointF p = QPointF(x() + event->pos().x(), y() + event->pos().y());
	parent->edgeContextMenuRequested(this, p);

}

QStringList Edge::variableNames() const
{
	QStringList r;
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableName != "") r.append(mP.VariableName);
	return r;
}
QString Edge::variableName(QString code) const
{
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode.toLower() == code.toLower()) return mP.VariableName;
	return QString("Error: code (%1) not found.").arg(code);
}
void Edge::changed()
{
	parent->edgeChanged(this);
}

void Edge::copyProps(Node *node, QString direction)
{
	QStringList exceptionList;
	exceptionList << "id";
	if (direction.toLower().contains("v"))
		exceptionList << "width";
	if (direction.toLower().contains("h"))
		exceptionList << "a";

	for each (QString code in node->codes())
	{
		if (!exceptionList.contains(code.toLower()) && codes().contains(code))
			setProp(variableName(code), node->getValue(node->variableName(code)));
	}
}

bool Edge::intersects(double x, double y)
{
	return false;
}
QString Edge::experimentName() const
{
	return parent->experimentName();
}
void Edge::copyProps(QString sourceExperiment, QString destExperiment)
{
	if (props.list.keys().contains(sourceExperiment))
		props.list[destExperiment] = props.list[sourceExperiment];
}