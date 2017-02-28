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
	CObjectiveFunction::CObjectiveFunction(const CObjectiveFunction &M);
	CObjectiveFunction& CObjectiveFunction::operator=(const CObjectiveFunction &M);
};

