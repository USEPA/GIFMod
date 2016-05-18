#pragma once
#include <vector>
#include "StringOP.h"
#include "Matrix.h"
#include "Vector.h"
#include "Reaction.h"
#include "BTCSet.h"
#include "function.h"
#include "Solid_Phase.h"
#include "RxnNetwork.h"
#include "Buildup.h"
#include "EnvExchange.h"
#include "Evaporation.h"

using namespace std;

enum Block_types { Soil, Pond, Storage, Catchment, Manhole, Darcy, Stream, NativeSoil };

class CMBBlock
{
public:
	CMBBlock(void);
	~CMBBlock(void);
	CMBBlock(const CMBBlock& BB);// copy constructor
	int n_constts; 	int n_phases; //number of chemical species, number of phases;
	void CMBBlock::set_num_phases_constts(int n, int m);
	void CMBBlock::set_num_phases(int n);
	void CMBBlock::set_num_constts(int m);
	CMBBlock& CMBBlock::operator=(const CMBBlock &BB); //equal operator
	string ID; //Identification of the Mass Balance Block
	double H, A, V, S;  //Head, Area, Volume, Storage
	double q; // Darcy flux
	double DS;  //Depression Storage (thickness)
	double H_star, A_star, V_star, S_star; //Head, Area, Volume, Storage
	double z0; //reference_elevation
	double bulk_density;
	int air_phase;
	double porosity; //MM
	vector<CSolid_Phase*> Solid_phase;
	vector<vector<double>> G; //Solid phases G[i][j] i: solid_type_counter, j: phase identifier
	vector<vector<double>> G_star; //Solid phases
	vector<vector<double>> CG; // Sorbed Phases
	vector<vector<double>> CG_star; // Sorbed Phases
	vector<vector<double>> G_stored_mass; //Solid phases G[i][j] i: solid_type_counter, j: phase identifier
	vector<vector<double>> CG_stored_mass; // Sorbed Phases
	

