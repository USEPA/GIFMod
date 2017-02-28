#include "edge.h"
#include "node.h"
//#include <QGraphicsScene>
//#include <QGraphicsSceneMouseEvent>
//#include <QPainter>
//#include <QStyleOption>
#include <qdebug.h>
#include "PropModel.h"
#include "GWidget.h"
#include "qtableview.h"
#include <QGraphicsSceneContextMenuEvent>
#include "treemodel.h"
#include "Proplist.h"
//#include "multiValues.h"
//#include "utility_Funcs.h"

Node::Node(GraphWidget *gwidget, QString _type, QString _name, int _ID, int x, int y, int _width, int _height)
	//: graph(gwidget)
{
	model = new PropModel<Node>(this);
	particleInitialConditions = new QMap < QString, QList<ParticleInitialConditionItem> >;
	constituentInitialConditions = new QMap < QString, QList<ConstituentInitialConditionItem> >;

	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);
	setAcceptHoverEvents(true);
	setCacheMode(DeviceCoordinateCache);
	setZValue(1);
	parent = gwidget;
	
	itemType = Object_Types::Block;
	setX(x);
	setY(y);
	width = max(minW, _width);
	height = max(minH, _height);
	objectType = parent->ModelSpace;//'*';
	objectType.GuiObject = "Block";
	GUI = "Block";
//	QList <mProp> QL;
//	QL = (*parent->mList).List;
//	PropList = (*parent->mList).filter(ObjectType());
//	for (int i = 0; i < PropList.size(); i++)
//		PropList.List[i].Value = QSplit (PropList.List[i].DefaultValues, ':')[0];
//		PropList.List[i].Value = PropList.List[i].DefaultValuesList()[0];
	QString type = (_type == "") ?
		(*parent->mList).filter(objectType).ObjectTypes()[0] :
		_type;
	objectType.ObjectType = type;
	mProp filter;
//	filter = (parent->ModelSpace & ObjectType);
//	filter = (*parent).ModelSpace & ObjectType();
	filter = objectType;
	objectType.SubType = (*parent->mList).filter(filter).SubTypes()[0];
//	props = new PropList<Node>(this);
	props.parent = this;
	parent->MainGraphicsScene->addItem(this);
	setName (newNodeName( (_name == "No Name") ? ObjectType().SubType : _name, parent->Nodes()));
	if (_type == "Tracer")
		parent->treeModel->addTracer(this);
	else if (_type == "Well")
		parent->treeModel->addWell(this);
	else 
		parent->treeModel->add(this);
	parent->log(QString("One block created, type:%1, name:%2.").arg(_type).arg(_name));
	//QObject::connect(this, SIGNAL(changed()), this, SLOT(sendChange()));

	
}

mPropList Node::getmList(const mProp& _filter) const
{
	int type = (_filter.ObjectType == "Tracer")? 1:0;
	
	static mProp filter;
	static mPropList r;
	if (filter %= _filter) return r;
	r = mList()->filter(_filter);
	filter = _filter;
	return r;// (*parent->mList).filter(Filter() & _filter);
}
mPropList Node::getmList(const QList<mProp> _filter) const
{
//	int type = (_filter.ObjectType == "Tracer") ? 1 : 0;

	static QList<mProp> filter;
	static mPropList r;
	
	bool check = true;
	if (mProp::areTheSame(filter, _filter)) return r;
	r = mList()->filter(_filter);
	filter = _filter;
	return r;// (*parent->mList).filter(Filter() & _filter);
}

