#include "BTCSet.h"
#include "string.h"
#include <fstream>
#include "StringOP.h"
#include "DistributionNUnif.h"
#include <iostream>
#include "Vector.h"
#include "Matrix.h"
#include "qdebug.h"
using namespace std;

CBTCSet::CBTCSet(void)
{
	nvars = 0;
	BTC.resize(nvars);
	unif = true;
}

CBTCSet::~CBTCSet(void)
{

}

CBTCSet::CBTCSet(int n)
{
	nvars = n;
	BTC.resize(nvars);
	names.resize(nvars);
	for (int i=0; i<nvars; i++) BTC[i] = CBTC();
	unif = true;
}
CBTCSet::CBTCSet(vector<vector<double>> &data, int writeInterval) :CBTCSet(data[0].size())
{
	for (int i = 0; i<data.size(); i++)
		if (i%writeInterval == 0) append(i, data[i]);
}
CBTCSet::CBTCSet(int numberofBTCs, int sizeofBTCvector)
{
	nvars = numberofBTCs;
	BTC.resize(nvars);
	names.resize(nvars);
	for (int i = 0; i<nvars; i++) 
		BTC[i] = CBTC(sizeofBTCvector);
	unif = true;
}

CBTCSet merge(CBTCSet A, CBTCSet &B)
{
	CBTCSet C = A;
	for (int i=0; i<B.nvars; i++)
	{	if (B.names.size()>i) C.names.push_back(B.names[i]);
		C.BTC.push_back(B.BTC[i]);
		C.nvars++;
	}
	return C;
}

CBTCSet merge(vector<CBTCSet> &A)
{
	CBTCSet C = A[0];
	for (int i=1; i<A.size(); i++)
		C = merge(C,A[i]);
	return C;
}

void CBTCSet::writetofile(char outputfile[])
{
	FILE *Fil;
	Fil = fopen(outputfile, "w");
	for (int i=0; i<names.size(); i++)
		fprintf(Fil , "t, %s, ", names[i].c_str());
	fprintf(Fil, "\n"); 
	for (int j=0; j<maxnumpoints(); j++)
	{
		for (int i=0; i<nvars; i++)
		{
			//MM
			//if (i<BTC[i].n)
			if (i<BTC[i].n+1)
				fprintf(Fil, "%lf, %le,", BTC[i].t[j], BTC[i].C[j]);
			else
				fprintf(Fil, ", ,");

		}
		fprintf(Fil, "\n");
	}

	fclose(Fil);

}

void CBTCSet::writetofile(string outputfile)
{
	FILE *Fil;
	Fil = fopen(outputfile.c_str() , "w");
	fprintf(Fil , "//");
	for (int i=0; i<names.size(); i++)
		fprintf(Fil , "t, %s, ", names[i].c_str());
	fprintf(Fil, "\n"); 
	for (int j=0; j<maxnumpoints(); j++)
	{
		for (int i=0; i<nvars; i++)
		{
			if (j<BTC[i].n)
				fprintf(Fil, "%lf, %le,", BTC[i].t[j], BTC[i].C[j]);
			else
				fprintf(Fil, ", ,");

		}
		fprintf(Fil, "\n");
	}

	fclose(Fil);

}

void CBTCSet::writetofile(string outputfile, int outputwriteinterval)
{
	FILE *Fil;
	Fil = fopen(outputfile.c_str() , "w");
	for (int i=0; i<names.size(); i++)
		fprintf(Fil , "t, %s, ", names[i].c_str());
	fprintf(Fil, "\n"); 
	for (int j=0; j<maxnumpoints(); j++)
	{
		for (int i=0; i<nvars; i++)
		{
			if (j%outputwriteinterval==0)
				if (j<BTC[i].n)
					fprintf(Fil, "%lf, %le,", BTC[i].t[j], BTC[i].C[j]);
				else
					fprintf(Fil, ", ,");

		}
		if (j%outputwriteinterval==0)
			fprintf(Fil, "\n");
	}

	fclose(Fil);

}

int CBTCSet::maxnumpoints()
{
	int m = 0;
	for (int i=0; i<nvars; i++)
		if (BTC[i].n>m) m = BTC[i].n;
	
	return m;
}

CBTCSet::CBTCSet(const CBTCSet &B)
{
	nvars = B.nvars;
	BTC.resize(nvars);
	names = B.names;
	BTC = B.BTC;
	unif = B.unif;

}

