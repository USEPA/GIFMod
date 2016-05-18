#include <GWidget.h>
#include <qdebug.h>
#include "entity.h"
#include "qdatetime.h"
#include "qtimezone.h"
//#include "PropModel.h"
#include "treemodel.h"
#include "Proplist.h"


Entity::Entity(const QString _type, QString _name, GraphWidget *_parent)
{
 	parent = _parent;
	
	objectType = parent->ModelSpace;//'*';
	objectType.GuiObject = "Entity";
	GUI = "Entity";


	QList <mProp> QL;
	QL = (*parent->mList).List;
	objectType.ObjectType = _type;
	mProp filter;
	filter = objectType;
	objectType.SubType = (*parent->mList).filter(filter).SubTypes()[0];
	static int ID;
	if (_name == "No Name" || _name == "") 
		_name = _type;
	if (_type == "Reaction parameter" || _type == "Constituent")
	{
		QStringList existingNames;
		for each (Entity *e in parent->entitiesByType("Constituent"))
			existingNames.append(e->Name());
		for each (Entity *e in parent->entitiesByType("Reaction parameter"))
			existingNames.append(e->Name());
		name = newEntityName(_name, existingNames);
	}
	else
		name = newEntityName(_type, _name, &parent->Entities);
	props.parent = this;
	model = new PropModel<Entity>(this);
	parent->Entities.append(this);
	parent->treeModel->add(this);
	changed();
	parent->log(QString("One %1 created, name:%2.").arg(_type).arg(_name));
}
void Entity::setName(const QString _name)
{
	if (name == _name) return;
	name = _name;
	changed();
}
QString Entity::newEntityName(const QString &type, const QString name, QList<Entity*> *entities) const
{
	QStringList existingNames;
	for each(Entity *e in *entities)
		if (e->objectType.ObjectType == type)
			existingNames.append(e->Name());
	return newEntityName(name, existingNames);
}
QString Entity::newEntityName(const QString name, QStringList &existingNames) const
{
	bool repeated = false;
	for each(QString existingName in existingNames)
		if (existingName == name)
			repeated = true;
	if (!repeated)
		return name;

	QString r = name;

	int op = r.lastIndexOf("(");
	int cp = r.lastIndexOf(")");
	if (op == -1 || cp == -1 || op >= (cp + 1))
		return newEntityName(QString("%1 (1)").arg(r), existingNames);
	else
	{
		//		QString mid = r.mid(op + 1, cp - op - 1);
		r = r.left(op).trimmed();
		int i = 1;
		while (true)
		{
			repeated = false;
			QString testName = QString("%1 (%2)").arg(r).arg(i++);
			for each(QString existingName in existingNames)
				if (existingName == testName)
					repeated = true;
			if (!repeated)
				return testName;
		}
	}
	return "something is wrong.";
}

void Entity::setObjectType(const mProp _type)
{
	if (objectType == _type) return;
	; objectType = _type;
	changed();
}

mProp Entity::Filter()
{
	return((*parent).ModelSpace & objectType);
}
mPropList Entity::getmList(const mProp _filter) const
{
	static mProp filter;
	static mPropList r;
	if (filter %= _filter) return r;
	r = mList()->filter(_filter);
	filter = _filter;
	return r;// (propList.filter((*parent).ModelSpace & objectType & _filter));
}

Entity::Entity(const Entity &E)
{
//	ID = E.ID;
	name = E.name;
//	itemType = E.itemType;
//	propList = E.propList;
	objectType = E.objectType;
	parent = E.parent;
	solidAqueousExchangeParameters = E.solidAqueousExchangeParameters;
	props.list = E.props.list;
}

Entity Entity::operator=(const Entity &E)
{
	//	ID = E.ID;
	name = E.name;
	//	itemType = E.itemType;
//	propList = E.propList;
	objectType = E.objectType;
	parent = E.parent;
	solidAqueousExchangeParameters = E.solidAqueousExchangeParameters;
	props.list = E.props.list;
	return *this;
}

