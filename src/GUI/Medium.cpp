//#ifndef DEBUG_MATRIX
//	#define DEBUG_MATRIX
//#endif
#ifdef GIFMOD

#include "stdafx.h"
#include "Medium.h"
#include "BTCSet.h"
#include "Precipitation.h"
#include "MediumSet.h"
#include "Matrix_arma.h"
#include "Vector_arma.h"
#ifdef QT_version
#include "qmessagebox.h"
#endif // QT_version
#include "Vector.h"


#ifndef QT_version
#define string2QString string2QString_nqt
#else
#define string2QString string2QString_qt
#endif


using namespace std;

CMedium::CMedium(bool create_parent)
{
    #ifdef Debug_API
    show_message("Creating MediumSet...");
    #endif // Debug_API
    if (create_parent)
        parent = new CMediumSet();
    else
        parent = 0;
#ifdef QT_version
    showmessages = false;
#else
    showmessages = true;
#endif // QT_version
#ifdef Debug_API
    show_message("CMedium Created!");
#endif // Debug_API
}

CMedium::~CMedium(void)
{
    Results.ANS.clear();
    Results.ANS_colloids.clear();
    Results.ANS_constituents.clear();
    Results.ANS_obs.clear();
    Results.ANS_obs_noise.clear();
	//qDebug() << "Deleting Blocks";
	Blocks.clear();
	//qDebug() << "Deleting Connectors";
    Connectors.clear();
	//qDebug() << "Connectors deleted";

	parent = 0;
#ifdef QT_version
	gw = 0;
	runtimewindow = 0;
#endif // QT_version
	//qDebug() << "Setting parents to Null";
}

CMedium::CMedium(const CMedium &M)
{
	parent = M.parent;
	name = M.name;

	InvJ1 = M.InvJ1;
	InvJ2 = M.InvJ2;
    Solution_State = M.Solution_State;
	J_update = M.J_update;


    Results = M.Results;


	Blocks = M.Blocks;
    Connectors = M.Connectors;

	lid_config = M.lid_config;

	Timemax = M.Timemax;
	Timemin = M.Timemin;

	detoutfilename_hydro = M.detoutfilename_hydro;
	detoutfilename_wq = M.detoutfilename_wq;
	detoutfilename_prtcle = M.detoutfilename_prtcle;
	detoutfilename_control = M.detoutfilename_control;

    Precipitation = M.Precipitation;
	Precipitation_filename = M.Precipitation_filename;

    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        Connectors[i].Block1 = &Blocks[getblocksq(Connectors[i].Block1ID)];
        Connectors[i].Block2 = &Blocks[getblocksq(Connectors[i].Block2ID)];
        Connectors[i].Block1N = getblocksq(Connectors[i].Block1ID);
        Connectors[i].Block2N = getblocksq(Connectors[i].Block2ID);
        Connectors[i].RXN = &RXN();
	}

	for (unsigned int ii=0; ii<Blocks.size(); ii++)
	{
		if (Blocks[ii].fs_params[5]==0)
			if (Blocks[ii].fs_params[4] != 0) Blocks[ii].fs_params[5]=1-1.0/Blocks[ii].fs_params[4];

		if (Blocks[ii].fs_params[4]==0)
			if (Blocks[ii].fs_params[5] != 0) Blocks[ii].fs_params[4]=1.0/(1.0-Blocks[ii].fs_params[5]);

		Blocks[ii].RXN = &RXN();
	}

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

    InvJ1 = M.InvJ1;
	InvJ2 = M.InvJ2;

    Solution_State = M.Solution_State;
	J_update = M.J_update;

    Results = M.Results;


	Blocks = M.Blocks;
    Connectors = M.Connectors;

	lid_config = M.lid_config;

	Timemax = M.Timemax;
	Timemin = M.Timemin;

	detoutfilename_hydro = M.detoutfilename_hydro;
	detoutfilename_wq = M.detoutfilename_wq;
	detoutfilename_prtcle = M.detoutfilename_prtcle;
	detoutfilename_control = M.detoutfilename_control;

    Precipitation = M.Precipitation;
	Precipitation_filename = M.Precipitation_filename;


    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        Connectors[i].Block1 = &Blocks[getblocksq(Connectors[i].Block1ID)];
        Connectors[i].Block2 = &Blocks[getblocksq(Connectors[i].Block2ID)];
        Connectors[i].RXN = &RXN();
	}

	for (unsigned int ii = 0; ii<Blocks.size(); ii++)
	{
		if (Blocks[ii].fs_params[5] == 0)
			if (Blocks[ii].fs_params[4] != 0) Blocks[ii].fs_params[5] = 1 - 1.0 / Blocks[ii].fs_params[4];

		if (Blocks[ii].fs_params[4] == 0)
			if (Blocks[ii].fs_params[5] != 0) Blocks[ii].fs_params[4] = 1.0 / (1.0 - Blocks[ii].fs_params[5]);

		Blocks[ii].RXN = &RXN();
	}

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


void CMedium::get_state(const CMedium &M)
{
	InvJ1 = M.InvJ1;
	InvJ2 = M.InvJ2;
	Blocks = M.Blocks;
    Connectors = M.Connectors;

    Solution_State = M.Solution_State;
	J_update = M.J_update;


}






void CMedium::f_get_environmental_params()
{
    for (unsigned int i=0; i<lid_config.keyword.size(); i++)
	{
        if (tolower(lid_config.keyword[i])=="time_min") {Timemin = atof(lid_config.value[i].c_str()); Solution_State.t=Timemin;}
		if (tolower(lid_config.keyword[i])=="time_max") Timemax = atof(lid_config.value[i].c_str());

		if (tolower(lid_config.keyword[i])=="precipitation") Precipitation_filename.push_back(lid_config.value[i]);
		//if (tolower(lid_config.keyword[i])=="evaporation") Evaporation_filename.push_back(lid_config.value[i]);
		if (tolower(lid_config.keyword[i]) == "temperature") temperature_filename.push_back(lid_config.value[i]);
		if (tolower(lid_config.keyword[i]) == "light") light_filename.push_back(lid_config.value[i]);
		if ((tolower(lid_config.keyword[i]) == "relative_humidity") || (tolower(lid_config.keyword[i]) == "humidity")) r_humidity_filename.push_back(lid_config.value[i]);
		if (tolower(lid_config.keyword[i]) == "wind") wind_filename.push_back(lid_config.value[i]);


		if (tolower(lid_config.keyword[i])=="hydro_detout_filename")
		{   vector<string> names = split_curly_semicolon(lid_config.value[i]);
		for (unsigned int ii=0; ii<names.size(); ii++)
			detoutfilename_hydro = names[ii];
		}

		if (tolower(lid_config.keyword[i]) == "wq_detout_filename")
		{
			vector<string> names = split_curly_semicolon(lid_config.value[i]);
			for (unsigned int ii = 0; ii<names.size(); ii++)
				detoutfilename_wq = names[ii];
		}

		if ((tolower(lid_config.keyword[i]) == "prtcl_detout_filename") || (tolower(lid_config.keyword[i]) == "particle_detout_filename"))
		{
			vector<string> names = split_curly_semicolon(lid_config.value[i]);
			for (unsigned int ii = 0; ii<names.size(); ii++)
				detoutfilename_prtcle = names[ii];
		}

	}

	if (detoutfilename_hydro.size() == 0) detoutfilename_hydro = "hydro_output_"+ name +".txt";
	if (detoutfilename_wq.size() == 0) detoutfilename_wq = "wq_output_"+name+".txt";
	if (detoutfilename_prtcle.size() == 0) detoutfilename_prtcle = "prtcl_output_"+name+".txt";
	if (detoutfilename_obs().size() == 0) detoutfilename_obs() = "observed_output.txt";
	if (detoutfilename_control.size() == 0) detoutfilename_control = "control_output.txt";

}

void CMedium::f_get_model_configuration()
{
    for (unsigned int i=0; i<lid_config.keyword.size(); i++)
	{

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

			for (unsigned int j=0; j<lid_config.param_names[i].size(); j++)
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
			for (unsigned int ii=0; ii<lid_config.est_param[i].size(); ii++)
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

			for (unsigned int j=0; j<lid_config.param_names[i].size(); j++)
			{
				C.flow_params.resize(n_flow_params);

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
				if (tolower(lid_config.param_names[i][j]) == "controller")
				{
					C.control = true;
					C.controller_id = lid_config.param_vals[i][j];
				}
			}
            Connectors.push_back(C);
			for (unsigned int ii=0; ii<lid_config.est_param[i].size(); ii++)
			{
				if (lookup_parameters(lid_config.est_param[i][ii])!=-1)
				{
                    parameters()[lookup_parameters(lid_config.est_param[i][ii])].location.push_back(Connectors.size() - 1);
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].quan.push_back(lid_config.param_names[i][ii]);
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].location_type.push_back(1);
					parameters()[lookup_parameters(lid_config.est_param[i][ii])].experiment_id.push_back(name);
				}
			}
		}

		if (tolower(lid_config.keyword[i])=="c_block")
		{
			string ID = lid_config.value[i].c_str();
			for (unsigned int j=0; j<lid_config.param_names[i].size(); j++)
			{
				Blocks[getblocksq(ID)].set_val(lid_config.param_names[i][j],atof(lid_config.param_vals[i][j].c_str()));
				if (tolower(lid_config.param_names[i][j])=="h_s_expression") {Blocks[getblocksq(ID)].H_S_expression = CStringOP(lid_config.param_vals[i][j]); Blocks[getblocksq(ID)].H_S_expression_txt=lid_config.param_vals[i][j];}
				if (tolower(lid_config.param_names[i][j])=="inflow") Blocks[getblocksq(ID)].inflow_filename.push_back(lid_config.param_vals[i][j]);
			}

		}

		if (tolower(lid_config.keyword[i])=="c_connector")
		{
			int ID = atoi(lid_config.value[i].c_str());
			for (unsigned int j=0; j<lid_config.param_names[i].size(); j++)
			{
                Connectors[ID].set_val(lid_config.param_names[i][j],atof(lid_config.param_vals[i][j].c_str()));  //only sets d value
                if (tolower(lid_config.param_names[i][j]) == "area_expression") { Connectors[ID].area_expression = CStringOP(lid_config.param_vals[i][j]); Connectors[ID].area_expression_strng = lid_config.param_vals[i][j]; Connectors[ID].const_area = false; }
                if (tolower(lid_config.param_names[i][j]) == "a") { Connectors[ID].const_area = true; }
                if (tolower(lid_config.param_names[i][j])=="flow_expression") {Connectors[ID].flow_expression = CStringOP(lid_config.param_vals[i][j]); Connectors[ID].flow_expression_strng =  lid_config.param_vals[i][j];} //flow-expression was fixed
                if (tolower(lid_config.param_names[i][j]) == "area_expression") { Connectors[ID].area_expression = CStringOP(lid_config.param_vals[i][j]); Connectors[ID].area_expression_strng = lid_config.param_vals[i][j]; } //flow-expression was fixed
                if (tolower(lid_config.param_names[i][j])=="block1") Connectors[ID].Block1ID = atoi(lid_config.param_vals[i][j].c_str());
                if (tolower(lid_config.param_names[i][j])=="block2") Connectors[ID].Block2ID = atoi(lid_config.param_vals[i][j].c_str());

			}
		}
	}

    for (unsigned int i=0; i<Connectors.size(); i++) Connectors[i].ID = Connectors[i].Block1ID + "_" + Connectors[i].Block2ID;
}

void CMedium::f_load_inflows()
{
	for (unsigned int j = 0; j < Precipitation_filename.size(); j++)
	{
		CPrecipitation P = CPrecipitation(pathname() + Precipitation_filename[j]);
		if (P.n>0) Precipitation.push_back(P);
	}
	for (unsigned int j = 0; j < evaporation_model().size(); j++)
	{
		if (evaporation_model()[j].evaporation_filename != "")
		{
			CBTC E = CBTC(pathname() + evaporation_model()[j].evaporation_filename);
			if (E.n>0) evaporation_model()[j].evaporation_TS = E;
		}

		if (evaporation_model()[j].single_crop_coefficient_filename != "")
		{
			CBTC E = CBTC(pathname() + evaporation_model()[j].single_crop_coefficient_filename);
			if (E.n>0) evaporation_model()[j].evaporation_TS = E;
		}
	}
	for (unsigned int j = 0; j<temperature_filename.size(); j++)
		{
		CBTC T = CBTC(pathname() + temperature_filename[j]);
		if (T.n>0) temperature.push_back(T);
	}

	for (unsigned int j = 0; j<r_humidity_filename.size(); j++)
	{
		CBTC T = CBTC(pathname() + r_humidity_filename[j]);
		if (T.n>0) r_humidity.push_back(T);
	}

	for (unsigned int j = 0; j<wind_filename.size(); j++)
	{
		CBTC T = CBTC(pathname() + wind_filename[j]);
		if (T.n>0) wind.push_back(T);
	}

	for (unsigned int j = 0; j<light_filename.size(); j++)
	{
		CBTC L = CBTC(pathname() + light_filename[j]);
		if (L.n>0) light.push_back(L);
	}

    for (unsigned int i=0; i<light.size(); i++)
		light[i] = light[i].make_uniform(dt());

    for (unsigned int i=0; i<temperature.size(); i++)
		temperature[i] = temperature[i].make_uniform(dt());


    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		for (unsigned int j = 0; j<Blocks[i].inflow_filename.size(); j++)
			Blocks[i].inflow.push_back(CBTCSet(pathname() + Blocks[i].inflow_filename[j], 1));


		if ((Blocks[i].indicator == Pond) || (Blocks[i].indicator == Catchment) || (Blocks[i].indicator == Stream))
		{
			if (Blocks[i].precipitation_swch == true)
				for (unsigned int j = 0; j<Precipitation_filename.size(); j++)
					Blocks[i].inflow.push_back(Precipitation[j].getflow(Blocks[i].A, 1.0 / 24.0 / 4));
		}
	}

    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        if (Connectors[i].pre_flow_filename != "") {
            Connectors[i].presc_flow = true; Connectors[i].presc_flowrate = CBTC(pathname() + Connectors[i].pre_flow_filename);
		}

	}

	f_make_uniform_inflows();
}

void CMedium::f_make_uniform_inflows()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		for (unsigned int j = 0; j<Blocks[i].inflow.size(); j++)
			Blocks[i].inflow[j] = Blocks[i].inflow[j].make_uniform(dt());
	}
    for (unsigned int i=0; i < evaporation_model().size(); i++)
		if (evaporation_model()[i].evaporation_TS.n>0)
			evaporation_model()[i].evaporation_TS = evaporation_model()[i].evaporation_TS.make_uniform(dt());
    for (unsigned int i=0; i<light.size(); i++)
		light[i] = light[i].make_uniform(dt());

    for (unsigned int i=0; i<temperature.size(); i++)
		temperature[i] = temperature[i].make_uniform(dt());

    for (unsigned int i=0; i<r_humidity.size(); i++)
		r_humidity[i] = r_humidity[i].make_uniform(dt());

    for (unsigned int i=0; i<wind.size(); i++)
		wind[i] = wind[i].make_uniform(dt());

    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        if (Connectors[i].presc_flow == true) Connectors[i].presc_flowrate = Connectors[i].presc_flowrate.make_uniform(dt());
	}

}





void CMedium::f_set_default_connector_expressions()
{
    for (unsigned int ii=0; ii<Connectors.size(); ii++)
	{
        if (Connectors[ii].flow_expression.terms.size() == 0)
		{
            if (Blocks[getblocksq(Connectors[ii].Block1ID)].z0 >= Blocks[getblocksq(Connectors[ii].Block2ID)].z0)
			{
                if (vaporTransport()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator] == true)
                {	Connectors[ii].flow_expression = CStringOP(formulasQ()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator]+ "+" + formulas()[Vapor]) ;
                    Connectors[ii].flow_expression_strng = formulasQ()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator]+ "+" + formulas()[Vapor] ;

				}
				else
                {	Connectors[ii].flow_expression = CStringOP(formulasQ()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator]) ;
                    Connectors[ii].flow_expression_strng = formulasQ()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator];

				}
			}
			else
			{
                if (vaporTransport()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator] == true)
				{
                    Connectors[ii].flow_expression = CStringOP(formulasQ2()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator] + "+" + formulas()[Vapor]);
                    Connectors[ii].flow_expression_strng = formulasQ2()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator] + "+" + formulas()[Vapor];
				}
				else
                {	Connectors[ii].flow_expression = CStringOP(formulasQ2()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator]) ;
                    Connectors[ii].flow_expression_strng = formulasQ2()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator];
				}
			}
            if (vaporTransport()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator])
			{
                Connectors[ii].flow_expression_v = CStringOP(formulas()[Vapor]);
                Connectors[ii].flow_expression_strng_v = formulas()[Vapor];
			}
		}
	}

    for (unsigned int ii = 0; ii < Connectors.size(); ii++)
	{
        Connectors[ii].area_expression = formulasA()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator];
        Connectors[ii].area_expression_strng = formulasA()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator];
        if (Connectors[ii].A == 0)
            Connectors[ii].const_area = const_area()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator];
		else
            Connectors[ii].const_area = true;
	}

    for (unsigned int ii = 0; ii < Connectors.size(); ii++)
	{
        if (Connectors[ii].settling == -1)
		{
            Connectors[ii].settling = settling()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator];
		}
	}


    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        for (unsigned int j = 0;  j< Solid_phase().size(); j++)Connectors[i].Solid_phase_id.push_back(j);
        Connectors[i].Block1 = &Blocks[getblocksq(Connectors[i].Block1ID)];
        Connectors[i].Block2 = &Blocks[getblocksq(Connectors[i].Block2ID)];
        Connectors[i].Block1N = getblocksq(Connectors[i].Block1ID);
        Connectors[i].Block2N = getblocksq(Connectors[i].Block2ID);
        Blocks[getblocksq(Connectors[i].Block1ID)].connectors.push_back(i);
        Blocks[getblocksq(Connectors[i].Block1ID)].connectors_se.push_back(0);
        Blocks[getblocksq(Connectors[i].Block2ID)].connectors.push_back(i);
        Blocks[getblocksq(Connectors[i].Block2ID)].connectors_se.push_back(1);
        for (unsigned int ii=0; ii<Connectors[i].Solid_phase_id.size(); ii++) Connectors[i].Solid_phase.push_back(&(Solid_phase()[Connectors[i].Solid_phase_id[ii]]));

	}
}

void CMedium::f_set_default_block_expressions()
{
	for (unsigned int ii=0; ii<Blocks.size(); ii++)
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
    for (unsigned int j=0; j<Connectors.size(); j++)
	{
        if (i==2) Connectors[j].A = v;
        if (i==6) Connectors[j].d = v;
        if (i==7) Connectors[j].Q = v;
        if (i==8) Connectors[j].v = v;
        if ((i>=50) && (i<100)) Connectors[j].flow_params[i-50] = v;

		//MM  ----------------->???
		//if ((i>=3000) && (i<3100))
        //	Connectors[i].dispersion[i-3000] = v;
		//if ((i>=3100) && (i<3199))
        //	Connectors[j].tr_alpha[i-3100] = v;
		//if ((i>=3200) && (i<3299))
        //	Connectors[j].dispersivity[i-3200] = v;
        if (i==4000)				Connectors[j].c_dispersion;
        if ((i>=5000) && (i<5100)) 	Connectors[j].dispersion[i-5000];
	}

	for (unsigned int j=0; j<Blocks.size(); j++)
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
	if (S=="dtt") Solution_State.dtt = v;
	if (S=="dt") dt() = v;

    for (unsigned int j=0; j<Connectors.size(); j++)
        Connectors[j].set_val(tolower(trim(S)), v);


	for (unsigned int j=0; j<Blocks.size(); j++)
		Blocks[j].set_val(tolower(trim(S)), v);
}

bool CMedium::set_property(const string &S, const string &v)
{
	bool success = true;
	if (tolower(trim(S))=="dtt") {Solution_State.dtt = atof(v.c_str()); return true;}
	if (tolower(trim(S))=="dt") {dt() = atof(v.c_str()); return true;}
    if (tolower(trim(S))=="name") {name = v; return true;}
    if (tolower(trim(S))=="tstart") {Timemin = atof(v.c_str()); return true;}
    if (tolower(trim(S))=="tend") {Timemax = atof(v.c_str()); return true;}

    for (unsigned int j=0; j<Connectors.size(); j++)
        success &= Connectors[j].set_val(tolower(trim(S)), atof(v.c_str()));


	for (unsigned int j=0; j<Blocks.size(); j++)
		success &= Blocks[j].set_val(tolower(trim(S)), atof(v.c_str()));

    return success;
}

bool CMedium::set_properties(const string &S)
{
    bool success = true;
    vector<string> ss = split(S);
    for (unsigned int i=0; i<ss.size(); i++)
    {
        vector<string> prop = split(ss[i],'=');
        if (prop.size()!=2)
        {
            build_errors.push_back("Syntax error [" + ss[i] + "] in '" + S + "'");
            show_message("Syntax error [" + ss[i] + "] in '" + S + "'");
            return false;
        }
        if (set_property(prop[0],prop[1]))
        {   show_message("Property [" + prop[0] + "] was set to " + prop[1]);
            success&=true;
        }
        else
        {
            build_errors.push_back("Property [" + prop[0] + "] was not found!");
            show_message("Property [" + prop[0] + "] was not found!");
            success&=false;
        }
    }
    return success;
}


void CMedium::set_var(const string &S, int i, const double &v)
{
    for (unsigned int j=0; j<Connectors.size(); j++)
        Connectors[j].set_val(tolower(trim(S)), v);

	for (unsigned int j=0; j<Blocks.size(); j++)
		Blocks[j].set_val(tolower(trim(S)), v);
}

void CMedium::set_var(const string &S, const vector<double> &v)
{
	for (unsigned int j=0; j<Blocks.size(); j++)
		Blocks[j].set_val(S, v[j]);
}

void CMedium::set_var(const string &S, const double &v, const int &start, const int &end)
{
	for (int j=start; j<end-1; j++)
        Connectors[j].set_val(tolower(trim(S)), v);


	for (int j=start; j<end; j++)
		Blocks[j].set_val(tolower(trim(S)), v);

}

void CMedium::set_var(const string &S, const vector<double> &v, const int &start, const int &end)
{
	for (int j=start; j<end-1; j++)
        Connectors[j].set_val(S, v[j]);

	for (int j=start; j<end; j++)
		Blocks[j].set_val(S, v[j]);

}

void CMedium::set_var(const string &S, const double &v, const int &n)
{
    for (unsigned int j=Connectors.size()-n+1; j<Connectors.size(); j++)
        Connectors[j].set_val(tolower(trim(S)), v);


	for (unsigned int j=Blocks.size()-n; j<Blocks.size(); j++)
		Blocks[j].set_val(tolower(trim(S)), v);

}


void CMedium::set_var(const string &S, const vector<double> &v, const int &n)
{
    for (unsigned int j=Connectors.size()-n+1; j<Connectors.size(); j++)
        Connectors[j].set_val(S, v[j]);

	for (unsigned int j=Blocks.size()-n; j<Blocks.size(); j++)
		Blocks[j].set_val(S, v[j]);

}




void CMedium::set_z0(double down, double up)
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].set_val("z0",down+i/(Blocks.size()-1)*(up-down));
	set_var("d", (up-down)/(Blocks.size()-1));
}

void CMedium::set_z0(double down)
{
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
        Blocks[i].set_val("z0",down+i*Connectors[0].d);   // Blocks[i].set_val("z0",down+(i+0.5)*Connectors[0].d); to set z0 for center of block
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


void CMedium::set_num_phases(int n)
{
	for (unsigned int j=0; j<Blocks.size(); j++)
		Blocks[j].set_num_phases(n);

}

void CMedium::set_num_constituents(int n)
{
	for (unsigned int j=0; j<Blocks.size(); j++)
		Blocks[j].set_num_constts(n);

}




CVector CMedium::getres_S(const CVector &X, const double &dt)
{
	CVector F(X.num);  //Create Vector F with size num
	setS_star(X);     //Blocks[i].set_val("S*", X[i]);
	CVector X1 = X;

    for (unsigned int i=0; i<Blocks.size(); i++)
	{	//if (Blocks[i].setzero==1) {dt=X.vec[i]; Blocks[i].S_star=0;}
        if (Blocks[i].setzero==2) {Blocks[i].outflow_corr_factor=X1[i];	Blocks[i].S_star=0;}
        if (Blocks[i].setzero!=0) X1[i]=0; else Blocks[i].outflow_corr_factor = 1;
	}


	setH_star();      //Blocks[i].set_val("H*", Blocks[i].calc_star(Blocks[i].H_S_expression));
	evaluate_area();  //Update connector areas;
    setQ_star();      //Connectors[i].set_val("Q*",Connectors[i].calc_star(Connectors[i].flow_expression));


    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		if  (Blocks[i].inflow.size()!=0)
            F[i]= (X1[i]-Blocks[i].S)/dt-sum_interpolate(Blocks[i].inflow, Solution_State.t,"flow");
		else F[i]= (X1[i]-Blocks[i].S)/dt;
        F[i]+=Blocks[i].outflow_corr_factor*Blocks[i].get_evaporation(Solution_State.t);
	}




    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        if  (w()*Connectors[i].Q + (1-w())*Connectors[i].Q_star>0) Connectors[i].flow_factor=Blocks[Connectors[i].Block1N].outflow_corr_factor;
        if  (w()*Connectors[i].Q + (1-w())*Connectors[i].Q_star<0) Connectors[i].flow_factor=Blocks[Connectors[i].Block2N].outflow_corr_factor;
	}

    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        F[Connectors[i].Block1N] += (w()*Connectors[i].Q + (1-w())*Connectors[i].Q_star)*Connectors[i].flow_factor;
        F[Connectors[i].Block2N] -= (w()*Connectors[i].Q + (1-w())*Connectors[i].Q_star)*Connectors[i].flow_factor;
	}



    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		if (Blocks[i].setzero==2)
		{
			bool all_out_flows_zero=true;
            if (Blocks[i].get_evaporation(Solution_State.t) > 1e-30) all_out_flows_zero=false;
			for (unsigned int k=0; k<Blocks[i].connectors.size(); k++)
                if ((w()*Connectors[Blocks[i].connectors[k]].Q + (1-w())*Connectors[Blocks[i].connectors[k]].Q_star)*(Blocks[i].connectors_se[k]-0.5)<0)
					all_out_flows_zero = false;
			if (all_out_flows_zero==true)
				F[i]=Blocks[i].outflow_corr_factor-1.1;
		}
	}

	return F;
}


