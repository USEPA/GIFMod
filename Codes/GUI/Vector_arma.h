
#pragma once

#include <iostream>
#include <vector>
#include "armadillo"


using namespace arma;
using namespace std;

class CMatrix_arma;
class CVector;
class SizeDist;
class CVector_arma
{
private:
	
	
public:
	vec vect;
	CVector_arma();
	CVector_arma(int);
	CVector_arma(const vector<double>, int);
	CVector_arma(const vector<double> &v);
	CVector_arma(const vec &v);
	CVector_arma(CVector &v);
	CVector_arma(const vector<int> &v);
	CVector_arma(const double x, int n);
	CVector_arma::CVector_arma(const double x_min, const double x_max, int n);  //CVector_arma:: is redundant. However, works fine here.
	CVector_arma(const CVector_arma&);
	double& CVector_arma::operator[](int);
	virtual ~CVector_arma();
	int num;
	int CVector_arma::range(int);
	CVector_arma& CVector_arma::operator=(const CVector_arma&);
	CVector_arma& CVector_arma::operator=(const CVector&);
	CVector_arma& CVector_arma::operator=(const vector<double>&);
	CVector_arma& CVector_arma::operator=(const double &v);
	CVector_arma CVector_arma::operator=(mat);
	CVector_arma& CVector_arma::operator+();
	void CVector_arma::swap(int , int );
	int CVector_arma::getsize();
	CVector_arma& CVector_arma::operator*=(double);
	CVector_arma& CVector_arma::operator/=(double);
	CVector_arma& CVector_arma::operator+=(const CVector_arma&);
	CVector_arma& CVector_arma::operator-=(const CVector_arma&);
	CVector_arma& CVector_arma::operator*=(const CVector_arma&);
	friend double dotproduct(CVector_arma, CVector_arma);
	friend CVector_arma mult(CMatrix_arma&, CVector_arma&);	
	friend double norm(CVector_arma);			//Friend can be deleted. we don't have any private or protected variable in this class  //
	friend double dotproduct(CVector_arma v1, CVector_arma v2);
	bool CVector_arma::operator==(double v);
	bool CVector_arma::operator==(CVector_arma &v);
	double CVector_arma::max();
	double CVector_arma::min();
	double CVector_arma::norm2();
	double CVector_arma::sum();
	double CVector_arma::abs_max();
	CMatrix_arma CVector_arma::T();
	CVector_arma CVector_arma::Log();
	CVector_arma CVector_arma::abs();
	CVector_arma CVector_arma::H();
	void CVector_arma::writetofile(FILE *f);
	void CVector_arma::writetofile(string filename);
	void CVector_arma::writetofile(ofstream &f);
	void CVector_arma::writetofile_app(string filename);
	CVector_arma CVector_arma::Exp();
	vector<int> CVector_arma::Int();
	CMatrix_arma CVector_arma::diagmat();
	CVector_arma CVector_arma::append(const CVector_arma& V1);
	CVector_arma CVector_arma::append(double d);
	CVector_arma CVector_arma::sort();
	vector<int> CVector_arma::lookup(double val);
	void CVector_arma::print(string s);
	CVector_arma CVector_arma::sub(int i, int j);
	bool CVector_arma::is_finite();

};

CVector_arma Log(CVector_arma &);
CVector_arma Exp(CVector_arma &);
CVector_arma abs(CVector_arma &);  //works w/o reference. if const included means read only
double abs_max(CVector_arma &);
double min(CVector_arma &);
double max(CVector_arma &);
CVector_arma H(CVector_arma &);
CVector_arma operator+(const CVector_arma&, const CVector_arma&);
CVector_arma operator+(double, CVector_arma);
CVector_arma operator+(CVector_arma, double);
CVector_arma operator-(const CVector_arma&, const CVector_arma&);
CVector_arma operator-(double, CVector_arma&);
CVector_arma operator*(CVector_arma, CVector_arma);
CVector_arma operator*(double, CVector_arma);
CVector_arma operator*(CVector_arma, double);
CVector_arma operator/(CVector_arma, double); 
CVector_arma operator/(CVector_arma, CVector_arma);
CVector_arma operator/(double, CVector_arma);
CVector_arma zeros_ar(int i);
double avg(CVector_arma &);
