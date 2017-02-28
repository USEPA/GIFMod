#include "propListItem.h"
#include "XString.h"
#include "qstring.h"
#include "node.h"
#include "entity.h"
#include "edge.h"
#include "mProp.h"
#include "mProplist.h"
#include "GWidget.h"

//PropList<Node>::PropList(Node *Parent){ parent = Parent; };

XString PropListItem<Node>::getProp(const QString &inpPropName, Node* parent) const{
	QString propName = inpPropName.toLower();
	if (list.keys().contains(propName))
		return XString(list.value(propName));
	else{
		mProp filter = parent->Filter();
		filter.VariableName = propName;
		mPropList mPL = parent->getmList(filter);
		if (mPL.size() == 0) return ".";
		QList<XString> LX = mPL[0].DefaultValuesList(0,0,parent->parent);
		XString r = (LX.count()) ? LX[0]: "";
		return r;

		//return parent->getmList(filter)[0].DefaultValuesList(0,0,parent->parent)[0];
	}
}

bool PropListItem<Node>::setProp(const QString& inpPropName, const XString& Value, Node* parent) {
//	QString classType = typeid(this).name();
	QString propName = inpPropName.toLower();
	if (propName == "name") {
		if (Value != "") parent->setName(Value);
		return true;
	}
	if (propName == "type") {
//		mProp objType = parent->ObjectType();
//		objType.ObjectType = Value;
		parent->setObjectType(Value);
		return true;
	}
//	if (classType.contains("Node") && (propName == "SubType")) {
	if (propName == "subtype") {
//			mProp objType = parent->ObjectType();
//		objType.SubType = Value;
		parent->setObjectSubType(Value);
		return true;
	}
	if (list.contains(propName)){
		if (Value == "") {
			list.remove(propName);
			return true;
		}
		if (list.value(propName) != Value) {
			XString r = Value;
			if (propName.contains("inflow"))
			{
				QString files;
				for each(QString file in Value.split(';'))
				{
					if (!parent->parent->inflowFileNames.contains(relativePathFilename(file, parent->parent->modelPathname())))
						parent->parent->inflowFileNames.append(relativePathFilename(file, parent->parent->modelPathname()));
					files.append(relativePathFilename(file, parent->parent->modelPathname()));
					files.append(';');
				}
				r = files.left(files.count()-1);
			}
			list[propName] = r;
			return true;
		}
	}
	else
		if (Value != "") {
			list[propName] = Value;
			return true;
		}
	return false;
}

XString PropListItem<Edge>::getProp(const QString &inpPropName, Edge* parent) const{
	QString propName = inpPropName.toLower();
	if (list.contains(propName))
		return XString(list.value(propName));
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

bool PropListItem<Edge>::setProp(const QString& inpPropName, const XString& Value, Edge* parent) {
	QString propName = inpPropName.toLower();
	QString classType = typeid(this).name();
	if (propName == "name") {
		if (Value != "") parent->setName(Value);
		return true;
	}
	if (propName == "type") {
		//		mProp objType = parent->ObjectType();
		//		objType.ObjectType = Value;
		return parent->setObjectType(Value);
	}
	//	if (classType.contains("Node") && (propName == "SubType")) {
//	if (propName == "SubType") {
//		parent->setObjectSubType(Value);
//		return true;
//	}
	if (list.contains(propName)){
		if (Value == "") {
			list.remove(propName);
			return true;
		}
		if (list.value(propName) != Value) {
			list[propName] = Value;
			return true;
		}
	}
	else
	{
		list[propName] = Value;
		return true;
	}
	return false;
	
}

XString PropListItem<Entity>::getProp(const QString &inpPropName, Entity* parent) const{
	QString propName = inpPropName.toLower();
	if (list.contains(propName))
		return XString(list.value(propName));
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

bool PropListItem<Entity>::setProp(const QString& inpPropName, const XString& Value, Entity* parent) {
	QString propName = inpPropName.toLower();
	QString classType = typeid(this).name();
	if (propName == "name") {
		if (Value != "") parent->setName(Value);
		return true;
	}
	if (propName == "type") {
		parent->setObjectType(Value);
		return true;
	}
	if (propName == "subtype") {
		parent->setObjectSubType(Value);
		return true;
	}
	if (list.contains(propName)){
		if (Value == "") {
			list.remove(propName);
			return true;
		}
		if (list.value(propName) != Value) {
			list[propName] = Value;
			return true;
		}
	}
	else
	{
		list[propName] = Value;
		return true;
	}
	return false;
}

