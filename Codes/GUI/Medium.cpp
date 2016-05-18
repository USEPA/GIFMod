#ifdef WQV

#include "stdafx.h"
#include "Medium.h"
#include "BTCSet.h"
#include "Precipitation.h"
#include "MediumSet.h"

using namespace std;

CMedium::CMedium(void)
{

}


CMedium::~CMedium(void)
{
}

CMedium::CMedium(const CMedium &M)
{
	parent = M.parent;
	name = M.name;
	counter_colloid = M.counter_colloid;
	counter_flow = M.counter_flow;
	cr = M.cr;
	InvJ1 = M.InvJ1;
	InvJ2 = M.InvJ2;
	failed = M.failed;
	J_update = M.J_update;
	
	t = M.t;
	
	ANS = M.ANS;
	ANS_obs = M.ANS_obs;
	ANS_colloids = M.ANS_colloids;
	ANS_constituents = M.ANS_constituents;
	ANS_obs_noise = M.ANS_obs_noise;
	ANS_MB = M.ANS_MB;


	Blocks = M.Blocks;
	Connector = M.Connector;
	
	lid_config = M.lid_config;
	
	Timemax = M.Timemax;
	Timemin = M.Timemin;
		
	detoutfilename_hydro = M.detoutfilename_hydro;
	detoutfilename_wq = M.detoutfilename_wq;
	detoutfilename_prtcle = M.detoutfilename_prtcle;

	t = M.t;
	Precipitation = M.Precipitation;
	Precipitation_filename = M.Precipitation_filename;

	Solution_dt = M.Solution_dt;
		
	for (int i=0; i<Connector.size(); i++)
	{
		Connector[i].Block1 = &Blocks[getblocksq(Connector[i].Block1ID)];
		Connector[i].Block2 = &Blocks[getblocksq(Connector[i].Block2ID)];
		Connector[i].RXN = &RXN();
	}

	for (int ii=0; ii<Blocks.size(); ii++)
	{	
		if (Blocks[ii].fs_params[5]==0)
			if (Blocks[ii].fs_params[4] != 0) Blocks[ii].fs_params[5]=1-1.0/Blocks[ii].fs_params[4];

		if (Blocks[ii].fs_params[4]==0)
			if (Blocks[ii].fs_params[5] != 0) Blocks[ii].fs_params[4]=1.0/(1.0-Blocks[ii].fs_params[5]);

		Blocks[ii].RXN = &RXN();
	}
	
	epoch_count = M.epoch_count;
	
	
	temperature = M.temperature;
	light = M.light;
	temperature_filename = M.temperature_filename;
	light_filename = M.light_filename;
	current_light = M.current_light;
	current_temperature = M.current_temperature;
	r_humidity = M.r_humidity;
	wind = M.wind;
	r_humidity_filename = M.r_humidity_filename;
	wind_filename = M.wind_filename;
	
	
}

CMedium& CMedium::operator=(const CMedium &M)
{
	parent = M.parent;
	name = M.name;
	counter_colloid = M.counter_colloid;
	counter_flow = M.counter_flow;
	cr = M.cr;
	InvJ1 = M.InvJ1;
	InvJ2 = M.InvJ2;
	
	failed = M.failed;
	J_update = M.J_update;
		
	t = M.t;

	ANS = M.ANS;
	ANS_colloids = M.ANS_colloids;
	ANS_constituents = M.ANS_constituents;
	ANS_obs = M.ANS_obs;
	ANS_obs_noise = M.ANS_obs_noise;
	ANS_MB = M.ANS_MB;
	

	Blocks = M.Blocks;
	Connector = M.Connector;
	
	lid_config = M.lid_config;
	
	Timemax = M.Timemax;
	Timemin = M.Timemin;
		
	detoutfilename_hydro = M.detoutfilename_hydro;
	detoutfilename_wq = M.detoutfilename_wq;
	detoutfilename_prtcle = M.detoutfilename_prtcle;
	
	t = M.t;
	Precipitation = M.Precipitation;
	Precipitation_filename = M.Precipitation_filename;

	
	Solution_dt = M.Solution_dt;

	for (int i = 0; i<Connector.size(); i++)
	{
		Connector[i].Block1 = &Blocks[getblocksq(Connector[i].Block1ID)];
		Connector[i].Block2 = &Blocks[getblocksq(Connector[i].Block2ID)];
		Connector[i].RXN = &RXN();
	}

	for (int ii = 0; ii<Blocks.size(); ii++)
	{
		if (Blocks[ii].fs_params[5] == 0)
			if (Blocks[ii].fs_params[4] != 0) Blocks[ii].fs_params[5] = 1 - 1.0 / Blocks[ii].fs_params[4];

		if (Blocks[ii].fs_params[4] == 0)
			if (Blocks[ii].fs_params[5] != 0) Blocks[ii].fs_params[4] = 1.0 / (1.0 - Blocks[ii].fs_params[5]);

		Blocks[ii].RXN = &RXN();
	}
	
	epoch_count = M.epoch_count;
	

	temperature = M.temperature;
	light = M.light;
	temperature_filename = M.temperature_filename;
	light_filename = M.light_filename;
	r_humidity = M.r_humidity;
	wind = M.wind;
	r_humidity_filename = M.r_humidity_filename;
	wind_filename = M.wind_filename;
		
	return *this;
}  






void CMedium::f_get_environmental_params()
{
	for (int i=0; i<lid_config.keyword.size(); i++)  
	{
		if (tolower(lid_config.keyword[i])=="time_min") {Timemin = atof(lid_config.value[i].c_str()); t=Timemin;}
		if (tolower(lid_config.keyword[i])=="time_max") Timemax = atof(lid_config.value[i].c_str());
		
		if (tolower(lid_config.keyword[i])=="precipitation") Precipitation_filename.push_back(lid_config.value[i]);
		//if (tolower(lid_config.keyword[i])=="evaporation") Evaporation_filename.push_back(lid_config.value[i]); 
		if (tolower(lid_config.keyword[i]) == "temperature") temperature_filename.push_back(lid_config.value[i]);
		if (tolower(lid_config.keyword[i]) == "light") light_filename.push_back(lid_config.value[i]);
		if ((tolower(lid_config.keyword[i]) == "relative_humidity") || (tolower(lid_config.keyword[i]) == "humidity")) r_humidity_filename.push_back(lid_config.value[i]);
		if (tolower(lid_config.keyword[i]) == "wind") wind_filename.push_back(lid_config.value[i]);
		

		if (tolower(lid_config.keyword[i])=="hydro_detout_filename") 
		{   vector<string> names = split_curly_semicolon(lid_config.value[i]);
		for (int ii=0; ii<names.size(); ii++)
			detoutfilename_hydro = names[ii];
		}

		if (tolower(lid_config.keyword[i]) == "wq_detout_filename")
		{
			vector<string> names = split_curly_semicolon(lid_config.value[i]);
			for (int ii = 0; ii<names.size(); ii++)
				detoutfilename_wq = names[ii];
		}

		if ((tolower(lid_config.keyword[i]) == "prtcl_detout_filename") || (tolower(lid_config.keyword[i]) == "particle_detout_filename"))
		{
			vector<string> names = split_curly_semicolon(lid_config.value[i]);
			for (int ii = 0; ii<names.size(); ii++)
				detoutfilename_prtcle = names[ii];
		}
				
	}

	if (detoutfilename_hydro.size() == 0) detoutfilename_hydro = "hydro_output_"+ name +".txt";
	if (detoutfilename_wq.size() == 0) detoutfilename_wq = "wq_output_"+name+".txt";
	if (detoutfilename_prtcle.size() == 0) detoutfilename_prtcle = "prtcl_output_"+name+".txt";
	if (detoutfilename_obs().size() == 0) detoutfilename_obs() = "observed_output.txt";

}

void CMedium::f_get_model_configuration()
{
	for (int i=0; i<lid_config.keyword.size(); i++)  
	{
		if (tolower(trim(lid_config.keyword[i]))=="medium")
		{
			if ((tolower(lid_config.value[i]) == "soil") || (tolower(lid_config.value[i]) == "darcy") || (tolower(lid_config.value[i]) == "stream") || (tolower(lid_config.value[i]) == "catchment"))
			{		
				int n=1;
				double dz = 1;
				double slope = 0;
				double z0 = 0;
				double length = 0;
				vector<int> s_phase_id;
			
				if (lookup(lid_config.param_names[i],"n")!=-1)  
					n = atoi(lid_config.param_vals[i][lookup(lid_config.param_names[i],"n")].c_str());
				if (lookup(lid_config.param_names[i],"dz")!=-1)  
					dz = atof(lid_config.param_vals[i][lookup(lid_config.param_names[i],"dz")].c_str());
				else
					dz=atof(lid_config.param_vals[i][lookup(lid_config.param_names[i],"depth")].c_str())/n;
				
				if (lookup(lid_config.param_names[i], "slope") != -1)
					slope = atof(lid_config.param_vals[i][lookup(lid_config.param_names[i], "slope")].c_str());
				if (lookup(lid_config.param_names[i], "z0") != -1)
					z0 = atof(lid_config.param_vals[i][lookup(lid_config.param_names[i], "z0")].c_str());
				if (lookup(lid_config.param_names[i], "length") != -1)
					length = atof(lid_config.param_vals[i][lookup(lid_config.param_names[i], "length")].c_str());

				if (lookup(lid_config.param_names[i],"id")==-1)  
				{
					if (tolower(lid_config.value[i])=="soil")	
						add_Richards_medium(n,dz);
					else if (tolower(lid_config.value[i])=="darcy")	
						add_Darcy_medium(n,dz);
					else if (tolower(lid_config.value[i]) == "stream")
						add_stream_medium(n, z0, slope, length);
					else if (tolower(lid_config.value[i]) == "catchment")
						add_catchment_medium(n, z0, slope, length);
				}
				else
				{
					if (tolower(lid_config.value[i])=="soil")	
						add_Richards_medium(n,dz,atoi(lid_config.param_vals[i][lookup(lid_config.param_names[i],"id")].c_str()));
					else if (tolower(lid_config.value[i])=="darcy")
						add_Darcy_medium(n,dz,atoi(lid_config.param_vals[i][lookup(lid_config.param_names[i],"id")].c_str()));
					else if (tolower(lid_config.value[i]) == "stream")
						add_stream_medium(n, z0, slope, length,  atoi(lid_config.param_vals[i][lookup(lid_config.param_names[i], "id")].c_str()));
					else if (tolower(lid_config.value[i]) == "catchment")
						add_catchment_medium(n, z0, slope, length, atoi(lid_config.param_vals[i][lookup(lid_config.param_names[i], "id")].c_str()));
				}
				
				for (int j=0; j<lid_config.param_names[i].size(); j++)
				{	set_var(lid_config.param_names[i][j],atof(lid_config.param_vals[i][j].c_str()), n);

					//Following Are OverWritten to the previous set_var
					if (tolower(lid_config.param_names[i][j])=="h_s_expression") for (int ii=Blocks.size()-n; ii<Blocks.size(); ii++) {Blocks[ii].H_S_expression = CStringOP(lid_config.param_vals[i][j]); Blocks[ii].H_S_expression_txt = lid_config.param_vals[i][j];}
					if (tolower(lid_config.param_names[i][j])=="flow_expression") for (int ii=Connector.size()-(n-1); ii<Connector.size(); ii++)
						{Connector[ii].flow_expression = CStringOP(lid_config.param_vals[i][j]); Connector[ii].flow_expression_strng = lid_config.param_vals[i][j];}
					if (tolower(lid_config.param_names[i][j])=="dispersion_expression") for (int ii=Connector.size()-(n-1); ii<Connector.size(); ii++)
						{Connector[ii].dispersion_expression = CStringOP(lid_config.param_vals[i][j]); Connector[ii].dispersion_strng = lid_config.param_vals[i][j];}
					if (tolower(lid_config.param_names[i][j])=="z0") 
					{
						if (lookup(lid_config.param_names[i],"id")==-1)	
							set_z0(atof(lid_config.param_vals[i][j].c_str()));
						else	
							set_z0(Blocks.size()-n, n, atof(lid_config.param_vals[i][j].c_str()), dz);
					}

					if (tolower(lid_config.param_names[i][j])=="inflow") for (int ii=Blocks.size()-n; ii<Blocks.size(); ii++) Blocks[ii].inflow_filename.push_back(lid_config.param_vals[i][j]);
					if ((tolower(lid_config.param_names[i][j])=="solid_phase") || (tolower(lid_config.param_names[i][j])=="particulate_phase"))
					{	for (int ii=Blocks.size()-n; ii<Blocks.size(); ii++) Blocks[ii].Solid_phase_id.push_back(atoi(lid_config.param_vals[i][j].c_str()));
						for (int ii=Connector.size()-(n-1); ii<Connector.size(); ii++) Connector[ii].Solid_phase_id.push_back(atoi(lid_config.param_vals[i][j].c_str()));
					}
					if ((tolower(lid_config.param_names[i][j]) == "externalflux") || (tolower(lid_config.param_names[i][j]) == "external_flux"))
					{
						for (int ii = Blocks.size() - n; ii<Blocks.size(); ii++) Blocks[ii].envexchange_id.push_back(lid_config.param_vals[i][j]);
					}
				}

				if (lookup(lid_config.param_names[i],"V") == -1) 
				{	set_var("v",Blocks[Blocks.size()-n].A*dz, n);

					for (int j=0; j<lid_config.param_names[i].size(); j++)
					{	
						if (tolower(lid_config.param_names[i][j])=="se") set_var("s",Blocks[Blocks.size()-n].V*(atof(lid_config.param_vals[i][j].c_str())*(Blocks[Blocks.size()-n].fs_params[1]-Blocks[Blocks.size()-n].fs_params[2]) + Blocks[Blocks.size()-n].fs_params[2]), n);
						if (tolower(lid_config.param_names[i][j])=="theta") set_var("s",Blocks[Blocks.size()-n].V*atof(lid_config.param_vals[i][j].c_str()), n);
					}
				}

				for (int ii=0; ii<lid_config.est_param[i].size(); ii++)
				{
					for (int iii=Blocks.size()-n; iii<Blocks.size(); iii++)
						if (lookup_parameters(lid_config.est_param[i][ii]) != -1) {
							parameters()[lookup_parameters(lid_config.est_param[i][ii])].location.push_back(iii);
							parameters()[lookup_parameters(lid_config.est_param[i][ii])].quan.push_back(lid_config.param_names[i][ii]);
							parameters()[lookup_parameters(lid_config.est_param[i][ii])].location_type.push_back(0);
							parameters()[lookup_parameters(lid_config.est_param[i][ii])].experiment_id.push_back(name);
						}

					for (int iii=Connector.size()-n+1; iii<Connector.size(); iii++)
						if (lookup_parameters(lid_config.est_param[i][ii]) != -1) {
							parameters()[lookup_parameters(lid_config.est_param[i][ii])].location.push_back(iii);
							parameters()[lookup_parameters(lid_config.est_param[i][ii])].quan.push_back(lid_config.param_names[i][ii]);
							parameters()[lookup_parameters(lid_config.est_param[i][ii])].location_type.push_back(1);
							parameters()[lookup_parameters(lid_config.est_param[i][ii])].experiment_id.push_back(name);
						}
				}

			}
	}
		
		if (tolower(lid_config.keyword[i])=="block")
		{
			CMBBlock B;
						
			if (tolower(lid_config.value[i]) == "soil") { B.indicator = Block_types::Soil; } // 0
			if (tolower(lid_config.value[i]) == "pond") { B.indicator = Block_types::Pond; } //1
			if (tolower(lid_config.value[i]) == "storage") { B.indicator = Block_types::Storage; } //2
			if (tolower(lid_config.value[i]) == "catchment") { B.indicator = Block_types::Catchment; } //3
			if (tolower(lid_config.value[i]) == "manhole") { B.indicator = Block_types::Manhole; } //4
			if (tolower(lid_config.value[i]) == "darcy") { B.indicator = Block_types::Darcy; } //5
			if (tolower(lid_config.value[i]) == "stream") { B.indicator = Block_types::Stream; } //6

			B.ID = to_string(Blocks.size());
					
			B.set_val("a", atof(lid_config.param_vals[i][lookup(tolower(lid_config.param_names[i]), "a")].c_str()));

			if ((lookup(lid_config.param_names[i], "V") == -1) && (lookup(lid_config.param_names[i], "depth") != -1))
				B.set_val("v", B.A*atof(lid_config.param_vals[i][lookup(lid_config.param_names[i], "depth")].c_str()));
						
			for (int j=0; j<lid_config.param_names[i].size(); j++)
			{
				B.set_val(lid_config.param_names[i][j],atof(lid_config.param_vals[i][j].c_str()));
				if (tolower(lid_config.param_names[i][j])=="h_s_expression") {B.H_S_expression = lid_config.param_vals[i][j]; B.H_S_expression_txt=lid_config.param_vals[i][j];}
				if (tolower(lid_config.param_names[i][j])=="inflow") B.inflow_filename.push_back(lid_config.param_vals[i][j]);
				if (tolower(lid_config.param_names[i][j])=="id") B.ID = lid_config.param_vals[i][j].c_str();
				if ((tolower(lid_config.param_names[i][j])=="solid_phase") || (tolower(lid_config.param_names[i][j])=="particulate_phase")) B.Solid_phase_id.push_back(atoi(lid_config.param_vals[i][j].c_str()));
				if (tolower(lid_config.param_names[i][j])=="buildup") B.buildup_id.push_back(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "evaporation") B.evaporation_id.push_back(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j])=="externalflux") B.envexchange_id.push_back(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j])=="precipitation") B.precipitation_swch = atoi(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j]) == "light") B.light_swch = atoi(lid_config.param_vals[i][j].c_str());
				if ((tolower(lid_config.param_names[i][j]) == "rxn") || (tolower(lid_config.param_names[i][j]) == "reaction")) B.perform_rxn = atoi(lid_config.param_vals[i][j].c_str());
			}
			Blocks.push_back(B);  
			for (int ii=0; ii<lid_config.est_param[i].size(); ii++)
			{
				if (lookup_parameters(lid_config.est_param[i][ii]) != -1) {
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].location.push_back(Blocks.size() - 1);
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].quan.push_back(lid_config.param_names[i][ii]);
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].location_type.push_back(0);
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].experiment_id.push_back(name);
				}
			}

			
		}

		if (tolower(lid_config.keyword[i])=="connector")
		{
			CConnection C;
			
			for (int j=0; j<lid_config.param_names[i].size(); j++)
			{
				C.flow_params.resize(30);
				
				if (tolower(lid_config.value[i]) == "darcy") C.flow_expression = CStringOP(formulas()[Darcy]);
				if (tolower(lid_config.value[i]) == "pipe1") C.flow_expression = CStringOP(formulas()[Pipe1]);
				if (tolower(lid_config.value[i]) == "pipe2") C.flow_expression = CStringOP(formulas()[Pipe2]);
				if (tolower(lid_config.value[i]) == "pipe") C.flow_expression = CStringOP(formulas()[Pipe2]);
				if (tolower(lid_config.value[i]) == "normal") C.flow_expression = CStringOP(formulas()[Normal]);
				if (tolower(lid_config.value[i]) == "rating_curve") C.flow_expression = CStringOP(formulas()[Rating_curve]);
				
				C.set_val(lid_config.param_names[i][j],atof(lid_config.param_vals[i][j].c_str()));  //sets all values except theta (theta was removed in input file)
				if (tolower(lid_config.param_names[i][j])=="flow_expression") {C.flow_expression = CStringOP(lid_config.param_vals[i][j]); C.flow_expression_strng = lid_config.param_vals[i][j]; }
				if (tolower(lid_config.param_names[i][j]) == "area_expression") { C.area_expression = CStringOP(lid_config.param_vals[i][j]); C.area_expression_strng = lid_config.param_vals[i][j]; C.const_area = false; }
				if (tolower(lid_config.param_names[i][j]) == "a") {C.const_area = true;}
				if (tolower(lid_config.param_names[i][j])=="dispersion_expression") {C.dispersion_expression = CStringOP(lid_config.param_vals[i][j]); C.dispersion_strng = lid_config.param_vals[i][j]; }
				if (tolower(lid_config.param_names[i][j]) == "prescribed_flow") { C.presc_flow = true; C.pre_flow_filename = lid_config.param_vals[i][j]; }
				if (tolower(lid_config.param_names[i][j]) == "pre_flow_filename") { C.presc_flow = true; C.pre_flow_filename = lid_config.param_vals[i][j]; }
				if (tolower(lid_config.param_names[i][j])=="block1") C.Block1ID = lid_config.param_vals[i][j].c_str();
				if (tolower(lid_config.param_names[i][j])=="block2") C.Block2ID = lid_config.param_vals[i][j].c_str();
				if ((tolower(lid_config.param_names[i][j])=="solid_phase") || (tolower(lid_config.param_names[i][j])=="particulate_phase")) C.Solid_phase_id.push_back(atoi(lid_config.param_vals[i][j].c_str()));
				if (tolower(lid_config.param_names[i][j]) == "settling") C.settling = atoi(lid_config.param_vals[i][j].c_str());
			}
			Connector.push_back(C);
			for (int ii=0; ii<lid_config.est_param[i].size(); ii++)
			{
				if (lookup_parameters(lid_config.est_param[i][ii])!=-1) 
				{
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].location.push_back(Connector.size() - 1);
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].quan.push_back(lid_config.param_names[i][ii]);
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].location_type.push_back(1);
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].experiment_id.push_back(name);
				}
			}
		}

		if (tolower(lid_config.keyword[i])=="c_block")
		{
			string ID = lid_config.value[i].c_str();
			for (int j=0; j<lid_config.param_names[i].size(); j++)
			{
				Blocks[getblocksq(ID)].set_val(lid_config.param_names[i][j],atof(lid_config.param_vals[i][j].c_str()));
				if (tolower(lid_config.param_names[i][j])=="h_s_expression") {Blocks[getblocksq(ID)].H_S_expression = CStringOP(lid_config.param_vals[i][j]); Blocks[getblocksq(ID)].H_S_expression_txt=lid_config.param_vals[i][j];}
				if (tolower(lid_config.param_names[i][j])=="inflow") Blocks[getblocksq(ID)].inflow_filename.push_back(lid_config.param_vals[i][j]);
			}

		}

		if (tolower(lid_config.keyword[i])=="c_connector")
		{
			int ID = atoi(lid_config.value[i].c_str());
			for (int j=0; j<lid_config.param_names[i].size(); j++)
			{
				Connector[ID].set_val(lid_config.param_names[i][j],atof(lid_config.param_vals[i][j].c_str()));  //only sets d value
				if (tolower(lid_config.param_names[i][j]) == "area_expression") { Connector[ID].area_expression = CStringOP(lid_config.param_vals[i][j]); Connector[ID].area_expression_strng = lid_config.param_vals[i][j]; Connector[ID].const_area = false; }
				if (tolower(lid_config.param_names[i][j]) == "a") { Connector[ID].const_area = true; }
				if (tolower(lid_config.param_names[i][j])=="flow_expression") {Connector[ID].flow_expression = CStringOP(lid_config.param_vals[i][j]); Connector[ID].flow_expression_strng =  lid_config.param_vals[i][j];} //flow-expression was fixed
				if (tolower(lid_config.param_names[i][j]) == "area_expression") { Connector[ID].area_expression = CStringOP(lid_config.param_vals[i][j]); Connector[ID].area_expression_strng = lid_config.param_vals[i][j]; } //flow-expression was fixed
				if (tolower(lid_config.param_names[i][j])=="block1") Connector[ID].Block1ID = atoi(lid_config.param_vals[i][j].c_str());
				if (tolower(lid_config.param_names[i][j])=="block2") Connector[ID].Block2ID = atoi(lid_config.param_vals[i][j].c_str());

			}
		}
	}

	for (int i = 0; i<Connector.size(); i++) Connector[i].ID = Connector[i].Block1ID + "_" + Connector[i].Block2ID;
}