CBTCSet::CBTCSet(const CBTC &B)
{
	nvars = 1;
	BTC.resize(1);
	
	BTC[0] = B; 
}

CBTCSet& CBTCSet::operator = (const CBTCSet &B)
{
	nvars = B.nvars;
	BTC.resize(nvars);
	names = B.names;
	for (int i=0; i<nvars; i++)
		BTC[i] = B.BTC[i]; 
	unif = B.unif;
	return *this;

}

vector<double> CBTCSet::interpolate(double t)
{
	vector<double> out;
	out.resize(nvars); 
	for (int i=0; i<nvars; i++) 
		out[i] = BTC[i].interpol(t);

	return out;
}

vector<double> CBTCSet::interpolate(double t, int fnvars)
{
	vector<double> out;
	out.resize(fnvars); 
	for (int i=0; i<min(nvars,fnvars); i++) 
		out[i] = BTC[i].interpol(t);

	return out;
}

CBTCSet::CBTCSet(string filename, bool varytime)
{
	unif = false;
	vector<string> units;
	ifstream file(filename);
	vector<string> s;
	nvars = 0;
	if (file.good() == false)
	{
		file_not_found = true;
		return;
	}
	if (varytime == false)
		while (file.eof() == false)
		{
			s = getline(file);
			if (s.size()>0)
			{
				if (s[0] == "names")
					for (int i = 1; i < s.size(); i++) names.push_back(s[i]);
				if (s[0] == "units")
					for (int i = 1; i < s.size(); i++) units.push_back(s[i]);
				if ((s[0].substr(0, 2) != "//") && (s[0] != "names") && (s[0] != "units"))
				{
					if (nvars == 0) { nvars = s.size() - 1; BTC.resize(nvars); }
					if (s.size() == nvars + 1)
						for (int i = 0; i < nvars; i++)
						{
							BTC[i].t.push_back(atof(s[0].c_str()));
							BTC[i].C.push_back(atof(s[i + 1].c_str()));
							BTC[i].n++;
							if (BTC[i].t.size()>2)
								if ((BTC[i].t[BTC[i].t.size() - 1] - BTC[i].t[BTC[i].t.size() - 2]) != (BTC[i].t[BTC[i].t.size() - 2] - BTC[i].t[BTC[i].t.size() - 3]))
									BTC[i].structured = false;

						}

				}
			}
		}
	else
		while (file.eof() == false)
		{
			s = getline(file);
			if (s.size() > 0)
			{
				if (s[0] == "names")
					for (int i = 1; i < s.size(); i++) names.push_back(s[i]);
				if (s[0] == "units")
					for (int i = 1; i < s.size(); i++) units.push_back(s[i]);
				if ((s[0].substr(0, 2) != "//") && (s[0] != "names") && (s[0] != "units"))
				{
					if (nvars == 0) { nvars = s.size() / 2; BTC.resize(nvars); }

					for (int i = 0; i < nvars; i++)
					{
						if (s.size() >= 2 * (i + 1))
							if ((trim(s[2 * i]) != "") && (trim(s[2 * i + 1]) != ""))
							{
								BTC[i].t.push_back(atof(s[2 * i].c_str()));
								BTC[i].C.push_back(atof(s[2 * i + 1].c_str()));
								BTC[i].n++;
								if (BTC[i].t.size()>2)
									if ((BTC[i].t[BTC[i].t.size() - 1] - BTC[i].t[BTC[i].t.size() - 2]) != (BTC[i].t[BTC[i].t.size() - 2] - BTC[i].t[BTC[i].t.size() - 3]))
										BTC[i].structured = false;
							}
					}
				}
			}
		}
	file.close();

	for (int i = 0; i < min(int(names.size()), nvars); i++)
		BTC[i].name = names[i];

	for (int i = 0; i < min(int(units.size()), nvars); i++)
		BTC[i].unit = units[i];

	//for (int i=0; i<nvars; i++)
	//	BTC[i].assign_D();

	if (names.size() < nvars)
	{
		names.resize(nvars);
	}
	if (nvars == 1 && names[0] == "")
		names[0] = "Data";
	if (nvars > 1)
		for (int i = 0; i < nvars; i++)
			if (names[i] == "")
				names[i] = "Data (" + i + ')';
}