QVariant Entity::getProp(const QString &propName, const int role) const
{
	mProp filter = Filter();
	filter.VariableName = propName;
	if (!mList()->filter(filter).size())
		return QVariant(); //has error
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

	if (role == fullFileNameRole)
		return getValue(propName);
	if (role == Qt::DisplayRole)// || role == Qt::ToolTipRole)
	{
		if (getProp(propName, differentValuesRole).toBool())
			return QVariant("Different values.");
		if (mValue.VariableType == "filename")
			return OnlyFilenames(getValue(propName));
		else if (mValue.VariableType == "directory")
			return OnlyFilenames(getValue(propName));
		else if (mValue.VariableType == "datetime")
		{
			qint64 currentDate = getValue(propName).toFloat();
			currentDate += QDate(1900, 1, 1).toJulianDay();
			QDate date = QDate::fromJulianDay(currentDate);
			
//			QDateTime r = QDateTime::fromTime_t(getValue(propName).toFloat() * 86400 - 2209161600, QTimeZone(0));
//			return r.toString("MMM dd yyyy HH:mm");// QDateTime(getValue(propName));
			return date.toString("MMM dd yyyy");// QDateTime(getValue(propName));

		}
		else if (mValue.Delegate == "CheckBox")
			return "";
		else return getValue(propName).toStringUnit();
	}
	if (role == VariableNameToolTipRole) return (mValue.Description != "*") ? mValue.Description : QString("");
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

XString Entity::getValue(const QString& propName) const
{
	if (propName == "Name") return Name();
	if (propName == "Type") return ObjectType().ObjectType;
	if (experimentName() == "Global" && !getProp(propName, differentValuesRole).toBool())
		return props.getProp(propName, parent->experimentsList()[0]);
	if (propName == "SubType") return ObjectType().SubType;
	if (propName == "SubType" || (objectType.ObjectType=="Evapotranpiration" && propName == "Model")
		|| (objectType.ObjectType == "Particle" && propName == "Model")) return ObjectType().SubType;
	if (getProp(propName, VariableTypeRole).toString().toLower() == "filename" || getProp(propName, VariableTypeRole).toString().toLower() == "directory")
		return (relativePathFilename(props.getProp(propName, experimentName()), parent->modelPathname()));
	if (propName =="Exchange rate")
		return QString("%1 ...").arg(exchangeRate());
	if (propName.contains("Partitioning"))
		return QString("%1 ...").arg(partitioningCoefficient());

	return props.getProp(propName, experimentName());
}
bool Entity::setProp(const QString &propName, const QVariant &Value, const int role)
{
	if ((parent->applicationShortName == "GIFMod" && objectType.ObjectType == "Evapotranspiration" && propName == "Model") ||
		(parent->applicationShortName == "GIFMod" && objectType.ObjectType == "Particle" && propName == "Model"))
		return setProp("SubType", Value, role);
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

bool Entity::setValue(const QString &propName, const XString &Value)
{
	QString experiment = (getProp(propName, experimentDependentRole) == "Yes") ? experimentName() : "Global";
	bool r = props.setProp(propName, Value, experiment);
	if (r)
	{
		update();
		changed();
	}
	return r;
}

bool Entity::setObjectSubType(const QString &subType)
{
	if (objectType.SubType == subType) return false;
	else
	{
		objectType.SubType = subType;
		changed();
		return true;
	}
}

mPropList* Entity::mList() const { 
	return parent->mList; };

QMap<QString, QVariant> Entity::compact() const
{
	QMap<QString, QVariant> r;
	r["GUI"] = GUI;
	r["Name"] = name;
	r["Type"] = objectType.ObjectType;
	r["SubType"] = objectType.SubType;
	
	r["Properties"] = props.compact();

	QStringList xChangeList;
	for each (SolidAqueousExchangeParameterItem i in solidAqueousExchangeParameters)
		xChangeList.append(QString("%1;%2;%3").arg(i.Particle).arg(i.ExchangeRate).arg(i.PartitioningCoefficient));
	r["Solid exchange parameters"] = xChangeList;

	/*for each (QString key in props.keys())
	{
		if (key == "Working Path" && props.list[key].toQString() == OnlyPath(parent->modelFilename))
			r[key] = XString('.').compact();
		else
			r[key] = item.listItem[key].compact();
	}*/
	return r;
}

Entity* Entity::unCompact(QMap<QString, QVariant> n, GraphWidget *gwidget)
{
#ifdef GWA
	if (n["Name"] == "Solver setting") return 0;
	QStringList subTypeConversionEntities;
	subTypeConversionEntities << "Global Settings" << "Genetic Algorithm" << "Markov Chain Monte Carlo";

	if (n["SubType"] == "*" && subTypeConversionEntities.contains(n["Name"].toString()))
		n["SubType"] = n["Name"];
#endif
	QStringList list;
	list << "solver setting" << "global Settings" << "climate setting" << "genetic algorithm" << "markov chain monte carlo";
	if (n["Type"].toString().contains("Solver"))
	{
		n["Type"] = "Solver setting";
		n["Name"] = n["Type"];
		n["SubType"] = n["Type"];

	}
	else if (n["Type"] == "Global Settings")
	{
		n["Type"] = "Project setting";
		n["Name"] = n["Type"];
		n["SubType"] = n["Type"];
	}
	else if (n["Type"].toString().contains("Climate"))
	{
		n["Type"] = "Climate setting";
		n["Name"] = n["Type"];
		n["SubType"] = n["Type"];
	}
	else if (n["Type"] == "Genetic Algorithm")
	{
		n["Type"] = "Genetic algorithm";
		n["Name"] = n["Type"];
		n["SubType"] = n["Type"];
	}
	else if (n["Type"] == "Markov Chain Monte Carlo")
	{
		n["Type"] = "Markov chain Monte Carlo";
		n["Name"] = n["Type"];
		n["SubType"] = n["Type"];
	}
	else if (!list.contains(n["Type"].toString().toLower()))
	{
		QString s = n["Type"].toString();
		QString newS;
		int i = s.split(' ').size();
		if (i == 1)
			newS = s;
		else
		{
			QStringList exceptions;
			exceptions << "MCMC" << "Arrhenius" << "GA" << "Newton" << "Raphson" << "NR" << "Hazen" << "Williams";
			QStringList list;
			QStringList klist = s.split(' ');
			list << klist[0];
			for (int i = 1; i < klist.size(); i++)
				if (!exceptions.contains(klist[i]))
					list << klist[i].toLower();
				else
					list << klist[i];
			newS = list.join(' ');
		}
		n["Type"] = newS;

		s = n["SubType"].toString();
		
		i = s.split(' ').size();
		if (i == 1)
			newS = s;
		else
		{
			QStringList list;
			QStringList klist = s.split(' ');
			list << klist[0];
			for (int i = 1; i < klist.size(); i++)
				list << klist[i].toLower();
			newS = list.join(' ');
		}
		n["SubType"] = newS;

	}

	Entity *entity = new Entity(n["Type"].toString(), n["Name"].toString(), gwidget);

	entity->objectType.SubType = n["SubType"].toString();

	n.remove("Name");
	n.remove("GUI");
	n.remove("Type");
	n.remove("SubType");

	entity->props.list = PropList<Entity>::unCompact(n.value("Properties").toString()); 
	
	/*for each (QString key in n.keys())
			entity->props.list[key] = XString::unCompact(n[key].toString());*/

	QStringList xChangeList;
	xChangeList = n["Solid exchange parameters"].toStringList();
	for each (QString i in xChangeList)
	{
		SolidAqueousExchangeParameterItem item;
		item.Particle = i.split(";")[0];
		item.ExchangeRate = i.split(";")[1];
		item.PartitioningCoefficient = i.split(";")[2];
		entity->solidAqueousExchangeParameters.append(item);
	}
	n.remove("Solid exchange parameters");

	return entity;
}
Entity* Entity::unCompact10(QMap<QString, QVariant> n, GraphWidget *gwidget)
{
	qDebug() << "FUNCTION CANCELED!";
	/*
	Entity *entity = new Entity(n["Type"].toString(), n["Name"].toString(), gwidget);

	entity->objectType.SubType = n["SubType"].toString();

	n.remove("Name");
	n.remove("GUI");
	n.remove("Type");
	n.remove("SubType");

	for each (QString key in n.keys())
		entity->props.list[key] = XString::unCompactOld(n[key].toString());
		*/
	return &Entity("","",gwidget);
}
QStringList Entity::variableNames() const
{
	QStringList r;
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableName != "") r.append(mP.VariableName);
	return r;
}
QStringList Entity::codes() const
{
	QStringList r;
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode != "") r.append(mP.VariableCode);
	return r;
}
QMap<QString, condition> Entity::variableNameConditions() const
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