QVariant Node::getProp(const QString &propName, const int role) const
{
	mProp filter = Filter();
	filter.VariableName = propName;
	mProp mValue = mList()->filter(filter)[0];

	if (role == TypeRole) return mValue.Delegate;
	if (role == InputMethodRole) return mValue.inputMethod;
	if (role == DefaultValuesListRole) 
		return mValue.DefaultValuesStringList(mList(), &Filter(), parent);
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
		return allUnits;}
	if (role == DescriptionCodeRole) return mValue.DescriptionCode;
	if (role == warningConditionRole)
		return mValue.Condition;
	if (role == warningErrorRole)
		return mValue.error;
	if (role == warningErrorDescRole)
		return mValue.errorDesc;
	
	if (role == Qt::ForegroundRole)
	{
		if (warnings.keys().contains(propName)) 
			return QBrush((QColor(255, 111, 28)));
		if (errors.keys().contains(propName)) 
			return QBrush(Qt::red);
		if (getProp(propName, differentValuesRole).toBool())
			return QBrush(Qt::gray);
	}
	if (role == Qt::FontRole) 
	{
		QFont boldFont, redBoldFont;
		boldFont.setBold(true);
		redBoldFont.setBold(true);
		if (parent->applicationShortName == "GWA" && propName == "Name") return boldFont;
		if (parent->applicationShortName != "GWA" && (propName == "Name" || propName == "Type" || propName == "SubType")) return boldFont;
		if (mValue.DefaultValuesStringList(0, 0, parent).indexOf(getValue(propName)) != -1) return boldFont;
		else return QFont();
	}
	if (role == Qt::CheckStateRole)
	if ((mValue.Delegate == "CheckBox") || (mValue.VariableType == "boolean"))
		if ((getValue(propName) != "0") && (getValue(propName).toLower() != "false") && (getValue(propName).toLower() != "no")) return Qt::Checked; else return Qt::Unchecked;

	if (role == fullFileNameRole) return getValue(propName);
	if (role == Qt::DisplayRole)
	{
		if (getProp(propName, differentValuesRole).toBool())
			return QVariant("Different values.");
		if (mValue.VariableType == "filename"){
			qDebug() << OnlyFilenames(getValue(propName));
			return OnlyFilenames(getValue(propName));
		}
		else if (mValue.Delegate == "CheckBox")
			return "";
		else return getValue(propName).toStringUnit();
	}
	if (role == Qt::ToolTipRole)
	{
		QString toolTip;
		if (errors.keys().contains(propName)) toolTip.append("\n Error: ").append(errors[propName]);
		if (warnings.keys().contains(propName)) toolTip.append("\n Warning: ").append(warnings[propName]);
		
		if (mValue.VariableType.contains("filename"))
			return getValue(propName).append(toolTip);
		else if (mValue.Delegate.contains("CheckBox"))
			return toolTip;
		else return (getValue(propName).toStringUnit() == " ...") ? QVariant() : getValue(propName).toStringUnit().remove("...").append(toolTip);
	}
	if (role == VariableNameToolTipRole) return (mValue.Description != "*") ? mValue.Description : QVariant();
	if (role == Qt::EditRole){
		if (getProp(propName, differentValuesRole).toBool())
			return QVariant("Different values");
		return getValue(propName);
	}
	if (role == XStringEditRole){
		return getValue(propName).list();
	}
	return QVariant();
}
QVariant Node::getProp(const QString &propName, const QList<Node*> nodes, const int role) const
{
	QList<mProp> filter = Filter(nodes);
	for (int i=0;i<filter.size();i++)filter[i].VariableName = propName;
	mProp mValue = mList()->filter(filter)[0];

	if (role == TypeRole) return mValue.Delegate;
	if (role == InputMethodRole) return mValue.inputMethod;
	if (role == DefaultValuesListRole)
	{
		multiValues<> defaultValues;
		for (int i = 0; i<filter.size(); i++)
			defaultValues.append(mValue.DefaultValuesStringList(mList(), &filter[i], parent));
		if (defaultValues.sameValues())
			return defaultValues.value();
		return QString("different values");
	}
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
			return props.getPropMultiValues(propName, nodes, parent->experimentsList()).differentValues();
	}
	if (role == differentValuesMultiObjectRole)
	{
//		if (mValue.ExperimentDependent == "No" || parent->experimentID())
//			return false;
//		else if (propName == "Name" && nodes.count())
			return true;
//else
			return props.getPropMultiValues(propName, nodes, parent->experimentsList()).differentValues();
	}
	if (role == allUnitsRole) {
		QStringList allUnits;
		allUnits.append(getValue(propName).unit);
		allUnits.append(getValue(propName).unitsList);
		allUnits.append(getValue(propName).defaultUnit);
		return allUnits;
	}
	if (role == DescriptionCodeRole) return mValue.DescriptionCode;
	if (role == warningConditionRole)
		return mValue.Condition;
	if (role == warningErrorRole)
		return mValue.error;
	if (role == warningErrorDescRole)
		return mValue.errorDesc;

	if (role == Qt::ForegroundRole)
	{
		if (warnings.keys().contains(propName))
			return QBrush((QColor(255, 111, 128)));
		if (errors.keys().contains(propName))
			return QBrush(Qt::red);
		if (getProp(propName, nodes, differentValuesRole).toBool())
			return QBrush(Qt::gray);
	}
	if (role == Qt::FontRole)
	{
		QFont boldFont, redBoldFont;
		boldFont.setBold(true);
		redBoldFont.setBold(true);
		if (parent->applicationShortName == "GWA" && propName == "Name") return boldFont;
		if (parent->applicationShortName != "GWA" && (propName == "Name" || propName == "Type" || propName == "SubType")) return boldFont;
		if (mValue.DefaultValuesStringList(0, 0, parent).indexOf(getValue(propName)) != -1) return boldFont;
		else return QFont();
	}
	if (role == Qt::CheckStateRole)
		if ((mValue.Delegate == "CheckBox") || (mValue.VariableType == "boolean"))
			if ((getValue(propName) != "0") && (getValue(propName).toLower() != "false") && (getValue(propName).toLower() != "no")) return Qt::Checked; else return Qt::Unchecked;

	if (role == fullFileNameRole) return getValue(propName);
	if (role == Qt::DisplayRole)
	{
		if (getProp(propName, nodes, differentValuesMultiObjectRole).toBool())
			return QVariant("Different values.");
		if (mValue.VariableType == "filename") {
			qDebug() << OnlyFilenames(getValue(propName));
			return OnlyFilenames(getValue(propName));
		}
		else if (mValue.Delegate == "CheckBox")
			return "";
		else return getValue(propName).toStringUnit();
	}
	if (role == Qt::ToolTipRole)
	{
		QString toolTip;
		if (errors.keys().contains(propName)) toolTip.append("\n Error: ").append(errors[propName]);
		if (warnings.keys().contains(propName)) toolTip.append("\n Warning: ").append(warnings[propName]);

		if (mValue.VariableType.contains("filename"))
			return getValue(propName).append(toolTip);
		else if (mValue.Delegate.contains("CheckBox"))
			return toolTip;
		else return (getValue(propName).toStringUnit() == " ...") ? QVariant() : getValue(propName).toStringUnit().remove("...").append(toolTip);
	}
	if (role == VariableNameToolTipRole) return (mValue.Description != "*") ? mValue.Description : QVariant();
	if (role == Qt::EditRole) {
		if (getProp(propName, nodes, differentValuesRole).toBool())
			return QVariant("Different values");
		return getValue(propName);
	}
	if (role == XStringEditRole) {
		return getValue(propName).list();
	}
	return QVariant();
}
XString Node::getValue(const QString& propName) const
{
	//qDebug() << "propName = " << propName;
	if (propName == "Name") return Name();
	if (propName == "Type") return ObjectType().ObjectType;
	if (propName == "SubType" || propName == "Distribution") return ObjectType().SubType;
	if (propName.contains("Particle initial")) 
		return QString ("%1 ...").arg(g());
	if (propName.contains("Constituent initial")) 
		return QString("%1 ...").arg(cg());
	if (experimentName() == "All experiments" && !getProp(propName, differentValuesRole).toBool())
		return props.getProp(propName, parent->experimentsList()[0]);

	return props.getProp(propName, experimentName());
}

