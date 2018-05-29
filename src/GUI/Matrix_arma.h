// Matrix.h: interface for the CMatrix_arma class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Vector_arma.h"
#include <iostream>
#include <math.h>
#define ARMA_DONT_PRINT_ERRORS
#include "armadillo"
class QVariant;
//class QString;
//class QList;
//#include "QMap"
using namespace arma;
class CVector_arma;
class CMatrix_arma  
{

private:
	int numrows;
	int numcols;
	
public:
	mat matr;
	CMatrix_arma(int, int);
	CMatrix_arma(int);
	CMatrix_arma();
	CMatrix_arma(const CMatrix_arma&);
	CMatrix_arma(const CVector_arma&);
	//CVector_arma operator[](int);
    double& get(int i, int j);
	double& operator()(int i, int j);
    vector<double*> get(int i);
    int getnumrows() const;
    int getnumcols() const;
	virtual ~CMatrix_arma();
    CMatrix_arma& operator=(const CMatrix_arma&);
    CMatrix_arma& operator+=(const CMatrix_arma&);
    CMatrix_arma& operator-=(const CMatrix_arma &);
    CMatrix_arma& operator=(mat&);
	friend CMatrix_arma mult(CMatrix_arma&, CMatrix_arma&);
	friend CVector_arma mult(CMatrix_arma&, CVector_arma&);
	friend CVector_arma mult(CVector_arma&, CMatrix_arma&);
	friend void triangulate(CMatrix_arma&, CVector_arma&);
	friend void backsubst(CMatrix_arma&, CVector_arma&, CVector_arma&);
	friend CVector_arma gauss0(CMatrix_arma, CVector_arma);
	friend CVector_arma diag(CMatrix_arma);
	friend CMatrix_arma Cholesky_factor(CMatrix_arma &M);
	friend CMatrix_arma LU_decomposition(CMatrix_arma &M);
    CMatrix_arma LU_decomposition();
    CMatrix_arma Cholesky_factor();
    double det();
    void Print(FILE *FIL);
    void print(string s);
    void setval(double a);
    void setvaldiag(double a);
    void writetofile(FILE *f);
    void writetofile(string filename);
    void writetofile_app(string filename);
	friend void write_to_file(vector<CMatrix_arma> M, string filename);
	friend CMatrix_arma Average(vector<CMatrix_arma> M);
    CVector_arma diag_ratio();
    vector<vector<bool>> non_posdef_elems(double tol = 1);
    CMatrix_arma non_posdef_elems_m(double tol = 1);
    CMatrix_arma Preconditioner(double tol = 1);
	vector<string> toString(string format = "", vector<string> columnHeaders = vector<string>(), vector<string> rowHeaders = vector<string>());
	vector<string> toHtml(string format = "", vector<string> columnHeaders = vector<string>(), vector<string> rowHeaders = vector<string>());
    void setnumcolrows();
    void setrow(int i, CVector_arma V);
    void setrow(int i, CVector V);
    void setcol(int i, CVector_arma V);
    void setcol(int i, CVector V);

	
};
	
double det(CMatrix_arma &);
CMatrix_arma Log(CMatrix_arma &M1);
CMatrix_arma Exp(CMatrix_arma &M1);
CMatrix_arma Sqrt(CMatrix_arma &M1);
CMatrix_arma operator+(const CMatrix_arma&, const CMatrix_arma&);
CMatrix_arma operator+(double, CMatrix_arma);
CMatrix_arma operator+(CMatrix_arma, double);
CMatrix_arma operator-(double d, CMatrix_arma m1);
CMatrix_arma operator+(CMatrix_arma m1, double d);
CMatrix_arma operator-(CMatrix_arma m1,double d);
CMatrix_arma operator/(CMatrix_arma m1,double d);
CMatrix_arma operator/(double d, CMatrix_arma m1);
CMatrix_arma operator-(const CMatrix_arma&, const CMatrix_arma&);
CMatrix_arma operator*(CMatrix_arma, CMatrix_arma);
CVector_arma operator*(CMatrix_arma, CVector_arma);
CMatrix_arma operator*(CVector_arma, CMatrix_arma);
CMatrix_arma operator*(double, CMatrix_arma);
CVector_arma operator/(CVector_arma&, CMatrix_arma&);
CMatrix_arma Transpose(CMatrix_arma &M1);
CMatrix_arma Invert(CMatrix_arma M1);
CVector_arma SpareSolve(CMatrix_arma, CVector_arma);
CMatrix_arma oneoneprod(CMatrix_arma &m1, CMatrix_arma &m2);
CVector_arma solve_ar(CMatrix_arma&, CVector_arma&);
CMatrix_arma inv(CMatrix_arma&);
CMatrix_arma normalize_diag( CMatrix_arma,  CMatrix_arma);
CVector_arma normalize_diag( CVector_arma,  CMatrix_arma);
CMatrix_arma Identity_ar(int rows);


