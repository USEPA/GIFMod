#include "stdafx.h"
#include "LIDconfig.h"
#include <iostream>
#include <fstream>


CLIDconfig::CLIDconfig(void)
{
}


CLIDconfig::~CLIDconfig(void)
{
}

CLIDconfig& CLIDconfig::operator = (const CLIDconfig &CC)
{
	keyword = CC.keyword;
	value = CC.value;
	param_names = CC.param_names;
	param_vals = CC.param_vals;
	experiment_id = CC.experiment_id;
	est_param = CC.est_param;
	experiment_names = CC.experiment_names;
	return *this;
}


CLIDconfig::CLIDconfig(const CLIDconfig &CC)
{
	keyword = CC.keyword;
	value = CC.value;
	param_names = CC.param_names;
	param_vals = CC.param_vals;
	experiment_id = CC.experiment_id;
	est_param = CC.est_param;
	experiment_names = CC.experiment_names;
}

CLIDconfig::CLIDconfig(string filename)
{
	getconfigfromfile(filename);
	experiment_names = get_experiment_names();
}

void CLIDconfig::getconfigfromfile(string filename)
{
	
	ifstream file(filename);

	bool open_bracket = false;
	vector<char> del2; del2.push_back('{'); del2.push_back('}'); del2.push_back(';'); //del2= { , } , ;
	vector<char> brackets; brackets.push_back('['); brackets.push_back(']'); //brackets= [ , ]
	vector<char> expbracks; expbracks.push_back('<'); expbracks.push_back('>'); //experiment indicator brackets= < , >
	vector<vector<string>> param_list;
	vector<string> params;
	if (file.good())
		while (file.eof() == false)
		{
			if (open_bracket == false)
			{
				vector<string> s = getline(file);  //read a line and split it by ","
				if (s.size() > 0)
				{
					vector<string> ss = split(s[0], '=');   //s[0] is separated by "="
					if (ss.size() > 1)
					{
						keyword.push_back(ss[0]);
						value.push_back(ss[1]);
						vector<string> ss2;
						int st_br = 0;
						int en_br = 0;
						if (s.size() > 1) st_br = look_up(s[1], '{')[0];
						vector<int> en_br_v;
						if (s.size() > 1) en_br_v = look_up(s[1], '}');
						if (s.size() > 1)
							if (en_br_v.size() == 0) en_br = s[1].size(); else en_br = en_br_v[0];

						if ((s.size() > 1) && (s[1].substr(st_br, en_br - st_br)).size() > 0)  //Returns a vector with indices of "del" 

						{
							ss2 = split(s[1].substr(st_br, en_br - st_br), del2);

							for (int i = 1; i < ss2.size(); i++) params.push_back(ss2[i]);
							open_bracket = true;

						}
						if ((s.size() > 1) && (look_up(s[1], '}').size() > 0))

						{
							open_bracket = false;
							param_list.push_back(params);
							params.clear();
						}
						if (((s.size() > 1) && (look_up(s[1], '{').size() == 0) && (look_up(s[1], '}').size() == 0)) || (s.size() == 1))
							param_list.push_back(params);    //param_list stores parameters inside the {} and leaves space for other parameters
					}
				}
			}
			else if (open_bracket == true)
			{
				vector<string> s = getline(file);
				if (s.size()>0)
				{
					vector<string> ss2;

					ss2 = split(s[0], del2);
					int ii;
					if (ss2[0] == "") ii = 1; else ii = 0;
					for (int i = ii; i<ss2.size(); i++) params.push_back(ss2[i]);

					if (look_up(s[0], '}').size()>0)
					{
						open_bracket = false;
						param_list.push_back(params);
						params.clear();
					}
				}
			}

		}
	for (int i = 0; i<keyword.size(); i++)
	{
		vector<string> pp_name, pp_val;
		vector<string> param_est_no;
		for (int j = 0; j<param_list[i].size(); j++)
		{
			if (param_list[i][j] != "")
			{
				
				pp_name.push_back(split(param_list[i][j], '=')[0]);
				
				
				if (split(param_list[i][j], '=')[1].substr(0, 2) == "p[")   //e.g. theta_r=p[0]
				{
					param_est_no.push_back(split(split(param_list[i][j], '=')[1], brackets)[1]);
					vector<int> X;
					pp_val.push_back("0");
				}
				else
				{
					param_est_no.push_back("");
					pp_val.push_back(split(param_list[i][j], '=')[1]);
				}
			}
		}
		param_names.push_back(pp_name);
		param_vals.push_back(pp_val);
		est_param.push_back(param_est_no);
	}
	set_experiment_ids();
}




