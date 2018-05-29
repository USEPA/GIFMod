#include "wizardItem.h"
#include <qdebug.h>
#include <QGraphicsSceneContextMenuEvent>
#include "wizard.h"
#include "qpainter.h"

wizardItem::wizardItem(wizard *parent, const QString &name, const QString &iconFilename, const QString &description, const QPixmap &icon)
{
	//this->parent = parent;
	//setFlag(ItemIsMovable);
	//setFlag(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);
	setAcceptHoverEvents(true);
	setCacheMode(DeviceCoordinateCache);
	//setZValue(1);
	//parent = gwidget;
	
	//itemType = Object_Types::Block;
	//setX(x);
	//setY(y);
	//width = max(minW, _width);
	//height = max(minH, _height);
	//objectType.GuiObject = "Block";
	GUI = "Block";
	this->name = name;
	this->iconFilename = iconFilename;
	this->description = description;
	this->icon = icon;
}

//wizardItem::wizardItem(wizard *parent, const templateIcon &tIcon)
//{
	//wizardItem(parent, tIcon.name, "", tIcon.description, tIcon.icon);
//}

/*
QVariant wizardItem::getProp(const QString &propName, const int role) const
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
*/

/*
QVariant wizardItem::getProp(const QString &propName, const QList<wizardItem*> wizardItems, const int role) const
{
	QList<mProp> filter = Filter(wizardItems);
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
			return props.getPropMultiValues(propName, wizardItems, parent->experimentsList()).differentValues();
	}
	if (role == differentValuesMultiObjectRole)
	{
//		if (mValue.ExperimentDependent == "No" || parent->experimentID())
//			return false;
//		else if (propName == "Name" && wizardItems.count())
			return true;
//else
			return props.getPropMultiValues(propName, wizardItems, parent->experimentsList()).differentValues();
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
		if (getProp(propName, wizardItems, differentValuesRole).toBool())
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
		if (getProp(propName, wizardItems, differentValuesMultiObjectRole).toBool())
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
		if (getProp(propName, wizardItems, differentValuesRole).toBool())
			return QVariant("Different values");
		return getValue(propName);
	}
	if (role == XStringEditRole) {
		return getValue(propName).list();
	}
	return QVariant();
}
*/

/*
XString wizardItem::getValue(const QString& propName) const
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
*/
/*
bool wizardItem::setProp(const QString &propName, const QVariant &Value, const int role)
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

bool wizardItem::setValue(const QString &propName, const XString &Value)
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

QRectF wizardItem::boundingRect() const
{
	qreal adjust = 20;
	return QRectF( 0 - adjust, 0 - adjust, width + adjust, height + adjust);	
}

QPainterPath wizardItem::shape() const
{
    QPainterPath path;
	path.addRect(boundingRect());
	//	path.addRect(0, 0, width, height);
	return path;
}

corners wizardItem::corner(const int _x, const int _y)
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

edgesides wizardItem::edge(const int _x, const int _y)
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
void wizardItem::setBold(const bool bold) {
	this->bold = bold;
	update();
}
*/

void wizardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	painter->setPen(Qt::NoPen);
	painter->setOpacity(0.7);
	QColor Color1 = Qt::yellow;
	QColor Color2 = Qt::darkYellow;

	painter->setBrush(Qt::darkGray);
	QRadialGradient radialGrad(QPointF(width / 2, height / 2), min(width, height));
	if (isSelected())
	{
		radialGrad.setColorAt(0, Qt::green);
		radialGrad.setColorAt(1, Qt::darkGreen);
	}
	radialGrad.setColorAt(0, QColor(Qt::yellow).light(120));
	radialGrad.setColorAt(1, QColor(Qt::darkYellow).light(120));

	QRectF source(0.0, 0.0, 512, 512);
	QPixmap pixmap(icon);
	QRectF rect = QRectF(boundingRect().left() + 10, boundingRect().top(), boundingRect().width() - 20, boundingRect().height() - 20);
	painter->drawPixmap(rect, pixmap, source);
	painter->setPen(QPen(Qt::black, (bold) ? 2 : 0));
	//		qreal factor = parent->transform().scale(1, 1).mapRect(QRectF(0, 0, 1, 1)).width();
	//		int size = 4 + 6 / factor;
	//		QFont QF = painter->font(); QF.setPointSize(size);// QF.pointSize() + 2);
	//		QF.setBold(bold);
	//		painter->setFont(QF);
	painter->drawText(boundingRect(), Qt::AlignCenter | Qt::AlignBottom, Name());
	if (isSelected())
	{
		painter->setBrush(Qt::transparent);
		painter->drawRoundRect(boundingRect(), 10, 10);
	}
}

