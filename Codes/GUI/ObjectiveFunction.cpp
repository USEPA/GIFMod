#include "ObjectiveFunction.h"



CObjectiveFunction::CObjectiveFunction()
{
}


CObjectiveFunction::~CObjectiveFunction()
{
}

CObjectiveFunction::CObjectiveFunction(const CObjectiveFunction &M)
{
	name = M.name;
	expression = M.expression;
	location = M.location;
	loc_type = M.loc_type;
}

CObjectiveFunction& CObjectiveFunction::operator=(const CObjectiveFunction &M)
{
	
	name = M.name;
	expression = M.expression;
	location = M.location;
	loc_type = M.loc_type;
	return *this;
}
