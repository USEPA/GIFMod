#include "Evaporation.h"


#include "MBBlock.h"


CEvaporation::CEvaporation(void)
{
	
}


CEvaporation::~CEvaporation(void)
{
}


CEvaporation::CEvaporation(const CEvaporation &B)
{
	expression = B.expression;
	model = B.model;
	name = B.name;
	parameters = B.parameters;
	evaporation_filename = B.evaporation_filename;
	evaporation_TS = B.evaporation_TS;
	uptake = B.uptake;

}

CEvaporation::CEvaporation(string _model)
{
	model = _model;
	if (model == "aerodynamic")
	{
		expression = CStringOP("f[6000]*f[20]*(1-f[21])*611*_exp(17.27*f[19]/(237.3+f[19]))*f[2]");
		parameters.resize(2);
	}
	if (model == "time_series")
	{	}
	if (model == "transpiration_moisture_based")
	{
		expression = CStringOP("f[6000]*f[20]*(1-f[21])*611*_exp(17.27*f[19]/(237.3+f[19]))*f[2]*_min(_max((f[10]-f[6001])/(f[6002]-f[6001]):1):0)/f[3]");
		parameters.resize(4);

	}
	if (model == "transpiration_suction_based")
	{
		expression = CStringOP("f[6000]*f[20]*(1-f[21])*611*_exp(17.27*f[19]/(237.3+f[19]))*f[2]*_min(_max((f[1]-f[6001])/(f[6002]-f[6001]):1):0)/f[3]");
		parameters.resize(4);
	}

	if (model == "priestley-taylor")
	{
		expression = CStringOP("1.3*f[6000]*f[18]/(1+(0.0000004*f[6003]*((237.3+f[19])^2)*_exp(17.27*f[19]/(237.3+f[19]))))/(2501000-(2370*f[19]))/1000");
		parameters.resize(4);
		parameters[3] = 66;
	}

	if (model == "combined")
	{
		expression = CStringOP("(f[6004]*f[18]/(1+(0.0000004*f[6003]*((237.3+f[19])^2)*_exp(17.27*f[19]/(237.3+f[19]))))/(2501000-(2370*f[19]))/1000)+((f[6000]*611*f[6003])/((f[6003]*_exp(-17.27*f[19]/(237.3 + f[19])))+(2503878/((237.3+f[19])^2)))*f[20]*(1-f[21]))");
		parameters.resize(5);
		parameters[3] = 66;
	}


}
CEvaporation& CEvaporation::operator=(const CEvaporation &B)
{
	expression = B.expression;
	model = B.model;
	name = B.name;
	parameters = B.parameters;
	evaporation_filename = B.evaporation_filename;
	evaporation_TS = B.evaporation_TS;
	uptake = B.uptake;
	return *this;
}

double CEvaporation::calculate(CMBBlock *Block)
{
	vector<int> ii;
	return Block->calc(expression, ii);
}

double CEvaporation::calculate_star(CMBBlock *Block)
{
	vector<int> ii;
	for (int j = 0; j < Block->evaporation_m.size(); j++)
		if (name == Block->evaporation_m[j]->name) ii.push_back(j);
	return Block->calc_star(expression, ii);
}


void CEvaporation::set_val(string S, double val)
{
	if (S == "coefficient") parameters[0] = val;
	if (S == "coefficient_wind") parameters[0] = val;
	if (S == "wilting_point_water_content") parameters[1] = val;
	if (S == "wilting_pwc") parameters[1] = val;
	if (S == "wilting_point_matric_potential") parameters[1] = val;
	if (S == "field_capacity_moisture") parameters[2] = val;
	if (S == "field_capacity_matric_potential") parameters[2] = val;
	if (S == "psychrometric_constant") parameters[3] = val;
	if (S == "coefficient_light") parameters[4] = val;
	
}

void CEvaporation::set_val(int i, double val)
{
	parameters[i] = val;

}