CVector_arma CMedium::getres_S(CVector_arma &X, const double &dt)
{
	CVector_arma F(X.num);  //Create Vector F with size num
	setS_star(X);     //Blocks[i].set_val("S*", X[i]);
	CVector_arma X1 = X;

    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		//if (Blocks[i].setzero == 1) { dt = X[i]; Blocks[i].S_star = 0; }
		if (Blocks[i].setzero == 2) { Blocks[i].outflow_corr_factor = X1[i];	Blocks[i].S_star = 0; }
		if (Blocks[i].setzero != 0) X1[i] = 0; else Blocks[i].outflow_corr_factor = 1;
	}


	setH_star();      //Blocks[i].set_val("H*", Blocks[i].calc_star(Blocks[i].H_S_expression));
	evaluate_area();  //Update connector areas;
    setQ_star();      //Connectors[i].set_val("Q*",Connectors[i].calc_star(Connectors[i].flow_expression));


    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		if (Blocks[i].inflow.size() != 0)
            F[i] = (X1[i] - Blocks[i].S) / dt - sum_interpolate(Blocks[i].inflow, Solution_State.t, "flow");
		else F[i] = (X1[i] - Blocks[i].S) / dt;
        F[i] += Blocks[i].outflow_corr_factor*Blocks[i].get_evaporation(Solution_State.t);
	}

    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        if (w()*Connectors[i].Q + (1 - w())*Connectors[i].Q_star>0) Connectors[i].flow_factor = Connectors[i].Block1->outflow_corr_factor;
        if (w()*Connectors[i].Q + (1 - w())*Connectors[i].Q_star<0) Connectors[i].flow_factor = Connectors[i].Block2->outflow_corr_factor;
	}

    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        F[Connectors[i].Block1N] += (w()*Connectors[i].Q + (1 - w())*Connectors[i].Q_star)*Connectors[i].flow_factor;
        F[Connectors[i].Block2N] -= (w()*Connectors[i].Q + (1 - w())*Connectors[i].Q_star)*Connectors[i].flow_factor;
	}

    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		if (Blocks[i].setzero == 2)
		{
			bool all_out_flows_zero = true;
            if (Blocks[i].get_evaporation(Solution_State.t) > 1e-30) all_out_flows_zero = false;
			for (unsigned int k = 0; k<Blocks[i].connectors.size(); k++)
                if ((w()*Connectors[Blocks[i].connectors[k]].Q + (1 - w())*Connectors[Blocks[i].connectors[k]].Q_star)*(Blocks[i].connectors_se[k] - 0.5)<0)
					all_out_flows_zero = false;
			if (all_out_flows_zero == true)
				F[i] = Blocks[i].outflow_corr_factor - 1.1;
		}
	}

	return F;
}

void CMedium::setS_star(const CVector &X)
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].set_val_star(4, X.vec[i]);

}

void CMedium::setS_star(CVector_arma &X)
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].set_val_star(4, X[i]);

}

void CMedium::set_G_star(const CVector &X)
{
	for (unsigned int i=0; i<Blocks.size(); i++)
		for (unsigned int p=0; p<Blocks[i].Solid_phase.size() ; p++)
			for (int l=0; l<Blocks[i].Solid_phase[p]->n_phases ; l++)
				Blocks[i].G_star[p][l] = X.vec[get_member_no(i,p,l)];
}

void CMedium::setH_star()
{
	if (!steady_state_hydro())
        for (unsigned int i=0; i<Blocks.size(); i++)
			Blocks[i].set_val_star(1, Blocks[i].calc_star(Blocks[i].H_S_expression));
}

void CMedium::setH()
{
	if (!steady_state_hydro())
        for (unsigned int i=0; i<Blocks.size(); i++)
			Blocks[i].set_val(1, Blocks[i].calc(Blocks[i].H_S_expression));
}


double CMedium::calc_log_likelihood() //calculate sum log likelihood for time series data ts
{
	double sum=0;
	solve();
    if (Solution_State.failed==true) return -1e30;

    Results.ANS_obs.make_uniform(dt());
    for (unsigned int i=0; i<measured_quan().size(); i++)
		sum+=calc_log_likelihood(i);

	return sum;
}

double CMedium::calc_log_likelihood(int i) //calculate sum log likelihood for observed quantity i
{
	double sum=0;
	if (measured_quan()[i].error_structure == 0)
	{
		int k = measured_data().lookup(measured_quan()[i].name);
        if (k != -1) sum -= diff(Results.ANS_obs.BTC[i], measured_data().BTC[k]) / (2 * std()[measured_quan()[i].std_no] * std()[measured_quan()[i].std_no]);

	}
	if (measured_quan()[i].error_structure == 1)
	{
		int k = measured_data().lookup(measured_quan()[i].name);
        if (k != -1) sum -= diff(Results.ANS_obs.BTC[i].Log(1e-4), measured_data().BTC[k].Log(1e-4)) / (2 * std()[measured_quan()[i].std_no] * std()[measured_quan()[i].std_no]);

	}
	sum-=measured_data().BTC[i].n*log(std()[measured_quan()[i].std_no]);

	return sum;
}

void CMedium::setQ_star()
{
    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        if (Connectors[i].control)
		{
            if (Connectors[i].Controller->output.n>0)
                Connectors[i].set_val_star(7, Connectors[i].Controller->output.interpol(Solution_State.t));
			else
                Connectors[i].set_val_star(7, Connectors[i].Controller->value);
		}
        else if (!Connectors[i].presc_flow)
            Connectors[i].set_val_star(7, Connectors[i].calc_star(Connectors[i].flow_expression));
		else
            Connectors[i].set_val_star(7, Connectors[i].presc_flowrate.interpol(Solution_State.t));


        if (Connectors[i].vapor_transport)
            Connectors[i].set_val_star(13, Connectors[i].calc_star(Connectors[i].flow_expression_v));
        Connectors[i].flow_factor = 1;

	}

}

void CMedium::setQ0()
{
	if (!steady_state_hydro())
        for (unsigned int i=0; i < Connectors.size(); i++)
		{
            if (!Connectors[i].presc_flow)
                Connectors[i].set_val("q", Connectors[i].calc(Connectors[i].flow_expression));
			else
                Connectors[i].set_val("q", Connectors[i].presc_flowrate.interpol(Solution_State.t));


            Connectors[i].set_val("q_v", Connectors[i].calc(Connectors[i].flow_expression_v));
            Connectors[i].flow_factor = 1;

		}
	else
	{
		CVector Q = hydro_steady_matrix_inv*get_steady_hydro_RHS();
        for (unsigned int i=0; i < Connectors.size(); i++)
            Connectors[i].Q = Q[i];
	}

}

void CMedium::setQ()
{
    for (unsigned int i=0; i<Connectors.size(); i++)
    {   Connectors[i].set_val("q",Connectors[i].Q_star);
        Connectors[i].set_val("q_v",Connectors[i].Q_v_star);
	}
}

void CMedium::setQ(const CVector &X)
{

	CVector X_Temp=getS();
	set_var("S",X.vec);

    for (unsigned int i=0; i<Connectors.size(); i++)
        if (Connectors[i].fixed == false)
        {	Connectors[i].set_val("q",Connectors[i].calc(Connectors[i].flow_expression));
    Connectors[i].set_val("q_v",Connectors[i].calc(Connectors[i].flow_expression_v));
	}

	set_var("S",X_Temp.vec);

}


CVector CMedium::getH(const CVector &X)
{
	CVector H(X.num);
    for (unsigned int i=0; i<Blocks.size(); i++)
		H[i] = Blocks[i].calc_star(Blocks[i].H_S_expression);

	return H;
}

CVector CMedium::getQ(const CVector &X)
{
    CVector H(Connectors.size());
    for (unsigned int i=0; i < Connectors.size(); i++)
	{
        if (!Connectors[i].presc_flow)
            H[i] = Connectors[i].calc_star(Connectors[i].flow_expression);
		else
            H[i] = Connectors[i].presc_flowrate.interpol(Solution_State.t);

        if (Connectors[i].control)
            H[i] = Connectors[i].Controller->output.interpol(Solution_State.t);
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
        for (int i=0; i<X.num; i++)
			M[i] = Jacobian_S(X, F0, i, dt);  //M is a matrix
	}
	else
	{
//#pragma omp parallel for //ask arash
        for (int i=0; i<X.num; i++)
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
    CVector res = (F1 - F0)/epsilon;
    return res;
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


CVector CMedium::getLAI()
{
	//CVector X(Blocks.size()-2);
	CVector X(Blocks.size());
    for (unsigned int i=0; i<Blocks.size(); i++)
		X[i] = Blocks[i].plant_prop.LAI;
	return X;
}

CVector CMedium::getV()
{
	//CVector X(Blocks.size()-2);
	CVector X(Blocks.size());
    for (unsigned int i=0; i<Blocks.size(); i++)
		X[i] = Blocks[i].V;
	return X;
}



CVector CMedium::getS()
{
	//CVector X(Blocks.size()-2);
	CVector X(Blocks.size());
    for (unsigned int i=0; i<Blocks.size(); i++)
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

    for (unsigned int i=0; i<Blocks.size(); i++)
	{	if (Blocks[i].setzero==1) X_old[i]=dtt;
		if (Blocks[i].setzero==2) X_old[i]=Blocks[i].outflow_corr_factor;
	}
    Solution_State.fail_reason = "none";

	CVector correction_factor_old = get_flow_factors();
	int ini_max_error_elements = -1;
	while ((indicator==1) && (done<2))
	{
		bool fixed_connect;
		done+=1;

		int err_expand_counter=0;
		int J_update1, J_update2;
		J_update1 = J_update2 = J_update;

		CVector X = getS();    //getting the Storage of blocks, test
		fixed_connect=false;
        for (unsigned int i=0; i<Blocks.size(); i++)
		{	if (Blocks[i].setzero==1) X[i]=dtt;
			if (Blocks[i].setzero==2) X[i]=Blocks[i].outflow_corr_factor;
			if (Blocks[i].setzero!=0) fixed_connect=true;
		}

		CVector F = getres_S(X,dtt);

		double err = F.norm2();
		double err_p = err;

        Solution_State.counter_flow = 0;
		double lambda = 1;
        Solution_State.J_h_update_count = 0;
		while (err>tol())
		{
			if ((F==F)!=true || (X==X)!=true || (F.is_finite()==false) || (X.is_finite()==false) )
			{
                Solution_State.fail_reason = "indefinite X or F in hydro";
				set_fixed_connect_status(old_fixed_connect_status);
				setS_star(X_old);
				return;
			}


			if (((J_update1==true)||(InvJ1.getnumrows()==0))&&(fixed_connect==false))
			{
                Solution_State.J_h_update_count++;
				M = Jacobian_S(X,dtt);
				CMatrix M1 = normalize_diag(M,M);
				pos_def = M.diag_ratio();
                Solution_State.epoch_count++;

				InvJ1 = inv(M1);
				if (InvJ1.getnumcols()!=int(Blocks.size()))
				{
                    Solution_State.fail_reason = "Hydro Jacobian in not inversible";
					set_flow_factors(correction_factor_old);
					set_fixed_connect_status(old_fixed_connect_status);
					return;
				}
                Solution_State.dtt_J_h1 = dtt;
				J_update1=false;
			}

			if (((J_update2==true)||(InvJ2.getnumrows()==0))&&(fixed_connect==true))
			{
                Solution_State.J_h_update_count++;
				M = Jacobian_S(X,dtt);
				CMatrix M1 = normalize_diag(M,M);
                Solution_State.epoch_count++;
				pos_def = M.diag_ratio();

				InvJ2 = inv(M1);
				if (InvJ2.getnumcols()!=int(Blocks.size()))
				{
                    Solution_State.fail_reason = "Hydro Jacobian in not inversible";
					set_flow_factors(correction_factor_old);
					set_fixed_connect_status(old_fixed_connect_status);
					return;
				}
                Solution_State.dtt_J_h2 = dtt;
				J_update2=false;
			}

			CVector dx;
			if (fixed_connect)
			{
				dx = (InvJ2*normalize_diag(F, M));
                X -= lambda*((dtt / Solution_State.dtt_J_h2)*dx);
			}
			else
			{
				dx = (InvJ1*normalize_diag(F, M));
                X -= lambda*((dtt / Solution_State.dtt_J_h1)*dx);
			}


			F = getres_S(X,dtt);

			err_p = err;
			err = F.norm2();
			if (err/err_p>0.8)
			{
			    if (fixed_connect) J_update2=true; else J_update1=true;
			}

			if ((dx/X).abs_max()<1e-15) err=0;
			if (err>=err_p)
			{
				if (fixed_connect) J_update2 = true; else J_update1 = true; err_expand_counter++; X = X_old; set_flow_factors(correction_factor_old); lambda /= 2.0;
				if (ini_max_error_elements == -1) ini_max_error_elements = F.abs_max_elems();
				err = 10000;
			}
			if (err_expand_counter>4)
			{
                Solution_State.fail_reason = "Expanding error in hydro";
                Solution_State.fail_reason = Solution_State.fail_reason + ", max error @ " + Blocks[F.abs_max_elems()].ID;
				set_flow_factors(correction_factor_old);
				set_flow_factors(correction_factor_old);
				set_fixed_connect_status(old_fixed_connect_status);
				setS_star(X_old);
				return;
			}

			if ((err == err) == false)
			{
                Solution_State.fail_reason = "infinite error in hydro";
				set_flow_factors(correction_factor_old);
				set_fixed_connect_status(old_fixed_connect_status);
				setS_star(X_old);
				return;
			}


            Solution_State.counter_flow++;
            if (Solution_State.counter_flow>nr_failure_criteria())
			{
                Solution_State.fail_reason = "Number of iteration exceeded the limit in hydro";
				set_flow_factors(correction_factor_old);
				set_fixed_connect_status(old_fixed_connect_status);
				setS_star(X_old);
				return;
			}

		}

        Solution_State.failed = false;
        Solution_State.fail_reason = "none";
		indicator=0;

        for (int i=0; i<X.num; i++)
		{
			if ((X[i]<1e-15) && (X[i]>-1e-15)) X[i]=0;
			if ((Blocks[i].setzero==0) && (X[i]<0))
			{
				J_update=true;
				indicator=1;
                Solution_State.failed = true;
				if (Blocks[i].S==0)
					Blocks[i].setzero=2;
				else
					Blocks[i].setzero=2;
			}
			if (Blocks[i].outflow_corr_factor>1)
			{
                Solution_State.fail_reason = "block " + Blocks[i].ID + " is wet, " + "outflow factor = " + numbertostring(Blocks[i].outflow_corr_factor);
				J_update=true;
				indicator = 1;
                Solution_State.failed = true;
				Blocks[i].setzero=0;
				Blocks[i].outflow_corr_factor=1;
			}
		}
	}

}

void CMedium::Blocksmassbalance()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		Blocks[i].MBBlocks =(Blocks[i].S_star-Blocks[i].S);
        Blocks[i].MBBlocks -= sum_interpolate(Blocks[i].inflow, Solution_State.t)[0]* Solution_State.dtt;
        Blocks[i].MBBlocks +=Blocks[i].outflow_corr_factor*Blocks[i].get_evaporation(Solution_State.t)*Solution_State.dtt;
	}
    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        Blocks[Connectors[i].Block1N].MBBlocks += (w()*Connectors[i].Q + (1-w())*Connectors[i].Q_star)*Solution_State.dtt*Connectors[i].flow_factor;
        Blocks[Connectors[i].Block2N].MBBlocks -= (w()*Connectors[i].Q + (1-w())*Connectors[i].Q_star)*Solution_State.dtt*Connectors[i].flow_factor;
	}


}




void CMedium::renew()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		Blocks[i].A = Blocks[i].A_star;
		Blocks[i].CG = Blocks[i].CG_star;
		Blocks[i].G = Blocks[i].G_star;
		Blocks[i].H = Blocks[i].H_star;
		Blocks[i].S = Blocks[i].S_star;
		Blocks[i].V = Blocks[i].V_star;

	}

    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        Connectors[i].A = Connectors[i].A_star;
        Connectors[i].Q = Connectors[i].Q_star;
        Connectors[i].v = Connectors[i].v_star;
	}

}


void CMedium::renew_G()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
	Blocks[i].G = Blocks[i].G_star;
}

void CMedium::renew_CG()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].CG = Blocks[i].CG_star;
}


void CMedium::initialize()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		Blocks[i].A_star = Blocks[i].A;
		Blocks[i].CG_star = Blocks[i].CG;
		Blocks[i].G_star = Blocks[i].G;
		Blocks[i].H_star = Blocks[i].H;
		Blocks[i].S_star = Blocks[i].S;
		Blocks[i].V_star = Blocks[i].V;
		Blocks[i].G_star = Blocks[i].G;
	}

    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        Connectors[i].A_star = Connectors[i].A;
        Connectors[i].Q_star = Connectors[i].Q;
        Connectors[i].v_star = Connectors[i].v;
	}
}


void CMedium::do_plant_growth(double dtt)
{
    for (unsigned int i=0; i < Blocks.size(); i++)
		if (Blocks[i].indicator == Plant)
		{
			Blocks[i].V_star = max(Blocks[i].V + 0.5*(Blocks[i].calc(Blocks[i].plant_prop.plant_growth_rate_expression) + Blocks[i].calc_star(Blocks[i].plant_prop.plant_growth_rate_expression))*dtt,1e-6);
			Blocks[i].plant_prop.LAI = max(Blocks[i].plant_prop.LAI + 0.5*(Blocks[i].calc(Blocks[i].plant_prop.LAI_growth_rate_expression)+ Blocks[i].calc_star(Blocks[i].plant_prop.LAI_growth_rate_expression))*dtt,1e-5);
		}
}

