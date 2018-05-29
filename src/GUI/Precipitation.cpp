
#include "Precipitation.h"
#include <iostream>
#include <fstream>
#include <string>
#include "BTC.h"
#include "StringOP.h"

using namespace std;

CPrecipitation::CPrecipitation(void)
{
}

CPrecipitation::CPrecipitation(int nn)
{
	n = nn;
	s.resize(n);
	e.resize(n);
	i.resize(n);
}

CPrecipitation::CPrecipitation(const CPrecipitation &Precip)
{
	n = Precip.n;
	s = Precip.s;
	e = Precip.e;
	i = Precip.i;


}

CPrecipitation CPrecipitation::operator = (const CPrecipitation &Precip)
{
	n = Precip.n;
	s = Precip.s;
	e = Precip.e;
	i = Precip.i;
	return *this;

}

CPrecipitation::~CPrecipitation(void)
{

}

double CPrecipitation::getval(double time)
{

	structured = false;
	double res = 0;
	if (!structured)
	{
		for (int ii=0; ii<n; ii++)
			if ((time<e[ii]) && (time>=s[ii]))
				res = i[ii]/(e[ii]-s[ii]);
	}
	else
	{
		int ii = static_cast<int>((time-s[0])/dt);
		if (ii<n)
			res = i[ii]/(e[ii]-s[ii]);
		else
			res = 0;

	}
	return res;
}

CPrecipitation::CPrecipitation(string filename)
{
	ifstream fil(filename);
	n=0;
	if (!fil.good()) return;
	vector<string> ss;
	s.clear(); e.clear(); i.clear();

	while (fil.eof()==false)
	{	ss = getline(fil);

		if (ss.size()>=3)
			if (ss[0].substr(0, 2) != "//")
			{
				s.push_back(atof(ss[0].c_str()));
				e.push_back(atof(ss[1].c_str()));
				i.push_back(atof(ss[2].c_str()));
				n++;
			}
	}
	dt = e[1] - s[1];

}

bool CPrecipitation::isFileValid(string filename)
{
	CPrecipitation temp;
	ifstream fil(filename);
	temp.n = 0;
	if (!fil.good()) return false;
	vector<string> ss;
	temp.s.clear(); temp.e.clear(); temp.i.clear();

	while (fil.eof() == false)
	{
		ss = getline(fil);

		if (ss.size() >= 3)
			if (ss[0].substr(0, 2) != "//")
			{
				temp.s.push_back(atof(ss[0].c_str()));
				temp.e.push_back(atof(ss[1].c_str()));
				temp.i.push_back(atof(ss[2].c_str()));
				temp.n++;
			}
	}
	if (!temp.n)
		return false;
	if (temp.e.size() < 1 || temp.s.size() < 1)
		return false;
	for (unsigned int i = 0; i < temp.e.size(); i++)
		if (temp.e[i] - temp.s[i] <= 0)
			return false;
//	temp.dt = temp.e[1] - temp.s[1];
//	if (!temp.dt)
//		return false;
	return true;
}

void CPrecipitation::getfromfile(string filename)
{
	ifstream fil(filename);
	vector<string> ss;
	s.clear(); e.clear(); i.clear();
	n=0;
	while (fil.eof()==false)
	{	ss = getline(fil);
		if (ss.size()>=3)
		{	s.push_back(atof(ss[0].c_str()));
			e.push_back(atof(ss[1].c_str()));
			i.push_back(atof(ss[2].c_str()));
			n++;
		}
	}
	dt = e[1] - s[1];
}


CBTCSet CPrecipitation::getflow (double A, double dt)
{
	CBTCSet Rainflowout(1);
	if (n == 0) return Rainflowout;
	Rainflowout.names.clear();
	Rainflowout.names.push_back("flow");
	for (double t = s[0]; t<e[n-1]; t+=dt)
		Rainflowout.BTC[0].append(t,getval(t)*A);  //i [m]

	Rainflowout.BTC[0].assign_D();
	return Rainflowout;
}

CBTCSet CPrecipitation::getflow(double A)
{
	CBTCSet Rainflowout(1);
	Rainflowout.names.clear();
	Rainflowout.names.push_back("flow");
	for (int ii = 0; ii<n; ii++)
		Rainflowout.BTC[0].append((e[ii] + s[ii])*0.5, i[ii] / (e[ii] - s[ii])*A);  //i [m]

	Rainflowout.BTC[0].assign_D();
	return Rainflowout;
}


