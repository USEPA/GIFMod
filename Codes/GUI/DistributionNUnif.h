
#pragma once
#include <vector>

using namespace std;

class CDistributionNUnif  
{
public:
	CDistributionNUnif();
	CDistributionNUnif(int n);
	virtual ~CDistributionNUnif();
	int n;
	vector<double> x;
	vector<double> y;
	double CDistributionNUnif::GetRndNorm(double mean, double std);
	double CDistributionNUnif::GetRndGamma();
	void CDistributionNUnif::initializeNormal(double dx0, double dxmult,int nint);
	void CDistributionNUnif::initializeGamma(double dx0, double dxmult, int nint, double r, double lambda);
	bool set;
	bool symetrical;
	CDistributionNUnif(const CDistributionNUnif &D);
	CDistributionNUnif CDistributionNUnif::operator=(const CDistributionNUnif &D);
};

double Gammapdf(double x, double r, double lambda);
double NormalStdpdf(double x);
double gamma(double x);
double GetRndUniF(double xmin, double xmax);
