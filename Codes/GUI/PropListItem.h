#pragma once
#include "qmap.h"



class XString;
class QString;
#include "qvariant.h"

template<class T>
class PropListItem
{
public:
	PropListItem() {};
	//PropListItem(T *parent) { 
		//this->parent = parent; 
	//};
	PropListItem(const PropListItem &P){
		//parent = P.parent; 
		list = P.list; };
//	PropList operator =(const PropList &P){ 
	//	parent = P.parent; list = P.list; return this; };
	~PropListItem(){};

	//T *parent;

	QMap<QString, XString > list;

	QMap<QString, QString> compact() const{
		QMap<QString, QString> r;
		for each (QString key in list.keys())
		{
			r[key.toLower()] = list[key].compact();
		}
		return r;
	}
	static QMap<QString, XString> unCompact(const QMap<QString, QVariant> &c) {
		QMap<QString, XString> r;
		for each (QString key in c.keys())
		{
			QString newKey;
			int i = key.split(' ').size();
			if (i == 1)
				newKey = key;
			else
			{
				QStringList list;
				QStringList klist = key.split(' ');
				list << klist[0];
				for (int i = 1; i < klist.size(); i++)
					list << klist[i].toLower();
				newKey = list.join(' ');
			}
			r[newKey.toLower()] = XString::unCompact(c[key].toString());
		}
		return r;
	}
	/*static QMap<QString, XString> unCompact10(const QMap<QString, QVariant> &c) {
		QMap<QString, XString> r;
		for each (QString key in c.keys())
		{
			XString a = XString::unCompactOld(c[key].toString());
			r[key] = a;
		}
		return r;
	}*/
	XString getProp(const QString& propName, T* parent) const;
	bool setProp(const QString& propName, const XString& Value, T* parent); 

};
