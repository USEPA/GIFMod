#pragma once

//#include "XString.h"
//#include "node.h"
#include "mprop.h"
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
    mListReadStatus mPropList::getconfigfromfile(QString filename);
//	mListReadStatus mPropList::getconfigfromfile(QString filename, GraphWidget *GW);
	QList<mProp> List;
	QStringList mPropList::Models(const mProp &mP = '*') const;
	QStringList mPropList::GuiObjects(const mProp &mP = '*') const;
	QStringList mPropList::ObjectTypes(const mProp &mP = '*') const;
	QStringList mPropList::SubTypes(const mProp &mP = '*') const;
	QStringList mPropList::Descriptions(const mProp &mP = '*') const;
	QStringList mPropList::VariableNames(const mProp &mP = '*') const;
	QStringList mPropList::VariableUnits(const mProp &mP = '*') const;
	QStringList mPropList::VariableTypes(const mProp &mP = '*') const;
	QList<QStringList> mPropList::DefaultValueLists(const mProp &mP = '*') const;
	QStringList mPropList::Delegates(const mProp &mP = '*') const;
	QStringList mPropList::Categories(const mProp &mP = '*') const;
	QStringList mPropList::inputMethods(const mProp &mP = '*') const;

	mPropList mPropList::filter(const mProp &mP) const;

	int setProp(const QString _PropertyName, const XString _Value = "", const mProp _Filter = '*');
};
