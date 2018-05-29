// Matrix.h: interface for the CMatrix class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <iostream>
#include <math.h>
#define ARMA_DONT_PRINT_ERRORS
#include "armadillo"
class QVariant;
//class QString;
//class QList;
#ifdef QT_version
#include <QMap>
#endif // QT_version


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
	CMatrix(string filename);
	CMatrix(const CMatrix&);
	CMatrix(CMatrix_arma&);
	CMatrix(const CVector&);
	CVector& operator[](int);
    int getnumrows() const;
    int getnumcols() const;
	virtual ~CMatrix();
    CMatrix& operator=(const CMatrix&);
    CMatrix& operator+=(const CMatrix&);
    CMatrix& operator-=(const CMatrix &);
    CMatrix& operator=(mat&);
	friend CMatrix mult(CMatrix&, CMatrix&);
	friend CVector mult(CMatrix&, CVector&);
	friend CVector mult(CVector&, CMatrix&);
	friend void triangulate(CMatrix&, CVector&);
	friend void backsubst(CMatrix&, CVector&, CVector&);
	friend CVector gauss0(CMatrix, CVector);
	friend CVector diag(CMatrix);
	friend CMatrix Cholesky_factor(CMatrix &M);
	friend CMatrix LU_decomposition(CMatrix &M);
    CMatrix LU_decomposition();
    CMatrix Cholesky_factor();
    double det();
    void Print(FILE *FIL);
    void print(string s);
    void setval(double a);
    void setvaldiag(double a);
    void writetofile(FILE *f);
    void writetofile(string filename);
    void writetofile_app(string filename);
	friend void write_to_file(vector<CMatrix> M, string filename);
	friend CMatrix Average(vector<CMatrix> M);
    CVector diag_ratio();
    vector<vector<bool>> non_posdef_elems(double tol = 1);
    CMatrix non_posdef_elems_m(double tol = 1);
    CMatrix Preconditioner(double tol = 1);
	vector<string> toString(string format = "", vector<string> columnHeaders = vector<string>(), vector<string> rowHeaders = vector<string>());
	vector<string> toHtml(string format = "", vector<string> columnHeaders = vector<string>(), vector<string> rowHeaders = vector<string>());
    void setnumcolrows();

#ifdef QT_version
    QMap<QString, QVariant> compact() const;
    static CMatrix unCompact(QMap<QString, QVariant>);
#endif // QT_version

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
CMatrix operator*(CMatrix, CMatrix);
CVector operator*(CMatrix, CVector);
CMatrix operator*(CVector, CMatrix);
CMatrix operator*(double, CMatrix);
CVector operator/(CVector&, CMatrix&);
CMatrix Transpose(CMatrix &M1);
CMatrix Invert(CMatrix M1);
CVector SpareSolve(CMatrix, CVector);
CMatrix oneoneprod(CMatrix &m1, CMatrix &m2);
CVector solve_ar(CMatrix&, CVector&);
CMatrix inv(CMatrix);
CMatrix normalize_diag(CMatrix&, CMatrix&);
CVector normalize_diag(CVector&, CMatrix&);
CMatrix Identity(int rows);


