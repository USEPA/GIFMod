#include "mProp.h"
#include "mProplist.h"
#include "GWidget.h"
#include "entity.h"


mProp::mProp(const mProp &mP)
{
	Model = mP.Model;
	GuiObject = mP.GuiObject;
	ObjectType = mP.ObjectType;
	SubType = mP.SubType;
	Description = mP.Description;
	VariableName = mP.VariableName;
	VariableCode = mP.VariableCode;
	VariableUnit = mP.VariableUnit;
	DefaultUnit = mP.DefaultUnit;
	VariableType = mP.VariableType;
	DefaultValues = mP.DefaultValues;
	Delegate = mP.Delegate;
	Category = mP.Category;
	inputMethod = mP.inputMethod;
	DescriptionCode = mP.DescriptionCode;
	Condition = mP.Condition;
	error = mP.error;
	errorDesc = mP.errorDesc;
	ExperimentDependent = mP.ExperimentDependent;
}

mProp mProp::operator= (const mProp &mP)
{
	setValue(mP.getList());
	return(*this);
}
mProp mProp::operator = (const QString &QS)
{
	setValue(QS);
	return(*this);
}
mProp mProp::operator = (const char &ch)
{
	setValue(QString::fromUtf8(&ch,1));
	return (*this);
}

bool mProp::operator==(const mProp &mP) const
{
	bool r = true;
	QStringList QL = getList(), mPQL = mP.getList();
	if (QL.size() != mPQL.size())
		return false;
	for (int i = 0; i < QL.size(); ++i)
		if ((QL[i] != "*") && (mPQL[i] != "*") && (QL[i] != mPQL[i]))
			return false;
	return r;
}
bool mProp::operator%=(const mProp &mP) const
{
	bool r = true;
	QStringList QL = getList(), mPQL = mP.getList();
	if (QL.size() != mPQL.size())
		return false;

	for (int i = 0; i < QL.size(); ++i)
		if (QL[i] != mPQL[i])
			return (r = false);
	return r;
}

mProp mProp::operator & (const mProp &mP) const
{
	QStringList r, QL=getList() , mPQL = mP.getList();
	if (QL.size() != mPQL.size())
		return false;
	for (int i = 0; i < mPQL.size(); i++)
		r << ( QL[i] & mPQL[i]);
	return mProp(r);
}

void mProp::setValue(const QStringList &QL)
{
	if (QL.size() != getList().size())
		return;

	Model = QL[0];
	GuiObject = QL[1];
	ObjectType = QL[2];
	SubType = QL[3];
	Description = QL[4];
	VariableName = QL[5];
	VariableCode = QL[6];
	VariableUnit = QL[7];
	DefaultUnit = QL[8];
	VariableType = QL[9];
	DefaultValues = QL[10];
	Delegate = QL[11];
	Category = QL[12];
	inputMethod = QL[13];
	ExperimentDependent = QL[14];
	DescriptionCode = QL[15];
	Condition = QL[16].split(';');
	error = QL[17].split(';');
	errorDesc = QL[18].split(';');

}
void mProp::setValue(const QString &QS)
{
	QStringList QL;
	int size = getList().size();
	for (int i = 0; i < size; i++)
		QL << QS;
	setValue(QL);
}

QStringList mProp::getList() const
{

	QStringList r;
	r << Model << GuiObject << ObjectType << SubType << Description << VariableName << VariableCode << VariableUnit << DefaultUnit << VariableType << DefaultValues
		<< Delegate << Category << inputMethod << ExperimentDependent << DescriptionCode << Condition.join(':') << error.join(':') << errorDesc.join(':');
	return(r);
}

