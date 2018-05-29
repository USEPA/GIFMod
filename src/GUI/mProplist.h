#pragma once

//#include "XString.h"
//#include "node.h"
#include "mProp.h"
//#include "qstring.h"
#include "enums.h"
//#include "GWidget.h"

//typedef void(*FPointer)();

class mPropList
{
public:
	mPropList();
	mPropList(const QList<mProp> &mPL){ List = mPL; };
	mPropList(QString filename);
	mPropList(const mPropList &CC);
	mPropList& operator= (const mPropList &CC); 
	mProp operator[](int i) const { 
		if (i >= List.size())
			return mProp(); //has error
		return List[i]; };
	~mPropList(){};
	int size() const { return List.size(); };
    mListReadStatus getconfigfromfile(QString filename);
//	mListReadStatus mPropList::getconfigfromfile(QString filename, GraphWidget *GW);
	QList<mProp> List;
	QStringList Models(const mProp &mP = '*') const;
	QStringList GuiObjects(const mProp &mP = '*') const;
	QStringList ObjectTypes(const mProp &mP = '*') const;
	QStringList SubTypes(const mProp &mP = '*') const;
	QStringList Descriptions(const mProp &mP = '*') const;
	QStringList VariableNames(const mProp &mP = '*') const;
	QStringList VariableNames_w_abv(const mProp & mP='*') const;
//	QStringList mPropList::VariableNames(const QList<mProp> objectTypes) const;
	QStringList VariableUnits(const mProp &mP = '*') const;
	QStringList VariableTypes(const mProp &mP = '*') const;
	QList<QStringList> DefaultValueLists(const mProp &mP = '*') const;
	QStringList Delegates(const mProp &mP = '*') const;
	QStringList Categories(const mProp &mP = '*') const;
	QStringList inputMethods(const mProp &mP = '*') const;

	mPropList filter(const mProp &mP) const;
	mPropList filter_abv(const mProp & mP) const;
	QStringList extract_props_for_type(QString s); // lists all the properties for a specific type
	QString get_proper_property(QString s, QString propname); // get the proper name of a property
	QStringList extract_units_for_prop(QString type, QString property);
	mPropList filter(const QList<mProp> mP) const;
	int setProp(const QString _PropertyName, const XString _Value = "", const mProp _Filter = '*');
};