void CBTCSet::getfromfile(string filename, bool varytime)
{
	unif = false;
	vector<string> units;
	ifstream file(filename);
	vector<string> s;
	nvars = 0;
	if (varytime==false)
		while (file.eof()== false)
		{
			s = getline(file);
			if (s.size()>0)
			{ 
				if (s[0] == "names")
					for (int i = 1; i < s.size(); i++) names.push_back(s[i]);
				if (s[0] == "units")
					for (int i = 1; i < s.size(); i++) units.push_back(s[i]);
				if ((s[0].substr(0, 2) != "//") && (s[0] != "names") && (s[0] != "units"))
				{
					if (nvars==0) {nvars = s.size()-1; BTC.resize(nvars); for (int i=0; i<nvars; i++) BTC[i].structured = true;}
					if (s.size()==nvars+1)
						for (int i=0; i<nvars; i++)
						{
							BTC[i].t.push_back(atof(s[0].c_str()));
							BTC[i].C.push_back(atof(s[i+1].c_str()));
							BTC[i].n++;
							if (BTC[i].n>2)
									if ((BTC[i].t[BTC[i].n-1]-BTC[i].t[BTC[i].n-2]) != (BTC[i].t[BTC[i].n-2]-BTC[i].t[BTC[i].n-3]))
										BTC[i].structured = false;
						}
				
				}
			}
		}
	else
		while (file.eof()== false)
		{
			s = getline(file);
			if (s.size() > 0)
			{
				if (s[0] == "names")
					for (int i = 1; i < s.size(); i++) names.push_back(s[i]);
				if (s[0] == "units")
					for (int i = 1; i < s.size(); i++) units.push_back(s[i]);
				if ((s[0].substr(0, 2) != "//") && (s[0] != "names") && (s[0] != "units"))
				{
					if (nvars == 0) { nvars = s.size() / 2; BTC.resize(nvars); for (int i = 0; i < nvars; i++) BTC[i].structured = true; }

					int n_line = s.size() / 2;
					for (int i = 0; i < n_line; i++)
					{
						if ((trim(s[2 * i]) != "") && (trim(s[2 * i + 1]) != ""))
						{
							BTC[i].t.push_back(atof(s[2 * i].c_str()));
							BTC[i].C.push_back(atof(s[2 * i + 1].c_str()));
							BTC[i].n++;
							if (BTC[i].n>2)
								if ((BTC[i].t[BTC[i].n - 1] - BTC[i].t[BTC[i].n - 2]) != (BTC[i].t[BTC[i].n - 2] - BTC[i].t[BTC[i].n - 3]))
									BTC[i].structured = false;
						}
					}
				}
			}
		}
	
	for (int i = 0; i < min(int(names.size()), nvars); i++)
		BTC[i].name = names[i];

	for (int i = 0; i < min(int(units.size()), nvars); i++)
		BTC[i].unit = units[i];

	file.close();
}

double CBTCSet::maxtime()
{
	return BTC[0].t[BTC[0].n-1];

}

double CBTCSet::mintime()
{
	return BTC[0].t[0];

}

double diff(CBTCSet B1, CBTCSet B2)
{
	int sum = 0;
	for (int i=0; i<B1.nvars; i++)
		sum += diff(B1.BTC[i],B2.BTC[i]);

	return sum;

}

CBTCSet operator * (const CBTCSet &BTC, const double &C)
{
	CBTCSet A = BTC;
	A.BTC[0] = A.BTC[0]*C;
	return A;
}

vector<double> CBTCSet::getrandom()
{
	int a = int(GetRndUniF(0,BTC[0].n));
	vector<double> res(nvars);
	for (int i=0; i<nvars; i++)
		res[i] = BTC[i].C[a];

	return res;
}

vector<double> CBTCSet::getrandom(int burnin)
{
	int a = int(GetRndUniF(0,BTC[0].n-burnin));
	vector<double> res(nvars);
	for (int i=0; i<nvars; i++)
		res[i] = BTC[i].C[a+burnin];

	return res;
}


vector<double> CBTCSet::getrow(int a)
{

	vector<double> res(nvars);
	for (int i = 0; i<nvars; i++)
		res[i] = BTC[i].C[a];

	return res;
}

vector<double> CBTCSet::percentile(double x)
{
	vector<double> v;
	for (int i=0; i<nvars; i++)
		v.push_back(BTC[i].percentile(x));

	return v;
}