void CMedium::initialize_ANSs()
{
	int max_phase;
	if (sorption())
		max_phase = 10000;
	else
		max_phase = -1;

	Results.ANS = CBTCSet(5 * Blocks.size() + 3 * Connectors.size());
	Results.ANS_colloids = CBTCSet(Blocks.size()*Blocks[0].n_phases);
	Results.ANS_constituents = CBTCSet(Blocks.size()*(Blocks[0].n_phases + n_default_phases)*RXN().cons.size());
	Results.ANS_control = CBTCSet(controllers().size());
	if (mass_balance_check())
	{
		Results.ANS_MB = CBTCSet(Blocks.size());
		Results.ANS_MB.names.clear();
		for (unsigned int i = 0; i < Blocks.size(); i++)
			Results.ANS_MB.pushBackName("S_" + Blocks[i].ID);
	}
	Results.ANS.names.clear();
	for (unsigned int i = 0; i < Blocks.size(); i++) Results.ANS.pushBackName("S_" + Blocks[i].ID);
	for (unsigned int i = 0; i < Connectors.size(); i++) Results.ANS.pushBackName("Q_" + Connectors[i].ID);
	for (unsigned int i = 0; i < Blocks.size(); i++) Results.ANS.pushBackName("H_" + Blocks[i].ID);
	for (unsigned int i = 0; i < Blocks.size(); i++) Results.ANS.pushBackName("E_" + Blocks[i].ID);
	for (unsigned int i = 0; i < Connectors.size(); i++) Results.ANS.pushBackName("A_" + Connectors[i].ID);
	for (unsigned int i = 0; i < Connectors.size(); i++) Results.ANS.pushBackName("Qv_" + Connectors[i].ID);
	for (unsigned int i = 0; i < Blocks.size(); i++) Results.ANS.pushBackName("V_" + Blocks[i].ID);
	for (unsigned int i = 0; i < Blocks.size(); i++) Results.ANS.pushBackName("LAI_" + Blocks[i].ID);

	for (unsigned int j = 0; j < Blocks[0].Solid_phase.size(); j++)
		for (int k = 0; k < Blocks[0].Solid_phase[j]->n_phases; k++)
			for (unsigned int i = 0; i < Blocks.size(); i++)
				Results.ANS_colloids.setname(get_member_no(i, j, k), "C_" + Blocks[i].ID + "_" + Solid_phase()[j].name + "_" + Solid_phase()[j].phase_names[k]);


	for (unsigned int k = 0; k < RXN().cons.size(); k++)
		for (int p = -2; p<min(int(Blocks[0].Solid_phase.size()), max_phase); p++)
		{
			int _t;
			if (p < 0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
			for (int l = 0; l < _t; l++)
				for (unsigned int i = 0; i < Blocks.size(); i++)
				{
					if (p == -2)
						Results.ANS_constituents.setname(get_member_no(i, p, l, k), RXN().cons[k].name + "_" + Blocks[i].ID + "_" + "aq");
					else if (p == -1)
						Results.ANS_constituents.setname(get_member_no(i, p, l, k), RXN().cons[k].name + "_" + Blocks[i].ID + "_" + "sorbed");
					else
						Results.ANS_constituents.setname(get_member_no(i, p, l, k), RXN().cons[k].name + "_" + Blocks[i].ID + "_" + Solid_phase()[p].name + "_" + Solid_phase()[p].phase_names[l]);
				}
		}

	for (unsigned int i = 0; i < controllers().size(); i++)
		Results.ANS_control.setname(i, controllers()[i].name);

	Solution_State.wiggle_dt_mult = 4;
	Solution_State.max_wiggle_id = -1;
	Solution_State.pos_def_mult = 1;
	Solution_State.pos_def_mult_Q = 1;
	Solution_State.t = Timemin;
	J_update = true;
	J_update_C = true;
	J_update_Q = true;

	Solution_State.counter_flow = 0; Solution_State.counter_colloid = 0; Solution_State.counter_const = 0;
	Solution_State.failed_colloid = false; Solution_State.failed_const = false;

	for (unsigned int i = 0; i < controllers().size(); i++)
		controllers()[i].output.clear();


}

void CMedium::solve_fts_m2(double dt)
{

	int max_phase;
	if (sorption())
		max_phase = 10000;
	else
		max_phase = -1;

	Solution_State.t = Timemin; //int oscillation_counter;
    Results.Solution_dt.clear();
    Results.Solution_dt = CBTCSet(3);
    Solution_State.dt_fail = 10000;
    Solution_State.epoch_count = 0;


	redo_parameters Redo_parameters;
	Redo_parameters.redo_time = Solution_State.t;
	Redo_parameters.redo_dt = 10000;
	Redo_parameters.redo_to_time = Solution_State.t;
	Redo_parameters.in_redo = false;

	initialize_ANSs();

	int iii = 0;
	int jjj = 0;
	int fail_counter = 0;

	Solution_State.dtt = dt;
	Solution_State.base_dtt = dt;
	dt0 = dt;

	if (write_details()) write_to_detail_file("Experiment " + name + ":");

	setH();
	if (steady_state_hydro())
	{
		string failed_res = create_hydro_steady_matrix_inv();
		if (failed_res != "")
		{
            Solution_State.fail_reason = failed_res;
			return;
		}
	}
	setQ0();



	Redo_parameters.Res.push_back(getrestoreinfo());
	CRestoreInfo Res_temp;

	double dt_last = Solution_State.dtt;
	avg_redo_dtt = Solution_State.dtt;

    time_t time_start = time(NULL);

    while (Solution_State.t - Solution_State.dtt < Timemax)
	{

		Redo_parameters.in_redo = (Solution_State.t < Redo_parameters.redo_time);

        show_status("t = " + numbertostring(Solution_State.t));
        Redo_parameters.in_redo = (Solution_State.t < Redo_parameters.redo_time);
		update_light_temperature();
		iii++;
        Solution_State.failed = true; Solution_State.failed_colloid = true; Solution_State.failed_const = true;
        while ((Solution_State.failed == true) || (Solution_State.failed_colloid == true) || (Solution_State.failed_const == true))
		{
			if (jjj%restore_interval() == 0) Redo_parameters.restore = true;
			if ((Redo_parameters.restore == true) && (dt_last <= Solution_State.dtt) && (Redo_parameters.redo == false))
			{
				Res_temp = getrestoreinfo(); Res_temp.iii = iii;
			}
			do_plant_growth(Solution_State.dtt);
			if (!steady_state_hydro())
				if (use_arma)
					onestepsolve_flow_ar(Solution_State.dtt);
				else
					onestepsolve_flow(Solution_State.dtt);
			else
				onestepsolve_flow_bioest(Solution_State.dtt);
            vector<int> max_wiggle_v = Results.ANS.max_wiggle_sl(Blocks.size(), wiggle_tolerance());
            Solution_State.max_wiggle = max_wiggle_v[0]; Solution_State.max_wiggle_id = max_wiggle_v[1];


            if (Solution_State.failed == false)
			{
				jjj++;
                if ((Redo_parameters.restore == true) && (dt_last <= Solution_State.dtt) && (Redo_parameters.redo == false) && Solution_State.t > Redo_parameters.redo_time)
				{
					if (write_details()) write_to_detail_file("Restore point: " + numbertostring(int(Redo_parameters.Res.size())) + ", dt = " + numbertostring(Res_temp.dt_res));

					Redo_parameters.Res.push_back(Res_temp);
					Redo_parameters.restore = false;
				}
				Redo_parameters.redo = false;
				set_block_fluxes();
				correct_S(Solution_State.dtt);
				if (colloid_transport()) Solution_State.failed_colloid = false;
				if (constituent_transport()) Solution_State.failed_const = false;
				if (colloid_transport())
				{
					if (use_arma)
						onestepsolve_colloid_ar(Solution_State.dtt);
					else
						onestepsolve_colloid(Solution_State.dtt);

					if (pos_def_limit())
                        Solution_State.pos_def_mult = 1 / (Solution_State.pos_def_ratio*Solution_State.dtt / Solution_State.dtt_J_c);
					else
                        Solution_State.pos_def_mult = 4;
                    if ((Solution_State.failed_colloid == true) || (Solution_State.counter_colloid > nr_iteration_treshold_min()))
						J_update_C = true;
					if (Solution_State.failed_colloid == true)
					{
                        J_update = true;
                        Solution_State.dt_fail = Solution_State.dtt;
						if (write_details()) writedetails();
					}
					else
					{
						if (constituent_transport())
						{
							if (use_arma)
								onestepsolve_const_ar(Solution_State.dtt);
							else
								onestepsolve_const(Solution_State.dtt);
							if (pos_def_limit())
                                Solution_State.pos_def_mult_Q = 1 / (Solution_State.pos_def_ratio_const*Solution_State.dtt / Solution_State.dtt_J_q);
							else
                                Solution_State.pos_def_mult_Q = 4;

                            if ((Solution_State.failed_const == true) || (Solution_State.counter_const > nr_iteration_treshold_min()))
								J_update_Q = true;
							if (Solution_State.failed_const == true)
							{
								J_update = true;
								J_update_C = true;
                                Solution_State.dt_fail = Solution_State.dtt;

								if (write_details()) writedetails();
							}

						}
						else
						{
							Solution_State.failed_const = false;
                            Solution_State.pos_def_mult_Q = 10000;
						}
					}
				}
				else
				{
					Solution_State.failed_colloid = false;
                    Solution_State.pos_def_mult = 10000;
                    Solution_State.pos_def_mult_Q = 10000;
				}

                if ((Solution_State.max_wiggle > 0.1) && (!Redo_parameters.redo) && check_oscillation() && Solution_State.dtt > 0.01*dt0)
				{
                    Solution_State.fail_reason = "Oscillation at: " + Blocks[Solution_State.max_wiggle_id].ID + " Redoing";
					doredo(Redo_parameters);

					iii = Redo_parameters.Res[max(int(Redo_parameters.Res.size()) - Redo_parameters.redo_counter, 0)].iii;

					if (write_details())
						write_to_detail_file("Redoing due to Oscillation!");

				}
				else
				{
					Redo_parameters.redo_counter = 0;
				}

			}
			else
				if (write_details()) writedetails();


#ifdef QT_version
			updateProgress();
#endif // QT_version

			if (!colloid_transport()) Solution_State.failed_colloid = false;
			if (!constituent_transport()) Solution_State.failed_const = false;

            if ((Solution_State.failed == true) || (Solution_State.failed_colloid == true) || (Solution_State.failed_const == true))
			{
				fail_counter++;
				Solution_State.base_dtt *= dt_change_failure();
				J_update = true;
                Solution_State.dt_fail = Solution_State.dtt;

				Solution_State.dtt = min(Solution_State.base_dtt, 1000 * avg_redo_dtt*1.2);

				if (controllers().size())
					Solution_State.dtt = min(Solution_State.dtt, get_nextcontrolinterval(Solution_State.t) - Solution_State.t);

				if (fail_counter > 3 || Solution_State.dtt<dt0*1e-4)
				{
					if (Solution_State.dtt<dt0*1e-8)
						Solution_State.fail_reason += " Redoing! Time step < " + numbertostring(Solution_State.dtt);
					if (fail_counter>3)
						Solution_State.fail_reason += " Redoing! Failed with three attempts";

					doredo(Redo_parameters);

					iii = Redo_parameters.Res[max(int(Redo_parameters.Res.size()) - Redo_parameters.redo_counter, 0)].iii;
					if (write_details())
					{
						if (Solution_State.dtt<dt0*1e-8)
							write_to_detail_file("Redoing for small dt, dt = " + numbertostring(Solution_State.dtt));
						if (fail_counter>3)
							write_to_detail_file("Redoing! Failed with three attempts");
					}

				}
				else Redo_parameters.redo_counter = 0;
			}
			else
			{
				fail_counter = 0;
			}
			if (fail_counter > 30)
			{
				if (write_details())
				{
					write_to_detail_file("Failed count > 30");
					write_state(outputpathname() + "state.txt");
					write_flows(outputpathname() + "flows.txt");
				}

                Solution_State.failed = true;
                Solution_State.fail_reason = "failed count > 30";
                for (unsigned int i=0; i < controllers().size(); i++)
					Results.ANS_control.BTC[i] = controllers()[i].output;

                show_message("Simulation Failed! + Number of unsuccessful time-step reductions > 30");
#ifdef QT_version
				updateProgress();

				if (runtimewindow != 0)
				{
					QMessageBox::warning(runtimewindow, "Simulation Failed", "Simulation Failed! + Number of unsuccessful time-step reductions > 30", QMessageBox::Ok);
				}
#endif // QT_version
				write_flows(outputpathname() + "flows.txt");
				return;
			}

			if (stop_triggered)
			{
                Solution_State.failed = true;
                Solution_State.fail_reason = "Simulation was stopped by user";
                for (unsigned int i=0; i < controllers().size(); i++)
                    Results.ANS_control.BTC[i] = controllers()[i].output;
				write_flows(outputpathname() + "flows.txt");
				if (write_details())
				{
					write_to_detail_file("Simulation ended by the user");

#ifdef QT_version
					if (runtimewindow != 0)
					{
						QMessageBox::StandardButton reply;
						QMessageBox::question(runtimewindow, "Simulation Stopped by the user", "Simulation Ended", QMessageBox::Ok);
					}
#endif // QT_version

				}

				return;
			}
            //double runtime = ((float)(clock() - time_start)) / CLOCKS_PER_SEC;
            long int runtime = (time(NULL) - time_start);
			if (runtime > maximum_run_time())
			{
                Solution_State.failed = true;
                Solution_State.fail_reason = "Simulation time exceeded the maximum simulation time";
                for (unsigned int i=0; i < controllers().size(); i++)
                    Results.ANS_control.BTC[i] = controllers()[i].output;
				write_flows(outputpathname() + "flows.txt");
				if (write_details()) write_to_detail_file("Simulation time exceeded the maximum simulation time");


#ifdef QT_version
				if (runtimewindow != 0)
				{
					QMessageBox::warning(runtimewindow, "Simulation Failed", "Runtime greater than the runtime limit set by the user", QMessageBox::Ok);
				}
#endif // QT_version


				return;
			}

            if (((((iii > 100) && ((Solution_State.t - Timemin) / double(iii) / dt0 < avg_dt_limit())) || (Solution_State.epoch_count > epoch_limit())) && dt <= dt_last) || (Solution_State.dtt<1e-20))
			{
				if (write_details())
				{
					if (Solution_State.dtt<1e-20)
						write_to_detail_file("dt = " + numbertostring(Solution_State.dtt) + " too small, epoch = " + numbertostring(Solution_State.epoch_count) + " average_dt = " + numbertostring((Solution_State.t - Timemin) / double(iii)));

					if ((Solution_State.t - Timemin) / double(iii) / dt0 < avg_dt_limit())
						write_to_detail_file("dt = " + numbertostring(Solution_State.dtt) + ", epoch = " + numbertostring(Solution_State.epoch_count) + " average_dt = " + numbertostring((Solution_State.t - Timemin) / double(iii)) + " < " + numbertostring(avg_dt_limit()*dt0));

					if (Solution_State.epoch_count > epoch_limit())
						write_to_detail_file("epoch = " + numbertostring(Solution_State.epoch_count) + " > " + numbertostring(epoch_limit()));

					write_state(outputpathname() + "state.txt");

				}
                Solution_State.fail_reason = "dt = " + numbertostring(Solution_State.dtt) + " too small, epoch = " + numbertostring(Solution_State.epoch_count) + ", average_dt = " + numbertostring((Solution_State.t - Timemin) / double(iii)) + "<" + numbertostring(avg_dt_limit()*dt0) + ", number of actual time-steps = " + numbertostring(iii);
                show_message("dt = " + numbertostring(Solution_State.dtt) + " too small, epoch = " + numbertostring(Solution_State.epoch_count) + ", average_dt = " + numbertostring((Solution_State.t - Timemin) / double(iii)) + "<" + numbertostring(avg_dt_limit()*dt0) + ", number of actual time-steps = " + numbertostring(iii));
                Solution_State.failed = true;
				write_flows(outputpathname() + "flows.txt");
                for (unsigned int i=0; i < controllers().size(); i++)
                    Results.ANS_control.BTC[i] = controllers()[i].output;
#ifdef QT_version
				if (runtimewindow != 0)
				{
                    if (Solution_State.epoch_count > epoch_limit())
                        QMessageBox::warning(runtimewindow, "Simulation Failed", "Number of epochs (" + QString::number(Solution_State.epoch_count) + " ) exceeded the limit (" + QString::number(epoch_limit()) + ")", QMessageBox::Ok);
                    if ((Solution_State.t - Timemin) / double(iii) / dt0 < avg_dt_limit())
                        QMessageBox::warning(runtimewindow, "Simulation Failed", "Average time-step size (" + QString::number((Solution_State.t - Timemin) / double(iii)) + " ) is too small < " + QString::number(avg_dt_limit()*dt0)  , QMessageBox::Ok);
				}
				updateProgress();
#endif // QT_version

				return;
			}

		}

        Solution_State.dt_fail = max(Solution_State.base_dtt, Solution_State.dt_fail);


		//dt=dt*max(atoi(0.5*atoi(interpolate(Blocks[11].BTC.D, t))),1);
		double maxt = 100000;
		for (unsigned int ii = 0; ii < Blocks.size(); ii++)
			for (unsigned int jj = 0; jj < Blocks[ii].inflow.size(); jj++)
				if (Blocks[ii].inflow[jj].BTC[0].D.size() != 0)
                    maxt = min(maxt, max(Blocks[ii].inflow[jj].BTC[0].interpol_D(Solution_State.t), dt0));

		dt_last = Solution_State.dtt;
		if (Redo_parameters.redo == false)
		{
			where_base_dtt_changed = 0;
            if (max(max(Solution_State.counter_flow, Solution_State.counter_colloid), Solution_State.counter_const) < nr_iteration_treshold_min())
			{
				Solution_State.base_dtt = min(Solution_State.base_dtt*min(min(min(1 / (pow(dt_change_rate(), 1 - double(Redo_parameters.in_redo) / 2.0)), Solution_State.wiggle_dt_mult), Solution_State.pos_def_mult), Solution_State.pos_def_mult_Q), maxt);
				where_base_dtt_changed = 11;
				J_update = false;
				J_update_C = false;
				J_update_Q = false;
			}
            else if (max(max(Solution_State.counter_flow, Solution_State.counter_colloid), Solution_State.counter_const) > nr_iteration_treshold_max())
			{
				Solution_State.base_dtt = min(Solution_State.base_dtt*min(min(min(dt_change_rate(), Solution_State.wiggle_dt_mult), Solution_State.pos_def_mult), Solution_State.pos_def_mult_Q), maxt);  //dt=max(dt*dt_change_rate,dt0)??
				where_base_dtt_changed = 12;
				J_update = true;
				J_update_C = true;
				J_update_Q = true;
			}
			else
			{
                J_update = true;
				Solution_State.base_dtt = min(Solution_State.base_dtt*min(min(min(Solution_State.wiggle_dt_mult, 1.0), Solution_State.pos_def_mult), Solution_State.pos_def_mult_Q), maxt);
				where_base_dtt_changed = 13;
			}
			Solution_State.base_dtt = min(Solution_State.base_dtt, Solution_State.dt_fail*pow(1.1, 1 - double(Redo_parameters.in_redo) / 2.0));
			where_base_dtt_changed += 10;
            if (Solution_State.t < Redo_parameters.redo_time)
				Solution_State.dtt = min(pow((Redo_parameters.redo_time - Solution_State.t) / (Redo_parameters.redo_time - Redo_parameters.redo_to_time), 1)*Solution_State.base_dtt + (1 - pow((Redo_parameters.redo_time - Solution_State.t) / (Redo_parameters.redo_time - Redo_parameters.redo_to_time), 1))*dt_change_failure()*Redo_parameters.redo_dt, 1000 * avg_redo_dtt*1.2);
            else if (Solution_State.t > Redo_parameters.redo_time && Solution_State.t - Solution_State.dtt < Redo_parameters.redo_time)
			{
				Solution_State.base_dtt = Solution_State.dtt;
				where_base_dtt_changed += 20;
			}
			else
				Solution_State.dtt = min(Solution_State.base_dtt, 1000 * avg_redo_dtt*1.2);

			if (controllers().size())
				Solution_State.dtt = min(Solution_State.dtt, get_nextcontrolinterval(Solution_State.t) - Solution_State.t);

			Solution_State.dtt = min(Solution_State.dtt, max_dt());

			if (Solution_State.dtt<dt0*1e-8)
			{
				Solution_State.fail_reason += " Redoing, dtt = " + numbertostring(Solution_State.dtt);
				doredo(Redo_parameters);
				iii = Redo_parameters.Res[max(int(Redo_parameters.Res.size()) - Redo_parameters.redo_counter, 0)].iii;
				if (write_details())
					write_to_detail_file("redo");


			}
		}
		if (iii%max_J_interval() == 0)
		{
			J_update = true;
			J_update_Q = true;
			J_update_C = true;
		}


		if (write_details()) writedetails();
#ifdef QT_version
		updateProgress();
#endif

		if (!Redo_parameters.redo)
		{
            for (unsigned int i=0; i < Blocks.size(); i++)
                Results.ANS.BTC[i].append(Solution_State.t, Blocks[i].S);

            for (unsigned int i=0; i < Connectors.size(); i++)
                Results.ANS.BTC[i + Blocks.size()].append(Solution_State.t, Connectors[i].Q*Connectors[i].flow_factor);

            for (unsigned int i=0; i < Blocks.size(); i++)
                Results.ANS.BTC[i + Blocks.size() + Connectors.size()].append(Solution_State.t, Blocks[i].H);

            for (unsigned int i=0; i < Blocks.size(); i++)
                Results.ANS.BTC[i + 2 * Blocks.size() + Connectors.size()].append(Solution_State.t, Blocks[i].outflow_corr_factor*Blocks[i].get_evaporation(Solution_State.t));

            for (unsigned int i=0; i < Connectors.size(); i++)
                Results.ANS.BTC[i + 3 * Blocks.size() + Connectors.size()].append(Solution_State.t, Connectors[i].A);

            for (unsigned int i=0; i < Connectors.size(); i++)
                Results.ANS.BTC[i + 3 * Blocks.size() + 2 * Connectors.size()].append(Solution_State.t, Connectors[i].Q_v);

            for (unsigned int i=0; i < Blocks.size(); i++)
                Results.ANS.BTC[i + 3 * Blocks.size() + 3* Connectors.size()].append(Solution_State.t, Blocks[i].V);

            for (unsigned int i=0; i < Blocks.size(); i++)
                Results.ANS.BTC[i + 4 * Blocks.size() + 3 * Connectors.size()].append(Solution_State.t, Blocks[i].plant_prop.LAI);


            for (unsigned int i=0; i < measured_quan().size(); i++)
				if (measured_quan()[i].experiment == name)
                    Results.ANS_obs.BTC[i].append(Solution_State.t, get_var(measured_quan()[i].loc_type, measured_quan()[i].id, measured_quan()[i].quan));

			//updating sensors
            for (unsigned int i=0; i < sensors().size(); i++)
			{
                int no_intervals_at_t = Solution_State.t / sensors()[i].interval;
                int no_intervals_at_t_minus_dtt = (Solution_State.t - Solution_State.dtt) / sensors()[i].interval;
				int delta_no_intervals = no_intervals_at_t - no_intervals_at_t_minus_dtt;
				if (delta_no_intervals > 0)
				{
					double C_1 = calc_term(sensors()[i].loc_type, sensors()[i].id, sensors()[i].quan);
					double C_2 = calc_term_star(sensors()[i].loc_type, sensors()[i].id, sensors()[i].quan);
					for (int no_intervals = 1; no_intervals <= delta_no_intervals; ++no_intervals) {
						double t_sensor = (no_intervals_at_t_minus_dtt + no_intervals) * sensors()[i].interval;
                        sensors()[i].append_output(t_sensor, C_1 + (C_2 - C_1) / Solution_State.dtt*(t_sensor - Solution_State.t + Solution_State.dtt), lookup_experiment(name));
					}
				}
			}

            for (unsigned int i=0; i < controllers().size(); i++)
			{
                if (int(Solution_State.t / controllers()[i].interval) > int((Solution_State.t - Solution_State.dtt) / controllers()[i].interval))
				{
                    controllers()[i].calc_value(Solution_State.t, lookup_experiment(name));
					parent->set_control_param(i, lookup_experiment(name));
				}

			}




            Results.Solution_dt.BTC[0].append(Solution_State.t, Solution_State.dtt);
            Results.Solution_dt.BTC[1].append(Solution_State.t, double(Solution_State.counter_flow));
            Results.Solution_dt.BTC[2].append(Solution_State.t, Solution_State.max_wiggle);

			if (colloid_transport())
				for (unsigned int j = 0; j < Blocks[0].Solid_phase.size(); j++)
					for (int k = 0; k < Blocks[0].Solid_phase[j]->n_phases; k++)
                        for (unsigned int i=0; i < Blocks.size(); i++)
                            Results.ANS_colloids.BTC[get_member_no(i, j, k)].append(Solution_State.t, Blocks[i].G[j][k]);

			if (constituent_transport())
				for (unsigned int kk = 0; kk < RXN().cons.size(); kk++)
					for (int p = -2; p<min(int(Blocks[0].Solid_phase.size()),max_phase); p++)
					{
						int _t;
						if (p < 0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
						for (int l = 0; l < _t; l++)
                            for (unsigned int i=0; i < Blocks.size(); i++)
                                Results.ANS_constituents.BTC[get_member_no(i, p, l, kk)].append(Solution_State.t, Blocks[i].CG[kk][Blocks[i].get_member_no(p, l)]);

					}

			if (mass_balance_check())
			{
				Blocksmassbalance();
                for (unsigned int i=0; i < Blocks.size(); i++) Results.ANS_MB.BTC[i].append(Solution_State.t, Blocks[i].MBBlocks);
			}

			setQ();

			if (!Redo_parameters.redo)
			{
				renew();
				renew_G();
				renew_CG();
                Solution_State.t += Solution_State.dtt;
			}
		}
#ifdef QT_version
        if (iii%120==0)
            QCoreApplication::processEvents(QEventLoop::AllEvents,10*1000);
#endif // QT_version
	}
	//qDebug() << "Solution Ended!";

	//qDebug() << "Making answers uniform";

    for (unsigned int i=0; i < controllers().size(); i++)
        Results.ANS_control.BTC[i] = controllers()[i].output;

	if (uniformoutput())
	{
        if (Results.ANS.BTC[0].n)
		{
			//qDebug() << "Making hydro answers uniform";
            Results.ANS = Results.ANS.make_uniform(dt0, false);
			//qDebug() << "Making colloid answers uniform";
            if (colloid_transport()) Results.ANS_colloids = Results.ANS_colloids.make_uniform(dt0, false);
			//qDebug() << "Making transport answers uniform";
            if (constituent_transport()) Results.ANS_constituents = Results.ANS_constituents.make_uniform(dt0, false);
            Results.ANS_obs.unif = false;
            Results.ANS_obs = Results.ANS_obs.make_uniform(dt0, false);
		}
	}
    Solution_State.failed = false;
    Solution_State.fail_reason = "Simulation conducted successfully";
#ifdef QT_version
	updateProgress(true);
#endif // QT_version
}

bool CMedium::solve()
{
	show_message("Solving started...");
	evaluate_functions();
	setH();	// Set Total Head for all blocks
	evaluate_area(true);  //Update connector areas;
	setQ();	//Set Q for all connectors
	initialize();


    Results.ANS_obs = CBTCSet(measured_quan().size());
    Results.ANS_obs.names.clear();
    for (unsigned int i=0; i < measured_quan().size(); i++)
        Results.ANS_obs.pushBackName(measured_quan()[i].name);

	if (solution_method() == "Partial Inverse Jacobian Evaluation")
		solve_fts_m2(dt());
	else
		solve_fts_m2(dt());

	return true;
}


double CMedium::get_var(int i, string j, int k) //i: ID, j: type (i.e. connector vs. block), k: variable
{
	if (i==0)
		return Blocks[getblocksq(j)].get_val(k);
	else if (i==1)
        return Connectors[getconnectorsq(j)].get_val(k);
    return 0;
}

double CMedium::get_var(int i, string j, string k) //i: ID, j: type (i.e. connector vs. block), k: variable
{
	if (i == 0)
		return Blocks[getblocksq(j)].get_val(k);
	else if (i == 1)
        return Connectors[getconnectorsq(j)].get_val(k);
    return 0;
}

CVector CMedium::get_val(string I, int j) //I: quantity name, j: type (i.e. connector vs. block)
{
	CVector X;
	if (j==0)
        for (unsigned int i=0; i<Blocks.size(); i++)
			X.append(Blocks[j].get_val(I));
	return X;
}

void CMedium::set_param(int param_no, double _value)
{

    for (unsigned int i=0; i<parameters()[param_no].location.size(); i++)
	{
		if (parameters()[param_no].location_type[i] == 2)

		{	Blocks[parameters()[param_no].location[i]].set_val(parameters()[param_no].quan[i],_value);
            Connectors[parameters()[param_no].location[i]].set_val(parameters()[param_no].quan[i],_value);
		}
		else if (parameters()[param_no].location_type[i] == 0)
			Blocks[parameters()[param_no].location[i]].set_val(parameters()[param_no].quan[i],_value);
		else if (parameters()[param_no].location_type[i] == 1)
            Connectors[parameters()[param_no].location[i]].set_val(parameters()[param_no].quan[i],_value);

	}

}

void CMedium::finalize_set_param()
{
	if (parent==nullptr)
    {
        parent = new CMediumSet();
        show_message("Setting a dummy parent...");
    }
	for (unsigned int ii=0; ii<Blocks.size(); ii++)
	{
		Blocks[ii].parent = this;
		if (Blocks[ii].fs_params[5]==0)
			if (Blocks[ii].fs_params[4] != 0) Blocks[ii].fs_params[5]=1-1.0/Blocks[ii].fs_params[4];

		if (Blocks[ii].fs_params[4]==0)
			if (Blocks[ii].fs_params[5] != 0) Blocks[ii].fs_params[4]=1.0/(1.0-Blocks[ii].fs_params[5]);

		if (Blocks[ii].fs_params[5]==0)
			if (Blocks[ii].fs_params[4] != 0) Blocks[ii].fs_params[5]=1-1.0/Blocks[ii].fs_params[4];
		if (Blocks[ii].fs_params[4]==0)
			if (Blocks[ii].fs_params[5] != 0) Blocks[ii].fs_params[4]=1.0/(1.0-Blocks[ii].fs_params[5]);

		for (unsigned int jj = 0; jj<Blocks[ii].buildup_id.size(); jj++) if (lookup_buildup(Blocks[ii].buildup_id[jj])!=-1) Blocks[ii].buildup.push_back(&(buildup()[lookup_buildup(Blocks[ii].buildup_id[jj])]));
		for (unsigned int jj = 0; jj<Blocks[ii].envexchange_id.size(); jj++) if (lookup_external_flux(Blocks[ii].envexchange_id[jj]) != -1) Blocks[ii].envexchange.push_back(&(externalflux()[lookup_external_flux(Blocks[ii].envexchange_id[jj])]));
		for (unsigned int jj = 0; jj<Blocks[ii].evaporation_id.size(); jj++) if (lookup_evaporation(Blocks[ii].evaporation_id[jj]) != -1) Blocks[ii].evaporation_m.push_back(&(evaporation_model()[lookup_evaporation(Blocks[ii].evaporation_id[jj])])); //newly added

		Blocks[ii].Solid_phase_id.clear();
        for (unsigned int i=0; i < Solid_phase().size(); i++)
			Blocks[ii].Solid_phase_id.push_back(i);

		Blocks[ii].Solid_phase.clear();
		//Resizing G and CG vectors
		for (unsigned int jj = 0; jj<Blocks[ii].Solid_phase_id.size(); jj++) Blocks[ii].Solid_phase.push_back(&(Solid_phase()[Blocks[ii].Solid_phase_id[jj]]));
		Blocks[ii].n_phases = Blocks[ii].get_tot_num_phases();

		Blocks[ii].G.resize(Blocks[ii].Solid_phase.size());
		Blocks[ii].G_star.resize(Blocks[ii].Solid_phase.size());
		Blocks[ii].G_stored_mass.resize(Blocks[ii].Solid_phase.size());//newly added
		for (unsigned int jj = 0; jj<Blocks[ii].Solid_phase.size(); jj++)
		{
			Blocks[ii].G[jj].resize(Blocks[ii].Solid_phase[jj]->n_phases);
			Blocks[ii].G_star[jj].resize(Blocks[ii].Solid_phase[jj]->n_phases);
			Blocks[ii].G_stored_mass[jj].resize(Blocks[ii].Solid_phase[jj]->n_phases);

		}


		if (sorption())
			n_default_phases = 2;
		else
			n_default_phases = 1;


		Blocks[ii].CG.resize(RXN().cons.size());
		Blocks[ii].CG_star.resize(RXN().cons.size());
		Blocks[ii].CG_stored_mass.resize(RXN().cons.size());//newly added
		for (unsigned int kk = 0; kk<RXN().cons.size(); kk++)
		{

			Blocks[ii].CG[kk].resize(Blocks[ii].get_tot_num_phases() + n_default_phases);
			Blocks[ii].CG_stored_mass[kk].resize(Blocks[ii].get_tot_num_phases() + n_default_phases);
			Blocks[ii].CG_star[kk].resize(Blocks[ii].get_tot_num_phases() + n_default_phases);
		}


		if (colloid_transport())
		{
			Blocks[ii].K.resize(Blocks[ii].Solid_phase.size());
			Blocks[ii].K_star.resize(Blocks[ii].Solid_phase.size());
			Blocks[ii].capacity_c.resize(Blocks[ii].Solid_phase.size());
			Blocks[ii].capacity_c_star.resize(Blocks[ii].Solid_phase.size());
			Blocks[ii].capacity_c_Q.resize(Blocks[ii].Solid_phase.size());
			Blocks[ii].capacity_c_star_Q.resize(Blocks[ii].Solid_phase.size());

			for (unsigned int jj = 0; jj<Blocks[ii].initial_g.size(); jj++)
			{
				int p = lookup_particle_type(Blocks[ii].initial_g_counter_p[jj]);
				int l = Solid_phase()[p].lookup_phase(Blocks[ii].initial_g_counter_l[jj]);
				if ((p!=-1) && (l!=-1)) Blocks[ii].G[p][l] = Blocks[ii].initial_g[jj];

			}
			for (unsigned int jj = 0; jj < Blocks[ii].initial_cg.size(); jj++)
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

    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        for (unsigned int j=0; j<Connectors[i].flow_params.size(); j++)
            if (Connectors[i].flow_params[j]==0)
			{

                if (((Blocks[Connectors[i].Block1N].indicator == Soil) || (Blocks[Connectors[i].Block1N].indicator == Darcy) || (Blocks[Connectors[i].Block1N].indicator == Storage)) && ((Blocks[getblocksq(Connectors[i].Block2ID)].indicator == Pond) || (Blocks[getblocksq(Connectors[i].Block2ID)].indicator == Stream)))
                    Connectors[i].flow_params[j] = Blocks[Connectors[i].Block1N].fs_params[j];

                if (((Blocks[getblocksq(Connectors[i].Block2ID)].indicator == Soil) || (Blocks[getblocksq(Connectors[i].Block2ID)].indicator == Darcy) || (Blocks[getblocksq(Connectors[i].Block2ID)].indicator == Storage)) && ((Blocks[getblocksq(Connectors[i].Block1ID)].indicator == Pond) || (Blocks[getblocksq(Connectors[i].Block1ID)].indicator == Stream)))
                    Connectors[i].flow_params[j] = Blocks[getblocksq(Connectors[i].Block2ID)].fs_params[j];

                if (Blocks[getblocksq(Connectors[i].Block1ID)].vapor_diffusion!=0 && Blocks[getblocksq(Connectors[i].Block2ID)].vapor_diffusion!=0)
                    Connectors[i].vapor_transport = true;
                if (Blocks[getblocksq(Connectors[i].Block1ID)].indicator == Blocks[getblocksq(Connectors[i].Block2ID)].indicator)
                    Connectors[i].flow_params[j]=0.5*(Blocks[getblocksq(Connectors[i].Block1ID)].fs_params[j] + Blocks[getblocksq(Connectors[i].Block2ID)].fs_params[j]);
                else if (Blocks[getblocksq(Connectors[i].Block1ID)].indicator==0)
                    Connectors[i].flow_params[j]=Blocks[getblocksq(Connectors[i].Block1ID)].fs_params[j];
                else if (Blocks[getblocksq(Connectors[i].Block2ID)].indicator==0)
                    Connectors[i].flow_params[j]=Blocks[getblocksq(Connectors[i].Block2ID)].fs_params[j];
			}


            if (Connectors[i].A==0)
			{
                if (Connectors[i].const_area == true)
				{
                    if (Blocks[getblocksq(Connectors[i].Block1ID)].indicator == Blocks[getblocksq(Connectors[i].Block2ID)].indicator)
                        Connectors[i].A = Connectors[i].A_star = 0.5*(Blocks[getblocksq(Connectors[i].Block1ID)].A + Blocks[getblocksq(Connectors[i].Block2ID)].A);
                    else if (Blocks[getblocksq(Connectors[i].Block1ID)].indicator == Soil)
                        Connectors[i].A = Connectors[i].A_star = Blocks[getblocksq(Connectors[i].Block1ID)].A;
                    else if (Blocks[getblocksq(Connectors[i].Block2ID)].indicator == Soil)
                        Connectors[i].A = Connectors[i].A_star = Blocks[getblocksq(Connectors[i].Block2ID)].A;
				}

			}
            Connectors[i].A_star = Connectors[i].A;

            if (Connectors[i].d==0)

            {	if (Blocks[getblocksq(Connectors[i].Block1ID)].indicator == Blocks[getblocksq(Connectors[i].Block2ID)].indicator)
                    Connectors[i].d=fabs(Blocks[getblocksq(Connectors[i].Block1ID)].z0 - Blocks[getblocksq(Connectors[i].Block2ID)].z0);
                else if (Blocks[getblocksq(Connectors[i].Block1ID)].indicator==0)
                    Connectors[i].d=Blocks[getblocksq(Connectors[i].Block1ID)].V/Blocks[getblocksq(Connectors[i].Block1ID)].A/2.0;
                else if (Blocks[getblocksq(Connectors[i].Block2ID)].indicator==0)
                    Connectors[i].d=Blocks[getblocksq(Connectors[i].Block2ID)].V/Blocks[getblocksq(Connectors[i].Block2ID)].A/2.0;
			}

            if ((Blocks[getblocksq(Connectors[i].Block1ID)].indicator == Catchment) && (Blocks[getblocksq(Connectors[i].Block2ID)].indicator == Catchment))
                if (Connectors[i].flow_params[7] == 0) Connectors[i].flow_params[7] = 0.667;

            Connectors[i].c_dispersion.resize(Connectors[i].Solid_phase.size());
            Connectors[i].c_dispersion_star.resize(Connectors[i].Solid_phase.size());
            Connectors[i].dispersion.resize(RXN().cons.size());
            Connectors[i].dispersion_star.resize(RXN().cons.size());
            Connectors[i].RXN = &RXN();
            if ((lookup_controllers(Connectors[i].controller_id) != -1) && Connectors[i].control == true)
                Connectors[i].Controller = &controllers()[lookup_controllers(Connectors[i].controller_id)];
	}

    for (unsigned int i=0; i < controllers().size(); i++)
		controllers()[i].Sensor = &sensors()[lookup_sensors(controllers()[i].sensor_id)];

	//setting the formulas for LAI and biomass growth for plant blocks
    for (unsigned int i=0; i < Blocks.size(); i++)
		if (Blocks[i].indicator == Plant)
			Blocks[i].set_up_plant_growth_expressions();
}

void CMedium::set_default_params()
{
    //for (unsigned int i=0; i<parameters().size(); i++)
	//	set_param(i, parameters()[i].value);

	finalize_set_param();

	if (!Solid_phase().size())
		colloid_transport() = false;

	if (!RXN().cons.size())
		constituent_transport() = false;
	else
		if (constituent_transport())
			colloid_transport() = true;

	if (steady_state_hydro())
        for (unsigned int i=0; i < Connectors.size(); i++)
            if (Connectors[i].d == 0) Connectors[i].d = 1;

#ifdef API_version
        f_set_default_block_expressions();
        f_set_default_connector_expressions();
        get_funcs();
#endif // API_version

}


void CMedium::set_default()
{
    if (!parent)
    {
        parent = new CMediumSet();
        show_message("Assigned a dummy parent");
    }
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
    evaluate_area(true);
    for (unsigned int i=0; i<Connectors.size(); i++)
        Connectors[i].evaluate_functions();

    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_functions();

}

void CMedium::get_funcs()
{
    for (unsigned int i=0; i<Connectors.size(); i++)
        Connectors[i].get_funcs(Connectors[i].flow_expression);

    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].get_funcs(Blocks[i].H_S_expression);

}

int CMedium::getblocksq(const string &id)
{
#ifdef QT_version
	if (!Blocks.size())
		return parent->blockIndex[id];
#endif
    for (unsigned int i=0; i<Blocks.size(); i++)
		if (Blocks[i].ID == id)
			return i;

	return -1;
}

int CMedium::getconnectorsq(const string &id)
{
#ifdef QT_version
    if (!Connectors.size())
		return parent->connectorIndex[id];
#endif
    for (unsigned int i=0; i<Connectors.size(); i++)
        if (Connectors[i].ID == id) return i;

	return -1;
}

bool CMedium::is_there_any_fixed_connectors()
{
    for (unsigned int i=0; i<Connectors.size(); i++)
        if (Connectors[i].fixed==true) return true;
    for (unsigned int i=0; i<Blocks.size(); i++)
		if (Blocks[i].fixed_evaporation==true) return true;
	return false;

}


CVector CMedium::get_flow_factors()
{
	CVector X(Blocks.size());
    for (unsigned int i=0; i<Blocks.size(); i++)
		X[i]=Blocks[i].outflow_corr_factor;

	return X;
}

void CMedium::set_flow_factors(CVector &X)
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].outflow_corr_factor = X[i];


}

