#pragma once
#include <QList>
#include "mProplist.h"
#include "Proplist.h"
#include "PropModel.h"
#include "GWidget.h"
//class mPropList;

//enum EntityType {Precipitation, Inflow, Buildup, Environmental, ExternalFlux, Observation, Parameters, Constituents};


struct SolidAqueousExchangeParameterItem
{
	QString Particle, ExchangeRate, PartitioningCoefficient;
}; 
class Entity //: public QGraphicsItem
{
public:
    //Node(GraphWidget *Widget);
	Entity(const QString _type, QString _name = "No Name", GraphWidget *_parent = 0);
	Entity(const Entity &);
	~Entity() {
		delete model;
	}

	Entity Entity::operator=(const Entity &);
//	*Entity *Entity::operator=(const Entity *);
	//void setProp(const QString _Property, const QString _Value);
	mPropList getmList(const mProp _filter = '*') const;
	void Entity::setName(const QString _name);
	void Entity::setObjectType(const mProp _type);
	mProp ObjectType() const{ return objectType; };
	mProp Filter() const { return ObjectType(); };

	QVariant getProp(const QString &propName, const int role = Qt::DisplayRole) const;
	XString getValue(const QString &propName) const;
	bool setProp(const QString &propName, const QVariant &Value, const int role = Qt::EditRole);
	bool setValue(const QString &propName, const XString &Value);
	mPropList *mList() const;

//	mPropList propList;

	QString Name() const {
		return name;};
//	EntityType Type;
	QString GUI;
	mProp objectType;
	GraphWidget *parent;
	mProp Filter();
	PropModel<Entity> *model;
	PropList<Entity> props;
	QString Entity::newEntityName(const QString &type, const QString name, QList<Entity*> *entities) const;
	QString Entity::newEntityName(const QString name, QStringList &existingNames) const;
	QString name;
	QMap<QString, QVariant> compact() const;
	static Entity* unCompact(QMap<QString, QVariant>, GraphWidget *gwidget, bool oldVersionLoad = false);
	static Entity* unCompact10(QMap<QString, QVariant>, GraphWidget *gwidget);
	QStringList codes() const;
	QMap<QString, condition> variableNameConditions() const;
	void update(bool fast = false);
	bool setObjectSubType(const QString &subType);
	XString val(const QString & code) const;

	QMap<QString, QString> warnings, errors;
	bool errorDetected() const { return (errors.count()) ? true : false; }
	QStringList variableNames() const;
	QString experimentName() const;
	QStringList phases(QString model = "") const{
		QStringList r;
		if (model == "")
			model = objectType.SubType;
		if (objectType.ObjectType == "Particle")
		{
			if (model.contains("Single")) r.append("Mobile");
			if (model.contains("Dual"))
			{
				r.append("Mobile");
				r.append("Attached");
			}
			if (model.contains("Triple"))
			{
				r.append("Mobile");
				r.append("Reversible attached");
				r.append("Irreversible attached");
			}
		}
		return r;
	};
	/*QMap<QString, QList<SolidAqueousExchangeParameterItem>>* solidAqueousExchangeParameters;
	QList<SolidAqueousExchangeParameterItem> &solidAqueousExchangeParameter(QString experimentName = "") const;
	QList<SolidAqueousExchangeParameterItem> &solidAqueousExchangeParameter(QString experimentName = "");
	QString exchangeRate(QString experimentName = "") const{
		QString r;
		if (experimentName == "")
			experimentName = parent->experimentName();
		for each (SolidAqueousExchangeParameterItem i in solidAqueousExchangeParameter(experimentName)){
			if (r != "") r.append("; ");
			r.append("rate[");
			r.append(i.Particle);
			r.append("]=");
			r.append(i.ExchangeRate);
		}
		return r;
	}
	QString partitioningCoefficient(QString experimentName = "") const{
		QString r;
		if (experimentName == "")
			experimentName = parent->experimentName();
		for each (SolidAqueousExchangeParameterItem i in solidAqueousExchangeParameter(experimentName)){
			if (r != "") r.append("; ");
			r.append("kd[");
			r.append(i.Particle);
			r.append("]=");
			r.append(i.PartitioningCoefficient);
		}
		return r;
	}*/
	QList<SolidAqueousExchangeParameterItem> solidAqueousExchangeParameters;
	//QList<SolidAqueousExchangeParameterItem> &solidAqueousExchangeParameter(QString experimentName = "") const;
	//QList<SolidAqueousExchangeParameterItem> &solidAqueousExchangeParameter(QString experimentName = "");
	QString exchangeRate() const{
		QString r;
		for each (SolidAqueousExchangeParameterItem i in solidAqueousExchangeParameters){
			if (r != "") r.append("; ");
			r.append("rate[");
			r.append(i.Particle);
			r.append("]=");
			r.append(i.ExchangeRate);
		}
		return r;
	}
	QString partitioningCoefficient() const{
		QString r;
		for each (SolidAqueousExchangeParameterItem i in solidAqueousExchangeParameters){
			if (r != "") r.append("; ");
			r.append("kd[");
			r.append(i.Particle);
			r.append("]=");
			r.append(i.PartitioningCoefficient);
		}
		return r;
	}
	bool selected = false;
	bool isSelected() const{
		return selected;
	}
	void setSelected(bool select){
		selected = select;
		if (selected)
		{
			parent->tableProp->setModel(model);
			//parent->projectExplorer->ex
		}
	}
	void copyProps(QString sourceExperiment, QString destExperiment);
protected:

signals :
	void changed();

private:
};
