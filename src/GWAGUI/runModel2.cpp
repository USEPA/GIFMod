#include "Medium.h"
#include "BTC.h"
#include "gwidget.h"
#include "entity.h"
#include "process.h"
#include "edge.h"
#include "mainwindow.h"
#include "runtimeWindow.h"

#ifdef WQV
CMedium::CMedium(GraphWidget* gw, runtimeWindow *progress)
{
	//progress->setValue(0);
	this->gw = gw;
	set_default();
	this->runtimewindow = progress;
	//progress->setValue(2);
	
	//progress->setValue(5);
	g_get_environmental_params(); // load environmental parameters
	//progress->setValue(8);
	g_get_params(); //load charachterictics of unknown parameters *
	//progress->setValue(10);

	g_get_observed(); // load properties of observed data *
	//progress->setValue(13);

	g_get_model_configuration(progress); // load model structure (blocks, connectors)
	//progress->setValue(30);

	g_get_particle_types(); // load particle types *
	//progress->setValue(31);
	g_get_constituents(); // load constituent properties *
	//progress->setValue(32);
	g_set_default_connector_expressions();
	g_set_default_block_expressions();
	//progress->setValue(33);
	g_get_reactions(); // load reaction network
	g_get_buildup(); // load buildup properties *
	//progress->setValue(34);

	g_get_external_flux(); // load external fluxes *
	//progress->setValue(35);


	log_file_name = "log.txt";

//*************	for (int j = 0; j<Precipitation_filename.size(); j++)
//		Precipitation.push_back(CPrecipitation(pathname + Precipitation_filename[j]));
//	for (int j = 0; j<Evaporation_filename.size(); j++)
//		Evaporation.push_back(CBTC(pathname + Evaporation_filename[j]));

/*	for (int i = 0; i<Blocks.size(); i++)
	{
		for (int j = 0; j < Blocks[i].inflow_filename.size(); j++)
			Blocks[i].inflow.push_back(CBTCSet(Blocks[i].inflow_filename[j], 1));


		if ((Blocks[i].indicator == Pond) || (Blocks[i].indicator == Catchment) || (Blocks[i].indicator == Stream))
		{
			if (Blocks[i].precipitation_swch == true)
				for (int j = 0; j<Precipitation_filename.size(); j++)
					Blocks[i].inflow.push_back(Precipitation[j].getflow(Blocks[i].A));
		}
	}
	progress->setValue(37);

*/	
	g_load_inflows();

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
			P.low = exp(-4); P.high = exp(4);
			parameters.push_back(P);

		}
	}
	std.resize(stds.size());
	get_funcs();

	

	set_default_params();

	//progress->setValue(30);

}
#endif
#ifdef WQV
void MainWindow::forwardRun (CMedium *model, runtimeWindow* progress)

{
	if ((model->parameters.size() == 0) || (model->forward == true))
	{
		clock_t t0, t1;
 		t0 = clock();
		ofstream runtime_file;
		runtime_file.open(model->pathname + "Runing_times.txt");
		model->solve(progress);
		t1 = clock() - t0;
		float run_time = ((float)t1) / CLOCKS_PER_SEC;
		string st = " sec";
		if (run_time >= 60) { run_time /= 60; st = " min"; }
		if (run_time >= 60) { run_time /= 60; st = " hr"; }
		runtime_file << "Run Time :  " << run_time << st << endl;
		runtime_file << "Epoch count: " << model->epoch_count << endl;

		model->ANS.writetofile(model->pathname + model->detoutfilename_hydro[0]);
		model->ANS_obs.writetofile(model->pathname + model->detoutfilename_obs[0]);
		model->ANS_colloids.writetofile(model->pathname + model->detoutfilename_prtcle[0]);
		model->ANS_constituents.writetofile(model->pathname + model->detoutfilename_wq[0]);
		if (model->mass_balance_check == true) model->ANS_MB.writetofile(model->pathname + "output_MB.txt");
		model->Solution_dt.writetofile(model->pathname + "dt.txt");
		runtime_file.close();
		
	}
}

