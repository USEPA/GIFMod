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
    CBuildup& operator=(const CBuildup &BB);
	~CBuildup(void);
	string model;
	string constituent;
	string phase;
	string solid;
	vector<double> parameters;
	double buildup(double C, CMBBlock *Block);
	void set_val(string S, double val);
	void set_val(int i, double val);
	string name;

};

