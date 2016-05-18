#pragma once
#include <QList>
#include <QMap>
#include <qvariant.h>
//#include "XString.h"

//#include "mProplist.h"
//#include "Proplist.h"
//#include "PropModel.h"
//class mPropList;

//enum EntityType {Precipitation, Inflow, Buildup, Environmental, ExternalFlux, Observation, Parameters, Constituents};

class GraphWidget;

class Process //: public QGraphicsItem
{
public:

	Process(QString name, GraphWidget *_parent);

	~Process() {
	}
	QString rateExpression() const;
	QMap<QString, QString> productExpressions() const;

	GraphWidget *parent;

	QString Process::newProcessName(const QString name, QList<Process*> *precesses) const;
	QString name;
	QString rate;
	QMap<QString, QString> stoichiometries;
	QMap<QString, QVariant> compact() const;
	static Process* unCompact(QMap<QString, QVariant>, GraphWidget *gwidget);


protected:

private:
};
QString toexp(const QString &s, const QStringList& Constituents, const QStringList& Parameters, const QStringList& Functions, const QStringList& Physicals);