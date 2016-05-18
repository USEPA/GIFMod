#ifdef GWA
#pragma once
#include "Distribution.h"
#include "Tracer.h"
#include <vector>
#include <string>
#include <iostream>
#include "StringOP.h"
#include "Well.h"
#include "LIDconfig.h"
#include "math.h"
#include "enums.h"

#include "MBBlock.h"
#include "gwidget.h"
class runtimeWindow;

using namespace std;

struct range //properties of unknown parameters
{
	string name;
	double low;
	double high;
	double tempcorr;
	bool fixed;
	int param_no;
	int log;
	bool applytoall;
	vector<string> location;
	vector<string> quan;
	vector<int> location_type; //0: well, 1: tracer
	double value;
	vector<double> conversion_factor;

};


struct measured_chrc //Properties of observed data
{
	string name;
	int loc_type; 
	int quan;
	int id;
	int std_no;
	int error_structure; 
	int std_to_param;
	string observed_filename;
	CBTC observed_data;
	bool detect_limit;
	double detect_limit_value;
	int max_data_no;
	bool count_max;
	double minstdev;
};

class CGWA
{
public:
	CGWA(void);
	~CGWA(void);
	CGWA(string filename);
	CGWA(const CGWA &m);
	CGWA& CGWA::operator=(const CGWA &m);
	void CGWA::getconfigfromfile(string filename);
	vector<CTracer> Tracer;
	vector<CWell> Well;
	CLIDconfig lid_config;
	vector<range> parameters;
	vector<double> params_vals;
	string pathname;
	string outpathname;
	vector<measured_chrc> measured_quan;
//	double CGWA::getlogp();
	void CGWA::getmodeled();
	CBTCSet modeled;  
	CBTCSet projected;
	double CGWA::getoldest();
	vector<double> obs_std;
	string PE_info_filename;
	string detoutfilename;
	string realizedparamfilename;
	bool single_vz_delay;
	void CGWA::set_constant_inputs();
	bool project;
	double project_start, project_finish;
	double project_interval;
	CBTCSet CGWA::Do_project();
	bool inverse;
	bool fixed_old_tracer;
	int lookup_observation(string S);
	int CGWA::lookup_parameters(string S);
	int CGWA::lookup_tracers(string S);
	int CGWA::lookup_wells(string S);
	//Sassan
	int n_time_series = 1;
	double calc_log_likelihood(int ts = 0);
	int epoch_count = 1;
	void set_param(int param_no, double value){
		setparams(param_no, value);
	};
	void setparams(int param_no, double value);
	void CGWA::setparams(vector<double> inp, int n_Params);
	void finalize_set_param(){};
	CBTCSet ANS;
	CBTCSet ANS_obs;
	CBTCSet ANS_obs_noise;
	vector<CMBBlock> Blocks;
	int writeinterval = 1;
	double Timemin, Timemax;
	bool solved() { return !this->failed; } //needs a function
	bool failed = true;
	void CGWA::load_parameters();
	void CGWA::load_wells();
	void CGWA::load_tracers();
	void CGWA::load_observed();
	void CGWA::load_settings();
	double CGWA::doModel();
	
	//SASSAN
	CGWA::CGWA(GraphWidget* gw, runtimeWindow *progress);
	void setDefaults();
	void CGWA::g_get_settings();
	void CGWA::g_get_params();
	void CGWA::g_get_observed();
	void CGWA::g_get_model_configuration(runtimeWindow* progress);
	CWell *wells(const QString &name) {
		return wells(name.toStdString());
	}
	CWell *wells(const string &name) {
		for (int i = 0; Well.size(); i++)
			if (Well[i].name == name)
				return &Well[i];
		return nullptr;
	}
	CTracer *tracers(const QString &name) {
		return tracers(name.toStdString());
	}
	CTracer *tracers(const string &name) {
		for (int i = 0; Tracer.size(); i++)
			if (Tracer[i].name == name)
				return &Tracer[i];
		return nullptr;
	}
	int wellIndex(const QString &name) {
		for (int i = 0; Well.size(); i++)
			if (Well[i].name == name.toStdString())
				return i;
		return -1;
	}
	GraphWidget* gw;
	bool stop_triggered = false;
};

#endif