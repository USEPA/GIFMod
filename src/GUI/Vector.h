
#pragma once

#include <iostream>
#include <vector>
#include "QuickSort.h"
#include "armadillo"


using namespace arma;
using namespace std;

class CVector_arma;
class CMatrix;
class SizeDist;
class CVector  
{
private:
	
	
public:
	vector<double> vec;
	CVector();
	CVector(int);
	CVector(const vector<double>, int);
	CVector(const vector<double> &v);
	CVector(CVector_arma &v);
	CVector(const vector<int> &v);
	CVector(const double x, int n);
    CVector(const double x_min, const double x_max, int n);  //cvector:: is redundant. However, works fine here.
	CVector(const CVector&);
    double& operator[](int);
	virtual ~CVector();
	int num;
    int range(int);
    CVector& operator=(const CVector&);
    CVector& operator=(const vector<double>&);
    CVector& operator=(CVector_arma&);
    CVector& operator=(const double &v);
    CVector operator=(mat);
	//mat CVector::operator=(const CVector&);
    CVector& operator+();
    void swap(int , int );
    int getsize();
    CVector& operator*=(double);
    CVector& operator/=(double);
    CVector& operator+=(const CVector&);
    CVector& operator-=(const CVector&);
    CVector& operator*=(const CVector&);
	friend double dotproduct(CVector, CVector);
	friend CVector mult(CMatrix&, CVector&);	
	friend double norm(CVector);			//Friend can be deleted. we don't have any private or protected variable in this class  //
	friend double dotproduct(CVector v1, CVector v2);
    bool operator==(double v);
    bool operator==(CVector &v);
    double max();
    double min();
    double norm2();
    double sum();
    double abs_max();
	int abs_max_elems();
    CMatrix T();
    CVector Log();
    CVector abs();
    CVector H();
    void writetofile(FILE *f);
    void writetofile(string filename);
    void writetofile(ofstream &f);
    void writetofile_app(string filename);
    CVector Exp();
    vector<int> Int();
    CMatrix diagmat();
    CVector append(const CVector& V1);
    CVector append(double d);
    CVector sort();
    vector<int> lookup(double val);
    void print(string s);
    CVector sub(int i, int j);
    bool is_finite();

};

CVector Log(CVector );
CVector Exp(CVector );
CVector abs(CVector );  //works w/o reference. if const included means read only
double abs_max(CVector );
double min(CVector );
double max(CVector );
CVector H(CVector );
double H(double x);
CVector operator+(CVector, CVector);
CVector operator+(double, CVector);
CVector operator+(CVector, double);
CVector operator-(CVector, CVector);
CVector operator-(double, CVector);
CVector operator*(CVector, CVector);
CVector operator*(double, CVector);
CVector operator/(CVector, double); 
CVector operator/(CVector, CVector);
CVector operator/(double, CVector);
CVector zeros(int i);
CVector combinesort(const CVector V1, const CVector V2);
CVector combinesort_s(const CVector V1, const CVector V2);
int lookup(vector<int> v, int val);
int lookup(vector<double> v, double val);
int lookup(vector<string> v, string val);
double avg(CVector &);
vector<double> create_vector(int i);
vector<vector<double>> create_vector(int i, int j);
template<typename T> bool isfinite(T arg);
