#pragma once

#include "StringOP.h"
#include <vector>
#include "MBBlock.h"
#include "Solid_Phase.h"
#include "RxnNetwork.h"
#include "Controller.h"

enum physical_params_Q
{
	diameter=2, width=5, n_manning=6, flow_exponent=8, pipe_c=17, z1=10, z2=11, rating_curve_coeff=12, rating_curve_power=13, rating_curve_datum=14
};



class CConnection
{
public:
	CConnection(void);
	CConnection(const CConnection &CC); // copy constructor (Creating a new Object as a copy of an existing object)//should be CConnection(const CConnection& CC); however, both are the same
	CConnection& CConnection::operator=(const CConnection &CC);//equal operator
	~CConnection(void);
	string Block1ID, Block2ID;
	CMBBlock *Block1, *Block2; //The blocks connected via this connection AM
	vector<CSolid_Phase*> Solid_phase;
	vector<int> Solid_phase_id;
	vector <double> flow_params; 
	bool fixed;// fixed flow
	double fixed_val; //the value of fixed flow
	/*Richards: 
		50: K_s: Saturated Hydraulic Conductivity
		51: theta_s, saturated moisture content
		52: theta_r, residual moisture content
		53: alpha, van Genuchten alpha
		54: n, van Genuchten n
		55: m, van Genuchten m
		56: L, van Genuchten L
	Pipe (HW):
		50: Roughness
		51: Exponent
	Kinematic Wave:
		50: Roughness coefficient
		51: Exponent
	Saturated Flow:
		50: Saturated Hydraulic Conductivity
	*/
	/* variable codes: 
		H: 1
		A: 2
		V: 3
		S: 4
		z0: 5
		d: 6
		Q: 7
		v: 8
		s: 9 :effective saturation
		theta: 10: volumetric water content 
		Hydraulic Parameters: 50-99
		G: 101-199
		GS: 1000-1999
		Reaction parameters: 2000-2999
		Dispersion: 3000-3099
		tr_alpha: 3100-3199
	*/
	double Q, Q_star; // flow rate: positive from block1 to block 2;
	double Q_v, Q_v_star; // vapor diffusion flow rate: positive from block1 to block 2;
	double d; //distance from centers of blocks //dz for richards equation
	double A, A_star;//Cross-sectional Area
	double v, v_star;
	double dispersivity;
	int settling;

	vector<double> c_dispersion; //4000
	vector<double> c_dispersion_star; //4000
	vector <double> dispersion; //5000-5099
	vector <double> dispersion_star;	

	CStringOP flow_expression; //flow rate expression
	string flow_expression_strng;
	CStringOP flow_expression_v; //vapor flow rate expression
	string flow_expression_strng_v;
	CStringOP dispersion_expression;
	string dispersion_strng;
	CStringOP area_expression;
	string area_expression_strng;
	bool const_area;
	double CConnection::calc(CStringOP &C, int ii=-1); //The function to calculate any expression
	double CConnection::calc_star(CStringOP &term, int ii=-1); //The function to calculate any for star values
	double CConnection::get_val(int i, int ii=0); // get the value of physical properties, variables and parameters
	double CConnection::get_val_star(int i,int ii=0); //get the value of physical properties, variables and parameters based on star values
	double CConnection::get_val(string S);
	double CConnection::set_val(int i, double val);// set the value of physical properties, variables and parameters
	double CConnection::set_val_star(int i, double val); //set the value of physical properties, variables and parameters based on star values
	void CConnection::set_val(string SS, double val); //set the values of physical parameters
	
	vector<CFunction> funcs;
	void CConnection::get_funcs(CStringOP &term);
	void CConnection::evaluate_functions(int i);
	void CConnection::evaluate_functions();
	void CConnection::evaluate_dispersion();
	void CConnection::evaluate_dispersion_star();
	double flow_factor;
	CRxnNetwork *RXN;
	void CConnection::evaluate_const_dispersion();
	void CConnection::evaluate_const_dispersion_star();
	string ID;
	CBTC presc_flowrate;
	string pre_flow_filename;
	bool presc_flow;
	bool flow_calc_done = false;
	bool control = false;
	string controller_id=""; 
	CController *Controller; 

};


