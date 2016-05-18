#pragma once
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

	CRxnNetwork CRxnNetwork::operator=(const CRxnNetwork &CR); 
	void CRxnNetwork::getreactions(ifstream& file);
	void CRxnNetwork::getreactions(string filename);	 	
	void CRxnNetwork::getconstituents(string filename);
	void CRxnNetwork::setnumconcs(int n);
	void CRxnNetwork::setnumparams(int n);
	void CRxnNetwork::onestepbatchsolve(double dt);
	void CRxnNetwork::setfixedvals(int ts, double t);
	void CRxnNetwork::setiniconcs(int ts);	

	double CRxnNetwork::calcterm(CStringOP &term);
	double CRxnNetwork::calcterm(CStringOP &term, vector<CConstituent> &C_prime);
	double CRxnNetwork::calcterm(CStringOP &term, int ts, double temperature);
	double CRxnNetwork::calcterm(CStringOP &term, int ts, double temperature, vector<CConstituent> &C_prime);
		
	CVector CRxnNetwork::getrate(vector<CConstituent> &c_star, int ts);	
	CVector CRxnNetwork::reaction_rate(vector<CConstituent> &, int);
	CVector CRxnNetwork::influx_rate(vector<CConstituent> &, int);
	void CRxnNetwork::fixed_val(double &, int, int);
	bool CRxnNetwork::fixed_val_check(int, int);
	vector<rxparam> parameters;
	int CRxnNetwork::look_up_constituent_no(string const_name);
	int CRxnNetwork::look_up_rxn_parameters(string S);
};



