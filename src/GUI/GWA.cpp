#ifdef GWA
#include "GWA.h"
#include <string>



CGWA::CGWA(void)
{
}


CGWA::~CGWA(void)
{
}

CGWA& CGWA::operator=(const CGWA &m)
{
	Tracer = m.Tracer;
	Well = m.Well;
	lid_config = m.lid_config;
	parameters = m.parameters;
	params_vals = m.params_vals;
	pathname = m.pathname;
	outpathname = m.outpathname;
	modeled = m.modeled;
	obs_std = m.obs_std;
	PE_info_filename = m.PE_info_filename;
	realizedparamfilename = m.realizedparamfilename;
	measured_quan = m.measured_quan;
	detoutfilename = m.detoutfilename;
	single_vz_delay = m.single_vz_delay;
	project = m.project;
	project_finish = m.project_finish;
	project_start = m.project_start;
	project_interval = m.project_interval;
	projected = m.projected;
	inverse = m.inverse;
	fixed_old_tracer = m.fixed_old_tracer;
	gw = m.gw;
	for (int i = 0; i<Tracer.size(); i++)
		if (Tracer[i].source != "") Tracer[i].SourceTr = &Tracer[lookup_tracers(Tracer[i].source)];
	return *this;

}

CGWA::CGWA(const CGWA &m)
{
	Tracer = m.Tracer;
	Well = m.Well;
	lid_config = m.lid_config;
	parameters = m.parameters;
	params_vals = m.params_vals;
	pathname = m.pathname;
	outpathname = m.outpathname;
	modeled = m.modeled;
	obs_std = m.obs_std;
	PE_info_filename = m.PE_info_filename;
	realizedparamfilename = m.realizedparamfilename;
	measured_quan = m.measured_quan;
	detoutfilename = m.detoutfilename;
	single_vz_delay = m.single_vz_delay;
	project = m.project;
	project_finish = m.project_finish;
	project_start = m.project_start;
	projected = m.projected;
	project_interval = m.project_interval;
	inverse = m.inverse;
	fixed_old_tracer = m.fixed_old_tracer;
	gw = m.gw; 
	for (int i = 0; i<Tracer.size(); i++)
		if (Tracer[i].source != "") Tracer[i].SourceTr = &Tracer[lookup_tracers(Tracer[i].source)];
}

CGWA::CGWA(string filename)
{
	inverse = false;
	getconfigfromfile(filename);   //reads input file and creates lid_config
	load_settings();
	load_parameters();
	load_tracers();
	load_wells();
	load_observed();

	vector<int> stds;

	for (int i = 0; i<measured_quan.size(); i++)
	{
		if (lookup(stds, measured_quan[i].std_no) == -1)
		{
			stds.push_back(measured_quan[i].std_no);
			measured_quan[i].std_to_param = int(parameters.size());
			range P;

			P.fixed = false;
			P.log = true;
			P.applytoall = true;
			P.tempcorr = 1;
			P.name = "std_" + numbertostring(i);
			double min = measured_quan[i].minstdev;
			for (int j = 0; i<measured_quan.size(); j++)
				if (min < measured_quan[j].minstdev)
					min = measured_quan[j].minstdev;

			P.low = max(min, exp(-4)); P.high = exp(4);
			parameters.push_back(P);
			params_vals.push_back(sqrt(P.low*P.high));

		}
	}
	for (int i = 0; i < stds.size(); i++)
	{

	}
	obs_std.resize(stds.size());

	set_constant_inputs();

	// maximum number of observed data in each well 
	vector<int> each_well_max_data(Well.size());
	for (int j = 0; j<Well.size(); j++)
	{
		each_well_max_data[j] = 0;
		for (int i = 0; i<measured_quan.size(); i++)
			if (measured_quan[i].id == j)
				each_well_max_data[j] = max(each_well_max_data[j], measured_quan[i].observed_data.n);
		for (int i = 0; i<measured_quan.size(); i++)
			if (measured_quan[i].id == j)
				measured_quan[i].max_data_no = each_well_max_data[j];
	}

}