void CMedium::g_get_params()
{
	for each (Entity *e in gw->entitiesByType("Parameter"))
	{
		range P;
		P.low = 0;
		P.high = 0;
		P.fixed = false;
		P.log = false;
		P.applytoall = true;
		P.tempcorr = 1;
		P.name = e->Name().toStdString();
		P.low = e->val("low").toFloat();  //low range
		P.high = e->val("high").toFloat();
		P.applytoall = (e->val("applytoall") == "Yes") ? 1 : 0;
		P.log = (e->val("log") == "Normal") ? 0 : 1;
		parameters.push_back(P);
		if (P.log == 0)
			params_vals.push_back(0.5*(P.low + P.high));
		else
			params_vals.push_back(P.low / fabs(P.low)*sqrt(P.low*P.high));
	}
}

void CMedium::g_get_observed()
{
	for each (Entity *e in gw->entitiesByType("Observed"))
	{
		measured_chrc M;
		M.error_structure = 0;
		M.name = e->Name().toStdString();
		M.id = split(e->val("id").toStdString(), '+');  //location id //BLOCK/CONNECTOR's NAME
		M.loc_type = (e->val("loc_type") == "Block") ? 0 : 1; //OBSERVED SUBTYPE
		M.quan = e->val("quan").toStdString();
		M.std_no = e->val("std_no").toInt();
		M.error_structure = (e->val("error_structure") == "Normal") ? 0 : 1; //NORM 0 ; LOg 1
		measured_quan.push_back(M);
	}
//*****************************************
//	if (tolower(lid_config.keyword[i]) == "observed_data")
//	measured_data.push_back(CBTCSet("OBSERVED.TXT", 1)); //*****TEMPORARY // pathname + e->props.list[key], 1)); 
}

void CMedium::g_get_environmental_params()
{
	pos_def_limit = true;
	QList<Entity*> list;
	list.append(gw->entitiesByType("Global Settings"));
	list.append(gw->entitiesByType("Solver"));

	for each (Entity *e in list)
		for each (QString key in e->codes())
		{
			if (key == "path") 
				pathname = e->val(key).toStdString() + '/';
			if (key == "forward")
				forward = (e->val(key) == "Forward") ? 1 : 0;
			if (key == "solution_method") 
				solution_method = e->val(key).toInt();
			if (key == "wiggle_tolerance") 
				wiggle_tolerance = e->val(key).toFloat();
			if (key == "max_j_update_interval")
				max_J_interval = e->val(key).toInt();
			if (key == "dt") 
				dt = e->val(key).toFloat();
			if (key == "w") 
				w = e->val(key).toFloat();
			if (key == "tol") 
				tol = e->val(key).toFloat();
			if (key == "time_min") 
			{ Timemin = e->val(key).toFloat(); t = Timemin; }
			if (key == "time_max") 
				Timemax = e->val(key).toFloat();
			if (key == "uniformoutput") 
				uniformoutput = e->val(key).toInt();
			if (key == "nr_iteration_treshold_max") 
				nr_iteration_treshold_max = e->val(key).toFloat();
			if (key == "nr_iteration_treshold_min") 
				nr_iteration_treshold_min = e->val(key).toFloat();
			if (key == "dt_change_rate") 
				dt_change_rate = e->val(key).toFloat();
			if (key == "dt_change_failure") 
				dt_change_failure = e->val(key).toFloat();
			if (key == "nr_failure_criteria")
				nr_failure_criteria = e->val(key).toInt();
			if (key == "write_details")
				write_details = (e->val(key) == "Yes") ? 1 : 0; //ask Arash Default,Yes,No?
			if (key == "mass_balance_check")
				mass_balance_check = (e->val(key) == "Yes") ? 1 : 0;
//				if(e->val(key).toQString() == "Yes")
	//				mass_balance_check = true;
		//		else 
			//		mass_balance_check = false;
			if (key == "colloid_transport") 
				colloid_transport = (e->val(key) == "Yes") ? 1 : 0;
			if (key == "water_quality") 
				constituent_transport = (e->val(key) == "Yes") ? 1 : 0;
			if (key == "epoch_limit") 
				epoch_limit = e->val(key).toInt();
			if (key == "avg_dt_limit")
				avg_dt_limit = e->val(key).toFloat();
			if (key == "restore_interval") 
				restore_interval = e->val(key).toInt();
			if (key == "log_file")
				log_file_name = pathname + "log.txt"; // pathname + e->val(key).toStdString();
			if (key == "n_timeseries") 
				n_time_series = e->val(key).toInt();
			if (key == "pe_info_filename") 
				PE_info_filename = e->val(key).toStdString();
			if (key == "writeinterval") 
				writeinterval = e->val(key).toInt();
			if (key == "precipitation") 
				if (e->val(key)!=".") Precipitation_filename.push_back(e->val(key).toStdString());
			if (key == "evaporation") 
				if (e->val(key) != ".") Evaporation_filename.push_back(e->val(key).toStdString());
			if (key == "pos_def_limit") 
				pos_def_limit = e->val(key).toInt();
			if (key == "negative_concentration_allowed")
				negative_concentration_allowed = e->val(key).toInt();
		}

	if (detoutfilename_hydro.size() == 0) detoutfilename_hydro.push_back("hydro_output.txt");
	if (detoutfilename_wq.size() == 0) detoutfilename_wq.push_back("wq_output.txt");
	if (detoutfilename_prtcle.size() == 0) detoutfilename_prtcle.push_back("prtcl_output.txt");
	if (detoutfilename_obs.size() == 0) detoutfilename_obs.push_back("observed_output.txt");
//	detout_filename = det_out.txt
//		detout_obs_filename = obs_det_out.txt;
	PE_info_filename = "GA_info.txt";
}

