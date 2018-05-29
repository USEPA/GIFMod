#pragma once
#include "BTCSet.h"
#include "Vector.h"
#include <vector>
#include "Distribution.h"

using namespace std;
class CWell
{
public:
	CWell(void);
	CWell(const CWell &W);
    CWell& operator=(const CWell &m);
	~CWell(void);
	double fraction_old;
	double age_old;
	CBTC young_age_distribution;
	vector<double> params;
	string name;
	string distribution;
	int hist_bin_num;
	double hist_binsize;
    void createdistribution(vector<double> pdfparams, double tlast, int numinc, double mult);
    CBTC creat_age_dist_dirac (vector<double> params, double tlast, int numinc, double mult);
    CBTC creat_age_dist_exp(vector<double> params, double tlast, int numinc, double mult);
    CBTC creat_age_dist_lognormal(vector<double> params,double tlast, int numinc, double mult);
    CBTC creat_age_dist_InvG(vector<double> params, double tlast, int numinc, double mult);
    CBTC creat_age_dispersion(vector<double> params, double tlast, int numinc, double mult);
    CBTC creat_age_dist_Hist(vector<double> pdfparams, double tlast, int numinc, double mult);
    CBTC creat_age_dist_Gamma(vector<double> params, double tlast, int numinc, double mult);
    CBTC creat_age_dist_Levy(vector<double> params, double tlast, int numinc, double mult);
    CBTC creat_age_dist_ShLevy(vector<double> params, double tlast, int numinc, double mult);
    CBTC creat_age_dist_Shexp(vector<double> params, double tlast, int numinc, double mult);
    CBTC creat_age_dist_GIG(vector<double> params, double tlast, int numinc, double mult);
	double vz_delay;
	double fm;
    void set_val(string S, double val);
    int get_numparams(string S, int n_bins = 0);

};

