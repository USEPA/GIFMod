
#pragma once
#include <vector>

using namespace std;

class CDistribution  
{
public:
	CDistribution();
	virtual ~CDistribution();
	int n;
	vector<double> s;
	vector<double> e;
	CDistribution::CDistribution(int nn);
	CDistribution(const CDistribution &C);
	CDistribution CDistribution::operator = (const CDistribution &C);
	int CDistribution::GetRand();

};