void CMedium::g_get_model_configuration(runtimeWindow* progress)
{
	for each(Node *n in gw->Nodes())
	{
		CMBBlock B;
		if (n->objectType.ObjectType == "Soil") { B.indicator = Block_types::Soil; } // 0
		if (n->objectType.ObjectType == "Pond") { B.indicator = Block_types::Pond; } //1
		if (n->objectType.ObjectType == "Storage") { B.indicator = Block_types::Storage; } //2
		if (n->objectType.ObjectType == "Catchment") { B.indicator = Block_types::Catchment; } //3
		if (n->objectType.ObjectType == "Manhole") { B.indicator = Block_types::Manhole; } //4
		if (n->objectType.ObjectType == "Darcy") { B.indicator = Block_types::Darcy; } //5
		if (n->objectType.ObjectType == "Stream") { B.indicator = Block_types::Stream; } //6
		B.set_val("a", n->val("a").toFloat());
		B.set_val("v", n->val("v").toFloat());
		
		if (B.V == 0) //&& (lookup(lid_config.param_names[i], "depth") != -1))
		{
			B.set_val("v", B.A*n->val("depth").toFloat());
		}

		for each (QString code in n->codes())
		{
//			qDebug() << code;
			B.set_val(code.toStdString(), n->val(code).toFloat());
			if (code == "inflow" && !n->val(code).isEmpty() && n->val(code) != ".")
				B.inflow_filename.push_back(fullFilename(n->val(code),gw->modelPathname()).toStdString());
			if (code == "id") B.ID = n->val(code).toStdString();
			if (code == "buildup" && !n->val(code).isEmpty()) B.buildup_id.push_back(n->val(code).toStdString());
			if (code == "externalflux" && !n->val(code).isEmpty()) B.envexchange_id.push_back(n->val(code).toStdString());
			if (code == "precipitation")
				B.precipitation_swch = (n->val(code) == "Yes") ? 1 : 0;
			if (code == "h_s_expression" && !n->val(code).isEmpty()) B.H_S_expression = n->val(code).toStdString();

		}
		for each (QString text in n->g().split(';'))
			B.set_val(text.split('=').first().toStdString(), text.split('=').last().toFloat());
		for each (QString text in n->cg().split(';'))
			B.set_val(text.split('=').first().toStdString(), text.split('=').last().toFloat());


		Blocks.push_back(B);

		for each (QString code in n->codes())
		{
			if (gw->EntityNames("Parameter").contains(n->val(code)))
				{
					if (lookup_parameters(n->val(code).toStdString()) != -1) {
						parameters[lookup_parameters(n->val(code).toStdString())].location.push_back(Blocks.size() - 1);  // Check for everything
						parameters[lookup_parameters(n->val(code).toStdString())].quan.push_back(code.toStdString());
						parameters[lookup_parameters(n->val(code).toStdString())].location_type.push_back(0);
					}
				}
		}
		//progress->setValue(progress->value() + 1);
	}

	for each(Edge *e in gw->Edges())
	{
		CConnection C;
		C.flow_params.resize(20);
		C.ID = e->Name().toStdString();

		QString type = e->objectType.ObjectType;//connector type 

		for each (QString code in e->codes())
			if (!e->val(code).isEmpty()) C.set_val(code.toStdString(), e->val(code).toFloat());  //sets all values except theta (theta was removed in input file)
		if (!e->val("flow_expression").isEmpty())
			C.flow_expression = CStringOP(e->val("flow_expression").toStdString());
		else
		{
			if (type == "Darcy") C.flow_expression = formulas[QDarcy];
			if (type == "Pipe") C.flow_expression = CStringOP(formulas[Pipe2]);
			if (type == "Normal Flow") C.flow_expression = CStringOP(formulas[Normal]);
			if (type == "Rating Curve") C.flow_expression = CStringOP(formulas[Rating_curve]);
		}
		if (!e->val("area_expression").isEmpty()) { C.area_expression = CStringOP(e->val("area_expression").toStdString()); C.const_area = false; }
		if (!e->val("dispersion_expression").isEmpty()) C.dispersion_expression = CStringOP(e->val("dispersion_expression").toStdString());
		C.Block1ID = e->sourceNode()->Name().toStdString();
		C.Block2ID = e->destNode()->Name().toStdString();
	
		if (e->val("settling") == "Yes") C.settling = 1;
		if (e->val("settling") == "No") C.settling = 0;
		if (e->val("a").toFloat() != 0) { C.const_area = true; }

		Connector.push_back(C);
		//progress->setValue(progress->value() + 1);

		for each (QString code in e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code)))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(Connector.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(1);
				}
			}
		}
		//progress->setValue(progress->value() + 1);
	}
}

