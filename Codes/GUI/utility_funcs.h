#pragma once
#include <string>
#include <vector>
using namespace std;

struct range //properties of unknown parameters
{
	string name;
	double low;
	double high;
	double value;
	vector<double> conversion_factor;
	double tempcorr;
	bool fixed;
	int param_no;
	bool log;
	bool applytoall;
	vector<int> location;
	vector<string> quan;
	vector<string> experiment_id;
	vector<int> location_type; //0: block, 1: connector; 2: both
};
QString float2date(const double d, QString format = "MMM dd yyyy", bool ignorefirst50years = true);
qint64 xldate2julian(const qint64 xldate);

qint64 julian2xldate(const qint64 juliandate);
int dayOfYear(const qint64 xldate);
double dayOfYear(const double xldate);


double xldate2julian(const double xldate);

double julian2xldate(const double juliandate);
double timetodayfraction(int hh = 0, int mm = 0, int ss = 0);
QList<int> dayfractiontotime(double dayFraction = 0);

QString float2datetime(const double d, QString format = "MM/dd/yyyy hh:mm:ss", bool ignorefirst50years = true);
