#pragma once
#include "qmap.h"
#include "proplistitem.h"
#include "multiValues.h"

class Node;
class XString;
class QString;
#include "qvariant.h"

template<class T>
class PropList
{
public:
	PropList() {};
	PropList(T *parent) {this->parent = parent; };
	PropList(const PropList &P){ 
		parent = P.parent; list = P.list; };
//	PropList operator =(const PropList &P){ 
	//	parent = P.parent; list = P.list; return this; };
	~PropList(){};

	T *parent;
	QMap<QString, PropListItem<T>> list;

	QString compact() const{
		QStringList r;
		for each (QString experiment in list.keys())
		{
			QMap<QString, QString> experimentProps = list.value(experiment).compact();
			for each (QString propertyName in experimentProps.keys())
			{
				QString val = experimentProps.value(propertyName);
				if (val == OnlyPath(parent->parent->modelFilename))
					val = ".";
				r.append(experiment + "$$$" + propertyName + "$$$" + val);// experimentProps.value(propertyName));
			}
		}
		if (r.size() == 0)
			return QString();
		else if (r.size() == 1)
			return r[0];
		else
		{
			QString rr = r[0];
			for (int i = 0; i < r.size(); i++)
				rr.append("###" + r[i]);
			return rr;
		}
	}

	static QMap<QString, PropListItem<T>> unCompact(const QString &cInp) {
		QString c = cInp;
		c = c.replace("Global", "All experiments");
		QMap<QString, PropListItem<T>> r;
		if (c.isEmpty())
			return r;
		QStringList l = c.split("###");
		QMultiMap<QString, QStringList> compacted;
		QStringList experiments;
		for (int i = 0; i < l.size(); i++)
			if (!experiments.contains(l[i].split("$$$")[0]) && l[i].split("$$$")[0] != "All experiments")
				experiments.append(l[i].split("$$$")[0]);
		experiments.removeDuplicates();

		for (int i = 0; i < experiments.count(); i++)
			for each (QString line in l)
			{
				QString experiment = experiments[i];
				QStringList list = line.split("$$$");
				if (experiment == list[0])
				{
				//	QStringList applytoExperiments;
				//	applytoExperiments << experiment;
				//	if (experiment == "Global")
				//		applytoExperiments << "experiment1" << "experiment2";
					if (list[1] == "Interface / cross sectional area")
						list[1] = "Interface/cross sectional area";
					if (list[1] == "Saturated hydraulic conductivity")
						list[1] = "Saturated hydraulic conductivity (ks)";
					QStringList propVal = QStringList() << list[1] << list[2];
				//	for each (QString experiment in applytoExperiments)
						compacted.insertMulti(experiment, propVal);
				}
			}
		experiments = compacted.keys();
		experiments.removeDuplicates();

		for each (QString experiment in experiments)
		{
			QList<QStringList> experimentProps = compacted.values(experiment);
			QMap<QString, QVariant> compactedPropListItem;
			for each(QStringList propVal in experimentProps)
				compactedPropListItem[propVal[0]] = propVal[1];
			PropListItem<T> propListItem;
			propListItem.list = PropListItem<T>::unCompact(compactedPropListItem);
			//			QMap<QString, XString> list = PropListItem<T>::unCompact(compactedPropListItem);
			if (!propListItem.list.isEmpty())
			{
				
				r.insert(experiment, propListItem);
			}
		}
		return r;
	}

	XString getProp(const QString& propName, const QString& experimentName, T* parentSub = 0) const; // add a default value for experiment name
	XString getProp(const QString& propName, QList<T*>, const QString& experimentName) const; // add a default value for experiment name
	bool setProp(const QString& propName, const XString& Value, const QString& experimentName);
	multiValues<> getPropMultiValues(const QString& propName, const QStringList &experimentsList) const{
		vector<QVariant> values;
		for (int i = 0;i<experimentsList.count();i++)
			values.push_back(getProp(propName, experimentsList[i]));
		return multiValues<>(values);
	}
	multiValues<> getPropMultiValues(const QString& propName, const QList<Node*> nodes, const QStringList &experimentsList) const;
};