bool Node::setProp(const QString &propName, const QVariant &Value, const int role)
{
	if ((parent->applicationShortName == "GWA" && propName == "Distribution") || (parent->applicationShortName == "GIFMod" && propName == "Evapotranspiration model") ||
		(parent->applicationShortName == "GIFMod" && propName == "Particle model")) return setProp("SubType", Value, role);
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
		
		return setValue(propName, Value.toStringList());
	}

	return false;
}

bool Node::setValue(const QString &propName, const XString &Value)
{
	QString experiment = (getProp(propName, experimentDependentRole) == "Yes") ? experimentName() : "All experiments";
	bool r = props.setProp(propName, Value, experiment);
	if (r)
	{
		update();
		changed();
	}
	return r;
}

mPropList* Node::mList() const
{
	return parent->mList; 
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

/*bool Node::advance()
{
	setPos(pos());
	update();
	return true;
}*/

QRectF Node::boundingRect() const
{
	qreal adjust = 20;
	return QRectF( 0 - adjust, 0 - adjust, width + adjust, height + adjust);	
}

QPainterPath Node::shape() const
{
    QPainterPath path;
	path.addRect(boundingRect());
	//	path.addRect(0, 0, width, height);
	return path;
}

corners Node::corner(const int _x, const int _y)
{
	if (parent->applicationShortName == "GWA")return corners::none;

	int border = 15;
	if (abs(_x - x()) < border)
	{
		if (abs(_y - y()) < border) return (topleft);
		if (abs(_y - y() - height) < border) return (bottomleft);
	}
	if (abs(_x - x()-width) < border)
	{
		if (abs(_y - y()) < border) return (topright);
		if (abs(_y - y() - height) < border) return (bottomright);
	}
	return (corners::none);
}

edgesides Node::edge(const int _x, const int _y)
{
	if (parent->applicationShortName == "GWA")return edgesides::noside;
	int hBorder = (height > 60) ? 15 : height / 4;
	int vBorder = (width > 60) ? 15 : width / 4;
	if (corner(_x, _y)) return edgesides::noside;
	if (abs(_x - x()) < vBorder) return edgesides::leftside;
	if (abs(_y - y()) < hBorder) return edgesides::topside;
	if (abs(_y - y() - height) < hBorder) return edgesides::bottomside;
	if (abs(_x - x() - width) < vBorder) return edgesides::rightside;
	return edgesides::noside;
}
void Node::setBold(const bool bold) {
	this->bold = bold;
	update();
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	painter->setPen(Qt::NoPen);
	painter->setOpacity(0.7);
	QColor Color1, Color2;
	if (parent->colorCode.nodes)
	{
		Color1 = color.color1; Color2 = color.color2;
	}

	else
	{
		if (ObjectType().ObjectType == "Soil")
		{
			Color1 = Qt::yellow; Color2 = Qt::darkYellow;
		}
		if (ObjectType().ObjectType == "Catchment")
		{
			Color1 = Qt::darkGray; Color2 = Qt::black;
		}
		if (ObjectType().ObjectType == "Pond")
		{
			Color1 = Qt::cyan; Color2 = Qt::darkCyan;
		}
		if (ObjectType().ObjectType == "Stream")
		{
			Color1 = Qt::blue; Color2 = Qt::darkBlue;
		}
		if (ObjectType().ObjectType == "Storage")
		{
			Color1 = Qt::gray; Color2 = Qt::darkGray;
		}
		if (ObjectType().ObjectType == "Darcy")
		{
			Color1 = Qt::darkCyan; Color2 = Qt::darkGray;
		}
	}
	if (ObjectType().ObjectType == "Well")
	{
		Color1 = Qt::yellow; Color2 = Qt::darkYellow;
	}
	painter->setBrush(Qt::darkGray);
	QRadialGradient radialGrad(QPointF(width / 2, height / 2), min(width, height));
	if (isSelected())
	{
		radialGrad.setColorAt(0, Qt::green);
		radialGrad.setColorAt(1, Qt::darkGreen);
	}
	else if (!parent->colorCode.nodes)

	{
		if (ObjectType().ObjectType == "Soil")
		{
			radialGrad.setColorAt(0, QColor(Qt::yellow).light(120));
			radialGrad.setColorAt(1, QColor(Qt::darkYellow).light(120));
		}
		if (ObjectType().ObjectType == "Catchment")
		{
			radialGrad.setColorAt(0, QColor(Qt::darkGray).light(120));
			radialGrad.setColorAt(1, QColor(Qt::black).light(120));
		}
		if (ObjectType().ObjectType == "Pond")
		{
			radialGrad.setColorAt(0, QColor(Qt::cyan).light(120));
			radialGrad.setColorAt(1, QColor(Qt::darkCyan).light(120));
		}
		if (ObjectType().ObjectType == "Stream")
		{
			radialGrad.setColorAt(0, QColor(Qt::blue).light(120));
			radialGrad.setColorAt(1, QColor(Qt::darkBlue).light(120));
		}
		if (ObjectType().ObjectType == "Storage")
		{
			radialGrad.setColorAt(0, QColor(Qt::gray).light(120));
			radialGrad.setColorAt(1, QColor(Qt::darkGray).light(120));
		}
		if (ObjectType().ObjectType == "Darcy")
		{
			radialGrad.setColorAt(0, QColor(Qt::darkCyan).light(120));
			radialGrad.setColorAt(1, QColor(Qt::darkGray).light(120));
		}
		if (ObjectType().ObjectType == "Well")
		{
			radialGrad.setColorAt(0, QColor(Qt::yellow).light(120));
			radialGrad.setColorAt(1, QColor(Qt::darkYellow).light(120));
		}
		if (errorDetected())
		{
			radialGrad.setColorAt(0, QColor(Qt::red).light(120));
			radialGrad.setColorAt(1, QColor(Qt::gray).light(120));
		}
	}

	if (ObjectType().ObjectType == "Well")
	{
		QRectF source(0.0, 0.0, 512, 512);
		QPixmap pixmap("icons/well.png");
		QRectF rect = QRectF(boundingRect().left() + 10, boundingRect().top(), boundingRect().width() - 20, boundingRect().height() - 20);
		painter->drawPixmap(rect, pixmap, source);
		painter->setPen(QPen(Qt::black, (bold) ? 2 : 0));
		qreal factor = parent->transform().scale(1, 1).mapRect(QRectF(0, 0, 1, 1)).width();
		int size = 4 + 6 / factor;
		QFont QF = painter->font(); QF.setPointSize(size);// QF.pointSize() + 2);
		QF.setBold(bold);
		painter->setFont(QF);
		painter->drawText(boundingRect(), Qt::AlignCenter | Qt::AlignBottom, Name());
		if (isSelected())
		{
			painter->setBrush(Qt::transparent);
			painter->drawRoundRect(boundingRect(), 10, 10);
			if (parent->tableProp->model() != model) {
				parent->tableProp->setModel(model);
			}
		}
	}
	else if (ObjectType().ObjectType == "Tracer")
	{
		QRectF source(0.0, 0.0, 211.0, 239.0);
		QPixmap pixmap("icons/tracer.png");
		QRectF rect = QRectF(boundingRect().left() + 10, boundingRect().top(), boundingRect().width() - 20, boundingRect().height() - 20);
		painter->drawPixmap(rect, pixmap, source);
		painter->setPen(QPen(Qt::black, (bold) ? 2 : 0));
		qreal factor = parent->transform().scale(1, 1).mapRect(QRectF(0, 0, 1, 1)).width();
		int size = 4 + 6 / factor;
		QFont QF = painter->font(); QF.setPointSize(size);// QF.pointSize() + 2);
		QF.setBold(bold);
		painter->setFont(QF);
		painter->drawText(boundingRect(), Qt::AlignCenter | Qt::AlignBottom, Name());
		if (isSelected())
		{
			painter->setBrush(Qt::transparent);
			painter->drawRoundRect(boundingRect(), 10, 10);
			if (parent->tableProp->model() != model) {
				parent->tableProp->setModel(model);
			}
		}
	}
	else
	{
		if (parent->colorCode.nodes)
			painter->setBrush(color.color1);
		else
			painter->setBrush(radialGrad);
		painter->setPen(QPen(Qt::black, (bold) ? 2 : 0));
		painter->drawRoundRect(0, 0, width, height, 10, 10);
		qreal factor = parent->transform().scale(1, 1).mapRect(QRectF(0, 0, 1, 1)).width();
		int size = 4 + 6 / factor;
		QFont QF = painter->font(); QF.setPointSize(size);// QF.pointSize() + 2);
		QF.setBold(bold);
		painter->setFont(QF);
		if (ObjectType().ObjectType == "Well" || ObjectType().ObjectType == "Tracer")
			painter->drawText(10, height - 10, QString("%1: %2").arg(ObjectType().ObjectType).arg(Name()));
		else
		{
			painter->drawText(10, height - 10, QString("%1, z0=%2").arg(Name()).arg(XString(getProp(variableName("z0")).toStringList())));
			if (parent->colorCode.nodes && middleText!="")
			{
				painter->drawText(10, height / 2 - 10, middleText);
			}
		}
		if (parent->selectedNodes().count() == 1 && parent->selectedEdges().count() == 0) // only one node is selected
		{
			if (isSelected())
			{
				if (parent->tableProp->model() != model) {
					parent->tableProp->setModel(model);
				}
			}
		}
		else
		{
			QList<Node*> l = parent->selectedNodes();
			QList<Edge*> m = parent->selectedEdges();
			if (parent->selectedNodes().count() > 1 && parent->selectedEdges().count() == 0)//multi nodes are selected and no edges
			{
				static PropModel<Node>* multiModel = 0;
				if (!multiModel)
					multiModel = new PropModel<Node>(parent->selectedNodes());
				if (multiModel->itemsList()!=parent->selectedNodes())
					multiModel = new PropModel<Node>(parent->selectedNodes());
				parent->tableProp->setModel(multiModel);
			}
		}
	}
}

void Node::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	bold = true;
	update();
	//QGraphicsSce::mouseMoveEvent(event);
}