void CMedium::g_get_particle_types()
{
	for each (Entity *e in gw->entitiesByType("Particle"))
	{
		QString model = e->props.list["Model"];
		if (model == "Single Phase") model = "single_phase";
		if (model == "Dual Phase") model = "dual_phase_lm";
		if (model == "Triple Phase") model = "triple_phase_lm";

		CSolid_Phase S(model.toStdString());
		S.name = e->Name().toStdString();
		for each (QString key in e->props.list.keys())
		{
			S.set_val(key.toStdString(), e->props.list[key].toFloat());
		}

		Solid_phase.push_back(S);

		for each (QString code in e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code)))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(Solid_phase.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(7);
				}
			}
		}
	}
}

void CMedium::g_get_buildup()
{
	for each (Entity *e in gw->entitiesByType("Build-Up"))
	{
		QString model = e->props.list["Model"].toLower();

		CBuildup S(model.toStdString());
		S.name = e->Name().toStdString();
		if (gw->entityByName(e->val("constituent"))->objectType.ObjectType == "Constituent"){
			S.constituent = e->val("constituent").toStdString();
			if (e->val("sorb") == "No") S.phase = "aqueous";
			else S.phase = "sorbed";
		}
		else {
			S.solid = e->val("constituent").toStdString();
			if (e->val("sorb") == "No") S.phase = "mobile";
			else S.phase = "attached";
		}

		for each (QString code in e->codes())
		{
			S.set_val(code.toStdString(), e->val(code).toFloat());
		}

		buildup.push_back(S);

		for each (QString code in e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code)))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(buildup.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(5);
				}
			}
		}
	}
}

