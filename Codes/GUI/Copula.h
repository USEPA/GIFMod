#pragma once

#include <vector>
#include <string>
#include "Distribution.h"

using namespace std;
class CCopula
{
public:
	CCopula(void);
	CCopula(vector<string> names, vector<vector<double>> params);
	CCopula(vector<string> names);
	~CCopula(void);
	vector<string> distribution_name; //normal, lognormal, inverse_gaussian, levy, gamma, uniform
	double evaluate(vector<double> x);
	vector<vector<double>> params;
	double CCopula::evaluate_marginal(double x, int i);
	vector<CDistribution> marginals;

};