void CMedium::f_load_inflows()
{
	for (int j = 0; j < Precipitation_filename.size(); j++)
	{
		CPrecipitation P = CPrecipitation(pathname() + Precipitation_filename[j]);
		if (P.n>0) Precipitation.push_back(P);
	}
	for (int j = 0; j < evaporation_model().size(); j++)
	{
		if (evaporation_model()[j].evaporation_filename != "")
		{
			CBTC E = CBTC(pathname() + evaporation_model()[j].evaporation_filename);
			if (E.n>0) evaporation_model()[j].evaporation_TS = E;
		}
	}
	for (int j = 0; j<temperature_filename.size(); j++)
		{
		CBTC T = CBTC(pathname() + temperature_filename[j]);
		if (T.n>0) temperature.push_back(T);
	}

	for (int j = 0; j<r_humidity_filename.size(); j++)
	{
		CBTC T = CBTC(pathname() + r_humidity_filename[j]);
		if (T.n>0) r_humidity.push_back(T);
	}

	for (int j = 0; j<wind_filename.size(); j++)
	{
		CBTC T = CBTC(pathname() + wind_filename[j]);
		if (T.n>0) wind.push_back(T);
	}

	for (int j = 0; j<light_filename.size(); j++)
	{
		CBTC L = CBTC(pathname() + light_filename[j]);
		if (L.n>0) light.push_back(L);
	}
		
	for (int i = 0; i<light.size(); i++)
		light[i] = light[i].make_uniform(dt());

	for (int i = 0; i<temperature.size(); i++)
		temperature[i] = temperature[i].make_uniform(dt());


	for (int i = 0; i<Blocks.size(); i++)
	{
		for (int j = 0; j<Blocks[i].inflow_filename.size(); j++)
			Blocks[i].inflow.push_back(CBTCSet(pathname() + Blocks[i].inflow_filename[j], 1));


		if ((Blocks[i].indicator == Pond) || (Blocks[i].indicator == Catchment) || (Blocks[i].indicator == Stream))
		{
			if (Blocks[i].precipitation_swch == true)
				for (int j = 0; j<Precipitation_filename.size(); j++)
					Blocks[i].inflow.push_back(Precipitation[j].getflow(Blocks[i].A));
		}
	}

	for (int i = 0; i<Connector.size(); i++)
	{
		if (Connector[i].pre_flow_filename != "") {
			Connector[i].presc_flow = true; Connector[i].presc_flowrate = CBTC(pathname() + Connector[i].pre_flow_filename);
		}
			
	}

	f_make_uniform_inflows();
}

void CMedium::f_make_uniform_inflows()
{
	for (int i = 0; i<Blocks.size(); i++)
	{
		for (int j = 0; j<Blocks[i].inflow.size(); j++)
			Blocks[i].inflow[j] = Blocks[i].inflow[j].make_uniform(dt());
	}
	for (int i = 0; i < evaporation_model().size(); i++)
		if (evaporation_model()[i].evaporation_TS.n>0)
			evaporation_model()[i].evaporation_TS = evaporation_model()[i].evaporation_TS.make_uniform(dt());
	for (int i = 0; i<light.size(); i++)
		light[i] = light[i].make_uniform(dt());

	for (int i = 0; i<temperature.size(); i++)
		temperature[i] = temperature[i].make_uniform(dt());

	for (int i = 0; i<r_humidity.size(); i++)
		r_humidity[i] = r_humidity[i].make_uniform(dt());

	for (int i = 0; i<wind.size(); i++)
		wind[i] = wind[i].make_uniform(dt());
	
	for (int i = 0; i<Connector.size(); i++)
	{
		if (Connector[i].presc_flow == true) Connector[i].presc_flowrate = Connector[i].presc_flowrate.make_uniform(dt());
	}

}





void CMedium::f_set_default_connector_expressions()
{
	for (int ii=0; ii<Connector.size(); ii++)
	{
		if (Connector[ii].flow_expression.terms.size() == 0)
		{
			if (Blocks[getblocksq(Connector[ii].Block1ID)].z0 >= Blocks[getblocksq(Connector[ii].Block2ID)].z0)
			{
				if (vaporTransport()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator] == true)
				{	Connector[ii].flow_expression = CStringOP(formulasQ()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator]+ "+" + formulas()[Vapor]) ;
					Connector[ii].flow_expression_strng = formulasQ()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator]+ "+" + formulas()[Vapor] ;
					
				}
				else
				{	Connector[ii].flow_expression = CStringOP(formulasQ()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator]) ;
					Connector[ii].flow_expression_strng = formulasQ()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
					
				}
			}
			else
			{
				if (vaporTransport()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator] == true)
				{
					Connector[ii].flow_expression = CStringOP(formulasQ2()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator] + "+" + formulas()[Vapor]);
					Connector[ii].flow_expression_strng = formulasQ2()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator] + "+" + formulas()[Vapor];
				}
				else 
				{	Connector[ii].flow_expression = CStringOP(formulasQ2()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator]) ;
					Connector[ii].flow_expression_strng = formulasQ2()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
				}
			}
			if (vaporTransport()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator])
			{
				Connector[ii].flow_expression_v = CStringOP(formulas()[Vapor]);
				Connector[ii].flow_expression_strng_v = formulas()[Vapor];
			}
		}								  
	}

	for (int ii = 0; ii < Connector.size(); ii++)
	{
		Connector[ii].area_expression = formulasA()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
		Connector[ii].area_expression_strng = formulasA()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
		if (Connector[ii].A == 0)
			Connector[ii].const_area = const_area()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
		else
			Connector[ii].const_area = true;
	}

	for (int ii = 0; ii < Connector.size(); ii++)
	{
		if (Connector[ii].settling == -1)
		{
			Connector[ii].settling = settling()[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
		}
	}


	for (int i=0; i<Connector.size(); i++)
	{
		for (int j = 0;  j< Solid_phase().size(); j++)Connector[i].Solid_phase_id.push_back(j);
		Connector[i].Block1 = &Blocks[getblocksq(Connector[i].Block1ID)]; 
		Connector[i].Block2 = &Blocks[getblocksq(Connector[i].Block2ID)]; 
		Blocks[getblocksq(Connector[i].Block1ID)].connectors.push_back(i);
		Blocks[getblocksq(Connector[i].Block1ID)].connectors_se.push_back(0);
		Blocks[getblocksq(Connector[i].Block2ID)].connectors.push_back(i);
		Blocks[getblocksq(Connector[i].Block2ID)].connectors_se.push_back(1);
		for (int ii=0; ii<Connector[i].Solid_phase_id.size(); ii++) Connector[i].Solid_phase.push_back(&(Solid_phase()[Connector[i].Solid_phase_id[ii]]));

	}
}

void CMedium::f_set_default_block_expressions()
{
	for (int ii=0; ii<Blocks.size(); ii++)
	{
		if (Blocks[ii].H_S_expression.terms.size() == 0)
		{
			Blocks[ii].H_S_expression = CStringOP(formulasH()[Blocks[getblocksq(Blocks[ii].ID)].indicator]); Blocks[ii].H_S_expression_txt = formulasH()[Blocks[getblocksq(Blocks[ii].ID)].indicator];
		}
		if (Blocks[ii].air_phase==-1) 
			Blocks[ii].air_phase = air_phase()[Blocks[getblocksq(Blocks[ii].ID)].indicator];

	}    
}

CMedium::CMedium(string filename, CMediumSet *_parent)
{
	parent = _parent;
	lid_config.getconfigfromfile(filename);   //reads input file and creates lid_config
	*this = CMedium(lid_config, parent);
}



void CMedium::set_var(int i, double v)
{
	for (int j=0; j<Connector.size(); j++)
	{
		if (i==2) Connector[j].A = v;
		if (i==6) Connector[j].d = v;
		if (i==7) Connector[j].Q = v;
		if (i==8) Connector[j].v = v;
		if ((i>=50) && (i<100)) Connector[j].flow_params[i-50] = v;

		//MM  ----------------->???
		//if ((i>=3000) && (i<3100))
		//	Connector[i].dispersion[i-3000] = v;
		//if ((i>=3100) && (i<3199))
		//	Connector[j].tr_alpha[i-3100] = v;
		//if ((i>=3200) && (i<3299))
		//	Connector[j].dispersivity[i-3200] = v;
		if (i==4000)				Connector[j].c_dispersion;
		if ((i>=5000) && (i<5100)) 	Connector[j].dispersion[i-5000];
	}

	for (int j=0; j<Blocks.size(); j++)
	{
		if (i==1) Blocks[j].H = v;
		if (i==2) Blocks[j].A = v;
		if (i==3) Blocks[j].V = v;
		if (i==4) Blocks[j].S = v;
		if (i==5) Blocks[j].z0 = v;
		if (i==9) Blocks[j].S = Blocks[j].V*(v*(Blocks[j].fs_params[1]-Blocks[j].fs_params[2]) + Blocks[j].fs_params[2]);
		if (i==10) Blocks[j].S = Blocks[j].V*v;    
		if (i>=50 && i<100) Blocks[j].fs_params[i-50] = v;      
		if (i>=100 && i<200) Blocks[j].G[(i-100)/100][(i-100)%100] = v; 
		if (i>=1000 && i<2000) Blocks[j].CG[int((i-1000)/Blocks[j].n_phases)][(i-1000)%Blocks[j].n_phases] = v;
		if (i>=2000 && i<3000) Blocks[j].rxn_params[i-2000] = v;
	}

	if ((i>=3000) && (i<4000)) std()[i-3000] = v; //the parameter code for std is from 3000-4000
}

void CMedium::set_var(const string &S, const double &v)
{
	if (S=="dtt") dtt = v;
	if (S=="dt") dt() = v;
	
	for (int j=0; j<Connector.size(); j++)
		Connector[j].set_val(tolower(trim(S)), v);


	for (int j=0; j<Blocks.size(); j++)
		Blocks[j].set_val(tolower(trim(S)), v);
}

void CMedium::set_var(const string &S, int i, const double &v)
{
	for (int j=0; j<Connector.size(); j++)
		Connector[j].set_val(tolower(trim(S)), v);
	
	for (int j=0; j<Blocks.size(); j++)
		Blocks[j].set_val(tolower(trim(S)), v);
}

void CMedium::set_var(const string &S, const vector<double> &v)
{
	for (int j=0; j<Blocks.size(); j++)
		Blocks[j].set_val(S, v[j]);
}

void CMedium::set_var(const string &S, const double &v, const int &start, const int &end)
{
	for (int j=start; j<end-1; j++)
		Connector[j].set_val(tolower(trim(S)), v);


	for (int j=start; j<end; j++)
		Blocks[j].set_val(tolower(trim(S)), v);

}

void CMedium::set_var(const string &S, const vector<double> &v, const int &start, const int &end)
{
	for (int j=start; j<end-1; j++)
		Connector[j].set_val(S, v[j]);

	for (int j=start; j<end; j++)
		Blocks[j].set_val(S, v[j]);

}

void CMedium::set_var(const string &S, const double &v, const int &n)
{
	for (int j=Connector.size()-n+1; j<Connector.size(); j++)
		Connector[j].set_val(tolower(trim(S)), v);


	for (int j=Blocks.size()-n; j<Blocks.size(); j++)
		Blocks[j].set_val(tolower(trim(S)), v);

}


void CMedium::set_var(const string &S, const vector<double> &v, const int &n)
{
	for (int j=Connector.size()-n+1; j<Connector.size(); j++)
		Connector[j].set_val(S, v[j]);

	for (int j=Blocks.size()-n; j<Blocks.size(); j++)
		Blocks[j].set_val(S, v[j]);

}



void CMedium::add_Richards_medium(int n, double dz,  int id)
{
	/* van Genuchten parameters:
	50: Ks
	51: theta_s
	52: theta_r
	53: alpha
	54: n
	55: m
	57: storitivity
	*/
	//Blocks.resize(n);
	//Connector.resize(n-1);
	if (id==-1) id=Blocks.size();

	for (int i=0; i<n; i++)
	{
		CMBBlock B;
		B.indicator = 0;
		
		//                                         61       1 543     2      1       123  4 1       156
		//                                     z0 -((1/alpha)*((( Se ^(n    /(1-n    )))-1)^(1/n    )))
		//B.H_S_expression = CStringOP("f[5]-((1/f[53])*((f[9]^(f[54]/(1-f[54])))-1)^(1/f[54]))");   //z0+h=Total Head for botom of block
		B.H_S_expression = CStringOP(formulasH()[Soil]);   
		B.H_S_expression_txt = formulasH()[Soil];
		B.fs_params.resize(30);
		B.ID=to_string(id+i);
		Blocks.push_back(B);
	}

	for (int i=0; i<n-1; i++)
	{
		CConnection C;
		
		C.Block1ID = to_string(i+id);
		C.Block2ID = to_string(i+id+1);
		C.Block1 = &(Blocks[getblocksq(to_string(i+id))]);
		C.Block2 = &(Blocks[getblocksq(to_string(i+id+1))]);
		C.d = dz;
		C.flow_params.resize(30);  //0=Ks, 1=theta_s, 2=theta_r, 3=VG_alpha, 4=VG_n, 5=VG_m, 6=lambda
		C.flow_expression = CStringOP(formulasQ()[Soil][Soil]); 
		C.flow_expression_strng = formulasQ()[Soil][Soil];
		C.flow_expression_v = CStringOP(formulas()[Vapor]); 
		C.flow_expression_strng_v = formulas()[Vapor];
		Connector.push_back(C);
	}
}

