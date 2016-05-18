#pragma once
#include <string>
#include <vector>
#include "StringOP.h"

class CMBBlock;

class CBuildup
{
public:
	CBuildup(void);
	CBuildup(string m);
	CBuildup(const CBuildup &B);
	CBuildup& CBuildup::operator=(const CBuildup &BB);
	~CBuildup(void);
	string model;
	string constituent;
	string phase;
	string solid;
	vector<double> parameters;
	double CBuildup::buildup(double C, CMBBlock *Block);
	void CBuildup::set_val(string S, double val);
	void CBuildup::set_val(int i, double val);
	string name;

};

