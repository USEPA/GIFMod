#include "Tracer.h"
#include "StringOP.h"
#include "qdebug.h"

CTracer::CTracer(void)
{

}


CTracer::~CTracer(void)
{
}

CTracer::CTracer(const CTracer &m)
{
	input = m.input;
	source = m.source;
	input_multiplier=m.input_multiplier;
	co = m.co;
	decay_rate=m.decay_rate;
	retard=m.retard;
	fm_max=m.fm_max;
	SourceTr = m.SourceTr;
	obs_std=m.obs_std;
	name = m.name;
	cm = m.cm;
	linear_prod=m.linear_prod;
	vz_delay = m.vz_delay;
	constant_input = m.constant_input;
	constant_input_val = m.constant_input_val;	
}

CTracer& CTracer::operator=(const CTracer &m)
{
	input = m.input;
	source = m.source;
	input_multiplier=m.input_multiplier;
	co = m.co;
	decay_rate=m.decay_rate;
	retard=m.retard;
	fm_max=m.fm_max;
	SourceTr = m.SourceTr;
	obs_std=m.obs_std;
	name = m.name;
	cm = m.cm;
	linear_prod=m.linear_prod;
	vz_delay = m.vz_delay;
	constant_input = m.constant_input;
	constant_input_val = m.constant_input_val;	

	return *this;
}

double CTracer::calc_conc(double t, CBTC &young_dist, double f, double _vz_delay, bool fixed_old_conc, double age_old, double fm)
{
	double sum = 0;
	double vz1 = 0;
	double vz2 = 0;
	if (vz_delay == true) vz1 = _vz_delay;
	if (source != "") if (SourceTr->vz_delay == true) vz2 = _vz_delay;
	if (source == "")
	{
		if (linear_prod == false)
		{
			for (int i = 1; i<young_dist.n; i++)
			{
				double a1, a2;
				a1 = input_multiplier*young_dist.C[i - 1] * input.interpol(t - retard*(young_dist.t[i - 1] + vz1))*exp(-decay_rate*retard*(young_dist.t[i - 1] + vz1));
				a2 = input_multiplier*young_dist.C[i] * input.interpol(t - retard*(young_dist.t[i] + vz1))*exp(-decay_rate*retard*(young_dist.t[i] + vz1));
				sum += (1 - fm*fm_max)*0.5*(a1 + a2)*(young_dist.t[i] - young_dist.t[i - 1]);
			}
		}
		else
		{
			for (int i = 1; i<young_dist.n; i++)
			{
				double a1 = input_multiplier*young_dist.C[i - 1] * (input.interpol(t - retard*(young_dist.t[i - 1] + vz1)) + decay_rate*retard*(young_dist.t[i - 1] + vz1));
				double a2 = input_multiplier*young_dist.C[i] * (input.interpol(t - retard*(young_dist.t[i] + vz1)) + decay_rate*retard*(young_dist.t[i] + vz1));
				sum += (1 - fm*fm_max)*0.5*(a1 + a2)*(young_dist.t[i] - young_dist.t[i - 1]);
			}
		}
	}
	else
	{
		for (int i = 1; i<young_dist.n; i++)
		{
			double a1, a2;
			a1 = SourceTr->input_multiplier*young_dist.C[i - 1] * SourceTr->input.interpol(t - SourceTr->retard*(young_dist.t[i - 1] + vz2))*(1 - exp(-SourceTr->decay_rate*SourceTr->retard*young_dist.t[i - 1]))*exp(-SourceTr->decay_rate*SourceTr->retard*vz2);
			a2 = SourceTr->input_multiplier*young_dist.C[i] * SourceTr->input.interpol(t - SourceTr->retard*(young_dist.t[i] + vz2))*(1 - exp(-SourceTr->decay_rate*SourceTr->retard*young_dist.t[i]))*exp(-SourceTr->decay_rate*SourceTr->retard*vz2);
			sum += (1 - fm*fm_max)*0.5*(a1 + a2)*(young_dist.t[i] - young_dist.t[i - 1]);
		}
	}
	if (fixed_old_conc == true)
		sum = sum*(1 - f) + (1 - fm*fm_max)*f*co + fm*fm_max*cm;
	else
		if (source == "")
			if (linear_prod == false)
				sum = sum*(1 - f) + input_multiplier*(1 - fm*fm_max)*f*input.interpol(t - retard*(age_old + vz1))*exp(-decay_rate*retard*(age_old + vz1)) + fm*fm_max*cm;
			else
				sum = sum*(1 - f) + input_multiplier*(1 - fm*fm_max)*f*(input.interpol(t - retard*(age_old + vz1)) + decay_rate*retard*(age_old + vz1)) + fm*fm_max*cm;
		else
			if (linear_prod == false)
				sum = sum*(1 - f) + input_multiplier*(1 - fm*fm_max)*f*SourceTr->input.interpol(t - retard*(age_old + vz1))*(1-exp(-decay_rate*retard*(age_old + vz1))) + fm*fm_max*cm;
			else
				sum = sum*(1 - f) + fm*fm_max*cm;


	return sum;
}


