#pragma once
#include "StringOP.h"

class CObjectiveFunction
{
public:
	string name; 
	CObjectiveFunction();
	~CObjectiveFunction();
	CStringOP expression;
	int loc_type;
	string location; 
    CObjectiveFunction(const CObjectiveFunction &M);
    CObjectiveFunction& operator=(const CObjectiveFunction &M);
};

