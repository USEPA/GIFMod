
#pragma once
#include <string>
#include <vector>
#include "StringOP.h"
#include "BTC.h"

class CMBBlock;

class CEvaporation
{
public:
	CEvaporation();
	~CEvaporation();
	CEvaporation(const CEvaporation &B);
	CEvaporation(string _model);
	CEvaporation& CEvaporation::operator=(const CEvaporation &BB);
	CStringOP expression;
	string model;
	vector<double> parameters;
	double CEvaporation::calculate(CMBBlock *Block);
	double CEvaporation::calculate_star(CMBBlock *Block);
	void CEvaporation::set_val(string S, double val);
	void CEvaporation::set_val(int i, double val);
	string name;
	bool uptake;
	CBTC evaporation_TS; 
	string evaporation_filename;
};