bool Node::setObjectType(const QString &type)
{
	if (objectType.ObjectType == type) return false;
	else
	{
		objectType.ObjectType = type;
		objectType.SubType = '*';
		objectType.SubType = (*parent->mList).filter(Filter()).SubTypes()[0];
		for each (Edge * edge in edgeList)
			edge->updateSubType();
		//update();
		changed();
		return true;
	}
}

bool Node::setObjectSubType(const QString &subType)
{
	if (objectType.SubType == subType) return false;
	else
	{
		objectType.SubType = subType;
		changed();
		return true;
	}
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    };
    return QGraphicsItem::itemChange(change, value);
}

Node::Node(const Node &E)
{
	setFlags(E.flags());
	setAcceptHoverEvents(true);
	setCacheMode(DeviceCoordinateCache);
	setZValue(E.zValue());
	setPos(E.pos());

	edgeList = E.edgeList;
	newPos = E.newPos;
//	graph = E.graph; 
//	ID = E.ID;
	name = E.name;
	GUI = E.GUI;
	width = E.Width();
	height = E.Height();
	parent = E.parent;
	props = E.props;
	props.parent = this;
	itemType = E.itemType;
	width = E.Width();
	height = E.Height();
	objectType = E.objectType;
	particleInitialConditions = new QMap<QString, QList<ParticleInitialConditionItem>>(*E.particleInitialConditions);
	constituentInitialConditions = new QMap<QString, QList<ConstituentInitialConditionItem>>(*E.constituentInitialConditions);

	model = new PropModel<Node>(this);
}