vector<double> CBTCSet::mean(int limit)
{
	vector<double> v;
	for (int i=0; i<nvars; i++)
		v.push_back(BTC[i].mean(limit));
	return v;

}

vector<double> CBTCSet::std(int limit)
{
	vector<double> v;
	for (int i=0; i<nvars; i++)
		v.push_back(BTC[i].std(limit));
	return v;

}

CMatrix CBTCSet::correlation(int limit, int n)	
{
	CMatrix r_xy(n);

	for (int i=0; i<n; i++)
		for (int j=0; j<=i; j++)
			r_xy[i][j] = R(BTC[i], BTC[j], limit);
	
	return r_xy;

}

vector<double> CBTCSet::average()
{
	vector<double> v;
	for (int i=0; i<nvars; i++)
		v.push_back(BTC[i].average());
	return v;

}

vector<double> CBTCSet::integrate()
{
	vector<double> v;
	for (int i=0; i<nvars; i++)
		v.push_back(BTC[i].integrate());
	return v;

}

vector<double> CBTCSet::percentile(double x, int limit)
{
	vector<double> v;
	for (int i=0; i<nvars; i++)
		v.push_back(BTC[i].percentile(x,limit));

	return v;
}
CBTCSet CBTCSet::sort(int burnOut)
{
	CBTCSet r(nvars);
	if (burnOut < 0)
		burnOut = 0;
	vector<vector<double>> temp;
	temp.resize(nvars);
	vector<double> tempVec;

	int counter = 0;
	clock_t tt0 = clock();

#pragma omp parallel 
	
	#pragma omp for
	for (int i = 0; i < nvars; i++)
	{
		counter++;
		qDebug() << "sorting BTC " << i << "(" << counter << ")";
		clock_t t0 = clock();
//		r.BTC[i].C.resize(BTC[i].n - burnOut);
		tempVec.resize(BTC[i].n - burnOut);
		for (int j = 0; j < tempVec.size(); j++)
			tempVec[j] = BTC[i].C[j + burnOut];

		temp[i] = bubbleSort(tempVec);
//		r.BTC[i].C = QSort(temp);
		clock_t t1 = clock() - t0;
		float run_time = ((float)t1) / CLOCKS_PER_SEC;
		qDebug() << "sorting BTC " << i << " finished in" << run_time << " sec (" << --counter << ")";
	}
	for (int i = 0; i < nvars; i++)
	{
		r.BTC[i].C.resize(BTC[i].n - burnOut);
		r.BTC[i].C = temp[i];
		r.BTC[i].n = temp[i].size();
	}
	clock_t tt1 = clock() - tt0;
	float run_time = ((float)tt1) / CLOCKS_PER_SEC;
	qDebug() << "total time << " << run_time << "sec";

	return r;
}
CBTCSet CBTCSet::distribution(int n_bins, int n_columns, int limit)
{
	qDebug() << "Distribution bins, columns, limit" << n_bins << n_columns << limit;
	CBTCSet A(n_columns);		
	for (int i = 0; i < n_columns; i++)
	{
		A.BTC[i] = BTC[i].distribution(n_bins, limit);
		qDebug() << "BTC[" << i << "] done";
	}

	return A;
}

CVector norm2dif(CBTCSet &A, CBTCSet &B)
{
	CVector res;
	for (int i=0; i<min(A.nvars,B.nvars); i++)		
		res.append(diff_abs(A.BTC[i].Log(1e-5),B.BTC[i].Log(1e-5))/B.BTC[i].n);	    
	
	return res;

}

void CBTCSet::append(double t, vector<double> c)
{	
	for (int i=0; i<min(int(c.size()), nvars); i++)
	{	BTC[i].structured = true;
		BTC[i].append(t,c[i]);
		if (BTC[i].n>2)
			if ((BTC[i].t[BTC[i].n-1]-BTC[i].t[BTC[i].n-2]) != (BTC[i].t[BTC[i].n-2]-BTC[i].t[BTC[i].n-3]))
				BTC[i].structured = false;
	}
}

