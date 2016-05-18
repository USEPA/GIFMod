// Vector.cpp: implementation of the CVector class.
//
//////////////////////////////////////////////////////////////////////

#include "Vector.h"
#include "Math.h"
#include "Matrix.h"
#include <cfloat>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVector::CVector()
{
	num=0;
}

CVector::CVector(int n)
{
	num = n;
	vec.resize(num);
}

CVector::CVector(const vector<double> a, int n)
{
	num = n;
	vec = a;
}

CVector::CVector(const double x, int n)
{
	num = n;
	vec.resize(num);
	for (int i=0; i<num; i++) vec[i] = x;
}

CVector::CVector(const double x_min, const double x_max, int n)
{
	num = n+1;
	vec.resize(num);
	for (int i=0; i<num; i++) vec[i] = x_min+(x_max-x_min)/double(n)*i;
}

CVector::CVector(const CVector &v)

{
	num = v.num;
	vec = v.vec;
}

CVector::CVector(const vector<double> &v)
{
	num = v.size();
	vec = v;
}

CVector::CVector(const vector<int> &v)
{
	num = v.size();
	vec.resize(num);
	for (int i=0; i<num; i++)
		vec[i] = v[i];
}

CVector::~CVector()
{
	vec.clear();
}

double& CVector::operator[](int i)
{
	double *p = 0;
	if ((i<num) & (i>-1))
		return vec[i];
	else
		return *p;
}

int CVector::range(int i)
{
	return i;
}

CVector& CVector::operator=(const CVector &v)
{
	num = v.num;
	vec = v.vec;
	return *this;
}

CVector& CVector::operator=(const vector<double> &v)
{
	num = v.size();
	vec = v;
	return *this;
}

CVector& CVector::operator=(const double &v)
{
	for (int i=0; i<num; ++i)
		vec[i] = v;
	return *this;
}

CVector& CVector::operator+() 
{return *this;}

void CVector::swap(int i, int j)
{	double tmp = vec[range(i)];
	vec[i] = vec[range(j)];
	vec[j] = tmp;
	
}

int CVector::getsize() {return num;}

CVector& CVector::operator*=(double x)
{
	for (int i=0; i<num; ++i)
		vec[i] *= x;
	return *this;

}

CVector& CVector::operator/=(double x)
{
	for (int i=0; i<num; ++i)
		vec[i] /= x;
	return *this;

}

CVector& CVector::operator+=(const CVector &v)
{
	for (int i=0; i<num; ++i)
		vec[i] += v.vec[i];
	return *this;
}

CVector& CVector::operator-=(const CVector &v)
{
	for (int i=0; i<num; ++i)
		vec[i] -= v.vec[i];
	return *this;
}

CVector operator+(const CVector &v1, const CVector &v2) 
{
	CVector v=v1; 
	for (int i=0; i<v1.num; i++) v[i]=v1.vec[i]+v2.vec[i];
	return v;
}

CVector operator-(const CVector &v1, const CVector &v2) 
{
	CVector v=v1; 
	for (int i=0; i<v1.num; i++) v[i]=v1.vec[i]-v2.vec[i];
	return v;

}

double dotproduct(CVector v1, CVector v2) 
{
	double d;
	if (v1.num = v2.num) 
	{
	d = 0;
	for (int i=0; i<v1.num; ++i)
		d += v1.vec[i]*v2.vec[i];
	return d;
	}
}

CVector& CVector::operator*=(const CVector& v)
{
	for (int i=0; i<num; ++i)
		vec[i] *= v.vec[i];
	return *this;
}
	

CVector operator*(CVector v1, CVector v2) 
{
	return v1 *= v2;
}

double norm(CVector v)
{
	double sum = 0;
	for (int i=0; i<v.num; i++)
		sum += pow(v.vec[i],2);
	return sqrt(sum);
}

CVector operator*(double a, CVector v)
{
	return v*=a;

}

CVector operator/(CVector v, double a)
{
	return v*=(1/a);
}

