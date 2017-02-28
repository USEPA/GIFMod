#pragma once
#ifdef GIFMOD
#include "Connection.h"
#include "MBBlock.h"
#include <vector>
#include "Vector.h"
#include "Matrix.h"
#include "BTCSet.h"
#include "string.h"
#include "LIDconfig.h"
#include "Precipitation.h"
#include "Solid_Phase.h"
#include "RxnNetwork.h"
#include "Buildup.h"
#include "RestoreInfo.h"
#include "Evaporation.h"
#include "Sensor.h"
#include "utility_funcs.h"
#include "ObjectiveFunction.h"
#include "Vector_arma.h"
#include "Matrix_arma.h"


//Sassan 
#include "qstring.h"

class GraphWidget;
class QProgressBar;
class runtimeWindow;

using namespace std;



struct measured_chrc //Properties of observed data
{
	string name;
	int loc_type; //0: block, 1: connector
	string quan;
	vector<string> id;
	int std_no;
	int error_structure; //0: normal-additive, 1:lognormal-multiplicative
	int std_to_param;
	string experiment; 
};

class CMediumSet;

enum formulas 
	{
		Pipe1, Pipe2, QDarcy, Vapor, Normal, Rating_curve
	};

class CMedium
{
public:
	bool use_arma = true;
	string name;
	CMediumSet *parent;
	CMedium(void);
	~CMedium(void);
	CMedium(const CMedium &M);
	CMedium(string filename, CMediumSet *P = 0);
	CMedium(CLIDconfig _lid_config, CMediumSet *_parent = 0);
	void CMedium::create(CLIDconfig _lid_config, CMediumSet *_parent);
	double& w(); //current time weight for CN solution
	CMedium& CMedium::operator=(const CMedium &BB); //equal operator
	void get_state(const CMedium & M);
	vector<CConnection> Connector;
	vector<CMBBlock> Blocks;
	vector<CSolid_Phase>& Solid_phase();
	void CMedium::add_Richards_medium(int n, double dz, int id=-1);
	void CMedium::add_Darcy_medium(int n, double dz,  int id=-1);
	void CMedium::add_stream_medium(int n, double z0, double slope, double lenght, int id=-1);
	void CMedium::add_catchment_medium(int n, double z0, double slope, double lenght, int id=-1);
	void CMedium::add_KW_medium(int n, double dz, vector<double> params); 
	void CMedium::add_KW_medium(int n, double dz); 
	void CMedium::set_num_phases(int n); 
	void CMedium::set_num_constituents(int n); 
	void CMedium::set_var(int i, double v);
	void CMedium::set_var(const string &S, const double &v);
	void CMedium::set_var(const string &S, const vector<double> &v);
	void CMedium::set_var(const string &S, const double &v, const int&, const int&);
	void CMedium::set_var(const string &S, const vector<double> &v, const int&, const int&);
	void CMedium::set_var(const string &S, const double &v, const int&);
	void CMedium::set_var(const string &S, const vector<double> &v, const int&);
	void CMedium::set_var(const string &S, int i, const double &v);
	void CMedium::set_z0(double down, double up);
	void CMedium::set_z0(double down);
	void CMedium::set_z0(int id, int n, double z0, double dz);
	void CMedium::setS_star(const CVector &X);
	void CMedium::setS_star(CVector_arma &X);
	void CMedium::set_G_star(const CVector &X);
	
	void CMedium::setH_star();
	void CMedium::setH();
	void CMedium::setQ_star();
	void CMedium::setQ();
	void CMedium::setQ(const CVector &X);
	void CMedium::setQ0();
	double CMedium::get_var(int i, string j, int k); //i: type (i.e. connector vs. block), j: ID, k: variable ID
	double CMedium::get_var(int i, string j, string k); //i: ID, j: type (i.e. connector vs. block), k: variable
	CVector CMedium::get_val(string I, int j);
	double CMedium::get_var(int i, vector<string> j, int k);
	double CMedium::get_var(int i, vector<string> j, string k);
	double CMedium::calc_term(int i, string loc_id, CStringOP k);
	double CMedium::calc_term_star(int i, string loc_id, CStringOP k);
	CVector CMedium::getS();
	CVector CMedium::getres_S(const CVector &X, double dt);
	CVector CMedium::getres_C(const CVector &X, double dt);
	CVector CMedium::getres_Q(const CVector &X, double dtt);

