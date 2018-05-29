#pragma once
#ifdef GIFMOD
#include <vector>
#include "Reaction.h"
#include <string>
#include "BTCSet.h"
#include <iostream>
#include <fstream>
#include "BTCSet.h"
#include "Constituent.h"

using namespace std;

struct rxparam
{
	string name;
	double tempcorr;
	int param_no;
	double value;
	double eff_value;
	bool param;

};

class CRxnNetwork
{
public:
	double rate_change_norm;
	int n_constts;
	int n_params;
	int n_Rxns;
	double volume;
	int ASM_ID;
	int ntseris;
	string pathname;	
	
	vector<double> temperature_avg;
	vector<double> inflows_avg;
	vector<vector<int>> inflowconsts;
	vector<vector<int>> controlconsts;
	vector<double> params;
	vector<double> param_corrected;
	vector<CReaction> Rxts;
	vector<CConstituent> cons;
	vector<CBTC> inflows;
	vector<CBTC> temperature;
	
	CRxnNetwork(void);
	~CRxnNetwork(void);
	CRxnNetwork(int n_Rxns, int n_params, int n_constts);
	CRxnNetwork(const CRxnNetwork &CRxnNetwork);

    CRxnNetwork operator=(const CRxnNetwork &CR);
    void getreactions(ifstream& file);
    void getreactions(string filename);
    void getconstituents(string filename);
    void setnumconcs(int n);
    void setnumparams(int n);
    void onestepbatchsolve(double dt);
    void setfixedvals(int ts, double t);
    void setiniconcs(int ts);

    double calcterm(CStringOP &term);
    double calcterm(CStringOP &term, vector<CConstituent> &C_prime);
    double calcterm(CStringOP &term, int ts, double temperature);
    double calcterm(CStringOP &term, int ts, double temperature, vector<CConstituent> &C_prime);
		
    CVector getrate(vector<CConstituent> &c_star, int ts);
    CVector reaction_rate(vector<CConstituent> &, int);
    CVector influx_rate(vector<CConstituent> &, int);
    void fixed_val(double &, int, int);
    bool fixed_val_check(int, int);
	vector<rxparam> parameters;
    int look_up_constituent_no(string const_name);
    int look_up_rxn_parameters(string S);
};



#endif