CVector operator+(CVector v, double a)
{
	CVector v1(v.num);
	for (int i=0; i<v.num; i++)
		v1[i] = a + v[i];
	return v1;
}

CVector operator+(double a, CVector v)
{
	CVector v1(v.num);
	for (int i=0; i<v.num; i++)
		v1[i] = a + v[i];
	return v1;
}

CVector operator-(double a, CVector &v)
{
	CVector v1(v.num);
	for (int i=0; i<v.num; i++)
		v1[i] = a - v[i];
	return v1;

}


CVector operator/(CVector v1, CVector v2)
{
	CVector x(v1.getsize());
	for (int i = 0; i<v1.getsize(); ++i)
		x[i] = v1[i]/v2[i];
	return x;
}

CVector operator/(double a, CVector v2)
{
	CVector x(v2.getsize());
	for (int i = 0; i<v2.getsize(); ++i)
		x[i] = a/v2[i];
	return x;

}

bool CVector::operator==(double v)
{
	bool r=true;
	for (int i=0; i<num; ++i)
		if (vec[i] != v)
			r=false;
	return r;

}

bool CVector::operator==(CVector &v)
{
	bool r=true;
	if (num!=v.num) return false;
	for (int i=0; i<num; ++i)
		if ((vec[i]== v[i])!=true)
			r=false;
	return r;

}

bool CVector::is_finite()
{
	bool r=true;
	for (int i=0; i<num; ++i)
		if (isfinite(vec[i])!=true)
			r=false;
	return r;
}

double CVector::max()
{
	double a = -1E14;
	for (int i=0;i<num; i++)
	{
		if (vec[i]>a)
			a = vec[i];
	}
	return a;

}

double max(CVector &V)
{	
	return V.max();
}

double CVector::min()
{
	double a = 1E14;
	for (int i=0;i<num; i++)
	{
		if (vec[i]<a)
			a = vec[i];
	}
	return a;

}

double min(CVector &V)
{	
	return V.min();
}
double CVector::abs_max()
{
	double a = -1E14;
	for (int i=0;i<num; i++)
	{
		if (fabs(vec[i])>a)
			a = fabs(vec[i]);
	}
	return a;
}

double abs_max(CVector &V)
{	
	return V.abs_max();
}

double CVector::norm2()
{
	double a = 0;
	for (int i=0;i<num; i++)
	{
		a+=vec[i]*vec[i];
	}
	return pow(a,0.5);

}

double CVector::sum()
{
		double a = 0;
	for (int i=0;i<num; i++)
	{
		a+=vec[i];
	}
	return a;
}

CMatrix CVector::T()
{
	CMatrix K(1,num);
	for (int i=0; i<num; i++)
		K[0][i] = vec[i];
	return K;
}

CVector CVector::Log()
{
	CVector x(getsize());
	for (int i = 0; i<getsize(); i++)
		x[i] = log(vec[i]);
	return x;
}

CVector Log(CVector &V)
{
	return V.Log();

}

double avg(CVector &V)
{
	return V.sum()/V.num;
}

CVector CVector::Exp()
{
	CVector x(getsize());
	for (int i = 0; i<getsize(); i++)
		x[i] = exp(vec[i]);
	return x;
}

vector<int> CVector::Int()
{
	vector<int> x(getsize());
	for (int i = 0; i<getsize(); i++)
		x[i] = int(vec[i]);
	return x;
}

CVector CVector::abs()
{
	CVector x(getsize());
	for (int i = 0; i<getsize(); i++)
		x[i] = fabs(vec[i]);
	return x;
}


CVector Exp(CVector &V)
{
	return V.Exp();

}

CVector abs(CVector &V)
{
	return V.abs();
}

void CVector::writetofile(FILE *f)
{
	for (int i=0; i<num; i++)
		fprintf(f, "%le, ", vec[i]);
	fprintf(f, "\n");
}

void CVector::writetofile(ofstream &f)
{
	for (int i=0; i<num-1; i++)
		f<<vec[i]<<",";
	f<<vec[num-1]<<endl;
		
}