Node Node::operator=(const Node &E)
{
	setFlags(E.flags());
	setAcceptHoverEvents(true);
	setCacheMode(DeviceCoordinateCache);
	setZValue(E.zValue());
	setPos(E.pos());

	edgeList = E.edgeList;
	newPos = E.newPos;
	//	graph = E.graph; 
	//	ID = E.ID;
	name = E.name;
	GUI = E.GUI;
	width = E.Width();
	height = E.Height();
	parent = E.parent;
	props.list = E.props.list;
	props.parent = this;
	itemType = E.itemType;
	width = E.Width();
	height = E.Height();
	objectType = E.objectType;
	particleInitialConditions = E.particleInitialConditions;
	constituentInitialConditions = E.constituentInitialConditions;

	model = new PropModel<Node>(this);
/*	setPos(E.pos());
	edgeList = E.edgeList;
	newPos = E.newPos;
//	graph = E.graph;
//	ID = E.ID;
	name = E.name;
//	itemType = E.itemType;
	GUI = E.GUI;
	width = E.Width();
	height = E.Height();
	parent = E.parent;
	props = E.props; */
	return *this;
}

bool Node::setName(const QString &Name){
	if (Name == "") return false;
	for each (Node *N in parent->Nodes())
		if (N->Name() == Name) return false;
	name = Name;
	changed();
	return true;
}

