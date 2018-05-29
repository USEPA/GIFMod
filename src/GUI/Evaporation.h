
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
    CEvaporation& operator=(const CEvaporation &BB);
	CStringOP expression;
	string model;
	vector<double> parameters;
    double calculate(CMBBlock *Block);
    double calculate_star(CMBBlock *Block);
    void set_val(string S, double val);
    void set_val(int i, double val);
	string name;
	bool uptake;
	CBTC evaporation_TS; 
	CBTC single_crop_coefficient; 
	string evaporation_filename;
	string single_crop_coefficient_filename;
};