void CMedium::g_get_external_flux()
{
	for each (Entity *e in gw->entitiesByType("External FLux"))
	{
		QString model = e->props.list["Model"].toLower();
		if (model == "free surface") model = "free_surface";

		CEnvExchange S(model.toStdString());
		S.name = e->Name().toStdString();

		S.constituent = e->val("constituent").toStdString();
		S.solid = e->val("solid").toStdString();
		
		S.expression = e->val("expression").toStdString();

		for each (QString code in e->codes())
		{
			S.set_val(code.toStdString(), e->val(code).toFloat());
		}

		externalflux.push_back(S);

		for each (QString code in e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code)))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(externalflux.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(6);
				}
			}
		}
	}
}


void CMedium::g_get_constituents()
{
	for each (Entity *e in gw->entitiesByType("Constituent"))
	{
		CConstituent S;
		S.name = e->Name().toStdString();
		S.exchange_rate_scale_factor = CStringOP(e->val("exchange_rate_factor").toStdString());
		S.diffusion = e->val("diffusion").toFloat();
		for each (QString s in e->val("particle name").split(';'))
		{
			S.capacity_ptr.push_back(s.toStdString());
			S.rate_exchange_ptr.push_back(s.toStdString());
		}
		for each (QString s in e->val("partition coefficient").split(';'))
			S.capacity.push_back(s.toFloat());
		for each (QString s in e->val("exchamge rate").split(';'))
			S.rate_exchange.push_back(s.toFloat());
		RXN.cons.push_back(S);
	
		for each (QString code in e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code)))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(RXN.cons.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(4);
				}
			}
		}
	}
}

void CMedium::g_get_reactions()
{
	for each (Entity *e in gw->entitiesByType("Reaction Parameter"))
	{
		rxparam rxparameter;
		rxparameter.tempcorr = 1;
		rxparameter.name = e->Name().toStdString();
		rxparameter.value = e->val("value").toFloat();
		rxparameter.tempcorr = e->val("temperature_correction").toFloat();
		RXN.parameters.push_back(rxparameter);

		for each (QString code in e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code)))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(RXN.parameters.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(3);
				}
			}
		}
	}

	for each (Process *e in gw->Processes)
	{
		CReaction Rx;
		Rx.name = e->name.toStdString(); //NAME
		Rx.rate = CStringOP(e->rateExpression().toStdString(), &RXN);
		for each (QString key in e->productExpressions().keys())
		{
			Rx.products.push_back(RXN.look_up_constituent_no(key.toStdString()));
			Rx.prodrates.push_back(CStringOP(e->productExpressions()[key].split('=').last().toStdString(), &RXN));
			Rx.product_p_type.push_back(-2);
			Rx.product_phase.push_back(0);
		}
// WHAT IS THIS
/*				if (split(lid_config.param_names[i][j], del2).size()>2)
					Rx.product_p_type.push_back(atoi(split(lid_config.param_names[i][j], del2)[2].c_str()));
				else
					Rx.product_p_type.push_back(-2);

				if (split(lid_config.param_names[i][j], del2).size() > 3)
					Rx.product_phase.push_back(atoi(split(lid_config.param_names[i][j], del2)[3].c_str()));
				else
					Rx.product_phase.push_back(0);
					*/
		RXN.Rxts.push_back(Rx);
	}

}