void CGWA::getconfigfromfile(string filename)
{
	ifstream file(filename);

	bool open_bracket = false;
	vector<char> del2; del2.push_back('{'); del2.push_back('}'); del2.push_back(';'); //del2= { , } , ;
	vector<char> brackets; brackets.push_back('['); brackets.push_back(']'); //brackets= [ , ]
	vector<vector<string>> param_list;
	vector<string> params;
	if (file.good())
		while (file.eof()==false)
		{
			if (open_bracket == false)
			{	vector<string> s = getline(file);  //read a line and split it by ","
			if (s.size()>0)	
			{	vector<string> ss = split(s[0],'=');   //s[0] is separated by "="
			lid_config.keyword.push_back(ss[0]);
			lid_config.value.push_back(ss[1]);
			vector<string> ss2;
			int st_br = 0;
			int en_br = 0;
			if (s.size()>1) st_br = look_up(s[1],'{')[0];
			vector<int> en_br_v;
			if (s.size()>1) en_br_v = look_up(s[1],'}');
			if (s.size()>1)
				if (en_br_v.size()==0) en_br = s[1].size(); else en_br=en_br_v[0];
			if ((s.size()>1) && (s[1].substr(st_br,en_br-st_br)).size()>0)  //Returns a vector with indices of "del" 
			{	ss2 = split(s[1].substr(st_br,en_br-st_br),del2);
				
				for (int i=1; i<ss2.size(); i++) params.push_back(ss2[i]);	
				open_bracket = true;
			}
			if ((s.size()>1) && (look_up(s[1],'}').size()>0))
			{	open_bracket = false;
				param_list.push_back(params);
				params.clear();
			}
			if (((s.size()>1) && (look_up(s[1],'{').size()==0) && (look_up(s[1],'}').size()==0)) || (s.size()==1))
				param_list.push_back(params);    //param_list stores parameters inside the {} and leaves space for other parameters
			}
			}
			else if (open_bracket == true)
			{	vector<string> s = getline(file);
			if (s.size()>0)
			{
				vector<string> ss2;

				ss2 = split(s[0],del2);
				int ii;
				if (ss2[0]=="") ii = 1; else ii=0;
				for (int i=ii; i<ss2.size(); i++) params.push_back(ss2[i]);		

				if (look_up(s[0],'}').size()>0)
				{	open_bracket = false;
					param_list.push_back(params);
					params.clear();
				}	
			}
			}
		}	
		for (int i=0; i<lid_config.keyword.size(); i++)
		{	vector<string> pp_name, pp_val;
		vector<int> param_est_no;
		for (int j=0; j<param_list[i].size(); j++)
		{
			if (param_list[i][j]!="") 
			{
				pp_name.push_back(split(param_list[i][j],'=')[0]);
				if (split(param_list[i][j],'=')[1].substr(0,2)=="p[")   //e.g. theta_r=p[0]
				{
					param_est_no.push_back(atoi(split(split(param_list[i][j],'=')[1],brackets)[1].c_str()));
					vector<int> X;
					pp_val.push_back("0");  
				}
				else
				{
					param_est_no.push_back(-1);
					pp_val.push_back(split(param_list[i][j],'=')[1]);
				}
			}
		}
		lid_config.param_names.push_back(pp_name);
		lid_config.param_vals.push_back(pp_val);
//ask ARASH		lid_config.est_param.push_back(param_est_no);
		}
}

void CGWA::setparams(int param_no, double value)
{
	for (int i = 0; i<parameters[param_no].location.size(); i++)
	{
		if (parameters[param_no].location_type[i] == 1)
			Tracer[lookup_tracers(parameters[param_no].location[i])].set_val(parameters[param_no].quan[i], value);
		if (parameters[param_no].location_type[i] == 0)
			Well[lookup_wells(parameters[param_no].location[i])].set_val(parameters[param_no].quan[i], value);
	}


	for (int i = 0; i<measured_quan.size(); i++)
		if (measured_quan[i].std_to_param == param_no)
			obs_std[measured_quan[i].std_no] = value;

	set_constant_inputs();

}

