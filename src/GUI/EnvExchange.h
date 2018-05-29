#pragma once
#include <string>
#include <vector>
#include "StringOP.h"

class CMBBlock;

class CEnvExchange
{
public:
	CEnvExchange(void);
	~CEnvExchange(void);
	CEnvExchange(const CEnvExchange &B);
	CEnvExchange(string _model);
    CEnvExchange& operator=(const CEnvExchange &BB);
	CStringOP expression;
	string model;
	string constituent;
	string phase;
	string solid;
	vector<double> parameters;
    double calculate(CMBBlock *Block);
    double calculate_star(CMBBlock *Block);
    void set_val(string S, double val);
    void set_val(int i, double val);
	string name;
};