	CMatrix CMedium::Jacobian_S(const CVector &X, double dt, bool);
	CVector CMedium::Jacobian_S(const CVector &V, int &i, double &dt);
	CVector CMedium::Jacobian_S(const CVector &V, CVector &F0, int i, double dt);
	CMatrix CMedium::Jacobian_C(const CVector &X, double dt, bool base=true);
	CVector CMedium::Jacobian_C(const CVector &V, const int i, double dt);
	CVector CMedium::Jacobian_C(const CVector &V, const CVector &F0, int i, double dt);
	CMatrix CMedium::Jacobian_Q(const CVector &X, double dt, bool base=true);  
	CVector CMedium::Jacobian_Q(const CVector &V, int i, double dt);  
	CVector CMedium::Jacobian_Q(const CVector &V, const CVector &F0, int i, double dt);

	//use arma
	CVector_arma CMedium::getres_S(CVector_arma &X, double dt);
	CVector_arma CMedium::getres_C(CVector_arma &X, double dt);
	CVector_arma CMedium::getres_Q(CVector_arma &X, double dtt);
	CMatrix_arma CMedium::Jacobian_S(CVector_arma &X, double dt, bool);
	CVector_arma CMedium::Jacobian_S(CVector_arma &V, int &i, double &dt);
	CVector_arma CMedium::Jacobian_S(CVector_arma &V, CVector_arma &F0, int i, double dt);
	CMatrix_arma CMedium::Jacobian_C(CVector_arma &X, double dt, bool base = true);
	CVector_arma CMedium::Jacobian_C(CVector_arma &V, const int i, double dt);
	CVector_arma CMedium::Jacobian_C(CVector_arma &V, const CVector_arma &F0, int i, double dt);
	CMatrix_arma CMedium::Jacobian_Q(CVector_arma &X, double dt, bool base = true);
	CVector_arma CMedium::Jacobian_Q(CVector_arma &V, int i, double dt);
	CVector_arma CMedium::Jacobian_Q(CVector_arma &V, const CVector_arma &F0, int i, double dt);

	void CMedium::set_CG_star(CVector_arma &X);
	void CMedium::set_CG(CVector_arma &X);
	void CMedium::set_G(CVector_arma &X);
	void CMedium::set_G_star(CVector_arma &X);
	
	void CMedium::onestepsolve_flow_ar(double dt);
	void CMedium::onestepsolve_colloid_ar(double dt);
	void CMedium::onestepsolve_const_ar(double dtt);
	//use arma

	CVector CMedium::getH(const CVector &X);
	CVector CMedium::getQ(const CVector &X);
	void CMedium::onestepsolve_flow(double dt);
	void CMedium::onestepsolve_colloid(double dt);
	void CMedium::onestepsolve_const(double dtt);
	void CMedium::solve_fts_m2(double dt);

//Sassan
//	bool CMedium::solve();
	void CMedium::renew();
	void CMedium::renew_G();
	void CMedium::renew_CG();
	void CMedium::initialize();
	double& tol();
	CBTCSet ANS; //Solution containing all the outputs
	CBTCSet ANS_colloids; //Solution containing all the outputs for colloidal phases
	CBTCSet ANS_constituents; //Solution containing all the outputs for chemical constituents
	CBTCSet ANS_obs; //Solutions containing correspondents to the observed data
	CBTCSet ANS_obs_noise; ////Solutions containing correspondents to the observed data with added noise
	CBTCSet ANS_MB;//mass balance check timeseries
	CBTCSet ANS_control; //control outputs
	CBTCSet Solution_dt;
	double Timemin,Timemax;
	CLIDconfig lid_config;
	double& dt();
	vector<range>& parameters(); // properties of unknown parameters
	vector<CSensor>& sensors(); // properties of sensors
	vector<CController>& controllers(); //propoerties of controllers;
	vector<CObjectiveFunction>& objective_functions(); //objective functions for control;
	void CMedium::getparams(string filename);
	vector<measured_chrc>& measured_quan(); 
	vector<double>& std(); // the vector of measured error standard deviations
	CBTCSet& measured_data(); //measured data
	string& pathname(); //pathname of where the files are saved
	string& outputpathname(); //pathname of where the files are saved
	double CMedium::calc_log_likelihood(); //calculate sum log likelihood
	double CMedium::calc_log_likelihood(int i); //calculate sum log likelihood for observed quantity i
	void CMedium::set_param(int param_no, double value); //set the value of an unknown parameter
	void CMedium::finalize_set_param(); 
	void CMedium::set_default_params(); //set the value of the parameters to the default values
	int& solution_method(); //0: Fixed timestep 1: variable timestep
	string& PE_info_filename(); //file name containing parameter estimation configuration
	string detoutfilename_hydro; //file name where the deterministic hydraulic output is saved
	string detoutfilename_wq; //file name where the deterministic water quality output is saved
	string detoutfilename_control; //file name where the controller values are saved
	string detoutfilename_prtcle; //file name where the deterministic partcile output is saved
	string& detoutfilename_obs(); ////file name where the deterministic outputs corresponding to the observed data is saved
	int& writeinterval(); //the interval at which the output are save in output files (default = 1)
	string& realizeparamfilename(); //output file name for Monte-Carlo realizations
	double t; // time
	int counter_flow, counter_colloid, counter_const; // counter for number of iteration
	void CMedium::Blocksmassbalance();  
	vector<string> Precipitation_filename;
	vector<CPrecipitation> Precipitation;
	