void CMedium::g_set_default_connector_expressions()
{

	for (int ii = 0; ii<Connector.size(); ii++)
	{
		if (Connector[ii].flow_expression.terms.size() == 0)
		{
			if (Blocks[getblocksq(Connector[ii].Block1ID)].z0 >= Blocks[getblocksq(Connector[ii].Block2ID)].z0)
			{
				if (vaporTransport[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator] == true)
				{
					Connector[ii].flow_expression = CStringOP(formulasQ[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator] + "+" + formulas[Vapor]);
					Connector[ii].flow_expression_strng = formulasQ[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator] + "+" + formulas[Vapor];

				}
				else
				{
					Connector[ii].flow_expression = CStringOP(formulasQ[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator]);
					Connector[ii].flow_expression_strng = formulasQ[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];

				}
			}
			else
			{
				if (vaporTransport[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator] == true)
				{
					Connector[ii].flow_expression = CStringOP(formulasQ2[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator] + "+" + formulas[Vapor]);
					Connector[ii].flow_expression_strng = formulasQ2[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator] + "+" + formulas[Vapor];
				}
				else
				{
					Connector[ii].flow_expression = CStringOP(formulasQ2[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator]);
					Connector[ii].flow_expression_strng = formulasQ2[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
				}
			}
			if (vaporTransport[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator])
			{
				Connector[ii].flow_expression_v = CStringOP(formulas[Vapor]);
				Connector[ii].flow_expression_strng_v = formulas[Vapor];
			}
		}
	}

	for (int ii = 0; ii < Connector.size(); ii++)
	{
		Connector[ii].area_expression = formulasA[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
		Connector[ii].area_expression_strng = formulasA[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
		if (Connector[ii].A == 0)
			Connector[ii].const_area = const_area[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
		else
			Connector[ii].const_area = true;
	}

	for (int ii = 0; ii < Connector.size(); ii++)
	{
		if (Connector[ii].settling == -1)
		{
			Connector[ii].settling = settling[Blocks[getblocksq(Connector[ii].Block1ID)].indicator][Blocks[getblocksq(Connector[ii].Block2ID)].indicator];
		}
	}


	for (int i = 0; i<Connector.size(); i++)
	{
		for (int j = 0; j< Solid_phase.size(); j++)Connector[i].Solid_phase_id.push_back(j);
		Connector[i].Block1 = &Blocks[getblocksq(Connector[i].Block1ID)];
		Connector[i].Block2 = &Blocks[getblocksq(Connector[i].Block2ID)];
		Blocks[getblocksq(Connector[i].Block1ID)].connectors.push_back(i);
		Blocks[getblocksq(Connector[i].Block1ID)].connectors_se.push_back(0);
		Blocks[getblocksq(Connector[i].Block2ID)].connectors.push_back(i);
		Blocks[getblocksq(Connector[i].Block2ID)].connectors_se.push_back(1);
		for (int ii = 0; ii<Connector[i].Solid_phase_id.size(); ii++) Connector[i].Solid_phase.push_back(&(Solid_phase[Connector[i].Solid_phase_id[ii]]));

	}
}

void CMedium::g_set_default_block_expressions()
{
	for (int ii = 0; ii<Blocks.size(); ii++)
	{
		if (Blocks[ii].H_S_expression.terms.size() == 0)
		{
			Blocks[ii].H_S_expression = CStringOP(formulasH[Blocks[getblocksq(Blocks[ii].ID)].indicator]); Blocks[ii].H_S_expression_txt = formulasH[Blocks[getblocksq(Blocks[ii].ID)].indicator];
		}
	}
}

bool CMedium::solve(runtimeWindow *progress)
{
	evaluate_functions();
	//progress->setValue(35);
	setH();	// Set Total Head for all blocks
	evaluate_area(true);  //Update connector areas; 
	//progress->setValue(40);
	setQ();	//Set Q for all connectors
	initialize();
	//progress->setValue(60);


	ANS_obs = CBTCSet(measured_quan.size());
	//progress->setValue(70);

	ANS_obs.names.clear();
	for (int i = 0; i<measured_quan.size(); i++) ANS_obs.names.push_back(measured_quan[i].name);
	//progress->setValue(80);

	if (solution_method == 0)
		solve_fts_m2(dt);   //fts= fixed time steps

	return true;
}
void CMedium::g_load_inflows()
{
	for (int j = 0; j < Precipitation_filename.size(); j++)
	{
		CPrecipitation P = CPrecipitation(Precipitation_filename[j]);
		if (P.n>0) Precipitation.push_back(P);
	}
	for (int j = 0; j < Evaporation_filename.size(); j++)
	{
		CBTC E = CBTC(Evaporation_filename[j]);
		if (E.n>0) Evaporation.push_back(E);
	}

	for (int i = 0; i<Blocks.size(); i++)
	{
		for (int j = 0; j < Blocks[i].inflow_filename.size(); j++)
		{
			CBTCSet inflow = CBTCSet(Blocks[i].inflow_filename[j], 1);
			if (inflow.file_not_found)
				warning(QString("Inflow file:%1 is not valid.").arg(QString::fromStdString(Blocks[i].inflow_filename[j])));
			else
				Blocks[i].inflow.push_back(inflow);
		}

		if ((Blocks[i].indicator == Pond) || (Blocks[i].indicator == Catchment) || (Blocks[i].indicator == Stream))
		{
			if (Blocks[i].precipitation_swch == true)
				for (int j = 0; j<Precipitation_filename.size(); j++)
					Blocks[i].inflow.push_back(Precipitation[j].getflow(Blocks[i].A));
		}
	}

	f_make_uniform_inflows();
}
int CMedium::get_member_no(QString block_name, QString solid_name, QString phase_name)
{
	int solid_id;
	if (solid_name == "") solid_id = -2;
	else if (solid_name == "Soil") solid_id = -1;
	else solid_id = lookup_particle_type(solid_name.toStdString());

	int phase_no;
	if (phase_name == "Mobile" || phase_name == "") phase_no = 0;
	else if (phase_name == "Attached" || phase_name == "Reversible attached") phase_no = 1;
	else if (phase_name == "Irreversible attached") phase_no = 2;
	else gw->log(QString("Warning: Could not locate Phase(%1) for Particle(%2) in Block(%3).").arg(phase_name).arg(solid_name).arg(block_name));

	int k = 0;
	for (int i = 0; i<solid_id; i++) k += Blocks[0].Solid_phase[i]->n_phases*Blocks.size();
	return k + getblocksq(block_name.toStdString()) + phase_no*Blocks.size();
}

int CMedium::get_member_no(QString block_name, QString solid_name, QString phase_name, QString const_name)
{
	int block_no = getblocksq(block_name.toStdString());
	int solid_id;
	int const_no;
	int phase_no;
	if (solid_name == "") solid_id = -2;
	else if (solid_name == "Soil") solid_id = -1;
	else solid_id = lookup_particle_type(solid_name.toStdString());
	
	const_no = RXN.look_up_constituent_no(const_name.toStdString());
	
	if (phase_name == "Mobile" || phase_name == "") phase_no = 0;
	else if (phase_name == "Attached" || phase_name == "Reversible attached") phase_no = 1;
	else if (phase_name == "Irreversible attached") phase_no = 2;
	else gw->log(QString("Warning: Could not locate Phase(%1) for Particle(%2) in Block(%3).").arg(phase_name).arg(solid_name).arg(block_name));

	return get_member_no(block_no, solid_id, phase_no, const_no);
}
void CMedium::warning(QString text)
{ 
	gw->warning(text); 
}
void CMedium::updateProgress(bool finished)
{
	// t, dtt (graph), epoch_count
	if (runtimewindow != 0)
	{
		QMap<QString, QVariant> vars;
		if (finished)
		{
			vars["progress"] = 100;
			vars["finished"] = true;
		}
		else
		{
			int progress;
			progress = 100.0*(t - Timemin) / (Timemax - Timemin);
			vars["t"] = t;
			vars["progress"] = progress;
			vars["dtt"] = dtt;
			vars["epoch count"] = epoch_count;
		}
		runtimewindow->update(vars);

	}
}
#endif
QList <QMap <QVariant, QVariant>> CBTC::compact() const
{
	return QList <QMap <QVariant, QVariant>>();
}
CBTC::CBTC(QList <QMap <QVariant, QVariant>> data)
{

}
