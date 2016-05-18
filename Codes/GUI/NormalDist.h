#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "NormalDist.h"


class CNormalDist
{
public:
	CNormalDist(void);
	~CNormalDist(void);
	double CNormalDist::unitrandom();
	double CNormalDist::getstdnormalrand();
	double CNormalDist::getnormalrand(double mu, double std);
	CVector CNormalDist::getnormal(CVector &mu, CMatrix &sigma);
	CMatrix CNormalDist::getnormal(int m, int n, double mu, double std);
	CVector CNormalDist::getnormal(int m, double mu, double std);
	double CNormalDist::getlognormalrand(double mu, double std);
	CVector CNormalDist::getlognormal(CVector &mu, CMatrix &sigma);
	CMatrix CNormalDist::getlognormal(int m, int n, double mu, double std);
	CVector CNormalDist::getlognormal(int m, double mu, double std);
	double CNormalDist::likelihood_mixed(double x_mod, double x_obs, double std_ln, double std_n);

};

double getnormalrand(double mu, double std);
double getstdnormalrand();
double unitrandom();