QString Node::g(QString experimentName) const
{
	QString g;
	if (experimentName == "")
		experimentName = parent->experimentName();
	for each (ParticleInitialConditionItem i in particleInitialCondition(experimentName)){
		if (g != "") g.append("; ");
		g.append("g[");
		g.append(i.Particle);
		if (i.Model != "") {
			g.append(":");
			g.append(i.Model);
		}
		g.append("]=");
		g.append(i.Value);
	}
	return g;
}

QString Node::cg(QString experimentName) const
{
	QString cg;
	if (experimentName == "")
		experimentName = parent->experimentName();
	for each (ConstituentInitialConditionItem i in constituentInitialCondition(experimentName)){
		if (cg != "") cg.append("; ");
		cg.append("cg[");
		cg.append(i.Constituent);
		if (i.Particle != "") {
			cg.append(":");
			cg.append(i.Particle);
		}
		if (i.Model != "") {
			cg.append(":");
			cg.append(i.Model);
		}
		cg.append("]=");
		cg.append(i.Value);
	}
	return cg;
}
QList<ParticleInitialConditionItem> &Node::particleInitialCondition(QString experimentName) const
{
	//qDebug() << "particleInitialCondition const";
	if (experimentName == "")
		experimentName = parent->experimentName();
	if (experimentName != "All experiments")
		return particleInitialConditions->operator[](experimentName);
	if (parent->experiments->count() == 2)
		return particleInitialConditions->operator[](parent->firstExperimentName());
	vector<QVariant> gValues;
	for (int i = 0; i < parent->experimentsList().count(); i++)
		gValues.push_back(g(parent->experimentsList()[i]));
	multiValues<> gMultiValues(gValues);
	if (gMultiValues.sameValues())
		particleInitialConditions->operator[](experimentName) = particleInitialConditions->operator[](parent->firstExperimentName());
	else
		particleInitialConditions->operator[](experimentName) = QList<ParticleInitialConditionItem>();
	//qDebug() << "particleInitialCondition const exit";
	return particleInitialConditions->operator[](experimentName);
}
QList<ParticleInitialConditionItem> &Node::particleInitialCondition(QString experimentName)
{
	//qDebug() << "particleInitialCondition";
	if (experimentName == "")
		experimentName = parent->experimentName();
	if (experimentName != "All experiments")
		return particleInitialConditions->operator[](experimentName);
	if (parent->experiments->count() == 2)
		return particleInitialConditions->operator[](parent->firstExperimentName());
	vector<QVariant> gValues;
	for (int i = 0; i < parent->experimentsList().count(); i++)
		gValues.push_back(g(parent->experimentsList()[i]));
	multiValues<> gMultiValues(gValues);
	if (gMultiValues.sameValues())
		particleInitialConditions->operator[](experimentName) = particleInitialConditions->operator[](parent->firstExperimentName());
	else
		particleInitialConditions->operator[](experimentName) = QList<ParticleInitialConditionItem>();
	//qDebug() << "particleInitialCondition exit";
	return particleInitialConditions->operator[](experimentName);
}

