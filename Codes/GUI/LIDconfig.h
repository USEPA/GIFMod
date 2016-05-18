#pragma once
#include <vector>
#include <iostream>
#include "string.h"
#include "StringOP.h"

using namespace std;

class CLIDconfig
{
public:
	CLIDconfig(void);
	CLIDconfig(string filename);
	~CLIDconfig(void);
	CLIDconfig& CLIDconfig::operator = (const CLIDconfig &CC);
	CLIDconfig(const CLIDconfig &CC);
	vector<string> keyword;
	vector<string> value;
	vector<vector<string>> param_names;
	vector<vector<string>> param_vals;
	vector<vector<string>> est_param; 
	vector<string> params_string;
	void CLIDconfig::getconfigfromfile(string filename);
	void CLIDconfig::set_experiment_ids();
	int lookupkeyword(string key);
	vector<vector<string>> experiment_id;
	vector<string> experiment_id_keyword;
	CLIDconfig CLIDconfig::extract_subset(int i);
	vector<CLIDconfig> CLIDconfig::extract_subsets();
	vector<string> get_experiment_names();
	int lookupexperiments(string s);
	vector<string> experiment_names;

	
};

vector<string> extracts_experiment_id(string s);