/*double CTracer::calc_conc(double t, CBTC &young_dist, double f, double _vz_delay, bool fixed_old_conc, double age_old, double fm)
{
	//ts = QSort(
	double sum=0;
	double vz1=0;
	double vz2=0; 
	if (vz_delay==true) vz1=_vz_delay;
	if (source!=-1) if (SourceTr->vz_delay==true) vz2=_vz_delay;
	if (source==-1)
	{	if (linear_prod==false)
		{
			for (int i=1; i<young_dist.n; i++)
			{
				double a1,a2;
				a1=input_multiplier*young_dist.C[i-1]*input.interpol(t-retard*(young_dist.t[i-1]+vz1))*exp(-decay_rate*retard*(young_dist.t[i-1]+vz1));
				a2=input_multiplier*young_dist.C[i]*input.interpol(t-retard*(young_dist.t[i]+vz1))*exp(-decay_rate*retard*(young_dist.t[i]+vz1));
				sum += (1-fm*fm_max)*0.5*(a1+a2)*(young_dist.t[i]-young_dist.t[i-1]);
			}
		}
		else
		{
			for (int i=1; i<young_dist.n; i++)
			{
				double a1=input_multiplier*young_dist.C[i-1]*(input.interpol(t-retard*(young_dist.t[i-1]+vz1))+decay_rate*retard*(young_dist.t[i-1]+vz1));
				double a2=input_multiplier*young_dist.C[i]*(input.interpol(t-retard*(young_dist.t[i]+vz1))+decay_rate*retard*(young_dist.t[i]+vz1));
				sum += (1-fm*fm_max)*0.5*(a1+a2)*(young_dist.t[i]-young_dist.t[i-1]);
			}
		}
	}
	else
	{
		for (int i=1; i<young_dist.n; i++)
			{
				double a1,a2;
				a1=SourceTr->input_multiplier*young_dist.C[i-1]*SourceTr->input.interpol(t-SourceTr->retard*(young_dist.t[i-1]+vz2))*(1-exp(-SourceTr->decay_rate*SourceTr->retard*young_dist.t[i-1]))*exp(-SourceTr->decay_rate*SourceTr->retard*vz2);
				a2=SourceTr->input_multiplier*young_dist.C[i]*SourceTr->input.interpol(t-SourceTr->retard*(young_dist.t[i]+vz2))*(1-exp(-SourceTr->decay_rate*SourceTr->retard*young_dist.t[i]))*exp(-SourceTr->decay_rate*SourceTr->retard*vz2);
				sum += (1-fm*fm_max)*0.5*(a1+a2)*(young_dist.t[i]-young_dist.t[i-1]);
			}
	}
	if (fixed_old_conc==true)
		sum = sum*(1-f) + (1-fm*fm_max)*f*co + fm*fm_max*cm;
	else
		if (linear_prod==false)
			sum = sum*(1-f) + input_multiplier*(1-fm*fm_max)*f*input.interpol(t-retard*(age_old+vz1))*exp(-decay_rate*retard*(age_old+vz1))+ fm*fm_max*cm;
		else
			sum = sum*(1-f) + input_multiplier*(1-fm*fm_max)*f*(input.interpol(t-retard*(age_old+vz1))+decay_rate*retard*(age_old+vz1))+ fm*fm_max*cm;
	
	return sum;
}*/
void CTracer::set_val(string S, double val)
{
	if (tolower(S) == "co") co = val;
	if (tolower(S) == "cm") cm = val;
	if (tolower(S) == "decay") decay_rate = val;
	if (tolower(S) == "fm") fm_max = val;
	if (tolower(S) == "retard") retard = val;
	if (tolower(S) == "input_multiplier") input_multiplier = val;
	if (tolower(S) == "vz_delay") vz_delay = val;
	if (tolower(S) == "constant_input" || tolower(S) == "constant_input_val")
	{
		constant_input_val = val;
		constant_input = true;
	}
}