void CMedium::add_Darcy_medium(int n, double dz,  int id)
{
	/* van Genuchten parameters:
	50: Ks
	51: theta_s
	52: theta_r
	53: alpha
	54: n
	55: m
	57: storitivity
	*/
	//Blocks.resize(n);
	//Connector.resize(n-1);
	if (id==-1) id=Blocks.size();

	for (int i=0; i<n; i++)
	{
		CMBBlock B;
		B.indicator = 5;
		B.H_S_expression = CStringOP(formulasH()[Darcy]);   
		B.H_S_expression_txt = formulasH()[Darcy];
		B.fs_params.resize(8);
		B.ID=id+i;
		Blocks.push_back(B);
	}

	for (int i=0; i<n-1; i++)
	{
		CConnection C;
		
		C.Block1ID = to_string(i+id);
		C.Block2ID = to_string(i+id+1);
		C.Block1 = &(Blocks[getblocksq(to_string(i+id))]);
		C.Block2 = &(Blocks[getblocksq(to_string(i+id+1))]);
		C.d = dz;
		C.flow_params.resize(30);  //0=Ks, 1=theta_s, 2=theta_r, 3=VG_alpha, 4=VG_n, 5=VG_m, 6=lambda
		C.flow_expression = CStringOP(formulasQ()[Darcy][Darcy]); 
		C.flow_expression_strng = formulasQ()[Darcy][Darcy];
		Connector.push_back(C);
	}
}

void CMedium::add_stream_medium(int n, double z0, double slope, double length, int id)
{
	/* van Genuchten parameters:
	50: Ks
	51: theta_s
	52: theta_r
	53: alpha
	54: n
	55: m
	57: storitivity
	*/
	//Blocks.resize(n);
	//Connector.resize(n-1);
	if (id == -1) id = Blocks.size();

	for (int i = 0; i<n; i++)
	{
		CMBBlock B;
		B.indicator = 5;
		B.H_S_expression = CStringOP(formulasH()[Stream]);
		B.H_S_expression_txt = formulasH()[Stream];
		B.fs_params.resize(8);
		B.ID = id + i;
		B.z0 = z0 + length / double(n)*slope;
		Blocks.push_back(B);
	}

	for (int i = 0; i<n - 1; i++)
	{
		CConnection C;

		C.Block1ID = to_string(i + id);
		C.Block2ID = to_string(i + id + 1);
		C.Block1 = &(Blocks[getblocksq(to_string(i + id))]);
		C.Block2 = &(Blocks[getblocksq(to_string(i + id + 1))]);
		C.d = length/double(n);
		C.flow_params.resize(30);  //0=Ks, 1=theta_s, 2=theta_r, 3=VG_alpha, 4=VG_n, 5=VG_m, 6=lambda
		C.flow_expression = CStringOP(formulasQ()[Stream][Stream]);
		C.flow_expression_strng = formulasQ()[Stream][Stream];
		Connector.push_back(C);
	}
}

void CMedium::add_catchment_medium(int n, double z0, double slope, double length, int id)
{
	/* van Genuchten parameters:
	50: Ks
	51: theta_s
	52: theta_r
	53: alpha
	54: n
	55: m
	57: storitivity
	*/
	//Blocks.resize(n);
	//Connector.resize(n-1);
	if (id == -1) id = Blocks.size();

	for (int i = 0; i<n; i++)
	{
		CMBBlock B;
		B.indicator = 5;
		B.H_S_expression = CStringOP(formulasH()[Catchment]);
		B.H_S_expression_txt = formulasH()[Catchment];
		B.fs_params.resize(8);
		B.ID = id + i;
		B.z0 = z0 + length / double(n)*slope;
		Blocks.push_back(B);
	}

	for (int i = 0; i<n - 1; i++)
	{
		CConnection C;

		C.Block1ID = to_string(i + id);
		C.Block2ID = to_string(i + id + 1);
		C.Block1 = &(Blocks[getblocksq(to_string(i + id))]);
		C.Block2 = &(Blocks[getblocksq(to_string(i + id + 1))]);
		C.d = length / double(n);
		C.flow_params.resize(30);  //0=Ks, 1=theta_s, 2=theta_r, 3=VG_alpha, 4=VG_n, 5=VG_m, 6=lambda
		C.flow_expression = CStringOP(formulasQ()[Catchment][Catchment]);
		C.flow_expression_strng = formulasQ()[Catchment][Catchment];
		Connector.push_back(C);
	}
}



void CMedium::set_z0(double down, double up)
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].set_val("z0",down+i/(Blocks.size()-1)*(up-down));
	set_var("d", (up-down)/(Blocks.size()-1));
}

void CMedium::set_z0(double down)	
{
	for (int i=0; i<Blocks.size(); i++)
	{
		Blocks[i].set_val("z0",down+i*Connector[0].d);   // Blocks[i].set_val("z0",down+(i+0.5)*Connector[0].d); to set z0 for center of block
	}

}


//MM
void CMedium::set_z0(int id, int n, double z0, double dz)
{
	for (int i=0; i<n; i++)
	{
		Blocks[id+i].set_val("z0",z0+dz*i);   
	}
}




void CMedium::add_KW_medium(int n, double dz, vector<double> params)
{


}


void CMedium::add_KW_medium(int n, double dz)
{


}


void CMedium::set_num_phases(int n)
{
	for (int j=0; j<Blocks.size(); j++)
		Blocks[j].set_num_phases(n);

}

void CMedium::set_num_constituents(int n)
{
	for (int j=0; j<Blocks.size(); j++)
		Blocks[j].set_num_constts(n);

}




CVector CMedium::getres_S(const CVector &X, double dt)
{
	CVector F(X.num);  //Create Vector F with size num
	setS_star(X);     //Blocks[i].set_val("S*", X[i]);
	CVector X1 = X;

	for (int i=0; i<Blocks.size(); i++)
	{	if (Blocks[i].setzero==1) {dt=X.vec[i]; Blocks[i].S_star=0;}
	if (Blocks[i].setzero==2) {Blocks[i].outflow_corr_factor=X1[i];	Blocks[i].S_star=0;}  
	if (Blocks[i].setzero!=0) X1[i]=0; else Blocks[i].outflow_corr_factor = 1;
	}


	setH_star();      //Blocks[i].set_val("H*", Blocks[i].calc_star(Blocks[i].H_S_expression));  
	evaluate_area();  //Update connector areas; 
	setQ_star();      //Connector[i].set_val("Q*",Connector[i].calc_star(Connector[i].flow_expression));  
	

	for (int i=0; i<Blocks.size(); i++)
	{    
		if  (Blocks[i].inflow.size()!=0) 
			F[i]= (X1[i]-Blocks[i].S)/dt-sum_interpolate(Blocks[i].inflow, t,"flow"); 
		else F[i]= (X1[i]-Blocks[i].S)/dt;
		F[i]+=Blocks[i].outflow_corr_factor*Blocks[i].get_evaporation(t);
	}




	for (int i=0; i<Connector.size(); i++)
	{
		if  (w()*Connector[i].Q + (1-w())*Connector[i].Q_star>0) Connector[i].flow_factor=Blocks[getblocksq(Connector[i].Block1ID)].outflow_corr_factor;   
		if  (w()*Connector[i].Q + (1-w())*Connector[i].Q_star<0) Connector[i].flow_factor=Blocks[getblocksq(Connector[i].Block2ID)].outflow_corr_factor;
	}

	for (int i=0; i<Connector.size(); i++)
	{
		F[getblocksq(Connector[i].Block1ID)] += (w()*Connector[i].Q + (1-w())*Connector[i].Q_star)*Connector[i].flow_factor;
		F[getblocksq(Connector[i].Block2ID)] -= (w()*Connector[i].Q + (1-w())*Connector[i].Q_star)*Connector[i].flow_factor;
	}

	

	for (int i=0; i<Blocks.size(); i++)
	{
		if (Blocks[i].setzero==2)
		{
			bool all_out_flows_zero=true;
			if (Blocks[i].get_evaporation(t) > 1e-30) all_out_flows_zero=false;  
			for (int k=0; k<Blocks[i].connectors.size(); k++)
				if ((w()*Connector[Blocks[i].connectors[k]].Q + (1-w())*Connector[Blocks[i].connectors[k]].Q_star)*(Blocks[i].connectors_se[k]-0.5)<0)
					all_out_flows_zero = false;
			if (all_out_flows_zero==true)
				F[i]=Blocks[i].outflow_corr_factor-1.1;
		}
	}

	return F;
}

void CMedium::setS_star(const CVector &X)
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].set_val("s*", X.vec[i]);

}

void CMedium::set_G_star(const CVector &X)
{
	int jj=0;
	for (int i=0; i<Blocks.size(); i++)
		for (int p=0; p<Blocks[i].Solid_phase.size() ; p++)
			for (int l=0; l<Blocks[i].Solid_phase[p]->n_phases ; l++)
				Blocks[i].G_star[p][l] = X.vec[get_member_no(i,p,l)];
}

void CMedium::setH_star()
{
	if (!steady_state_hydro())
		for (int i=0; i<Blocks.size(); i++)
			Blocks[i].set_val("h*", Blocks[i].calc_star(Blocks[i].H_S_expression));  
}

void CMedium::setH()
{
	if (!steady_state_hydro())
		for (int i=0; i<Blocks.size(); i++)
			Blocks[i].set_val("h", Blocks[i].calc(Blocks[i].H_S_expression));
}


double CMedium::calc_log_likelihood() //calculate sum log likelihood for time series data ts
{
	double sum=0;
	solve();
	if (failed==true) return -1e30;

	if (solution_method()==1) ANS_obs.make_uniform(dt());
	for (int i=0; i<measured_quan().size(); i++)
		sum+=calc_log_likelihood(i);

	return sum;
}

double CMedium::calc_log_likelihood(int i) //calculate sum log likelihood for observed quantity i
{
	double sum=0; 
	if (measured_quan()[i].error_structure == 0)
	{
		int k = measured_data().lookup(measured_quan()[i].name);
		if (k != -1) sum -= diff(ANS_obs.BTC[i], measured_data().BTC[k]) / (2 * std()[measured_quan()[i].std_no] * std()[measured_quan()[i].std_no]);

	}
	if (measured_quan()[i].error_structure == 1)
	{
		int k = measured_data().lookup(measured_quan()[i].name);
		if (k != -1) sum -= diff(ANS_obs.BTC[i].Log(1e-4), measured_data().BTC[k].Log(1e-4)) / (2 * std()[measured_quan()[i].std_no] * std()[measured_quan()[i].std_no]);

	}
	sum-=measured_data().BTC[i].n*log(std()[measured_quan()[i].std_no]);
		
	return sum;
}

void CMedium::setQ_star()
{
	for (int i=0; i<Connector.size(); i++)	
	{
		if (!Connector[i].presc_flow)
			Connector[i].set_val("q*", Connector[i].calc_star(Connector[i].flow_expression));
		else
			Connector[i].set_val("q*", Connector[i].presc_flowrate.interpol(t));
			
		
		Connector[i].set_val("q_v*", Connector[i].calc_star(Connector[i].flow_expression_v));
		Connector[i].flow_factor = 1;

	}

}

void CMedium::setQ0()
{
	if (!steady_state_hydro())
		for (int i = 0; i < Connector.size(); i++)
		{
			if (!Connector[i].presc_flow)
				Connector[i].set_val("q", Connector[i].calc_star(Connector[i].flow_expression));
			else
				Connector[i].set_val("q", Connector[i].presc_flowrate.interpol(t));


			Connector[i].set_val("q_v", Connector[i].calc_star(Connector[i].flow_expression_v));
			Connector[i].flow_factor = 1;

		}
	else
	{
		CVector Q = hydro_steady_matrix_inv*get_steady_hydro_RHS();
		for (int i = 0; i < Connector.size(); i++)
			Connector[i].Q = Q[i];
	}

}

void CMedium::setQ()  
{
	for (int i=0; i<Connector.size(); i++)
	{   Connector[i].set_val("q",Connector[i].Q_star);
		Connector[i].set_val("q_v",Connector[i].Q_v_star); 
	}
}

void CMedium::setQ(const CVector &X)
{

	CVector X_Temp=getS();
	set_var("S",X.vec);

	for (int i=0; i<Connector.size(); i++)
		if (Connector[i].fixed == false)
		{	Connector[i].set_val("q",Connector[i].calc(Connector[i].flow_expression));
	Connector[i].set_val("q_v",Connector[i].calc(Connector[i].flow_expression_v));
	}

	set_var("S",X_Temp.vec);

}


CVector CMedium::getH(const CVector &X)
{
	CVector H(X.num);
	for (int i=0; i<Blocks.size(); i++)
		H[i] = Blocks[i].calc_star(Blocks[i].H_S_expression);

	return H;
}

CVector CMedium::getQ(const CVector &X)
{
	CVector H(Connector.size());
	for (int i = 0; i < Connector.size(); i++)
	{
		if (!Connector[i].presc_flow)
			H[i] = Connector[i].calc_star(Connector[i].flow_expression);
		else
			H[i] = Connector[i].presc_flowrate.interpol(t);
	}
	return H;
}

CMatrix CMedium::Jacobian_S(const CVector &X, double dt, bool base=true)  
{
	CMatrix M(X.num);

	if (base==true)
	{
		CVector F0 = getres_S(CVector(X),dt);
//#pragma omp parallel for //ask arash
		for (int i = 0; i<X.num; i++)
			M[i] = Jacobian_S(X, F0, i, dt);  //M is a matrix  
	}
	else
	{
//#pragma omp parallel for //ask arash
		for (int i = 0; i<X.num; i++)
			M[i] = Jacobian_S(X, i, dt);  //M is a matrix  
	}


	return Transpose(M);
}

CVector CMedium::Jacobian_S(const CVector &V, int &i, double &dt)  //Works also w/o reference (&) 
{
	double epsilon;
	if (V.vec[i]<1e-5) epsilon=-1e-6; else epsilon=1e-6;   //----->check shavad???
	CVector V1(V);
	V1[i]+=epsilon;
	CVector F1, F0;
	F1 = getres_S(V1,dt);
	F0 = getres_S(CVector(V),dt);

	return (F1 - F0)/epsilon;
}

CVector CMedium::Jacobian_S(const CVector &V, CVector &F0, int i, double dt)  //Works also w/o reference (&) 
{
	double epsilon;
	epsilon = -1e-6;
	CVector V1(V);
	V1[i]+=epsilon;
	CVector F1;
	F1 = getres_S(V1,dt);
	CVector grad = (F1 - F0) / epsilon;
	if (grad.norm2() == 0)
	{
		epsilon = 1e-6;
		V1 = V;
		V1[i] += epsilon;
		F1 = getres_S(V1, dt);
		grad = (F1 - F0) / epsilon;
	}
	return grad;
	
}



CMatrix CMedium::Jacobian_C(const CVector &X, double dt, bool base)  
{
	CMatrix M(X.num);

	if (base==true)
	{
		CVector F0 = getres_C(CVector(X),dt);
		for (int i=0; i<X.num; i++)
			M[i] = Jacobian_C(X, F0, i, dt);  
	}
	else
	{
		for (int i=0; i<X.num; i++)
			M[i] = Jacobian_C(X, i, dt);  
	}

	return Transpose(M);
}

CVector CMedium::Jacobian_C(const CVector &V, int i, double dt)  //Works also w/o reference (&) 
{
	double epsilon;
	if (V.vec[i]<1e-5) epsilon=-1e-6; else epsilon=1e-6;  
	CVector V1(V);
	V1[i]+=epsilon;
	CVector F1, F0;
	F1 = getres_C(V1,dt);
	F0 = getres_C(CVector(V),dt);

	return (F1 - F0)/epsilon;
}

CVector CMedium::Jacobian_C(const CVector &V, const CVector &F0, int i, double dt)  
{
	double epsilon;
	if ((V.vec[i]<1) && (V.vec[i]>-1))
		if (V.vec[i]<1e-5) epsilon=-1e-6; else epsilon=1e-6;   
	else
		epsilon = 1e-6*V.vec[i];
	CVector V1(V);
	V1[i]+=epsilon;
	CVector F1;
	F1 = getres_C(V1,dt);

	return (F1 - F0)/epsilon;
}

CMatrix CMedium::Jacobian_Q(const CVector &X, double dt, bool base)  
{
	CMatrix M(X.num);

	if (base==true)
	{
		CVector F0 = getres_Q(CVector(X),dt);
		for (int i=0; i<X.num; i++)
			M[i] = Jacobian_Q(X, F0, i, dt);  
	}
	else
	{
		for (int i=0; i<X.num; i++)
			M[i] = Jacobian_Q(X, i, dt);  
	}

	return Transpose(M);
}

CVector CMedium::Jacobian_Q(const CVector &V, int i, double dt)  
{
	double epsilon;
	if (V.vec[i]<1e-5) epsilon=-1e-6; else epsilon=1e-6;  
	CVector V1(V);
	V1[i]+=epsilon;
	CVector F1, F0;
	F1 = getres_Q(V1,dt);
	F0 = getres_Q(CVector(V),dt);

	return (F1 - F0)/epsilon;
}

CVector CMedium::Jacobian_Q(const CVector &V, const CVector &F0, int i, double dt)  
{
	double epsilon;
	if ((V.vec[i]<1) && (V.vec[i]>-1))
		if (V.vec[i]<1e-5) epsilon=-1e-6; else epsilon=1e-6;   
	else
		epsilon = 1e-6*V.vec[i];
	CVector V1(V);
	V1[i]+=epsilon;
	CVector F1;
	F1 = getres_Q(V1,dt);

	return (F1 - F0)/epsilon;
}




CVector CMedium::getS()
{
	//CVector X(Blocks.size()-2);
	CVector X(Blocks.size());
	for (int i=0; i<Blocks.size(); i++)
		X[i] = Blocks[i].S;
	return X;
}