	//vector<string> Evaporation_filename;
	//vector<CBTC> Evaporation;
	
	int& nr_iteration_treshold_max();
	int &nr_iteration_treshold_min();
	double& dt_change_rate();
	double& dt_change_failure(); 
	int& nr_failure_criteria();
	bool &sorption();
	int n_default_phases; //number of aquesous/soil matrix phases =1 for solely aqueous =2 when sorption to soil matrix occures
	bool failed;
	CMatrix InvJ1; //Inverse Jacobian Matrix
	CMatrix InvJ2; //Inverse Jacobian Matrix
	CMatrix InvJ_C;
	CMatrix InvJ_Q;

	//arma
	CMatrix_arma InvJ1_arma; //Inverse Jacobian Matrix
	CMatrix_arma InvJ2_arma; //Inverse Jacobian Matrix
	CMatrix_arma InvJ_C_arma;
	CMatrix_arma InvJ_Q_arma;
	//arma

	bool J_update,J_update_C, J_update_Q;
	int& max_J_interval();
	double cr;
	double dt0;
	vector<string>& formulas(); 
	vector<string>& formulasH(); 
	vector<vector<string>>& formulasQ(); 
	vector<vector<string>>& formulasQ2(); 
	vector<vector<string>>& formulasA();
	vector<vector<bool>>& const_area();
	vector<bool>& air_phase();
	vector<vector<bool>>& vaporTransport(); 
	vector<vector<bool>>& settling();
	void CMedium::get_funcs();
	void CMedium::evaluate_functions();
	int CMedium::getblocksq(string id);
	int CMedium::getconnectorsq(string id);
	bool& write_details();
	bool CMedium::is_there_any_fixed_connectors();
	double& wiggle_tolerance();
	CVector CMedium::get_flow_factors();
	void CMedium::set_flow_factors(CVector &X);
	vector<int> CMedium::get_fixed_connect_status();
	void CMedium::set_fixed_connect_status(vector<int> X);
	void CMedium::set_block_fluxes();
	void CMedium::evaluate_K();
	void CMedium::evaluate_K_star();
	void CMedium::evaluate_dispersion();
	void CMedium::evaluate_capacity();
	void CMedium::evaluate_capacity_star();
	void CMedium::evaluate_capacity_c();
	void CMedium::evaluate_capacity_c_star();
	void CMedium::evaluate_area(bool all=false);
	
	void CMedium::correct_S(double dtt);
	bool failed_colloid;
	bool failed_const;
	double pos_def_ratio;
	double pos_def_ratio_const;
	bool& uniformoutput();
	bool& mass_balance_check();
	bool& colloid_transport();
	bool& constituent_transport();
	bool& forward();
	int& epoch_limit();
	
	int epoch_count;
	double& avg_dt_limit();
	CMatrix M;
	CMatrix_arma M_arma;
	string fail_reason;
	
	void CMedium::write_state(string filename);
	void CMedium::read_state(string filename);
	int& restore_interval();
	double dtt;
	double base_dtt;
	int where_base_dtt_changed = 0;
	double avg_redo_dtt=0;
	int redo_count = 0;
	int CMedium::get_member_no(int solid_id, int phase_no);
	int CMedium::get_member_no(int block_no, int solid_id, int phase_no);
	int CMedium::get_member_no(int block_no, int solid_id, int phase_no, int const_no);
	double CMedium::get_capacity(int block_no, int phase_no, int particle_no);
	double CMedium::get_capacity_star(int block_no, int phase_no, int particle_no);
	vector<int> CMedium::get_phase_solid_id(int i);
	CRxnNetwork& RXN();
	void CMedium::evaluate_const_dispersion();
	void CMedium::set_CG_star(const CVector &X);
	void CMedium::set_CG(const CVector &X);
	void CMedium::set_G(const CVector &X);
	CVector CMedium::get_X_from_CG();
	CVector CMedium::get_X_from_G();
	void CMedium::set_default();
	CMatrix M_Q;
	CMatrix M_C;
	CMatrix_arma M_Q_arma;
	CMatrix_arma M_C_arma;
	string& log_file_name();

