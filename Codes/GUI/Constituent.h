#pragma once
#include <string>
#include <vector>
#include "btc.h"
#include "StringOP.h"
#include "Solid_Phase.h"

using namespace std;



class CConstituent
{
public:
	CConstituent(void);
	CConstituent(string _settling_model);
	~CConstituent(void);
	CConstituent& CConstituent::operator==(const CConstituent& CC);
	CConstituent::CConstituent(const CConstituent &C);
	string name;
	int solid;
	double conc;
	double conc_0;
	bool influx;
	char inlflux_model;
	vector<double> rate_constant_influx;
	vector<double> ini_conc;
	vector<bool> set_ini_conc;
	vector<double> equilibrium_conc;
	bool equil_to_be_estimated, rate_to_be_estimated;
	vector<double> value_fixed;
	vector<double> value_fixed_ts;
	bool fixed_file;
	bool fixed_val;
	vector<bool> fixed_val_ts;
	vector<CBTC> fixedts; 
	vector<double> fixedts_avg;
	vector<string> rate_exchange_ptr;
	vector<double> rate_exchange;
	double diffusion;
	vector<string> capacity_ptr;
	vector<double> capacity;
	double CConstituent::get_capacity(string &i) const;
	double CConstituent::get_exchange_rate(string &i) const;
	void CConstituent::set_val(int i, double val);
	void CConstituent::set_val(string S, double val);
	CStringOP exchange_rate_scale_factor;
	vector<double> exchange_params;
	double vs=0; 
	CStringOP vs_coefficient; 
	string settling_model;
	vector<double> settling_parameters;
	double CConstituent::get_val(int i);
	bool mobile = true; 
};

