#pragma once

#include <string>
#include <vector>
#include "BTCSet.h"
#include "StringOP.h"

using namespace std;
class CSensor
{
public:
	CSensor();
	~CSensor();
	CSensor(int numberofExperiments);
	CSensor(const CSensor &M);
    CSensor& operator=(const CSensor &BB);
	void append_output(double t, double C, int experiment_id);
	string name;
	int loc_type; //0: block, 1: connector
	CStringOP quan;
	string id;
	int error_structure; //0: normal-additive, 1:lognormal-multiplicative
	double error_std;
	double interval;
	vector<CBTC> output;
	void setNumberofExperiments(int n) {
		output.resize(n);
	}
};