void CVector::writetofile(string filename)
{
	FILE *f = fopen(filename.c_str(),"w");
	writetofile(f);
	fclose(f);
}

void CVector::writetofile_app(string filename)
{
	FILE *f = fopen(filename.c_str(),"a");
	writetofile(f);
	fclose(f);
}

CMatrix CVector::diagmat()
{
	CMatrix A(num,num);
	for (int i=0; i<num; i++)
		A[i][i] = vec[i];

	return A;

}

CVector zeros(int i)
{
	CVector V(i);
	return V;
}

CVector CVector::append(const CVector& V1)
{
	for (int i=0; i<V1.num; i++)
		vec.push_back(V1.vec[i]);
	num += V1.num;
	return *this;
}

CVector CVector::append(double d)
{
	vec.push_back(d);
	num += 1;
	return *this;
}

CVector CVector::sort()
{
	vec = QSort(vec);
	return *this;
}

CVector combinesort(const CVector& V1, const CVector &V2)
{
	CVector V3 = V1;
	CVector V = V3.append(V2);
	return V.sort();

}

CVector combinesort_s(const CVector& V1, const CVector &V2)
{
	int n1=0;
	int n2=0;
	CVector V3;
	for (int i=0; i<V1.num+V2.num; i++)
	{
		if (n2==V2.num)
		{	V3.append(V1.vec[n1]);
			n1++;
		}
		else if (n1==V1.num)
		{	V3.append(V2.vec[n2]);
			n2++;
		}
		else
		{
			if (V1.vec[n1]>V2.vec[n2])
			{	V3.append(V2.vec[n2]);
				n2++;
			}
			else
			{	V3.append(V1.vec[n1]);
				n1++;
			}
		}
	}
	
	return V3;

}

int lookup(vector<int> v, int val)
{
	int res = -1;
	for (int i=0; i<v.size(); i++)
		if (v[i] == val)
			res = i;

	return res;

}

int lookup(vector<string> v, string val)
{
	int res = -1;
	for (int i=0; i<v.size(); i++)
		if (v[i] == val)
			res = i;

	return res;

}

int lookup(vector<double> v, double val)
{
	int res = -1;
	for (int i=0; i<v.size(); i++)
		if (v[i] == val)
			res = i;
	return res;
}

vector<int> CVector::lookup(double val)
{
	vector<int> res;
	for (int i=0; i<num; i++)
		if (vec[i] == val)
			res.push_back(i);
	return res;
}

CVector H(CVector &V)
{
	CVector x(V.num);
	for (int i = 0; i<V.num; i++)
		x[i] = H(V[i]);
	return x;

}

double H(double x)
{
	if (x>0) return x; else return 1e-25;

}

vector<double> H(vector<double> x)
{
	vector<double> X(x.size());
	for (int i=0; i<x.size(); i++)
		X[i] = H(x[i]);

	return X;
}

void CVector::print(string s)
{		
	ofstream Afile;
	Afile.open(s);	

	for (int i=0; i<num; ++i)
		Afile << vec[i] << endl;

	Afile.close();							
	
}

CVector CVector::operator=(mat A)
{
	num = A.n_rows;
	vec.resize(num);
		
	for (int i = 0; i<num; ++i)		
			vec[i]=A(i,0);
	
	return *this;
}

CVector CVector::sub(int i, int j)
{
	CVector C(j-i);
	for (int ii=i; ii<j; ii++)
		C[ii-i] = vec[ii];

	return C;

}
//mat CVector::operator=(const CVector&V)
//{
//	mat A(num,1);
		
//	for (int i = 0; i<num; ++i)		
//			A(i,0) = vec[i];
	
//	return A;
//}

vector<double> create_vector(int i)
{
	vector<double> X(i);
	return X;

}

vector<vector<double>> create_vector(int i, int j)
{
	vector<vector<double>> X(i);
	for (int ii=0; ii<i; i++)
		X[i].resize(j);
	
	return X;
	
}