int CLIDconfig::lookupkeyword(string key)
{
	for (int i = 0; i < keyword.size(); i++)
		if (tolower(keyword[i]) == tolower(key))
			return i;
	return -1;
}

vector<string> extracts_experiment_id(string s)
{
	bool brack = false;
	vector<string> out(2);
	for (int i = 0; i < s.size(); i++)
	{	if ((s[i] == '<') && (brack == false)) brack = true;
		else if ((s[i] == '>') && (brack == true)) brack = false;
		else if (brack)
			out[1].push_back(s[i]);
		else
			out[0].push_back(s[i]);

	}
	
	return out;
}

CLIDconfig CLIDconfig::extract_subset(int n)
{
	CLIDconfig L;
	for (int i = 0; i< keyword.size(); i++)
	{
		if ((experiment_id_keyword[i] == "") || (lookupexperiments(experiment_id_keyword[i]) == n))
		{
			L.keyword.push_back(keyword[i]);
			L.value.push_back(value[i]);
			vector<string> v;
			L.param_names.push_back(v);
			L.param_vals.push_back(v);
			L.experiment_id.push_back(v);
			L.est_param.push_back(v);
			L.experiment_names = experiment_names;
			for (int j = 0; j < param_names[i].size(); j++)
			{
				if ((lookupexperiments(experiment_id[i][j]) == n) || (experiment_id[i][j] == ""))
				{
					L.param_names[i].push_back(param_names[i][j]);
					L.param_vals[i].push_back(param_vals[i][j]);
					L.experiment_id[i].push_back(experiment_id[i][j]);
					L.est_param[i].push_back(est_param[i][j]);
				}
			}
		}
	}
	
	return L;
}

void CLIDconfig::set_experiment_ids()
{
	for (int i = 0; i< keyword.size(); i++)
	{
		vector<string> k = extracts_experiment_id(keyword[i]);
		if (k[1].size() == 0)
			experiment_id_keyword.push_back("");
		else
			experiment_id_keyword.push_back(k[1]);

		vector<string> x;
		experiment_id.push_back(x);
		for (int j = 0; j < param_names[i].size(); j++)
		{
			vector<string> b = extracts_experiment_id(param_names[i][j]);
			param_names[i][j] = b[0];
			if (b[1].size() != 0)
				experiment_id[i].push_back(b[1]);
			else
				experiment_id[i].push_back("");
		}
	}
}

	



vector<CLIDconfig> CLIDconfig::extract_subsets()
{
	int n_experiments = lookupkeyword("n_experiments");
	vector<string> names = get_experiment_names();
	if (n_experiments == -1) 
		if (names.size()==0)
			n_experiments = 1;
		else
			n_experiments = names.size();
	
	vector<CLIDconfig> L(n_experiments); 

	for (int i = 0; i < n_experiments; i++)
		L[i] = extract_subset(i);

	return L;

}

vector<string> CLIDconfig::get_experiment_names()
{
	vector<string> x;
	for (int i = 0; i < keyword.size(); i++)
		if (tolower(keyword[i]) == "experiment")
			x.push_back(value[i]);

	return x;
}

int CLIDconfig::lookupexperiments(string s)
{
	for (int i = 0; i < experiment_names.size(); i++)
		if (experiment_names[i] == s) return i;
	return -1;
}