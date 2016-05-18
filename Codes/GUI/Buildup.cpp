#include "Buildup.h"
#include <string>

CBuildup::CBuildup(void)
{
	model = "linear";
}

CBuildup::CBuildup(string m)
{
	model = m;
	if (model=="linear") parameters.resize(1);
	if (model=="exponential") parameters.resize(2);
}

CBuildup::~CBuildup(void)
{
}

CBuildup::CBuildup(const CBuildup &B)
{
	model = B.model;
	parameters = B.parameters;
	phase = B.phase;
	constituent = B.constituent;
	solid = B.solid;
	name = B.name;

}

CBuildup& CBuildup::operator=(const CBuildup &B)
{
	model = B.model;
	parameters = B.parameters;
	phase = B.phase;
	constituent = B.constituent;
	solid = B.solid;
	name = B.name;
	return *this;
}

double CBuildup::buildup(double C, CMBBlock *B)
{
	if (model=="linear") return parameters[0];
	if (model=="exponential") return parameters[0]-parameters[0]*C/parameters[1];
}

void CBuildup::set_val(string S, double val)
{
	if (S=="acc_rate") parameters[0] = val;
	if (S=="saturation") parameters[1] = val;
}

void CBuildup::set_val(int i, double val)
{
	parameters[i] = val;
}