vector<int> CMedium::get_fixed_connect_status()
{
	vector<int> X;
    for (unsigned int i=0; i<Blocks.size(); i++)
		X.push_back(Blocks[i].setzero);

	return X;
}


void CMedium::set_fixed_connect_status(vector<int> X)
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].setzero=X[i];
}



void CMedium::set_block_fluxes()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		double x = -10;
		double x_star = -10;
		for (unsigned int j=0; j<Blocks[i].connectors.size(); j++)
		{
            if ((fabs((Connectors[Blocks[i].connectors[j]].Q-Connectors[Blocks[i].connectors[j]].Q_v)/Connectors[Blocks[i].connectors[j]].A))>x) x = fabs((Connectors[Blocks[i].connectors[j]].Q-Connectors[Blocks[i].connectors[j]].Q_v)/Connectors[Blocks[i].connectors[j]].A);
            if ((fabs((Connectors[Blocks[i].connectors[j]].Q_star-Connectors[Blocks[i].connectors[j]].Q_v_star)/Connectors[Blocks[i].connectors[j]].A_star))>x_star) x_star = fabs((Connectors[Blocks[i].connectors[j]].Q_star-Connectors[Blocks[i].connectors[j]].Q_v_star)/Connectors[Blocks[i].connectors[j]].A_star);

		}
		Blocks[i].q = w()*x + (1-w())*x_star;

	}
}



void CMedium::evaluate_K()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_K();
}

void CMedium::evaluate_K_star()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_K_star();
}

void CMedium::evaluate_capacity()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_capacity();
}

void CMedium::evaluate_capacity_star()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_capacity_star();


}

void CMedium::evaluate_capacity_c()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_capacity_c();
}

void CMedium::evaluate_capacity_c_star()
{
    for (unsigned int i=0; i<Blocks.size(); i++)
		Blocks[i].evaluate_capacity_c_star();


}

void CMedium::evaluate_dispersion()
{
    for (unsigned int i=0; i<Connectors.size(); i++)
    {	Connectors[i].evaluate_dispersion();
        Connectors[i].evaluate_dispersion_star();
	}
}

void CMedium::evaluate_const_dispersion()
{
    for (unsigned int i=0; i<Connectors.size(); i++)
    {	Connectors[i].evaluate_const_dispersion();
        Connectors[i].evaluate_const_dispersion_star();
	}
}

void CMedium::onestepsolve_colloid(double dtt)
{
	if (Blocks[0].n_phases == 0)
	{
		Solution_State.failed_colloid = false;
		return;
	}
    evaluate_dispersion();   //Connectors[i].evaluate_dispersion(); Connectors[i].evaluate_dispersion_star();  //Negative diffusion due to negative f[8]????
	evaluate_K();			//Blocks[i].evaluate_K();
	evaluate_capacity();	//Blocks[i].evaluate_capacity();
	//renew_G();

	CVector X(Blocks.size()*Blocks[0].n_phases);
	CVector X_old(Blocks.size()*Blocks[0].n_phases);

	for (unsigned int p=0; p<Blocks[0].Solid_phase.size(); p++)
		for (int l=0; l<Blocks[0].Solid_phase[p]->n_phases; l++)
            for (unsigned int i=0; i<Blocks.size(); i++)  X[get_member_no(i,p,l)] =Blocks[i].G[p][l];

	X_old = X;
	int error_expand_counter = 0;
	CVector F = getres_C(X,dtt);
	CVector F_old = F;
	double lambda = 1;
    Solution_State.counter_colloid = 0;

	if ((F == F) != true || (X == X) != true || (F.is_finite() == false) || (X.is_finite() == false))
	{	Solution_State.failed_colloid = true;
        Solution_State.fail_reason = "infinite X or F in colloids";
		Solution_State.pos_def_ratio = 1e-12;
		return;
	}

	double err = F.norm2();
	double err_p = err;

    Solution_State.J_c_update_count = 0;
	while ((err>tol()) || (X.min()<double(0)))
	{
		if ((J_update_C == true) || (M_C.getnumcols() != X.num))
		{
            Solution_State.J_c_update_count++;
			M_C = Jacobian_C(X,dtt);
            Solution_State.epoch_count++;
			CMatrix M1 = normalize_diag(M_C,M_C);
			if (M_C.getnumcols()>0) Solution_State.pos_def_ratio=M1.diag_ratio().abs_max(); else Solution_State.pos_def_ratio = 1e-12;
			if (fabs(det(M1))<1e-30)
			{	set_G_star(X_old);
                Solution_State.fail_reason = "non strongly positive definite Jacobian in colloids";
				Solution_State.failed_colloid = true;
				return;

			}

			InvJ_C = inv(M1);
			if (InvJ_C.getnumcols() == 0)
			{
                Solution_State.fail_reason = "Colloid Jacobian in not inversible";
				Solution_State.failed_colloid = true;
				Solution_State.pos_def_ratio = 1e-12;
				return;
			}

			J_update_C=false;
            Solution_State.dtt_J_c = dtt;
		}

        CVector dx = (dtt / Solution_State.dtt_J_c)*(InvJ_C*normalize_diag(F, M_C));

		X -= lambda*dx;

		F = getres_C(X,dtt);
		err_p = err;
		err = F.norm2();
		if (err>err_p)
		{
			error_expand_counter++;
			J_update_Q = true;
			X = X_old;
			F = F_old;
			lambda /= 2.0;
		}


		if (error_expand_counter == 4)
		{
			set_G_star(X_old);
            Solution_State.fail_reason = "error expansion in particle transport";
			Solution_State.failed_colloid = true;
			return;
		}

        Solution_State.counter_colloid++;
        if (Solution_State.counter_colloid>nr_failure_criteria())
		{
			set_G_star(X_old);
            Solution_State.fail_reason = "Number of iterations exceeded the limit in colloids";
			Solution_State.failed_colloid = true;
			return;
		}
	}

	set_G_star(X); //Added by Babak to avoid negative values in response 12/22/15

	if (X.min() < double(-1e-13))
	{
        Solution_State.fail_reason = "Negative value for colloid concentration";
		Solution_State.failed_colloid = true;
	}
	else
		Solution_State.failed_colloid = false;
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
    Solution_State.counter_const = 0;

	if ((F == F) != true || (X == X) != true || (F.is_finite() == false) || (X.is_finite() == false))
	{	Solution_State.failed_const=true;
        Solution_State.fail_reason = "infinite X or F in water quality";
		Solution_State.pos_def_ratio_const = 1e-12;
		return;
	}

	double err = F.norm2();
	double err_p = err;

	double lambda = 1;
    Solution_State.J_q_update_count = 0;
	while (err>tol())
	{
		if ((J_update_Q==true) || (M_Q.getnumrows()!= X.num ))
		{
            Solution_State.J_q_update_count++;
			M_Q = Jacobian_Q(X,dtt);
			CMatrix M1 = normalize_diag(M_Q,M_Q);
			CMatrix D = M1.non_posdef_elems_m();
            Solution_State.epoch_count++;
			if (M_Q.getnumcols()>0) Solution_State.pos_def_ratio_const=M1.diag_ratio().abs_max(); else Solution_State.pos_def_ratio_const = 1e-12;
			if (Solution_State.pos_def_ratio_const > 1)
				Preconditioner_Q = M1.Preconditioner();
			else
				Preconditioner_Q = Identity(F.num);
			if (fabs(det(M1))<1e-30)
			{	set_CG_star(X_old);
                Solution_State.fail_reason = "Not strongly positive definite Jacobian in wq";
				Solution_State.failed_const = true;
				return;

			}

			InvJ_Q = inv(Preconditioner_Q*M1);
			if (InvJ_Q.getnumcols() == 0)
			{
				set_CG_star(X_old);
                Solution_State.fail_reason = "Matrix not invertible in wq";
				Solution_State.failed_const = true;
				return;
			}
			J_update_Q=false;
            Solution_State.dtt_J_q = dtt;
		}

        CVector dx = dtt / Solution_State.dtt_J_q*(InvJ_Q*Preconditioner_Q*normalize_diag(F, M_Q));

		X -= lambda*dx;

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
			lambda /= 2.0;
		}

        Solution_State.counter_const++;

		if (error_expand_counter==4)
		{
			set_CG_star(X_old);
            Solution_State.fail_reason = "error expansion in wq";
			Solution_State.failed_const = true;
			return;
		}

        if (Solution_State.counter_const>nr_failure_criteria())
		{
			set_CG_star(X_old);
            Solution_State.fail_reason = "Number of iterations exceeded the limit in wq";
			Solution_State.failed_const = true;
			return;
		}
	}
	if (negative_concentration_allowed() == false)
	{
		if (X.min() < -fabs(minimum_acceptable_negative_conc()))
		{
			vector<int> neg_vals_cons;
			vector<int> neg_vals_block;
			vector<double> neg_vals;
            for (int i=0; i < X.getsize(); i++)
				if (X[i] < -fabs(minimum_acceptable_negative_conc())) {
					neg_vals_block.push_back(get_member_no_inv(i)[0]); neg_vals_cons.push_back(get_member_no_inv(i)[3]);  neg_vals.push_back(X[i]);
				}
				else if (X[i] < 0) X[i] = 0;

            Solution_State.fail_reason = "Negative value in constituent ";
            for (unsigned int i=0; i < neg_vals_block.size(); i++) Solution_State.fail_reason = Solution_State.fail_reason + RXN().cons[neg_vals_cons[i]].name +", ";
            Solution_State.fail_reason = Solution_State.fail_reason + " concentration at ";
            for (unsigned int i=0; i < neg_vals_block.size(); i++) Solution_State.fail_reason = Solution_State.fail_reason + Blocks[neg_vals_block[i]].ID +", ";
            Solution_State.fail_reason = Solution_State.fail_reason + " values respectively = ";
            for (unsigned int i=0; i < neg_vals_block.size(); i++) Solution_State.fail_reason = Solution_State.fail_reason + numbertostring(neg_vals[i]) + ", ";
			Solution_State.failed_const = true;
		}
		else
		{
			Solution_State.failed_const = false;
            Solution_State.fail_reason == "none";
		}
	}
	else
	{
		Solution_State.failed_const = false;
        Solution_State.fail_reason == "none";
	}
	int max_phases = 10000;
	if (!sorption())
		max_phases = -1;

	if (Solution_State.failed_const == false)
	{
        Solution_State.fail_reason = "none";
		for (unsigned int k = 0; k < RXN().cons.size(); k++)
		{
			for (int p = -2; p<min(int(Blocks[0].Solid_phase.size()),max_phases); p++)
			{
				int _t;
				if (p < 0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
				for (int l = 0; l < _t; l++)
				{
					// LHS
                    for (unsigned int i=0; i < Blocks.size(); i++)
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




CVector CMedium::getres_C(const CVector &X, const double &dtt)
{

	CVector F(Blocks[0].n_phases*Blocks.size());

	set_G_star(X);	//Blocks[i].G_star[p] = X[i+Blocks.size()*p];
	evaluate_K_star();
	evaluate_capacity_star();
	evaluate_dispersion();

	for (unsigned int p=0; p<Blocks[0].Solid_phase.size(); p++)
	{	for (int l=0; l<Blocks[0].Solid_phase[p]->n_phases; l++)
		{
			// LHS
            for (unsigned int i=0; i<Blocks.size(); i++)
			{
				F[get_member_no(i,p,l)] = (Blocks[i].capacity_c_star[p][l]*Blocks[i].G_star[p][l]-Blocks[i].capacity_c[p][l]*Blocks[i].G[p][l])/dtt;
				if ((l==0) && (Blocks[i].inflow.size()!=0))
                    F[get_member_no(i,p,l)] -= sum_interpolate(Blocks[i].inflow, Solution_State.t,"flow")*sum_interpolate(Blocks[i].inflow, Solution_State.t,Solid_phase()[p].name);
			}

            for (unsigned int i=0; i<Connectors.size(); i++)
			{
				//advection
				vector<int> ii;
				ii.push_back(p);

                double Q_adv_star = (Connectors[i].Q_star - Connectors[i].Q_v_star)*Connectors[i].flow_factor + Connectors[i].settling*Solid_phase()[p].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*Connectors[i].A_star*0.5*(Connectors[i].Block1->calc_star(Solid_phase()[p].vs_coefficient,ii) + Connectors[i].Block2->calc_star(Solid_phase()[p].vs_coefficient,ii));
                double Q_adv = (Connectors[i].Q - Connectors[i].Q_v)*Connectors[i].flow_factor + Connectors[i].settling*Solid_phase()[p].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*Connectors[i].A*0.5*(Connectors[i].Block1->calc(Solid_phase()[p].vs_coefficient,ii) + Connectors[i].Block2->calc(Solid_phase()[p].vs_coefficient,ii));

				if (((1-w())*Q_adv_star+w()*Q_adv)>0)
                {	F[get_member_no(Connectors[i].Block1N,p,l)] += (w()*Q_adv*Blocks[Connectors[i].Block1N].G[p][l] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block1N].G_star[p][l])*Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l];
                    F[get_member_no(Connectors[i].Block2N,p,l)] -= (w()*Q_adv*Blocks[Connectors[i].Block1N].G[p][l] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block1N].G_star[p][l])*Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l];
				}
				if (((1 - w())*Q_adv_star + w()*Q_adv)<0)
                {	F[get_member_no(Connectors[i].Block1N,p,l)] += (w()*Q_adv*Blocks[Connectors[i].Block2N].G[p][l] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block2N].G_star[p][l])*Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l];
                    F[get_member_no(Connectors[i].Block2N,p,l)] -= (w()*Q_adv*Blocks[Connectors[i].Block2N].G[p][l] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block2N].G_star[p][l])*Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l];
				}

			}
				//diffusion
            for (unsigned int i=0; i<Connectors.size(); i++)
			{
                double exchange = Connectors[i].A*(w()*Connectors[i].c_dispersion[p]+(1-w())*Connectors[i].c_dispersion_star[p])/Connectors[i].d*((w()*Blocks[Connectors[i].Block2N].G[p][l]+(1-w())*Blocks[Connectors[i].Block2N].G_star[p][l])-(w()*Blocks[Connectors[i].Block1N].G[p][l]+(1-w())*Blocks[Connectors[i].Block1N].G_star[p][l]))*Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l]*Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l];
                F[get_member_no(Connectors[i].Block1N,p,l)] -= exchange;
                F[get_member_no(Connectors[i].Block2N,p,l)] += exchange;
			}

		}

	}

	// phase mass transfer
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		for (unsigned int p=0; p<Blocks[i].Solid_phase.size(); p++)
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
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		for (unsigned int p=0; p<Blocks[i].Solid_phase.size(); p++)
		{	for (int k=0; k<Blocks[i].Solid_phase[p]->n_phases; k++)
				for (unsigned int j=0; j<Blocks[i].buildup.size(); j++)
					if ((buildup()[j].name == Blocks[i].Solid_phase[p]->name) && (Blocks[i].buildup[j]->phase == Blocks[i].Solid_phase[p]->phase_names[k]))
						F[get_member_no(i,p,k)] -= Blocks[i].buildup[j]->buildup((1-w())*Blocks[i].G_star[p][k] + w()*Blocks[i].G[p][k],&Blocks[i])*Blocks[i].V;
		}
	}

	return F;
}