QList<XString> mProp::DefaultValuesList(mPropList *mPL, mProp* filter, GraphWidget *gWidget) const
{
	
	if (DefaultValues.toLower().contains("add at runtime"))
	{
		QList<XString> r;
		for each (QString s in DefaultValues.split(';'))
		{
			if (s.toLower().contains("add at runtime"))
			{
				if (s.toLower().contains("particle phases"))
				{
					for each(Entity* p in gWidget->entitiesByType("Particle"))
						for each (QString phase in p->phases())
							r.append(QString("%1:%2").arg(p->Name()).arg(phase));
				}
				else if (s.toLower().contains("constituent phases"))
				{
					for each(Entity* c in gWidget->entitiesByType("Constituent"))
					{
						r.append(QString("%1:%2").arg(c->Name()).arg("Aqueous"));
						r.append(QString("%1:%2").arg(c->Name()).arg("Soil"));
						for each(Entity* p in gWidget->entitiesByType("Particle"))
							for each (QString phase in p->phases())
								r.append(QString("%1:%2:%3").arg(c->Name()).arg(p->Name()).arg(phase));
					}
				}
			}
			else
				r.append(XString(s, VariableUnit.split(';')[0], VariableUnit.split(';'), DefaultUnit));
		}
		return r;
	}
	else if (!DefaultValues.toLower().contains("populate at runtime"))
	{
		QList<XString> r;
		for each (QString s in DefaultValues.split(';'))
			r.append(XString(s, VariableUnit.split(';')[0], VariableUnit.split(';'), DefaultUnit));
		return r;
	}
	else
	{
		QList<XString> r;
		QStringList r1;
		if (DefaultValues.toLower().contains("blank")) r.append("");
		if (DefaultValues.toLower().contains("tracer"))
			r1.append(gWidget->nodeNames("Tracer"));
		if (DefaultValues.toLower().contains("well"))
			r1.append(gWidget->nodeNames("Well"));
		//		if (DefaultValues.toLower().contains("precipitation")) return QList<XString>() << "precip 1" << "precip 2" << "precip 3";
		if (DefaultValues.toLower().contains("particle"))
			r1.append(gWidget->EntityNames("Particle"));
		if (DefaultValues.toLower().contains("constituent")) 
			r1.append(gWidget->EntityNames("Constituent"));
		if (DefaultValues.toLower().contains("build-up"))
			r1.append(gWidget->EntityNames("Build-up"));
		if (DefaultValues.toLower().contains("External flux"))
			r1.append(gWidget->EntityNames("External flux"));
		if (DefaultValues.toLower().contains("evapotranspiration"))
			r1.append(gWidget->EntityNames("Evapotranspiration"));
		if (DefaultValues.toLower().contains("inflow"))
			r1.append(gWidget->inflowFileNames);
		if (DefaultValues.toLower().contains("block"))
			r1.append(gWidget->nodeNames());
		if (DefaultValues.toLower().contains("connector"))
			r1.append(gWidget->edgeNames());
		if (DefaultValues.toLower().contains("experiment"))
			r1.append(gWidget->experimentsList());
		if (DefaultValues.toLower().contains("cg[const]"))
			for each (QString c in gWidget->EntityNames("Constituent")) r.append(QString("cg[%1]").arg(c));
		if (DefaultValues.toLower().contains("subtype") || DefaultValues.toLower().contains("distribution")){
			if (mPL == nullptr || filter == nullptr) return QList<XString>() << "";
			mProp Filter = *filter;
			Filter.SubType = '*';
			QStringList subtypes = mPL->SubTypes(Filter);
			QList<XString> r;
			for (int i = 0; i < subtypes.size(); i++) r.append(subtypes[i]);
			return r;
		}
		if (DefaultValues.toLower().contains("type")){
			if (mPL == nullptr || filter == nullptr) return QList<XString>() << "";
			mProp Filter = *filter;
			Filter.ObjectType = '*';
			Filter.SubType = '*';
			QStringList types = mPL->ObjectTypes(Filter);
			QList<XString> r;
			for (int i = 0; i < types.size(); i++) r.append(types[i]);
			return r;
		}
		if (r1.count())
			for each (QString QS in r1)
				r.append(QS);
		return r;
//		qDebug() << getList();
//			return QList<XString>() << "Check the" << "mProp.cpp code" << "Default Values" << "for population.";
//		return QList<XString>() << "*";
	}

}
QStringList mProp::DefaultValuesStringList(mPropList *mPL, mProp* filter, GraphWidget *gWidget) const {
	QStringList R;
	QList<XString> DL = DefaultValuesList(mPL, filter, gWidget);
	for (int i = 0; i< DL.size(); i++)
		R.append(DL[i].toQString());
	return R;
};