void CGWA::getmodeled()
{
	modeled = CBTCSet(measured_quan.size());
	//vector<double> young_area;
	double oldest_time = getoldest();
	
	for (int i=0; i<Well.size(); i++)
	{	Well[i].createdistribution(Well[i].params,oldest_time,1000,0.02);
		//young_area.push_back(Well[i].young_age_distribution.integrate());
	}

	for (int i=0; i<measured_quan.size(); i++)
	{
		modeled.setname(i,measured_quan[i].name);
		for (int j=0; j<measured_quan[i].observed_data.n; j++)
			modeled.BTC[i].append(measured_quan[i].observed_data.t[j],Tracer[measured_quan[i].quan].calc_conc(measured_quan[i].observed_data.t[j], Well[measured_quan[i].id].young_age_distribution, Well[measured_quan[i].id].fraction_old,Well[measured_quan[i].id].vz_delay,fixed_old_tracer, Well[measured_quan[i].id].age_old,Well[measured_quan[i].id].fm)); 
	}
	if (project) Do_project();
}

CBTCSet CGWA::Do_project()
{
	projected = CBTCSet(Well.size()*Tracer.size());
	double oldest_time = getoldest();
	for (int i=0; i<Well.size(); i++)
		Well[i].createdistribution(Well[i].params,oldest_time,1000,0.02);
	for (int i=0; i<Well.size(); i++)
		for (int j=0; j<Tracer.size(); j++)	
		{
			projected.names[j + i*Tracer.size()] = Tracer[j].name + "@" + Well[i].name;
			for (int t=project_start; t<project_finish; t+=project_interval)
				projected.BTC[j + i*Tracer.size()].append(t, Tracer[j].calc_conc(t, Well[i].young_age_distribution, Well[i].fraction_old, Well[i].vz_delay, fixed_old_tracer, Well[measured_quan[i].id].age_old, Well[measured_quan[i].id].fm));
		}
	return projected;
}



double CGWA::calc_log_likelihood(int ts)
{
	//MCMC
	getmodeled();
	ANS_obs = modeled;
	ANS = modeled;
	double logp = 0;
	for (int i = 0; i<measured_quan.size(); i++)
	{
		double data_ratio = 1;
		if (measured_quan[i].count_max == true)
			data_ratio = (1 / double(measured_quan[i].observed_data.n));
		//data_ratio = (double(measured_quan[i].max_data_no)/double(measured_quan[i].observed_data.n));

		if (measured_quan[i].detect_limit == true)
		{
			if (measured_quan[i].error_structure == 0)
				logp += -norm2(max(measured_quan[i].observed_data, measured_quan[i].detect_limit_value)>max(modeled.BTC[i], measured_quan[i].detect_limit_value)) / (2 * obs_std[measured_quan[i].std_no] * obs_std[measured_quan[i].std_no]) - log(obs_std[measured_quan[i].std_no])*modeled.BTC[i].n;
			if (measured_quan[i].error_structure == 1)
				logp += -norm2((modeled.BTC[i].Log(measured_quan[i].detect_limit_value)>measured_quan[i].observed_data.Log(measured_quan[i].detect_limit_value))) / (2 * obs_std[measured_quan[i].std_no] * obs_std[measured_quan[i].std_no]) - log(obs_std[measured_quan[i].std_no])*modeled.BTC[i].n;
		}
		else
		{
			if (measured_quan[i].error_structure == 0)
				logp += data_ratio*(-norm2(modeled.BTC[i]>measured_quan[i].observed_data) / (2 * obs_std[measured_quan[i].std_no] * obs_std[measured_quan[i].std_no]) - log(obs_std[measured_quan[i].std_no])*modeled.BTC[i].n);
			if (measured_quan[i].error_structure == 1)
				logp += data_ratio*(-norm2((modeled.BTC[i].Log(1e-8)>measured_quan[i].observed_data.Log(1e-8))) / (2 * obs_std[measured_quan[i].std_no] * obs_std[measured_quan[i].std_no]) - log(obs_std[measured_quan[i].std_no])*modeled.BTC[i].n);
		}

	}

	if ((logp == logp) == false) logp = -30000;
	return logp;

}
double CGWA::getoldest()
{
	double oldest=-10000;
	for (int i=0; i<Tracer.size(); i++)
	{
		if (Tracer[i].input.n>1) oldest = max(Tracer[i].input.t[Tracer[i].input.n-1]-Tracer[i].input.t[0],oldest);
	}
	return oldest;
}