	vector<double> fs_params; //hydraulics_parameters;
	vector<double> rxn_params; //Reaction_parameters
	vector<vector<double>> phi; //Capacities
	vector<CVector> capacity_c; // colloid capacities
	vector<CVector> capacity_c_star;
	vector<CVector> capacity_c_Q; // constituent capacity
	vector<CVector> capacity_c_star_Q;
	void CMBBlock::evaluate_capacity_c();
	void CMBBlock::evaluate_capacity_c_star();
	vector<CMatrix> K; //exchange of phases
	vector<CMatrix> K_star; //exchange of phases
	CMatrix f; //interfacial surface area between phases
	vector<CMatrix> kappa; //contaminant mass exchange between phases
	CStringOP H_S_expression; //Storage-Head Relationship
	string H_S_expression_txt;
	CStringOP V_S_expression; //Storage-Volume Relationship
	double CMBBlock::calc_rxn_rate(CStringOP &C); //calculate reaction rate
	vector<double> CMBBlock::calc_rxn_prod_rate(); //calculate production rate of all constituents
	double CMBBlock::get_rate_exchange(int particle_type, int constituent);
	double CMBBlock::get_exchange_rate_star(int particule_type, int phase, int constituent);
	double CMBBlock::calc(CStringOP &C, vector<int> ii); //The function to calculate any expression
	double CMBBlock::calc_star(CStringOP &C, vector<int> ii); //The function to calculate any expression based on star values
	double CMBBlock::calc(CStringOP &C); //The function to calculate any expression
	double CMBBlock::calc_star(CStringOP &C); //The function to calculate any expression based on star values
	double CMBBlock::get_val(int i, vector<int> ii);// get the value of physical properties, variables and parameters
	double CMBBlock::get_val_star(int i, vector<int>); //get the value of physical properties, variables and parameters based on star values
	double CMBBlock::get_val(int i);// get the value of physical properties, variables and parameters
	double CMBBlock::get_val_star(int i); //get the value of physical properties, variables and parameters based on star values
	double CMBBlock::get_val(string SS); //get the value of physical properties, variables and parameters
	void CMBBlock::set_val(int i, double val);// set the value of physical properties, variables and parameters
	void CMBBlock::set_val_star(int i, double val); //set the value of physical properties, variables and parameters based on star values
	void CMBBlock::set_val(const string &SS, double val); //set the value of physical properties, variables and parameters based on star values
	vector<CBTCSet> inflow; //inflow time-series
	
	
	vector<string> inflow_filename;
	int indicator; //specify block medium 0: soil, 1: pond
	vector<int> connectors; //the id of connectors attached to the block
	vector<int> connectors_se;// 0: block1 of the connector 1: block 2 of the connector
	void CMBBlock::get_funcs(CStringOP &term);
	void CMBBlock::evaluate_functions(int i);
	void CMBBlock::evaluate_functions();
	double vapor_diffusion;
	bool fixed_evaporation;
	double fixed_evaporation_val;
	int setzero;
	double outflow_corr_factor;
	double CMBBlock::get_evaporation(double t);
	double CMBBlock::get_evaporation(int j, double t);
	vector<int> Solid_phase_id;
	/* variable codes: 
	H: 1
	A: 2
	V: 3
	S: 4
	z0: 5
	d: 6
	Q: 7
	v: 8
	Hydraulic Parameters: 50-99
	G: 101-199
	GS: 1000-1999
	Reaction parameters: 2000-2999
	*/
	vector<CFunction> funcs;
	double MBBlocks;  //MM
	bool fixed;// fixed flow for connected connector
	void CMBBlock::evaluate_K();
	void CMBBlock::evaluate_K_star();
	void CMBBlock::evaluate_capacity();
	void CMBBlock::evaluate_capacity_star();
	vector<string> initial_g_counter_p; vector<string> initial_g_counter_l; vector<double> initial_g;
	vector<string> initial_cg_counter_p; vector<string> initial_cg_counter_l; vector<string> initial_cg_counter_c;  vector<double> initial_cg;
	int CMBBlock::get_tot_num_phases();
	CRxnNetwork *RXN;
	double CMBBlock::get_exchange_rate(int particule_type, int phase, int constituent);
	double CMBBlock::get_kd(int particule_type, int phase, int constituent);
	void CMBBlock::set_CG(int particle_type, int phase, int constituent, double val);
	double &CMBBlock::_CG(int particle_type, int phase, int constituent);
	double CMBBlock::get_CG(int particle_type, int phase, int constituent);
	int CMBBlock::get_member_no(int solid_id, int phase_no);
	int CMBBlock::get_member_no(int solid_id, int phase_no, int constituent);
	double CMBBlock::get_reaction_rate(int i, CVector &X, bool star);
	CVector CMBBlock::get_reaction_rates(CVector &X, bool star);
	double CMBBlock::get_reaction_rate(int i, bool star);
	CVector CMBBlock::get_reaction_rates(bool star);
	CVector CMBBlock::get_rxn_change(bool star);
	CVector CMBBlock::get_rxn_change(CVector &X, bool star);
	CVector CMBBlock::get_X_from_CG();
	void CMBBlock::set_CG_star(const CVector &X);
	CMatrix CMBBlock::Eval_Stoichiometry(bool star);
	bool precipitation_swch;
	bool light_swch;
	vector<CBuildup*> buildup;
	vector<string> buildup_id;
	vector<CEnvExchange*> envexchange;
	vector<string> envexchange_id;
	vector<CEvaporation*> evaporation_m;
	vector<string> evaporation_id;
	int CMBBlock::lookup_particle_type(string S);
	int CMBBlock::lookup_env_exchange(string S);
	vector<CBTC> *light;
	vector<CBTC> *temperature;

	double *current_temperature;
	double *current_light;
	double *current_humidity;
	double *current_wind;
	double light_reduction_factor;
	bool perform_rxn = true;
};

