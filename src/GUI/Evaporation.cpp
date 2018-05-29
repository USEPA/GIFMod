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
	single_crop_coefficient_filename = B.single_crop_coefficient_filename;
	single_crop_coefficient = B.single_crop_coefficient;
	evaporation_TS = B.evaporation_TS;
	uptake = B.uptake;

}

CEvaporation::CEvaporation(string _model)
{
	model = _model;
	if (model == "aerodynamic")
	{
		//string Ea = "(0.0000000009424*f[20]*f[22]*(1-f[21])*f[6000])";
		string Ea =   "(0.00000000001522790*f[20]*f[22]*(1-f[21])*f[6000])";
		expression = CStringOP("(" + Ea + ")*86400*f[2]");
		parameters.resize(5);
		parameters[3] = 66;
	}
	if (model == "time_series")
	{	}
	if (model == "transpiration_moisture_based")
	{
		expression = CStringOP("f[6000]*f[20]*(1-f[21])*611*_exp(17.27*f[19]/(237.3+f[19]))*f[2]*_max(_min(((f[10]-f[6001])/(f[6002]-f[6001])):1):0)");
		parameters.resize(4);

	}
	if (model == "transpiration_suction_based")
	{
		expression = CStringOP("f[6000]*f[20]*(1-f[21])*611*_exp(17.27*f[19]/(237.3+f[19]))*f[2]*_max(_min(((f[1]-f[6001])/(f[6002]-f[6001])):1):0)");
		parameters.resize(4);
	}

	if (model == "priestley-taylor")
	{
		string Er = "(f[18]/(1000*f[23]))";
		string Delta = "(4098*f[22]/((237.3+f[19])^2))";
		string light_term = "(f[6004]*" + Delta + "/(" + Delta + " + f[6003])*" + Er + ")";
		expression = CStringOP("1.3*(" + light_term + ")*86400*f[2]");
		parameters.resize(5);
		parameters[3] = 66;
		parameters.resize(4);
		parameters[3] = 66;
	}

	if (model == "penman")
	{
		string Er = "(f[18]/(1000*f[23]))";
		string Ea = "(0.00000000001522790*f[20]*f[22]*(1-f[21])*f[6000])";
		string Delta = "(4098*f[22]/((237.3+f[19])^2))";
		string light_term = "(f[6004]*" + Delta + "/(" + Delta + " + f[6003])*" + Er + ")";
		string wind_term = "(f[6003]/(f[6003]+" + Delta + ")*" + Ea +")";
		expression = CStringOP("("+light_term + "+" + wind_term+")*86400*f[2]");
		//expression = CStringOP("(f[6004]*f[18]/(1+(0.0000004*f[6003]*((237.3+f[19])^2)*_exp(17.27*f[19]/(237.3+f[19]))))/(2501000-(2370*f[19]))/1000*86.400)+((f[6000]*611*f[6003])/((f[6003]*_exp(-17.27*f[19]/(237.3 + f[19])))+(2503878/((237.3+f[19])^2)))*f[20]*(1-f[21])*86.400)");
		parameters.resize(5);
		parameters[3] = 66;
	}

	if (model == "fao-56")
	{
		string Er = "(f[18]/(1000*f[23]))";
		string Ea = "(0.00000000001522790*f[20]*f[22]*(1-f[21])*f[6000])";
		string Delta = "(4098*f[22]/((237.3+f[19])^2))";
		string light_term = "(f[6004]*" + Delta + "/(" + Delta + "+ f[6003])*" + Er + ")";
		string wind_term = "(f[6003]/(f[6003]+" + Delta + ")*" + Ea + ")";
		string stress_term = "(_max(_min(((f[10] - f[6001]) / (f[6002] - f[6001])) : 1) :0))";

		expression = CStringOP("(" + light_term +"+"+ wind_term + ")*" + stress_term + "*f[6501]*86400*f[2]");

		//expression = CStringOP("((f[6004]*f[18]/(1+(0.0000004*f[6003]*((237.3+f[19])^2)*_exp(17.27*f[19]/(237.3+f[19]))))/(2501000-(2370*f[19]))/1000*86.400)+((f[6000]*611*f[6003])/((f[6003]*_exp(-17.27*f[19]/(237.3 + f[19])))+(2503878/((237.3+f[19])^2)))*f[20]*(1-f[21]))*86.400))*_max(_min(((f[10]-f[6001])/(f[6002]-f[6001])):1):0)*f[6501]");
		parameters.resize(5);
		parameters[3] = 66;
	}

	if (model == "priestley-taylor (plant)")
	{
		string Er = "(f[18]/(1000*f[23]))";
		string Delta = "(4098*f[22]/((237.3+f[19])^2))";
		string light_term = "(f[6004]*" + Delta + "/(" + Delta + " + f[6003])*" + Er + ")";
		expression = CStringOP("(1.3*(" + light_term + ")*86400*f[2])*f[24]/(f[24]+f[6006])");
		parameters.resize(5);
		parameters[3] = 66;
		parameters.resize(10);
		parameters[3] = 66;
	}

	if (model == "penman (plant)")
	{
		string Er = "(f[18]/(1000*f[23]))";
		string Ea = "(0.00000000001522790*f[20]*f[22]*(1-f[21])*f[6000])";
		string Delta = "(4098*f[22]/((237.3+f[19])^2))";
		string light_term = "(f[6004]*" + Delta + "/(" + Delta + "+ f[6003])*" + Er + ")";
		string wind_term = "(f[6003]/(f[6003]+" + Delta + ")*" + Ea + ")";
		expression = CStringOP("(" + light_term + "+" + wind_term + ")*86400*f[2]*f[24]/(f[24]+f[6006])");
		//expression = CStringOP("(f[6004]*f[18]/(1+(0.0000004*f[6003]*((237.3+f[19])^2)*_exp(17.27*f[19]/(237.3+f[19]))))/(2501000-(2370*f[19]))/1000*86.400)+((f[6000]*611*f[6003])/((f[6003]*_exp(-17.27*f[19]/(237.3 + f[19])))+(2503878/((237.3+f[19])^2)))*f[20]*(1-f[21])*86.400)");
		parameters.resize(10);
		parameters[3] = 66;
	}

	if (model == "aerodynamic (plant)")
	{
		string Ea = "(0.00000000001522790*f[20]*f[22]*(1-f[21])*f[6000])";
		string wind_term = "f[6003]*" + Ea + ")";
		expression = CStringOP("(" + wind_term + ")*86400*f[2]*f[24]/(f[24]+f[6006])");
		parameters.resize(10);
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
	single_crop_coefficient_filename = B.single_crop_coefficient_filename;
	single_crop_coefficient = B.single_crop_coefficient;
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
	for (unsigned int j = 0; j < Block->evaporation_m.size(); j++)
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
	if (S == "half_evaporation_lai") parameters[6] = val;

}

void CEvaporation::set_val(int i, double val)
{
	parameters[i] = val;

}


