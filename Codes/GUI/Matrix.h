// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "vector.h"
#include <iostream>
#include "math.h"
#define ARMA_DONT_PRINT_ERRORS
#include "armadillo"
class QVariant;
//class QString;
//class QList;
#include "QMap"
#include "Matrix_arma.h"
#include "Vector_arma.h"

using namespace arma;
class CVector;
class CMatrix  
{
friend class D5Matrix;
private:
	int numrows;
	int numcols;
	int range(int);
public:
	vector<CVector> matr;
	CMatrix(int, int);
	CMatrix(int);
	CMatrix();
	CMatrix(const CMatrix&);
	CMatrix(CMatrix_arma&);
	CMatrix(const CVector&);
	CVector& operator[](int);
	int CMatrix::getnumrows() const;
	int CMatrix::getnumcols() const;
	virtual ~CMatrix();
	CMatrix& CMatrix::operator=(const CMatrix&);
	CMatrix& CMatrix::operator+=(const CMatrix&);
	CMatrix& CMatrix::operator-=(const CMatrix &);	
	CMatrix& CMatrix::operator=(mat&);
	friend CMatrix mult(CMatrix&, CMatrix&);
	friend CVector mult(CMatrix&, CVector&);
	friend CVector mult(CVector&, CMatrix&);
	friend void triangulate(CMatrix&, CVector&);
	friend void backsubst(CMatrix&, CVector&, CVector&);
	friend CVector gauss0(CMatrix, CVector);
	friend CVector diag(CMatrix);
	friend CMatrix Cholesky_factor(CMatrix &M);
	friend CMatrix LU_decomposition(CMatrix &M);
	CMatrix CMatrix::LU_decomposition();
	CMatrix CMatrix::Cholesky_factor();	
	double CMatrix::det();
	void CMatrix::Print(FILE *FIL);
	void CMatrix::print(string s);
	void CMatrix::setval(double a);
	void CMatrix::setvaldiag(double a);
	void CMatrix::writetofile(FILE *f);
	void CMatrix::writetofile(string filename);
	void CMatrix::writetofile_app(string filename);
	friend void write_to_file(vector<CMatrix> M, string filename);
	friend CMatrix Average(vector<CMatrix> M);
	CVector CMatrix::diag_ratio();
	vector<vector<bool>> CMatrix::non_posdef_elems(double tol = 1);
	CMatrix CMatrix::non_posdef_elems_m(double tol = 1);
	CMatrix CMatrix::Preconditioner(double tol = 1);
	vector<string> toString(string format = "", vector<string> columnHeaders = vector<string>(), vector<string> rowHeaders = vector<string>());
	vector<string> toHtml(string format = "", vector<string> columnHeaders = vector<string>(), vector<string> rowHeaders = vector<string>());
	void CMatrix::setnumcolrows();

	QMap<QString, QVariant> CMatrix::compact() const;
	static CMatrix CMatrix::unCompact(QMap<QString, QVariant>);

};
	
double det(CMatrix &);
CMatrix Log(CMatrix &M1);
CMatrix Exp(CMatrix &M1);
CMatrix Sqrt(CMatrix &M1);
CMatrix operator+(const CMatrix&, const CMatrix&);
CMatrix operator+(double, CMatrix);
CMatrix operator+(CMatrix, double);
CMatrix operator-(double d, CMatrix m1);
CMatrix operator+(CMatrix m1, double d);
CMatrix operator-(CMatrix m1,double d);
CMatrix operator/(CMatrix m1,double d);
CMatrix operator/(double d, CMatrix m1);
CMatrix operator-(const CMatrix&, const CMatrix&);
CMatrix operator*(CMatrix&, CMatrix&);
CVector operator*(CMatrix&, CVector&);
CMatrix operator*(CVector, CMatrix);
CMatrix operator*(double, CMatrix);
CVector operator/(CVector&, CMatrix&);
CMatrix Transpose(CMatrix &M1);
CMatrix Invert(CMatrix M1);
CVector SpareSolve(CMatrix, CVector);
CMatrix oneoneprod(CMatrix &m1, CMatrix &m2);
CVector solve_ar(CMatrix&, CVector&);
CMatrix inv(CMatrix&);
CMatrix normalize_diag(CMatrix&, CMatrix&);
CVector normalize_diag(CVector&, CMatrix&);
CMatrix Identity(int rows);