/*
void wizardItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	bold = true;
	update();
	//QGraphicsSce::mouseMoveEvent(event);
}
*/
/*
QVariant wizardItem::itemChange(GraphicsItemChange change, const QVariant &value)
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
*/
/*
wizardItem::wizardItem(const wizardItem &E)
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

	model = new PropModel<wizardItem>(this);
}
*/
/*
wizardItem wizardItem::operator=(const wizardItem &E)
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

	model = new PropModel<wizardItem>(this);
	return *this;
}*/
/*
bool wizardItem::setName(const QString &Name){
	if (Name == "") return false;
	for each (wizardItem *N in parent->wizardItems())
		if (N->Name() == Name) return false;
	name = Name;
	changed();
	return true;
}

QStringList wizardItem::codes() const
{
	QStringList r;
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode != "") r.append(mP.VariableCode);
	return r;
}
QMap<QString, condition> wizardItem::variableNameConditions() const
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
XString wizardItem::val(const QString & code) const
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

wizardItem* wizardItem::unCompact(QMap<QString, QVariant> n, GraphWidget *gwidget, bool oldVersion)
{

	wizardItem *wizardItem = new wizardItem(gwidget, n["Type"].toString(), n["Name"].toString(), -1, n["X"].toInt(), n["Y"].toInt(), n["Width"].toInt(), n["Height"].toInt());
	wizardItem->setObjectSubType(n["SubType"].toString());
	QStringList connectorNames = n["Connector Names"].toStringList();
	qDebug() << "wizardItem: " << n["Name"].toString() << ", " << wizardItem->Name();
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
		wizardItem->particleInitialCondition(i.split(";")[0]).append(item);
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
		wizardItem->constituentInitialCondition(i.split(";")[0]).append(item);
	}
	n.remove("Constituent Initial Conditions");

	wizardItem->props.list = PropList<wizardItem>::unCompact(n.value("Properties").toString());
	if (!wizardItem->props.list.size() && oldVersion)
		for each(QString key in n.keys())
			wizardItem->props.setProp(key.toLower(), XString::unCompact(n[key].toString()), "experiment1");

	return wizardItem;
}

wizardItem* wizardItem::unCompact10(QMap<QString, QVariant> n, GraphWidget *gwidget)
{

	qDebug() << "FUNCTION CANCELED";
	/*wizardItem *wizardItem = new wizardItem(gwidget, n["Type"].toString(), n["Name"].toString(), -1, n["X"].toInt(), n["Y"].toInt(), n["Width"].toInt(), n["Height"].toInt());
	wizardItem->setObjectSubType(n["SubType"].toString());
	QStringList connectorNames = n["Connector Names"].toStringList();
	qDebug() << "wizardItem: " << n["Name"].toString() << ", " << wizardItem->Name();
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
		wizardItem->particleInitialCondition.append(item);
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
		wizardItem->constituentInitialCondition.append(item);
	}
	n.remove("Constituent Initial Conditions");

	wizardItem->props.list = PropList<wizardItem>::unCompact10(n);
/*	for each (QString key in n.keys()){
		XString r = XString::unCompactOld(n[key].toString());
		wizardItem->props.list.[key] = r;
	}
	return &wizardItem(0);
}
*/

void wizardItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QPointF p = QPointF(x() + event->pos().x(), y() + event->pos().y());
//	parent->wizardItemContextMenuRequested(this, p);

}
/*
QString wizardItem::newwizardItemName(const QString name, QList<wizardItem*> &wizardItems) const
{
	bool repeated = false;
	for each(wizardItem *n in wizardItems)
		if (n->Name() == name)
			repeated = true;
	if (!repeated)
		return name;

	QString r = name;

	int op = r.lastIndexOf("(");
	int cp = r.lastIndexOf(")");
	if (op == -1 || cp == -1 || op >= (cp + 1))
		return newwizardItemName(QString("%1 (1)").arg(r), wizardItems);
	else
	{
		//		QString mid = r.mid(op + 1, cp - op - 1);
		r = r.left(op).trimmed();
		int i = 1;
		while (true)
		{
			repeated = false;
			QString testName = QString("%1 (%2)").arg(r).arg(i++);
			for each(wizardItem *n in wizardItems)
				if (n->Name() == testName)
					repeated = true;
			if (!repeated)
				return testName;
		}
	}
	return "something is wrong.";
}
QStringList wizardItem::variableNames() const
{
	QStringList r;
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableName != "") r.append(mP.VariableName);
	return r;
}
QString wizardItem::variableName(QString code) const
{
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode.toLower() == code.toLower()) return mP.VariableName;
	return QString("Error: code (%1) not found.").arg(code);
}
void wizardItem::changed()
{
	parent->wizardItemChanged(this);
	//update();

}
int wizardItem::experimentID()
{
	return parent->experimentID();
}
QString wizardItem::experimentName() const
{
	return parent->experimentName();
}
void wizardItem::copyProps(QString sourceExperiment, QString destExperiment)
{
	if (props.list.keys().contains(sourceExperiment))
		props.list[destExperiment] = props.list[sourceExperiment];
	if (particleInitialConditions->keys().contains(sourceExperiment))
		particleInitialConditions->insert(destExperiment, particleInitialConditions->operator[](sourceExperiment));
	if (constituentInitialConditions->keys().contains(sourceExperiment))
		constituentInitialConditions->insert(destExperiment, constituentInitialConditions->operator[](sourceExperiment));
}
*/