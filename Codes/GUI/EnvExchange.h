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
	CEnvExchange& CEnvExchange::operator=(const CEnvExchange &BB);
	CStringOP expression;
	string model;
	string constituent;
	string phase;
	string solid;
	vector<double> parameters;
	double CEnvExchange::calculate(CMBBlock *Block);
	double CEnvExchange::calculate_star(CMBBlock *Block);
	void CEnvExchange::set_val(string S, double val);
	void CEnvExchange::set_val(int i, double val);
	string name;
};

