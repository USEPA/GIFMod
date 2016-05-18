
#pragma once


#include <string>
#include <vector>
#include "QuickSort.h"
#include "NormalDist.h"

//GUI
#include "qlist.h"
#include "qmap.h"
#include "qvariant.h"



using namespace std;


class CBTC  
{
public:
	bool structured;
	CBTC();
	CBTC(int n);
	virtual ~CBTC();
	int n;
	vector<double> t;
	vector<double> C;
	
	string name = "";
	string unit = "";
	string defaultUnit = "";
	vector<string> unitsList;

	//MM
	vector<double> D;

	double CBTC::interpol(double x);
	double CBTC::interpol_D(double x);
	CBTC CBTC::interpol(vector<double> x);
	CBTC CBTC::interpol(CBTC &x);
	CBTC(const CBTC &C);
	CBTC::CBTC(string Filename);
	CBTC& CBTC::operator = (const CBTC &C);
	void CBTC::readfile(string);
	void CBTC::writefile(string Filename);
	double CBTC::maxC();
	double CBTC::minC();
	void CBTC::setnumpoints(int);
	CBTC CBTC::Log();
	CBTC CBTC::Log(double min);
	double CBTC::std();
	double CBTC::mean();
	double CBTC::percentile(double x);
	double CBTC::percentile(double x, int limit);
	double CBTC::mean(int limit);
	double CBTC::std(int nlimit);
	double CBTC::mean_log(int limit);
	double CBTC::integrate();
	double CBTC::average();
	CBTC CBTC::distribution(int n_bins, int limit);
	void CBTC::append(double x);
	void CBTC::append(double tt, double xx);
	void CBTC::append(CBTC &CC);
	CBTC& CBTC::operator+=(CBTC &v);
	CBTC& CBTC::operator%=(CBTC &v);
	CBTC CBTC::make_uniform(double increment);
	CBTC CBTC::extract(double t1, double t2);
	vector<double> CBTC::trend();
	double CBTC::mean_t();
	CBTC CBTC::add_noise(double std, bool);
	void CBTC::assign_D();
	void CBTC::clear();
	double CBTC::wiggle();
	double CBTC::maxfabs();
	double max_fabs;
	void CBTC::knock_out(double t);
	double CBTC::AutoCor1(int i=0);
	bool file_not_found = false;
	CBTC CBTC::getcummulative();
	CBTC CBTC::Exp();

	//GUI 
	QList <QMap <QVariant, QVariant>> CBTC::compact() const;
	CBTC::CBTC(QList <QMap <QVariant, QVariant>> data);
	CBTC::CBTC(double a, double b, const vector<double>&x);
	CBTC::CBTC(double a, double b, const CBTC &btc);
	CBTC::CBTC(const vector<double> &t, const vector<double> &C);
	CBTC(vector<double>&, int writeInterval = 1);
	bool error = false;
};

double diff(CBTC &BTC_p, CBTC &BTC_d);
double diff_abs(CBTC &BTC_p, CBTC &BTC_d);
double diff_log(CBTC &BTC_p, CBTC &BTC_d, double lowlim);
double diff_norm(CBTC &BTC_p, CBTC &BTC_d);
double diff(CBTC BTC_p, CBTC BTC_d, int scale);
double diff(CBTC BTC_p, CBTC BTC_d, CBTC Q);
double diff2(CBTC BTC_p, CBTC BTC_d);
double diff_mixed(CBTC &BTC_p, CBTC &BTC_d, double lowlim, double std_n, double std_ln);
double ADD(CBTC &BTC_p, CBTC &BTC_d);
double diff_relative(CBTC &BTC_p, CBTC &BTC_d, double m);
double R2(CBTC BTC_p, CBTC BTC_d);
double R(CBTC BTC_p, CBTC BTC_d, int nlimit);
CBTC operator*(double, CBTC);
CBTC operator*(CBTC, double);
CBTC operator*(CBTC&, CBTC&);
CBTC operator/(CBTC, CBTC);
CBTC operator+(CBTC, CBTC);
CBTC operator-(CBTC, CBTC);
CBTC operator%(CBTC, CBTC);
CBTC operator&(CBTC, CBTC);
CBTC operator>(CBTC BTC1, CBTC BTC2);
double XYbar(CBTC BTC_p, CBTC BTC_d);
double X2bar(CBTC BTC_p, CBTC BTC_d);
double Y2bar(CBTC BTC_p, CBTC BTC_d);
double Ybar(CBTC BTC_p, CBTC BTC_d);
double Xbar(CBTC BTC_p, CBTC BTC_d);
CBTC operator+(CBTC v1, CBTC v2); 
double prcntl(vector<double> C, double x);
vector<double> prcntl(vector<double> C, vector<double> x);
double sgn(double val);
int sgn(int val);
double sum_interpolate(vector<CBTC>, double t);
double R2_c(CBTC BTC_p, CBTC BTC_d);
double norm2(CBTC BTC1);
CBTC max(CBTC A, double b);
//GUI
map<string, double> regression(vector<double> x, vector<double> y);
