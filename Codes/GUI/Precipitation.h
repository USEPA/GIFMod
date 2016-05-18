#pragma once

#include <vector>
#include <string>

//MM
#include "BTCSet.h"



using namespace std;

class CPrecipitation
{
public:
	CPrecipitation(void);
	CPrecipitation(int n);
	CPrecipitation::CPrecipitation(string filename);
	CPrecipitation::CPrecipitation(const CPrecipitation &Precip);
	CPrecipitation CPrecipitation::operator = (const CPrecipitation &Precip);
	int n;
	vector<double> s;
	vector<double> e;
	vector<double> i;
	bool structured;
	double dt;
	double CPrecipitation::getval(double time);
	void CPrecipitation::getfromfile(string filename);


	//MM
	//CBTCSet CPrecipitation::getflow_Evap(double A);
	
	CBTCSet CPrecipitation::getflow (double A);

	
public:
	~CPrecipitation(void);
};
