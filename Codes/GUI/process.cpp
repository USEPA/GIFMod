#include <GWidget.h>
#include <qdebug.h>
#include "process.h"
//#include "PropModel.h"
#include "entity.h"

Process::Process(QString name, GraphWidget *parent)
{
	this->parent = parent;
	this->name = newProcessName(name, &parent->Processes);
	parent->Processes.append(this);
	parent->log(QString("One Process created in Reaction Netetwork, name: %1.").arg(name));
}
QString Process::newProcessName(const QString name, QList<Process*> *processes) const
{
	bool repeated = false;
	for each(Process *e in *processes)
		if (e->name == name)
			repeated = true;
	if (!repeated)
		return name;

	QString r = name;

	int op = r.lastIndexOf("(");
	int cp = r.lastIndexOf(")");
	if (op == -1 || cp == -1 || op >= (cp + 1))
		return newProcessName(QString("%1 (1)").arg(r), processes);
	else
	{
		//		QString mid = r.mid(op + 1, cp - op - 1);
		r = r.left(op).trimmed();
		int i = 1;
		while (true)
		{
			repeated = false;
			QString testName = QString("%1 (%2)").arg(r).arg(i++);
			for each(Process *e in *processes)
				if (e->name == testName)
					repeated = true;
			if (!repeated)
				return testName;
		}
	}
	return "something is wrong.";
}
	
/*Process::Process(const Process &E)
{
//	ID = E.ID;
	name = E.name;
//	itemType = E.itemType;
//	propList = E.propList;
	objectType = E.objectType;
	parent = E.parent;
}*/

/*Process Process::operator=(const Process &E)
{
	//	ID = E.ID;
	name = E.name;
	//	itemType = E.itemType;
//	propList = E.propList;
	objectType = E.objectType;
	parent = E.parent;
	return *this;
}*/


QMap<QString, QVariant> Process::compact() const
{
	QMap<QString, QVariant> r;
	r["GUI"] = "Process";
	r["Name"] = name;
	r["Type"] = "Process";
	r["SubType"] = "Process";
	r["Rate"] = rate;
	for each (QString key in stoichiometries.keys())
		r[key] = stoichiometries[key];
	return r;
}

Process* Process::unCompact(QMap<QString, QVariant> n, GraphWidget *gwidget)
{
	Process *process = new Process(n["Name"].toString(), gwidget);
	process->rate = n["Rate"].toString();
	n.remove("Name");
	n.remove("GUI");
	n.remove("Type");
	n.remove("SubType");
	n.remove("Rate");

	for each (QString key in n.keys())
		process->stoichiometries[key] = n[key].toString();
	return process;
}

/*QStringList Process::codes() const
{
	QStringList r;
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode != "") r.append(mP.VariableCode);
	return r;
}*/
/*QMap<QString, condition> Process::variableNameConditions() const
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
}*/

/*XString Process::val(const QString & code) const
{
	for each (mProp mP in getmList(objectType).List)
		if (mP.VariableCode.toLower() == code.toLower())
		{
			XString r = getValue(mP.VariableName);
			if (r.unitsList.count() >1 ) return r.convertTo(r.unitsList[0]); //it used to be zero
			return r;
		}
	return XString();
}*/
QString Process::rateExpression() const {
	return 	toexp(rate
		, parent->EntityNames("Constituent"), parent->EntityNames("Reaction parameter"), parent->functionList, parent->PhysicalCharacteristicsList);
}

QMap<QString, QString> Process::productExpressions() const
{
	QMap<QString, QString> r;
	for each (QString key in stoichiometries.keys())
		if (stoichiometries[key]!="") r[key] = QString("Product[%1]=%2").arg(key).arg(toexp(stoichiometries[key]
			, parent->EntityNames("Constituent"), parent->EntityNames("Reaction parameter"), parent->functionList, parent->PhysicalCharacteristicsList));
	return r;
}
QString toexp(const QString &s, const QStringList& Constituents, const QStringList& Parameters, const QStringList& Functions, const QStringList& Physicals)
{
	QString d;
	int l = 0, r = s.length();
	for (int i = 0; i < s.length();) {
		if (s[i].isSpace()) {
			i++;  continue;
		}
		l = i;
		// the first char must start with a letter or underscore
		if (s[i].isLetter() || s[i] == QChar('_')) {
			while (i < s.length() && (s[i].isLetter() || s[i] == QChar('_') || s[i].isNumber())) i++;
		}
		r = i;
		if (l == r) {
			d.append(s[i]); i++;
		}
		else {
			QString var = s.mid(l, r - l);
			if (Constituents.contains(var))
				d.append(QString("c[%1]").arg(var));
			else if (Parameters.contains(var))
				d.append(QString("k[%1]").arg(var));
			else if (Functions.contains(var))
				d.append(QString("f_%1").arg(var));
			else if (Physicals.contains(var))
				d.append(QString("f[%1]").arg(var));
			else
				d.append(var);
		}
	}
	return d;
}