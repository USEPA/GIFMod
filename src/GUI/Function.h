#pragma once

#include "BTC.h"
#include "StringOP.h"

class CFunction
{
public:
	CStringOP Expression;
	int n_steps;
	double _min, _max;
	double evaluate(const double &s);
	CBTC X;
	CFunction(void);
	CFunction(const CFunction &F);
	int var_id;
    CFunction operator = (const CFunction &F);
	~CFunction(void);
};

