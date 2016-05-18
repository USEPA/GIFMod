#pragma once
#include "XString.h"
#include "enums.h"

class mPropList;
class GraphWidget;
class mProp
{
public:
	mProp(){};
	mProp(const QString &QS){ 
		//parent = _parent;  
		setValue(QS); };
	mProp(const char &ch){
		//parent = _parent;  
		setValue(QString::fromUtf8(&ch, 1));
	};
	mProp(const QStringList &QL){
		//parent = _parent;  
		setValue(QL); };
//	mProp(const vector<string> &);
//	mProp(const vector<QString> &);
	mProp(const mProp &);
//	mProp(const GProperty &GP);
	mProp mProp::operator = (const mProp &mP);
	mProp mProp::operator = (const QString &QS);
	mProp operator = (const char &);
	mProp operator & (const mProp &) const;

	bool mProp::operator==(const mProp &mP) const;
	bool mProp::operator %=(const mProp &mP) const; // exact comparison of to filter variable wo considering wildcards *
	void setValue(const QStringList &);
	void setValue(const QString &QS);
	QStringList getList() const;

	~mProp(void){};

	int mProp::size() const{ return(getList().size()); };
	QString Model;
	QString GuiObject;
	QString ObjectType;
	QString SubType;
	QString Description;
	XString VariableName;
	QString VariableUnit;
	QString DefaultUnit;
	QString VariableCode;
	QString ExperimentDependent;
	QString VariableType;
	//QString DefaultValues;
	QList<XString> DefaultValuesList(mPropList * , mProp * , GraphWidget *) const;
	QStringList DefaultValuesStringList(mPropList *, mProp *, GraphWidget *) const;
	//	QStringList DefaultValue;
	QString Delegate;
	QString Category;
	QString inputMethod;
	QString DescriptionCode;
	QStringList Condition, error, errorDesc;
	void setDefaultValues(const QString &value, const QString &units, const QString &defaultUnit);
//	XString Value;
	QString DefaultValuesString() const;
//	mPropList *parent;

private:
	XString DefaultValues;

};
bool conditionConformed(QString condition, QString value, GraphWidget *gw);