void CMedium::onestepsolve_flow(double dtt)
{
	int indicator=1;
	int done = 0;
	CVector pos_def;
	vector<int> old_fixed_connect_status = get_fixed_connect_status();
	CVector X_old = getS();

	for (int i=0; i<Blocks.size(); i++)
	{	if (Blocks[i].setzero==1) X_old[i]=dtt;
		if (Blocks[i].setzero==2) X_old[i]=Blocks[i].outflow_corr_factor;
	}
	fail_reason = "none";

	CVector correction_factor_old = get_flow_factors();
	
	while ((indicator==1) && (done<2))
	{	
		bool fixed_connect;
		done+=1;
		double err = 10000;
		double err_p = 10000;
		int err_expand_counter=0;
		double errF;
		int J_update1, J_update2;
		J_update1 = J_update2 = J_update;  
		
		CVector X = getS();    //getting the Storage of blocks, test
		fixed_connect=false;
		for (int i=0; i<Blocks.size(); i++)
		{	if (Blocks[i].setzero==1) X[i]=dtt;
			if (Blocks[i].setzero==2) X[i]=Blocks[i].outflow_corr_factor;
			if (Blocks[i].setzero!=0) fixed_connect=true; 
		}

		CVector F = getres_S(X,dtt);  
		
		counter_flow = 0;
		
		J_h_update_count = 0;
		while (err>tol())
		{

			bool a = (F==F);
			if ((F==F)!=true || (X==X)!=true || (F.is_finite()==false) || (X.is_finite()==false) ) 
			{
				fail_reason = "indefinite X or F in hydro";
				set_fixed_connect_status(old_fixed_connect_status);
				return; 
			}
			
			
			if (((J_update1==true)||(InvJ1.getnumrows()==0))&&(fixed_connect==false)) 
			{
				J_h_update_count++;
				M = Jacobian_S(X,dtt);
				CMatrix M1 = normalize_diag(M,M);
				pos_def = M.diag_ratio();
				epoch_count++;
				double xx = diag(M).max();
				
				InvJ1 = inv(M1);
				if (InvJ1.getnumcols()!=Blocks.size())
				{
					fail_reason = "Hydro Jacobian in not inversible";
					set_flow_factors(correction_factor_old);
					set_fixed_connect_status(old_fixed_connect_status);
					return;
				}
				dtt_J_h1 = dtt;
				J_update1=false;
			}

			if (((J_update2==true)||(InvJ2.getnumrows()==0))&&(fixed_connect==true)) 
			{
				J_h_update_count++;
				M = Jacobian_S(X,dtt);
				CMatrix M1 = normalize_diag(M,M);
				epoch_count++;
				pos_def = M.diag_ratio();
				double xx = diag(M).max();
				
				InvJ2 = inv(M1);
				if (InvJ2.getnumcols()!=Blocks.size())
				{
					fail_reason = "Hydro Jacobian in not inversible";
					set_flow_factors(correction_factor_old);
					set_fixed_connect_status(old_fixed_connect_status);
					return;
				}
				dtt_J_h2 = dtt;
				J_update2=false;
			}
			
			CVector dx; 
			if (fixed_connect)
			{
				dx = (InvJ2*normalize_diag(F, M));
				X -= ((dtt / dtt_J_h2)*dx);
			}
			else
			{
				dx = (InvJ1*normalize_diag(F, M));
				X -= ((dtt / dtt_J_h1)*dx);
			}
			
			
			F = getres_S(X,dtt);
			
			err_p = err;
			err = F.norm2();
			if (err/err_p>0.8) 
				if (fixed_connect) J_update2=true; else J_update1=true;

			if ((dx/X).abs_max()<1e-15) err=0;
			if (err>=err_p) 
			{
				if (fixed_connect) J_update2=true; else J_update1=true; err_expand_counter++; X=X_old; set_flow_factors(correction_factor_old);
				err = 10000;
			}  
			if (err_expand_counter>2)
			{	
				fail_reason = "Expanding error in hydro";
				set_flow_factors(correction_factor_old);
				set_fixed_connect_status(old_fixed_connect_status);
				return;
			}

			if ((err == err) == false)
			{
				fail_reason = "infinite error in hydro";
				set_flow_factors(correction_factor_old);
				set_fixed_connect_status(old_fixed_connect_status);
				return;
			}


			counter_flow++;
			if (counter_flow>nr_failure_criteria()) 
			{	
				fail_reason = "Number of iteration exceeded the limit in hydro";
				set_flow_factors(correction_factor_old);
				set_fixed_connect_status(old_fixed_connect_status);
				return;
			}

		}

		failed = false;
		fail_reason = "none";
		indicator=0;

		for (int i=0; i<X.num; i++)
		{
			if ((X[i]<1e-15) && (X[i]>-1e-15)) X[i]=0; 
			if ((Blocks[i].setzero==0) && (X[i]<0))
			{
				J_update=true;
				indicator=1;
				failed = true;
				if (Blocks[i].S==0) 
					Blocks[i].setzero=2;
				else
					Blocks[i].setzero=2;       
			}
			if (Blocks[i].outflow_corr_factor>1)
			{	
				fail_reason = "block" + numbertostring(i) + "is wet";
				J_update=true;
				indicator = 1;
				failed = true;
				Blocks[i].setzero=0;
				Blocks[i].outflow_corr_factor=1;
			}
		}
	}

}

void CMedium::Blocksmassbalance()
{	
	for (int i=0; i<Blocks.size(); i++)
	{
		Blocks[i].MBBlocks =(Blocks[i].S_star-Blocks[i].S);
		Blocks[i].MBBlocks -= sum_interpolate(Blocks[i].inflow, t)[0]*dtt; 
		Blocks[i].MBBlocks +=Blocks[i].outflow_corr_factor*Blocks[i].get_evaporation(t)*dtt;	
	}
	for (int i=0; i<Connector.size(); i++)
	{
		Blocks[getblocksq(Connector[i].Block1ID)].MBBlocks += (w()*Connector[i].Q + (1-w())*Connector[i].Q_star)*dtt*Connector[i].flow_factor;                 
		Blocks[getblocksq(Connector[i].Block2ID)].MBBlocks -= (w()*Connector[i].Q + (1-w())*Connector[i].Q_star)*dtt*Connector[i].flow_factor;
	}


}




void CMedium::renew()
{
	for (int i=0; i<Blocks.size(); i++)
	{
		Blocks[i].A = Blocks[i].A_star;
		Blocks[i].CG = Blocks[i].CG_star;
		Blocks[i].G = Blocks[i].G_star;
		Blocks[i].H = Blocks[i].H_star;
		Blocks[i].S = Blocks[i].S_star;
		Blocks[i].V = Blocks[i].V_star;

	}

	for (int i=0; i<Connector.size(); i++)
	{
		Connector[i].A = Connector[i].A_star;
		Connector[i].Q = Connector[i].Q_star;
		Connector[i].v = Connector[i].v_star;
	}

}


void CMedium::renew_G()
{
	for (int i=0; i<Blocks.size(); i++)
	Blocks[i].G = Blocks[i].G_star;
}

void CMedium::renew_CG()
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].CG = Blocks[i].CG_star;
}


void CMedium::initialize()
{
	for (int i=0; i<Blocks.size(); i++)
	{
		Blocks[i].A_star = Blocks[i].A;
		Blocks[i].CG_star = Blocks[i].CG;
		Blocks[i].G_star = Blocks[i].G;
		Blocks[i].H_star = Blocks[i].H;
		Blocks[i].S_star = Blocks[i].S;
		Blocks[i].V_star = Blocks[i].V;
		Blocks[i].G_star = Blocks[i].G;  
	}

	for (int i=0; i<Connector.size(); i++)
	{
		Connector[i].A_star = Connector[i].A;
		Connector[i].Q_star = Connector[i].Q;
		Connector[i].v_star = Connector[i].v;
	}
}


void CMedium::solve_fts_m2(double dt)
{

	FILE *FILEBTC;
	
	Solution_dt.clear();
	Solution_dt = CBTCSet(3);
	dt_fail = 10000;
	ANS = CBTCSet(3*Blocks.size()+3*Connector.size());
	ANS_colloids = CBTCSet(Blocks.size()*Blocks[0].n_phases);
	ANS_constituents = CBTCSet(Blocks.size()*(Blocks[0].n_phases+2)*RXN().cons.size());
	if (mass_balance_check()) ANS_MB = CBTCSet(Blocks.size());
	char buffer[33];
	epoch_count = 0;

	ANS.names.clear();

	ANS_MB.names.clear();

	mass_balance_check();

	for (int i=0; i<Blocks.size(); i++)
		ANS_MB.pushBackName("S_"+Blocks[i].ID);


	for (int i = 0; i<Blocks.size(); i++) ANS.pushBackName("S_" + Blocks[i].ID);
	for (int i = 0; i<Connector.size(); i++) ANS.pushBackName("Q_" + Connector[i].ID);
	for (int i = 0; i<Blocks.size(); i++) ANS.pushBackName("H_" + Blocks[i].ID);
	for (int i = 0; i<Blocks.size(); i++) ANS.pushBackName("E_" + Blocks[i].ID);
	for (int i = 0; i<Connector.size(); i++) ANS.pushBackName("A_" + Connector[i].ID);

	for (int i = 0; i<Connector.size(); i++) ANS.pushBackName("Qv_" + Connector[i].ID);

	for (int j=0; j<Blocks[0].Solid_phase.size(); j++) 
		for (int k=0; k<Blocks[0].Solid_phase[j]->n_phases; k++)
			for (int i=0; i<Blocks.size(); i++) 
				ANS_colloids.setname(get_member_no(i,j,k), "C_"+Blocks[i].ID+"_"+Solid_phase()[j].name+"_"+Solid_phase()[j].phase_names[k]);
	
	
	for (int k=0; k<RXN().cons.size(); k++) 
		for (int p=-2; p<int(Blocks[0].Solid_phase.size()); p++) 
		{	int _t;
			if (p<0) _t=1; else _t=Blocks[0].Solid_phase[p]->n_phases;
			for (int l=0; l<_t; l++)
				for (int i=0; i<Blocks.size(); i++) 
				{	if (p==-2)
						ANS_constituents.setname(get_member_no(i,p,l,k),RXN().cons[k].name+"_"+Blocks[i].ID+"_"+"aq");
					else if (p==-1)
						ANS_constituents.setname(get_member_no(i,p,l,k),RXN().cons[k].name+"_"+Blocks[i].ID+"_"+"sorbed");
					else
						ANS_constituents.setname(get_member_no(i, p, l, k), RXN().cons[k].name + "_" + Blocks[i].ID + "_" + Solid_phase()[p].name + "_" + Solid_phase()[p].phase_names[l]);
				}
		}
	dtt = dt;
	dt0 = dt;
	
	wiggle_dt_mult = 1;
	max_wiggle_id = -1;
	pos_def_mult=1;
	pos_def_mult_Q=1;
	t=Timemin; 	
	J_update = true;
	J_update_C = true;
	J_update_Q = true;
	cr = 1;
	int iii=0;
	int jjj=0;
	int fail_counter = 0;
	counter_flow = 0; counter_colloid=0; counter_const=0;
	failed_colloid=false; failed_const=false;

	if (write_details())
	{
		FILEBTC = fopen((outputpathname() + "Solution_details.txt").c_str(), "a");
		fprintf(FILEBTC, "Experiment %s: ", name);
		fclose(FILEBTC);
	}
	setH();      
	if (steady_state_hydro())
	{
		string failed_res = create_hydro_steady_matrix_inv();
		if (failed_res != "")
		{
			fail_reason = failed_res;
			return;
		}
	}
	setQ0();     
	

	vector<CRestoreInfo> Res;
	Res.push_back(getrestoreinfo());
	CRestoreInfo Res_temp;

	double dt_last = dtt;
	bool restore = true;
	bool redo = false;
	int redo_counter = 0;
	
	
	while (t-dtt<Timemax)
	{
		update_light_temperature();
		iii++;
		failed = true;failed_colloid = true; failed_const = true;
		while ((failed==true) || (failed_colloid==true) || (failed_const==true))
		{
			if (jjj%restore_interval() == 0) restore = true;
			if ((restore == true) && (dt_last <= dtt) && (redo == false)) { Res_temp = getrestoreinfo(); Res_temp.iii = iii; }
			if (!steady_state_hydro())
				onestepsolve_flow(dtt);
			else 
				onestepsolve_flow_bioest(dtt);
			vector<double> max_wiggle_v=ANS.max_wiggle();
			max_wiggle = max_wiggle_v[0]; max_wiggle_id = max_wiggle_v[1];
			if (max_wiggle>wiggle_tolerance()) 
			{	wiggle_dt_mult=wiggle_tolerance()/max_wiggle;
				if (dtt*wiggle_dt_mult/dt0<1e-6) wiggle_dt_mult=max(1.0,wiggle_dt_mult);
			}
			else
				wiggle_dt_mult=wiggle_tolerance()/(max_wiggle+wiggle_tolerance()*0.25);
			
			
			if (failed==false) 
			{	
				jjj++;
				if ((restore == true) && (dt_last<=dtt) && (redo==false))
				{
					if (write_details())
					{
						FILEBTC = fopen((outputpathname() +"Solution_Details.txt").c_str(), "a");  
						fprintf(FILEBTC, "restore point\n");
						fclose(FILEBTC);
					}
					Res.push_back(Res_temp);
					restore = false;
				}
				redo = false;
				set_block_fluxes();
				correct_S(dtt);
				if (colloid_transport()) failed_colloid = false;
				if (constituent_transport()) failed_const = false;
				if (colloid_transport())
				{
					onestepsolve_colloid(dtt);
					if (pos_def_limit())
						pos_def_mult = 1/(pos_def_ratio*dtt / dtt_J_c);
					else
						pos_def_mult = 4;
					if ((failed_colloid==true) || (counter_colloid>nr_iteration_treshold_min())) 
						J_update_C = true;
					if (failed_colloid==true)	
					{	
						J_update = true;
						dt_fail = dtt;
						if (write_details()) writedetails();
					}
					else
					{
						if (constituent_transport())
						{
							onestepsolve_const(dtt);
							if (pos_def_limit())
								pos_def_mult_Q = 1/(pos_def_ratio_const*dtt / dtt_J_q);
							else
								pos_def_mult_Q = 4;

							if ((failed_const==true) || (counter_const>nr_iteration_treshold_min())) 
								J_update_Q = true;
							if (failed_const==true)	
							{	J_update = true;
								J_update_C = true;
								dt_fail = dtt;
								
								if (write_details()) writedetails();
							}
							
						}
						else
						{
							failed_const = false;
							pos_def_mult_Q = 10000;
						}
					}
				}
				else 
				{	
					
					failed_colloid=false;
					pos_def_mult = 10000;
					pos_def_mult_Q = 10000;
				}
			
				if ((max_wiggle > 1) && (!redo))
				{
					redo_counter++;
					redo = true;
					Res = clean_up_restore_points(Res,t);
					doredo(Res[max(int(Res.size()) - redo_counter, 0)]);
					iii = Res[max(int(Res.size()) - redo_counter, 0)].iii;

					if (write_details())
					{
						FILEBTC = fopen((outputpathname() + "Solution_details.txt").c_str(), "a");  fprintf(FILEBTC, "redo\n"); fclose(FILEBTC);
					}
				}
				else
				{
					redo_counter = 0;
				}
			
			}
			else 
			{
				if (write_details()) writedetails();
				
			}

			if (!colloid_transport()) failed_colloid = false;
			if (!constituent_transport()) failed_const = false;

			if ((failed==true) || (failed_colloid==true) || (failed_const==true))
			{	fail_counter++;
				dtt*=dt_change_failure();
				J_update = true;
				dt_fail = dtt;

				if (fail_counter>1)
				{
					redo_counter++;
					redo = true;
					Res = clean_up_restore_points(Res, t);
					doredo(Res[max(int(Res.size())-redo_counter,0)]);

					iii = Res[max(int(Res.size()) - redo_counter, 0)].iii;
					if (write_details())
					{				
						FILEBTC = fopen((outputpathname() +"Solution_details.txt").c_str(), "a");  fprintf(FILEBTC, "redo\n");fclose(FILEBTC);
					}
				}
				else redo_counter = 0;
			}
			else
			{
				fail_counter = 0;
			}
			if (fail_counter>30)
			{	
				if (write_details())
				{
					FILEBTC = fopen((outputpathname() +"Solution_details.txt").c_str(), "a");  
					write_state(pathname() +"state.txt");
					fprintf(FILEBTC, "failed count > 30");
					fclose(FILEBTC);
				}
				return;
			}
// Sassan					
			if (stop_triggered)
			{
				failed = true;
				fail_reason = "Simulation was stopped by user";
				return;
			}

			if ((((iii>100) && ((t-Timemin)/double(iii)/dt0<avg_dt_limit())) || (epoch_count>epoch_limit())) && dt<=dt_last)
			{   if (write_details())
				{
					FILEBTC = fopen((outputpathname()+"Solution_details.txt").c_str(), "a");  
					write_state(outputpathname()+"state.txt");
					fprintf(FILEBTC, "dt too small, epoch = %i, avarage_dt = %e < %e", epoch_count, (t - Timemin) / double(iii), avg_dt_limit()*dt0);
					fclose(FILEBTC);
				}
				fail_reason = "dt too small, epoch = " + numbertostring(epoch_count) + ", avarage_dt = " + numbertostring((t - Timemin) / double(iii)) + "<" + numbertostring(avg_dt_limit()*dt0) + ", number of actual time-steps = " + numbertostring(iii);
				failed = true;   
				return;  
			}

		}
	
		dt_fail=max(dtt,dt_fail);
		
		
		//dt=dt*max(atoi(0.5*atoi(interpolate(Blocks[11].BTC.D, t))),1);
		double maxt = 100000;
		for (int ii=0; ii<Blocks.size(); ii++)
			for (int jj=0; jj<Blocks[ii].inflow.size(); jj++)
				if (Blocks[ii].inflow[jj].BTC[0].D.size() != 0)
					maxt= min(maxt,max(Blocks[ii].inflow[jj].BTC[0].interpol_D(t),dt0)); 

		dt_last = dtt;
		if (redo==false)
		{
			if (max(max(counter_flow,counter_colloid),counter_const)<nr_iteration_treshold_min()) 
			{	dtt=min(dtt*min(min(min(1/dt_change_rate(),wiggle_dt_mult),pos_def_mult),pos_def_mult_Q),maxt);
				J_update=false;
				J_update_C = false;
				J_update_Q = false;
			}
			else if (max(max(counter_flow,counter_colloid),counter_const)>nr_iteration_treshold_max()) 
			{	dtt=min(dtt*min(min(min(dt_change_rate(),wiggle_dt_mult),pos_def_mult),pos_def_mult_Q),maxt);  //dt=max(dt*dt_change_rate,dt0)??
				J_update = true;
				J_update_C = true;
				J_update_Q = true;
			}
			else
			{	J_update = true;
				dtt*=min(min(min(wiggle_dt_mult,1.0),pos_def_mult),pos_def_mult_Q);
			}
			dtt = min(dtt,dt_fail*1.1);
		}
		if (iii%max_J_interval()==0) 
		{	J_update = true;
			J_update_Q=true;
			J_update_C=true;
		}


		if (write_details()) writedetails();
// Sassan
		updateProgress();
		

		if (!redo)
		{
		for (int i=0; i<Blocks.size(); i++)
			ANS.BTC[i].append(t,Blocks[i].S);

		for (int i=0; i<Connector.size(); i++)
			ANS.BTC[i+Blocks.size()].append(t,Connector[i].Q*Connector[i].flow_factor);

		for (int i=0; i<Blocks.size(); i++)
			ANS.BTC[i+Blocks.size()+Connector.size()].append(t,Blocks[i].H);

		for (int i=0; i<Blocks.size(); i++)
			ANS.BTC[i+2*Blocks.size()+Connector.size()].append(t,Blocks[i].outflow_corr_factor*Blocks[i].get_evaporation(t));

		for (int i = 0; i<Connector.size(); i++)
			ANS.BTC[i + 3 * Blocks.size() + Connector.size()].append(t, Connector[i].A);

		for (int i = 0; i<Connector.size(); i++)
			ANS.BTC[i + 3 * Blocks.size() + 2*Connector.size()].append(t, Connector[i].Q_v);

		for (int i=0; i<measured_quan().size(); i++)
			if (measured_quan()[i].experiment == name)
				ANS_obs.BTC[i].append(t, get_var(measured_quan()[i].loc_type,measured_quan()[i].id, measured_quan()[i].quan));
		}


		
		Solution_dt.BTC[0].append(t,dtt);
		Solution_dt.BTC[1].append(t,double(counter_flow));
		Solution_dt.BTC[2].append(t,max_wiggle);

		if (colloid_transport())
			for (int j=0; j<Blocks[0].Solid_phase.size(); j++) 
				for (int k=0; k<Blocks[0].Solid_phase[j]->n_phases; k++)
					for (int i=0; i<Blocks.size(); i++) 	
						ANS_colloids.BTC[get_member_no(i,j,k)].append(t,Blocks[i].G[j][k]);	

		if (constituent_transport())
			for (int kk = 0; kk<RXN().cons.size(); kk++)
				for (int p=-2; p<int(Blocks[0].Solid_phase.size()); p++) 
				{	int _t;
					if (p<0) _t=1; else _t=Blocks[0].Solid_phase[p]->n_phases;
					for (int l=0; l<_t; l++)
						for (int i=0; i<Blocks.size(); i++) 
							ANS_constituents.BTC[get_member_no(i,p,l,kk)].append(t,Blocks[i].CG[kk][Blocks[i].get_member_no(p,l)]);	

				}
		
		if (mass_balance_check())
		{	Blocksmassbalance();
			for (int i=0; i<Blocks.size(); i++) ANS_MB.BTC[i].append(t,Blocks[i].MBBlocks);
		}

		setQ();
		if (!redo)
		{
			renew();
			renew_G();
			renew_CG();
			t += dtt;
		}
	}

// Sassan
	updateProgress(true);
	if (uniformoutput())
	{
		if (ANS.BTC[0].n)
		{
			ANS = ANS.make_uniform(dt0);
			if (colloid_transport()) ANS_colloids = ANS_colloids.make_uniform(dt0);
			if (constituent_transport()) ANS_constituents = ANS_constituents.make_uniform(dt0);
			ANS_obs.unif = false;
			ANS_obs = ANS_obs.make_uniform(dt0);
		}
	}
	failed = false;
	fail_reason = "Simulation conducted successfully";
}