void mProp::setDefaultValues(const QString &value, const QString &units, const QString &defaultUnit)
{
	DefaultValues = XString(value, units.split(";"), defaultUnit);
}

QString mProp::DefaultValuesString() const
{
	return (DefaultValues);
}


QStringList uniques(const QStringList& list)
{
	QStringList R= list;
	R.removeDuplicates();
	R.removeAll("*");
	return R;
}

QString operator&(const QString &q1, const QString &q2)
{

	if ((q1 == "*") || (q1 == q2))
		return q2;
	else if (q2 == "*")
		return q1;
	else
		return "error";
}
QString QMerge(const QStringList &QSL, QChar del)
{
	QString QS = QSL[0];
	for (int i = 1; i < QSL.size(); i++)
		QS += del + QSL[i];
	return QS;
}

QString OnlyFilename(const QString& PathandFilename)
{
	QString r = PathandFilename.split('\\')[PathandFilename.split('\\').size() - 1];
	r = r.split('/')[r.split('/').size() - 1];

	if (r == "..") r = "Parent Folder";
	if (r == ".") r = "Current Folder";
	return r;// PathandFilename.split('\\')[PathandFilename.split('\\').size() - 1];
}
QString OnlyFilenames(const QString& PathandFilenames)
{
	QStringList PathandFilenamesList = PathandFilenames.split(';');
	QString r = OnlyFilename(PathandFilenamesList[0]);
	for (int i = 1; i < PathandFilenamesList.size(); i++)
		r += QString("; %1").arg(OnlyFilename(PathandFilenamesList[i]));
	return r;
}
QString relativePathFilename(const QString &file, const QString &dir)
{
	QString r = file;
	if (OnlyPath(r).contains(dir)) r.replace(dir, ".");
	return r;
}
QString OnlyPath(const QString& PathandFilename)
{
	return PathandFilename.left(PathandFilename.count() - OnlyFilename(PathandFilename).count() - 1);
}

QString updateRelativePath(const QString&fileWithRelativePath, const QString& oldPath, const QString& newPath)
{
	QString newFilePath = fileWithRelativePath;
	newFilePath.replace('./', oldPath);
	return relativePathFilename(newFilePath, newPath);
}
QString fullFilename(QString fileWithRelativePath, QString  dir)
{
	return fileWithRelativePath.replace("./", dir.append('/'));
}

bool conditionConformed(QString condition, QString value, GraphWidget *gw)
{
	if (condition.toLower().contains("inflow format")) 
		return validInflowFile(fullFilename(value, gw->modelPathname()));
	if (condition.toLower().contains("not empty")) return !value.isEmpty();
	if (condition.contains("!=")) return(value.toFloat() != condition.remove("!=").toFloat());
	if (condition.contains(">=")) return(value.toFloat() >= condition.remove(">=").toFloat());
	if (condition.contains("<=")) return(value.toFloat() <= condition.remove("<=").toFloat());
	if (condition.contains('>')) return(value.toFloat() > condition.remove('>').toFloat());
	if (condition.contains('<')) return(value.toFloat() < condition.remove('<').toFloat());

	if (condition.contains('['))
	{
		if (condition.contains(']')) return ((value.toFloat() >= condition.remove('[').remove(']').split(':')[0].toFloat()) && (value.toFloat() <= condition.remove('[').remove(']').split(':')[1].toFloat()));
		if (condition.contains(')')) return ((value.toFloat() >= condition.remove('[').remove(')').split(':')[0].toFloat()) && (value.toFloat() < condition.remove('[').remove(')').split(':')[1].toFloat()));
	}
	if (condition.contains('('))
	{
		if (condition.contains(']')) return ((value.toFloat() > condition.remove('(').remove(']').split(':')[0].toFloat()) && (value.toFloat() <= condition.remove('(').remove(']').split(':')[1].toFloat()));
		if (condition.contains(')')) return ((value.toFloat() > condition.remove('(').remove(')').split(':')[0].toFloat()) && (value.toFloat() < condition.remove('(').remove(')').split(':')[1].toFloat()));
	}
}
