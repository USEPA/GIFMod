#pragma once
#include "BTC.h"
#include <vector>
#include "Vector.h"

class CBTCSet
{
public:
	CBTCSet(void);
	CBTCSet(int n);
	CBTCSet(int numberofBTCs, int sizeofBTCvector);
	CBTCSet(const CBTCSet &BTC);
	CBTCSet(const CBTC &BTC);
	CBTCSet::CBTCSet(string filename, bool varytime);
	int nvars;
	vector<CBTC> BTC;
	void CBTCSet::writetofile(char outputfile[]);
	int CBTCSet::maxnumpoints();
	CBTCSet& CBTCSet::operator = (const CBTCSet &C);
	vector<string> names;
	bool unif;
	void CBTCSet::writetofile(string outputfile, bool writeColumnHeaders = false);
	void CBTCSet::writetofile(string outputfile, int writeinterval);
	vector<double> CBTCSet::interpolate(double t);
	vector<double> CBTCSet::interpolate(double t, int fnvars);
	void CBTCSet::getfromfile(string filename, bool varytime);
	double CBTCSet::maxtime();
	double CBTCSet::mintime();
	vector<double> CBTCSet::getrandom();
	vector<double> CBTCSet::percentile(double x);
	vector<double> CBTCSet::mean(int limit);
	vector<double> CBTCSet::mean(int limit, vector<int> index);
	vector<double> CBTCSet::std(int limit);
	vector<double> CBTCSet::std(int limit, vector<int> index);
	CMatrix CBTCSet::correlation(int limit, int n);
	vector<double> CBTCSet::integrate();
	vector<double> CBTCSet::average();
	vector<double> CBTCSet::percentile(double x, int limit);
	vector<double> CBTCSet::percentile(double x, int limit, vector<int> index);
	vector<double> CBTCSet::getrandom(int burnin);
	void CBTCSet::append(double t, vector<double> c);
	CBTC CBTCSet::add(vector<int> ii);
	CBTC CBTCSet::add_mult(vector<int> ii, vector<double> mult);
	CBTC CBTCSet::add_mult(vector<int> ii, CBTCSet &mult);
	CBTC CBTCSet::divide(int ii, int jj);
	CBTCSet CBTCSet::make_uniform(double increment);
	CBTCSet CBTCSet::getpercentiles(vector<double> percents);
	CVector CBTCSet::out_of_limit(double limit);
	CBTCSet CBTCSet::distribution(int n_bins, int n_columns, int limit);
	CBTCSet CBTCSet::add_noise(vector<double> std, bool logd);
	void CBTCSet::clear();
	vector<double> CBTCSet::max_wiggle();
	vector<double> CBTCSet::max_wiggle_corr(int _n = 10);
	vector<int> CBTCSet::max_wiggle_sl(int ii, double tol);
	CBTCSet CBTCSet::getflow (double A);
	void CBTCSet::knockout(double t);
	int CBTCSet::lookup(string S);
	vector<double> CBTCSet::getrow(int a);
	void setname(int i, string name);

	//Sassan
	bool file_not_found=false;
	CBTC &operator[](int index);
	CBTC &operator[](string BTCName);
	CBTC &operator[](QString BTCName) {
		return operator[](BTCName.toStdString());
	}
	CBTCSet(vector < vector<double>> &, int writeInterval = 1);
	int indexOf(const string& name) const;
	void pushBackName(string name);
	void append(CBTC &BTC, string name = "");
	CBTCSet sort(int burnOut = 0);
	void CBTCSet::compact(QDataStream &data) const;
	static CBTCSet CBTCSet::unCompact(QDataStream &data);
	~CBTCSet(void);
};

double diff(CBTCSet B1, CBTCSet B2);
CBTCSet operator * (const CBTCSet &BTC, const double &C);
CVector norm2dif(CBTCSet &A, CBTCSet &B);
CBTCSet merge(CBTCSet A, const CBTCSet &B);
CBTCSet merge(vector<CBTCSet> &A);
CVector sum_interpolate(vector<CBTCSet> &BTC, double t);
double sum_interpolate(vector<CBTCSet> &BTC, double t, string name);
int max_n_vars(vector<CBTCSet> &BTC);