CBTC CBTCSet::add(vector<int> ii)
{
	CBTC A = BTC[ii[0]];
	A.structured = BTC[ii[0]].structured;
	for (int i=1; i<ii.size(); i++)
	if (unif==false)	
	{	A+=BTC[ii[i]];
		A.structured = (A.structured && BTC[ii[i]].structured);
	}
	else
	{	A%=BTC[ii[i]];
		A.structured = (A.structured && BTC[ii[i]].structured);
	}

	return A;
}

CBTC CBTCSet::add_mult(vector<int> ii, vector<double> mult)
{
	CBTC A;
	if (ii.size()>0)
	{	A = mult[0]*BTC[ii[0]];
		A.structured = BTC[ii[0]].structured;
		for (int i=1; i<ii.size(); i++)
		if (unif==false)	
		{	A+=mult[i]*BTC[ii[i]];
			A.structured = (A.structured && BTC[ii[i]].structured);
		}
		else
		{	A%=mult[i]*BTC[ii[i]];
			A.structured = (A.structured && BTC[ii[i]].structured);
		}
	}
	else
	{
		A.setnumpoints(2);
		A.t[0] = mintime();
		A.t[1] = maxtime();
	}
	return A;
}

CBTC CBTCSet::add_mult(vector<int> ii, CBTCSet &mult)
{
	CBTC A;
	if (ii.size()>0)
	{	A = mult.BTC[0]*BTC[ii[0]];
		A.structured = BTC[ii[0]].structured;
		for (int i=1; i<ii.size(); i++)
		if (unif==false)	
		{	A+=BTC[ii[i]]*mult.BTC[i];
			A.structured = (A.structured && BTC[ii[i]].structured && mult.BTC[i].structured);
		}
		else
		{	A%=BTC[ii[i]]*mult.BTC[i];
			A.structured = (A.structured && BTC[ii[i]].structured && mult.BTC[i].structured);
		}
	}
	else
	{
		A.setnumpoints(2);
		A.t[0] = mintime();
		A.t[1] = maxtime();
	}
	return A;
}


CBTC CBTCSet::divide(int ii, int jj)
{
	CBTC A;
	A.structured = (BTC[ii].structured && BTC[jj].structured);
	if (unif==false)	
		A=BTC[ii]/BTC[jj];
	else
		A=BTC[ii]%BTC[jj];

	return A;


}

CBTCSet CBTCSet::make_uniform(double increment)
{
	if (nvars==0) return CBTCSet();
	CBTCSet out(nvars);
	out.names = names;
	
	if (unif == true)
	{
		for (int i = 0; i < nvars; i++)
		{
			out.BTC[i].append(BTC[i].t[0], BTC[i].C[0]);
			if (BTC[i].D.size() == 0) BTC[i].assign_D();
		}
		for (int i=0; i<BTC[0].n-1; i++)
		{
			int i1 = int((BTC[0].t[i]-BTC[0].t[0])/increment);
			int i2 = int((BTC[0].t[i+1]-BTC[0].t[0])/increment);
			for (int j=i1+1; j<=i2; j++)
			{
				double x = j*increment+BTC[0].t[0];
				for (int k=0; k<nvars; k++)
				{
					double CC = (x-BTC[k].t[i])/(BTC[k].t[i+1]-BTC[k].t[i])*(BTC[k].C[i+1]-BTC[k].C[i])+BTC[k].C[i];
					double DD = (x - BTC[k].t[i]) / (BTC[k].t[i + 1] - BTC[k].t[i])*(BTC[k].D[i + 1] - BTC[k].D[i]) + BTC[k].D[i];
					out.BTC[k].append(x,CC);
					out.BTC[k].D.push_back(DD);
				}
			}
		}
	}
	else
	{
		for (int k=0; k<nvars; k++)
			out.BTC[k] = BTC[k].make_uniform(increment);
	}
	for (int k=0; k<nvars; k++)
		out.BTC[k].structured = true;

	out.unif=true;

	return out;

}

CBTCSet CBTCSet::getpercentiles(vector<double> percents)
{
	CBTCSet X(1+percents.size());
	
	X.names.clear();
	char buffer[33];
	X.setname(0, "Mean");
	for (int j=0; j<percents.size(); j++)
	{
		string Xname = to_string(percents[j]*100)+" %";
		X.setname(j + 1, Xname);
	}
		
	vector<double> XX(1+percents.size());
	vector<double> XX_prc(percents.size());
	
	double meanX;
	for (int i=0; i<BTC[0].n; i++)
	{
		vector<double> x;		
		int count = 0;
		for (int j=0; j<nvars; j++)
			if (i<BTC[j].n)
			{	x.push_back(BTC[j].C[i]);
				count++;
			}

		meanX = CVector(x).sum()/count;

		XX[0] = meanX;
		XX_prc = prcntl(x,percents);
		for (int j=0; j<percents.size(); j++)
			XX[j+1] = XX_prc[j];
		
		X.append(BTC[0].t[i],XX);
	}

	return X;
}

