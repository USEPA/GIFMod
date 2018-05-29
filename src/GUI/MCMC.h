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
	CGWASet G;
	CGWASet G_out;





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
    void initialize(vector<double> par);
    bool step(int k);
    bool step(int k, int nsamps, string filename, runtimeWindow* rtw = 0);
	vector<double> purturb(int k);
	CNormalDist ND;
	double purtscale;
    void writeoutput(string filename);
	vector<int> params;
	CBTCSet MData;
	int nActParams;
	int numBTCs;
    int getparamno(int j);
    double posterior(vector<double> par, bool out);
	bool logtrans, fixedstd;
    void getfromGA(const CGA &GA);
	string outputfilename;
	int nsamples;
    int getparamno(int i,int ts)const;
    int get_act_paramno(int i);
    int get_time_series(int i);
	vector<bool> apply_to_all;
	int n_ts;
	int writeinterval;
    CVector sensitivity(double d, vector<double> par);
    CVector sensitivity_ln(double d, vector<double> par);
	runtimeWindow * rtw = 0;
#ifdef GIFMOD
    vector<CBTCSet*> model(vector<double> par);
#endif
#ifdef GWA
    vector<CBTCSet> model(vector<double> par);
#endif
    vector<CBTCSet> model_lumped(vector<double> par);
#ifdef GIFMOD
    vector<CBTCSet> model_lumped(vector<double> par, CMedium &)const ;
    CMatrix sensitivity_mat_lumped(double d, vector<double> par, CMedium &) const;
#endif
#ifdef GWA
    vector<CBTCSet> model_lumped(vector<double> par, CGWA &)const ;
    CMatrix sensitivity_mat_lumped(double d, vector<double> par, CGWA &) const;
#endif
//CMatrix sensitivity_mat(double d, vector<double> par);
    CMatrix sensitivity_mat_lumped(double d, vector<double> par);
    CBTCSet prior_distribution(int n_bins);
	double purt_fac;
	bool mixederror;
	bool noinipurt;
	bool sensbasedpurt;
	vector<string> paramname;
	bool global_sensitivity;
	bool continue_mcmc;
    int readfromfile(string filename);

	vector<vector<CBTCSet>> BTCout_obs;
	vector<vector<CBTCSet>> BTCout_obs_noise;
	vector<vector<CBTCSet>> BTCout_obs_prcntle;
	vector<vector<CBTCSet>> BTCout_obs_prcntle_noise;
	vector<CMatrix> global_sens_lumped;
	CBTCSet paramsList;
	CBTCSet realized_paramsList;
    void getrealizations(CBTCSet &MCMCout);
    void get_outputpercentiles(CBTCSet &MCMCout);
	int n_realizations;
	double dp_sens;
	bool noise_realization_writeout;
	vector<double> calc_output_percentiles;
	int numberOfThreads = 8;
	double accepted_count=0, total_count=0;
	double acceptance_rate;

};