CVector_arma CMedium::getres_C(CVector_arma &X, const double &dtt)
{

	CVector_arma F(Blocks[0].n_phases*Blocks.size());

	set_G_star(X);	//Blocks[i].G_star[p] = X[i+Blocks.size()*p];
	evaluate_K_star();
	evaluate_capacity_star();
	evaluate_dispersion();

	for (unsigned int p = 0; p<Blocks[0].Solid_phase.size(); p++)
	{
		for (int l = 0; l<Blocks[0].Solid_phase[p]->n_phases; l++)
		{
			// LHS
            for (unsigned int i=0; i<Blocks.size(); i++)
			{
				F[get_member_no(i, p, l)] = (Blocks[i].capacity_c_star[p][l] * Blocks[i].G_star[p][l] - Blocks[i].capacity_c[p][l] * Blocks[i].G[p][l]) / dtt;
				if ((l == 0) && (Blocks[i].inflow.size() != 0))
                    F[get_member_no(i, p, l)] -= sum_interpolate(Blocks[i].inflow, Solution_State.t, "flow")*sum_interpolate(Blocks[i].inflow, Solution_State.t, Solid_phase()[p].name);
			}

            for (unsigned int i=0; i<Connectors.size(); i++)
			{
				//advection
				vector<int> ii;
				ii.push_back(p);

                double Q_adv_star = (Connectors[i].Q_star - Connectors[i].Q_v_star)*Connectors[i].flow_factor + Connectors[i].settling*Solid_phase()[p].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*Connectors[i].A_star*0.5*(Connectors[i].Block1->calc_star(Solid_phase()[p].vs_coefficient, ii) + Connectors[i].Block2->calc_star(Solid_phase()[p].vs_coefficient, ii));
                double Q_adv = (Connectors[i].Q - Connectors[i].Q_v)*Connectors[i].flow_factor + Connectors[i].settling*Solid_phase()[p].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*Connectors[i].A*0.5*(Connectors[i].Block1->calc(Solid_phase()[p].vs_coefficient, ii) + Connectors[i].Block2->calc(Solid_phase()[p].vs_coefficient, ii));

				if (((1 - w())*Q_adv_star + w()*Q_adv)>0)
				{
                    F[get_member_no(Connectors[i].Block1N, p, l)] += (w()*Q_adv*Blocks[Connectors[i].Block1N].G[p][l] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block1N].G_star[p][l])*Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l];
                    F[get_member_no(Connectors[i].Block2N, p, l)] -= (w()*Q_adv*Blocks[Connectors[i].Block1N].G[p][l] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block1N].G_star[p][l])*Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l];
				}
				if (((1 - w())*Q_adv_star + w()*Q_adv)<0)
				{
                    F[get_member_no(Connectors[i].Block1N, p, l)] += (w()*Q_adv*Blocks[Connectors[i].Block2N].G[p][l] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block2N].G_star[p][l])*Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l];
                    F[get_member_no(Connectors[i].Block2N, p, l)] -= (w()*Q_adv*Blocks[Connectors[i].Block2N].G[p][l] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block2N].G_star[p][l])*Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l];
				}

			}
			//diffusion
            for (unsigned int i=0; i<Connectors.size(); i++)
			{
                double exchange = Connectors[i].A*(w()*Connectors[i].c_dispersion[p] + (1 - w())*Connectors[i].c_dispersion_star[p]) / Connectors[i].d*((w()*Blocks[Connectors[i].Block2N].G[p][l] + (1 - w())*Blocks[Connectors[i].Block2N].G_star[p][l]) - (w()*Blocks[Connectors[i].Block1N].G[p][l] + (1 - w())*Blocks[Connectors[i].Block1N].G_star[p][l]))*Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l] * Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l];
                F[get_member_no(Connectors[i].Block1N, p, l)] -= exchange;
                F[get_member_no(Connectors[i].Block2N, p, l)] += exchange;
			}

		}

	}

	// phase mass transfer
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		for (unsigned int p = 0; p<Blocks[i].Solid_phase.size(); p++)
		{
			for (int l = 0; l<Blocks[i].Solid_phase[p]->n_phases; l++)
				for (int k = 0; k<Blocks[i].Solid_phase[p]->n_phases; k++)
				{
					double exchange = (1 - w())*Blocks[i].K_star[p][l][k] * Blocks[i].G_star[p][l] + w()*Blocks[i].K[p][l][k] * Blocks[i].G[p][l];
					F[get_member_no(i, p, l)] += exchange*Blocks[i].V;
					F[get_member_no(i, p, k)] -= exchange*Blocks[i].V;
				}
		}
	}

	// build_up
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		for (unsigned int p = 0; p<Blocks[i].Solid_phase.size(); p++)
		{
			for (int k = 0; k<Blocks[i].Solid_phase[p]->n_phases; k++)
				for (unsigned int j = 0; j<Blocks[i].buildup.size(); j++)
					if ((buildup()[j].name == Blocks[i].Solid_phase[p]->name) && (Blocks[i].buildup[j]->phase == Blocks[i].Solid_phase[p]->phase_names[k]))
						F[get_member_no(i, p, k)] -= Blocks[i].buildup[j]->buildup((1 - w())*Blocks[i].G_star[p][k] + w()*Blocks[i].G[p][k], &Blocks[i])*Blocks[i].V;
		}
	}

	return F;
}


void CMedium::correct_S(double dtt)
{
	CVector S(Blocks.size());
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
			if  (Blocks[i].inflow.size()!=0)
                S[i] = Blocks[i].S+sum_interpolate(Blocks[i].inflow, Solution_State.t)[0]*dtt;
			else S[i]= Blocks[i].S;
//			if (Blocks[i].indicator != Soil)
                S[i] -= Blocks[i].outflow_corr_factor*Blocks[i].get_evaporation(Solution_State.t)*dtt;
	}

    for (unsigned int i=0; i<Connectors.size(); i++)
	{
        S[Connectors[i].Block1N] -= (w()*Connectors[i].Q + (1-w())*Connectors[i].Q_star)*Connectors[i].flow_factor*dtt;
        S[Connectors[i].Block2N] += (w()*Connectors[i].Q + (1-w())*Connectors[i].Q_star)*Connectors[i].flow_factor*dtt;
	}

    for (unsigned int i=0; i<Blocks.size(); i++) Blocks[i].S_star = max(S[i],0.0);

}

double CMedium::get_var(int i, vector<string> j, int k)
{
	double sum=0;
	for (unsigned int ii=0; ii<j.size(); ii++)
		sum += get_var(i,j[ii],k);

	return sum;

}

double CMedium::get_var(int i, vector<string> j, string k)
{
	double sum = 0;
	for (unsigned int ii = 0; ii<j.size(); ii++)
		sum += get_var(i, j[ii], k);

	return sum;

}

double CMedium::calc_term(int i, string loc_id, CStringOP k)
{
	if (i == 0)
		return Blocks[getblocksq(loc_id)].calc(k);
	else if (i == 1)
        return Connectors[getconnectorsq(loc_id)].calc(k);
    return 0;
}

double CMedium::calc_term_star(int i, string loc_id, CStringOP k)
{
	if (i == 0)
		return Blocks[getblocksq(loc_id)].calc_star(k);
	else if (i == 1)
        return Connectors[getconnectorsq(loc_id)].calc_star(k);
    return 0;
}


void CMedium::write_state(string filename)
{
	FILE *Fil;
	Fil = fopen(filename.c_str(), "w");

	fprintf(Fil, "dtt=%le\n", Solution_State.dtt);
    fprintf(Fil, "t=%le\n", Solution_State.t);
	for (unsigned int j=0; j<Blocks.size(); j++)
	{
		fprintf(Fil, "S-%i=%le\n", j, Blocks[j].S);
	}
	for (unsigned int j=0; j<Blocks.size(); j++)
	{
		fprintf(Fil, "outflow_corr_factor-%i=%le\n", j, Blocks[j].outflow_corr_factor);
	}
	for (unsigned int j=0; j<Blocks.size(); j++)
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

	k[0] = i/Blocks[0].n_phases;
	int pp = 0;
	for (unsigned int ii=0; ii<Blocks[0].Solid_phase.size(); ii++)
		for (int jj=0; jj<Blocks[0].Solid_phase[ii]->n_phases; jj++)
		{ if (pp==i) {k[1]=ii, k[2]=jj;}
		  pp++;
		}
	return k;
}


