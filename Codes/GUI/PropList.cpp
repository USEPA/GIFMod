#include "propList.h"
#include "XString.h"
#include "qstring.h"
#include "node.h"
#include "entity.h"
#include "edge.h"
#include "mProp.h"
#include "mProplist.h"
#include "GWidget.h"

XString PropList<Node>::getProp(const QString &propName, const QString &experimentName) const{
	if (propName.contains("Particle initial"))
		return parent->g(experimentName);
	if (propName.contains("Constituent initial"))
		return parent->cg(experimentName);

	if (list.keys().contains(experimentName))
		return list.value(experimentName).getProp(propName, parent);
	else{
		mProp filter = parent->Filter();
		filter.VariableName = propName;
		mPropList mPL = parent->getmList(filter);
		if (mPL.size() == 0) return ".";
		QList<XString> LX = mPL[0].DefaultValuesList(0,0,parent->parent);
		XString r = (LX.count()) ? LX[0]: "";
		return r;
	}
}

bool PropList<Node>::setProp(const QString& propName, const XString& Value, const QString &experimentName) {
	if (propName == "Name") {
		if (Value != "") parent->setName(Value);
		return true;
	}
	if (propName == "Type") {
		parent->setObjectType(Value);
		return true;
	}
	if (propName == "SubType") {
		parent->setObjectSubType(Value);
		return true;
	}
	if (experimentName == "Global")
	{
		bool r = list[experimentName].setProp(propName, Value, parent);
		for (int i = 1; i < parent->parent->experiments->count(); i++)
			r |= setProp(propName, Value, parent->parent->experiments->itemText(i));
		return r;
	}
	return list[experimentName].setProp(propName, Value, parent);
}

XString PropList<Edge>::getProp(const QString &propName, const QString &experimentName) const{
	if (list.keys().contains(experimentName))
		return list.value(experimentName).getProp(propName, parent);
	else{
		mProp filter = parent->Filter();
		filter.VariableName = propName;
		mPropList mPL = parent->getmList(filter);
		QList<XString> LX = mPL[0].DefaultValuesList(0, 0, parent->parent);
		XString r = LX[0];
		return r;

		return parent->getmList(filter)[0].DefaultValuesList(0, 0, parent->parent)[0];
	}

}

bool PropList<Edge>::setProp(const QString& propName, const XString& Value, const QString &experimentName) {
	QString classType = typeid(this).name();
	if (propName == "Name") {
		if (Value != "") parent->setName(Value);
		return true;
	}
	if (propName == "Type") {
		return parent->setObjectType(Value);
	}
	if (experimentName == "Global")
	{
		bool r = list[experimentName].setProp(propName, Value, parent);
		for (int i = 1; i < parent->parent->experiments->count(); i++)
			r |= setProp(propName, Value, parent->parent->experiments->itemText(i));
		return r;
	}
	return list[experimentName].setProp(propName, Value, parent);
	}

XString PropList<Entity>::getProp(const QString &propName, const QString &experimentName) const{
	if (list.keys().contains(experimentName))
		return list.value(experimentName).getProp(propName, parent);
	else{
		mProp filter = parent->Filter();
		filter.VariableName = propName;
		mPropList mPL = parent->getmList(filter);
		QList<XString> LX = mPL[0].DefaultValuesList(0, 0, parent->parent);
		XString r = (LX.count())? LX[0]:"Some predecessors not defined.";
		return r;

		return parent->getmList(filter)[0].DefaultValuesList(0, 0, parent->parent)[0];
	}
}

bool PropList<Entity>::setProp(const QString& propName, const XString& Value, const QString &experimentName) {
	QString classType = typeid(this).name();
	if (propName == "Name") {
		if (Value != "") parent->setName(Value);
		return true;
	}
	if (propName == "Type") {
		parent->setObjectType(Value);
		return true;
	}
	if (propName == "SubType") {
		parent->setObjectSubType(Value);
		return true;
	}
	if (experimentName == "Global")
	{
		bool r = list[experimentName].setProp(propName, Value, parent); 
		for (int i = 1; i < parent->parent->experiments->count(); i++)
			r |= setProp(propName, Value, parent->parent->experiments->itemText(i));
		return r;
	}
	return list[experimentName].setProp(propName, Value, parent);
}