void CGWA::set_constant_inputs()
{
	for (int i=0; i<Tracer.size(); i++)
		if (Tracer[i].constant_input == true) 
		{	Tracer[i].input.clear();
			Tracer[i].input.append(0,Tracer[i].constant_input_val);
			Tracer[i].input.append(100,Tracer[i].constant_input_val);
		}
}
int CGWA::lookup_observation(string S)
{
	int out = -1;
	for (int i = 0; i < measured_quan.size(); i++)
		if (tolower(S) == tolower(measured_quan[i].name))
			out = i;

	return out;
}

int CGWA::lookup_parameters(string S)
{
	int out = -1;
	for (int i = 0; i < parameters.size(); i++)
		if (S == parameters[i].name)
			return i;

	return out;
}

int CGWA::lookup_tracers(string S)
{
	int out = -1;
	for (int i = 0; i < Tracer.size(); i++)
		if (tolower(S) == tolower(Tracer[i].name))
			out = i;

	return out;
}

int CGWA::lookup_wells(string S)
{
	int out = -1;
	for (int i = 0; i < Well.size(); i++)
		if (tolower(S) == tolower(Well[i].name))
			out = i;

	return out;
}
void CGWA::load_parameters()
{
	for (int i = 0; i<lid_config.keyword.size(); i++)  //Engine....
	{
		if (tolower(lid_config.keyword[i]) == "parameter")
		{
			inverse = true;
			range P;
			P.low = 0;
			P.high = 0;
			P.fixed = false;
			P.log = false;
			P.applytoall = true;
			P.tempcorr = 1;
			P.name = lid_config.value[i];

			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				if (tolower(lid_config.param_names[i][j]) == "low") P.low = atof(lid_config.param_vals[i][j].c_str());  //low range
				if (tolower(lid_config.param_names[i][j]) == "high") P.high = atof(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "fixed") P.fixed = atoi(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "applytoall") P.applytoall = atoi(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "log") P.log = atoi(lid_config.param_vals[i][j].c_str());
			}
			parameters.push_back(P);
			if (P.log != 1)
				params_vals.push_back(0.5*(P.low + P.high));
			else
				params_vals.push_back(P.low / fabs(P.low)*sqrt(P.low*P.high));

		}
	}
}

void CGWA::load_wells()
{
	for (int i = 0; i<lid_config.keyword.size(); i++)  //Engine....
	{
		if (tolower(trim(lid_config.keyword[i])) == "well")
		{
			CWell W;
			W.name = lid_config.value[i];
			W.vz_delay = 0;
			W.distribution = lid_config.param_vals[i][lookup(lid_config.param_names[i], "distribution")];
			W.params.resize(W.get_numparams(W.distribution));

			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				W.set_val(lid_config.param_names[i][j], atof(lid_config.param_vals[i][j].c_str()));


				if (lookup_parameters(lid_config.est_param[i][j]) != -1)
				{
					parameters[lookup_parameters(lid_config.est_param[i][j])].location.push_back(W.name);
					parameters[lookup_parameters(lid_config.est_param[i][j])].quan.push_back(lid_config.param_names[i][j]);
					parameters[lookup_parameters(lid_config.est_param[i][j])].location_type.push_back(0);
				}
			}

			Well.push_back(W);
		}
	}

}

