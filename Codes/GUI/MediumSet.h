#pragma once

#include "Medium.h"

//GUI
#include "GWidget.h"
class GraphWidget;
class runtimeWindow;

struct  Solver_parameters
{
	double w;
	double tol;
	double dt;
	int solution_method;
	int nr_iteration_treshold_max;
	int nr_iteration_treshold_min;
	double dt_change_rate;
	double dt_change_failure;
	int nr_failure_criteria;
	int max_J_interval;
	double wiggle_tolerance;
	bool mass_balance_check;
	bool forward;
	bool colloid_transport;
	bool constituent_transport;
	int epoch_limit;
	double avg_dt_limit;
	int restore_interval;
	bool pos_def_limit;
	bool negative_concentration_allowed;
	bool steady_state_hydro = false;
};

struct  file_info
{
	string pathname;
	string outputpathname;
	string detoutfilename_obs;
	int write_interval;
	string realizeparamfilename;
	bool write_details;
	bool uniformoutput;
	string log_file_name;
	
	
};

struct _formulas
{
	vector<string> formulas;
	vector<string> formulasH;
	vector<vector<string>> formulasQ;
	vector<vector<string>> formulasQ2;
	vector<vector<string>> formulasA;
	vector<vector<bool>> const_area;
	vector<bool> air_phase;
	vector<vector<bool>> vaporTransport;
	vector<vector<bool>> settling;
};

struct _set_features
{
	bool reactions = false;
	bool solids = false; 
	bool environmental_vars = false;
	bool observed = false;
	bool parameters = false;
	bool formulas = false;
	bool constituents = false;
	bool evaporation = false;
	bool external_flux = false;
	bool buildup = false;
};

class CMediumSet
{
public:
	CMediumSet();
	CMediumSet(string filename);
	~CMediumSet();
	CMediumSet(const CMediumSet &M);
	CMediumSet& CMediumSet::operator=(const CMediumSet &BB);
	vector<CMedium> Medium;
	Solver_parameters SP; //current time weight for CN solution
	vector<CSolid_Phase> Solid_phase;
	vector<range> parameters;
	vector<measured_chrc> measured_quan;
	CBTCSet measured_data; //measured data
	vector<double> std;
	file_info FI;
	string PE_info_filename;
	_formulas formulas;
	void set_formulas();
	CRxnNetwork RXN;
	vector <CBTCSet*> ANS_hyd;
	CBTCSet ANS_obs;
	CBTCSet ANS_obs_noise;
	vector <CBTCSet*> ANS_colloids;
	vector <CBTCSet*> ANS_constituents;
	void solve();
	void CMediumSet::set_default();
	void CMediumSet::f_get_environmental_params(CLIDconfig &lidconfig);
	void CMediumSet::f_get_params(CLIDconfig &lid_config);
	void CMediumSet::f_get_observed(CLIDconfig &lid_config);
	void CMediumSet::f_get_particle_types(CLIDconfig &lid_config);
	void CMediumSet::f_get_constituents(CLIDconfig &lid_config);
	void CMediumSet::f_get_reactions(CLIDconfig &lid_config);
	void CMediumSet::f_get_buildup(CLIDconfig &lid_config);
	void CMediumSet::f_get_external_flux(CLIDconfig &lid_config);
	void CMediumSet::f_get_evaporation_model(CLIDconfig &lid_config);
	_set_features set_features;
	void CMediumSet::writetolog(string S);
	int CMediumSet::lookup_parameters(string S);
//	int CMediumSet::lookup_external_flux(string S);
//	int CMediumSet::lookup_particle_type(string S);
//	int CMediumSet::lookup_buildup(string S);
//	int CMediumSet::lookup_evaporation(string S);
	int CMediumSet::lookup_observation(string S) const;

	vector<CBuildup> buildup;
	vector<CEnvExchange> externalflux;
	vector<CEvaporation> evaporation_model;
	void CMediumSet::set_param(int param_no, double _value);
	int CMediumSet::lookup_medium(string S);
	double CMediumSet::calc_log_likelihood(); //calculate sum log likelihood for time series data ts
	double CMediumSet::calc_log_likelihood(int i); //calculate sum log likelihood for observed quantity i
	bool failed;
	void CMediumSet::finalize_set_param();
	int CMediumSet::epoch_count();

//GUI
	GraphWidget *gw = 0;
	CMediumSet(GraphWidget* gw, runtimeWindow* rtw);
	void g_get_environmental_params();
	void g_get_params();
	void g_get_observed();
	void g_get_particle_types();
	void g_get_constituents();
	void g_get_reactions();
	void g_get_buildup();
	void g_get_external_flux();
	void g_get_evapotranspiration();
	void solve(runtimeWindow *rtw);


};

