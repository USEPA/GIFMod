#include "Constituent.h"


CConstituent::CConstituent(void)
{
	exchange_rate_scale_factor = CStringOP("1");
	vs_coefficient = CStringOP("1");
	settling_model = "constant-velocity";

}

CConstituent::CConstituent(string _settling_model)
{
	exchange_rate_scale_factor = CStringOP("1");
	vs_coefficient = CStringOP("1");
	settling_model = _settling_model;
	if (settling_model == "constant_velocity")
	{

	}
	if (settling_model == "double_exponential")
	{
		settling_parameters.resize(3);
		vs_coefficient = CStringOP("_max(_exp((-f[5050])*(cg[]-f[5052]))-_exp((-f[5051])*(cg[]-f[5052])):0)");
	}

}


CConstituent::~CConstituent(void)
{
}

CConstituent& CConstituent::operator==(const CConstituent& CC)
{
	name = CC.name;
	solid = CC.solid;
	conc = CC.conc;
	conc_0 = CC.conc_0;
	influx = CC.influx;
	inlflux_model = CC.inlflux_model;
	rate_constant_influx = CC.rate_constant_influx;
	ini_conc = CC.ini_conc;
	set_ini_conc = CC.set_ini_conc;
	equilibrium_conc = CC.equilibrium_conc;
	value_fixed = CC.value_fixed;
	value_fixed_ts = CC.value_fixed_ts;
	fixed_file = CC.fixed_file;
	fixed_val = CC.fixed_val;
	fixed_val_ts = CC.fixed_val_ts;
	fixedts = CC.fixedts;
	fixedts_avg = CC.fixedts_avg;
	rate_exchange = CC.rate_exchange;
	rate_exchange_ptr = CC.rate_exchange_ptr;
	diffusion = CC.diffusion;
	capacity = CC.capacity;
	capacity_ptr = CC.capacity_ptr;
	exchange_rate_scale_factor = CC.exchange_rate_scale_factor;
	exchange_params = CC.exchange_params;
	vs = CC.vs;
	vs_coefficient = CC.vs_coefficient;
	settling_parameters = CC.settling_parameters;
	settling_model = CC.settling_model;
	mobile = CC.mobile;
	return *this;

}

CConstituent::CConstituent(const CConstituent &CC)
{
	name = CC.name;
	solid = CC.solid;
	conc = CC.conc;
	conc_0 = CC.conc_0;
	influx = CC.influx;
	inlflux_model = CC.inlflux_model;
	rate_constant_influx = CC.rate_constant_influx;
	ini_conc = CC.ini_conc;
	set_ini_conc = CC.set_ini_conc;
	equilibrium_conc = CC.equilibrium_conc;
	value_fixed = CC.value_fixed;
	value_fixed_ts = CC.value_fixed_ts;
	fixed_file = CC.fixed_file;
	fixed_val = CC.fixed_val;
	fixed_val_ts = CC.fixed_val_ts;
	fixedts = CC.fixedts;
	fixedts_avg = CC.fixedts_avg;
	rate_exchange = CC.rate_exchange;
	rate_exchange_ptr = CC.rate_exchange_ptr;
	diffusion = CC.diffusion;
	capacity = CC.capacity;
	capacity_ptr = CC.capacity_ptr;
	exchange_rate_scale_factor = CC.exchange_rate_scale_factor;
	exchange_params = CC.exchange_params;
	vs = CC.vs;
	vs_coefficient = CC.vs_coefficient;
	settling_parameters = CC.settling_parameters;
	settling_model = CC.settling_model;
	mobile = CC.mobile;
}

double CConstituent::get_capacity(string &i) const
{
	return capacity[lookup(capacity_ptr,i)];

}

double CConstituent::get_exchange_rate(string &i) const
{
	return rate_exchange[lookup(capacity_ptr,i)];
}

void CConstituent::set_val(int i, double val)
{
	if (i==1) diffusion = val;
	if (i == 2) vs = val;
	if ((i>10) && (i<20)) capacity[i-11] = val;
	if ((i>20) && (i<30)) rate_exchange[i-21] = val;
	if ((i>30) && (i<40)) settling_parameters[i - 31] = val;
}


void CConstituent::set_val(string S, double val)
{
	vector<char> del;
	del.push_back('[');
	del.push_back(']');
	del.push_back(':');
	vector<string> s = split(S, del);
	if (s.size() == 1)
	{
		if (S == "diffusion") diffusion = val;
		if (S == "vs") vs = val;
		if (S == "r_hin") settling_parameters[r_hin] = val;
		if (S == "r_floc") settling_parameters[r_floc] = val;
		if (S == "x_min") settling_parameters[x_min] = val;

	}
	if (s.size() == 2)
	{
		if (s[0] == "kd")
		{
			int i = lookup(capacity_ptr, s[1]);
			capacity[i] = val;
		}
		if (s[0] == "rate")
		{
			int i = lookup(rate_exchange_ptr, s[1]);
			rate_exchange[i] = val;
		}

	}

}

double CConstituent::get_val(int i)
{
	if (i >= 5000 && i < 5050)
		return exchange_params[i - 5000];
	else if (i >= 5050)
		return settling_parameters[i - 5050];
    return 0;
}