	void CMedium::f_get_environmental_params();
	void CMedium::f_get_model_configuration();
	void CMedium::f_set_default_connector_expressions();
	void CMedium::f_set_default_block_expressions();
	void CMedium::f_load_inflows();
	void CMedium::f_make_uniform_inflows();
	
	
	CVector CMedium::get_rxn_chng_rate();
	
	vector<CBuildup>& buildup();
	vector<CEnvExchange>& externalflux();
	vector<CEvaporation>& evaporation_model();

	double dtt_J_h1, dtt_J_h2, dtt_J_q, dtt_J_c;
	int J_h_update_count, J_q_update_count, J_c_update_count;
	void CMedium::writedetails();
	double pos_def_mult;
	double pos_def_mult_Q;
	double max_wiggle, wiggle_dt_mult, dt_fail, max_wiggle_id;
	
	int CMedium::lookup_external_flux(string S);
	int CMedium::lookup_particle_type(string S);
	int CMedium::lookup_buildup(string S);
	int CMedium::lookup_evaporation(string S);
	int CMedium::lookup_parameters(string S);
	int CMedium::lookup_sensors(string S);
	int CMedium::lookup_controllers(string S);
	int CMedium::lookup_objective_functions(string S);
//	int CMedium::lookup_observation(string S);
	
	void CMedium::writetolog(string S);
	CMatrix Preconditioner_Q;
	CMatrix Preconditioner_C;
	CMatrix Preconditioner_S;
	CMatrix_arma Preconditioner_Q_arma;
	CMatrix_arma Preconditioner_C_arma;
	CMatrix_arma Preconditioner_S_arma;
	bool& pos_def_limit();
	bool& CMedium::check_oscillation();
	bool& negative_concentration_allowed();
	
	CRestoreInfo CMedium::getrestoreinfo();
	void CMedium::doredo(CRestoreInfo &R);
	vector<CBTC> temperature;
	vector<string> temperature_filename;
	vector<CBTC> light;
	vector<string> light_filename;
	vector<CBTC> wind;
	vector<string> wind_filename;
	vector<CBTC> r_humidity;
	vector<string> r_humidity_filename;


	void update_rxn_params();
	double current_light;
	double current_temperature;
	double current_wind;
	double current_relative_humidity;
	void update_light_temperature();
	void update_wind_humidity();
	double get_nextcontrolinterval(double _t);
	void set_control_params(int);
	


//Sassan

//	void CMedium::g_get_params();
//	void CMedium::g_get_observed();
	void CMedium::g_get_environmental_params();
	void CMedium::g_get_model_configuration(runtimeWindow* = 0);
//	void CMedium::g_get_particle_types();
//	void CMedium::g_get_constituents();
//	void CMedium::g_get_reactions();
	void CMedium::g_set_default_connector_expressions();
	void CMedium::g_set_default_block_expressions();
//	void CMedium::g_get_buildup();
//	void CMedium::g_get_external_flux();
	void CMedium::g_load_inflows();
//	void CMedium::g_get_evapotranspiration();
	void CMedium::warning(QString text);

	//CMedium(GraphWidget *, runtimeWindow * = 0);
	GraphWidget *gw;
	bool CMedium::solve();
	bool solved() { return !this->failed; }
	int CMedium::get_member_no(QString block_name, QString solid_name, QString phase_name);
	int CMedium::get_member_no(QString block_name, QString solid_name, QString phase_name, QString const_name);
	void CMedium::updateProgress(bool finished = false);
	runtimeWindow * runtimewindow = 0;
	bool stop_triggered = false;
	vector<string> CMedium::get_everything_from_id(int x);
	vector<CRestoreInfo> CMedium::clean_up_restore_points(vector<CRestoreInfo> &Res, double t);

	//Bioest
	void CMedium::onestepsolve_flow_bioest(double dtt);
	double CMedium::getflow(int connector_ID);
	vector<int> CMedium::get_num_block_unpres_inflows(int connector_ID);
	bool& steady_state_hydro();
	void false_connector_flow_calc();
	CMatrix hydro_steady_matrix_inv;
	string create_hydro_steady_matrix_inv();
	CVector get_steady_hydro_RHS();
	vector<int> CMedium::get_member_no_inv(int i);
	vector<int> CMedium::get_relevant_measured_quans();
	int CMedium::lookup_experiment(string S);


	// Control
	double CMedium::calc_obj_function(double time_interval);

};

#endif