XString Entity::val(const QString & code) const
{
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode.toLower() == code.toLower())
		{
			XString r = getValue(mP.VariableName);
//			if (r.unitsList.count() >1 ) return r.convertTo(r.unitsList[0]); 
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
void Entity::changed()
{
	parent->entityChanged(this);
}
QString Entity::experimentName() const
{
	return parent->experimentName();
}
/*QList<SolidAqueousExchangeParameterItem> &Entity::solidAqueousExchangeParameter(QString experimentName) const
{
	if (experimentName == "")
		experimentName = parent->experimentName();
	if (experimentName != "Global")
		return solidAqueousExchangeParameters->operator[](experimentName);
	if (parent->experiments->count() == 2)
		return solidAqueousExchangeParameters->operator[](parent->firstExperimentName());
	vector<QVariant> rateValues, kdValues;
	for (int i = 0; i < parent->experimentsList().count(); i++)
	{
		rateValues.push_back(exchangeRate(parent->experimentsList()[i]));
		kdValues.push_back(partitioningCoefficient(parent->experimentsList()[i]));
	}
	multiValues rateMultiValues(rateValues), kdMultiValues(kdValues);
	if (rateMultiValues.sameValues() && kdMultiValues.sameValues())
		solidAqueousExchangeParameters->operator[](experimentName) = solidAqueousExchangeParameters->operator[](parent->firstExperimentName());
	else
		solidAqueousExchangeParameters->operator[](experimentName) = QList<SolidAqueousExchangeParameterItem>();
	//qDebug() << "particleInitialCondition const exit";
	return solidAqueousExchangeParameters->operator[](experimentName);
}
QList<SolidAqueousExchangeParameterItem> &Entity::solidAqueousExchangeParameter(QString experimentName)
{
	//qDebug() << "particleInitialCondition";
	if (experimentName == "")
		experimentName = parent->experimentName();
	if (experimentName != "Global")
		return solidAqueousExchangeParameters->operator[](experimentName);
	if (parent->experiments->count() == 2)
		return solidAqueousExchangeParameters->operator[](parent->firstExperimentName());
	vector<QVariant> rateValues, kdValues;
	for (int i = 0; i < parent->experimentsList().count(); i++)
	{
		rateValues.push_back(exchangeRate(parent->experimentsList()[i]));
		kdValues.push_back(partitioningCoefficient(parent->experimentsList()[i]));
	}
	multiValues rateMultiValues(rateValues), kdMultiValues(kdValues);
	if (rateMultiValues.sameValues() && kdMultiValues.sameValues())
		solidAqueousExchangeParameters->operator[](experimentName) = solidAqueousExchangeParameters->operator[](parent->firstExperimentName());
	else
		solidAqueousExchangeParameters->operator[](experimentName) = QList<SolidAqueousExchangeParameterItem>();
	//qDebug() << "particleInitialCondition exit";
	return solidAqueousExchangeParameters->operator[](experimentName);
}
*/
void Entity::copyProps(QString sourceExperiment, QString destExperiment)
{
	if (props.list.keys().contains(sourceExperiment))
		props.list[destExperiment] = props.list[sourceExperiment];
}