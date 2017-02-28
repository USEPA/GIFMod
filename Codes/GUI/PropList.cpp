#include "propList.h"
#include "XString.h"
#include "qstring.h"
#include "node.h"
#include "entity.h"
#include "edge.h"
#include "mProp.h"
#include "mProplist.h"
#include "GWidget.h"

XString PropList<Node>::getProp(const QString &propName, const QString &experimentName, Node* parentSub) const{
	if (!parentSub)
		parentSub = parent;
	if (propName.contains("Particle initial"))
		return parentSub->g(experimentName);
	if (propName.contains("Constituent initial"))
		return parentSub->cg(experimentName);
	if (propName == "Name")
		return XString(parentSub->Name());
	if (propName == "Type")
		return XString(parentSub->objectType.ObjectType);
	if (propName == "SubType")
		return XString(parentSub->objectType.SubType);

	if (list.keys().contains(experimentName))
		return list.value(experimentName).getProp(propName, parentSub);
	else{
		mProp filter = parentSub->Filter();
		filter.VariableName = propName;
		mPropList mPL = parentSub->getmList(filter);
		if (mPL.size() == 0) return ".";
		QList<XString> LX = mPL[0].DefaultValuesList(0,0,parentSub->parent);
		XString r = (LX.count()) ? LX[0]: "";
		return r;
	}
}
XString PropList<Node>::getProp(const QString& propName, QList<Node*> nodes, const QString& experimentName) const {
	multiValues<XString> multi;
	for each (Node* n in nodes)
	{
		multi.append(getProp(propName, experimentName, n));
	}
	if (multi.sameValues())
		return multi.value();
	else
		return "Different values";
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
	if (experimentName == "All experiments")
	{
		bool r = list[experimentName].setProp(propName, Value, parent);
		for (int i = 1; i < parent->parent->experiments->count(); i++)
			r |= setProp(propName, Value, parent->parent->experiments->itemText(i));
		return r;
	}
	return list[experimentName].setProp(propName, Value, parent);
}

XString PropList<Edge>::getProp(const QString &propName, const QString &experimentName, Edge* parentSub) const{
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
	if (experimentName == "All experiments")
	{
		bool r = list[experimentName].setProp(propName, Value, parent);
		for (int i = 1; i < parent->parent->experiments->count(); i++)
			r |= setProp(propName, Value, parent->parent->experiments->itemText(i));
		return r;
	}
	return list[experimentName].setProp(propName, Value, parent);
	}

XString PropList<Entity>::getProp(const QString &propName, const QString &experimentName, Entity* parentSub) const{
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
	if (experimentName == "All experiments")
	{
		bool r = list[experimentName].setProp(propName, Value, parent); 
		for (int i = 1; i < parent->parent->experiments->count(); i++)
			r |= setProp(propName, Value, parent->parent->experiments->itemText(i));
		return r;
	}
	return list[experimentName].setProp(propName, Value, parent);
}
multiValues<> PropList<Node>::getPropMultiValues(const QString& propName, const QList<Node*> nodes, const QStringList &experimentsList) const {
	vector<QVariant> values;
	for (int i = 0; i<experimentsList.count(); i++)
		values.push_back(getProp(propName, nodes, experimentsList[i]));
	return multiValues<>(values);
}