void CGWA::load_tracers()
{
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if (tolower(trim(lid_config.keyword[i])) == "tracer")
		{
			CTracer T;
			T.co = 0;
			T.source = "";
			T.retard = 1;
			T.input_multiplier = 1;
			T.cm = 0;
			T.linear_prod = false;
			T.name = lid_config.value[i];
			T.vz_delay = false;
			T.constant_input = false;

			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				T.set_val(lid_config.param_names[i][j], atof(lid_config.param_vals[i][j].c_str()));
				if (tolower(lid_config.param_names[i][j]) == "input") T.input = CBTC(pathname + lid_config.param_vals[i][j]);
				if (tolower(lid_config.param_names[i][j]) == "source") T.source = lid_config.param_vals[i][j];
				if (tolower(lid_config.param_names[i][j]) == "linear_prod") T.linear_prod = atoi(lid_config.param_vals[i][j].c_str());


				if (lookup_parameters(lid_config.est_param[i][j]) != -1)
				{
					parameters[lookup_parameters(lid_config.est_param[i][j])].location.push_back(T.name);
					parameters[lookup_parameters(lid_config.est_param[i][j])].quan.push_back(lid_config.param_names[i][j]);
					parameters[lookup_parameters(lid_config.est_param[i][j])].location_type.push_back(1);
				}
			}

			Tracer.push_back(T);
		}
	}
	for (int i = 0; i<Tracer.size(); i++)
		if (Tracer[i].source != "") Tracer[i].SourceTr = &Tracer[lookup_tracers(Tracer[i].source)];
}

void CGWA::load_observed()
{
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if (tolower(lid_config.keyword[i]) == "observed")
		{
			measured_chrc M;
			M.error_structure = 0;
			M.name = lid_config.value[i];
			M.detect_limit = false;
			M.max_data_no = 1;
			M.count_max = 0;
			for (int j = 0; j<lid_config.param_names[i].size(); j++)
			{
				if (tolower(lid_config.param_names[i][j]) == "well") M.id = lookup_wells(lid_config.param_vals[i][j]);
				if (tolower(lid_config.param_names[i][j]) == "count_max") M.count_max = atoi(lid_config.param_vals[i][j].c_str());  //location id
				if (tolower(lid_config.param_names[i][j]) == "tracer") M.quan = lookup_tracers(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "std_no") M.std_no = atoi(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "error_structure") M.error_structure = atoi(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "observed_data")
				{
					M.observed_filename = lid_config.param_vals[i][j];
					M.observed_data = CBTC(pathname + M.observed_filename);
				}
				if (tolower(lid_config.param_names[i][j]) == "detect_limit_value")
				{
					M.detect_limit_value = atof(lid_config.param_vals[i][j].c_str());
					M.detect_limit = true;
				}
			}
			measured_quan.push_back(M);
		}
	}

}

void CGWA::load_settings()
{
	single_vz_delay = false;
	project_finish = 2040;
	project_interval = 1;
	project = false;
	fixed_old_tracer = false;
	for (int i = 0; i<lid_config.keyword.size(); i++)
	{
		if (tolower(lid_config.keyword[i]) == "path") pathname = lid_config.value[i];
		if (tolower(lid_config.keyword[i]) == "outpath") outpathname = lid_config.value[i];
		if (tolower(lid_config.keyword[i]) == "pe_info") PE_info_filename = lid_config.value[i];
		if (tolower(lid_config.keyword[i]) == "detout") detoutfilename = lid_config.value[i];
		if (tolower(lid_config.keyword[i]) == "realizedparam") realizedparamfilename = lid_config.value[i];
		if (tolower(lid_config.keyword[i]) == "single_vz_delay") single_vz_delay = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "fixed_old_tracer") fixed_old_tracer = atoi(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "project_start")
		{
			project = true;
			project_start = atof(lid_config.value[i].c_str());
		}
		if (tolower(lid_config.keyword[i]) == "project_finish") project_finish = atof(lid_config.value[i].c_str());
		if (tolower(lid_config.keyword[i]) == "project_interval") project_interval = atof(lid_config.value[i].c_str());
	}

	if (outpathname == "") outpathname = pathname;
}

double CGWA::doModel()
{
	double likelihood = 0;
	
	for (int ts = 0; ts < n_time_series; ts++)
		likelihood -= calc_log_likelihood();
	
	return likelihood;
}

void CGWA::setparams(vector<double> inp, int n_Params)
{
	
	for (int ts = 0; ts < n_time_series; ts++)
	{
		for (int i = 0; i < n_Params; i++)
			set_param(i, inp[i]);
		finalize_set_param();
		

	}
		
}

#endif