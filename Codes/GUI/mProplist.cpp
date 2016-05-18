#include "mPropList.h"
//#include <iostream>
#include <fstream>
#include "StringOP.h"
#include <QDebug>
//#include <qlist.h>
//#include "qstring.h"
//#include <string>
#include "XString.h"

mPropList::mPropList(void)
{
}
mPropList::mPropList(QString filename)
{
	if (List.size() == 0)
		getconfigfromfile(filename); 
}
mPropList& mPropList::operator= (const mPropList &CC)
{
	List = CC.List;
	return *this;
}
mPropList::mPropList(const mPropList &CC)
{
	List = CC.List;
}

/*mListReadStatus mPropList::getconfigfromfile(QString filename, GraphWidget *GW)
{
	mListReadStatus r = getconfigfromfile(filename);
//	if (r == mListReadStatus::readSuccessfully) GW->mList = this;
	return(r);
}*/
mListReadStatus mPropList::getconfigfromfile(QString filename)
{
	
	if (!List.isEmpty()) return (mListReadStatus::readBefore);
	ifstream file(filename.toStdString());
	if (!file.good()) return (mListReadStatus::fileNotValid);

	while (file.eof() == false)
	{
		string line;
		getline(file, line);

		int a = line.find_first_of('//');
		if (a != 0)
		{
//			QStringList s = QSplit(QString::fromStdString(line), ',');
			XString XS = QString::fromStdString(line);
			QStringList s = XS.toQString().split(',');
			if (s.size() >= 17)
			{
				QString QS;
				QStringList QSL;
				mProp mP;
				for (int i = 0; i < s.size(); i++)
				{
					s[i] = (s[i] == "!") ? "" : s[i].trimmed();
					while (s[i].indexOf(" ;") + s[i].indexOf("; ") != -2)
					{
						s[i].replace(" ;", ";");
						s[i].replace("; ", ";");
					}
				}
				mP.Model = s[0];
				mP.GuiObject = s[1];
				mP.ObjectType = s[2];
				mP.SubType = s[3];
				mP.Description = s[4];
				mP.VariableName = s[5];
				mP.VariableCode = s[6];
				mP.VariableUnit = s[7];
				mP.DefaultUnit = s[8];
				mP.VariableType = s[9];
				mP.setDefaultValues (s[10], mP.VariableUnit, mP.DefaultUnit);
				mP.Delegate = s[11];
				mP.Category = s[12];
				mP.inputMethod = s[13];
				mP.ExperimentDependent = s[14];
//				mP.Value = QSplit(mP.DefaultValues, ':')[0];
//				mP.Value = mP.DefaultValuesList()[0];
//				mP.parent = this;
				mP.DescriptionCode = s[15];
				if (s[16] != "") {
					mP.Condition = s[16].split(';');
					mP.error = s[17].split(';');
					mP.errorDesc = s[18].split(';');
					if ((mP.Condition.size() != mP.error.size()) || (mP.Condition.size() != mP.error.size())) 
						return (mListReadStatus::errorInContents);
				}

				List.append(mP);
			}
		}
	}	
	if (List.size() == 0)
		return (mListReadStatus::errorInContents);
	else
	{
		return (mListReadStatus::readSuccessfully);
	}
}

QStringList mPropList::Models(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].Model;
	}
	 return(uniques(r));
}
QStringList mPropList::GuiObjects(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].GuiObject;
	}
	return(uniques(r));
}
QStringList mPropList::ObjectTypes(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].ObjectType;
	}
	 return(uniques(r));
}
QStringList mPropList::SubTypes(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].SubType;
	}
	 return(uniques(r));
}
QStringList mPropList::Descriptions(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].Description;
	}
	 return(uniques(r));
}
QStringList mPropList::VariableNames(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].VariableName;
	}
	 return(uniques(r));
}
QStringList mPropList::VariableUnits(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].VariableUnit;
	}
	 return(uniques(r));
}
QStringList mPropList::VariableTypes(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].VariableType;
	}
	 return(uniques(r));
}
QList<QStringList> mPropList::DefaultValueLists(const mProp& mP)const 
{
	QList<QStringList> r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].DefaultValuesString().split(';');
	}
	 return(r);
}
QStringList mPropList::Delegates(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].Delegate;
	}
	 return(uniques(r));
}
QStringList mPropList::Categories(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].Category;
	}
	 return(uniques(r));
}
QStringList mPropList::inputMethods(const mProp& mP)const
{
	QStringList r;
	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) r << List[i].Model;
	}
	 return(uniques(r));
}

mPropList mPropList::filter(const mProp &mP) const
{
	static int counter = 0;
	static mProp filter;
	static mPropList r;
	if (filter %= mP) return r;
	filter = mP;
	r = mPropList();
	int n = List.size();
	for (int i = 0; i < n; i++)
	{
		if (List[i] == mP) r.List.append(List[i]);
	}
//	qDebug() << "filter:" << counter++;
	return(r);
}

int mPropList::setProp(const QString _PropertyName, const XString _Value, const mProp _Filter)
{/*
	mProp mP = _Filter;
	mP.VariableName = _PropertyName;
	mP.Value = '*';
	int r = 0;

	for (int i = 0; i < List.size(); i++)
	{
		if (List[i] == mP) {
			(_Value == "Set to Default") ?
				List[i].Value = List[i].DefaultValuesList()[0]
			:List[i].Value = _Value; r++; }
	}
	return(r);
	*/
	return 1;
}