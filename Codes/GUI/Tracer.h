#pragma once
#include "BTCSet.h"
#include "Vector.h"
#include <vector>
#include "Distribution.h"
#include "QuickSort.h"

using namespace std;
class CTracer
{
public:
	CTracer(void);
	CTracer(const CTracer &m);
	~CTracer(void);
	CBTC input;
	double input_multiplier;
	double obs_std;
	string source;
	double fm_max;
	double decay_rate;
	double co;
	double retard;
	double cm;
	bool linear_prod;	
	CTracer *SourceTr;
	string name;
	double CTracer::calc_conc(double t, CBTC &young_dist, double f, double vz_delay=0, bool fixed_old_tracer=0, double age_old=100000, double fm=0);
	CTracer& CTracer::operator=(const CTracer &m);
	bool vz_delay;
	double constant_input_val; bool constant_input;
	void CTracer::set_val(string S, double val);

};

