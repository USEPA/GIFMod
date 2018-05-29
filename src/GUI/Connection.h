#pragma once

#include "StringOP.h"
#include <vector>
#include "MBBlock.h"
#include "Solid_Phase.h"
#include "RxnNetwork.h"
#include "Controller.h"

#ifndef n_flow_params
#define n_flow_params 40
#endif

enum physical_params_Q
{
	diameter=2, width=35, n_manning=6, flow_exponent=8, pipe_c=17, z1=10, z2=11, rating_curve_coeff=12, rating_curve_power=13, rating_curve_datum=14
};


class CMedium;
class CConnection
{
friend class CMedium;
public:
	CConnection(void);
	CConnection(const CConnection &CC); // copy constructor (Creating a new Object as a copy of an existing object)//should be CConnection(const CConnection& CC); however, both are the same
	CConnection(string properties);
	CConnection& operator=(const CConnection &CC);//equal operator
	~CConnection(void);
	string Block1ID, Block2ID;
	CMBBlock *Block1, *Block2; //The blocks connected via this connection AM
    int Block1N, Block2N;
    string ID;
    bool set_properties(string s);
    bool set_property(string s, double value);
    bool set_property(string s, string value);
    void show_message(string s);
    bool show_messages();
    bool settype(string s);
    bool showmessages;
    vector<string> errors;
    string tostring();
private:
    CMedium *parent;
    vector<CSolid_Phase*> Solid_phase;
    vector<int> Solid_phase_id;
    vector <double> flow_params;
    bool fixed;// fixed flow
    double fixed_val; //the value of fixed flow

    double Q, Q_star; // flow rate: positive from block1 to block 2;
    double Q_v, Q_v_star; // vapor diffusion flow rate: positive from block1 to block 2;
    double d; //distance from centers of blocks //dz for richards equation
    double A, A_star;//Cross-sectional Area
    double v, v_star;
    double dispersivity;
    int settling;
    bool vapor_transport;
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
    double calc(const CStringOP &C, int ii=-1); //The function to calculate any expression
    double calc_star(const CStringOP &term, int ii=-1); //The function to calculate any for star values
    double get_val(int i, int ii=0); // get the value of physical properties, variables and parameters
    double get_val_star(int i,int ii=0); //get the value of physical properties, variables and parameters based on star values
    double get_val(const string &S);
    bool set_val(int i, const double &val);// set the value of physical properties, variables and parameters
    bool set_val_star(int i, const double &val); //set the value of physical properties, variables and parameters based on star values
    bool set_val(const string &SS, const double &val); //set the values of physical parameters

    vector<CFunction> funcs;
    void get_funcs(CStringOP &term);
    void evaluate_functions(int i);
    void evaluate_functions();
    void evaluate_dispersion();
    void evaluate_dispersion_star();
    double flow_factor;
    CRxnNetwork *RXN;
    void evaluate_const_dispersion();
    void evaluate_const_dispersion_star();

    CBTC presc_flowrate;
    string pre_flow_filename;
    bool presc_flow;
    bool flow_calc_done = false;
    bool control = false;
    string controller_id="";
    CController *Controller;
};