bool CMedium::solve()
{
	evaluate_functions();
	setH();	// Set Total Head for all blocks
	evaluate_area(true);  //Update connector areas; 
	setQ();	//Set Q for all connectors
	initialize();
	

	ANS_obs = CBTCSet(measured_quan().size());
	ANS_obs.names.clear();
	for (int i = 0; i < measured_quan().size(); i++)
		ANS_obs.pushBackName(measured_quan()[i].name);

	if (solution_method() == 0)  
		solve_fts_m2(dt());   //fts= fixed time steps

	return true;
}


double CMedium::get_var(int i, string j, int k) //i: ID, j: type (i.e. connector vs. block), k: variable
{
	if (i==0)
		return Blocks[getblocksq(j)].get_val(k);
	else if (i==1)
		return Connector[getconnectorsq(j)].get_val(k);
}

double CMedium::get_var(int i, string j, string k) //i: ID, j: type (i.e. connector vs. block), k: variable
{
	if (i == 0)
		return Blocks[getblocksq(j)].get_val(k);
	else if (i == 1)
		return Connector[getconnectorsq(j)].get_val(k);
}

CVector CMedium::get_val(string I, int j) //I: quantity name, j: type (i.e. connector vs. block)
{
	CVector X;
	if (j==0)
		for (int i=0; i<Blocks.size(); i++)
			X.append(Blocks[j].get_val(I));
	return X;
}

void CMedium::set_param(int param_no, double _value)
{
	
	for (int i=0; i<parameters()[param_no].location.size(); i++)
	{	
		if (parameters()[param_no].location_type[i] == 2)

		{	Blocks[parameters()[param_no].location[i]].set_val(parameters()[param_no].quan[i],_value);
			Connector[parameters()[param_no].location[i]].set_val(parameters()[param_no].quan[i],_value);
		}
		else if (parameters()[param_no].location_type[i] == 0)
			Blocks[parameters()[param_no].location[i]].set_val(parameters()[param_no].quan[i],_value);
		else if (parameters()[param_no].location_type[i] == 1)
 			Connector[parameters()[param_no].location[i]].set_val(parameters()[param_no].quan[i],_value);
		
	}

}

void CMedium::finalize_set_param()
{
	for (int ii=0; ii<Blocks.size(); ii++)
	{	
		if (Blocks[ii].fs_params[5]==0)
			if (Blocks[ii].fs_params[4] != 0) Blocks[ii].fs_params[5]=1-1.0/Blocks[ii].fs_params[4];

		if (Blocks[ii].fs_params[4]==0)
			if (Blocks[ii].fs_params[5] != 0) Blocks[ii].fs_params[4]=1.0/(1.0-Blocks[ii].fs_params[5]);

		if (Blocks[ii].fs_params[5]==0)
			if (Blocks[ii].fs_params[4] != 0) Blocks[ii].fs_params[5]=1-1.0/Blocks[ii].fs_params[4];
		if (Blocks[ii].fs_params[4]==0)
			if (Blocks[ii].fs_params[5] != 0) Blocks[ii].fs_params[4]=1.0/(1.0-Blocks[ii].fs_params[5]);

		for (int jj = 0; jj<Blocks[ii].buildup_id.size(); jj++) if (lookup_buildup(Blocks[ii].buildup_id[jj])!=-1) Blocks[ii].buildup.push_back(&(buildup()[lookup_buildup(Blocks[ii].buildup_id[jj])]));
		for (int jj = 0; jj<Blocks[ii].envexchange_id.size(); jj++) if (lookup_external_flux(Blocks[ii].envexchange_id[jj]) != -1) Blocks[ii].envexchange.push_back(&(externalflux()[lookup_external_flux(Blocks[ii].envexchange_id[jj])]));
		for (int jj = 0; jj<Blocks[ii].evaporation_id.size(); jj++) if (lookup_evaporation(Blocks[ii].evaporation_id[jj]) != -1) Blocks[ii].evaporation_m.push_back(&(evaporation_model()[lookup_evaporation(Blocks[ii].evaporation_id[jj])])); //newly added
		
		for (int i = 0; i < Solid_phase().size(); i++) Blocks[ii].Solid_phase_id.push_back(i);
				
		//Resizing G and CG vectors
		for (int jj = 0; jj<Blocks[ii].Solid_phase_id.size(); jj++) Blocks[ii].Solid_phase.push_back(&(Solid_phase()[Blocks[ii].Solid_phase_id[jj]]));
		Blocks[ii].n_phases = Blocks[ii].get_tot_num_phases();
		
		Blocks[ii].G.resize(Blocks[ii].Solid_phase.size());
		Blocks[ii].G_star.resize(Blocks[ii].Solid_phase.size());
		Blocks[ii].G_stored_mass.resize(Blocks[ii].Solid_phase.size());//newly added
		for (int jj = 0; jj<Blocks[ii].Solid_phase.size(); jj++)
		{
			Blocks[ii].G[jj].resize(Blocks[ii].Solid_phase[jj]->n_phases);
			Blocks[ii].G_star[jj].resize(Blocks[ii].Solid_phase[jj]->n_phases);
			Blocks[ii].G_stored_mass[jj].resize(Blocks[ii].Solid_phase[jj]->n_phases);

		}

		Blocks[ii].CG.resize(RXN().cons.size());
		Blocks[ii].CG_star.resize(RXN().cons.size());
		Blocks[ii].CG_stored_mass.resize(RXN().cons.size());//newly added
		for (int kk = 0; kk<RXN().cons.size(); kk++)
		{
			Blocks[ii].CG[kk].resize(Blocks[ii].get_tot_num_phases() + 2);
			Blocks[ii].CG_stored_mass[kk].resize(Blocks[ii].get_tot_num_phases() + 2);//newly added
			Blocks[ii].CG_star[kk].resize(Blocks[ii].get_tot_num_phases() + 2);
		}

		
		if (colloid_transport())
		{	
			Blocks[ii].K.resize(Blocks[ii].Solid_phase.size());
			Blocks[ii].K_star.resize(Blocks[ii].Solid_phase.size());
			Blocks[ii].capacity_c.resize(Blocks[ii].Solid_phase.size());
			Blocks[ii].capacity_c_star.resize(Blocks[ii].Solid_phase.size());
			Blocks[ii].capacity_c_Q.resize(Blocks[ii].Solid_phase.size());
			Blocks[ii].capacity_c_star_Q.resize(Blocks[ii].Solid_phase.size());

			for (int jj = 0; jj<Blocks[ii].initial_g.size(); jj++)
			{
				int p = lookup_particle_type(Blocks[ii].initial_g_counter_p[jj]);
				int l = Solid_phase()[p].lookup_phase(Blocks[ii].initial_g_counter_l[jj]);
				if ((p!=-1) && (l!=-1)) Blocks[ii].G[p][l] = Blocks[ii].initial_g[jj];

			}
			for (int jj = 0; jj < Blocks[ii].initial_cg.size(); jj++)
			{
				int p;
				p = lookup_particle_type(Blocks[ii].initial_cg_counter_p[jj]);
				if (Blocks[ii].initial_cg_counter_p[jj] == "sorbed") p = -1;
				if ((Blocks[ii].initial_cg_counter_p[jj] == "") || (tolower(Blocks[ii].initial_cg_counter_p[jj]) == "aqueous")) p = -2;
				int l = 0;
				if (p>=0) l = Solid_phase()[p].lookup_phase(Blocks[ii].initial_cg_counter_l[jj]);
				int k = RXN().look_up_constituent_no(Blocks[ii].initial_cg_counter_c[jj]);
				if (k!=-1) Blocks[ii].CG[k][Blocks[ii].get_member_no(p,l)] = Blocks[ii].initial_cg[jj];
			}
		}
		Blocks[ii].RXN = &RXN();
		Blocks[ii].light = &light;
		Blocks[ii].temperature = &temperature;
		Blocks[ii].current_temperature = &current_temperature;
		Blocks[ii].current_light = &current_light;
		Blocks[ii].current_humidity = &current_relative_humidity;
		Blocks[ii].current_wind = &current_wind;
		
	}

	for (int i=0; i<Connector.size(); i++)
	{
		for (int j=0; j<Connector[i].flow_params.size(); j++)
			if (Connector[i].flow_params[j]==0)  
			{
				
				if (((Blocks[getblocksq(Connector[i].Block1ID)].indicator == Soil) || (Blocks[getblocksq(Connector[i].Block1ID)].indicator == Darcy)) && ((Blocks[getblocksq(Connector[i].Block2ID)].indicator == Pond) || (Blocks[getblocksq(Connector[i].Block2ID)].indicator == Stream)))
					Connector[i].flow_params[j] = Blocks[getblocksq(Connector[i].Block1ID)].fs_params[j];
				
				if (((Blocks[getblocksq(Connector[i].Block2ID)].indicator == Soil) || (Blocks[getblocksq(Connector[i].Block2ID)].indicator == Darcy)) && ((Blocks[getblocksq(Connector[i].Block1ID)].indicator == Pond) || (Blocks[getblocksq(Connector[i].Block1ID)].indicator == Stream)))
					Connector[i].flow_params[j] = Blocks[getblocksq(Connector[i].Block2ID)].fs_params[j];


				if (Blocks[getblocksq(Connector[i].Block1ID)].indicator == Blocks[getblocksq(Connector[i].Block2ID)].indicator) 
					Connector[i].flow_params[j]=0.5*(Blocks[getblocksq(Connector[i].Block1ID)].fs_params[j] + Blocks[getblocksq(Connector[i].Block2ID)].fs_params[j]);
				else if (Blocks[getblocksq(Connector[i].Block1ID)].indicator==0) 
					Connector[i].flow_params[j]=Blocks[getblocksq(Connector[i].Block1ID)].fs_params[j];
				else if (Blocks[getblocksq(Connector[i].Block2ID)].indicator==0)
					Connector[i].flow_params[j]=Blocks[getblocksq(Connector[i].Block2ID)].fs_params[j];
			} 


			if (Connector[i].A==0)  
			{
				if (Connector[i].const_area == true)
				{
					if (Blocks[getblocksq(Connector[i].Block1ID)].indicator == Blocks[getblocksq(Connector[i].Block2ID)].indicator)
						Connector[i].A = Connector[i].A_star = 0.5*(Blocks[getblocksq(Connector[i].Block1ID)].A + Blocks[getblocksq(Connector[i].Block2ID)].A);
					else if (Blocks[getblocksq(Connector[i].Block1ID)].indicator == Soil)
						Connector[i].A = Connector[i].A_star = Blocks[getblocksq(Connector[i].Block1ID)].A;
					else if (Blocks[getblocksq(Connector[i].Block2ID)].indicator == Soil)
						Connector[i].A = Connector[i].A_star = Blocks[getblocksq(Connector[i].Block2ID)].A;
				}
			} 

			if (Connector[i].d==0)  

			{	if (Blocks[getblocksq(Connector[i].Block1ID)].indicator == Blocks[getblocksq(Connector[i].Block2ID)].indicator)    
					Connector[i].d=fabs(Blocks[getblocksq(Connector[i].Block1ID)].z0 - Blocks[getblocksq(Connector[i].Block2ID)].z0);
				else if (Blocks[getblocksq(Connector[i].Block1ID)].indicator==0)
					Connector[i].d=Blocks[getblocksq(Connector[i].Block1ID)].V/Blocks[getblocksq(Connector[i].Block1ID)].A/2.0;
				else if (Blocks[getblocksq(Connector[i].Block2ID)].indicator==0)
					Connector[i].d=Blocks[getblocksq(Connector[i].Block2ID)].V/Blocks[getblocksq(Connector[i].Block2ID)].A/2.0;   
			}

			if ((Blocks[getblocksq(Connector[i].Block1ID)].indicator == Catchment) && (Blocks[getblocksq(Connector[i].Block2ID)].indicator == Catchment))
				if (Connector[i].flow_params[7] == 0) Connector[i].flow_params[7] = 0.667;
			
			Connector[i].c_dispersion.resize(Connector[i].Solid_phase.size());
			Connector[i].c_dispersion_star.resize(Connector[i].Solid_phase.size());
			Connector[i].dispersion.resize(RXN().cons.size());
			Connector[i].dispersion_star.resize(RXN().cons.size());
			Connector[i].RXN = &RXN();
	}
}

void CMedium::set_default_params()
{
	for (int i=0; i<parameters().size(); i++)
		set_param(i, parameters()[i].value);


	finalize_set_param();

	if (!Solid_phase().size())
		colloid_transport() = false;

	if (!RXN().cons.size())
		constituent_transport() = false;
	else
		if (constituent_transport())
			colloid_transport() = true;

	if (steady_state_hydro())
		for (int i = 0; i < Connector.size(); i++)
			if (Connector[i].d == 0) Connector[i].d = 1;
}


void CMedium::set_default()
{	
	
	nr_iteration_treshold_max() = 8;
	nr_iteration_treshold_min() = 4;
	dt_change_rate() = 0.75;
	dt_change_failure() = 0.1; 
	nr_failure_criteria() = 100;
	max_J_interval() = 100;
	writeinterval()=1;
	write_details()=false;
	wiggle_tolerance()=0.02;
	uniformoutput() = true; 
	mass_balance_check() = false;
	forward() = false;
	colloid_transport() = false;
	constituent_transport() = false;
	epoch_limit() = 500000;
	avg_dt_limit() = 1e-5;
	restore_interval() = 20;

	if (formulasA().size()==0) parent->set_formulas();
}

void CMedium::evaluate_functions()
{
	for (int i=0; i<Connector.size(); i++)
		Connector[i].evaluate_functions();

	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_functions();

}

void CMedium::get_funcs()
{
	for (int i=0; i<Connector.size(); i++)
		Connector[i].get_funcs(Connector[i].flow_expression);

	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].get_funcs(Blocks[i].H_S_expression);

}

int CMedium::getblocksq(string id)
{
	for (int i=0; i<Blocks.size(); i++)
		if (Blocks[i].ID == id) return i;

	return -1;
}

int CMedium::getconnectorsq(string id)
{
	for (int i = 0; i<Connector.size(); i++)
		if (Connector[i].ID == id) return i;

	return -1;
}

bool CMedium::is_there_any_fixed_connectors()
{
	for (int i=0; i<Connector.size(); i++)
		if (Connector[i].fixed==true) return true;
	for (int i=0; i<Blocks.size(); i++)
		if (Blocks[i].fixed_evaporation==true) return true;
	return false;

}


CVector CMedium::get_flow_factors()
{
	CVector X(Blocks.size());
	for (int i=0; i<Blocks.size(); i++)
		X[i]=Blocks[i].outflow_corr_factor;

	return X;
}

void CMedium::set_flow_factors(CVector &X)
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].outflow_corr_factor = X[i];


}

vector<int> CMedium::get_fixed_connect_status()
{
	vector<int> X;
	for (int i=0; i<Blocks.size(); i++)
		X.push_back(Blocks[i].setzero);

	return X;
}


void CMedium::set_fixed_connect_status(vector<int> X)
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].setzero=X[i];
}



void CMedium::set_block_fluxes()
{
	for (int i=0; i<Blocks.size(); i++)
	{
		double x = -10;
		double x_star = -10;  
		for (int j=0; j<Blocks[i].connectors.size(); j++)
		{	
			if ((fabs((Connector[Blocks[i].connectors[j]].Q-Connector[Blocks[i].connectors[j]].Q_v)/Connector[Blocks[i].connectors[j]].A))>x) x = fabs((Connector[Blocks[i].connectors[j]].Q-Connector[Blocks[i].connectors[j]].Q_v)/Connector[Blocks[i].connectors[j]].A);
			if ((fabs((Connector[Blocks[i].connectors[j]].Q_star-Connector[Blocks[i].connectors[j]].Q_v_star)/Connector[Blocks[i].connectors[j]].A_star))>x_star) x_star = fabs((Connector[Blocks[i].connectors[j]].Q_star-Connector[Blocks[i].connectors[j]].Q_v_star)/Connector[Blocks[i].connectors[j]].A_star);

		}
		Blocks[i].q = w()*x + (1-w())*x_star;

	}
}



void CMedium::evaluate_K()
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_K();
}

void CMedium::evaluate_K_star()
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_K_star();
}

void CMedium::evaluate_capacity()
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_capacity();
}

void CMedium::evaluate_capacity_star()
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_capacity_star();
	

}

void CMedium::evaluate_capacity_c()
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_capacity_c();
}

