#include "stdafx.h"
#include "Function.h"


CFunction::CFunction(void)
{
	n_steps = 100;
}


CFunction::~CFunction(void)
{
	
}

 CFunction::CFunction(const CFunction &F)
{
	Expression = F.Expression;
	n_steps = F.n_steps;
	X = F.X;
	_max = F._max;
	_min = F._min;
	var_id = F.var_id;
}

CFunction CFunction::operator = (const CFunction &F)
{
	Expression = F.Expression;
	n_steps = F.n_steps;
	X = F.X;
	_max = F._max;
	_min = F._min;
	var_id = F.var_id;
	
	return *this;

}

double CFunction::evaluate(double x)
{
	return X.interpol(x);
}