QList<ConstituentInitialConditionItem> &Node::constituentInitialCondition(QString experimentName) const
{
	//qDebug() << "constituentInitialCondition const";
	if (experimentName == "")
		experimentName = parent->experimentName();
	if (experimentName != "All experiments")
		return constituentInitialConditions->operator[](experimentName);
	if (parent->experiments->count() == 2)
		return constituentInitialConditions->operator[](parent->firstExperimentName());
	vector<QVariant> gValues;
	for (int i = 0; i < parent->experimentsList().count(); i++)
		gValues.push_back(cg(parent->experimentsList()[i]));
	multiValues<> gMultiValues(gValues);
	if (gMultiValues.sameValues())
		constituentInitialConditions->operator[](experimentName) = constituentInitialConditions->operator[](parent->firstExperimentName());
	else
		constituentInitialConditions->operator[](experimentName) = QList<ConstituentInitialConditionItem>();
	//qDebug() << "constituentInitialCondition const exit";
	return constituentInitialConditions->operator[](experimentName);
}
QList<ConstituentInitialConditionItem> &Node::constituentInitialCondition(QString experimentName)
{
	//qDebug() << "constituentInitialCondition";
	if (experimentName == "")
		experimentName = parent->experimentName();
	if (experimentName != "All experiments")
		return constituentInitialConditions->operator[](experimentName);
	if (parent->experiments->count() == 2)
		return constituentInitialConditions->operator[](parent->firstExperimentName());
	vector<QVariant> gValues;
	for (int i = 0; i < parent->experimentsList().count(); i++)
		gValues.push_back(cg(parent->experimentsList()[i]));
	multiValues<> gMultiValues(gValues);
	if (gMultiValues.sameValues())
		constituentInitialConditions->operator[](experimentName) = constituentInitialConditions->operator[](parent->firstExperimentName());
	else
		constituentInitialConditions->operator[](experimentName) = QList<ConstituentInitialConditionItem>();
	//qDebug() << "constituentInitialCondition exit";
	return constituentInitialConditions->operator[](experimentName);
}

QStringList Node::codes() const
{
	QStringList r;
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode != "") r.append(mP.VariableCode);
	return r;
}

