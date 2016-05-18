#pragma once

#include <vector>
#include "variant.h"

class navigationList
{
public:
	navigationList(){};
	navigationList(vector<variant> list, int currentIndex = 0){
		this->list = list;
		descList.resize(list.size());
		index = currentIndex;
	}
	navigationList(vector<variant> list, vector<variant> desc, int currentIndex = 0){
		this->list = list;
		this->descList = desc;
		index = currentIndex;
	}

	~navigationList(){};

	variant operator()(int index){
		if (index < 0 || index > list.size() - 1)
			return variant();
		else
			return list[index];
	}
	variant operator[](int index){
		return operator()(index);
	}
	variant desc(int index){
		if (index < 0 || index > descList.size() - 1)
			return variant();
		else
			return descList[index];
	}
	int currentIndex(){
		return index;
	}
	int size(){
		return list.size();
	}

	variant first(){
		index = 0;
		return operator()(index);
	}
	variant last(){
		index = list.size() - 1;
		return operator()(index);
	}
	void back(int n = 1){
		index-=n;
		if (index < 0)
			index = 0;
	}
	void forward(int n = 1){
		index += n;
		if (index > list.size() - 1)
			index = list.size() - 1;
	}
	int append(variant item, variant itemDesc = ""){
		while (list.size() > index + 1)
		{
			list.pop_back();
			descList.pop_back();
		}
		list.push_back(item);
		descList.push_back(itemDesc);
		index = list.size()-1;
		return index;
	}
	bool backActive(){
		return (index > 0) ? true : false;
	}
	bool forewardActive(){
		return ((index >= 0) && (index < (list.size() - 1))) ? true : false;
	}
	vector<variant> list;
	vector<variant> descList;
	int index = 0;
};

