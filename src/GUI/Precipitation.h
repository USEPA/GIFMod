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
    CPrecipitation(string filename);
    CPrecipitation(const CPrecipitation &Precip);
    CPrecipitation operator = (const CPrecipitation &Precip);
	int n;
	vector<double> s;
	vector<double> e;
	vector<double> i;
	bool structured;
	double dt;
    double getval(double time);
    void getfromfile(string filename);


	//MM
    //CBTCSet getflow_Evap(double A);
	
    CBTCSet getflow (double A);
    CBTCSet getflow(double A, double dt);

	static bool isFileValid(string filename);
public:
	~CPrecipitation(void);
};