QMap<QString, condition> Node::variableNameConditions() const
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
XString Node::val(const QString & code) const
{
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode.toLower() == code.toLower())
		{
			XString r = getValue(mP.VariableName);
			//if (mP.VariableType.contains("float") &&
//			if (r.unitsList.count() > 1) return r.convertTo(r.unitsList[0]);
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

QMap<QString, QVariant> Node::compact() const
{
//	qDebug() << "Compacting: " << name;
	QMap<QString, QVariant> r;
	QStringList connectorNames;
	for each (Edge * e in edgeList)
		connectorNames.append(e->Name());

	QStringList PICList;
	for each (QString experiment in parent->experimentsList())
		for each (ParticleInitialConditionItem i in particleInitialCondition(experiment))
			PICList.append(QString("%1;%2;%3;%4").arg(experiment).arg(i.Particle).arg(i.Model).arg(i.Value));
	QStringList CICList;
	for each (QString experiment in parent->experimentsList())
		for each (ConstituentInitialConditionItem i in constituentInitialCondition(experiment))
			CICList.append(QString("%1;%2;%3;%4;%5").arg(experiment).arg(i.Constituent).arg(i.Particle).arg(i.Model).arg(i.Value));

	r["GUI"] = GUI;
	r["Name"] = name;
	r["Type"] = objectType.ObjectType;
	r["SubType"] = objectType.SubType;
	r["X"] = x();
	r["Y"] = y();
	r["Width"] = Width();
	r["Height"] = Height();
	r["Connector Names"] = connectorNames;
	r["Properties"] = props.compact();
	/*for each (QString key in props.list.keys())
		r[key] = props.list[key].compact();*/
	r["Particle Initial Conditions"] = PICList;
	r["Constituent Initial Conditions"] = CICList;
	return r;
}
Node* Node::unCompact(QMap<QString, QVariant> n, GraphWidget *gwidget, bool oldVersion)
{

	Node *node = new Node(gwidget, n["Type"].toString(), n["Name"].toString(), -1, n["X"].toInt(), n["Y"].toInt(), n["Width"].toInt(), n["Height"].toInt());
	node->setObjectSubType(n["SubType"].toString());
	QStringList connectorNames = n["Connector Names"].toStringList();
	qDebug() << "Node: " << n["Name"].toString() << ", " << node->Name();
	n.remove("Name");
	n.remove("GUI");
	n.remove("Type");
	n.remove("SubType");
	n.remove("X");
	n.remove("Y");
	n.remove("Width");
	n.remove("Height");
	n.remove("Connector Names");

	QStringList PICList;
	PICList = n["Particle Initial Conditions"].toStringList();
	for each (QString i in PICList)
	{
		if (i.split(";").size() == 3)
			i.push_front("All experiments;");
		ParticleInitialConditionItem item;
		item.Particle = i.split(";")[1];
		item.Model = i.split(";")[2];
		item.Value = i.split(";")[3];
		node->particleInitialCondition(i.split(";")[0]).append(item);
	}
	n.remove("Particle Initial Conditions");

	QStringList CICList;
	CICList = n["Constituent Initial Conditions"].toStringList();
	for each (QString i in CICList)
	{
		if (i.split(";").size() == 4)
			i.push_front("All experiments;");
		ConstituentInitialConditionItem item;
		item.Constituent = i.split(";")[1];
		item.Particle = i.split(";")[2];
		item.Model = i.split(";")[3];
		item.Value = i.split(";")[4];
		node->constituentInitialCondition(i.split(";")[0]).append(item);
	}
	n.remove("Constituent Initial Conditions");

	node->props.list = PropList<Node>::unCompact(n.value("Properties").toString());
	if (!node->props.list.size() && oldVersion)
		for each(QString key in n.keys())
			node->props.setProp(key.toLower(), XString::unCompact(n[key].toString()), "experiment1");

	return node;
}

Node* Node::unCompact10(QMap<QString, QVariant> n, GraphWidget *gwidget)
{

	qDebug() << "FUNCTION CANCELED";
	/*Node *node = new Node(gwidget, n["Type"].toString(), n["Name"].toString(), -1, n["X"].toInt(), n["Y"].toInt(), n["Width"].toInt(), n["Height"].toInt());
	node->setObjectSubType(n["SubType"].toString());
	QStringList connectorNames = n["Connector Names"].toStringList();
	qDebug() << "Node: " << n["Name"].toString() << ", " << node->Name();
	n.remove("Name");
	n.remove("GUI");
	n.remove("Type");
	n.remove("SubType");
	n.remove("X");
	n.remove("Y");
	n.remove("Width");
	n.remove("Height");
	n.remove("Connector Names");


	QStringList PICList;
	PICList = n["Particle Initial Conditions"].toStringList();
	for each (QString i in PICList)
	{
		ParticleInitialConditionItem item;
		item.Particle = i.split(";")[0];
		item.Model = i.split(";")[1];
		item.Value = i.split(";")[2];
		node->particleInitialCondition.append(item);
	}
	n.remove("Particle Initial Conditions");

	QStringList CICList;
	CICList = n["Constituent Initial Conditions"].toStringList();
	for each (QString i in CICList)
	{
		ConstituentInitialConditionItem item;
		item.Constituent = i.split(";")[0];
		item.Particle = i.split(";")[1];
		item.Model = i.split(";")[2];
		item.Value = i.split(";")[3];
		node->constituentInitialCondition.append(item);
	}
	n.remove("Constituent Initial Conditions");

	node->props.list = PropList<Node>::unCompact10(n);
/*	for each (QString key in n.keys()){
		XString r = XString::unCompactOld(n[key].toString());
		node->props.list.[key] = r;
	}*/
	return &Node(0);
}

void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QPointF p = QPointF(x() + event->pos().x(), y() + event->pos().y());
	parent->nodeContextMenuRequested(this, p);

}
QString Node::newNodeName(const QString name, QList<Node*> &nodes) const
{
	bool repeated = false;
	for each(Node *n in nodes)
		if (n->Name() == name)
			repeated = true;
	if (!repeated)
		return name;

	QString r = name;

	int op = r.lastIndexOf("(");
	int cp = r.lastIndexOf(")");
	if (op == -1 || cp == -1 || op >= (cp + 1))
		return newNodeName(QString("%1 (1)").arg(r), nodes);
	else
	{
		//		QString mid = r.mid(op + 1, cp - op - 1);
		r = r.left(op).trimmed();
		int i = 1;
		while (true)
		{
			repeated = false;
			QString testName = QString("%1 (%2)").arg(r).arg(i++);
			for each(Node *n in nodes)
				if (n->Name() == testName)
					repeated = true;
			if (!repeated)
				return testName;
		}
	}
	return "something is wrong.";
}
QStringList Node::variableNames() const
{
	QStringList r;
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableName != "") r.append(mP.VariableName);
	return r;
}
QString Node::variableName(QString code) const
{
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode.toLower() == code.toLower()) return mP.VariableName;
	return QString("Error: code (%1) not found.").arg(code);
}
void Node::changed()
{
	parent->nodeChanged(this);
	//update();

}
int Node::experimentID()
{
	return parent->experimentID();
}
QString Node::experimentName() const
{
	return parent->experimentName();
}
void Node::copyProps(QString sourceExperiment, QString destExperiment)
{
	if (props.list.keys().contains(sourceExperiment))
		props.list[destExperiment] = props.list[sourceExperiment];
	if (particleInitialConditions->keys().contains(sourceExperiment))
		particleInitialConditions->insert(destExperiment, particleInitialConditions->operator[](sourceExperiment));
	if (constituentInitialConditions->keys().contains(sourceExperiment))
		constituentInitialConditions->insert(destExperiment, constituentInitialConditions->operator[](sourceExperiment));
}