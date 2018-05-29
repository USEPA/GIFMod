#pragma once
#ifdef GIFMOD
#include "Medium.h"
#include "Sensor.h"
#include "Controller.h"
//GUI
#ifdef QT_version
#include "GWidget.h"
class GraphWidget;
class runtimeWindow;
#include "qmap.h"
#endif // QT_version
#include "StringOP.h"
#include "ObjectiveFunction.h"

struct  Solver_parameters
{
	double w;
	double tol;
	double dt;
	string solution_method;
	bool sorption;
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
	bool negative_concentration_allowed; //if set equal to true the calculated concentrations will be prevented from becomming negative
	bool steady_state_hydro = false; // the hydro will be solved assuming a steady-state condition
	bool check_oscillation=true; //whether oscillation in the solution is limited by the solver
	double maximum_run_time=30*86400; //This is the maximum time that a single simulation should take. The solver gives up when this time limit is reached and "failed" will be returned.
	double minimum_acceptable_negative_conc = 1e-13; //this parameter determines the absolute value of a negative concentration to be accepted. If a negative concentration with absolute value smaller than this criteria is calculated the program will replace it with a zero concentration.
	double max_dt; // maximum allowable time step
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

struct _control
{
	vector<CObjectiveFunction> ObjectiveFunctions;
	vector<CSensor> Sensors;
	vector<CController> Controllers;
};

class CMediumSet
{
public:
	CMediumSet();
	CMediumSet(string filename);
	~CMediumSet();
	CMediumSet(const CMediumSet &M);
    CMediumSet& operator=(const CMediumSet &BB);
	void SetDefaultSolverParameters();
	vector<CMedium> Medium;
	Solver_parameters SP; //current time weight for CN solution
	vector<CSolid_Phase> Solid_phase;
	vector<param_range> parameters;
	vector<measured_chrc> measured_quan;
	CBTCSet measured_data; //measured data
	vector<double> std;
	file_info FI;
	string PE_info_filename;
	_formulas formulas;
	void set_formulas();
    bool get_formulas_from_file(string filename);
	CRxnNetwork RXN;
	vector <CBTCSet*> ANS_hyd;
	vector <CBTCSet*> ANS_control;
	CBTCSet ANS_obs;
	CBTCSet ANS_obs_noise;
	vector <CBTCSet*> ANS_colloids;
	vector <CBTCSet*> ANS_constituents;
	void solve();
    void set_default();
    void f_get_environmental_params(CLIDconfig &lidconfig);
    void f_get_params(CLIDconfig &lid_config);
    void f_get_observed(CLIDconfig &lid_config);
    void f_get_particle_types(CLIDconfig &lid_config);
    void f_get_constituents(CLIDconfig &lid_config);
    void f_get_reactions(CLIDconfig &lid_config);
    void f_get_buildup(CLIDconfig &lid_config);
    void f_get_external_flux(CLIDconfig &lid_config);
    void f_get_evaporation_model(CLIDconfig &lid_config);
    void f_get_sensors(CLIDconfig &lid_config);
    void f_get_controller(CLIDconfig &lid_config);
	_set_features set_features;
    void writetolog(string S);
    int lookup_parameters(string S);
    int lookup_controllers(string S);
	//	int CMediumSet::lookup_external_flux(string S);
//	int CMediumSet::lookup_particle_type(string S);
//	int CMediumSet::lookup_buildup(string S);
//	int CMediumSet::lookup_evaporation(string S);
    int lookup_observation(string S) const;

	vector<CBuildup> buildup;
	vector<CEnvExchange> externalflux;
	vector<CEvaporation> evaporation_model;
    void set_param(int param_no, double _value);
    void set_control_param(int controller_no, int experiment_id); //setting control parameters
    int lookup_medium(string S);
    double calc_log_likelihood(); //calculate sum log likelihood for time series data ts
    double calc_log_likelihood(int i); //calculate sum log likelihood for observed quantity i
	double calc_MSE(int i); // calculates the mean squared error
	bool failed;
    void finalize_set_param();
    int epoch_count();
	void clear();
	int get_block_type(string s);
	string ID;
//GUI
	GraphWidget *gw = 0;
	void g_get_controllers();
	void load(GraphWidget * gw, runtimeWindow * rtw);
	CMediumSet(GraphWidget* gw, runtimeWindow* rtw);
	void g_get_environmental_params();
	void g_get_params();
	void g_get_observed();
	void g_get_sensors();
	void g_get_objective_functions();
	void g_get_particle_types();
	void g_get_constituents();
	void g_get_reactions();
	void g_get_buildup();
	void g_get_external_flux();
	void g_get_evapotranspiration();
	void solve(runtimeWindow *rtw);

#ifdef QT_version
	QMap<string, int> blockIndex;
	QMap<string, int> connectorIndex;
#endif // QT_version
	_control Control;
	double elapsed_time(); // gets the total elapsed computational time from the start of the simulation of the mediumset.
	double progress_percentage(); // shows the overall progress percentage of the simulation
	vector<double> MSE_obs;
    bool showmessages;
    void show_message(string s);



};

#endif