void CMedium::evaluate_capacity_c_star()
{
	for (int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_capacity_c_star();
	

}

void CMedium::evaluate_dispersion()
{
	for (int i=0; i<Connector.size(); i++)
	{	Connector[i].evaluate_dispersion();
		Connector[i].evaluate_dispersion_star();  
	}
}

void CMedium::evaluate_const_dispersion()
{
	for (int i=0; i<Connector.size(); i++)
	{	Connector[i].evaluate_const_dispersion();
		Connector[i].evaluate_const_dispersion_star();  
	}
}

void CMedium::onestepsolve_colloid(double dtt)
{
	if (Blocks[0].n_phases == 0)
	{
		failed_colloid = false;
		return;
	}
	evaluate_dispersion();   //Connector[i].evaluate_dispersion(); Connector[i].evaluate_dispersion_star();  //Negative diffusion due to negative f[8]????
	evaluate_K();			//Blocks[i].evaluate_K();  
	evaluate_capacity();	//Blocks[i].evaluate_capacity();  
	//renew_G();				
	
	CVector X(Blocks.size()*Blocks[0].n_phases);  
	CVector X_old(Blocks.size()*Blocks[0].n_phases);
	
	for (int p=0; p<Blocks[0].Solid_phase.size(); p++)
		for (int l=0; l<Blocks[0].Solid_phase[p]->n_phases; l++)
			for (int i=0; i<Blocks.size(); i++)  X[get_member_no(i,p,l)] =Blocks[i].G[p][l];

	X_old = X;
	CVector F = getres_C(X,dtt);  
	CVector F_old = F;
	counter_colloid = 0;
		
	if ((F == F) != true || (X == X) != true || (F.is_finite() == false) || (X.is_finite() == false))
	{	failed_colloid = true;
		fail_reason = "infinite X or F in colloids";
		pos_def_ratio = 1e-12;
		return; 
	}

	double err = 10000;
	double err_p = 10000;
	double errF;
	J_c_update_count = 0;
	while ((err>tol()) || (X.min()<double(0)))
	{
		if (J_update_C==true)
		{
			J_c_update_count++;
			M_C = Jacobian_C(X,dtt);
			epoch_count++;
			CMatrix M1 = normalize_diag(M_C,M_C);	
			if (M_C.getnumcols()>0) pos_def_ratio=M1.diag_ratio().abs_max(); else pos_def_ratio = 1e-12;	
			if (fabs(det(M1))<1e-30)
			{	set_G_star(X_old);
				fail_reason = "non strongly positive definite Jacobian in colloids";
				failed_colloid = true;
				return;
						
			}
			
			InvJ_C = inv(M1);
			if (InvJ_C.getnumcols() == 0)
			{
				fail_reason = "Colloid Jacobian in not inversible";
				failed_colloid = true;
				pos_def_ratio = 1e-12;
				return;
			}
			
			J_update_C=false;
			dtt_J_c = dtt;
		}
					
		X -= (dtt/dtt_J_c)*(InvJ_C*normalize_diag(F,M_C));

		F = getres_C(X,dtt);
		err_p = err;
		err = F.norm2();
		if (err>err_p) 
		{	J_update_C=true; 
			X = X_old;
			F = F_old;
			err = 10000;
		}

		counter_colloid++;
		if (counter_colloid>nr_failure_criteria()) 
		{
			set_G_star(X_old);
			fail_reason = "Number of iterations exceeded the limit in colloids";
			failed_colloid = true;
			return;
		}
	}

	set_G_star(X); //Added by Babak to avoid negative values in response 12/22/15
	
	if (X.min() < double(-1e-13))
	{
		fail_reason = "Negative value for colloid concentration";
		failed_colloid = true;
	}
	else
		failed_colloid = false;
}

void CMedium::onestepsolve_const(double dtt)
{
 	evaluate_const_dispersion();   
	//renew_CG();
	evaluate_capacity_c();
	int error_expand_counter=0;
	update_rxn_params();
	CVector X = get_X_from_CG();

	CVector X_old = X;
	CVector F = getres_Q(X,dtt);  
	CVector F_old = F;
	counter_const = 0;
		
	if ((F == F) != true || (X == X) != true || (F.is_finite() == false) || (X.is_finite() == false))
	{	failed_const=true;
		fail_reason = "infinite X or F in water quality";
		pos_def_ratio_const = 1e-12;
		return; 
	}
		
	double err = 10000;
	double err_p = 10000;
	double errF;
	J_q_update_count = 0;
	while (err>tol()) 
	{
		if (J_update_Q==true)
		{
			J_q_update_count++;
			M_Q = Jacobian_Q(X,dtt);
			CMatrix M1 = normalize_diag(M_Q,M_Q);	
			CMatrix D = M1.non_posdef_elems_m();
			epoch_count++;
			if (M_Q.getnumcols()>0) pos_def_ratio_const=M1.diag_ratio().abs_max(); else pos_def_ratio_const = 1e-12;
			if (pos_def_ratio_const > 1)
				Preconditioner_Q = M1.Preconditioner();
			else
				Preconditioner_Q = Identity(F.num);
			if (fabs(det(M1))<1e-30)
			{	set_CG_star(X_old);
				fail_reason = "Not strongly positive definite Jacobian in wq";
				failed_const = true;
				return;
						
			}
			
			InvJ_Q = inv(Preconditioner_Q*M1);
			if (InvJ_Q.getnumcols() == 0)
			{
				set_CG_star(X_old);
				fail_reason = "Matrix not invertible in wq";
				failed_const = true;
				return;

			}
			J_update_Q=false;
			dtt_J_q = dtt;
		}
				
		CVector dx = dtt / dtt_J_q*(InvJ_Q*Preconditioner_Q*normalize_diag(F, M_Q));
		
		X -= dx;

		F = getres_Q(X,dtt);
		err_p = min(err,err_p);
		err = F.norm2();
		if ((dx/X).abs_max()<1e-15) err=0;
		
		if (err / err_p>0.8) J_update_Q = true;

		if ((err>err_p) && (err>tol()))
		{
			error_expand_counter++;
			J_update_Q=true; 
			X = X_old;
			F = F_old;
		}

		counter_const++;
		
		if (error_expand_counter==3)
		{
			set_CG_star(X_old);
			fail_reason = "error expansion in wq";
			failed_const = true;
			return;
		}

		if (counter_const>nr_failure_criteria()) 
		{
			set_CG_star(X_old);
			fail_reason = "Number of iterations exceeded the limit in wq";
			failed_const = true;
			return;
		}
	}
	if (negative_concentration_allowed() == false)
	{
		if (X.min() < -1e-13)
		{
			vector<int> neg_vals_cons;
			vector<int> neg_vals_block;
			vector<double> neg_vals;
			for (int i = 0; i < X.getsize(); i++) if (X[i] < -1e-13) {
				neg_vals_block.push_back(get_member_no_inv(i)[0]); neg_vals_cons.push_back(get_member_no_inv(i)[3]);  neg_vals.push_back(X[i]);


			}
			
			fail_reason = "Negative value in constituent "; 
			for (int i = 0; i < neg_vals_block.size(); i++) fail_reason = fail_reason + RXN().cons[neg_vals_cons[i]].name; +", ";
			fail_reason = fail_reason + " concentration at ";
			for (int i = 0; i < neg_vals_block.size(); i++) fail_reason = fail_reason + Blocks[neg_vals_block[i]].ID; +", ";
			fail_reason = fail_reason + " values respectively = ";
			for (int i = 0; i < neg_vals_block.size(); i++) fail_reason = fail_reason + numbertostring(neg_vals[i]) + ", ";
			failed_const = true;
		}
		else
		{
			failed_const = false;
			fail_reason == "none";
		}
	}
	else
	{
		failed_const = false;
		fail_reason == "none";
	}
	
	//newly added
	if (failed_const == false)
	{
		fail_reason = "none";
		for (int k = 0; k < RXN().cons.size(); k++)
		{
			for (int p = -2; p<int(Blocks[0].Solid_phase.size()); p++)
			{
				int _t;
				if (p < 0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
				for (int l = 0; l < _t; l++)
				{
					// LHS
					for (int i = 0; i < Blocks.size(); i++)
					{
						if (get_capacity_star(i, l, p) > 1e-13)
						{
							Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)] = 0; // newly added
						}

					}
				}
			}
		}
	}
	else set_CG_star(X_old);
}




CVector CMedium::getres_C(const CVector &X, double dtt)
{

	CVector F(Blocks[0].n_phases*Blocks.size());

	set_G_star(X);	//Blocks[i].G_star[p] = X[i+Blocks.size()*p];
	evaluate_K_star();
	evaluate_capacity_star();  
	evaluate_dispersion();

	for (int p=0; p<Blocks[0].Solid_phase.size(); p++)
	{	for (int l=0; l<Blocks[0].Solid_phase[p]->n_phases; l++)
		{
			// LHS
			for (int i=0; i<Blocks.size(); i++)
			{
				F[get_member_no(i,p,l)] = (Blocks[i].capacity_c_star[p][l]*Blocks[i].G_star[p][l]-Blocks[i].capacity_c[p][l]*Blocks[i].G[p][l])/dtt;
				if ((l==0) && (Blocks[i].inflow.size()!=0)) 
					F[get_member_no(i,p,l)] -= sum_interpolate(Blocks[i].inflow, t,"flow")*sum_interpolate(Blocks[i].inflow, t,Solid_phase()[p].name); 
			}
						
			for (int i=0; i<Connector.size(); i++)
			{
				//advection
				double Q_adv_star = (Connector[i].Q_star - Connector[i].Q_v_star)*Connector[i].flow_factor + Connector[i].settling*Solid_phase()[p].vs*sgn(Connector[i].Block1->z0 - Connector[i].Block2->z0)*Connector[i].A_star;
				double Q_adv = (Connector[i].Q - Connector[i].Q_v)*Connector[i].flow_factor + Connector[i].settling*Solid_phase()[p].vs*sgn(Connector[i].Block1->z0 - Connector[i].Block2->z0)*Connector[i].A;

				if (((1-w())*Q_adv_star+w()*Q_adv)>0)
				{	F[get_member_no(getblocksq(Connector[i].Block1ID),p,l)] += (w()*Q_adv*Blocks[getblocksq(Connector[i].Block1ID)].G[p][l] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block1ID)].G_star[p][l])*Blocks[getblocksq(Connector[i].Block1ID)].Solid_phase[p]->mobility_factor[l];
					F[get_member_no(getblocksq(Connector[i].Block2ID),p,l)] -= (w()*Q_adv*Blocks[getblocksq(Connector[i].Block1ID)].G[p][l] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block1ID)].G_star[p][l])*Blocks[getblocksq(Connector[i].Block1ID)].Solid_phase[p]->mobility_factor[l];
				}
				if (((1 - w())*Q_adv_star + w()*Q_adv)<0)
				{	F[get_member_no(getblocksq(Connector[i].Block1ID),p,l)] += (w()*Q_adv*Blocks[getblocksq(Connector[i].Block2ID)].G[p][l] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block2ID)].G_star[p][l])*Blocks[getblocksq(Connector[i].Block2ID)].Solid_phase[p]->mobility_factor[l];
					F[get_member_no(getblocksq(Connector[i].Block2ID),p,l)] -= (w()*Q_adv*Blocks[getblocksq(Connector[i].Block2ID)].G[p][l] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block2ID)].G_star[p][l])*Blocks[getblocksq(Connector[i].Block2ID)].Solid_phase[p]->mobility_factor[l];
				}
			
			} 
				//diffusion
			for (int i=0; i<Connector.size(); i++)
			{	
				double exchange = Connector[i].A*(w()*Connector[i].c_dispersion[p]+(1-w())*Connector[i].c_dispersion_star[p])/Connector[i].d*((w()*Blocks[getblocksq(Connector[i].Block2ID)].G[p][l]+(1-w())*Blocks[getblocksq(Connector[i].Block2ID)].G_star[p][l])-(w()*Blocks[getblocksq(Connector[i].Block1ID)].G[p][l]+(1-w())*Blocks[getblocksq(Connector[i].Block1ID)].G_star[p][l]))*Blocks[getblocksq(Connector[i].Block1ID)].Solid_phase[p]->mobility_factor[l]*Blocks[getblocksq(Connector[i].Block2ID)].Solid_phase[p]->mobility_factor[l];
				F[get_member_no(getblocksq(Connector[i].Block1ID),p,l)] -= exchange;	
				F[get_member_no(getblocksq(Connector[i].Block2ID),p,l)] += exchange;  
			}
		
		}
		
	}

	// phase mass transfer
	for (int i=0; i<Blocks.size(); i++)
	{	
		for (int p=0; p<Blocks[i].Solid_phase.size(); p++)	
		{	for (int l=0; l<Blocks[i].Solid_phase[p]->n_phases; l++)
				for (int k=0; k<Blocks[i].Solid_phase[p]->n_phases; k++)
				{
					double exchange = (1-w())*Blocks[i].K_star[p][l][k]*Blocks[i].G_star[p][l] + w()*Blocks[i].K[p][l][k]*Blocks[i].G[p][l];
					F[get_member_no(i,p,l)] += exchange*Blocks[i].V;
					F[get_member_no(i,p,k)] -= exchange*Blocks[i].V;
				}
		}
	}

	// build_up
	for (int i=0; i<Blocks.size(); i++)
	{	
		for (int p=0; p<Blocks[i].Solid_phase.size(); p++)	
		{	for (int k=0; k<Blocks[i].Solid_phase[p]->n_phases; k++)
				for (int j=0; j<Blocks[i].buildup.size(); j++)
					if ((buildup()[j].name == Blocks[i].Solid_phase[p]->name) && (Blocks[i].buildup[j]->phase == Blocks[i].Solid_phase[p]->phase_names[k]))
						F[get_member_no(i,p,k)] -= Blocks[i].buildup[j]->buildup((1-w())*Blocks[i].G_star[p][k] + w()*Blocks[i].G[p][k],&Blocks[i])*Blocks[i].V;
		}
	}

	return F;
}

void CMedium::correct_S(double dtt)
{
	CVector S(Blocks.size());
	for (int i=0; i<Blocks.size(); i++)
	{
			if  (Blocks[i].inflow.size()!=0) 
				S[i] = Blocks[i].S+sum_interpolate(Blocks[i].inflow, t)[0]*dtt; 
			else S[i]= Blocks[i].S;
//			if (Blocks[i].indicator != Soil)
				S[i] -= Blocks[i].outflow_corr_factor*Blocks[i].get_evaporation(t)*dtt;
	}
	
	for (int i=0; i<Connector.size(); i++)
	{
		S[getblocksq(Connector[i].Block1ID)] -= (w()*Connector[i].Q + (1-w())*Connector[i].Q_star)*Connector[i].flow_factor*dtt;
		S[getblocksq(Connector[i].Block2ID)] += (w()*Connector[i].Q + (1-w())*Connector[i].Q_star)*Connector[i].flow_factor*dtt;
	}

	for (int i=0; i<Blocks.size(); i++) Blocks[i].S_star = max(S[i],0.0);
	
}

double CMedium::get_var(int i, vector<string> j, int k)
{
	double sum=0;
	for (int ii=0; ii<j.size(); ii++)
		sum += get_var(i,j[ii],k);

	return sum;

}

double CMedium::get_var(int i, vector<string> j, string k)
{
	double sum = 0;
	for (int ii = 0; ii<j.size(); ii++)
		sum += get_var(i, j[ii], k);

	return sum;

}

void CMedium::write_state(string filename)
{
	FILE *Fil;
	Fil = fopen(filename.c_str(), "w");
	
	fprintf(Fil, "dtt=%le\n", dtt);
	fprintf(Fil, "t=%le\n", t);
	for (int j=0; j<Blocks.size(); j++)
	{
		fprintf(Fil, "S-%i=%le\n", j, Blocks[j].S);
	}
	for (int j=0; j<Blocks.size(); j++)
	{
		fprintf(Fil, "outflow_corr_factor-%i=%le\n", j, Blocks[j].outflow_corr_factor);
	}
	for (int j=0; j<Blocks.size(); j++)
	{
		fprintf(Fil, "fixed_status-%i=%i\n", j, Blocks[j].fixed);
	}

	fclose(Fil);
}

void CMedium::read_state(string filename)
{
	ifstream file(filename);

	bool open_bracket = false;
		
	if (file.good())
		while (file.eof()==false)
		{
			vector<string> s;
			vector<string> keyword_param;
			vector<string> keyword;
			if (open_bracket == false)
			{	s = getline(file);  
				if (s.size()>0)	
				{	keyword_param = split(s[0],'=');  
					keyword = split(keyword_param[0],'-');
					if (keyword.size()>1)
						set_var(keyword[0],atoi(keyword[1].c_str()),atof(keyword_param[1].c_str()));
					else
						set_var(keyword[0],atof(keyword_param[1].c_str()));
				}
			}
		}

}

int CMedium::get_member_no(int block_no, int solid_id, int phase_no)
{
	int k=0;
	for (int i=0; i<solid_id; i++) k+=Blocks[0].Solid_phase[i]->n_phases*Blocks.size();
	return k+block_no+phase_no*Blocks.size();
}

vector<int> CMedium::get_phase_solid_id(int i)
{
	vector<int> k(3);
	int kk = i%Blocks.size(); 
	k[0] = i/Blocks[0].n_phases;
	int pp = 0;
	for (int ii=0; ii<Blocks[0].Solid_phase.size(); ii++)
		for (int jj=0; jj<Blocks[0].Solid_phase[ii]->n_phases; jj++)
		{ if (pp==i) {k[1]=ii, k[2]=jj;}
		  pp++;
		}
	return k;
}


