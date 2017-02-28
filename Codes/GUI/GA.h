#pragma once

#include "individual.h"
#include "Distribution.h"
#include <stdio.h>
#ifdef GIFMOD
#include "Medium.h"
#include "MediumSet.h"
#endif
#ifdef GWA
#include "gwa.h"
#endif

#include "BTC.h"
#include "DistributionNUnif.h"
#include "math.h"
#include <iostream>
#include "Matrix.h"
#include "omp.h"   
#include <vector>

//GUI
class runtimeWindow;

using namespace std;

class CGA  
{
public:
	
	
	int totnumparams;
	int maxpop;
	int totnParam, nParam;
	int numenhancements, num_enh;
	int nGen;
	int writeinterval;
	int nchains, burnout;
	int nMCMCsamples;
	double acceptance_rate;
	int nrealizations;
	int continue_mcmc;
	int cross_over_type;
	int no_bins;

	bool sensbasedpurt;
	bool justreadmcmc;
	bool mcmc_realization;
	bool calculate_correlation;
	bool sens_out;
	bool global_sensitivity;
	bool calculate_percentile;	
	bool noinipurt;
	bool RCGA;
	bool logtrans, fixedstd, mixederror;
	bool readfromgafile;
	bool calc_distributions;
	bool noise_realization_writeout;
	bool obs_realization_writeout;
	bool const_realization_writeout;
	bool influent_realization;

	double dp_sens;
	double purtscale;
	double purt_fac;
	double sumfitness;
	double N;
	double pcross;
	double pmute;
	double MaxFitness;
	double exponentcoeff;
	double biasfact;
	double shakescale, shakescalered;
	double pertscale, nonpurt;
	bool mcmc_run;

	string initialpopfilemame;
	string pathname;
	string getfromfilename;	
	string outputfilename;	
	string mcmcoutputfile;	
	string forward_inflow_ARIMA_params;
	char fitnesstype;

	vector<double> calc_output_percentiles;
	vector<vector<double>> initial_pop;
	vector<double> final_params;
	vector<int> params;
	vector<int> loged;
	vector<int> to_ts;
	vector<double> fixedinputvale;
	vector<double> minval, maxval;
	vector<bool> apply_to_all;
	vector<vector<int>> outcompare; 
	vector<CIndividual> Ind;
	vector<CIndividual> Ind_old;
	vector<string> paramname;
#ifdef GIFMOD
	vector<CMediumSet> Sys1;
	CMediumSet Sys_out;
	CMediumSet Sys;
#endif	
#ifdef GWA
	vector<CGWA> Sys1;
	vector<CGWA> Sys_out;
	CGWA Sys;
#endif	
	CDistribution fitdist;

	CGA();
	CGA(int n);
	CGA(int n, int nParams);
	CGA(const CGA &C);

	CGA CGA::operator=(CGA &C);

#ifdef GIFMOD
	CGA::CGA(string filename, CMediumSet &Sys);
	vector<CMediumSet>& CGA::assignfitnesses_p(vector<double> inp);
	CGA(CMediumSet *model, runtimeWindow* progress);
#endif
#ifdef GWA
	CGA::CGA(string filename, CGWA &Sys);
	vector<CGWA>& CGA::assignfitnesses_p(vector<double> inp);
	CGA(CGWA *model, runtimeWindow* progress);
#endif

	virtual ~CGA();
	void CGA::initialize();
	void CGA::Setminmax(int a, double minrange, double maxrange, int prec);		
	void CGA::fitnessdistini();	
	void CGA::crossover();
	double CGA::avgfitness();
	void CGA::mutate(double mu);
	void CGA::assignfitnesses();
	int CGA::maxfitness();
	double CGA::variancefitness();	
	double CGA::stdfitness();
	double CGA::avg_actual_fitness();	
	int CGA::optimize();	
	void CGA::setnumpop(int n);	
	double CGA::avg_inv_actual_fitness();
	int CGA::optimize(int nGens, char DefOutPutFileName[]);
	void CGA::setnparams(int n);	
	void CGA::assignfixedvalues();
	void CGA::assignrank();
	void CGA::assignfitness_rank(double N);	
	void CGA::shake();	
	void CGA::crossoverRC();	
	void CGA::getfromfile(char filename[]);	
	void CGA::fillfitdist();
	double CGA::assignfitnesses(vector<double> inp);
	int CGA::getparamno(int i, int ts);
	int CGA::get_act_paramno(int i);
	int CGA::get_time_series(int i);	
	double CGA::evaluateforward();
	double CGA::evaluateforward_mixed(vector<double> v);
	double CGA::getfromoutput(string filename);
	void CGA::getinifromoutput(string filename);	
	void CGA::getinitialpop(string filename);
	
	// GUI
	int numberOfThreads;
	int optimize(runtimeWindow* rtw);
	void assignfitnesses(runtimeWindow* rtw);
	void updateProgress(runtimeWindow* rtw, QMap<QString, QString> vars, bool finished = false) const;
	void updateProgress(runtimeWindow* rtw, bool resetGeneration = false) const;


};