CVector CBTCSet::out_of_limit(double limit)
{
	CVector v(nvars);
	for (int i=0; i<nvars; i++)
	{
		double n_tot = BTC[i].n;
		double n_exceed = 0;
		for (int j=0; j<BTC[i].n; j++)
		{		if (BTC[i].C[j] > limit)
				n_exceed++;		}

		v[i] = n_exceed/n_tot;
	}

	return v;
}

CBTCSet CBTCSet::add_noise(vector<double> std, bool logd)
{
	CBTCSet X(nvars);
	for (int i=0; i<nvars; i++)
		X.BTC[i] = BTC[i].add_noise(std[i],logd);

	return X;
}

CVector sum_interpolate(vector<CBTCSet> &BTC, double t)
{
	if (BTC.size()==0) return CVector(1); 
	CVector sum(max(max_n_vars(BTC),2)); //Be chacked later?
	for (int i=0; i<BTC.size(); i++)   //We can have several BTCs (Precipitation, Evaporation,...) and each one can have several variables (flow, concentration, ...)
	{
		for (int j=0; j<BTC[i].nvars; j++)
			sum[j]+=BTC[i].BTC[j].interpol(t);
	}
	return sum;
}

double sum_interpolate(vector<CBTCSet> &BTC, double t, string name)
{
	if (BTC.size() == 0) return 0;
	double sum=0;
	for (int i = 0; i<BTC.size(); i++)   
	{
		int ii = BTC[i].lookup(name);
		if (ii!=-1)
			sum += BTC[i].BTC[ii].interpol(t);
	}
	return sum;
}

void CBTCSet::clear()
{
	BTC.clear();
	nvars = 0;
}

vector<double> CBTCSet::max_wiggle()
{
	double max_wig=0;
	int wiggle_id=-1;
	for (int i=0; i<nvars; i++)
	{	double a = BTC[i].wiggle();
		if (a>max_wig) wiggle_id = i;
		max_wig = max(max_wig,a);

	}
	vector<double> out;
	out.push_back(max_wig);
	out.push_back(wiggle_id);
	return out;
}

int max_n_vars(vector<CBTCSet> &BTC)
{
	int k = 0;
	for (int i=0; i<BTC.size(); i++)
	{
		if (BTC[i].nvars>k) k = BTC[i].nvars;
	}
	return k;
}

void CBTCSet::knockout(double t)
{
	for (int i=0; i<nvars; i++) BTC[i].knock_out(t);
}

int CBTCSet::lookup(string S)
{
	int out = -1;
	for (int i = 0; i < names.size(); i++)
		if (tolower(S) == tolower(names[i]))
		{
			out = i;
			return out;
		}

	for (int i = 0; i < BTC.size(); i++)
		if (tolower(S) == tolower(BTC[i].name))
		{
			out = i;
			return out;
		}


	return out;
}
CBTC &CBTCSet::operator[](int index)
{
	while (names.size() < index+1)
		names.push_back("");

	if (BTC[index].name == "")
		if (names[index] != "")
			BTC[index].name = names[index];
	if (names[index] == "")
		if (BTC[index].name != "")
			names[index] = BTC[index].name;
	return BTC[index];
}
CBTC &CBTCSet::operator[](string BTCName)
{
	if (lookup(BTCName) != -1)
		return BTC[lookup(BTCName)];
	else
		return CBTC();
}

void CBTCSet::pushBackName(string name)
{
	names.push_back(name);
	BTC[names.size() - 1].name = name;
}
void CBTCSet::append(CBTC &BTC, string name)
{
	if (name != "")
		 BTC.name = name;
	this->BTC.push_back(BTC);
	pushBackName(name);
	nvars = this->BTC.size();
}


void CBTCSet::setname(int index, string name)
{
	while (names.size() < BTC.size())
		names.push_back("");

	names[index] = name;
	BTC[index].name = name;

}