CVector CMedium::getres_Q(const CVector &X, double dtt)
{
	CVector F(RXN().cons.size()*(Blocks[0].n_phases+2)*Blocks.size());
	set_CG_star(X);
	evaluate_capacity_c_star();
	for (int k=0; k<RXN().cons.size(); k++)
	{	for (int p=-2; p<int(Blocks[0].Solid_phase.size()); p++)
		{	int _t;
			if (p<0) _t=1; else _t=Blocks[0].Solid_phase[p]->n_phases;
			for (int l=0; l<_t; l++)
			{
				// LHS
				for (int i=0; i<Blocks.size(); i++)
				{
					F[get_member_no(i,p,l,k)] = (get_capacity_star(i,l,p)*Blocks[i].CG_star[k][get_member_no(p,l)]-get_capacity(i,l,p)*Blocks[i].CG[k][get_member_no(p,l)])/dtt;
					
					if ((p == -2) && (Blocks[i].inflow.size() != 0))//needs to be modified to account for colloid-associated inflow
					{
						F[get_member_no(i, p, l, k)] -= sum_interpolate(Blocks[i].inflow, t, "flow")*sum_interpolate(Blocks[i].inflow, t, RXN().cons[k].name);

						for (int j = 0; j < Blocks[i].evaporation_m.size(); j++) //newly added
						{
							if (Blocks[i].evaporation_m[j]->uptake==true)
								F[get_member_no(i, p, l, k)] += Blocks[i].get_evaporation(j, t)*(w()*Blocks[i].CG_star[k][get_member_no(p, l)] + (1 - w())*Blocks[i].CG[k][get_member_no(p, l)]);
						}

					}
				
						
				}
					



				for (int i=0; i<Connector.size(); i++)
				{
					//advection
					
					if (p==-2)
					{
							
						double Q_adv_star = (Connector[i].Q_star - Connector[i].Q_v_star)*Connector[i].flow_factor + Connector[i].settling*RXN().cons[k].vs*sgn(Connector[i].Block1->z0 - Connector[i].Block2->z0)*Connector[i].A_star;
						double Q_adv = (Connector[i].Q - Connector[i].Q_v)*Connector[i].flow_factor + Connector[i].settling*RXN().cons[k].vs*sgn(Connector[i].Block1->z0 - Connector[i].Block2->z0)*Connector[i].A;

						if (w()*Q_adv+(1-w())*Q_adv_star>0)
						{	F[get_member_no(getblocksq(Connector[i].Block1ID),p,l,k)] += (w()*Q_adv*Blocks[getblocksq(Connector[i].Block1ID)].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block1ID)].CG_star[k][get_member_no(p,l)]);
							F[get_member_no(getblocksq(Connector[i].Block2ID), p, l, k)] -= (w()*Q_adv*Blocks[getblocksq(Connector[i].Block1ID)].CG[k][get_member_no(p, l)] + (1 - w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block1ID)].CG_star[k][get_member_no(p, l)]);
							
						}
						if (Q_adv<0)
						{	F[get_member_no(getblocksq(Connector[i].Block1ID),p,l,k)] += (w()*Q_adv*Blocks[getblocksq(Connector[i].Block2ID)].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block2ID)].CG_star[k][get_member_no(p,l)]);
							F[get_member_no(getblocksq(Connector[i].Block2ID),p,l,k)] -= (w()*Q_adv*Blocks[getblocksq(Connector[i].Block2ID)].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block2ID)].CG_star[k][get_member_no(p,l)]);
						}

					}
					else if (p==-1)
					{


					}
					else
					{
						double Q_adv_star = (Connector[i].Q_star - Connector[i].Q_v_star)*Connector[i].flow_factor + Connector[i].settling*Solid_phase()[p].vs*sgn(Connector[i].Block1->z0 - Connector[i].Block2->z0);
						double Q_adv = (Connector[i].Q - Connector[i].Q_v)*Connector[i].flow_factor + Connector[i].settling*Solid_phase()[p].vs*sgn(Connector[i].Block1->z0 - Connector[i].Block2->z0);
						if (Q_adv>0)
						{	F[get_member_no(getblocksq(Connector[i].Block1ID),p,l,k)] += (w()*Q_adv*Blocks[getblocksq(Connector[i].Block1ID)].G[p][l]*Blocks[getblocksq(Connector[i].Block1ID)].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block1ID)].G_star[p][l]*Blocks[getblocksq(Connector[i].Block1ID)].CG_star[k][get_member_no(p,l)])*Blocks[getblocksq(Connector[i].Block1ID)].Solid_phase[p]->mobility_factor[l];
							F[get_member_no(getblocksq(Connector[i].Block2ID),p,l,k)] -= (w()*Q_adv*Blocks[getblocksq(Connector[i].Block1ID)].G[p][l]*Blocks[getblocksq(Connector[i].Block1ID)].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block1ID)].G_star[p][l]*Blocks[getblocksq(Connector[i].Block1ID)].CG_star[k][get_member_no(p,l)])*Blocks[getblocksq(Connector[i].Block1ID)].Solid_phase[p]->mobility_factor[l];
						}
						if (Q_adv<0)
						{	F[get_member_no(getblocksq(Connector[i].Block1ID),p,l,k)] += (w()*Q_adv*Blocks[getblocksq(Connector[i].Block2ID)].G[p][l]*Blocks[getblocksq(Connector[i].Block2ID)].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block2ID)].G_star[p][l]*Blocks[getblocksq(Connector[i].Block2ID)].CG_star[k][get_member_no(p,l)])*Blocks[getblocksq(Connector[i].Block2ID)].Solid_phase[p]->mobility_factor[l];
							F[get_member_no(getblocksq(Connector[i].Block2ID),p,l,k)] -= (w()*Q_adv*Blocks[getblocksq(Connector[i].Block2ID)].G[p][l]*Blocks[getblocksq(Connector[i].Block2ID)].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[getblocksq(Connector[i].Block2ID)].G_star[p][l]*Blocks[getblocksq(Connector[i].Block2ID)].CG_star[k][get_member_no(p,l)])*Blocks[getblocksq(Connector[i].Block2ID)].Solid_phase[p]->mobility_factor[l];
						}
					}
						
			
				} 
				//diffusion
				for (int i=0; i<Connector.size(); i++)
				{	
					if (p==-2)
					{
						double exchange = Connector[i].A*(w()*Connector[i].dispersion[k] + (1 - w())*Connector[i].dispersion_star[k]) / Connector[i].d*min(Heavyside(get_capacity_star(getblocksq(Connector[i].Block2ID), l, p) - 1e-13), Heavyside(get_capacity_star(getblocksq(Connector[i].Block1ID), l, p) - 1e-13));
						double term1 = w()*Blocks[getblocksq(Connector[i].Block2ID)].CG[k][get_member_no(p,l)]+(1-w())*Blocks[getblocksq(Connector[i].Block2ID)].CG_star[k][get_member_no(p,l)];
						double term2 = w()*Blocks[getblocksq(Connector[i].Block1ID)].CG[k][get_member_no(p,l)]+(1-w())*Blocks[getblocksq(Connector[i].Block1ID)].CG_star[k][get_member_no(p,l)];
						exchange*=(term1-term2);
						F[get_member_no(getblocksq(Connector[i].Block1ID),p,l,k)] -= exchange;	
						F[get_member_no(getblocksq(Connector[i].Block2ID),p,l,k)] += exchange;
					}
					else if (p==-1)
					{

					}
					else
					{
						double exchange = Connector[i].A*(w()*Connector[i].c_dispersion[p] + (1 - w())*Connector[i].c_dispersion_star[p]) / Connector[i].d*min(Heavyside(get_capacity_star(getblocksq(Connector[i].Block1ID), l, p) - 1e-13), Heavyside(get_capacity_star(getblocksq(Connector[i].Block2ID), l, p) - 1e-13));;
						double term1 = w()*Blocks[getblocksq(Connector[i].Block2ID)].G[p][l]*Blocks[getblocksq(Connector[i].Block2ID)].CG[k][get_member_no(p,l)]+(1-w())*Blocks[getblocksq(Connector[i].Block2ID)].G_star[p][l]*Blocks[getblocksq(Connector[i].Block2ID)].CG_star[k][get_member_no(p,l)];
						double term2 = w()*Blocks[getblocksq(Connector[i].Block1ID)].G[p][l]*Blocks[getblocksq(Connector[i].Block1ID)].CG[k][get_member_no(p,l)]+(1-w())*Blocks[getblocksq(Connector[i].Block1ID)].G_star[p][l]*Blocks[getblocksq(Connector[i].Block1ID)].CG_star[k][get_member_no(p,l)];
						exchange*=(Blocks[getblocksq(Connector[i].Block1ID)].Solid_phase[p]->mobility_factor[l]*Blocks[getblocksq(Connector[i].Block2ID)].Solid_phase[p]->mobility_factor[l])*(term1-term2);
						F[get_member_no(getblocksq(Connector[i].Block1ID),p,l,k)] -= exchange;	
						F[get_member_no(getblocksq(Connector[i].Block2ID),p,l,k)] += exchange;  
					}
				}
		
			}
		
		}

		// solid mass transfer
		for (int i=0; i<Blocks.size(); i++)
		{	
			for (int p=0; p<Blocks[i].Solid_phase.size(); p++)	
			{	for (int l=0; l<Blocks[i].Solid_phase[p]->n_phases; l++)
					for (int kk=0; kk<Blocks[i].Solid_phase[p]->n_phases; kk++)
					{
						double exchange = (1-w())*Blocks[i].K_star[p][l][kk]*Blocks[i].G_star[p][l]*Blocks[i].CG_star[k][get_member_no(p,l)] + w()*Blocks[i].K[p][l][kk]*Blocks[i].G[p][l]*Blocks[i].CG[k][get_member_no(p,l)];
						F[get_member_no(i,p,l,k)] += exchange*Blocks[i].V;
						F[get_member_no(i,p,kk,k)] -= exchange*Blocks[i].V;
					}
			}
		}

		//solute mass transfer
		for (int i=0; i<Blocks.size(); i++)
		{
			for (int p=-1; p<int (Blocks[i].Solid_phase.size()); p++)	
			{	int _t;
				if (p<0) _t=1; else _t=Blocks[0].Solid_phase[p]->n_phases;
				for (int l=0; l<_t; l++)
				{
					double exchange=0;
					if (p==-1)
					{
						exchange -= (1-w())*Blocks[i].get_exchange_rate_star(p,l,k)*Blocks[i].CG_star[k][get_member_no(p,l)] + w()*Blocks[i].get_exchange_rate(p,l,k)*Blocks[i].CG[k][get_member_no(p,l)];
						exchange += ((1-w())*Blocks[i].get_exchange_rate_star(p,l,k)*Blocks[i].CG_star[k][get_member_no(-2,l)] + w()*Blocks[i].get_exchange_rate(p,l,k)*Blocks[i].CG[k][get_member_no(-2,l)])*Blocks[i].get_kd(p,l,k);
						F[get_member_no(i, -2, 0, k)] += exchange*get_capacity(i, l, p);
						F[get_member_no(i, p, l, k)] -= exchange*get_capacity(i, l, p);
					}
					else
					{
						exchange -= (1-w())*Blocks[i].get_exchange_rate_star(p,l,k)*Blocks[i].CG_star[k][get_member_no(p,l)] + w()*Blocks[i].get_exchange_rate(p,l,k)*Blocks[i].CG[k][get_member_no(p,l)];
						exchange += ((1 - w())*Blocks[i].get_exchange_rate_star(p, l, k)* Blocks[i].CG_star[k][get_member_no(-2, l)] + w()*Blocks[i].get_exchange_rate(p, l, k)*Blocks[i].CG[k][get_member_no(-2, l)]) * Blocks[i].get_kd(p, l, k);
						F[get_member_no(i, -2, 0, k)] += exchange*get_capacity(i, l, p);
						F[get_member_no(i, p, l, k)] -= exchange*get_capacity(i, l, p);

					}
				}
			}
		}

		//reactions
		CVector rate_chng = get_rxn_chng_rate();
		F -= rate_chng;
		

		// build_up
		for (int i=0; i<Blocks.size(); i++)
		{
			for (int p=-1; p<int (Blocks[i].Solid_phase.size()); p++)	
			{	int _t;
				if (p<0) _t=1; else _t=Blocks[0].Solid_phase[p]->n_phases;
				for (int l=0; l<_t; l++)
				{
					double exchange=0;
					if (p==-1)
					{
						for (int j=0; j<Blocks[i].buildup.size(); j++)
							if (Blocks[i].buildup[j]->phase == "sorbed")
								F[get_member_no(i,p,l,k)] -= Blocks[i].buildup[j]->buildup((1-w())*Blocks[i].CG_star[k][get_member_no(p,l)] + w()*Blocks[i].CG[k][get_member_no(p,l)],&Blocks[i])*Blocks[i].V;
					}
					else
					{
						for (int j=0; j<Blocks[i].buildup.size(); j++)
							if ((Blocks[i].buildup[j]->solid == Blocks[i].Solid_phase[p]->name) && (Blocks[i].buildup[j]->phase == Blocks[i].Solid_phase[p]->phase_names[l]))
								F[get_member_no(i,p,l,k)] -= Blocks[i].buildup[j]->buildup((1-w())*Blocks[i].CG_star[k][get_member_no(p,l)] + w()*Blocks[i].CG[k][get_member_no(p,l)],&Blocks[i])*Blocks[i].V;


					}
				}
			}
		}

		//External flux
		for (int i=0; i<Blocks.size(); i++)
		{
			for (int p=-2; p<int (Blocks[i].Solid_phase.size()); p++)	
			{	int _t;
				if (p<0) _t=1; else _t=Blocks[0].Solid_phase[p]->n_phases;
				for (int l=0; l<_t; l++)
				{
					double exchange=0;
					if (p==-2)
					{
						for (int j=0; j<Blocks[i].envexchange.size(); j++)
							if ((Blocks[i].envexchange[j]->phase == "aqueous") && (Blocks[i].envexchange[j]->constituent == RXN().cons[k].name))
								F[get_member_no(i,p,l,k)] -= w()*Blocks[i].envexchange[j]->calculate(&Blocks[i]) + (1-w())*Blocks[i].envexchange[j]->calculate_star(&Blocks[i]);
					}
					else if (p==-1)
					{
						for (int j=0; j<Blocks[i].envexchange.size(); j++)
							if ((Blocks[i].envexchange[j]->phase == "sorbed") && (Blocks[i].envexchange[j]->constituent == RXN().cons[k].name))
								F[get_member_no(i,p,l,k)] -= w()*Blocks[i].envexchange[j]->calculate(&Blocks[i]) + (1-w())*Blocks[i].envexchange[j]->calculate_star(&Blocks[i]);
					}
					else if (p>-1)
					{
						for (int j = 0; j<Blocks[i].envexchange.size(); j++)
							if ((Blocks[i].envexchange[j]->solid == Blocks[i].Solid_phase[p]->name) && (Blocks[i].envexchange[j]->phase == Blocks[i].Solid_phase[p]->phase_names[l]) && (Blocks[i].envexchange[j]->constituent == RXN().cons[k].name))
								F[get_member_no(i,p,l,k)] -= w()*Blocks[i].envexchange[j]->calculate(&Blocks[i]) + (1-w())*Blocks[i].envexchange[j]->calculate_star(&Blocks[i]);


					}
				}
			}
		}

	}

	// set concentration zero when the storage is zero
	for (int k = 0; k < RXN().cons.size(); k++)
	{
		for (int p = -2; p<int(Blocks[0].Solid_phase.size()); p++)
		{
			int _t;
			if (p < 0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
			for (int l = 0; l < _t; l++)
			{
				// LHS
				for (int i = 0; i < Blocks.size(); i++)
				{
					if (get_capacity_star(i, l, p) < 1e-13)
					{
						F[get_member_no(i, p, l, k)] = X.vec[get_member_no(i, p, l, k)];
						if (Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)]==0) 
						{ 
							Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)] = get_capacity(i, l, p)*Blocks[i].CG[k][Blocks[i].get_member_no(p, l)]; // newly added
							if ((p == -2) && (Blocks[i].inflow.size() != 0))//needs to be modified to account for colloid-associated inflow
							{
								Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)] += sum_interpolate(Blocks[i].inflow, t, RXN().cons[k].name)*dtt; // newly added
							}
						}
							
						if ((p == -2) && (Blocks[i].inflow.size() != 0))//needs to be modified to account for colloid-associated inflow
						{
							F[get_member_no(i, p, l, k)] -= sum_interpolate(Blocks[i].inflow, t, RXN().cons[k].name);
						}

					}
					else
						F[get_member_no(i, p, l, k)] -= Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)] / dtt; //newly added
						

				}
			}
		}
	}

	return F;
}

int CMedium::get_member_no(int block_no, int solid_id, int phase_no, int const_no)
{
	if (solid_id==-2) 
		return const_no*Blocks.size()+block_no;
	else if (solid_id==-1) 
		return (const_no+int(RXN().cons.size()))*Blocks.size()+block_no;
	else
	{
		int k=0;
		for (int i=0; i<solid_id; i++) k+=Blocks[0].Solid_phase[i]->n_phases*Blocks.size()*RXN().cons.size();
		return k+(phase_no*Blocks.size())*RXN().cons.size()+block_no+2*int(RXN().cons.size())*Blocks.size();
	}

}