CVector CMedium::getres_Q(const CVector &X, const double &dtt)
{
	int max_phases = 10000;
	if (!sorption())
		max_phases = -1;
	CVector F(RXN().cons.size()*(Blocks[0].n_phases+n_default_phases)*Blocks.size());
	set_CG_star(X);
	evaluate_capacity_c_star();
	for (unsigned int k=0; k<RXN().cons.size(); k++)
	{	for (int p=-2; p<min(int(Blocks[0].Solid_phase.size()),max_phases); p++)
		{	int _t;
			if (p<0) _t=1; else _t=Blocks[0].Solid_phase[p]->n_phases;
			for (int l=0; l<_t; l++)
			{
				// LHS
                for (unsigned int i=0; i<Blocks.size(); i++)
				{
					F[get_member_no(i,p,l,k)] = (get_capacity_star(i,l,p)*Blocks[i].CG_star[k][get_member_no(p,l)]-get_capacity(i,l,p)*Blocks[i].CG[k][get_member_no(p,l)])/dtt;

					if ((p == -2) && (Blocks[i].inflow.size() != 0))//needs to be modified to account for colloid-associated inflow
					{
                        F[get_member_no(i, p, l, k)] -= sum_interpolate(Blocks[i].inflow, Solution_State.t, "flow")*sum_interpolate(Blocks[i].inflow, Solution_State.t, RXN().cons[k].name);

						for (unsigned int j = 0; j < Blocks[i].evaporation_m.size(); j++) //newly added
						{
							if (Blocks[i].evaporation_m[j]->uptake==true)
                                F[get_member_no(i, p, l, k)] += Blocks[i].outflow_corr_factor*(Blocks[i].get_evaporation(j, Solution_State.t)*(w()*Blocks[i].CG_star[k][get_member_no(p, l)] + (1 - w())*Blocks[i].CG[k][get_member_no(p, l)]));
						}

					}


				}




                for (unsigned int i=0; i<Connectors.size(); i++)
				{
					//advection

					if (p==-2)
					{
						vector<int> ii;
						ii.push_back(k);
                        double Q_adv_star = (Connectors[i].Q_star - Connectors[i].Q_v_star)*Connectors[i].flow_factor + Connectors[i].settling*RXN().cons[k].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*Connectors[i].A_star*0.5*(Connectors[i].Block1->calc_star(RXN().cons[k].vs_coefficient,ii) + Connectors[i].Block2->calc_star(RXN().cons[k].vs_coefficient,ii));
                        double Q_adv = (Connectors[i].Q - Connectors[i].Q_v)*Connectors[i].flow_factor + Connectors[i].settling*RXN().cons[k].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*Connectors[i].A*0.5*(Connectors[i].Block1->calc(RXN().cons[k].vs_coefficient,ii) + Connectors[i].Block2->calc(RXN().cons[k].vs_coefficient,ii));

						if (w()*Q_adv+(1-w())*Q_adv_star>0)
                        {	F[get_member_no(Connectors[i].Block1N,p,l,k)] += (w()*Q_adv*Blocks[Connectors[i].Block1N].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p,l)]);
                            F[get_member_no(Connectors[i].Block2N, p, l, k)] -= (w()*Q_adv*Blocks[Connectors[i].Block1N].CG[k][get_member_no(p, l)] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p, l)]);

						}
						if (Q_adv<0)
                        {	F[get_member_no(Connectors[i].Block1N,p,l,k)] += (w()*Q_adv*Blocks[Connectors[i].Block2N].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p,l)]);
                            F[get_member_no(Connectors[i].Block2N,p,l,k)] -= (w()*Q_adv*Blocks[Connectors[i].Block2N].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p,l)]);
						}

					}
					else if (p==-1)
					{


					}
					else
					{
						vector<int> ii;
						ii.push_back(p);
                        double Q_adv_star = (Connectors[i].Q_star - Connectors[i].Q_v_star)*Connectors[i].flow_factor + Connectors[i].settling*Solid_phase()[p].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*0.5*(Connectors[i].Block1->calc_star(Solid_phase()[p].vs_coefficient,ii)+ Connectors[i].Block2->calc_star(Solid_phase()[p].vs_coefficient,ii));
                        double Q_adv = (Connectors[i].Q - Connectors[i].Q_v)*Connectors[i].flow_factor + Connectors[i].settling*Solid_phase()[p].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*0.5*(Connectors[i].Block1->calc(Solid_phase()[p].vs_coefficient,ii) + Connectors[i].Block2->calc(Solid_phase()[p].vs_coefficient,ii));
						if (Q_adv>0)
                        {	F[get_member_no(Connectors[i].Block1N,p,l,k)] += (w()*Q_adv*Blocks[Connectors[i].Block1N].G[p][l]*Blocks[Connectors[i].Block1N].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block1N].G_star[p][l]*Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p,l)])*Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l];
                            F[get_member_no(Connectors[i].Block2N,p,l,k)] -= (w()*Q_adv*Blocks[Connectors[i].Block1N].G[p][l]*Blocks[Connectors[i].Block1N].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block1N].G_star[p][l]*Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p,l)])*Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l];
						}
						if (Q_adv<0)
                        {	F[get_member_no(Connectors[i].Block1N,p,l,k)] += (w()*Q_adv*Blocks[Connectors[i].Block2N].G[p][l]*Blocks[Connectors[i].Block2N].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block2N].G_star[p][l]*Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p,l)])*Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l];
                            F[get_member_no(Connectors[i].Block2N,p,l,k)] -= (w()*Q_adv*Blocks[Connectors[i].Block2N].G[p][l]*Blocks[Connectors[i].Block2N].CG[k][get_member_no(p,l)] + (1-w())*Q_adv_star*Blocks[Connectors[i].Block2N].G_star[p][l]*Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p,l)])*Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l];
						}
					}


				}
				//diffusion
                for (unsigned int i=0; i<Connectors.size(); i++)
				{
					if (p==-2)
					{
                        double exchange = Connectors[i].A*(w()*Connectors[i].dispersion[k] + (1 - w())*Connectors[i].dispersion_star[k]) / Connectors[i].d*min(Heavyside(get_capacity_star(Connectors[i].Block2N, l, p) - 1e-13), Heavyside(get_capacity_star(Connectors[i].Block1N, l, p) - 1e-13));
                        double term1 = w()*Blocks[Connectors[i].Block2N].CG[k][get_member_no(p,l)]+(1-w())*Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p,l)];
                        double term2 = w()*Blocks[Connectors[i].Block1N].CG[k][get_member_no(p,l)]+(1-w())*Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p,l)];
						exchange*=(term1-term2);
                        F[get_member_no(Connectors[i].Block1N,p,l,k)] -= exchange;
                        F[get_member_no(Connectors[i].Block2N,p,l,k)] += exchange;
					}
					else if (p==-1)
					{

					}
					else
					{
                        double exchange = Connectors[i].A*(w()*Connectors[i].c_dispersion[p] + (1 - w())*Connectors[i].c_dispersion_star[p]) / Connectors[i].d*min(Heavyside(get_capacity_star(Connectors[i].Block1N, l, p) - 1e-13), Heavyside(get_capacity_star(Connectors[i].Block2N, l, p) - 1e-13));;
                        double term1 = w()*Blocks[Connectors[i].Block2N].G[p][l]*Blocks[Connectors[i].Block2N].CG[k][get_member_no(p,l)]+(1-w())*Blocks[Connectors[i].Block2N].G_star[p][l]*Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p,l)];
                        double term2 = w()*Blocks[Connectors[i].Block1N].G[p][l]*Blocks[Connectors[i].Block1N].CG[k][get_member_no(p,l)]+(1-w())*Blocks[Connectors[i].Block1N].G_star[p][l]*Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p,l)];
                        exchange*=(Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l]*Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l])*(term1-term2);
                        F[get_member_no(Connectors[i].Block1N,p,l,k)] -= exchange;
                        F[get_member_no(Connectors[i].Block2N,p,l,k)] += exchange;
					}
				}

			}

		}

		// solid mass transfer
        for (unsigned int i=0; i<Blocks.size(); i++)
		{
			for (int p=0; p<min(int(Blocks[i].Solid_phase.size()),max_phases); p++)
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
        for (unsigned int i=0; i<Blocks.size(); i++)
		{
			for (int p=-1; p<min(int (Blocks[i].Solid_phase.size()),max_phases); p++)
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

		// build_up
        for (unsigned int i=0; i<Blocks.size(); i++)
		{
			for (int p=-1; p<min(int (Blocks[i].Solid_phase.size()),max_phases); p++)
			{	int _t;
				if (p<0) _t=1; else _t=Blocks[0].Solid_phase[p]->n_phases;
				for (int l=0; l<_t; l++)
				{

					if (p==-1)
					{
						for (unsigned int j=0; j<Blocks[i].buildup.size(); j++)
							if (Blocks[i].buildup[j]->phase == "sorbed")
								F[get_member_no(i,p,l,k)] -= Blocks[i].buildup[j]->buildup((1-w())*Blocks[i].CG_star[k][get_member_no(p,l)] + w()*Blocks[i].CG[k][get_member_no(p,l)],&Blocks[i])*Blocks[i].V;
					}
					else
					{
						for (unsigned int j=0; j<Blocks[i].buildup.size(); j++)
							if ((Blocks[i].buildup[j]->solid == Blocks[i].Solid_phase[p]->name) && (Blocks[i].buildup[j]->phase == Blocks[i].Solid_phase[p]->phase_names[l]))
								F[get_member_no(i,p,l,k)] -= Blocks[i].buildup[j]->buildup((1-w())*Blocks[i].CG_star[k][get_member_no(p,l)] + w()*Blocks[i].CG[k][get_member_no(p,l)],&Blocks[i])*Blocks[i].V;


					}
				}
			}
		}

		//External flux
        for (unsigned int i=0; i<Blocks.size(); i++)
		{
			for (int p=-2; p<min(int (Blocks[i].Solid_phase.size()),max_phases); p++)
			{	int _t;
				if (p<0) _t=1; else _t=Blocks[0].Solid_phase[p]->n_phases;
				for (int l=0; l<_t; l++)
				{
					if (p==-2)
					{
						for (unsigned int j=0; j<Blocks[i].envexchange.size(); j++)
							if ((Blocks[i].envexchange[j]->phase == "aqueous") && (Blocks[i].envexchange[j]->constituent == RXN().cons[k].name))
								F[get_member_no(i,p,l,k)] -= w()*Blocks[i].envexchange[j]->calculate(&Blocks[i]) + (1-w())*Blocks[i].envexchange[j]->calculate_star(&Blocks[i]);
					}
					else if (p==-1)
					{
						for (unsigned int j=0; j<Blocks[i].envexchange.size(); j++)
							if ((Blocks[i].envexchange[j]->phase == "sorbed") && (Blocks[i].envexchange[j]->constituent == RXN().cons[k].name))
								F[get_member_no(i,p,l,k)] -= w()*Blocks[i].envexchange[j]->calculate(&Blocks[i]) + (1-w())*Blocks[i].envexchange[j]->calculate_star(&Blocks[i]);
					}
					else if (p>-1)
					{
						for (unsigned int j = 0; j<Blocks[i].envexchange.size(); j++)
							if ((Blocks[i].envexchange[j]->solid == Blocks[i].Solid_phase[p]->name) && (Blocks[i].envexchange[j]->phase == Blocks[i].Solid_phase[p]->phase_names[l]) && (Blocks[i].envexchange[j]->constituent == RXN().cons[k].name))
								F[get_member_no(i,p,l,k)] -= w()*Blocks[i].envexchange[j]->calculate(&Blocks[i]) + (1-w())*Blocks[i].envexchange[j]->calculate_star(&Blocks[i]);


					}
				}
			}
		}

	}

	//reactions
	CVector rate_chng = get_rxn_chng_rate();
	F -= rate_chng;

	// set concentration zero when the storage is zero
	for (unsigned int k = 0; k < RXN().cons.size(); k++)
	{
		for (int p = -2; p<min(int(Blocks[0].Solid_phase.size()),max_phases); p++)
		{
			int _t;
			if (p < 0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
			for (int l = 0; l < _t; l++)
			{
				// LHS
                for (unsigned int i=0; i < Blocks.size(); i++)
				{
					if (get_capacity_star(i, l, p) < 1e-13)
					{
						F[get_member_no(i, p, l, k)] = X.vec[get_member_no(i, p, l, k)];
						if (Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)]==0)
						{
							Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)] = get_capacity(i, l, p)*Blocks[i].CG[k][Blocks[i].get_member_no(p, l)]; // newly added
							if ((p == -2) && (Blocks[i].inflow.size() != 0))//needs to be modified to account for colloid-associated inflow
							{
                                Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)] += sum_interpolate(Blocks[i].inflow, Solution_State.t, RXN().cons[k].name)*dtt; // newly added
							}
						}

						if ((p == -2) && (Blocks[i].inflow.size() != 0))//needs to be modified to account for colloid-associated inflow
						{
                            F[get_member_no(i, p, l, k)] -= sum_interpolate(Blocks[i].inflow, Solution_State.t, RXN().cons[k].name);
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


CVector_arma CMedium::getres_Q(CVector_arma &X, const double &dtt)
{
	int max_phases = 10000;
	if (!sorption())
		max_phases = -1;
	CVector F(RXN().cons.size()*(Blocks[0].n_phases + n_default_phases)*Blocks.size());
	set_CG_star(X);
	evaluate_capacity_c_star();
	for (unsigned int k = 0; k<RXN().cons.size(); k++)
	{
		for (int p = -2; p<min(int(Blocks[0].Solid_phase.size()), max_phases); p++)
		{
			int _t;
			if (p<0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
			for (int l = 0; l<_t; l++)
			{
				// LHS
                for (unsigned int i=0; i<Blocks.size(); i++)
				{
					F[get_member_no(i, p, l, k)] = (get_capacity_star(i, l, p)*Blocks[i].CG_star[k][get_member_no(p, l)] - get_capacity(i, l, p)*Blocks[i].CG[k][get_member_no(p, l)]) / dtt;

					if ((p == -2) && (Blocks[i].inflow.size() != 0))//needs to be modified to account for colloid-associated inflow
					{
                        F[get_member_no(i, p, l, k)] -= sum_interpolate(Blocks[i].inflow, Solution_State.t, "flow")*sum_interpolate(Blocks[i].inflow, Solution_State.t, RXN().cons[k].name);

						for (unsigned int j = 0; j < Blocks[i].evaporation_m.size(); j++) //newly added
						{
							if (Blocks[i].evaporation_m[j]->uptake == true)
                                F[get_member_no(i, p, l, k)] += Blocks[i].outflow_corr_factor*(Blocks[i].get_evaporation(j, Solution_State.t)*(w()*Blocks[i].CG_star[k][get_member_no(p, l)] + (1 - w())*Blocks[i].CG[k][get_member_no(p, l)]));
						}

					}


				}




                for (unsigned int i=0; i<Connectors.size(); i++)
				{
					//advection

					if (p == -2)
					{
						vector<int> ii;
						ii.push_back(k);
                        double Q_adv_star = (Connectors[i].Q_star - Connectors[i].Q_v_star)*Connectors[i].flow_factor + Connectors[i].settling*RXN().cons[k].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*Connectors[i].A_star*0.5*(Connectors[i].Block1->calc_star(RXN().cons[k].vs_coefficient, ii) + Connectors[i].Block2->calc_star(RXN().cons[k].vs_coefficient, ii));
                        double Q_adv = (Connectors[i].Q - Connectors[i].Q_v)*Connectors[i].flow_factor + Connectors[i].settling*RXN().cons[k].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*Connectors[i].A*0.5*(Connectors[i].Block1->calc(RXN().cons[k].vs_coefficient, ii) + Connectors[i].Block2->calc(RXN().cons[k].vs_coefficient, ii));

						if (w()*Q_adv + (1 - w())*Q_adv_star>0)
						{
                            F[get_member_no(Connectors[i].Block1N, p, l, k)] += (w()*Q_adv*Blocks[Connectors[i].Block1N].CG[k][get_member_no(p, l)] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p, l)])*RXN().cons[k].mobile;
                            F[get_member_no(Connectors[i].Block2N, p, l, k)] -= (w()*Q_adv*Blocks[Connectors[i].Block1N].CG[k][get_member_no(p, l)] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p, l)])*RXN().cons[k].mobile;

						}
						else
						{
                            F[get_member_no(Connectors[i].Block1N, p, l, k)] += (w()*Q_adv*Blocks[Connectors[i].Block2N].CG[k][get_member_no(p, l)] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p, l)])*RXN().cons[k].mobile;
                            F[get_member_no(Connectors[i].Block2N, p, l, k)] -= (w()*Q_adv*Blocks[Connectors[i].Block2N].CG[k][get_member_no(p, l)] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p, l)])*RXN().cons[k].mobile;
						}

					}
					else if (p == -1)
					{


					}
					else
					{
						vector<int> ii;
						ii.push_back(p);
                        double Q_adv_star = (Connectors[i].Q_star - Connectors[i].Q_v_star)*Connectors[i].flow_factor + Connectors[i].settling*Solid_phase()[p].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*0.5*(Connectors[i].Block1->calc_star(Solid_phase()[p].vs_coefficient, ii) + Connectors[i].Block2->calc_star(Solid_phase()[p].vs_coefficient, ii));
                        double Q_adv = (Connectors[i].Q - Connectors[i].Q_v)*Connectors[i].flow_factor + Connectors[i].settling*Solid_phase()[p].vs*sgn(Connectors[i].Block1->z0 - Connectors[i].Block2->z0)*0.5*(Connectors[i].Block1->calc(Solid_phase()[p].vs_coefficient, ii) + Connectors[i].Block2->calc(Solid_phase()[p].vs_coefficient, ii));
						if (Q_adv>0)
						{
                            F[get_member_no(Connectors[i].Block1N, p, l, k)] += (w()*Q_adv*Blocks[Connectors[i].Block1N].G[p][l] * Blocks[Connectors[i].Block1N].CG[k][get_member_no(p, l)] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block1N].G_star[p][l] * Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p, l)])*Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l];
                            F[get_member_no(Connectors[i].Block2N, p, l, k)] -= (w()*Q_adv*Blocks[Connectors[i].Block1N].G[p][l] * Blocks[Connectors[i].Block1N].CG[k][get_member_no(p, l)] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block1N].G_star[p][l] * Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p, l)])*Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l];
						}
						if (Q_adv<0)
						{
                            F[get_member_no(Connectors[i].Block1N, p, l, k)] += (w()*Q_adv*Blocks[Connectors[i].Block2N].G[p][l] * Blocks[Connectors[i].Block2N].CG[k][get_member_no(p, l)] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block2N].G_star[p][l] * Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p, l)])*Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l];
                            F[get_member_no(Connectors[i].Block2N, p, l, k)] -= (w()*Q_adv*Blocks[Connectors[i].Block2N].G[p][l] * Blocks[Connectors[i].Block2N].CG[k][get_member_no(p, l)] + (1 - w())*Q_adv_star*Blocks[Connectors[i].Block2N].G_star[p][l] * Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p, l)])*Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l];
						}
					}


				}
				//diffusion
                for (unsigned int i=0; i<Connectors.size(); i++)
				{
					if (p == -2)
					{
                        double exchange = Connectors[i].A*(w()*Connectors[i].dispersion[k] + (1 - w())*Connectors[i].dispersion_star[k]) / Connectors[i].d*min(Heavyside(get_capacity_star(Connectors[i].Block2N, l, p) - 1e-13), Heavyside(get_capacity_star(Connectors[i].Block1N, l, p) - 1e-13))*RXN().cons[k].mobile;
                        double term1 = w()*Blocks[Connectors[i].Block2N].CG[k][get_member_no(p, l)] + (1 - w())*Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p, l)];
                        double term2 = w()*Blocks[Connectors[i].Block1N].CG[k][get_member_no(p, l)] + (1 - w())*Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p, l)];
						exchange *= (term1 - term2);
                        F[get_member_no(Connectors[i].Block1N, p, l, k)] -= exchange;
                        F[get_member_no(Connectors[i].Block2N, p, l, k)] += exchange;
					}
					else if (p == -1)
					{

					}
					else
					{
                        double exchange = Connectors[i].A*(w()*Connectors[i].c_dispersion[p] + (1 - w())*Connectors[i].c_dispersion_star[p]) / Connectors[i].d*min(Heavyside(get_capacity_star(Connectors[i].Block1N, l, p) - 1e-13), Heavyside(get_capacity_star(Connectors[i].Block2N, l, p) - 1e-13));;
                        double term1 = w()*Blocks[Connectors[i].Block2N].G[p][l] * Blocks[Connectors[i].Block2N].CG[k][get_member_no(p, l)] + (1 - w())*Blocks[Connectors[i].Block2N].G_star[p][l] * Blocks[Connectors[i].Block2N].CG_star[k][get_member_no(p, l)];
                        double term2 = w()*Blocks[Connectors[i].Block1N].G[p][l] * Blocks[Connectors[i].Block1N].CG[k][get_member_no(p, l)] + (1 - w())*Blocks[Connectors[i].Block1N].G_star[p][l] * Blocks[Connectors[i].Block1N].CG_star[k][get_member_no(p, l)];
                        exchange *= (Blocks[Connectors[i].Block1N].Solid_phase[p]->mobility_factor[l] * Blocks[Connectors[i].Block2N].Solid_phase[p]->mobility_factor[l])*(term1 - term2);
                        F[get_member_no(Connectors[i].Block1N, p, l, k)] -= exchange;
                        F[get_member_no(Connectors[i].Block2N, p, l, k)] += exchange;
					}
				}

			}

		}

		// solid mass transfer
        for (unsigned int i=0; i<Blocks.size(); i++)
		{
			for (int p = 0; p<min(int(Blocks[i].Solid_phase.size()), max_phases); p++)
			{
				for (int l = 0; l<Blocks[i].Solid_phase[p]->n_phases; l++)
					for (int kk = 0; kk<Blocks[i].Solid_phase[p]->n_phases; kk++)
					{
						double exchange = (1 - w())*Blocks[i].K_star[p][l][kk] * Blocks[i].G_star[p][l] * Blocks[i].CG_star[k][get_member_no(p, l)] + w()*Blocks[i].K[p][l][kk] * Blocks[i].G[p][l] * Blocks[i].CG[k][get_member_no(p, l)];
						F[get_member_no(i, p, l, k)] += exchange*Blocks[i].V;
						F[get_member_no(i, p, kk, k)] -= exchange*Blocks[i].V;
					}
			}
		}

		//solute mass transfer
        for (unsigned int i=0; i<Blocks.size(); i++)
		{
			for (int p = -1; p<min(int(Blocks[i].Solid_phase.size()), max_phases); p++)
			{
				int _t;
				if (p<0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
				for (int l = 0; l<_t; l++)
				{
					double exchange = 0;
					if (p == -1)
					{
						exchange -= (1 - w())*Blocks[i].get_exchange_rate_star(p, l, k)*Blocks[i].CG_star[k][get_member_no(p, l)] + w()*Blocks[i].get_exchange_rate(p, l, k)*Blocks[i].CG[k][get_member_no(p, l)];
						exchange += ((1 - w())*Blocks[i].get_exchange_rate_star(p, l, k)*Blocks[i].CG_star[k][get_member_no(-2, l)] + w()*Blocks[i].get_exchange_rate(p, l, k)*Blocks[i].CG[k][get_member_no(-2, l)])*Blocks[i].get_kd(p, l, k);
						F[get_member_no(i, -2, 0, k)] += exchange*get_capacity(i, l, p);
						F[get_member_no(i, p, l, k)] -= exchange*get_capacity(i, l, p);
					}
					else
					{
						exchange -= (1 - w())*Blocks[i].get_exchange_rate_star(p, l, k)*Blocks[i].CG_star[k][get_member_no(p, l)] + w()*Blocks[i].get_exchange_rate(p, l, k)*Blocks[i].CG[k][get_member_no(p, l)];
						exchange += ((1 - w())*Blocks[i].get_exchange_rate_star(p, l, k)* Blocks[i].CG_star[k][get_member_no(-2, l)] + w()*Blocks[i].get_exchange_rate(p, l, k)*Blocks[i].CG[k][get_member_no(-2, l)]) * Blocks[i].get_kd(p, l, k);
						F[get_member_no(i, -2, 0, k)] += exchange*get_capacity(i, l, p);
						F[get_member_no(i, p, l, k)] -= exchange*get_capacity(i, l, p);

					}
				}
			}
		}

		// build_up
        for (unsigned int i=0; i<Blocks.size(); i++)
		{
            for (unsigned int j = 0; j<Blocks[i].buildup.size(); j++)
                if (Blocks[i].buildup[j]->phase == "aqueous" && Blocks[i].buildup[j]->constituent == RXN().cons[k].name)
                    F[get_member_no(i, -2, 0, k)] -= Blocks[i].buildup[j]->buildup((1 - w())*Blocks[i].CG_star[k][get_member_no(-2, 0)] + w()*Blocks[i].CG[k][get_member_no(-2, 0)], &Blocks[i])*Blocks[i].V;

            for (int p = -1; p<min(int(Blocks[i].Solid_phase.size()), max_phases); p++)
			{
				int _t;
				if (p<0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
				for (int l = 0; l<_t; l++)
				{
					if (p == -1)
					{
						for (unsigned int j = 0; j<Blocks[i].buildup.size(); j++)
                            if (Blocks[i].buildup[j]->phase == "sorbed" && Blocks[i].buildup[j]->constituent == RXN().cons[k].name)
								F[get_member_no(i, p, l, k)] -= Blocks[i].buildup[j]->buildup((1 - w())*Blocks[i].CG_star[k][get_member_no(p, l)] + w()*Blocks[i].CG[k][get_member_no(p, l)], &Blocks[i])*Blocks[i].V;
					}
					else
					{
						for (unsigned int j = 0; j<Blocks[i].buildup.size(); j++)
                            if ((Blocks[i].buildup[j]->solid == Blocks[i].Solid_phase[p]->name) && (Blocks[i].buildup[j]->phase == Blocks[i].Solid_phase[p]->phase_names[l]) && Blocks[i].buildup[j]->constituent == RXN().cons[k].name)
								F[get_member_no(i, p, l, k)] -= Blocks[i].buildup[j]->buildup((1 - w())*Blocks[i].CG_star[k][get_member_no(p, l)] + w()*Blocks[i].CG[k][get_member_no(p, l)], &Blocks[i])*Blocks[i].V;


					}
				}
			}
		}

		//External flux
        for (unsigned int i=0; i<Blocks.size(); i++)
		{
			for (int p = -2; p<min(int(Blocks[i].Solid_phase.size()), max_phases); p++)
			{
				int _t;
				if (p<0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
				for (int l = 0; l<_t; l++)
				{
					if (p == -2)
					{
						for (unsigned int j = 0; j<Blocks[i].envexchange.size(); j++)
							if ((Blocks[i].envexchange[j]->phase == "aqueous") && (Blocks[i].envexchange[j]->constituent == RXN().cons[k].name))
								F[get_member_no(i, p, l, k)] -= w()*Blocks[i].envexchange[j]->calculate(&Blocks[i]) + (1 - w())*Blocks[i].envexchange[j]->calculate_star(&Blocks[i]);
					}
					else if (p == -1)
					{
						for (unsigned int j = 0; j<Blocks[i].envexchange.size(); j++)
							if ((Blocks[i].envexchange[j]->phase == "sorbed") && (Blocks[i].envexchange[j]->constituent == RXN().cons[k].name))
								F[get_member_no(i, p, l, k)] -= w()*Blocks[i].envexchange[j]->calculate(&Blocks[i]) + (1 - w())*Blocks[i].envexchange[j]->calculate_star(&Blocks[i]);
					}
					else if (p>-1)
					{
						for (unsigned int j = 0; j<Blocks[i].envexchange.size(); j++)
							if ((Blocks[i].envexchange[j]->solid == Blocks[i].Solid_phase[p]->name) && (Blocks[i].envexchange[j]->phase == Blocks[i].Solid_phase[p]->phase_names[l]) && (Blocks[i].envexchange[j]->constituent == RXN().cons[k].name))
								F[get_member_no(i, p, l, k)] -= w()*Blocks[i].envexchange[j]->calculate(&Blocks[i]) + (1 - w())*Blocks[i].envexchange[j]->calculate_star(&Blocks[i]);


					}
				}
			}
		}

	}

	//reactions
	CVector rate_chng = get_rxn_chng_rate();
	F -= rate_chng;

	// set concentration zero when the storage is zero
	for (unsigned int k = 0; k < RXN().cons.size(); k++)
	{
		for (int p = -2; p<min(int(Blocks[0].Solid_phase.size()), max_phases); p++)
		{
			int _t;
			if (p < 0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
			for (int l = 0; l < _t; l++)
			{
				// LHS
                for (unsigned int i=0; i < Blocks.size(); i++)
				{
					if (get_capacity_star(i, l, p) < 1e-13)
					{
						F[get_member_no(i, p, l, k)] = X[get_member_no(i, p, l, k)];
						if (Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)] == 0)
						{
							Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)] = get_capacity(i, l, p)*Blocks[i].CG[k][Blocks[i].get_member_no(p, l)]; // newly added
							if ((p == -2) && (Blocks[i].inflow.size() != 0))//needs to be modified to account for colloid-associated inflow
							{
                                Blocks[i].CG_stored_mass[k][Blocks[i].get_member_no(p, l)] += sum_interpolate(Blocks[i].inflow, Solution_State.t, RXN().cons[k].name)*dtt; // newly added
							}
						}

						if ((p == -2) && (Blocks[i].inflow.size() != 0))//needs to be modified to account for colloid-associated inflow
						{
                            F[get_member_no(i, p, l, k)] -= sum_interpolate(Blocks[i].inflow, Solution_State.t, RXN().cons[k].name);
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
		return const_no*Blocks.size()+k+(phase_no*Blocks.size())*RXN().cons.size()+block_no+2*int(RXN().cons.size())*Blocks.size();
	}

}

vector<int> CMedium::get_member_no_inv(int ii)
{
	vector<int> v;
	int max_phases = 10000;
	if (!sorption())
		max_phases = -1;
	for (unsigned int kk = 0; kk<RXN().cons.size(); kk++)
		for (int p = -2; p<min(int(Blocks[0].Solid_phase.size()),max_phases); p++)
		{
			int _t;
			if (p < 0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
			for (int l = 0; l < _t; l++)
                for (unsigned int i=0; i < Blocks.size(); i++)
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
	for (unsigned int block_no = 0; block_no < Blocks.size(); block_no++)
		for (int solid_id = -2; solid_id < int(Solid_phase().size()); solid_id++)
			for (int phase_no = 0; phase_no < Solid_phase()[solid_id].n_phases; phase_no++)
				for (unsigned int const_no = 0; const_no < RXN().cons.size(); const_no++)
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
		return Blocks[block_no].S+1e-3;
	else if (particle_no==-1)
		return Blocks[block_no].V*Blocks[block_no].bulk_density;
	else
		return Blocks[block_no].capacity_c[particle_no][phase_no] * Blocks[block_no].capacity_c_Q[particle_no][phase_no];
}

double CMedium::get_capacity_star(int block_no, int phase_no, int particle_no)
{
	if (particle_no==-2)
		return Blocks[block_no].S_star+1e-3;
	else if (particle_no==-1)
		return Blocks[block_no].V*Blocks[block_no].bulk_density;
	else
		return Blocks[block_no].capacity_c_star[particle_no][phase_no] * Blocks[block_no].capacity_c_star_Q[particle_no][phase_no];

}

void CMedium::set_CG_star(const CVector &X)
{
	int max_phases = 10000;
	if (!sorption())
		max_phases = -1;
	for (unsigned int k=0; k<RXN().cons.size(); k++)
		for (int p=-2; p<min(int(Blocks[0].Solid_phase.size()),max_phases); p++)
		{	int _t=1; if (p>-1) _t=Blocks[0].Solid_phase[p]->n_phases;
			for (int l=0; l<_t; l++)
                for (unsigned int i=0; i<Blocks.size(); i++)  Blocks[i].CG_star[k][Blocks[i].get_member_no(p,l)] = X.vec[get_member_no(i,p,l,k)];
		}

}

void CMedium::set_CG(const CVector &X)
{
	int max_phases = 10000;
	if (!sorption())
		max_phases = -1;
	for (unsigned int k=0; k<RXN().cons.size(); k++)
		for (int p=-2; p<min(int(Blocks[0].Solid_phase.size()),max_phases); p++)
		{	int _t=1; if (p>-1) _t=Blocks[0].Solid_phase[p]->n_phases;
			for (int l=0; l<_t; l++)
                for (unsigned int i=0; i<Blocks.size(); i++)  Blocks[i].CG[k][Blocks[i].get_member_no(p,l)] = X.vec[get_member_no(i,p,l,k)];
		}

}

void CMedium::set_G(const CVector &X)
{
	for (int p=0; p<int(Blocks[0].Solid_phase.size()); p++)
		for (int l=0; l<Solid_phase()[p].n_phases; l++)
            for (unsigned int i=0; i<Blocks.size(); i++)  Blocks[i].G[p][l] = X.vec[get_member_no(i,p,l)];
}

CVector CMedium::get_X_from_CG()
{
	CVector X(Blocks.size()*(Blocks[0].n_phases+n_default_phases)*RXN().cons.size());
	int max_phase = 10000;
	if (!sorption())
		max_phase = -1;
	for (unsigned int k=0; k<RXN().cons.size(); k++)
		for (int p=-2; p<min(int (Blocks[0].Solid_phase.size()),max_phase); p++)
		{	int _t=1; if (p>-1) _t=Blocks[0].Solid_phase[p]->n_phases;
			for (int l=0; l<_t; l++)
                for (unsigned int i=0; i<Blocks.size(); i++)  X[get_member_no(i,p,l,k)] = Blocks[i].get_CG(p,l,k);
		}
	return X;
}
CVector CMedium::get_X_from_G()
{
	CVector X(Blocks.size()*Blocks[0].n_phases);

	for (unsigned int p=0; p<Blocks[0].Solid_phase.size(); p++)
		for (int l=0; l<Blocks[0].Solid_phase[p]->n_phases; l++)
            for (unsigned int i=0; i<Blocks.size(); i++)  X[get_member_no(i,p,l)] =Blocks[i].G[p][l];

	return X;
}

CVector CMedium::get_rxn_chng_rate()
{
	int max_phases = 10000;
	if (!sorption())
		max_phases = -1;
	CVector F(RXN().cons.size()*(Blocks[0].n_phases+n_default_phases)*Blocks.size());
    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		if (Blocks[i].perform_rxn)
		{
			CVector rate_change = w()*Blocks[i].get_rxn_change(false) + (1 - w())*Blocks[i].get_rxn_change(true);
			for (unsigned int k = 0; k < RXN().cons.size(); k++)
				for (int p = -2; p < min(int(Blocks[0].Solid_phase.size()),max_phases); p++)
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
	FILEBTC = fopen((outputpathname() + "Solution_details_" + parent->ID + ".txt").c_str(), "a");
    fprintf(FILEBTC, "t: %lf, %le, %le(%i), %le, counters:, %i, %i, %i, J_updates:, %i, %i, %i, update_counts: %i, %i, %i, multis: %le, %le, pos_defs: %le, %le, wiggle: %le, %le, %le, %i, %s\n", Solution_State.t, Solution_State.dtt, Solution_State.base_dtt, where_base_dtt_changed, avg_redo_dtt, Solution_State.counter_flow, Solution_State.counter_colloid, Solution_State.counter_const, J_update, J_update_C, J_update_Q, Solution_State.J_h_update_count, Solution_State.J_c_update_count, Solution_State.J_q_update_count, Solution_State.pos_def_mult, Solution_State.pos_def_mult_Q, Solution_State.pos_def_ratio, Solution_State.pos_def_ratio_const, Solution_State.max_wiggle, Solution_State.wiggle_dt_mult, Solution_State.dt_fail, Solution_State.max_wiggle_id, Solution_State.fail_reason.c_str());
	fclose(FILEBTC);
}

void CMedium::evaluate_area(bool all)
{
    for (unsigned int i=0; i < Connectors.size(); i++)
	{
        if ((Connectors[i].const_area == false) || (all==true))
		{
            if ((Connectors[i].A == 0) || (Connectors[i].const_area == false))
			{
                Connectors[i].A = Connectors[i].calc(Connectors[i].area_expression);
                Connectors[i].A_star = Connectors[i].calc_star(Connectors[i].area_expression);
			}
		}
	}
}


int CMedium::lookup_external_flux(const string &S)
{
	int out = -1;
    for (unsigned int i=0; i < externalflux().size(); i++)
		if (tolower(S) == tolower(externalflux()[i].name))

			return i;

	return out;

}

int CMedium::lookup_evaporation(const string &S)
{
	int out = -1;
    for (unsigned int i=0; i < evaporation_model().size(); i++)
		if (tolower(S) == tolower(evaporation_model()[i].name))
			out = i;

	return out;

}

int CMedium::lookup_parameters(const string &S)
{
	int out = -1;
    for (unsigned int i=0; i < parameters().size(); i++)
		if (tolower(S) == tolower(parameters()[i].name))

			return i;

	return out;
}


int CMedium::lookup_sensors(const string &S)
{
	int out = -1;
    for (unsigned int i=0; i < sensors().size(); i++)
		if (tolower(S) == tolower(sensors()[i].name))
			return i;

	return out;
}

/*
int CMedium::lookup_observation(string S)
{
	int out = -1;
    for (unsigned int i=0; i < measured_quan().size(); i++)
		if (tolower(S) == tolower(measured_quan()[i].name))

			return i;

	return out;
}

*/
int CMedium::lookup_particle_type(const string &S)
{
	int out = -1;
    for (unsigned int i=0; i < Solid_phase().size(); i++)
		if (tolower(S) == tolower(Solid_phase()[i].name))

			return i;

	return out;
}

int CMedium::lookup_buildup(const string &S)
{
	int out = -1;
    for (unsigned int i=0; i < buildup().size(); i++)
		if (tolower(S) == tolower(buildup()[i].name))

			return i;

	return out;
}

void CMedium::writetolog(const string &S)
{
	fstream file(outputpathname() + log_file_name());
	file << S << std::endl;
	file.close();

}

CRestoreInfo CMedium::getrestoreinfo()
{
	CRestoreInfo R;
	R.LAI_res = getLAI();
	R.V_res = getV();
	R.X_res = getS();
	R.corr_fac_res = get_flow_factors();
	R.fix_stats_res = get_fixed_connect_status();
	R.CG_res = get_X_from_CG();
	R.G_res = get_X_from_G();
    R.t_res = Solution_State.t;
	R.dt_res = Solution_State.dtt;

	return R;

}

void CMedium::doredo(CRestoreInfo &R)
{
	set_flow_factors(R.corr_fac_res);
	set_fixed_connect_status(R.fix_stats_res);
	set_var("s", R.X_res.vec);
	set_var("V", R.V_res.vec);
	set_var("LAI", R.LAI_res.vec);
	avg_redo_dtt = (avg_redo_dtt*redo_count + Solution_State.dtt) / (redo_count + 1);
	redo_count++;
    Solution_State.t = R.t_res;
	R.dt_res = R.dt_res*dt_change_failure();
	Solution_State.dtt = R.dt_res;
	setH();
	evaluate_area();
	setQ0();
    Results.ANS.knockout(R.t_res);
    Results.ANS_obs.knockout(R.t_res);
    Results.ANS_colloids.knockout(R.t_res);
    Results.ANS_constituents.knockout(R.t_res);
    Results.ANS_MB.knockout(R.t_res);
	set_CG(R.CG_res);
	set_G(R.G_res);
}

void CMedium::doredo(redo_parameters &Redo_parameters)
{
	Redo_parameters.redo_counter++;
	Redo_parameters.redo = true;
	Redo_parameters.Res = clean_up_restore_points(Redo_parameters.Res, Solution_State.t);
	Redo_parameters.redo_time = Solution_State.t;
	Redo_parameters.redo_dt = Solution_State.dtt* dt_change_failure();;
	doredo(Redo_parameters.Res[max(int(Redo_parameters.Res.size()) - Redo_parameters.redo_counter, 0)]);
	Solution_State.base_dtt = Solution_State.dtt;
	Redo_parameters.redo_to_time = Solution_State.t;
}

void CMedium::update_rxn_params()
{
	if (temperature.size() > 0)
	{
        for (unsigned int i=0; i < RXN().parameters.size(); i++)
			RXN().parameters[i].eff_value = RXN().parameters[i].value*pow(RXN().parameters[i].tempcorr, current_temperature - 20);
	}
	else
        for (unsigned int i=0; i < RXN().parameters.size(); i++)
			RXN().parameters[i].eff_value = RXN().parameters[i].value;
}

void CMedium::update_light_temperature()
{
	if (light.size() > 0)
        current_light = light[0].interpol(Solution_State.t);
	else
		current_light = 0;

	if (temperature.size() > 0)
        current_temperature = temperature[0].interpol(Solution_State.t);
	else
		current_temperature = 20;


	if (wind.size() > 0)
        current_wind = wind[0].interpol(Solution_State.t);
	else
		current_wind = 0;


	if (r_humidity.size() > 0)
        current_relative_humidity = r_humidity[0].interpol(Solution_State.t);
	else
		current_relative_humidity = 0;

}

double CMedium::get_nextcontrolinterval(double _t)
{
	double t_min = 1e100;
    for (unsigned int i=0; i<controllers().size(); i++)
	{
		double t_1 = Timemin + (int((_t - Timemin) / controllers()[i].interval) + 1)*controllers()[i].interval;
		if (t_1 == _t) t_1 = _t+controllers()[i].interval;
		t_min = min(t_min, t_1);
	}

	return t_min;
}

void CMedium::set_control_params(int controller_no)
{
    for (unsigned int i=0; i<controllers()[controller_no].application_spec.location.size(); i++)
	{
		if (controllers()[controller_no].application_spec.location_type[i] == 2)

		{
			Blocks[controllers()[controller_no].application_spec.location[i]].set_val(controllers()[controller_no].application_spec.quan[i], controllers()[controller_no].value);
            Connectors[controllers()[controller_no].application_spec.location[i]].set_val(controllers()[controller_no].application_spec.quan[i], controllers()[controller_no].value);
		}
		else if (controllers()[controller_no].application_spec.location_type[i] == 0)
			Blocks[controllers()[controller_no].application_spec.location[i]].set_val(controllers()[controller_no].application_spec.quan[i], controllers()[controller_no].value);
		else if (controllers()[controller_no].application_spec.location_type[i] == 1)
            Connectors[controllers()[controller_no].application_spec.location[i]].set_val(controllers()[controller_no].application_spec.quan[i], controllers()[controller_no].value);

	}

}

void CMedium::clear()
{
	parent = 0;
    Results.ANS.clear();
    Results.ANS_colloids.clear();
    Results.ANS_constituents.clear();
    Results.Solution_dt.clear();
    Results.ANS_control.clear();
    Results.ANS_obs.clear();

    //ANS_MB.clear();
    Connectors.clear();
	Blocks.clear();

}

vector<CRestoreInfo> CMedium::clean_up_restore_points(vector<CRestoreInfo> &Res, double t)
{
	vector<CRestoreInfo> Res_out;
    for (unsigned int i=0; i < Res.size(); i++)
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
    for (unsigned int i=0; i < Connectors.size(); i++)
        Connectors[i].Q_star = Q[i];
    Solution_State.failed = false;
    Solution_State.fail_reason = "none";
}

double CMedium::getflow(int connector_ID)
{
	double flow = 0;
    if (Connectors[connector_ID].presc_flow)
	{
        Connectors[connector_ID].Q_star = flow;
        Connectors[connector_ID].flow_calc_done = true;
        return Connectors[connector_ID].presc_flowrate.interpol(Solution_State.t);
	}
    if (Connectors[connector_ID].flow_calc_done == true)
        return Connectors[connector_ID].Q_star;
	vector<int> num_pre_flows = get_num_block_unpres_inflows(connector_ID);
	if ((num_pre_flows[0] <= num_pre_flows[1]) || num_pre_flows[1]==1)
	{
        for (unsigned int j = 0; j < Connectors[connector_ID].Block1->connectors.size(); j++)
		{
            if (Connectors[connector_ID].Block1->connectors[j] != connector_ID)
                flow += getflow(Connectors[connector_ID].Block1->connectors[j])*(2*Connectors[connector_ID].Block1->connectors_se[j]-1);
		}
        for (unsigned int j = 0; j < Connectors[connector_ID].Block1->inflow.size(); j++)
		{
            flow += Connectors[connector_ID].Block1->inflow[j].interpolate(Solution_State.t)[0];
		}
	}
	else
	{
        for (unsigned int j = 0; j < Connectors[connector_ID].Block2->connectors.size(); j++)
		{
            if (Connectors[connector_ID].Block2->connectors[j] != connector_ID)
                flow -= getflow(Connectors[connector_ID].Block2->connectors[j])*(2 * Connectors[connector_ID].Block1->connectors_se[j] - 1);
		}
        for (unsigned int j = 0; j < Connectors[connector_ID].Block2->inflow.size(); j++)
		{
            flow -= Connectors[connector_ID].Block2->inflow[j].interpolate(Solution_State.t)[0];
		}

	}
    Connectors[connector_ID].Q_star = flow;
    Connectors[connector_ID].flow_calc_done = true;
	return flow;
}

vector<int> CMedium::get_num_block_unpres_inflows(int connector_ID)
{
	vector<int> num_inflows(2);

	int numpresflow = 0;
    for (unsigned int i=0; i < Connectors[connector_ID].Block1->connectors.size(); i++)
	{
        if (!Connectors[Connectors[connector_ID].Block1->connectors[i]].presc_flow) numpresflow++;
	}
	num_inflows[0] = numpresflow;

	numpresflow = 0;
    for (unsigned int i=0; i < Connectors[connector_ID].Block2->connectors.size(); i++)
	{
        if (!Connectors[Connectors[connector_ID].Block2->connectors[i]].presc_flow) numpresflow++;
	}
	num_inflows[1] = numpresflow;

	return num_inflows;
}

void CMedium::false_connector_flow_calc()
{
    for (unsigned int i=0; i < Connectors.size(); i++)
        Connectors[i].flow_calc_done = false;

}

string CMedium::create_hydro_steady_matrix_inv()
{
	CMatrix M;
    for (unsigned int i=0; i < Blocks.size(); i++)
	{
		if (Blocks[i].connectors.size() > 1)
        {	vector<double> v(Connectors.size());
			for (unsigned int j = 0; j < Blocks[i].connectors.size(); j++)
				v[Blocks[i].connectors[j]] = 2 * Blocks[i].connectors_se[j] - 1;
			M.matr.push_back(v);
		}
	}
    for (unsigned int i=0; i < Connectors.size(); i++)
	{
        if (Connectors[i].presc_flow)
		{
            vector<double> v(Connectors.size());
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
	else if (M.getnumcols() == 0 && M.getnumrows() == 0)
	{
		failed_res = "The steady-state hydraulic system cannot be solved";
	}
	else
		hydro_steady_matrix_inv = inv(M);
	return failed_res;


}

CVector CMedium::get_steady_hydro_RHS()
{
	vector<double> v;
    for (unsigned int i=0; i < Blocks.size(); i++)
	{
        if (Blocks[i].connectors.size() > 1) v.push_back(-sum_interpolate(Blocks[i].inflow, Solution_State.t, "flow"));
	}
    for (unsigned int i=0; i < Connectors.size(); i++)
	{
        if (Connectors[i].presc_flow)
            v.push_back(Connectors[i].presc_flowrate.interpol(Solution_State.t));
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
    Blocks.clear(); Connectors.clear();
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

vector<param_range>& CMedium::parameters()
{
	return parent->parameters;
}

vector<CSensor>& CMedium::sensors()
{
	return parent->Control.Sensors;
}

vector<CController>& CMedium::controllers()
{
	return parent->Control.Controllers;
}

vector<CObjectiveFunction>& CMedium::objective_functions()
{
	return parent->Control.ObjectiveFunctions;
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

string& CMedium::solution_method()
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

double& CMedium::minimum_acceptable_negative_conc()
{
	return parent->SP.minimum_acceptable_negative_conc;
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

bool & CMedium::sorption()
{
	return parent->SP.sorption;
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

void CMedium::write_to_detail_file(string s)
{
	std::ofstream outfile(outputpathname() + "Solution_details_" + parent->ID + ".txt", std::fstream::app);
	outfile << s << std::endl;
	outfile.close();
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

double & CMedium::maximum_run_time()
{
	return parent->SP.maximum_run_time;
}

bool& CMedium::check_oscillation()
{
	return parent->SP.check_oscillation;
}

bool& CMedium::negative_concentration_allowed()
{
	return parent->SP.negative_concentration_allowed;
}

bool& CMedium::steady_state_hydro()
{
	return parent->SP.steady_state_hydro;
}

double& CMedium::max_dt()
{
	return parent->SP.max_dt;
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
    for (unsigned int i=0; i < measured_quan().size(); i++)
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

int CMedium::lookup_experiment(const string &S)
{
    for (unsigned int i=0; i < parent->Medium.size(); i++)
		if (S == parent->Medium[i].name) return i;
	return -1;
}

double CMedium::calc_obj_function(double time_interval)
{
	return 0.0;
}

int CMedium::lookup_controllers(const string &S)
{
	int out = -1;
    for (unsigned int i=0; i < controllers().size(); i++)
		if (tolower(S) == tolower(controllers()[i].name))
			return i;

	return out;
}

int CMedium::lookup_objective_functions(const string &S)
{
	int out = -1;
    for (unsigned int i=0; i < objective_functions().size(); i++)
		if (tolower(S) == tolower(objective_functions()[i].name))
			return i;

	return out;
}


void CMedium::onestepsolve_flow_ar(double dt)
{
	int indicator = 1;
	int done = 0;
	CVector_arma pos_def;
	vector<int> old_fixed_connect_status = get_fixed_connect_status();
    CVector _S = getS();
    CVector_arma X_old = _S;

    for (unsigned int i=0; i<Blocks.size(); i++)
	{
		if (Blocks[i].setzero == 1) X_old[i] = dt;
		if (Blocks[i].setzero == 2) X_old[i] = Blocks[i].outflow_corr_factor;
	}
    Solution_State.fail_reason = "none";
	solution_detail = "none";
	CVector correction_factor_old = get_flow_factors();

	while ((indicator == 1) && (done<2))
	{
		bool fixed_connect;
		done += 1;

		int err_expand_counter = 0;
		int J_update1, J_update2;
		J_update1 = J_update2 = J_update;
        CVector _S = getS();
        CVector_arma X = _S;    //getting the Storage of blocks, test
		fixed_connect = false;
        for (unsigned int i=0; i<Blocks.size(); i++)
		{
			if (Blocks[i].setzero == 1) X[i] = dt;
			if (Blocks[i].setzero == 2) X[i] = Blocks[i].outflow_corr_factor;
			if (Blocks[i].setzero != 0) fixed_connect = true;
		}

		CVector_arma F = getres_S(X, dt);

		double err = F.norm2();
		double err_p = err;

        Solution_State.counter_flow = 0;
		double lambda = 1;
        Solution_State.J_h_update_count = 0;
		if ((err==err) !=true)
		{
			vector<int> nans = F.get_nan_elements();
            Solution_State.fail_reason = "indefinite X or F in hydro @";
			solution_detail = "indefinite X or F in hydro @";
			for (unsigned int kk = 0; kk < nans.size(); kk++)
			{
                Solution_State.fail_reason = Solution_State.fail_reason + Blocks[nans[kk]].ID + ",";
				solution_detail = solution_detail +"<b>" + string2QString(Blocks[nans[kk]].ID) + "</b>,";
			}
			nans = infnan_H_blocks();
			if (nans.size() > 0)
			{
				for (unsigned int kk = 0; kk < nans.size(); kk++)
				{
                    Solution_State.fail_reason = Solution_State.fail_reason + "Head not a number @ " + Blocks[nans[kk]].ID + ",";
					solution_detail = solution_detail + "<b>" + string2QString(Blocks[nans[kk]].ID) + "</b>,";
				}
			}
			nans = infnan_H_flows();
			if (nans.size() > 0)
			{
				for (unsigned int kk = 0; kk < nans.size(); kk++)
				{
                    Solution_State.fail_reason = Solution_State.fail_reason + "Flow not a number @ " + Connectors[nans[kk]].ID + ",";
                    solution_detail = solution_detail + "<b>" + string2QString(Connectors[nans[kk]].ID) + "</b>,";
				}
			}

			set_fixed_connect_status(old_fixed_connect_status);
			setS_star(X_old);
			return;
		}
		int ini_max_error_elements = -1;
		while (err>tol())
		{

			if ((F == F) != true || (X == X) != true || (F.is_finite() == false) || (X.is_finite() == false))
			{
				vector<int> nans = F.get_nan_elements();
                Solution_State.fail_reason = "indefinite X or F in hydro @";
				solution_detail = "indefinite X or F in hydro @";
				for (unsigned int kk = 0; kk < nans.size(); kk++)
				{
                    Solution_State.fail_reason = Solution_State.fail_reason + Blocks[nans[kk]].ID + ",";
					solution_detail = solution_detail + "<b>" + string2QString(Blocks[nans[kk]].ID) + "</b>,";
				}
				nans = infnan_H_blocks();
				if (nans.size() > 0)
				{
					for (unsigned int kk = 0; kk < nans.size(); kk++)
					{
                        Solution_State.fail_reason = Solution_State.fail_reason + "Head not a number @ " + Blocks[nans[kk]].ID + ",";
						solution_detail = solution_detail + "<b>" + string2QString(Blocks[nans[kk]].ID) + "</b>,";
					}
				}
				nans = infnan_H_flows();
				if (nans.size() > 0)
				{
					for (unsigned int kk = 0; kk < nans.size(); kk++)
					{
                        Solution_State.fail_reason = Solution_State.fail_reason + "Flow not a number @ " + Connectors[nans[kk]].ID + ",";
						solution_detail = solution_detail + "Flow not a number @  <b>" + string2QString(Blocks[nans[kk]].ID) + "</b>,";
					}
				}

				set_fixed_connect_status(old_fixed_connect_status);
				setS_star(X_old);
				return;
			}

			CVector_arma dx;
			CMatrix_arma M1;
			if ((((J_update1 == true) || M_arma.getnumrows() == 0 || ((InvJ1_arma.getnumrows() == 0)) && solution_method() == "Partial Inverse Jacobian Evaluation")) && (fixed_connect == false))
			{
				Solution_State.J_h_update_count++;
				M_arma = Jacobian_S(X, dt, true);
				M1 = normalize_diag(M_arma, M_arma);
				pos_def = M_arma.diag_ratio();
				Solution_State.epoch_count++;

				if (solution_method() == "Partial Inverse Jacobian Evaluation")
				{
					InvJ1_arma = inv(M1);
					if (InvJ1_arma.getnumcols() != int(Blocks.size()))
					{
						Solution_State.fail_reason = "Hydro Jacobian in not inversible";
						set_flow_factors(correction_factor_old);
						set_fixed_connect_status(old_fixed_connect_status);
						return;
					}
				}
				Solution_State.dtt_J_h1 = dt;
				J_update1 = false;
			}

			if (((J_update2 == true) || M_arma.getnumrows() == 0 || ((InvJ2_arma.getnumrows() == 0 && solution_method() == "Partial Inverse Jacobian Evaluation"))) && (fixed_connect == true))
			{
				Solution_State.J_h_update_count++;
				M_arma = Jacobian_S(X, dt, true);
				M1 = normalize_diag(M_arma, M_arma);
				Solution_State.epoch_count++;
				pos_def = M_arma.diag_ratio();

				if (solution_method() == "Partial Inverse Jacobian Evaluation")
				{
					InvJ2_arma = inv(M1);

					if (InvJ2_arma.getnumcols() != int(Blocks.size()))
					{
						Solution_State.fail_reason = "Hydro Jacobian in not inversible";
						solution_detail = "Hydro Jacobian in not inversible";
						set_flow_factors(correction_factor_old);
						set_fixed_connect_status(old_fixed_connect_status);
						return;
					}
				}

				Solution_State.dtt_J_h2 = dt;
				J_update2 = false;
			}

			if (fixed_connect)
			{
				if (solution_method() == "Partial Inverse Jacobian Evaluation")
					dx = (InvJ2_arma*normalize_diag(F, M_arma));
				else if (solution_method() == "Direct Solution")
					dx = F / M_arma;
				else
					dx = (InvJ2_arma*normalize_diag(F, M_arma));

                if (dx.num==0)
                {
                    Solution_State.fail_reason = "Hydro Jacobian in not inversible";
                    solution_detail = "Hydro Jacobian in not inversible";
                    set_flow_factors(correction_factor_old);
                    set_fixed_connect_status(old_fixed_connect_status);
                    return;
                }
                X -= lambda * ((dt / Solution_State.dtt_J_h2)*dx);
			}
			else
			{
				if (solution_method() == "Partial Inverse Jacobian Evaluation")
					dx = (InvJ1_arma*normalize_diag(F, M_arma));
				else if (solution_method() == "Direct Solution")
					dx = F/ M_arma;
				else
					dx = (InvJ1_arma*normalize_diag(F, M_arma));

                if (dx.num==0)
                {
                    Solution_State.fail_reason = "Hydro Jacobian in not inversible";
                    solution_detail = "Hydro Jacobian in not inversible";
                    set_flow_factors(correction_factor_old);
                    set_fixed_connect_status(old_fixed_connect_status);
                    return;
                }

				X -= lambda * ((dt / Solution_State.dtt_J_h1)*dx);
			}



			F = getres_S(X, dt);

			err_p = err;
			err = F.norm2();
			if (err / err_p>0.9)
			{
			    if (fixed_connect) J_update2 = true; else J_update1 = true;
			}

			if ((dx / X).abs_max()<1e-15) err = 0;
			if (err >= err_p)
			{
				if (fixed_connect) J_update2 = true; else J_update1 = true; err_expand_counter++; X = X_old; set_flow_factors(correction_factor_old); lambda /= 2.0;
				if (ini_max_error_elements == -1) ini_max_error_elements = F.abs_max_elems();
				err = 10000;
			}
			if (err_expand_counter>4)
			{
                Solution_State.fail_reason = "Expanding error in hydro ";
                Solution_State.fail_reason = Solution_State.fail_reason + ", max error @ " + Blocks[F.abs_max_elems()].ID;
                Solution_State.fail_reason = Solution_State.fail_reason + ", ini max error @ " + Blocks[ini_max_error_elements].ID;
				solution_detail = "Expanding error in hydro ";
                solution_detail = solution_detail + ", max error @ " + string2QString(Blocks[F.abs_max_elems()].ID);
                solution_detail = solution_detail + ", ini max error @ <b>" + string2QString(Blocks[ini_max_error_elements].ID + "</b>");

				set_flow_factors(correction_factor_old);
				set_fixed_connect_status(old_fixed_connect_status);
				setS_star(X_old);
				return;
			}

			if ((err == err) != true || !isfinite(err))
			{
				vector<int> nans = F.get_nan_elements();
                Solution_State.fail_reason = "indefinite X or F in hydro @";
				solution_detail = "indefinite X or F in hydro @";
				for (unsigned int kk = 0; kk < nans.size(); kk++)
				{
                    Solution_State.fail_reason = Solution_State.fail_reason + Blocks[nans[kk]].ID + ",";
					solution_detail = solution_detail + "<b>" + string2QString(Blocks[nans[kk]].ID) + "</b>,";
				}
				nans = infnan_H_blocks();
				if (nans.size() > 0)
				{
					for (unsigned int kk = 0; kk < nans.size(); kk++)
					{
                        Solution_State.fail_reason = Solution_State.fail_reason + "Head not a number @ " + Blocks[nans[kk]].ID + ",";
						solution_detail = solution_detail + "<b>" + string2QString(Blocks[nans[kk]].ID) + "</b>,";
					}
				}
				nans = infnan_H_flows();
				if (nans.size() > 0)
				{
					for (unsigned int kk = 0; kk < nans.size(); kk++)
					{
                        Solution_State.fail_reason = Solution_State.fail_reason + "Flow not a number @ " + Connectors[nans[kk]].ID + ",";
                        solution_detail = solution_detail + "<b>" + string2QString(Connectors[nans[kk]].ID) + "</b>,";
					}
				}
				set_flow_factors(correction_factor_old);
				set_fixed_connect_status(old_fixed_connect_status);
				setS_star(X_old);
				return;
			}


            Solution_State.counter_flow++;
            if (Solution_State.counter_flow>nr_failure_criteria())
			{
                Solution_State.fail_reason = "Number of iteration exceeded the limit in hydro";
				solution_detail = "Number of iteration exceeded the limit in hydro";
				set_flow_factors(correction_factor_old);
				set_fixed_connect_status(old_fixed_connect_status);
				setS_star(X_old);
				return;
			}

		}

        Solution_State.failed = false;
        Solution_State.fail_reason = "none";
		solution_detail = "none";
		indicator = 0;

        for (int i=0; i<X.num; i++)
		{
			if ((X[i]<1e-15) && (X[i]>-1e-15)) X[i] = 0;
			if ((Blocks[i].setzero == 0) && (X[i]<0))
			{
				J_update = true;
				indicator = 1;
                Solution_State.failed = true;
				if (Blocks[i].S == 0)
					Blocks[i].setzero = 2;
				else
					Blocks[i].setzero = 2;
			}
			if (Blocks[i].outflow_corr_factor>1)
			{
                Solution_State.fail_reason = "block " + Blocks[i].ID + " is wet, " + "outflow factor = " + numbertostring(Blocks[i].outflow_corr_factor);
				solution_detail = string2QString("block " + Blocks[i].ID + " is wet, " + "outflow factor = " + numbertostring(Blocks[i].outflow_corr_factor));
				J_update = true;
				indicator = 1;
                Solution_State.failed = true;
				Blocks[i].setzero = 0;
				Blocks[i].outflow_corr_factor = 1;
			}
		}
	}
}

void CMedium::onestepsolve_colloid_ar(double dt)
{
	if (Blocks[0].n_phases == 0)
	{
		Solution_State.failed_colloid = false;
		return;
	}
    evaluate_dispersion();   //Connectors[i].evaluate_dispersion(); Connectors[i].evaluate_dispersion_star();  //Negative diffusion due to negative f[8]????
	evaluate_K();			//Blocks[i].evaluate_K();
	evaluate_capacity();	//Blocks[i].evaluate_capacity();
							//renew_G();

	CVector_arma X(Blocks.size()*Blocks[0].n_phases);
	CVector_arma X_old(Blocks.size()*Blocks[0].n_phases);

	for (unsigned int p = 0; p<Blocks[0].Solid_phase.size(); p++)
		for (int l = 0; l<Blocks[0].Solid_phase[p]->n_phases; l++)
            for (unsigned int i=0; i<Blocks.size(); i++)  X[get_member_no(i, p, l)] = Blocks[i].G[p][l];

	X_old = X;
	int error_expand_counter = 0;
	CVector_arma F = getres_C(X, dt);
	CVector_arma F_old = F;
	double lambda = 1;
    Solution_State.counter_colloid = 0;

	if ((F == F) != true || (X == X) != true || (F.is_finite() == false) || (X.is_finite() == false))
	{
		Solution_State.failed_colloid = true;
        Solution_State.fail_reason = "infinite X or F in colloids";
		Solution_State.pos_def_ratio = 1e-12;
		return;
	}

	double err = F.norm2();
	double err_p = err;

    Solution_State.J_c_update_count = 0;
	while ((err>tol()) || (X.min()<double(0)))
	{
		if ((J_update_C == true) || (M_C.getnumcols() != X.num))
		{
            Solution_State.J_c_update_count++;
			M_C_arma = Jacobian_C(X, dt);
            Solution_State.epoch_count++;
			CMatrix_arma M1 = normalize_diag(M_C_arma, M_C_arma);
			if (M_C_arma.getnumcols()>0) Solution_State.pos_def_ratio = M1.diag_ratio().abs_max(); else Solution_State.pos_def_ratio = 1e-12;
			if (fabs(det(M1))<1e-30)
			{
				set_G_star(X_old);
                Solution_State.fail_reason = "non strongly positive definite Jacobian in colloids";
				Solution_State.failed_colloid = true;
				return;

			}

			InvJ_C_arma = inv(M1);
			if (InvJ_C_arma.getnumcols() == 0)
			{
                Solution_State.fail_reason = "Colloid Jacobian in not inversible";
				Solution_State.failed_colloid = true;
				Solution_State.pos_def_ratio = 1e-12;
				return;
			}

			J_update_C = false;
            Solution_State.dtt_J_c = dt;
		}

        CVector_arma dx = (dt / Solution_State.dtt_J_c)*(InvJ_C_arma*normalize_diag(F, M_C_arma));

		X -= lambda*dx;

		F = getres_C(X, dt);
		err_p = err;
		err = F.norm2();
		if (err>err_p)
		{
			error_expand_counter++;
			J_update_Q = true;
			X = X_old;
			F = F_old;
			lambda /= 2.0;
		}


		if (error_expand_counter == 4)
		{
			set_G_star(X_old);
            Solution_State.fail_reason = "error expansion in particle transport";
			Solution_State.failed_colloid = true;
			return;
		}

        Solution_State.counter_colloid++;
        if (Solution_State.counter_colloid>nr_failure_criteria())
		{
			set_G_star(X_old);
            Solution_State.fail_reason = "Number of iterations exceeded the limit in colloids";
			Solution_State.failed_colloid = true;
			return;
		}
	}

	set_G_star(X); //Added by Babak to avoid negative values in response 12/22/15

	if (X.min() < double(-1e-13))
	{
        Solution_State.fail_reason = "Negative value for colloid concentration";
		Solution_State.failed_colloid = true;
	}
	else
		Solution_State.failed_colloid = false;
}

void CMedium::onestepsolve_const_ar(double dtt)
{
	evaluate_const_dispersion();
	//renew_CG();
	evaluate_capacity_c();
	int error_expand_counter = 0;
	update_rxn_params();
    CVector _CG = get_X_from_CG();
    CVector_arma X = _CG;

	CVector_arma X_old = X;
	CVector_arma F = getres_Q(X, dtt);
	CVector_arma F_old = F;
    Solution_State.counter_const = 0;

	if ((F == F) != true || (X == X) != true || (F.is_finite() == false) || (X.is_finite() == false))
	{
		Solution_State.failed_const = true;
        Solution_State.fail_reason = "infinite X or F in water quality";
		Solution_State.pos_def_ratio_const = 1e-12;
		return;
	}

	double err = F.norm2();
	double err_p = err;

	double lambda = 1;
    Solution_State.J_q_update_count = 0;
	while (err>tol())
	{
		CVector_arma dx;
		if ((J_update_Q == true) || (M_Q_arma.getnumrows() != X.num))
		{
            Solution_State.J_q_update_count++;
			M_Q_arma = Jacobian_Q(X, dtt);
			CMatrix_arma M1 = normalize_diag(M_Q_arma, M_Q_arma);
			CMatrix_arma D = M1.non_posdef_elems_m();
            Solution_State.epoch_count++;
			if (M_Q_arma.getnumcols()>0) Solution_State.pos_def_ratio_const = M1.diag_ratio().abs_max(); else Solution_State.pos_def_ratio_const = 1e-12;
			//if (pos_def_ratio_const > 1)
			//	Preconditioner_Q_arma = M1.Preconditioner();
			//else
			//Preconditioner_Q_arma = Identity_ar(F.num);
			if (fabs(det(M1))<1e-30)
			{
				set_CG_star(X_old);
                Solution_State.fail_reason = "Not strongly positive definite Jacobian in wq";
				Solution_State.failed_const = true;
				return;

			}
			if (solution_method() == "Partial Inverse Jacobian Evaluation")
			{
				InvJ_Q_arma = inv(M1);
				J_update_Q = false;
			}
			Solution_State.dtt_J_q = dtt;
		}


		if (InvJ_Q_arma.getnumcols() != 0 && solution_method() == "Partial Inverse Jacobian Evaluation")
		{
			dx = dtt / Solution_State.dtt_J_q*(InvJ_Q_arma*normalize_diag(F, M_Q_arma));
		}
		else if (M_Q_arma.getnumcols() > 0 || (dx == dx) != true || solution_method()=="Direct Solution")
		{

			//CVector FF = F;
			//CMatrix M_Q = M_Q_arma;
			//FF.writetofile("F.txt");
			//M_Q.writetofile("m.txt");
			//CMatrix Precond_Q = Preconditioner_Q_arma;
			//Precond_Q.writetofile("Precond.txt");

			dx = dtt / Solution_State.dtt_J_q*solve_ar(M_Q_arma, F);
			if ((dx.num == 0) || (dx == dx) != true)
			{
				set_CG_star(X_old);
				Solution_State.fail_reason = "Matrix not invertible in wq";
				Solution_State.failed_const = true;
				return;
			}
		}
		else
            dx = dtt / Solution_State.dtt_J_q*(InvJ_Q_arma*normalize_diag(F, M_Q_arma));



		X -= lambda*dx;

		F = getres_Q(X, dtt);
		err_p = min(err, err_p);
		err = F.norm2();
		if ((dx / X).abs_max()<1e-15) err = 0;

		if (err / err_p>0.8) J_update_Q = true;

		if ((err>err_p) && (err>tol()))
		{
			error_expand_counter++;
			J_update_Q = true;
			X = X_old;
			F = F_old;
			lambda /= 2.0;
		}

        Solution_State.counter_const++;

		if (error_expand_counter == 4)
		{
			set_CG_star(X_old);
            Solution_State.fail_reason = "error expansion in wq";
			Solution_State.failed_const = true;
			return;
		}

        if (Solution_State.counter_const>nr_failure_criteria())
		{
			set_CG_star(X_old);
            Solution_State.fail_reason = "Number of iterations exceeded the limit in wq";
			Solution_State.failed_const = true;
			return;
		}
	}
	if (negative_concentration_allowed() == false)
	{
		if (X.min() < -fabs(minimum_acceptable_negative_conc()))
		{
			vector<int> neg_vals_cons;
			vector<int> neg_vals_block;
			vector<double> neg_vals;
            for (int i=0; i < X.getsize(); i++)
				if (X[i] < -fabs(minimum_acceptable_negative_conc())) {
				neg_vals_block.push_back(get_member_no_inv(i)[0]); neg_vals_cons.push_back(get_member_no_inv(i)[3]);  neg_vals.push_back(X[i]);
				}
				else if (X[i] < 0) X[i] = 0;

            Solution_State.fail_reason = "Negative value in constituent ";
            for (unsigned int i=0; i < neg_vals_block.size(); i++) Solution_State.fail_reason = Solution_State.fail_reason + RXN().cons[neg_vals_cons[i]].name +", ";
            Solution_State.fail_reason = Solution_State.fail_reason + " concentration at ";
            for (unsigned int i=0; i < neg_vals_block.size(); i++) Solution_State.fail_reason = Solution_State.fail_reason + Blocks[neg_vals_block[i]].ID +", ";
            Solution_State.fail_reason = Solution_State.fail_reason + " values respectively = ";
            for (unsigned int i=0; i < neg_vals_block.size(); i++) Solution_State.fail_reason = Solution_State.fail_reason + numbertostring(neg_vals[i]) + ", ";
			Solution_State.failed_const = true;
		}
		else
		{
			Solution_State.failed_const = false;
            Solution_State.fail_reason == "none";
		}
	}
	else
	{
		Solution_State.failed_const = false;
        Solution_State.fail_reason == "none";
	}
	int max_phases = 10000;
	if (!sorption())
		max_phases = -1;

	if (Solution_State.failed_const == false)
	{
        Solution_State.fail_reason = "none";
		for (unsigned int k = 0; k < RXN().cons.size(); k++)
		{
			for (int p = -2; p<min(int(Blocks[0].Solid_phase.size()), max_phases); p++)
			{
				int _t;
				if (p < 0) _t = 1; else _t = Blocks[0].Solid_phase[p]->n_phases;
				for (int l = 0; l < _t; l++)
				{
					// LHS
                    for (unsigned int i=0; i < Blocks.size(); i++)
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



CMatrix_arma CMedium::Jacobian_S(CVector_arma &X, const double &dt, bool base = true)
{
	CMatrix_arma M(X.num);

	if (base == true)
	{
		CVector_arma F0 = getres_S(X, dt);
        for (int i=0; i < X.num; i++)
		{
			CVector_arma V = Jacobian_S(X, F0, i, dt);
			for (int j=0; j<X.num; j++)
				M(i,j) = V[j];
		}

	}
	else
	{
        for (int i=0; i < X.num; i++)
		{
			CVector_arma V = Jacobian_S(X, i, dt);
			for (int j = 0; j < X.num; j++)
				M(i, j) = V[j];
		}

	}


	return Transpose(M);
}

CVector_arma CMedium::Jacobian_S(CVector_arma &V, int &i, const double &dt)  //Works also w/o reference (&)
{
	double epsilon;
	if (V[i]<1e-5) epsilon = -1e-6; else epsilon = 1e-6;
	CVector_arma V1(V);
	V1[i] += epsilon;
	CVector_arma F1, F0;
	F1 = getres_S(V1, dt);
	F0 = getres_S(V, dt);

	return (F1 - F0) / epsilon;
}

CVector_arma CMedium::Jacobian_S(CVector_arma &V, CVector_arma &F0, int i, const double &dt)  //Works also w/o reference (&)
{
	double epsilon;
	epsilon = -1e-6;
	CVector_arma V1(V);
	V1[i] += epsilon;
	CVector_arma F1;
	F1 = getres_S(V1, dt);
	CVector_arma grad = (F1 - F0) / epsilon;
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



CMatrix_arma CMedium::Jacobian_C(CVector_arma &X, const double &dt, bool base)
{
	CMatrix_arma M(X.num);

	if (base == true)
	{
		CVector_arma F0 = getres_C(X, dt);
        for (int i=0; i < X.num; i++)
		{
			CVector_arma V = Jacobian_C(X, F0, i, dt);
			for (int j = 0; j < X.num; j++)
				M(i, j) = V[j];
		}
	}
	else
	{
        for (int i=0; i < X.num; i++)
		{
			CVector_arma V = Jacobian_C(X, i, dt);
			for (int j = 0; j < X.num; j++)
				M(i, j) = V[j];
		}
	}

	return Transpose(M);
}

CVector_arma CMedium::Jacobian_C(CVector_arma &V, int i, const double &dt)  //Works also w/o reference (&)
{
	double epsilon;
	if (V[i]<1e-5) epsilon = -1e-6; else epsilon = 1e-6;
	CVector_arma V1(V);
	V1[i] += epsilon;
	CVector_arma F1, F0;
	F1 = getres_C(V1, dt);
	F0 = getres_C(V, dt);

	CVector_arma grad = (F1 - F0) / epsilon;
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

CVector_arma CMedium::Jacobian_C(CVector_arma &V, const CVector_arma &F0, int i, const double &dt)
{
	double epsilon;
	if ((V[i]<1) && (V[i]>-1))
		if (V[i]<1e-5) epsilon = -1e-6; else epsilon = 1e-6;
	else
		epsilon = 1e-6*V[i];
	CVector_arma V1(V);
	V1[i] += epsilon;
	CVector_arma F1;
	F1 = getres_C(V1, dt);

	return (F1 - F0) / epsilon;
}

CMatrix_arma CMedium::Jacobian_Q(CVector_arma &X, double dt, bool base)
{
	CMatrix_arma M(X.num);

	if (base == true)
	{
		CVector_arma F0 = getres_Q(X, dt);
        for (int i=0; i<X.num; i++)
			M.setrow(i, Jacobian_Q(X, F0, i, dt));
	}
	else
	{
        for (int i=0; i<X.num; i++)
			M.setrow(i, Jacobian_Q(X, i, dt));
	}

	return Transpose(M);
}

CVector_arma CMedium::Jacobian_Q(CVector_arma &V, int i, const double &dt)
{
	double epsilon;
	if (V[i]<1e-5) epsilon = -1e-6; else epsilon = 1e-6;
	CVector_arma V1(V);
	V1[i] += epsilon;
	CVector_arma F1, F0;
	F1 = getres_Q(V1, dt);
	F0 = getres_Q(V, dt);

	CVector_arma grad = (F1 - F0) / epsilon;
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

CVector_arma CMedium::Jacobian_Q(CVector_arma &V, const CVector_arma &F0, int i, const double &dt)
{
	double epsilon;
	if ((V[i]<1) && (V[i]>-1))
		if (V[i]<1e-5) epsilon = -1e-6; else epsilon = 1e-6;
	else
		epsilon = 1e-6*V[i];
	CVector_arma V1(V);
	V1[i] += epsilon;
	CVector_arma F1;
	F1 = getres_Q(V1, dt);

	CVector_arma grad = (F1 - F0) / epsilon;
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

void CMedium::set_CG_star(CVector_arma &X)
{
	int max_phases = 10000;
	if (!sorption())
		max_phases = -1;
	for (unsigned int k = 0; k<RXN().cons.size(); k++)
		for (int p = -2; p<min(int(Blocks[0].Solid_phase.size()), max_phases); p++)
		{
			int _t = 1; if (p>-1) _t = Blocks[0].Solid_phase[p]->n_phases;
			for (int l = 0; l<_t; l++)
                for (unsigned int i=0; i<Blocks.size(); i++)  Blocks[i].CG_star[k][Blocks[i].get_member_no(p, l)] = X[get_member_no(i, p, l, k)];
		}

}
void CMedium::set_CG(CVector_arma &X)
{
	int max_phases = 10000;
	if (!sorption())
		max_phases = -1;
	for (unsigned int k = 0; k<RXN().cons.size(); k++)
		for (int p = -2; p<min(int(Blocks[0].Solid_phase.size()), max_phases); p++)
		{
			int _t = 1; if (p>-1) _t = Blocks[0].Solid_phase[p]->n_phases;
			for (int l = 0; l<_t; l++)
                for (unsigned int i=0; i<Blocks.size(); i++)  Blocks[i].CG[k][Blocks[i].get_member_no(p, l)] = X[get_member_no(i, p, l, k)];
		}

}
void CMedium::set_G(CVector_arma &X)
{
	for (int p = 0; p<int(Blocks[0].Solid_phase.size()); p++)
		for (int l = 0; l<Solid_phase()[p].n_phases; l++)
            for (unsigned int i=0; i<Blocks.size(); i++)  Blocks[i].G[p][l] = X[get_member_no(i, p, l)];

}

void CMedium::set_G_star(CVector_arma & X)
{
	for (unsigned int i=0; i<Blocks.size(); i++)
		for (unsigned int p = 0; p<Blocks[i].Solid_phase.size(); p++)
			for (int l = 0; l<Blocks[i].Solid_phase[p]->n_phases; l++)
				Blocks[i].G_star[p][l] = X[get_member_no(i, p, l)];
}

void CMedium::write_flows(string filename)
{
	std::ofstream outfile(filename);
    for (unsigned int i=0; i < Connectors.size(); i++)
        outfile << Connectors[i].ID << ", " << Connectors[i].Q << ", " << Connectors[i].Q_star <<std::endl;

	outfile.close();

}

vector<int> CMedium::infnan_H_blocks()
{
	vector<int> out;
    for (unsigned int i=0; i < Blocks.size(); i++)
		if ((Blocks[i].H_star == Blocks[i].H_star) != true || !isfinite(Blocks[i].H_star))
			out.push_back(i);

	return out;

}

vector<int> CMedium::infnan_H_flows()
{
	vector<int> out;
    for (unsigned int i=0; i < Connectors.size(); i++)
        if ((Connectors[i].Q_star == Connectors[i].Q_star) != true || !isfinite(Connectors[i].Q_star))
			out.push_back(i);

	return out;

}

bool CMedium::AddBlock(const CMBBlock &B )
{
    if (getblocksq(B.ID) != -1)
    {   build_errors.push_back("Block "+ B.ID + " already exists!");
        show_message("Block [" + B.ID + "] already exists!");
        return false;
    }
    else
    {   Blocks.push_back(B);
        Block(B.ID)->parent = this;
        show_message("Block [" + B.ID + "] was added to model [" + name + "]");
        return true;
    }
}

bool CMedium::AddConnector(string source, string destination, const CConnection &C)
{

    if (getblocksq(source) == -1)
    {
        build_errors.push_back("Block "+ source + " does not exist!");
        show_message("Block "+ source + " does not exist!");
        return false;
    }

    if (getblocksq(destination) == -1)
    {
        build_errors.push_back("Block "+ destination + " does not exist!");
        show_message("Block "+ destination + " does not exist!");
        return false;
    }

    if (getconnectorsq(C.ID) !=-1)
    {
        build_errors.push_back("Connector "+ C.ID + " already exists!");
        show_message("Connector "+ C.ID + " already exists!");
        return false;
    }

    Connectors.push_back(C);
    Connector(C.ID)->parent = this;
    Connectors[Connectors.size()-1].Block1ID = source;
    Connectors[Connectors.size()-1].Block2ID = destination;
    if (Connectors[Connectors.size()-1].ID == "")
        Connectors[Connectors.size()-1].ID = source + "-" + destination;

    return true;
}

CMBBlock* CMedium::Block(string B)
{
    if (getblocksq(B)!=-1)
        return &Blocks[getblocksq(B)];
    else
        return NULL;

}

CConnection* CMedium::Connector(string C)
{
    if (getconnectorsq(C)!=-1)
        return &Connectors[getconnectorsq(C)];
    else
        return NULL;
}

bool CMedium::show_messages()
{
    if (showmessages)
        return true;
    else
        return false;
}

void CMedium::show_message(const string &s)
{
#ifndef QT_version
    if (show_messages())
    {
        cout << string("Model [") + name + "]:" + s << std::endl;
    }
#endif
}

void CMedium::show_status(const string &s)
{
#ifndef QT_version
    if (show_messages())
    {
        cout << "\rModel [" + name + "]:" + s << std::flush;
    }
#endif
}


#ifdef USE_VTK
VTK_grid CMedium::VTK_get_snap_shot(string var, double t, double z_scale, string fieldname)
{
    VTK_grid out;
    if (fieldname!="")
        out.names.push_back(fieldname);
    else
        out.names.push_back(var);
    for (unsigned int i=0; i<Blocks.size(); i++)
    {
        bool not_push = false;
        VTK_point pt;
        pt.x = Blocks[i].location.x;
        pt.y = Blocks[i].location.y;
        pt.z = Blocks[i].location.z*z_scale;
        if (var=="s")
            pt.vals.push_back(Results.ANS.BTC[i].interpol(t));
        else if (var=="h")
            pt.vals.push_back(Results.ANS.BTC[i + Blocks.size() + Connectors.size()].interpol(t));
        else if (var=="theta")
        {
            if (Blocks[i].indicator == Block_types::Soil || Blocks[i].indicator == Block_types::Darcy)
                pt.vals.push_back(Results.ANS.BTC[i].interpol(t)/Blocks[i].V);
            else
            {
                pt.vals.push_back(0);
                pt.beshown = false;
            }
        }
        else if (var=="depth")
        {
            if (Blocks[i].indicator != Block_types::Soil && Blocks[i].indicator != Block_types::Darcy)
                pt.vals.push_back(Results.ANS.BTC[i + Blocks.size() + Connectors.size()].interpol(t)-Blocks[i].z0);
            else
            {
                pt.vals.push_back(0);
                pt.beshown = false;
            }
        }
        else
            pt.vals.push_back(Blocks[i].get_val(var));

        out.p.push_back(pt);
    }
    return out;
}


void CMedium::merge_to_snapshot(VTK_grid& grid, string var, double t, string fieldname)
{
    if (fieldname!="") grid.names.push_back(fieldname);
    else if (t!=0)
        grid.names.push_back(var + "_" + numbertostring(t));
    else
        grid.names.push_back(var);
    for (unsigned int i=0; i<Blocks.size(); i++)
    {
        if (var=="s")
            grid.p[i].vals.push_back(Results.ANS.BTC[i].interpol(t));
        else if (var=="h")
            grid.p[i].vals.push_back(Results.ANS.BTC[i + Blocks.size() + Connectors.size()].interpol(t));
        else if (var=="theta")
        {
            if (Blocks[i].indicator == Block_types::Soil || Blocks[i].indicator == Block_types::Darcy)
                grid.p[i].vals.push_back(Results.ANS.BTC[i].interpol(t)/Blocks[i].V);
            else
                grid.p[i].vals.push_back(0);
        }
        else if (var=="depth")
        {
            if (Blocks[i].indicator != Block_types::Soil && Blocks[i].indicator != Block_types::Darcy)
                grid.p[i].vals.push_back(Results.ANS.BTC[i + Blocks.size() + Connectors.size()].interpol(t)-Blocks[i].z0);
            else
                grid.p[i].vals.push_back(0);
        }
        else
            grid.p[i].vals.push_back(Blocks[i].get_val(var));
    }
}

void CMedium::write_grid_to_text(VTK_grid& grid, const string &filename, const vector<string> &names)
{
    ofstream file(filename,std::ofstream::out);
    file << "Block_name, x, y, z";
    for (unsigned int i=0; i<grid.p[0].vals.size(); i++)
    {
        if (names.size())
            file << names[i];
        else if (grid.names.size())
            file << ","<< grid.names[i];
        else
            file << ", var_" + numbertostring(i);
    }
    file << endl;
    for (unsigned int j=0; j<Blocks.size(); j++)
    {
        file << Blocks[j].ID << "," << Blocks[j].location.x << "," << Blocks[j].location.y << "," << Blocks[j].location.z;
        for (unsigned int i=0; i<grid.p[0].vals.size(); i++)
            file << "," << grid.p[j].vals[i];
        file << endl;
    }
    file.close();
}

void CMedium::write_grid_to_vtp(VTK_grid& grid, const string &filename, const vector<string> &names)
{

  // Create the geometry of a point (the coordinate)
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();

  // Create the topology of the point (a vertex)
    //vtkSmartPointer<vtkCellArray> vertices =
    //    vtkSmartPointer<vtkCellArray>::New();

    vector<vtkSmartPointer<vtkDoubleArray>> vals;

    for (unsigned int i=0; i<grid.p[0].vals.size(); i++)
    {
        vals.push_back(vtkSmartPointer<vtkDoubleArray>::New());
        if (names.size())
            vals[i]->SetName(names[i].c_str());
        else if (grid.names.size())
            vals[i]->SetName(grid.names[i].c_str());
        else
            vals[i]->SetName(("var_" + numbertostring(i)).c_str());
    }


    for (unsigned int i=0;i<grid.p.size(); i++)
        {

            if (grid.p[i].beshown)
            {
                const double p[3] = {grid.p[i].x, grid.p[i].y, grid.p[i].z};

                // We need an an array of point id's for InsertNextCell.

                vtkIdType pid[1];
                pid[0] = points->InsertNextPoint(p);
                //vertices->InsertNextCell(1,pid);
                for (unsigned int j=0; j<grid.p[i].vals.size(); j++)
                    vals[j]->InsertNextValue(grid.p[i].vals[j]);
            }
        }
    // Create a polydata object
    vtkSmartPointer<vtkPolyData> point =
    vtkSmartPointer<vtkPolyData>::New();

  // Set the points and vertices we created as the geometry and topology of the polydata
  point->SetPoints(points);
  //point->SetVerts(vertices);

  for (unsigned int j=0; j<grid.p[0].vals.size(); j++)
    point->GetPointData()->AddArray(vals[j]);

    //vtkSmartPointer<vtkCleanPolyData> cleaner =
    //vtkSmartPointer<vtkCleanPolyData>::New();
  //cleaner->SetInputConnection (point);

  // Triangulate the grid points
	vtkSmartPointer<vtkDelaunay3D> delaunay =
		vtkSmartPointer<vtkDelaunay3D>::New();
#if VTK_MAJOR_VERSION <= 5
	delaunay->SetInput(point);
#else
	delaunay->SetInputData(point);
#endif
	delaunay->Update();
	vtkSmartPointer<vtkDataSetMapper> delaunayMapper =
    vtkSmartPointer<vtkDataSetMapper>::New();
  delaunayMapper->SetInputConnection(delaunay->GetOutputPort());

 vtkSmartPointer<vtkActor> delaunayActor =
    vtkSmartPointer<vtkActor>::New();
  delaunayActor->SetMapper(delaunayMapper);
  delaunayActor->GetProperty()->SetColor(1,0,0);

  /*
   vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetWindowName("Point");
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(delaunayActor);
  renderer->SetBackground(colors->GetColor3d("DarkOliveGreen").GetData());

  renderWindow->Render();
  renderWindowInteractor->Start();
  */

  vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
		vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
	writer->SetFileName(filename.c_str());
	writer->SetInputData(delaunayMapper->GetInput());
	// This is set so we can see the data in a text editor.
	writer->SetDataModeToAscii();
	writer->Write();

/*	vtkPolyData* outputPolyData = delaunay->GetOutput();


  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
  mapper->SetInput(outputPolyData);
#else
  mapper->SetInputData(outputPolyData);
#endif

  show_VTK(mapper, filename);
*/
  return;
}


void CMedium::write_grid_to_vtp_surf(VTK_grid& grid, const string &filename, const vector<string> &names)
{

  // Create the geometry of a point (the coordinate)
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();

  // Create the topology of the point (a vertex)
    //vtkSmartPointer<vtkCellArray> vertices =
    //    vtkSmartPointer<vtkCellArray>::New();

    vector<vtkSmartPointer<vtkDoubleArray>> vals;

    for (unsigned int i=0; i<grid.p[0].vals.size(); i++)
    {
        vals.push_back(vtkSmartPointer<vtkDoubleArray>::New());
        if (names.size())
            vals[i]->SetName(names[i].c_str());
        else if (grid.names.size())
            vals[i]->SetName(grid.names[i].c_str());
        else
            vals[i]->SetName(("var_" + numbertostring(i)).c_str());
    }


    for (unsigned int i=0;i<grid.p.size(); i++)
        {

            if (grid.p[i].beshown)
            {
                const double p[3] = {grid.p[i].x, grid.p[i].y, grid.p[i].z};

                // We need an an array of point id's for InsertNextCell.

                vtkIdType pid[1];
                pid[0] = points->InsertNextPoint(p);
                //vertices->InsertNextCell(1,pid);
                for (unsigned int j=0; j<grid.p[i].vals.size(); j++)
                    vals[j]->InsertNextValue(grid.p[i].vals[j]);
            }
        }
    // Create a polydata object
    vtkSmartPointer<vtkPolyData> point =
    vtkSmartPointer<vtkPolyData>::New();

  // Set the points and vertices we created as the geometry and topology of the polydata
  point->SetPoints(points);
  //point->SetVerts(vertices);

  for (unsigned int j=0; j<grid.p[0].vals.size(); j++)
    point->GetPointData()->AddArray(vals[j]);

    //vtkSmartPointer<vtkCleanPolyData> cleaner =
    //vtkSmartPointer<vtkCleanPolyData>::New();
  //cleaner->SetInputConnection (point);

  // Triangulate the grid points
	vtkSmartPointer<vtkDelaunay2D> delaunay =
		vtkSmartPointer<vtkDelaunay2D>::New();
#if VTK_MAJOR_VERSION <= 5
	delaunay->SetInput(point);
#else
	delaunay->SetInputData(point);
#endif
	delaunay->Update();
	vtkSmartPointer<vtkPolyDataMapper> delaunayMapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  delaunayMapper->SetInputConnection(delaunay->GetOutputPort());

 vtkSmartPointer<vtkActor> delaunayActor =
    vtkSmartPointer<vtkActor>::New();
  delaunayActor->SetMapper(delaunayMapper);
  delaunayActor->GetProperty()->SetColor(1,0,0);

  /*
   vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetWindowName("Point");
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(delaunayActor);
  renderer->SetBackground(colors->GetColor3d("DarkOliveGreen").GetData());

  renderWindow->Render();
  renderWindowInteractor->Start();
  */

  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
		vtkSmartPointer<vtkXMLPolyDataWriter>::New();
	writer->SetFileName(filename.c_str());
	writer->SetInputData(delaunayMapper->GetInput());
	// This is set so we can see the data in a text editor.
	writer->SetDataModeToAscii();
	writer->Write();

/*	vtkPolyData* outputPolyData = delaunay->GetOutput();


  // Visualize
  vtkSmartPointer<vtkPolyDataMapper> mapper =
    vtkSmartPointer<vtkPolyDataMapper>::New();
#if VTK_MAJOR_VERSION <= 5
  mapper->SetInput(outputPolyData);
#else
  mapper->SetInputData(outputPolyData);
#endif

  show_VTK(mapper, filename);
*/
  return;
}


void CMedium::show_VTK(vtkSmartPointer<vtkPolyDataMapper> mapper, const string &filename)
{
  vtkSmartPointer<vtkActor> actor =
  vtkSmartPointer<vtkActor>::New();
  vtkSmartPointer<vtkNamedColors> colors =
  vtkSmartPointer<vtkNamedColors>::New();

  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
  actor->GetProperty()->SetPointSize(20);

  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetWindowName("Point");
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(actor);
  renderer->SetBackground(colors->GetColor3d("DarkOliveGreen").GetData());

  renderWindow->Render();
  renderWindowInteractor->Start();

  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
		vtkSmartPointer<vtkXMLPolyDataWriter>::New();
	writer->SetFileName(filename.c_str());
	writer->SetInputData(mapper->GetInput());
	// This is set so we can see the data in a text editor.
	writer->SetDataModeToAscii();
	writer->Write();

	return;
}


#endif // USE_VTK

#endif
