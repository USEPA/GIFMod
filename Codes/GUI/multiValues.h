#pragma once
#include "qvariant.h"
#include "Vector.h"
#include "qdebug.h"


class multiValues
{
public:
	multiValues(){};
	~multiValues(){};
//	multiValues(const multiValues &m){
//		values = m.values;
//	}
	multiValues(vector<QVariant> values){
		this->values = values;
	}
	vector<QVariant> values;
	int size(){
		return values.size();
	}
	int count(){
		return size();
	}
	QVariant& operator[](int index){
	//	qDebug() << "MV operator []" << index;
		return values[index];
	}
	void push_back(QVariant value){
		values.push_back(value);
	}
	void append(QVariant value){
		push_back(value);
	}
	QVariant value(){
		//qDebug() << "MV value()";
		if (sameValues())
			return values[0];
		return QVariant();
	}
	bool sameValues(){
		//qDebug() << "MV sameValues()";
		if (!size())
			return false;
		if (size() == 1)
			return true;
		QVariant value = values[0];
		for (int i = 1; i < size(); i++)
			if (value != values[i])
				return false;
		return true;
	}
	bool differentValues(){
		if (!size())
			return false;
		else return !sameValues();
	}
};