vector<int> CMedium::get_member_no_inv(int ii)
{
	vector<int> v;
	
	for (int kk = 0; kk<RXN().cons.size(); kk++)
		for (int p = -2; p<int(Blocks[0].Solid_phase.size()); p++)
		{
			int _t;
			if (p < 0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
			for (int l = 0; l < _t; l++)
				for (int i = 0; i < Blocks.size(); i++)
					if (get_member_no(i, p, l, kk) == ii)
					{
						v.push_back(i);
						v.push_back(p);
						v.push_back(l);
						v.push_back(kk);
					}
		}

	return v;
}

vector<string> CMedium::get_everything_from_id(int x)
{
	vector<string> V;
	for (int block_no = 0; block_no < Blocks.size(); block_no++)
		for (int solid_id = -2; solid_id < Solid_phase().size(); solid_id++)
			for (int phase_no = 0; phase_no < Solid_phase()[solid_id].n_phases; phase_no++)
				for (int const_no = 0; const_no < RXN().cons.size(); const_no++)
					if (get_member_no(block_no, solid_id, phase_no, const_no) == x)
					{
						V.push_back(Blocks[block_no].ID);
						V.push_back(Solid_phase()[solid_id].name);
						V.push_back(Solid_phase()[solid_id].phase_names[phase_no]);
						V.push_back(RXN().cons[const_no].name);
						return V;
					}

	return vector<string>(4);
}


int CMedium::get_member_no(int solid_id, int phase_no)
{
	if (solid_id==-2) 
		return 0;
	else if (solid_id==-1) 
		return 1;
	else
	{
		int k=0;
		for (int i=0; i<solid_id; i++) k+=Blocks[0].Solid_phase[i]->n_phases;
		return k+phase_no+2;
	}

}

double CMedium::get_capacity(int block_no, int phase_no, int particle_no)
{
	if (particle_no==-2) 
		return Blocks[block_no].S;
	else if (particle_no==-1) 
		return Blocks[block_no].V*Blocks[block_no].bulk_density;
	else
		return Blocks[block_no].capacity_c[particle_no][phase_no] * Blocks[block_no].capacity_c_Q[particle_no][phase_no];
}

double CMedium::get_capacity_star(int block_no, int phase_no, int particle_no)
{
	if (particle_no==-2) 
		return Blocks[block_no].S_star;
	else if (particle_no==-1) 
		return Blocks[block_no].V*Blocks[block_no].bulk_density;
	else
		return Blocks[block_no].capacity_c_star[particle_no][phase_no] * Blocks[block_no].capacity_c_star_Q[particle_no][phase_no];

}

void CMedium::set_CG_star(const CVector &X)
{
	for (int k=0; k<RXN().cons.size(); k++)
		for (int p=-2; p<int(Blocks[0].Solid_phase.size()); p++)
		{	int _t=1; if (p>-1) _t=Blocks[0].Solid_phase[p]->n_phases;
			for (int l=0; l<_t; l++)
				for (int i=0; i<Blocks.size(); i++)  Blocks[i].CG_star[k][Blocks[i].get_member_no(p,l)] = X.vec[get_member_no(i,p,l,k)];
		}

}

void CMedium::set_CG(const CVector &X)
{
	for (int k=0; k<RXN().cons.size(); k++)
		for (int p=-2; p<int(Blocks[0].Solid_phase.size()); p++)
		{	int _t=1; if (p>-1) _t=Blocks[0].Solid_phase[p]->n_phases;
			for (int l=0; l<_t; l++)
				for (int i=0; i<Blocks.size(); i++)  Blocks[i].CG[k][Blocks[i].get_member_no(p,l)] = X.vec[get_member_no(i,p,l,k)];
		}

}

void CMedium::set_G(const CVector &X)
{
	for (int p=0; p<int(Blocks[0].Solid_phase.size()); p++)
		for (int l=0; l<Solid_phase()[p].n_phases; l++)
			for (int i=0; i<Blocks.size(); i++)  Blocks[i].G[p][l] = X.vec[get_member_no(i,p,l)];
}

CVector CMedium::get_X_from_CG()
{
	CVector X(Blocks.size()*(Blocks[0].n_phases+2)*RXN().cons.size());  
	
	for (int k=0; k<RXN().cons.size(); k++)
		for (int p=-2; p<int (Blocks[0].Solid_phase.size()); p++)
		{	int _t=1; if (p>-1) _t=Blocks[0].Solid_phase[p]->n_phases;
			for (int l=0; l<_t; l++)
				for (int i=0; i<Blocks.size(); i++)  X[get_member_no(i,p,l,k)] = Blocks[i].get_CG(p,l,k);
		}
	return X;
}
CVector CMedium::get_X_from_G()
{
	CVector X(Blocks.size()*Blocks[0].n_phases);  
		
	for (int p=0; p<Blocks[0].Solid_phase.size(); p++)
		for (int l=0; l<Blocks[0].Solid_phase[p]->n_phases; l++)
			for (int i=0; i<Blocks.size(); i++)  X[get_member_no(i,p,l)] =Blocks[i].G[p][l];

	return X;
}

CVector CMedium::get_rxn_chng_rate()
{
	CVector F(RXN().cons.size()*(Blocks[0].n_phases+2)*Blocks.size());
	for (int i=0; i<Blocks.size(); i++)
	{
		if (Blocks[i].perform_rxn)
		{
			CVector rate_change = w()*Blocks[i].get_rxn_change(false) + (1 - w())*Blocks[i].get_rxn_change(true);
			for (int k = 0; k < RXN().cons.size(); k++)
				for (int p = -2; p < int(Blocks[0].Solid_phase.size()); p++)
				{
					int _t = 1; if (p>-1) _t = Blocks[0].Solid_phase[p]->n_phases;
					for (int l = 0; l < _t; l++)
						F[get_member_no(i, p, l, k)] += rate_change[Blocks[i].get_member_no(p, l, k)] * (w()*get_capacity(i, l, p) + (1 - w())*get_capacity_star(i, l, p));
				}
		}

	}
	return F;

}

void CMedium::writedetails()
{
	FILE *FILEBTC;
	FILEBTC = fopen((outputpathname() + "Solution_details.txt").c_str(), "a");
	fprintf(FILEBTC, "dt:, %lf, %le, counters:, %i, %i, %i, J_updates:, %i, %i, %i, update_counts: %i, %i, %i, multis: %le, %le, pos_defs: %le, %le, wiggle: %le, %le, %le, %i, %s\n", t, dtt, counter_flow, counter_colloid, counter_const, J_update, J_update_C, J_update_Q, J_h_update_count, J_c_update_count, J_q_update_count, pos_def_mult, pos_def_mult_Q, pos_def_ratio, pos_def_ratio_const, max_wiggle, wiggle_dt_mult, dt_fail, max_wiggle_id, fail_reason.c_str());
	fclose(FILEBTC);
}

void CMedium::evaluate_area(bool all)
{
	for (int i = 0; i < Connector.size(); i++)
	{
		if ((Connector[i].const_area == false) || (all==true))
		{
			if ((Connector[i].A == 0) || (Connector[i].const_area == false))
			{
				Connector[i].A = Connector[i].calc(Connector[i].area_expression);
				Connector[i].A_star = Connector[i].calc_star(Connector[i].area_expression);
			}
		}
	}
}


int CMedium::lookup_external_flux(string S)
{
	int out = -1;
	for (int i = 0; i < externalflux().size(); i++)
		if (tolower(S) == tolower(externalflux()[i].name))

			return i;

	return out;

}

int CMedium::lookup_evaporation(string S)
{
	int out = -1;
	for (int i = 0; i < evaporation_model().size(); i++)
		if (tolower(S) == tolower(evaporation_model()[i].name))
			out = i;

	return out;

}

int CMedium::lookup_parameters(string S)
{
	int out = -1;
	for (int i = 0; i < parameters().size(); i++)
		if (tolower(S) == tolower(parameters()[i].name))

			return i;

	return out;
}
/*
int CMedium::lookup_observation(string S)
{
	int out = -1;
	for (int i = 0; i < measured_quan().size(); i++)
		if (tolower(S) == tolower(measured_quan()[i].name))

			return i;

	return out;
}

*/
int CMedium::lookup_particle_type(string S)
{
	int out = -1;
	for (int i = 0; i < Solid_phase().size(); i++)
		if (tolower(S) == tolower(Solid_phase()[i].name))

			return i;

	return out;
}

int CMedium::lookup_buildup(string S)
{
	int out = -1;
	for (int i = 0; i < buildup().size(); i++)
		if (tolower(S) == tolower(buildup()[i].name))

			return i;

	return out;
}

void CMedium::writetolog(string S)
{
	fstream file(outputpathname() + log_file_name());
	file << S << endl;
	file.close();

}

CRestoreInfo CMedium::getrestoreinfo()
{
	CRestoreInfo R;
	R.X_res = getS();
	R.corr_fac_res = get_flow_factors();
	R.fix_stats_res = get_fixed_connect_status();
	R.CG_res = get_X_from_CG();
	R.G_res = get_X_from_G();
	R.t_res = t;
	R.dt_res = dtt;

	return R;

}

void CMedium::doredo(CRestoreInfo &R)
{
	set_flow_factors(R.corr_fac_res);
	set_fixed_connect_status(R.fix_stats_res);
	set_var("s", R.X_res.vec);
	t = R.t_res;
	R.dt_res = R.dt_res*dt_change_failure();
	dtt = R.dt_res;
	setH();
	evaluate_area();
	setQ0();
	ANS.knockout(R.t_res);
	ANS_obs.knockout(R.t_res);
	ANS_colloids.knockout(R.t_res);
	ANS_constituents.knockout(R.t_res);
	ANS_MB.knockout(R.t_res);
	set_CG(R.CG_res);
	set_G(R.G_res);
}

void CMedium::update_rxn_params()
{
	if (temperature.size() > 0)
	{
		for (int i = 0; i < RXN().parameters.size(); i++)
			RXN().parameters[i].eff_value = RXN().parameters[i].value*pow(RXN().parameters[i].tempcorr, current_temperature - 20);
	}
	else
		for (int i = 0; i < RXN().parameters.size(); i++)
			RXN().parameters[i].eff_value = RXN().parameters[i].value;
}

void CMedium::update_light_temperature()
{
	if (light.size() > 0)
		current_light = light[0].interpol(t);
	else
		current_light = 0;

	if (temperature.size() > 0)
		current_temperature = temperature[0].interpol(t);
	else
		current_temperature = 20;


	if (wind.size() > 0)
		current_wind = wind[0].interpol(t);
	else
		current_wind = 0;


	if (r_humidity.size() > 0)
		current_relative_humidity = r_humidity[0].interpol(t);
	else
		current_relative_humidity = 0;

}

vector<CRestoreInfo> CMedium::clean_up_restore_points(vector<CRestoreInfo> &Res, double t)
{
	vector<CRestoreInfo> Res_out;
	for (int i = 0; i < Res.size(); i++)
	{
		if (Res[i].t_res <= t)
			Res_out.push_back(Res[i]);
	}
	return Res_out;
}

void CMedium::onestepsolve_flow_bioest(double dtt)
{
	CVector v = get_steady_hydro_RHS();
	CVector Q = hydro_steady_matrix_inv*v;
	for (int i = 0; i < Connector.size(); i++)
		Connector[i].Q_star = Q[i];
	failed = false;
	fail_reason = "none";
}

double CMedium::getflow(int connector_ID)
{
	double flow = 0;
	if (Connector[connector_ID].presc_flow)
	{
		Connector[connector_ID].Q_star = flow;
		Connector[connector_ID].flow_calc_done = true;
		return Connector[connector_ID].presc_flowrate.interpol(t);
	}
	if (Connector[connector_ID].flow_calc_done == true) 
		return Connector[connector_ID].Q_star;
	vector<int> num_pre_flows = get_num_block_unpres_inflows(connector_ID);
	if ((num_pre_flows[0] <= num_pre_flows[1]) || num_pre_flows[1]==1)
	{
		for (int j = 0; j < Connector[connector_ID].Block1->connectors.size(); j++)
		{
			if (Connector[connector_ID].Block1->connectors[j] != connector_ID)
				flow += getflow(Connector[connector_ID].Block1->connectors[j])*(2*Connector[connector_ID].Block1->connectors_se[j]-1);
		}
		for (int j = 0; j < Connector[connector_ID].Block1->inflow.size(); j++)
		{
			flow += Connector[connector_ID].Block1->inflow[j].interpolate(t)[0];
		}
	}
	else
	{
		for (int j = 0; j < Connector[connector_ID].Block2->connectors.size(); j++)
		{
			if (Connector[connector_ID].Block2->connectors[j] != connector_ID)
				flow -= getflow(Connector[connector_ID].Block2->connectors[j])*(2 * Connector[connector_ID].Block1->connectors_se[j] - 1);
		}
		for (int j = 0; j < Connector[connector_ID].Block2->inflow.size(); j++)
		{
			flow -= Connector[connector_ID].Block2->inflow[j].interpolate(t)[0];
		}

	}
	Connector[connector_ID].Q_star = flow;
	Connector[connector_ID].flow_calc_done = true;
	return flow;
}

vector<int> CMedium::get_num_block_unpres_inflows(int connector_ID)
{
	vector<int> num_inflows(2);
	
	int numpresflow = 0;
	for (int i = 0; i < Connector[connector_ID].Block1->connectors.size(); i++)
	{ 
		if (!Connector[Connector[connector_ID].Block1->connectors[i]].presc_flow) numpresflow++;
	}
	num_inflows[0] = numpresflow;

	numpresflow = 0;
	for (int i = 0; i < Connector[connector_ID].Block2->connectors.size(); i++)
	{
		if (!Connector[Connector[connector_ID].Block2->connectors[i]].presc_flow) numpresflow++;
	}
	num_inflows[1] = numpresflow;

	return num_inflows;
}

void CMedium::false_connector_flow_calc()
{
	for (int i = 0; i < Connector.size(); i++)
		Connector[i].flow_calc_done = false;

}

string CMedium::create_hydro_steady_matrix_inv()
{
	CMatrix M;
	for (int i = 0; i < Blocks.size(); i++)
	{
		if (Blocks[i].connectors.size() > 1)
		{	vector<double> v(Connector.size());
			for (int j = 0; j < Blocks[i].connectors.size(); j++)
				v[Blocks[i].connectors[j]] = 2 * Blocks[i].connectors_se[j] - 1;
			M.matr.push_back(v);
		}
	}
	for (int i = 0; i < Connector.size(); i++)
	{
		if (Connector[i].presc_flow)
		{
			vector<double> v(Connector.size());
			v[i] = 1;
			M.matr.push_back(v);
		}
	}
	M.setnumcolrows();
	string failed_res;
	if (M.getnumcols()>M.getnumrows())
	{
		failed_res = "Hydraulics is over-determined";
	}
	else if (M.getnumcols()<M.getnumrows())
	{
		failed_res = "Hydraulics is undetermined";
	}
	else 
		hydro_steady_matrix_inv = inv(M);
	return failed_res;
	

}

CVector CMedium::get_steady_hydro_RHS()
{
	vector<double> v;
	for (int i = 0; i < Blocks.size(); i++)
	{
		if (Blocks[i].connectors.size() > 1) v.push_back(-sum_interpolate(Blocks[i].inflow, t, "flow"));
	}
	for (int i = 0; i < Connector.size(); i++)
	{
		if (Connector[i].presc_flow)
			v.push_back(Connector[i].presc_flowrate.interpol(t));
	}
	
	return CVector(v);

}

CMedium::CMedium(CLIDconfig _lid_config, CMediumSet *_parent)
{
	parent = _parent;
	lid_config = _lid_config;
	if (!parent->set_features.formulas) set_default();
	if (!parent->set_features.environmental_vars) parent->f_get_environmental_params(_lid_config);
	f_get_environmental_params(); // load environmental parameters
	if (!parent->set_features.parameters) parent->f_get_params(_lid_config); //load charachterictics of unknown parameters *
	if (!parent->set_features.observed) parent->f_get_observed(_lid_config); // load properties of observed data *
	f_get_model_configuration(); // load model structure (blocks, connectors)
	if (!parent->set_features.solids) parent->f_get_particle_types(_lid_config); // load particle types *
	if (!parent->set_features.constituents) parent->f_get_constituents(_lid_config); // load constituent properties *
	f_set_default_connector_expressions();
	f_set_default_block_expressions();
	if (!parent->set_features.constituents) parent->f_get_reactions(_lid_config); // load reaction network
	if (!parent->set_features.buildup) parent->f_get_buildup(_lid_config); // load buildup properties *
	if (!parent->set_features.external_flux) parent->f_get_external_flux(_lid_config); // load External fluxes *
	if (!parent->set_features.evaporation) parent->f_get_evaporation_model(_lid_config);// load evaporation models

	log_file_name() = "log.txt";

	get_funcs();

	f_load_inflows();


	set_default_params();
}

void CMedium::create(CLIDconfig _lid_config, CMediumSet *_parent)
{
	Blocks.clear(); Connector.clear(); 
	parent = _parent;
	lid_config = _lid_config;
	if (!parent->set_features.formulas) set_default();
	if (!parent->set_features.environmental_vars) parent->f_get_environmental_params(_lid_config);
	f_get_environmental_params(); // load environmental parameters
	if (!parent->set_features.parameters) parent->f_get_params(_lid_config); //load charachterictics of unknown parameters *
	if (!parent->set_features.observed) parent->f_get_observed(_lid_config); // load properties of observed data *
	f_get_model_configuration(); // load model structure (blocks, connectors)
	if (!parent->set_features.solids) parent->f_get_particle_types(_lid_config); // load particle types *
	if (!parent->set_features.constituents) parent->f_get_constituents(_lid_config); // load constituent properties *
	f_set_default_connector_expressions();
	f_set_default_block_expressions();
	if (!parent->set_features.constituents) parent->f_get_reactions(_lid_config); // load reaction network
	if (!parent->set_features.buildup) parent->f_get_buildup(_lid_config); // load buildup properties *
	if (!parent->set_features.external_flux) parent->f_get_external_flux(_lid_config); // load External fluxes *
	if (!parent->set_features.evaporation) parent->f_get_evaporation_model(_lid_config);// load evaporation models

	log_file_name() = "log.txt";

	get_funcs();

	f_load_inflows();

	set_default_params();
}


double& CMedium::w()
{
	return parent->SP.w;
}

vector<CSolid_Phase>& CMedium::Solid_phase()
{
	return parent->Solid_phase;
}

double& CMedium::tol()
{
	return parent->SP.tol;
}

double& CMedium::dt()
{
	return parent->SP.dt;
}

vector<range>& CMedium::parameters()
{
	return parent->parameters;
}

vector<double>& CMedium::std()
{
	return parent->std;
}

string& CMedium::pathname()
{
	return parent->FI.pathname;
}

string& CMedium::outputpathname()
{
	return parent->FI.outputpathname;
}

int& CMedium::solution_method()
{
	return parent->SP.solution_method;
}

string& CMedium::PE_info_filename()
{
	return parent->PE_info_filename;
}

string& CMedium::detoutfilename_obs()
{
	return parent->FI.detoutfilename_obs;
}

int& CMedium::writeinterval()
{
	return parent->FI.write_interval;
}

string& CMedium::realizeparamfilename()
{
	return parent->FI.realizeparamfilename;
}

int& CMedium::nr_iteration_treshold_max()
{
	return parent->SP.nr_iteration_treshold_max;
}

int &CMedium::nr_iteration_treshold_min()
{
	return parent->SP.nr_iteration_treshold_min;
}
double& CMedium::dt_change_rate()
{
	return parent->SP.dt_change_rate;
}
double& CMedium::dt_change_failure()
{
	return parent->SP.dt_change_failure;
}
int& CMedium::nr_failure_criteria()
{
	return parent->SP.nr_failure_criteria;
}

int& CMedium::max_J_interval()
{
	return parent->SP.max_J_interval;
}

vector<string>& CMedium::formulas()
{
	return parent->formulas.formulas;
}
vector<string>& CMedium::formulasH()
{
	return parent->formulas.formulasH;
}
vector<vector<string>>& CMedium::formulasQ()
{
	return parent->formulas.formulasQ;
}
vector<vector<string>>& CMedium::formulasQ2()
{
	return parent->formulas.formulasQ2;
}
vector<vector<string>>& CMedium::formulasA()
{
	return parent->formulas.formulasA;
}

vector<vector<bool>>& CMedium::const_area()
{
	return parent->formulas.const_area;

}
vector<bool>& CMedium::air_phase()
{
	return parent->formulas.air_phase;

}
vector<vector<bool>>& CMedium::vaporTransport()
{
	return parent->formulas.vaporTransport;

}
vector<vector<bool>>& CMedium::settling()
{
	return parent->formulas.settling;
}

bool& CMedium::write_details()
{
	return parent->FI.write_details;
}

double& CMedium::wiggle_tolerance()
{
	return parent->SP.wiggle_tolerance;
}

bool& CMedium::uniformoutput()
{
	return parent->FI.uniformoutput;
}
bool& CMedium::mass_balance_check()
{
	return parent->SP.mass_balance_check;
}
bool& CMedium::colloid_transport()
{
	return parent->SP.colloid_transport;
}
bool& CMedium::constituent_transport()
{
	return parent->SP.constituent_transport;
}
bool& CMedium::forward()
{
	return parent->SP.forward;
}
int& CMedium::epoch_limit()
{
	return parent->SP.epoch_limit;
}

double & CMedium::avg_dt_limit()
{
	return parent->SP.avg_dt_limit;
}

int& CMedium::restore_interval()
{
	return parent->SP.restore_interval;
}

CRxnNetwork& CMedium::RXN()
{
	return parent->RXN;
}

string& CMedium::log_file_name()
{
	return parent->FI.log_file_name;
}

bool& CMedium::pos_def_limit()
{
	return parent->SP.pos_def_limit;
}

bool& CMedium::negative_concentration_allowed()
{
	return parent->SP.negative_concentration_allowed;
}

bool& CMedium::steady_state_hydro()
{
	return parent->SP.steady_state_hydro;
}

vector<measured_chrc>& CMedium::measured_quan()
{
	return parent->measured_quan;
}

CBTCSet& CMedium::measured_data()
{
	return parent->measured_data;
}

vector<int> CMedium::get_relevant_measured_quans()
{
	vector<int> v;
	for (int i = 0; i < measured_quan().size(); i++)
		if (measured_quan()[i].experiment == name) v.push_back(i);
	
	return v;
}

vector<CBuildup>& CMedium::buildup()
{
	return parent->buildup;
}
vector<CEnvExchange>& CMedium::externalflux()
{
	return parent->externalflux;
}
vector<CEvaporation>& CMedium::evaporation_model()
{
	return parent->evaporation_model;
}

#endif
