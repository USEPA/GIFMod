#pragma once
#include "qvariant.h"
#include "Vector.h"
#include "qdebug.h"

template <class T = QVariant>
class multiValues
{
public:
	multiValues(){};
	~multiValues(){};
//	multiValues(const multiValues &m){
//		values = m.values;
//	}
	multiValues(vector<T> values){
		this->values = values;
	}
	vector<T> values;
	int size(){
		return values.size();
	}
	int count(){
		return size();
	}
	T& operator[](int index){
	//	//qDebug() << "MV operator []" << index;
		return values[index];
	}
	void push_back(T value){
		values.push_back(value);
	}
	void append(T value){
		push_back(value);
	}
	T value(){
		////qDebug() << "MV value()";
		if (sameValues())
			return values[0];
		return T();
	}
	bool sameValues(){
		////qDebug() << "MV sameValues()";
		if (!size())
			return false;
		if (size() == 1)
			return true;
		T value = values[0];
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