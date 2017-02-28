#pragma once
#include <vector>
#include "math.h"
#include <iostream>
#include "NormalDist.h"
#include "GA.h"
#include "Vector.h"
#ifdef GIFMOD
#include "Medium.h"





#endif
#ifdef GWA
#include"gwa.h"




#endif

using namespace std;

struct Param
{
	int param_ID;
	int type; // 0: normal, 1: lognormal, 2: uniform
	double low, high;
	bool loged;
	double mean, std;
};

class CMCMC
{
public:
#ifdef GIFMOD
	CMediumSet G;
	CMediumSet G_out;





#endif
#ifdef GWA
	CGWA G;
	CGWA G_out;





#endif

	CMCMC(void);
	CMCMC(int nn, int nn_chains);
	CMCMC(const CGA &GA);
	~CMCMC(void);
	int n;
	int n_chains;
	int n_burnout;
	double ini_purt_fact = 1;
	vector<Param> MCMCParam;
	vector<vector<double>> Params;
	vector<double> pertcoeff;
	vector<double> logp;
	vector<double> logp1;
	vector<double> u;
	double posterior(vector<double> par);
	void initialize();
	void CMCMC::initialize(vector<double> par);
	bool CMCMC::step(int k);
	bool CMCMC::step(int k, int nsamps, string filename, runtimeWindow* rtw = 0);
	vector<double> purturb(int k);
	CNormalDist ND;
	double purtscale;
	void CMCMC::writeoutput(string filename);
	vector<int> params;
	CBTCSet MData;
	int nActParams;
	int numBTCs;
	int CMCMC::getparamno(int j);
	double CMCMC::posterior(vector<double> par, bool out);
	bool logtrans, fixedstd;
	void CMCMC::getfromGA(const CGA &GA);
	string outputfilename;
	int nsamples;
	int CMCMC::getparamno(int i,int ts)const;
	int CMCMC::get_act_paramno(int i);
	int CMCMC::get_time_series(int i);
	vector<bool> apply_to_all;
	int n_ts;
	int writeinterval;
	CVector CMCMC::sensitivity(double d, vector<double> par);
	CVector CMCMC::sensitivity_ln(double d, vector<double> par);
	runtimeWindow * rtw = 0;
#ifdef GIFMOD
	vector<CBTCSet*> CMCMC::model(vector<double> par);
#endif
#ifdef GWA
	vector<CBTCSet> CMCMC::model(vector<double> par);
#endif
	vector<CBTCSet> CMCMC::model_lumped(vector<double> par);
#ifdef GIFMOD
	vector<CBTCSet> CMCMC::model_lumped(vector<double> par, CMedium &)const ;
	CMatrix CMCMC::sensitivity_mat_lumped(double d, vector<double> par, CMedium &) const;
#endif
#ifdef GWA
	vector<CBTCSet> CMCMC::model_lumped(vector<double> par, CGWA &)const ;
	CMatrix CMCMC::sensitivity_mat_lumped(double d, vector<double> par, CGWA &) const;
#endif
//CMatrix CMCMC::sensitivity_mat(double d, vector<double> par);
	CMatrix CMCMC::sensitivity_mat_lumped(double d, vector<double> par);
	CBTCSet CMCMC::prior_distribution(int n_bins);
	double purt_fac;
	bool mixederror;
	bool noinipurt;
	bool sensbasedpurt;
	vector<string> paramname;
	bool global_sensitivity;
	bool continue_mcmc;
	int CMCMC::readfromfile(string filename); 

	vector<vector<CBTCSet>> BTCout_obs;
	vector<vector<CBTCSet>> BTCout_obs_noise;
	vector<vector<CBTCSet>> BTCout_obs_prcntle;
	vector<vector<CBTCSet>> BTCout_obs_prcntle_noise;
	vector<CMatrix> global_sens_lumped;
	CBTCSet paramsList;
	CBTCSet realized_paramsList;
	void CMCMC::getrealizations(CBTCSet &MCMCout);
	void CMCMC::get_outputpercentiles(CBTCSet &MCMCout);
	int n_realizations;
	double dp_sens;
	bool noise_realization_writeout;
	vector<double> calc_output_percentiles;
	int numberOfThreads = 8;
	double accepted_count=0, total_count=0;
	double acceptance_rate;

};

