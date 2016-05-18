#include "EnvExchange.h"
#include "MBBlock.h"


CEnvExchange::CEnvExchange(void)
{
	phase = "aqueous";
}


CEnvExchange::~CEnvExchange(void)
{
}


CEnvExchange::CEnvExchange(const CEnvExchange &B)
{
	constituent = B.constituent;
	expression = B.expression;
	model = B.model;
	name = B.name;
	parameters = B.parameters;
	phase = B.phase;
	solid = B.solid;
}

CEnvExchange::CEnvExchange(string _model)
{
	phase = "aqueous"; 
	model = _model;
	if (model == "constant_rate")
	{
		expression = CStringOP("f[4001]*(f[4000]-c[])*f[4]");
		parameters.resize(2);
	}
	
	if (model == "constant_influx")
	{
		expression = CStringOP("f[4001]*f[4]");
		parameters.resize(2);
	}
	
	if (model=="soil") 
	{
		expression = CStringOP("f[4001]*(f[51]-f[10])*(f[4000]-c[])*f[3]");
		parameters.resize(2);
	}
	if ((model=="free_surface") || (model=="freesurface"))
	{
		expression = CStringOP("f[4001]*(f[8]^f[4002])*((f[1]-f[5])^f[4003])*(f[4000]-c[])*f[4]");
		parameters.resize(4);
	}


}
CEnvExchange& CEnvExchange::operator=(const CEnvExchange &B)
{
	constituent = B.constituent;
	expression = B.expression;
	model = B.model;
	name = B.name;
	parameters = B.parameters;
	phase = B.phase;
	solid = B.solid;
	return *this;
}

double CEnvExchange::calculate(CMBBlock *Block)
{
	vector<int> ii; 
	ii.push_back(Block->RXN->look_up_constituent_no(constituent));
	ii.push_back(Block->lookup_env_exchange(name));
	return Block->calc(expression,ii);
	
}

double CEnvExchange::calculate_star(CMBBlock *Block)
{
	vector<int> ii;
	ii.push_back(Block->RXN->look_up_constituent_no(constituent));
	ii.push_back(Block->lookup_env_exchange(name));
	return Block->calc_star(expression,ii);
}


void CEnvExchange::set_val(string S, double val)
{
	if (S=="saturation") parameters[0] = val;
	if (S=="coefficient") parameters[1] = val;
	if (S=="velocity_exponent") parameters[2] = val;
	if (S=="depth_exponent") parameters[3] = val;
}

void CEnvExchange::set_val(int i, double val)
{
	parameters[i] = val;

}

