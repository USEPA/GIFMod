#include "BTC.h"
#include "GWidget.h"
#include "entity.h"
#include "process.h"
#include "edge.h"
#include "mainwindow.h"
#include "runtimeWindow.h"
#include "GA.h"
#include "MCMC.h"
#include "results.h"
#include <QMessageBox>
#include <omp.h>
#include "Vector.h"
#include "reactiontablemodel.h"
#ifdef GIFMOD
#include "MediumSet.h"
#include "Medium.h"

using namespace std; 

void CMediumSet::load(GraphWidget* gw, runtimeWindow *rtw)
{
	this->gw = gw;
	QString savedExperiment = gw->experiments->currentText();
	gw->experiments->setCurrentText("All experiments");
	set_default();
	g_get_environmental_params();
	g_get_params();
	g_get_observed();
	g_get_particle_types();
	g_get_constituents();
	g_get_sensors();
	g_get_controllers();
	g_get_reactions();
	g_get_buildup();
	g_get_external_flux();
	g_get_evapotranspiration();


	for (int i = 0; i<parameters.size(); i++)
		set_param(i, parameters[i].value);


	Medium.resize(gw->experiments->count() - 1);
	for (int i = 1; i < gw->experiments->count(); i++)
	{
		gw->experiments->setCurrentIndex(i);
		Medium[i - 1].parent = this;
		Medium[i - 1].name = gw->experimentName().toStdString();
		Medium[i - 1].gw = gw;
		Medium[i - 1].Blocks.clear();
        Medium[i - 1].Connectors.clear();
		Medium[i - 1].g_get_environmental_params();
		Medium[i - 1].g_get_model_configuration(rtw); // load model structure (blocks, connectors)
		Medium[i - 1].g_set_default_connector_expressions();
		Medium[i - 1].g_set_default_block_expressions();
		Medium[i - 1].g_load_inflows();
		Medium[i - 1].get_funcs();
		//		Medium[i - 1].set_default_params();
		Medium[i - 1].log_file_name() = "log.txt";
	}
	for (int i = 0; i < parameters.size(); i++)
	{
		set_param(i, parameters[i].value);
	}
	for (int i = 1; i < gw->experiments->count(); i++)
	{
		Medium[i - 1].set_default_params();
	}

	gw->experiments->setCurrentText(savedExperiment);

}

CMediumSet::CMediumSet(GraphWidget* gw, runtimeWindow *rtw)
{
//	omp_set_num_threads(16);	//progress->setValue(0);
	this->gw = gw;
	QString savedExperiment = gw->experiments->currentText();
	gw->experiments->setCurrentText("All experiments");
	set_default();
	g_get_environmental_params();
	g_get_params();
	g_get_observed();
	g_get_particle_types();
	g_get_constituents();
	g_get_sensors();
	g_get_controllers();
	g_get_reactions();
	g_get_buildup();
	g_get_external_flux();
	g_get_evapotranspiration();
	

	for (int i = 0; i<parameters.size(); i++)
		set_param(i, parameters[i].value);

	
	Medium.resize(gw->experiments->count() - 1);
	for (int i = 1; i < gw->experiments->count(); i++)
	{
		gw->experiments->setCurrentIndex(i);
		Medium[i - 1].parent = this;
		Medium[i - 1].name = gw->experimentName().toStdString();
		Medium[i - 1].gw = gw;
		Medium[i - 1].Blocks.clear();
        Medium[i - 1].Connectors.clear();
		Medium[i - 1].g_get_environmental_params();
		Medium[i - 1].g_get_model_configuration(rtw); // load model structure (blocks, connectors)
		Medium[i - 1].g_set_default_connector_expressions();
		Medium[i - 1].g_set_default_block_expressions();
		Medium[i - 1].g_load_inflows();
		Medium[i - 1].get_funcs();
//		Medium[i - 1].set_default_params();
		Medium[i - 1].log_file_name() = "log.txt";
	}
	for (int i = 0; i < parameters.size(); i++)
	{
		set_param(i, parameters[i].value);
	}
	for (int i = 1; i < gw->experiments->count(); i++)
	{
		Medium[i - 1].set_default_params();
	}

	gw->experiments->setCurrentText(savedExperiment);

}
void MainWindow::forwardRun(CMediumSet *model, runtimeWindow* progress)
{
	//progress->setValue(45);
	//	if ((model->parameters().size() == 0) || (model->forward()))
	//	{
	clock_t t0, t1;
	t0 = clock();
	ofstream runtime_file;
	string a = model->FI.pathname;
	runtime_file.open(model->FI.outputpathname + "Runing_times.txt");
	model->solve(progress);
#ifdef GIFMOD
	//progress->setLabel(QString::fromStdString(model->fail_reason));
#endif
	t1 = clock() - t0;
	float run_time = ((float)t1) / CLOCKS_PER_SEC;
	string st = " sec";
	if (run_time >= 60) { run_time /= 60; st = " min"; }
	if (run_time >= 60) { run_time /= 60; st = " hr"; }
	runtime_file << "Run Time :  " << run_time << st << std::endl;
	runtime_file << "Epoch count: " << model->epoch_count() << std::endl;

	for (int i = 0; i < model->Medium.size(); i++)
	{
        model->Medium[i].Results.ANS.writetofile(model->Medium[i].detoutfilename_hydro, true);
        model->Medium[i].Results.ANS_colloids.writetofile(model->Medium[i].detoutfilename_prtcle, true);
        model->Medium[i].Results.ANS_constituents.writetofile(model->Medium[i].detoutfilename_wq, true);
        model->Medium[i].Results.ANS_control.writetofile(model->Medium[i].detoutfilename_control, true);
        if (model->SP.mass_balance_check) model->Medium[i].Results.ANS_MB.writetofile(model->FI.outputpathname + "output_MB" + model->Medium[i].name + ".txt", true);
		/*			model->Medium[i].ANS.writetofile(model->FI.outputpathname + model->Medium[i].detoutfilename_hydro);
					model->Medium[i].ANS_colloids.writetofile(model->FI.outputpathname + model->Medium[i].detoutfilename_prtcle);
					model->Medium[i].ANS_constituents.writetofile(model->FI.outputpathname + model->Medium[i].detoutfilename_wq);
					if (model->SP.mass_balance_check) model->Medium[i].ANS_MB.writetofile(model->FI.outputpathname + "output_MB" + model->Medium[i].name + ".txt");
					*/
	}

	model->ANS_obs.writetofile(model->FI.detoutfilename_obs, true);
	
	//system.Solution_dt.writetofile(system.outputpathname()+"dt.txt");
	runtime_file.close();

//	mainGraphWidget->results->projected = system->projected;
//	model->ANS_obs = model->modeled;
//	mainGraphWidget->results->ANS_obs = model->ANS_obs;
	//mainGraphWidget->model = &model->Medium[0];
	mainGraphWidget->experimentSelect(mainGraphWidget->experimentsList()[0]);
	mainGraphWidget->hasResults = true;

}
#endif
#ifdef GWA
#include "GWA.h"


CGWA::CGWA(GraphWidget* gw, runtimeWindow *progress)
{
	this->gw = gw;
	setDefaults();
	g_get_settings();
	g_get_params();
	g_get_model_configuration(progress);
	g_get_observed();
	

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
			for (int j = 0; j<measured_quan.size(); j++)
				if (min < measured_quan[j].minstdev)
					min = measured_quan[j].minstdev;

			P.low = max(min, exp(-4)); P.high = exp(4);
			parameters.push_back(P);
			params_vals.push_back(sqrt(P.low*P.high));

		}
	}
	obs_std.resize(stds.size());

	for (int i = 0; i<parameters.size(); i++)
		set_param(i, params_vals[i]);

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

#endif
#ifdef GIFMOD
CGA::CGA(CMediumSet *model, runtimeWindow* rtw)
#endif
#ifdef GWA
CGA::CGA(CGWASet *model, runtimeWindow* rtw)
#endif
{
	int ii;
	char buffer[33];
	Sys = *model;
	//ifstream file(filename);
	nParam = 0;
	totnParam = 0;
	vector<string> s;
	pcross = 1;
	N = 1;
	fixedstd = true;
	purt_fac = 0.1;
	noinipurt = false;
	sensbasedpurt = false;
	justreadmcmc = false;
	nrealizations = 60;
	writeinterval = 1;
	dp_sens = 0.001;
	//purt_fac = 0.5;
	sens_out = false;
	RCGA = false;
	purtscale = 0.75;
	global_sensitivity = false;
	continue_mcmc = false;
	calculate_percentile = true;
	mcmc_realization = true;
	calculate_correlation = true;
#ifdef GIFMOD
	pathname = model->FI.outputpathname;
#endif
#ifdef GWA
	pathname = model->operator()().pathname;
#endif
	readfromgafile = false;
	calc_distributions = false;
	noise_realization_writeout = false;
	obs_realization_writeout = false;
	const_realization_writeout = false;
	influent_realization = false;
	no_bins = 10;
	mcmc_run = false;
	outputfilename = "GAout.txt";
	numberOfThreads = 15;
	QList<Entity*> GAMCMC;
	Entity *e;
#ifdef GIFMOD
	e = model->gw->entityByName("Genetic algorithm");
	if (e != nullptr)	
		GAMCMC.append(e);
	e = model->gw->entityByName("Markov chain Monte Carlo");
#endif
#ifdef GWA
	e = model->Medium[0].gw->entityByName("Genetic algorithm");
	if (e != nullptr)
		GAMCMC.append(e);
	e = model->Medium[0].gw->entityByName("Markov chain Monte Carlo");
#endif
	if (e != nullptr)
		GAMCMC.append(e);
	QStringList intVars;
	intVars << "maxpop" << "ngen" << "burnout" << "nchains" << "nmcmcsamples" << "stucklimit" << "sensbasedpurt" << "justreadmcmc" << "nrealizations" << "calculate_sensitivity" << "writeinterval" << "noinipurt" << "global_sensitivity" <<
		"continuemcmc" << "calculate_percentile" << "calculate_correlation" << "mcmc_realization" << "noise_realization_writeout" << "obs_realization_writeout" << "const_realization_writeout" << "influent_realization" <<
		"readfromgafile" << "calc_distributions" << "no_bins" << "mcmc_run";

    foreach(Entity* e , GAMCMC)
	{
        foreach (QString code , e->codes())
		{
			if (!e->val(code).isEmpty() && e->val(code) != ".")
			{
				if (code.toStdString() == "number_of_threads") numberOfThreads = e->val(code).toInt();
				if (code.toStdString() == "maxpop") maxpop = e->val(code).toInt();
				if (code.toStdString() == "ngen") nGen = e->val(code).toInt();
				if (code.toStdString() == "burnout") 
					burnout = e->val(code).toInt();
				if (code.toStdString() == "nchains") 
					nchains = e->val(code).toInt();
				if (code.toStdString() == "nmcmcsamples") nMCMCsamples = e->val(code).toInt();
				if (code.toStdString() == "acceptance_rate") acceptance_rate = e->val(code).toFloat();
				if (code.toStdString() == "nrealizations") nrealizations = e->val(code).toInt();
				if (code.toStdString() == "writeinterval") writeinterval = e->val(code).toInt();
				if (code.toStdString() == "continuemcmc") continue_mcmc = e->val(code).toBool();
				if (code.toStdString() == "no_bins") no_bins = e->val(code).toInt();

				if (code.toStdString() == "mcmc_realization") mcmc_realization = e->val(code).toBool();
				if (code.toStdString() == "noise_realization_writeout") noise_realization_writeout = e->val(code).toBool();
				if (code.toStdString() == "obs_realization_writeout") obs_realization_writeout = e->val(code).toBool();
				if (code.toStdString() == "const_realization_writeout") const_realization_writeout = e->val(code).toBool();
				if (code.toStdString() == "influent_realization") influent_realization = e->val(code).toBool();
				if (code.toStdString() == "readfromgafile") readfromgafile = e->val(code).toBool();
				if (code.toStdString() == "calc_distributions") calc_distributions = e->val(code).toBool();
				if (code.toStdString() == "mcmc_run") mcmc_run = e->val(code).toBool();
				if (code.toStdString() == "sensbasedpurt") sensbasedpurt = e->val(code).toBool();
				if (code.toStdString() == "justreadmcmc") justreadmcmc = e->val(code).toBool();
				if (code.toStdString() == "calculate_sensitivity") sens_out = e->val(code).toBool();
				if (code.toStdString() == "noinipurt") noinipurt = e->val(code).toBool();
				if (code.toStdString() == "global_sensitivity") global_sensitivity = e->val(code).toBool();
				if (code.toStdString() == "calculate_percentile") calculate_percentile = e->val(code).toBool();
				if (code.toStdString() == "calculate_correlation") calculate_correlation = e->val(code).toBool();

				if (code.toStdString() == "pcross") pcross = e->val(code).toFloat();
				if (code.toStdString() == "pmute") pmute = e->val(code).toFloat();
				if (code.toStdString() == "shakescale") shakescale = e->val(code).toFloat();
				if (code.toStdString() == "shakescalered") shakescalered = e->val(code).toFloat();
				if (code.toStdString() == "purtscale") purtscale = e->val(code).toFloat();
				if (code.toStdString() == "purtfac") purt_fac = e->val(code).toFloat();
				if (code.toStdString() == "purt_fac") purt_fac = e->val(code).toFloat();
				if (code.toStdString() == "dp_sensitivity") dp_sens = e->val(code).toFloat();
#ifdef GIFMOD
				if (code.toStdString() == "outputfile") 
					outputfilename = fullFilename(e->val(code), model->gw->modelPathname()).toStdString();
				if (code.toStdString() == "mcmcoutputfile") 
					mcmcoutputfile = fullFilename(e->val(code), model->gw->modelPathname()).toStdString();
				if (code.toStdString() == "readfromgafile")
					getfromfilename = fullFilename(e->val("readfromgafile"), model->gw->modelPathname()).toStdString();

#endif
#ifdef GWA
				if (code.toStdString() == "outputfile")
					outputfilename = fullFilename(e->val(code), model->Medium[0].gw->modelPathname()).toStdString();
				if (code.toStdString() == "mcmcoutputfile")
					mcmcoutputfile = fullFilename(e->val(code), model->Medium[0].gw->modelPathname()).toStdString();
				if (code.toStdString() == "readfromgafile")
					getfromfilename = fullFilename(e->val("readfromgafile"), model->Medium[0].gw->modelPathname()).toStdString();
#endif
				//if (code.toStdString() == "forward_montecarlo_filename") forward_inflow_ARIMA_params = pathname + e->val(code).toStdString();
				if (code.toStdString() == "initial_population") initialpopfilemame = e->val(code).toStdString();

				if (code.toStdString() == "calc_output_percentiles")
				{
					QStringList list = e->val(code).toQString().split(':');
					for (int i = 0; i<list.size(); i++)

						calc_output_percentiles.push_back(list[i].toFloat());
					//						calc_output_percentiles.push_back(atof(s[i + 1].c_str()));
				}
			}
		}
	}
	//outputfilename = fullFilename(QString::fromStdString(outputfilename), model->gw->modelPathname()).toStdString();
	//model->numberOfThreads = numberOfThreads;
	for (int i = 0; i<Sys.parameters.size(); i++)
		if (Sys.parameters[i].fixed == false)
		{
			totnParam++;
			nParam++;
			if (Sys.parameters[i].log == 1)
			{
				minval.push_back(log10(Sys.parameters[i].low));
				maxval.push_back(log10(Sys.parameters[i].high));
			}
			else
			{
				minval.push_back(Sys.parameters[i].low);
				maxval.push_back(Sys.parameters[i].high);
			}
			paramname.push_back(Sys.parameters[i].name);
			apply_to_all.push_back(true);
			loged.push_back(Sys.parameters[i].log);
			to_ts.push_back(-1);
		}



	Ind.resize(maxpop);
	Ind_old.resize(maxpop);

	fitdist = CDistribution(maxpop);
	cross_over_type = 1;

	for (int i = 0; i<maxpop; i++)
	{
		Ind[i] = CIndividual(totnParam);
		Ind_old[i] = CIndividual(totnParam);

	}

	for (int i = 0; i<totnParam; i++)
		Setminmax(i, minval[get_act_paramno(i)], maxval[get_act_paramno(i)], 4);

	MaxFitness = 0;
}
int CGA::optimize(runtimeWindow* rtw)
{
	string RunFileName = outputfilename;

	FILE *FileOut;
	FILE *FileOut1;

	FileOut = fopen(RunFileName.c_str(), "w");
	fclose(FileOut);
#ifdef GIFMOD
	FileOut1 = fopen((Sys.FI.outputpathname + "detail_GA.txt").c_str(), "w");
#endif
#ifdef GWA
	FileOut1 = fopen((Sys.pathname() + "detail_GA.txt").c_str(), "w");
#endif

	fclose(FileOut1);

	double shakescaleini = shakescale;

	vector<double> X(Ind[0].nParams);

	Sys1.resize(maxpop);

	initialize();
	double ininumenhancements = numenhancements;
	numenhancements = 0;

	CMatrix Fitness(nGen, 3);

	for (int i = 0; i<nGen; i++)
	{

        ////qDebug()<<"Assign Fitnesses";
        assignfitnesses(rtw);
        ////qDebug()<<"Assign Fitnesses done! ";
		if (rtw->stopTriggered)
			break;
		FileOut = fopen(RunFileName.c_str(), "a");
		printf("Generation: %i\n", i);
		fprintf(FileOut, "Generation: %i\n", i);
		fprintf(FileOut, "ID, ");
		for (int k = 0; k<Ind[0].nParams; k++)
			fprintf(FileOut, "%s, ", paramname[k].c_str());
		fprintf(FileOut, "%s, %s, %s", "likelihood", "Fitness", "Rank");
		
		for (int i = 0; i < Sys.measured_quan.size(); i++)
			fprintf(FileOut, ", MSE_%s" , Sys.measured_quan[i].name.c_str());
		fprintf(FileOut, "\n");

		for (int j1 = 0; j1<maxpop; j1++)
		{
			fprintf(FileOut, "%i, ", j1);

			for (int k = 0; k<Ind[0].nParams; k++)
				if (loged[get_act_paramno(k)] == true)
					fprintf(FileOut, "%le, ", pow(10, Ind[j1].x[k]));
				else
					fprintf(FileOut, "%le, ", Ind[j1].x[k]);

			fprintf(FileOut, "%le, %le, %i, ", Ind[j1].actual_fitness, Ind[j1].fitness, Ind[j1].rank);
			for (int i = 0; i < Sys.measured_quan.size(); i++)
				fprintf(FileOut, "%e, ", Sys1[j1].MSE_obs[i]);
			fprintf(FileOut, "\n");
		}
		fclose(FileOut);

		int j = maxfitness();

		Fitness[i][0] = Ind[j].actual_fitness;
		QMap<QString, QString> vars;
		vars["i"] = QString::number(i);
		vars["likelihood"] = QString::number(-Fitness[i][0]);
		//qDebug() << i << -Fitness[i][0];

		rtw->experiment = &Sys.Medium[0];//should represent experiment
        ////qDebug()<<"Line 499 in runmodel.cpp";
		updateProgress(rtw, vars);
        ////qDebug()<<"Update progress done! (501)";
        QApplication::processEvents(QEventLoop::AllEvents,100*1000);

		//plot i as ngen, exp(-Fitness[i][0] )
		if (i>10)
		{
			if ((Fitness[i][0] == Fitness[i - 3][0]) && shakescale>pow(10.0, -Ind[0].precision[0]))
				shakescale *= shakescalered;

			if ((Fitness[i][0]>Fitness[i - 1][0]) && (shakescale<shakescaleini))
				shakescale /= shakescalered;
			numenhancements = 0;
		}
		if (i>50)
		{
			if ((Fitness[i][0] == Fitness[i - 20][0]))
			{
				numenhancements *= 1.05;
				if (numenhancements == 0) numenhancements = ininumenhancements;
			}

			if ((Fitness[i][0] == Fitness[i - 50][0]))
				numenhancements = ininumenhancements * 10;
		}

		Fitness[i][1] = shakescale;
		Fitness[i][2] = pmute;

		if (i>20)
		{
			if (shakescale == Fitness[i - 20][1])
				shakescale = shakescaleini;
		}

		//Form1.label1->Text=L"Cross-over and Mutation ";
		//Form1.Refresh();
		j = maxfitness();
		MaxFitness = Ind[j].actual_fitness;
		Fitness[i][0] = Ind[j].actual_fitness;

		fillfitdist();
        ////qDebug()<<"Cross over";
        if (RCGA == true)
			crossoverRC();
		else
			crossover();

        ////qDebug()<<"Cross over done!";
        mutate(pmute);
        ////qDebug()<<"Mutation done!";
		shake();
        ////qDebug()<<"Shaking done!";
	}
	//Form1.label1->Text=L"finalizing GA ";
	//Form1.Refresh();
	assignfitnesses(rtw);
#ifdef GWA
	Sys_out.Medium[0].Do_project();
#endif
	FileOut = fopen(RunFileName.c_str(), "a");
	fprintf(FileOut, "Final Enhancements\n");
	double l_MaxFitness = 1;
	int j = maxfitness();

	MaxFitness = Ind[j].actual_fitness;
	final_params.resize(totnParam);

	for (int k = 0; k<Ind[0].nParams; k++)
	{
		if (loged[get_act_paramno(k)] == true) final_params[k] = pow(10, Ind[j].x[k]); else final_params[k] = Ind[j].x[k];
		fprintf(FileOut, "%s, ", paramname[k].c_str());
		fprintf(FileOut, "%le, ", final_params[k]);
		fprintf(FileOut, "%le, %le\n", Ind[j].actual_fitness, Ind[j].fitness);
	}
	fclose(FileOut);

	if (!rtw->stopTriggered)
		assignfitnesses(final_params);

    ////qDebug()<<"clearing sys1";
    Sys1.clear();
    ////qDebug()<<"Systems cleared";
	QMap<QString, QString> vars;
	updateProgress(rtw, vars, true);
	
	return maxfitness();
}
#ifdef GWA
void CGWA::setDefaults()
{
	inverse = false;
	//		getconfigfromfile(filename);   //reads input file and creates lid_config
	single_vz_delay = false;
	project_finish = 2040;
	project_interval = 1;
	project = false;
	fixed_old_tracer = false;
}
void CGWA::g_get_params()
{
	for each (Entity *e in gw->entitiesByType("Parameter"))
	{
		inverse = true;
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
		P.value = e->val("value").toFloat();
		P.applytoall = (e->val("applytoall") == "Yes") ? 1 : 0;
		if (e->val("log") == "Normal")
			P.log = 0;
		else if (e->val("log") == "Uniform")
			P.log = 2;
		else 
			P.log = 1;
		parameters.push_back(P);
		if (e->val("value") == "")
		{
			if (P.log == 0 || P.log==2)
				params_vals.push_back(0.5*(P.low + P.high));
			else
				params_vals.push_back(P.low / fabs(P.low)*sqrt(P.low*P.high));
		}
		else params_vals.push_back(P.value);
	}
}
#endif

void CGA::updateProgress(runtimeWindow* rtw, QMap<QString, QString> params, bool finished) const
{
	if (rtw != 0)
	{
		QMap<QString, QVariant> vars;
		vars["mode"] = "inverse";
		if (finished)
		{
			vars["progress"] = 100;
			vars["progress2"] = 100;
			vars["finished"] = true;
		}
		else
		{
			int progress;
			int i = params["i"].toInt()+1;
			progress = 100.0*double(i) / double(nGen);
			vars["i"] = params["i"];
			vars["progress"] = progress;
			vars["likelihood"] = params["likelihood"];
		}
		rtw->update(vars);
	}
    ////qDebug()<<"update progress done! (658)";
}
void CGA::updateProgress(runtimeWindow* rtw, bool resetGeneration) const
{
	static int populationProgress;
	populationProgress++; 
	if (resetGeneration) populationProgress = 0;
	
	if (rtw != 0)
	{
		QMap<QString, QVariant> vars;
		vars["mode"] = "inverse";
			int progress;
			
			progress = 100.0*populationProgress / maxpop;
			vars["progress2"] = progress;
			//vars["x-axis label"] = "Generation";

		rtw->update(vars);
	}
}

void CGA::assignfitnesses(runtimeWindow* rtw)
{
	sumfitness = 0;

	vector<vector<double>> inp;

	inp.resize(maxpop);


	for (int k = 0; k<maxpop; k++) inp[k].resize(totnParam);

	vector<double> time_(maxpop);
	vector<int> epochs(maxpop);
	clock_t t0, t1;

	for (int k = 0; k < maxpop; k++)
	{
		//QMap<QString, QString> vars;
		//vars["i"] = i;
		//vars["likelihood"] = QString::number(exp(-Fitness[i][0]));
		updateProgress(rtw);
		if (rtw->stopTriggered)
			break;
		for (int i = 0; i < totnParam; i++)
		{
			if (loged[get_act_paramno(i)] != 1)
			{
				inp[k][i] = Ind[k].x[i];    //Ind
			}
			else
			{
				inp[k][i] = pow(10, Ind[k].x[i]);
			}
		}

		int jj = 0;
		Ind[k].actual_fitness = 0;

		Sys1[k] = Sys;
		Sys1[k].ID = numbertostring(k);
#ifdef GIFMOD
		//Sys1[k].FI.write_details = false;
		int l = 0;
		for (int i = 0; i < nParam; i++) Sys1[k].set_param(i, inp[k][i]);
		Sys1[k].finalize_set_param();
#endif

#ifdef GWA
	Sys1[k].Medium[0].project = false;
	//			Sys1[k][ts].write_details = false;
	int l = 0;
	for (int i = 0; i < nParam; i++) Sys1[k].Medium[0].set_param(i, inp[k][i]);
	Sys1[k].Medium[0].finalize_set_param();
#endif
	}
    ////qDebug()<<"Update progress inside Assignfitnesses";
    updateProgress(rtw, true);
    ////qDebug()<<"Update progress inside Assignfitnesses, done! ";

/*	int populationProgress;
	QMap<QString, QVariant> vars;
	vars["mode"] = "inverse";
	int progress;
*/
	omp_set_num_threads(numberOfThreads);
#pragma omp parallel for //private(ts,l) ATTENTION
	for (int k = 0; k<maxpop; k++)
	{
//			if (k%(numberOfThreads) == 0)
//				updateProgress(rtw);

			//int ts,l;

		FILE *FileOut;
#ifdef GIFMOD
		FileOut = fopen((Sys.FI.outputpathname + "detail_GA.txt").c_str(), "a");
#endif
#ifdef GWA
		FileOut = fopen((Sys.pathname() + "detail_GA.txt").c_str(), "a");
#endif

        std::fprintf(FileOut, "%i, ", k);
        QString s = "Begining individual " + QString::number(k) + " Parameter values: ";
		for (int l = 0; l < Ind[0].nParams; l++)
			if (loged[get_act_paramno(l)] == true)
            {	std::fprintf(FileOut, "%le, ", pow(10, Ind[k].x[l]));
                s=s+QString::number(pow(10, Ind[k].x[l]),'e',2) + ", ";
            }
			else
            {   std::fprintf(FileOut, "%le, ", Ind[k].x[l]);
                s=s+QString::number(Ind[k].x[l],'e',2);
            }
#pragma omp critical
        {if (rtw->sln_dtl_active)
            rtw->slndetails_append(s);
        }
		//fprintf(FileOut, "%le, %le, %i, %e, %i, %i", Ind[k].actual_fitness, Ind[k].fitness, Ind[k].rank, time_[k], threads_num[k],num_threads[k]);
		//fprintf(FileOut, "%le, %le, %i, %e", Ind[k].actual_fitness, Ind[k].fitness, Ind[k].rank, time_[k]);
		std::fprintf(FileOut, "\n");
		std::fclose(FileOut);
        QCoreApplication::processEvents(QEventLoop::AllEvents,100*1000);
		clock_t t0 = clock();
//		for (int ts = 0; ts<1; ts++)
//		{
#ifdef GIFMOD
		Ind[k].actual_fitness -= Sys1[k].calc_log_likelihood();
#endif
#ifdef GWA
		Ind[k].actual_fitness -= Sys1[k]().calc_log_likelihood();
#endif
		epochs[k] += Sys1[k].epoch_count();
//		}
		time_[k] = ((float)(clock() - t0)) / CLOCKS_PER_SEC;
#ifdef GIFMOD
		FileOut = fopen((Sys.FI.outputpathname + "detail_GA.txt").c_str(), "a");
#endif
#ifdef GWA
		FileOut = fopen((Sys.Medium[0].pathname + "detail_GA.txt").c_str(), "a");
#endif

#pragma omp critical
        {
            if (rtw->sln_dtl_active)
                rtw->slndetails_append("Finished Individual " + QString::number(k) + ":" + " fitness: " + QString::number(Ind[k].actual_fitness) + "time: " + QString::number(time_[k]) + "epochs: " + QString::number(epochs[k]));
        }

        std::fprintf(FileOut, "%i, fitness=%le, time=%e, epochs=%i\n", k, Ind[k].actual_fitness, time_[k], epochs[k]);
		std::fclose(FileOut);

	}

	Sys_out = Sys1[maxfitness()];
#ifdef GWA
	Sys_out.Medium[0].project = Sys.Medium[0].project;
#endif
	inp.clear();
	assignfitness_rank(N);

}
#ifdef GIFMOD
void MainWindow::inverseRun(CMediumSet *modelSet, runtimeWindow* rtw)
{

	clock_t t0, t1;
	t0 = clock();
	ofstream runtime_file;
	runtime_file.open(modelSet->FI.outputpathname + "Runing_times.txt");
	CGA GA(modelSet, rtw); //initialize GA
//	CMCMC MCMCtemp(GA);

	mainGraphWidget->results = new Results();
	//**************************************** Genetic ****************************************
	
	if (GA.justreadmcmc == false && GA.continue_mcmc == false)
	{
		if (GA.getfromfilename == "")
		{
			rtw->setLabel("Deterministic Parameter Estimation");
			GA.optimize(rtw);

			// Genetic run time :
			t1 = clock() - t0;
			float run_time = ((float)t1) / CLOCKS_PER_SEC;
			string st = " sec";
			if (run_time >= 60) { run_time /= 60; st = " min"; }
			if (run_time >= 60) { run_time /= 60; st = " hr"; }

			runtime_file << " Genetic Run Time :  " << run_time << st << std::endl;
		}
		else
		{
			CVector out(1);
			out[0] = GA.getfromoutput(GA.getfromfilename);
			//out.writetofile(modelSet->outputpathname +"likelihood.txt");
		}
		rtw->setLabel("Updating Model Parameters");
		//qDebug() << 900;
		mainGraphWidget->trackingUndo = false;
		QList<Entity*> GUIparameters = mainGraphWidget->entitiesByType("Parameter");

		for (int i = 0; i < GUIparameters.size(); i++)
			GUIparameters[i]->setValue("Value", QString::number(GA.final_params[GA.Sys.lookup_parameters(GUIparameters[i]->name.toStdString())]));
		mainGraphWidget->trackingUndo = true;

		for (int i = 0; i < GA.Sys.Medium.size(); i++)
		{
			GA.Sys_out.ANS_hyd[i]->writetofile(GA.Sys.Medium[i].detoutfilename_hydro, modelSet->FI.write_interval);
			GA.Sys_out.ANS_colloids[i]->writetofile(GA.Sys.Medium[i].detoutfilename_prtcle, modelSet->FI.write_interval);
			GA.Sys_out.ANS_constituents[i]->writetofile(GA.Sys.Medium[i].detoutfilename_wq, modelSet->FI.write_interval);
			GA.Sys_out.ANS_control[i]->writetofile(GA.Sys.Medium[i].detoutfilename_control, modelSet->FI.write_interval);
//			GA.Sys_out.ANS_hyd[i]->writetofile(modelSet->FI.outputpathname + GA.Sys.Medium[i].detoutfilename_hydro, modelSet->FI.write_interval);
//			GA.Sys_out.ANS_colloids[i]->writetofile(modelSet->FI.outputpathname + GA.Sys.Medium[i].detoutfilename_prtcle, modelSet->FI.write_interval);
//			GA.Sys_out.ANS_constituents[i]->writetofile(modelSet->FI.outputpathname + GA.Sys.Medium[i].detoutfilename_wq, modelSet->FI.write_interval);
		}


		if (modelSet->FI.detoutfilename_obs != "")
		{
			//Form1.label1->Text=L"Saving Deterministic Output";
			//Form1.Refresh();

			for (int i = 0; i < 1; i++)
				GA.Sys_out.ANS_obs.writetofile(GA.Sys.FI.detoutfilename_obs, modelSet->FI.write_interval);
//			GA.Sys_out.ANS_obs.writetofile(modelSet->FI.outputpathname + GA.Sys.FI.detoutfilename_obs, modelSet->FI.write_interval);
		}


	}

	//**************************************** local sensitivity ****************************************
	t0 = clock();
	//qDebug() << 1000;
	CMCMC MCMC(GA);
	int mcmcstart = MCMC.n_chains;
	if (GA.justreadmcmc == false && rtw->stopTriggered == false)
	{
		//Form1.label1->Text=L"MCMC Initialization";
		//	Form1.Refresh();
		if (MCMC.continue_mcmc == false)
		{
			//qDebug() << 1001;
			MCMC.initialize(GA.final_params);
		}
		else
		{
			//qDebug() << 1002;
			mcmcstart = MCMC.readfromfile(MCMC.outputfilename);
		}
		//qDebug() << 1003;

		if ((GA.sens_out == true) && (MCMC.continue_mcmc == false))
		{
			//Form1.label1->Text=L"Sensitivity Analysis ";
			//Form1.Refresh();

			//CMatrix S_mat = MCMC.sensitivity_mat(GA.dp_sens, GA.final_params);
			//S_mat.writetofile(modelSet->pathname + "sensitivity_mat.txt");
            ////qDebug() << 1004;
	rtw->setLabel("Local Sensitivity Analysis");
			CMatrix S_mat_lumped = MCMC.sensitivity_mat_lumped(GA.dp_sens, GA.final_params);
            ////qDebug() << 1005;
			S_mat_lumped.writetofile(modelSet->FI.outputpathname + "sensitivity_mat_lumped.txt");
            ////qDebug() << 1006;
			mainGraphWidget->results->localSensitivityMatrix = S_mat_lumped;

		}
	}
	//qDebug() << 1007;
	//**************************************** MCMC ****************************************
	if (GA.mcmc_run == true && rtw->stopTriggered == false)	//Switch for MCMC Section
	{
        ////qDebug() << 1008;
		if (GA.justreadmcmc == false)
		{
            ////qDebug() << 1009;
			rtw->setMode("MCMC");
			rtw->setLabel("Stochastic Parameter Estimation");
			MCMC.step(mcmcstart, int((MCMC.nsamples - mcmcstart) / MCMC.n_chains)*MCMC.n_chains, MCMC.outputfilename, rtw);
            ////qDebug() << 1010;
			// MCMC run time :			
			t1 = clock() - t0;
			float run_time = ((float)t1) / CLOCKS_PER_SEC;
			string st = " sec";
			if (run_time >= 60) { run_time /= 60; st = " min"; }
			if (run_time >= 60) { run_time /= 60; st = " hr"; }

			runtime_file << " MCMC Run Time :  " << run_time << st << std::endl;
		}
		//qDebug() << 1011;
		CBTCSet MCMCOut(MCMC.outputfilename, false);
		MCMCOut.names = MCMC.paramname;
		if (GA.calc_distributions == true)
		{
			//qDebug() << 1012;
			CBTCSet posterior_distribution = MCMCOut.distribution(GA.no_bins, MCMC.paramname.size(), MCMC.n_burnout);
			posterior_distribution.names = MCMC.paramname;
			posterior_distribution.writetofile(modelSet->FI.outputpathname + "posterior_distribution.txt");
			//qDebug() << 1013;
			mainGraphWidget->results->posteriors = posterior_distribution;

			CBTCSet prior_distribution = MCMC.prior_distribution(GA.no_bins * 10);
			//qDebug() << 1014;
			prior_distribution.names = MCMC.paramname;
			prior_distribution.writetofile(modelSet->FI.outputpathname + "prior_distribution.txt");

			mainGraphWidget->results->priors = prior_distribution;
		}

		//********************************* MCMC Percentile **************************************
		//qDebug() << 1015;
		t0 = clock();
		char buffer[33];

		if (GA.calculate_percentile == true)
		{
			//qDebug() << 1016;
			vector<int> index;
			for (int i=0; i<MCMC.nActParams; i++) index.push_back(i+1);
			vector<double> p2dot5 = MCMCOut.percentile(0.025, MCMC.n_burnout);
			vector<double> p50 = MCMCOut.percentile(0.5, MCMC.n_burnout);
			vector<double> mean = MCMCOut.mean(MCMC.n_burnout);
			vector<double> std = MCMCOut.std(MCMC.n_burnout);
			vector<double> p97dot5 = MCMCOut.percentile(0.975, MCMC.n_burnout);

			FILE *filepercentile;
			filepercentile = fopen((modelSet->FI.outputpathname + "percentiles.txt").c_str(), "w");
			//qDebug() << 1017;
			for (int i = 0; i < MCMCOut.names.size(); i++)
				fprintf(filepercentile, "%s, ", MCMCOut.names[i].c_str());

			fprintf(filepercentile, "\n");

			CVector(p2dot5).writetofile(filepercentile);
			CVector(p50).writetofile(filepercentile);
			CVector(p97dot5).writetofile(filepercentile);
			CVector(mean).writetofile(filepercentile);
			CVector(std).writetofile(filepercentile);

			double mean_log = mean[MCMCOut.nvars - 4];
			fprintf(filepercentile, "mean log: %e\n", 2 * mean_log);

			double logp_mean = -GA.assignfitnesses(mean);
			fprintf(filepercentile, "f(mean): %e\n", 2 * logp_mean);

			double DIC = 4 * mean_log - 2 * logp_mean;
			fprintf(filepercentile, "DIC: %e\n", DIC);

			fclose(filepercentile);

			int n = MCMCOut.names.size();
			mainGraphWidget->results->percentiles.resize(n);
			//qDebug() << 1018;
			for (int i = 0; i < n; i++)
			{
				mainGraphWidget->results->percentiles[i].parameter = MCMCOut.names[i];
				mainGraphWidget->results->percentiles[i].p25 = p2dot5[i];
				mainGraphWidget->results->percentiles[i].p50 = p50[i];
				mainGraphWidget->results->percentiles[i].mean = mean[i];
				mainGraphWidget->results->percentiles[i].p975 = p97dot5[i];
				mainGraphWidget->results->percentiles[i].std = std[i];
			}

		//********************************* MCMC mean out **************************************

			double out1 = GA.assignfitnesses(mean);
			//qDebug() << 1019;

			GA.Sys_out.ANS_obs.writetofile(modelSet->FI.outputpathname + "BTC_Obs_mean.txt", modelSet->FI.write_interval);
			
		}

		//********************************* MCMC Correlation **************************************

		if (GA.calculate_correlation == true)
		{
			//qDebug() << 1020;
			CMatrix correlation_mat = MCMCOut.correlation(MCMC.n_burnout, MCMC.MCMCParam.size());
			correlation_mat.writetofile(modelSet->FI.outputpathname + "correlation_mat.txt");
			mainGraphWidget->results->correlationMatrix = correlation_mat;
		}

		//********************************* MCMC realizations out **************************************

		if (GA.mcmc_realization == true)
		{
			//qDebug() << 1021;
			rtw->setLabel("Generating Posterior Realizations");
			MCMC.rtw = rtw;
			MCMC.get_outputpercentiles(MCMCOut);
			//qDebug() << 1022;
			if (GA.obs_realization_writeout)
				for (int i = 0; i < MCMC.G.measured_quan.size(); i++)
					for (int j = 0; j < 1; j++)
						MCMC.BTCout_obs[j][i].writetofile(modelSet->FI.outputpathname + "BTC_obs_" + MCMC.G.measured_quan[i].name + ".txt", modelSet->FI.write_interval);
			//qDebug() << 1023;
			if (GA.noise_realization_writeout)
				for (int i = 0; i < MCMC.G.measured_quan.size(); i++)
					for (int j = 0; j < 1; j++)
						MCMC.BTCout_obs_noise[j][i].writetofile(modelSet->FI.outputpathname + "BTC_obs_noise_" + MCMC.G.measured_quan[i].name + ".txt", modelSet->FI.write_interval);
			//qDebug() << 1024;
			// calculating percentiles
			
			mainGraphWidget->results->realizations = MCMC.BTCout_obs;
			mainGraphWidget->results->realizationsPercentiles = MCMC.BTCout_obs_prcntle;
			mainGraphWidget->results->noiseRealizations = MCMC.BTCout_obs_noise;
			mainGraphWidget->results->noiseRealizationsPercentiles = MCMC.BTCout_obs_prcntle_noise;  
			mainGraphWidget->results->model = mainGraphWidget->modelSet;
			// Observations mainGraphWidget->modelSet->measured_quan
			//qDebug() << 1025;
			if (modelSet->FI.realizeparamfilename != "") MCMC.paramsList.writetofile(modelSet->FI.outputpathname + modelSet->FI.realizeparamfilename);
			//qDebug() << 1026;

			if (MCMC.global_sensitivity == true)
			{
				//qDebug() << 1027;
				CMatrix global_sens_lumped_avg = Average(MCMC.global_sens_lumped);
				global_sens_lumped_avg.writetofile(modelSet->FI.outputpathname + "global_sensitivity_lumped_avg.txt");
				mainGraphWidget->results->globalSensitivityMatrix = global_sens_lumped_avg;

			}
			//qDebug() << 1028;
			// post-MCMC run time :		
			t1 = clock() - t0;
			float run_time = ((float)t1) / CLOCKS_PER_SEC;
			string st = " sec";
			if (run_time >= 60) { run_time /= 60; st = " min"; }
			if (run_time >= 60) { run_time /= 60; st = " hr"; }

			runtime_file << " Post-MCMC Run Time :  " << run_time << st << std::endl;
	    	runtime_file.close();
		}
	}
	if (mainGraphWidget->model != 0) delete mainGraphWidget->model;
	mainGraphWidget->modelSet = new CMediumSet(GA.Sys_out);
	mainGraphWidget->results->model = mainGraphWidget->modelSet;
	mainGraphWidget->experimentSelect(mainGraphWidget->experimentsList()[0]);
	mainGraphWidget->hasResults = true;

}

void CMediumSet::g_get_observed()
{
	QStringList stdlist;
    foreach (Entity *e , gw->entitiesByType("Observation"))
	{
		measured_chrc M;
		M.error_structure = 0;
		M.name = e->Name().toStdString();
		M.id = split(e->val("id").toStdString(), '+');  //location id //BLOCK/CONNECTOR's NAME
		M.loc_type = (e->val("loc_type") == "Block") ? 0 : 1; //OBSERVED SUBTYPE
		if (gw->EntityNames("Particle").contains(e->val("quan").toQString().split(':')[0]))
			M.quan = QString("g[%1]").arg(e->val("quan").toQString()).toStdString();
		else if (gw->EntityNames("Constituent").contains(e->val("quan").toQString().split(':')[0]))
			M.quan = QString("cg[%1]").arg(e->val("quan").toQString()).toStdString();
		else
            M.quan = XString::reformBack(e->val("quan")).toStdString();


		QString std = e->val("std_no");
		if (std.isEmpty())
			std = e->Name() + "@" + e->val("quan");
		if (!stdlist.contains(std))
			stdlist.append(std);
		M.std_no = stdlist.indexOf(std); // .toInt();
		M.error_structure = (e->val("error_structure") == "Normal") ? 0 : 1; //NORM 0 ; LOg 1
		M.experiment = e->val("experiment").toStdString();
		measured_quan.push_back(M);
		string file = fullFilename(e->val("observed_data"), gw->modelPathname()).toStdString();
		CBTC observed = CBTC();
		if (file.size())
		{
			CBTCSet _observed(file, true);
			if (_observed.file_not_found)
			{
				gw->newError(QString("Could not read observation data file %1 for %2").arg(QString::fromStdString(file)).arg(e->name));
			}
			else
			{
				if (_observed.BTC.size())
					observed = _observed[0];
				else
					gw->newError(QString("Could not read observation data file %1 for %2").arg(QString::fromStdString(file)).arg(e->name));
			}
			
				
		}
		measured_data.append(observed, e->name.toStdString());
	}
	vector<int> stds;
	for (int i = 0; i<measured_quan.size(); i++)
	{
		if (lookup(stds, measured_quan[i].std_no) == -1)
		{
			stds.push_back(measured_quan[i].std_no);
			measured_quan[i].std_to_param = int(parameters.size());
			param_range P;

			P.fixed = false;
			P.log = true;
			P.applytoall = true;
			P.tempcorr = 1;
			P.name = "std_" + numbertostring(i);
			P.low = exp(-4); P.high = exp(4);
			P.value = sqrt(P.high*P.low);
			parameters.push_back(P);
		}
	}
	std.resize(stds.size());
	set_features.observed = true;
}

void CMediumSet::g_get_sensors()
{
    foreach (Entity *e , gw->entitiesByType("Sensor"))
	{
		CSensor M(gw->experimentsList().count());
		M.error_structure = 0;
		M.name = e->Name().toStdString();
		M.loc_type = (e->val("loc_type") == "Block") ? 0 : 1; //OBSERVED SUBTYPE
		string equation = convertstringtoStringOP(e->val("quan").toQString(), gw);
		
		M.quan = CStringOP(equation, &RXN);

		M.id = (e->val("id").toStdString());
		M.error_std = e->val("error_std").toFloat();
		M.error_structure = (e->val("error_structure") == "Normal") ? 0 : 1; //NORM 0 ; LOg 1
		M.interval = e->val("interval").toFloat();
		M.error_std = e->val("std").toFloat();


		Control.Sensors.push_back(M);
	}

}

void CMediumSet::g_get_objective_functions()
{
	
    foreach (Entity *e , gw->entitiesByType("Objective Function"))
	{
		CObjectiveFunction M;
		M.name = e->Name().toStdString();
		M.loc_type = (e->val("loc_type") == "Block") ? 0 : 1; //OBSERVED SUBTYPE
		M.location = (e->val("id").toStdString());
		string equation = convertstringtoStringOP(e->val("quan").toQString(), gw);

		M.expression = CStringOP(equation, &RXN);

		Control.ObjectiveFunctions.push_back(M);
	}

}


void CMediumSet::g_get_controllers()
{
    foreach (Entity *e , gw->entitiesByType("Controller"))
	{
		CController M;

		M.name = e->Name().toStdString();
		M.type = e->val("type").toStdString(); //Added by Arash, please check
		M.sensor_id = e->val("sensor").toStdString(); //Added by Arash, please check
		M.zn_controller_type = e->val("zn_controller_type").toStdString();
        foreach (QString key , e->codes())
			if (e->val(key).toQString() != "")
				M.set_val(key.toStdString(), e->val(key).toFloat());

//		M.interval = e->val("interval").toFloatDefaultUnit();
		if (tolower(M.type) == "ziegler-nichols") M.set_zn();
		Control.Controllers.push_back(M);
	}
}
void CMediumSet::g_get_environmental_params()
{
	SP.pos_def_limit = true;
	SP.negative_concentration_allowed = false;

	QList<Entity*> list;
	list.append(gw->entitiesByType("Climate settings"));
	list.append(gw->entitiesByType("Project settings"));
	list.append(gw->entitiesByType("Solver settings"));

    foreach (Entity *e , list)
        foreach (QString key , e->codes())
		{
			if (key == "path")
			{
				if (e->val(key) == ".")
					FI.outputpathname = gw->modelPathname().toStdString() + '/';
				//				FI.pathname = gw->modelPathname().toStdString() + '/';
				else
					FI.outputpathname = e->val(key).toStdString() + '/';
				//				FI.pathname = e->val(key).toStdString() + '/';
			}
//			if (key == "forward")
//				SP.forward = (e->val(key) == "Forward") ? 1 : 0;
			if (key == "solution_method")
				SP.solution_method = e->val(key).toStdString();
			if (key == "sorption")
				SP.sorption = e->val(key).toBool();
			if (key == "wiggle_tolerance")
				SP.wiggle_tolerance = e->val(key).toFloat();
			if (key == "max_j_update_interval")
				SP.max_J_interval = e->val(key).toInt();
			if (key == "dt")
				if (!e->val(key).isEmpty()) SP.dt = e->val(key).toFloat();
			if (key == "w")
				if (!e->val(key).isEmpty()) SP.w = e->val(key).toFloat();
			if (key == "tol")
				if (!e->val(key).isEmpty()) SP.tol = e->val(key).toFloat();
			if (key == "max_dt")
				if (!e->val(key).isEmpty()) SP.max_dt = e->val(key).toFloat();
			if (key == "uniformoutput")
				FI.uniformoutput = e->val(key).toBool();
			if (key == "nr_iteration_treshold_max")
				if (!e->val(key).isEmpty()) SP.nr_iteration_treshold_max = e->val(key).toInt();
			if (key == "nr_iteration_treshold_min")
				if (!e->val(key).isEmpty()) SP.nr_iteration_treshold_min = e->val(key).toInt();
			if (key == "dt_change_rate")
				if (!e->val(key).isEmpty()) SP.dt_change_rate = e->val(key).toFloat();
			if (key == "dt_change_failure")
				if (!e->val(key).isEmpty()) SP.dt_change_failure = e->val(key).toFloat();
			if (key == "nr_failure_criteria")
				SP.nr_failure_criteria = e->val(key).toInt();
			if (key == "write_details")
				FI.write_details = e->val(key).toBool();
			if (key == "mass_balance_check")
				SP.mass_balance_check = e->val(key).toBool();
			if (key == "colloid_transport")
				SP.colloid_transport = e->val(key).toBool();
			if (key == "water_quality")
				SP.constituent_transport = e->val(key).toBool();
			if (key == "epoch_limit")
				SP.epoch_limit = e->val(key).toInt();
			if (key == "avg_dt_limit")
				if (!e->val(key).isEmpty()) SP.avg_dt_limit = e->val(key).toFloat();
			if (key == "restore_interval")
				SP.restore_interval = e->val(key).toInt();
			if (key == "log_file")
				FI.log_file_name = FI.outputpathname + "log.txt"; // pathname + e->val(key).toStdString();
			if (key == "pe_info_filename")
				PE_info_filename = e->val(key).toStdString();
			if (key == "writeinterval")
				FI.write_interval = e->val(key).toInt();
			if (key == "pos_def_limit")
				SP.pos_def_limit = e->val(key).toBool();
			if (key == "negative_concentration_allowed")
				SP.negative_concentration_allowed = e->val(key).toBool();
			if (key == "steady_state_hydro" && !e->val(key).isEmpty())
				SP.steady_state_hydro = e->val(key).toBool();
			if (key == "check_oscillation" && !e->val(key).isEmpty())
				SP.check_oscillation = e->val(key).toBool();
			if (key == "maximum_run_time" && !e->val(key).isEmpty())
				SP.maximum_run_time = e->val(key).toFloat();
			if (key == "minimum_acceptable_negative_conc" && !e->val(key).isEmpty()) 
				SP.minimum_acceptable_negative_conc = e->val(key).toFloat();
		}

	/*		if (tolower(lid_config.keyword[i]) == "detout_obs_filename")
			{
			vector<string> names = split_curly_semicolon(lid_config.value[i]);
			for (int ii = 0; ii<names.size(); ii++)
			FI.detoutfilename_obs = names[ii];
			}*/
    if (SP.solution_method != "Direct Solution") SP.solution_method = "Partial Inverse Jacobian Evaluation";
	if (SP.constituent_transport) SP.colloid_transport = true;
	set_features.environmental_vars = true;
}
void CMedium::g_get_environmental_params()
{
	QList<Entity*> list;
	list.append(gw->entitiesByType("Climate settings"));
	list.append(gw->entitiesByType("Project settings"));
	list.append(gw->entitiesByType("Solver settings"));

    foreach (Entity *e , list)
        foreach (QString key , e->codes())
		{
			if (key == "time_min")
			{
                Timemin = e->val(key).toFloat(); Solution_State.t = Timemin;
			}
			if (key == "time_max")
				if (!e->val(key).isEmpty()) Timemax = e->val(key).toFloat();

			if (key == "precipitation"  && !e->val(key).isEmpty() && e->val(key) != ".")
				Precipitation_filename.push_back(fullFilename(e->val(key), gw->modelPathname()).toStdString());
			//			if (key == "evaporation") 
			//				if (e->val(key) != ".") Evaporation_filename.push_back(fullFilename(e->val(key), gw->modelPathname()).toStdString());

			if (key == "temperature" && !e->val(key).isEmpty() && e->val(key) != ".")
				temperature_filename.push_back(e->val(key).toFileName(gw->modelPathname()).toStdString());
			if (key == "light" && !e->val(key).isEmpty() && e->val(key) != ".")
				light_filename.push_back(e->val(key).toFileName(gw->modelPathname()).toStdString());
			if (key == "humidity" && !e->val(key).isEmpty() && e->val(key) != ".")
				r_humidity_filename.push_back(e->val(key).toFileName(gw->modelPathname()).toStdString());
			if (key == "wind" && !e->val(key).isEmpty() && e->val(key) != ".")
				wind_filename.push_back(e->val(key).toFileName(gw->modelPathname()).toStdString());
		}

	/*		if (tolower(lid_config.keyword[i]) == "detout_obs_filename")
	{
	vector<string> names = split_curly_semicolon(lid_config.value[i]);
	for (int ii = 0; ii<names.size(); ii++)
	FI.detoutfilename_obs = names[ii];
	}*/
	
	if (detoutfilename_hydro.size() == 0) detoutfilename_hydro = outputpathname() + "hydro_output_" + name + ".txt";
	if (detoutfilename_wq.size() == 0) detoutfilename_wq = outputpathname() +"wq_output_" + name + ".txt";
	if (detoutfilename_prtcle.size() == 0) detoutfilename_prtcle = outputpathname() + "prtcl_output_" + name + ".txt";
	if (detoutfilename_obs().size() == 0) detoutfilename_obs() = outputpathname() + "observed_output.txt";
	if (detoutfilename_control.size() == 0) detoutfilename_control = outputpathname() + "control_output_" + name + ".txt";

//	PE_info_filename() = "GA_info.txt";

}
void CMediumSet::g_get_params()
{
    foreach (Entity *e , gw->entitiesByType("Parameter"))
	{
		param_range P;
		P.low = 0;
		P.high = 0;
		P.fixed = false;
		P.log = false;
		P.applytoall = true;
		P.tempcorr = 1;
		P.name = e->Name().toStdString();
		P.low = e->val("low").toFloat();  //low range
		P.high = e->val("high").toFloat();
		P.value = e->val("value").toFloat();
		P.applytoall = (e->val("applytoall") == "Yes") ? 1 : 0;
		P.log = (e->val("log") == "Normal") ? 0 : 1;
		if (e->val("value") == "")
		{
			if (P.log == 0)
				P.value = 0.5*(P.low + P.high);
			else
				P.value = P.low / fabs(P.low)*sqrt(P.low*P.high);
		}
		parameters.push_back(P);
	}
	set_features.parameters = true;
}

#endif

#ifdef GIFMOD
void CMedium::g_get_model_configuration(runtimeWindow* rtw)
{
	QList <Node*> nodes = gw->Nodes();
	QStringList nodenames_sorted = gw->nodeNames();
	nodenames_sorted.sort();
//#pragma omp parallel for 
	for (int i = 0; i < nodenames_sorted.count(); i++)
	{
		Node* n = nodes[gw->nodeNames().indexOf(nodenames_sorted[i])];
		CMBBlock B;
		if (n->objectType.ObjectType == "Soil") { B.indicator = Block_types::Soil; } // 0
		if (n->objectType.ObjectType == "Pond") { B.indicator = Block_types::Pond; } //1
		if (n->objectType.ObjectType == "Storage") { B.indicator = Block_types::Storage; } //2
		if (n->objectType.ObjectType == "Catchment") { B.indicator = Block_types::Catchment; } //3
		if (n->objectType.ObjectType == "Manhole") { B.indicator = Block_types::Manhole; } //4
		if (n->objectType.ObjectType == "Darcy") { B.indicator = Block_types::Darcy; } //5
		if (n->objectType.ObjectType == "Stream") { B.indicator = Block_types::Stream; } //6
		if (n->objectType.ObjectType == "Plant") { B.indicator = Block_types::Plant; } //7
		B.set_val("a", n->val("a").toFloat());
		B.set_val("v", n->val("v").toFloat());

		if (B.V == 0) //&& (lookup(lid_config.param_names[i], "depth") != -1))
		{
			B.set_val("v", B.A*n->val("depth").toFloat());
		}

		QStringList codes = n->codes();
		codes.removeOne("a");
		codes.removeOne("v");
		if (codes.contains("theta_s"))
		{
			codes.removeOne("theta_s");
			codes.push_front("theta_s");
		}
        foreach (QString code , codes)
		{
			//			//qDebug() << code;
			if (!n->val(code).isEmpty() && n->val(code) != ".") B.set_val(code.toStdString(), n->val(code).toFloat());
			if (code == "inflow" && !n->val(code).isEmpty() && n->val(code) != ".")
				B.inflow_filename.push_back(fullFilename(n->val(code), gw->modelPathname()).toStdString());
			if (code == "id") 
				B.ID = n->val(code).toStdString();
			if (code == "buildup" && !n->val(code).isEmpty()) B.buildup_id.push_back(n->val(code).toStdString());
			if (code == "externalflux" && !n->val(code).isEmpty())
                foreach (QString flux , n->val(code).toQString().split(';'))
					B.envexchange_id.push_back(flux.trimmed().toStdString());
			if (code == "precipitation")
				B.precipitation_swch = n->val(code).toBool();
			if (code == "h_s_expression" && !n->val(code).isEmpty()) B.H_S_expression = convertstringtoStringOP(n->val(code),gw);
			if (code == "evapotranspiration")
				B.evaporation_id.push_back(n->val(code).toStdString());
			if (code == "light")
				B.light_swch = n->val(code).toBool();
			if (code == "perform_rxn")
				B.perform_rxn = n->val(code).toBool();
		}
        foreach (QString text , n->g().split(';'))
			B.set_val(text.split('=').first().toStdString(), text.split('=').last().toFloat());
        foreach (QString text , n->cg().split(';'))
			B.set_val(text.split('=').first().toStdString(), text.split('=').last().toFloat());
        foreach (QString text , n->planthsc().split(';'))
			B.set_val(text.split('=').first().toStdString(), text.split('=').last().toFloat());

		QStringList bulkDensity1Blocks = QStringList() << "Pond" << "Stream" << "Catchment";
		if (bulkDensity1Blocks.contains(n->objectType.ObjectType))
			B.set_val("bd", 1);
		
		QStringList depth1Blocks = QStringList() << "Pond" << "Stream" << "Catchment";
		if (bulkDensity1Blocks.contains(n->objectType.ObjectType))
			B.set_val("depth", 1);
		Blocks.push_back(B);

        foreach (QString code , n->codes())
		{
			if (gw->EntityNames("Parameter").contains(n->val(code).toQString()))
			{
				if (lookup_parameters(n->val(code).toStdString()) != -1) {
					parameters()[lookup_parameters(n->val(code).toStdString())].location.push_back(Blocks.size() - 1);  // Check for everything
					parameters()[lookup_parameters(n->val(code).toStdString())].conversion_factor.push_back(n->val(code).conversionCoefficient(n->val(code).unit, n->val(code).defaultUnit));
					parameters()[lookup_parameters(n->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters()[lookup_parameters(n->val(code).toStdString())].location_type.push_back(0);
					parameters()[lookup_parameters(n->val(code).toStdString())].experiment_id.push_back(name);
				}
			}
		}
        foreach (QString code , n->codes())
		{
			if (gw->EntityNames("Controller").contains(n->val(code).toQString()))
			{
				if (lookup_controllers(n->val(code).toStdString()) != -1) {
					controllers()[lookup_controllers(n->val(code).toStdString())].application_spec.location.push_back(Blocks.size() - 1);  // Check for everything
					controllers()[lookup_controllers(n->val(code).toStdString())].application_spec.conversion_factor.push_back(n->val(code).conversionCoefficient(n->val(code).unit, n->val(code).defaultUnit));
					controllers()[lookup_controllers(n->val(code).toStdString())].application_spec.quan.push_back(code.toStdString());
					controllers()[lookup_controllers(n->val(code).toStdString())].application_spec.location_type.push_back(0);
					controllers()[lookup_controllers(n->val(code).toStdString())].application_spec.experiment_id.push_back(name);
				}
			}
		}
	}
	QList <Edge*> edges = gw->Edges();
	QStringList edgenames_sorted = gw->edgeNames();
	edgenames_sorted.sort();
//#pragma omp parallel for
	for (int i = 0; i < edges.count(); i++)
	{
		Edge *e = edges[gw->edgeNames().indexOf(edgenames_sorted[i])];
		CConnection C;
		C.flow_params.resize(n_flow_params);
		C.ID = e->Name().toStdString();

		QString type = e->objectType.ObjectType;//connector type 

        foreach (QString code , e->codes())
			if (!e->val(code).isEmpty()) C.set_val(code.toStdString(), e->val(code).toFloat());  //sets all values except theta (theta was removed in input file)
		if (!e->val("flow_expression").isEmpty())
			C.flow_expression = CStringOP(e->val("flow_expression").toStdString());
		else
		{
			if (type == "Darcy") C.flow_expression = formulas()[QDarcy];
			if (type == "Pipe") C.flow_expression = CStringOP(formulas()[Pipe2]);
			if (type == "Normal Flow") C.flow_expression = CStringOP(formulas()[Normal]);
			if (type == "Rating Curve") C.flow_expression = CStringOP(formulas()[Rating_curve]);
		}
		if (!e->val("area_expression").isEmpty()) { C.area_expression = CStringOP(e->val("area_expression").toStdString()); C.const_area = false; }
		if (!e->val("dispersion_expression").isEmpty()) C.dispersion_expression = CStringOP(e->val("dispersion_expression").toStdString());
		C.Block1ID = e->sourceNode()->Name().toStdString();
		C.Block2ID = e->destNode()->Name().toStdString();

		if (e->val("settling") == "Yes") C.settling = 1;
		if (e->val("settling") == "No") C.settling = 0;

		if (e->objectType.ObjectType == "Prescribed flow")
			if (e->val("pre_flow_filename").toQString() != "")// || e->val("pre_flow_filename").toQString() != ".")
			{
				C.pre_flow_filename = e->val("pre_flow_filename").toFileName(gw->modelPathname()).toStdString();
				C.presc_flow = true;
			}
		if (e->objectType.ObjectType == "Controlled")
		{
			C.control = true;
			C.controller_id = e->val("contoller_id").toStdString();
		}

		if (!e->val("a").isEmpty() && e->val("a").toFloat() != 0) { C.const_area = true; }

        Connectors.push_back(C);
		//progress->setValue(progress->value() + 1);

        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code).toQString()))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
                    parameters()[lookup_parameters(e->val(code).toStdString())].location.push_back(Connectors.size() - 1);  // Check for everything
					parameters()[lookup_parameters(e->val(code).toStdString())].conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					parameters()[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters()[lookup_parameters(e->val(code).toStdString())].location_type.push_back(1);
					parameters()[lookup_parameters(e->val(code).toStdString())].experiment_id.push_back(name);
				}
			}
		}
        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Controller").contains(e->val(code).toQString()))
			{
				if (lookup_controllers(e->val(code).toStdString()) != -1) {
                    controllers()[lookup_controllers(e->val(code).toStdString())].application_spec.location.push_back(Connectors.size() - 1);  // Check for everything
					controllers()[lookup_controllers(e->val(code).toStdString())].application_spec.conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					controllers()[lookup_controllers(e->val(code).toStdString())].application_spec.quan.push_back(code.toStdString());
					controllers()[lookup_controllers(e->val(code).toStdString())].application_spec.location_type.push_back(1);
					controllers()[lookup_controllers(e->val(code).toStdString())].application_spec.experiment_id.push_back(name);
				}
			}
		}

	}
}
#endif
#ifdef GWA
	void CGWA::g_get_settings()
	{
		single_vz_delay = false;
		project_finish = 2040;
		project_interval = 1;
		project = false;
		fixed_old_tracer = false;

		QList<Entity*> list;
		list.append(gw->entitiesByType("Project settings"));
		list.append(gw->entitiesByType("Solver"));
		list.append(gw->entitiesByType("Markov Chain Monte Carlo"));
		for each (Entity *e in list)
			for each (QString key in e->codes())
			{
				if (key == "path")
				{
					if (e->val(key) == ".")
						pathname = gw->modelPathname().toStdString() + '/';
					else
						pathname = e->val(key).toStdString() + '/';
				}
				if (key == "outpath")
				{
					if (e->val(key) == ".")
						outpathname = gw->modelPathname().toStdString() + '/';
					else
						outpathname = e->val(key).toStdString() + '/';
				}
				if (key == "pe_info")
					PE_info_filename = e->val(key).toStdString();
				if (key == "detout")
					detoutfilename = e->val(key).toStdString();
				if (key == "realizedparam")
					realizedparamfilename = e->val(key).toStdString();
				if (key == "single_vz_delay")
					single_vz_delay = e->val(key).toBool();
				if (key == "fixed_old_tracer")
					fixed_old_tracer = e->val(key).toBool();
				if (key == "project_start")
				{
					project = true;
					project_start = e->val(key).toDouble();
				}
				if (key == "project_finish")
					project_finish = e->val(key).toDouble();
				if (key == "project_interval")
					project_interval = e->val(key).toDouble();


			}
		if (outpathname == "") outpathname = pathname;
	}

	void CGWA::g_get_model_configuration(runtimeWindow* progress)
	{

		for each(Node *n in gw->Nodes())
		{
			if (n->objectType.ObjectType == "Tracer")
			{
				CTracer T;
				T.co = 0;
				T.source = "";
				T.retard = 1;
				T.input_multiplier = 1;
				T.cm = 0;
				T.linear_prod = false;
				T.name = n->Name().toStdString();
				T.vz_delay = false;
				T.constant_input = false;
				T.input = CBTC();
				T.input.append(2000, 0);
				T.input.append(2010, 0);
				QStringList manual;
				manual << "constant_input" << "constant_input_val";
				for each (QString code in n->codes())
				{
					//			//qDebug() << code;
					if (!manual.contains(code) && !n->val(code).isEmpty() && n->val(code) != ".") T.set_val(code.toStdString(), n->val(code).toFloat());

					if (code == "constant_input_val" && n->val("constant_input").toBool())  T.set_val("constant_input", n->val("constant_input_val").toFloat());
					if (code == "input") T.input = CBTC(fullFilename(n->val(code), gw->modelPathname()).toStdString());
					if (code == "source")
						T.source = n->val(code).toStdString();
					if (code == "linear_prod") T.linear_prod = n->val(code).toBool();
					if (code == "fm")
						if (n->val(code).toBool())
							T.fm_max = 1;
						else
							T.fm_max = 0;

				}

				for each (QString code in n->codes())
				{
					if (gw->EntityNames("Parameter").contains(n->val(code).toQString()))
					{
						if (lookup_parameters(n->val(code).toStdString()) != -1) {
							parameters[lookup_parameters(n->val(code).toStdString())].location.push_back(T.name);  // Check for everything
							parameters[lookup_parameters(n->val(code).toStdString())].conversion_factor.push_back(n->val(code).conversionCoefficient(n->val(code).unit, n->val(code).defaultUnit));
							parameters[lookup_parameters(n->val(code).toStdString())].quan.push_back(code.toStdString());
							parameters[lookup_parameters(n->val(code).toStdString())].location_type.push_back(1);
						}
					}
				}


				Tracer.push_back(T);
			}
		}
		for (int i = 0; i < Tracer.size(); i++)
			if (Tracer[i].source != "")
				Tracer[i].SourceTr = tracers(Tracer[i].source);

		for each(Node *n in gw->Nodes())
		{
			if (n->objectType.ObjectType == "Well")
			{
				CWell W;
				W.name = n->Name().toStdString();
				W.vz_delay = 0;
				W.distribution = n->objectType.SubType.toStdString();
				W.params.resize(W.get_numparams(W.distribution));

				for each (QString code in n->codes())
				{
					if (!n->val(code).isEmpty() && n->val(code) != ".") W.set_val(code.toStdString(), n->val(code).toFloat());
				}

				for each (QString code in n->codes())
				{
					if (gw->EntityNames("Parameter").contains(n->val(code).toQString()))
					{
						if (lookup_parameters(n->val(code).toStdString()) != -1) {
							parameters[lookup_parameters(n->val(code).toStdString())].location.push_back(W.name);  // Check for everything
							parameters[lookup_parameters(n->val(code).toStdString())].conversion_factor.push_back(n->val(code).conversionCoefficient(n->val(code).unit, n->val(code).defaultUnit));
							parameters[lookup_parameters(n->val(code).toStdString())].quan.push_back(code.toStdString());
							parameters[lookup_parameters(n->val(code).toStdString())].location_type.push_back(0);
						}
					}
				}


				Well.push_back(W);
			}
		}

	}
	void CGWA::g_get_observed()
	{
		measured_quan.clear();
		QStringList stds;
		CBTCSet observed_data;
		for each (Entity *e in gw->entitiesByType("Observation"))
		{
			measured_chrc M;
			M.error_structure = 0;
			M.name = e->Name().toStdString();
			M.detect_limit = false;
			M.max_data_no = 1;
			M.count_max = 0;
			M.id = lookup_wells(e->val("well").toStdString());
			M.count_max = e->val("count_max").toInt();  //location id
			M.quan = lookup_tracers(e->val("tracer").toStdString());
			M.minstdev = e->val("minstdev").toFloat();

			QString std = e->val("std_no");
			if (std.isEmpty())
				std = e->Name() + "@" + e->val("well");
			if (!stds.contains(std))
				stds.append(std);
			M.std_no = stds.indexOf(std); // .toInt();

			M.error_structure = e->val("error_structure").toInt();
			string file = fullFilename(e->val("observed_data"), gw->modelPathname()).toStdString();
			CBTC observed = CBTC(file);
			if (observed.file_not_found)
				gw->newError(QString("Could not read observation data file %1 for %2").arg(QString::fromStdString(file)).arg(e->name));
			M.observed_data = observed;

			if (!e->val("detect_limit_value").isEmpty())
			{
				M.detect_limit_value = e->val("detect_limit_value").toFloat();
				M.detect_limit = true;
			}

			measured_quan.push_back(M);
		}

	}
#endif

#ifdef GIFMOD
void CMediumSet::g_get_particle_types()
{
	QList <Entity *> entities = gw->entitiesByType("Particle");
//#pragma omp parallel for
	for (int i = 0;i<entities.count();i++)
	{
		Entity *e = entities[i];
		QString model = e->getValue("SubType");// e->props.list["Model"];
		if (model == "Single phase") model = "single_phase";
		if (model == "Dual phase") model = "dual_phase_lm";
		if (model == "Triple phase") model = "triple_phase_lm";
		
		string _settling_model = e->val("settling_model").toStdString();// e->props.list["Model"];
		


		CSolid_Phase S(model.toStdString());
		if (tolower(_settling_model) == "constant velocity") S.settling_model = "constant_velocity";
		if (tolower(_settling_model) == "double exponential") S.settling_model = "double_exponential";
		S.set_settling_model(S.settling_model);
		S.name = e->Name().toStdString();
        foreach (QString key , e->codes())//props.list.keys())
		{
			if (!e->val(key).isEmpty()) S.set_val(key.toStdString(), e->val(key).toFloat());
		}
		Solid_phase.push_back(S);
        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code).toQString()))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(Solid_phase.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(7);
					parameters[lookup_parameters(e->val(code).toStdString())].experiment_id.push_back("");
				}
			}
		}
        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Controller").contains(e->val(code).toQString()))
			{
				if (lookup_controllers(e->val(code).toStdString()) != -1) {
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location.push_back(Solid_phase.size() - 1);  // Check for everything
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.quan.push_back(code.toStdString());
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location_type.push_back(7);
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.experiment_id.push_back("");
				}
			}
		}

	}
	set_features.solids = true;
}
void CMediumSet::g_get_constituents()
{
	QList <Entity *> entities = gw->entitiesByType("Constituent");
//#pragma omp parallel for
	for (int i = 0; i<entities.count(); i++)
	{
		Entity *e = entities[i];
		string _settling_model = e->val("settling_model").toStdString();// e->props.list["Model"];
		if (tolower(_settling_model) == "constant velocity") _settling_model = "constant_velocity";
		if (tolower(_settling_model) == "double exponential") _settling_model = "double_exponential";
		CConstituent S(_settling_model);
		S.name = e->Name().toStdString();
		S.exchange_rate_scale_factor = CStringOP(e->val("exchange_rate_factor").toStdString());
		if (e->val("mobile") != "") S.mobile = e->val("mobile").toBool();
		
		//S.diffusion = e->val("diffusion").toFloat(); 
        foreach (QString code , e->codes())
		{
			//qDebug() << code;
			if (!e->val(code).isEmpty()) S.set_val(code.toStdString(), e->val(code).toFloat());
		}

        foreach (SolidAqueousExchangeParameterItem	i , e->solidAqueousExchangeParameters)
		{
			string particle = (i.Particle == "Soil") ? "soil" : i.Particle.toStdString();
			if (i.PartitioningCoefficient != "")
			{
				S.capacity.push_back(i.PartitioningCoefficient.toDouble());
				S.capacity_ptr.push_back(particle);
			}
			if (i.ExchangeRate != "")
			{
				S.rate_exchange.push_back(i.ExchangeRate.toDouble());
				S.rate_exchange_ptr.push_back(particle);
			}
		}
		// to be added S.exchange_params.push_back(atof(lid_config.param_vals[i][j].c_str()));
		RXN.cons.push_back(S);

        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code).toQString()))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(RXN.cons.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(4);
					parameters[lookup_parameters(e->val(code).toStdString())].experiment_id.push_back("");
				}
			}
		}
        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Controller").contains(e->val(code).toQString()))
			{
				if (lookup_controllers(e->val(code).toStdString()) != -1) {
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location.push_back(RXN.cons.size() - 1);  // Check for everything
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.quan.push_back(code.toStdString());
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location_type.push_back(4);
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.experiment_id.push_back("");
				}
			}
		}

		set_features.constituents = true;
	}
}

void CMediumSet::g_get_reactions()
{
    foreach (Entity *e , gw->entitiesByType("Reaction parameter"))
	{
		rxparam rxparameter;
		rxparameter.tempcorr = 1;
		rxparameter.name = e->Name().toStdString();
		rxparameter.value = e->val("value").toFloat();
		if (!e->val("temerature_corrction").isEmpty()) rxparameter.tempcorr = e->val("temperature_correction").toFloat();
		else rxparameter.tempcorr = 1;
		RXN.parameters.push_back(rxparameter);

        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code).toQString()) && code!="Name")
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(RXN.parameters.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(e->name.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(3);
					parameters[lookup_parameters(e->val(code).toStdString())].experiment_id.push_back("");
				}
			}
		}
        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Controller").contains(e->val(code).toQString()))
			{
				if (lookup_controllers(e->val(code).toStdString()) != -1) {
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location.push_back(RXN.parameters.size() - 1);  // Check for everything
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.quan.push_back(code.toStdString());
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location_type.push_back(3);
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.experiment_id.push_back("");
				}
			}
		}

	}

	QList <Process *> processes = gw->Processes;
//#pragma omp parallel for
	for (int i = 0; i<processes.count(); i++)
	{
		Process *e = processes[i];
		CReaction Rx;
		Rx.name = e->name.toStdString(); //NAME
		Rx.rate = CStringOP(e->rateExpression().toStdString(), &RXN);
        foreach (QString key , e->productExpressions().keys())
			if (RXN.look_up_constituent_no(key.toStdString()) != -1)
			{
				Rx.products.push_back(RXN.look_up_constituent_no(key.toStdString()));
				Rx.prodrates.push_back(CStringOP(e->productExpressions()[key].split('=').last().toStdString(), &RXN));
				Rx.product_p_type.push_back(-2);
				Rx.product_phase.push_back(0);
			}
		RXN.Rxts.push_back(Rx);
	}	
	set_features.reactions = true;
}
void CMediumSet::g_get_buildup()
{
	QList <Entity *> entities = gw->entitiesByType("Build-up");
//#pragma omp parallel for
	for (int i = 0; i<entities.count(); i++)
	{
		Entity *e = entities[i];
		QString model = e->getValue("Model").toLower();// props.list["Model"].toLower();

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

        foreach (QString code , e->codes())
		{
			if (!e->val(code).isEmpty()) S.set_val(code.toStdString(), e->val(code).toFloat());
		}

		buildup.push_back(S);

        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code).toQString()))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(buildup.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(5);
					parameters[lookup_parameters(e->val(code).toStdString())].experiment_id.push_back("");
				}
			}
		}
        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Controller").contains(e->val(code).toQString()))
			{
				if (lookup_controllers(e->val(code).toStdString()) != -1) {
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location.push_back(buildup.size() - 1);  // Check for everything
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.quan.push_back(code.toStdString());
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location_type.push_back(5);
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.experiment_id.push_back("");
				}
			}
		}

	}
	set_features.buildup = true;
}
void CMediumSet::g_get_external_flux()
{
	QList <Entity *> entities = gw->entitiesByType("External flux");
//#pragma omp parallel for
	for (int i = 0; i<entities.count(); i++)
	{
		Entity *e = entities[i];
		QString model = e->getValue("Model").toLower();
		if (model == "free surface") model = "free_surface";
		if (model == "constant influx") model = "constant_influx";
		if (model == "constant rate") model = "constant_rate";

		CEnvExchange S(model.toStdString());
		S.name = e->Name().toStdString();

		S.constituent = e->val("constituent").toStdString();
		S.solid = e->val("solid").toStdString();

		if (e->val("expression") != "") S.expression = e->val("expression").toStdString();

        foreach (QString code , e->codes())
		{
			if (!e->val(code).isEmpty()) S.set_val(code.toStdString(), e->val(code).toFloat());
		}

		externalflux.push_back(S);

        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code).toQString()))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(externalflux.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(6);
					parameters[lookup_parameters(e->val(code).toStdString())].experiment_id.push_back("");
				}
			}
		}
        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Controller").contains(e->val(code).toQString()))
			{
				if (lookup_controllers(e->val(code).toStdString()) != -1) {
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location.push_back(externalflux.size() - 1);  // Check for everything
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.quan.push_back(code.toStdString());
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location_type.push_back(6);
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.experiment_id.push_back("");
				}
			}
		}

	}
	set_features.external_flux = true;
}

void CMediumSet::g_get_evapotranspiration()
{
	QList <Entity *> entities = gw->entitiesByType("Evapotranspiration");
//#pragma omp parallel for
	for (int i = 0; i<entities.count(); i++)
	{
		Entity *e = entities[i];
		QString model = e->getValue("SubType").toLower();
		if (model == "time series") model = "time_series";
		if (model == "transpiration based on moisture content") model = "transpiration_moisture_based";
		if (model == "transpiration tased on tatric totential") model = "transpiration_suction_based";


		CEvaporation S(model.toStdString());

		S.name = e->Name().toStdString();

		if (e->val("expression") != "") S.expression = e->val("expression").toStdString();
		if (e->val("time_series") != "") S.evaporation_filename = e->val("time_series").toFileName(gw->modelPathname()).toStdString();
		if (e->val("fao_56_kc_coefficient") != "") S.single_crop_coefficient_filename = e->val("fao_56_kc_coefficient").toFileName(gw->modelPathname()).toStdString();
		if (e->val("uptake_constituents") != "") S.uptake = e->val("uptake_constituents").toBool();

        foreach (QString code , e->codes())
		{
			if (!e->val(code).isEmpty()) S.set_val(code.toStdString(), e->val(code).toFloat());
		}
		evaporation_model.push_back(S);

        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Parameter").contains(e->val(code).toQString()))
			{
				if (lookup_parameters(e->val(code).toStdString()) != -1) {
					parameters[lookup_parameters(e->val(code).toStdString())].location.push_back(evaporation_model.size() - 1);  // Check for everything
					parameters[lookup_parameters(e->val(code).toStdString())].conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					parameters[lookup_parameters(e->val(code).toStdString())].quan.push_back(code.toStdString());
					parameters[lookup_parameters(e->val(code).toStdString())].location_type.push_back(7);
					parameters[lookup_parameters(e->val(code).toStdString())].experiment_id.push_back("");
				}
			}
		}
        foreach (QString code , e->codes())
		{
			if (gw->EntityNames("Controller").contains(e->val(code).toQString()))
			{
				if (lookup_controllers(e->val(code).toStdString()) != -1) {
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location.push_back(evaporation_model.size() - 1);  // Check for everything
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.conversion_factor.push_back(e->val(code).conversionCoefficient(e->val(code).unit, e->val(code).defaultUnit));
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.quan.push_back(code.toStdString());
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.location_type.push_back(7);
					Control.Controllers[lookup_controllers(e->val(code).toStdString())].application_spec.experiment_id.push_back("");
				}
			}
		}

	}
	set_features.evaporation = true;
}

void CMedium::g_set_default_connector_expressions()
{

    for (int ii = 0; ii<Connectors.size(); ii++)
	{
        if (Connectors[ii].flow_expression.terms.size() == 0)
		{

            if (Blocks[getblocksq(Connectors[ii].Block1ID)].z0 >= Blocks[getblocksq(Connectors[ii].Block2ID)].z0)
			{
                if (vaporTransport()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator] == true)
				{
                    Connectors[ii].flow_expression = CStringOP(formulasQ()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator] + "+" + formulas()[Vapor]);
                    Connectors[ii].flow_expression_strng = formulasQ()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator] + "+" + formulas()[Vapor];

				}
				else
				{
                    Connectors[ii].flow_expression = CStringOP(formulasQ()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator]);
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
				{
                    Connectors[ii].flow_expression = CStringOP(formulasQ2()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator]);
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

    for (int ii = 0; ii < Connectors.size(); ii++)
	{
        Connectors[ii].area_expression = formulasA()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator];
        Connectors[ii].area_expression_strng = formulasA()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator];
        if (Connectors[ii].A == 0)
            Connectors[ii].const_area = const_area()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator];
		else
            Connectors[ii].const_area = true;
	}

    for (int ii = 0; ii < Connectors.size(); ii++)
	{
        if (Connectors[ii].settling == -1)
		{
            Connectors[ii].settling = settling()[Blocks[getblocksq(Connectors[ii].Block1ID)].indicator][Blocks[getblocksq(Connectors[ii].Block2ID)].indicator];
		}
	}


    for (int i = 0; i<Connectors.size(); i++)
	{
        for (int j = 0; j< Solid_phase().size(); j++)Connectors[i].Solid_phase_id.push_back(j);
        Connectors[i].Block1 = &Blocks[getblocksq(Connectors[i].Block1ID)];
        Connectors[i].Block2 = &Blocks[getblocksq(Connectors[i].Block2ID)];
		Connectors[i].Block1N = getblocksq(Connectors[i].Block1ID);
		Connectors[i].Block2N = getblocksq(Connectors[i].Block2ID);
        Blocks[getblocksq(Connectors[i].Block1ID)].connectors.push_back(i);
        Blocks[getblocksq(Connectors[i].Block1ID)].connectors_se.push_back(0);
        Blocks[getblocksq(Connectors[i].Block2ID)].connectors.push_back(i);
        Blocks[getblocksq(Connectors[i].Block2ID)].connectors_se.push_back(1);
        for (int ii = 0; ii<Connectors[i].Solid_phase_id.size(); ii++) Connectors[i].Solid_phase.push_back(&(Solid_phase()[Connectors[i].Solid_phase_id[ii]]));

	}
}

void CMedium::g_set_default_block_expressions()
{
	for (int ii = 0; ii<Blocks.size(); ii++)
	{
		if (Blocks[ii].H_S_expression.terms.size() == 0)
		{
			Blocks[ii].H_S_expression = CStringOP(formulasH()[Blocks[getblocksq(Blocks[ii].ID)].indicator]); Blocks[ii].H_S_expression_txt = formulasH()[Blocks[getblocksq(Blocks[ii].ID)].indicator];
		}
	}
}
void CMediumSet::solve(runtimeWindow *rtw)
{
	failed = false;
	if (FI.write_details)
	{
		FILE *FILEBTC = fopen((FI.outputpathname + "Solution_details_" + ID + ".txt").c_str(), "w");
		fclose(FILEBTC);
	}
	failed = false;
	ANS_hyd.clear();
	ANS_colloids.clear();
	ANS_constituents.clear();
	ANS_control.clear();
	for (int i = 0; i < Medium.size(); i++)
	{
		if (rtw)
			rtw->setExperiment( &Medium[i]);
		Medium[i].runtimewindow = rtw;
		gw->log(QString("Solving %1").arg(QString::fromStdString(Medium[i].name)));
		Medium[i].solve();
        failed = failed || Medium[i].Solution_State.failed;
        ANS_hyd.push_back(&Medium[i].Results.ANS);
        ANS_colloids.push_back(&Medium[i].Results.ANS_colloids);
        ANS_constituents.push_back(&Medium[i].Results.ANS_constituents);
        ANS_colloids.push_back(&Medium[i].Results.ANS_control);
        if (Medium[i].Solution_State.failed)
            gw->log(QString("%1 failed, (%2)").arg(QString::fromStdString(Medium[i].name)).arg(QString::fromStdString(Medium[i].Solution_State.fail_reason)));
		else
			gw->log(QString("%1 finished").arg(QString::fromStdString(Medium[i].name)));

	}

	ANS_obs = CBTCSet(measured_quan.size());
	MSE_obs.clear(); 
	for (int i = 0; i < measured_quan.size(); i++)
	{
		if (lookup_medium(measured_quan[i].experiment) != -1)
		{
            ANS_obs.BTC[i] = Medium[lookup_medium(measured_quan[i].experiment)].Results.ANS_obs.BTC[i];
			ANS_obs.setname(i, measured_quan[i].name);
			calc_MSE(i);
		}
		else
			gw->log(QString::fromStdString(measured_quan[i].experiment) + " does not exist!");
	}
	CVector(MSE_obs).writetofile(FI.outputpathname + "MSE.txt");
	gw->log("Simulation ended.");
	QMessageBox::StandardButton reply;
	QMessageBox::question(rtw, "Simulation Ended", "Simulation Finished!", QMessageBox::Ok);
}



/*
bool CMedium::solve(runtimeWindow *rtw)
{
	evaluate_functions();
	setH();	// Set Total Head for all blocks
	evaluate_area(true);  //Update connector areas; 
	setQ();	//Set Q for all connectors
	initialize();

	ANS_obs = CBTCSet(measured_quan().size());

	ANS_obs.names.clear();
	for (int i = 0; i<measured_quan().size(); i++) ANS_obs.names.push_back(measured_quan()[i].name);

	if (solution_method() == 0)
		solve_fts_m2(dt());   //fts= fixed time steps

	return true;
}
*/
void CMedium::g_load_inflows()
{
	for (int j = 0; j < evaporation_model().size(); j++)
	{
		if (evaporation_model()[j].evaporation_filename != "")
		{
			CBTC E = CBTC(evaporation_model()[j].evaporation_filename);
			if (E.n>0) evaporation_model()[j].evaporation_TS = E;
		}

		if (evaporation_model()[j].single_crop_coefficient_filename != "")
		{
			CBTC E = CBTC(evaporation_model()[j].single_crop_coefficient_filename);
			if (E.n>0) evaporation_model()[j].single_crop_coefficient = E;
		}
	}
	for (int j = 0; j<temperature_filename.size(); j++)
	{
		CBTC T = CBTC(temperature_filename[j]);
		if (T.n>0) temperature.push_back(T);
	}

	for (int j = 0; j<r_humidity_filename.size(); j++)
	{
		CBTC T = CBTC(r_humidity_filename[j]);
		if (T.n>0) r_humidity.push_back(T);
	}

	for (int j = 0; j<wind_filename.size(); j++)
	{
		CBTC T = CBTC(wind_filename[j]);
		if (T.n>0) wind.push_back(T);
	}

	for (int j = 0; j<light_filename.size(); j++)
	{
		CBTC L = CBTC(light_filename[j]);
		if (L.n>0) light.push_back(L);
	}
//ask arash
//	for (int i = 0; i<light.size(); i++)
//		light[i] = light[i].make_uniform(dt);
//	for (int i = 0; i<temperature.size(); i++)
//		temperature[i] = temperature[i].make_uniform(dt);
	for (int j = 0; j < Precipitation_filename.size(); j++)
	{
		CPrecipitation P = CPrecipitation(Precipitation_filename[j]);
		if (P.n>0) 
			Precipitation.push_back(P);
		else
			warning(QString("Precipitation file:%1 is not valid.").arg(QString::fromStdString(Precipitation_filename[j])));
	}
//	for (int j = 0; j < Evaporation_filename.size(); j++)
//	{
//		CBTC E = CBTC(Evaporation_filename[j]);
//		if (E.n>0) Evaporation.push_back(E);
//	}

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

		
		{
			if (Blocks[i].precipitation_swch == true)
				for (int j = 0; j<Precipitation.size(); j++)
					Blocks[i].inflow.push_back(Precipitation[j].getflow(Blocks[i].A,1.0/24.0/4));
		}
	}

    for (int i = 0; i<Connectors.size(); i++)
	{
        if (Connectors[i].pre_flow_filename != "") {
            Connectors[i].presc_flow = true; Connectors[i].presc_flowrate = CBTC(Connectors[i].pre_flow_filename);
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

	return get_member_no(getblocksq(block_name.toStdString()), solid_id, phase_no);
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
	
	const_no = RXN().look_up_constituent_no(const_name.toStdString());
	
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
		vars["mode"] = "forward";
		if (finished)
		{
			vars["progress"] = 100;
			vars["finished"] = true;
		}
		else
		{
			int progress;
            progress = 100.0*(Solution_State.t - Timemin) / (Timemax - Timemin);
            vars["t"] = Solution_State.t;
			vars["progress"] = progress;
			vars["dtt"] = Solution_State.dtt;
            vars["epoch count"] = Solution_State.epoch_count;
            QString reason = QString::fromStdString(Solution_State.fail_reason);
            ////qDebug() << reason;
			if (!reason.toLower().contains("none"))
				vars["label"] = reason;
            ////qDebug()<< t<<dtt;
			
			if (runtimewindow->sln_dtl_active)
				if (!reason.toLower().contains("none"))
                    runtimewindow->slndetails_append(QString::number(Solution_State.epoch_count) + ":" + reason + ", time step size: " + QString::number(Solution_State.dtt));
		}
		runtimewindow->update(vars);
		if (finished)
		{
			//QMessageBox::StandardButton reply;
			//QMessageBox::question(runtimewindow, "Simulation Ended", "Simulation Finished!", QMessageBox::Ok);
		}
	}
}
#endif

CBTC::CBTC(double a, double b, const vector<double> &x)
{
	int n = x.size();
	vector<double> y(n);
	for (int i = 0; i < n; i++)
		y[i] = a + b*x[i];
	*this = CBTC(x,y);
}
CBTC::CBTC(double a, double b, const CBTC &btc)
{
	CBTC(a, b, btc.t);
}

map<string, double> regression(vector<double> x, vector<double> y)
{
	map<string, double> reg;
	double xbar = 0;
	double ybar = 0;
	double sumxerr = 0;
	double sumyerr = 0;
	double sumxerryerr = 0;
	double sumxerr2 = 0;
	double sumxy = 0;
	double sumx = 0;
	double sumy = 0;
	double sumx2 = 0;
	double sumy2 = 0;

	int n = x.size();
	if (x.size() != y.size() || x.size() == 0 || y.size() == 0)
	{
		reg["error"] = 1;
		return reg;
	}

	for (int i = 0; i < n; i++)
	{
		sumx += x[i];
		sumy += y[i];
		sumxy += x[i] * y[i];
		sumx2 += x[i] * x[i];
		sumy2 += y[i] * y[i];
	}
	xbar += sumx / n;
	ybar += sumy / n;

	for (int i = 0; i < n; i++)
	{
		sumxerryerr += (x[i] - xbar)*(y[i] - ybar);
		sumxerr += x[i] - xbar;
		sumyerr += y[i] - ybar;
		sumxerr2 += (x[i] - xbar)*(x[i] - xbar);
	}
	double b = sumxerryerr / sumxerr2;
	double a = ybar - b*xbar;
	double r2 = (n*sumxy - sumx*sumy) *(n*sumxy - sumx*sumy) / ((n*sumx2 - (sumx*sumx))*(n*sumy2 - (sumy*sumy))); //coefficient of determination 
	string A = "a";
	reg["a"] = a;
	reg["b"] = b;
	reg["r2"] = r2;
	reg["error"] = 0;
	return reg;
}
CBTC::CBTC(const vector<double> &t, const vector<double> &C)
{
	if (t.size() != C.size()) return;
	n = t.size();
	structured = true;
	this->t = t;
	this->C = C;
	if (n > 2) for (int i = 2; i < n; i++)
		if ((t[i] - t[i - 1]) != (t[i - 1] - t[i - 2]))structured = false;
}
/*CBTC &CBTCSet::operator[](int index)
{
	if (index < 0 || index >= BTC.size())
		return CBTC();
	return BTC[index];
}
CBTC &CBTCSet::operator[](string BTCName)
{
	return operator[](indexOf(BTCName));
}*/
int CBTCSet::indexOf(const string& name) const
{
	for (int i = 0; i < BTC.size(); i++)
		if (name == BTC[i].name) 
			return i;
	return -1;
}
std::string convertstringtoStringOP(const QString& s, GraphWidget *gWidget)
{
	QStringList Constituents = gWidget->EntityNames("Constituent");
	QStringList Parameters = gWidget->EntityNames("Reaction parameter");
	QStringList Physicals = gWidget->PhysicalCharacteristicsList;
	QStringList Functions = gWidget->functionList;
	QString d;
	int l = 0, r = s.length();
	for (int i = 0; i < s.length();) {
		if (s[i].isSpace()) {
			i++;  continue;
		}
		l = i;
		// the first char must start with a letter or underscore
		if (s[i].isLetter() || s[i] == QChar('_')) {
			while (i < s.length() && (s[i].isLetter() || s[i] == QChar('_') || s[i].isNumber())) i++;
		}
		r = i;
		if (l == r) {
			d.append(s[i]); i++;
		}
		else {
			QString var = s.mid(l, r - l);
			if (Constituents.contains(var))
				d.append(QString("c[%1]").arg(var));
			else if (Parameters.contains(var))
				d.append(QString("p[%1]").arg(var));
			else if (Functions.contains(var))
				d.append(QString("f[%1]").arg(var));
			else if (Physicals.contains(var))
				d.append(QString("f[%1]").arg(var)); // ASK
			else
				d.append(var);
		}
	}
	return d.toStdString();
}

#ifdef GWA	
void MainWindow::forwardRun(CGWASet *model, runtimeWindow *runtimewindow)
{
	CGWASet *system = model;
//	if (system->inverse == false)
	{
//		system->doModel();
		system->operator()().getmodeled();
		system->operator()().modeled.writetofile(system->operator()().outpathname + "modeled.txt");
		if (system->operator()().project == true) {
			//system->Do_project();
			system->operator()().projected.writetofile(system->operator()().outpathname + "projected.txt");
			mainGraphWidget->results->projected = system->operator()().projected;
		}
		model->operator()().ANS_obs = model->operator()().modeled;
		mainGraphWidget->results->ANS_obs = model->operator()().ANS_obs;
	}
}

void MainWindow::inverseRun(CGWASet *model, runtimeWindow* progress)
{
	CGWASet system = *model;
	if (system().inverse == true)
	{
		clock_t t0, t1;
		t0 = clock();

		ofstream runtime_file;
		runtime_file.open(system().outpathname + "Runing_times.txt");
		CGA GA(&system, progress); //initialize GA
		mainGraphWidget->results = new Results;
		//**************************************** Genetic ****************************************
		if (GA.justreadmcmc == false && GA.continue_mcmc == false)
		{
			progress->setLabel("Deterministic Parameter Estimation");
			if (GA.getfromfilename == "")
			{
				GA.optimize(progress);

				// Genetic run time :
				t1 = clock() - t0;
				float run_time = ((float)t1) / CLOCKS_PER_SEC;
				string st = " sec";
				if (run_time >= 60) { run_time /= 60; st = " min"; }
				if (run_time >= 60) { run_time /= 60; st = " hr"; }

				runtime_file << " Genetic Run Time :  " << run_time << st << endl;
			}
			else
			{
				CVector out(1);
				model->operator()().gw->log(QString("Loading GA data from: %1").arg(QString::fromStdString(GA.getfromfilename)));
				model->operator()().gw->logW->writetotempfile();
				out[0] = GA.getfromoutput(GA.getfromfilename);
				//				out[0] = GA.getfromoutput(system.outpathname + GA.getfromfilename);
				out.writetofile(system().outpathname + "likelihood.txt");
			}

			//			GA.Sys_out.modeled.writetofile(system.outpathname + GA.Sys.detoutfilename);
			GA.Sys_out.Medium[0].modeled.writetofile(system().outpathname + "detout.txt");
			if (GA.Sys_out.Medium[0].project == true)
			{
				GA.Sys_out.Medium[0].projected.writetofile(system().outpathname + "projected.txt");
			}


			progress->setLabel("Updating Model Parameter Values");
			mainGraphWidget->trackingUndo = false;
			QList<Entity*> GUIparameters = mainGraphWidget->entitiesByType("Parameter");

			for (int i = 0; i < GUIparameters.size(); i++)
				GUIparameters[i]->setValue("Value", QString::number(GA.final_params[GA.Sys.Medium[0].lookup_parameters(GUIparameters[i]->name.toStdString())]));
			mainGraphWidget->trackingUndo = true;
			mainGraphWidget->changedState = true;

			if (mainGraphWidget->model->modeled.nvars == 0)
			{
				delete mainGraphWidget->model;
				mainGraphWidget->model = new CGWA(GA.Sys_out.Medium[0]);
			}

			mainGraphWidget->results->ANS_obs = GA.Sys_out.Medium[0].modeled;
			mainGraphWidget->results->projected = GA.Sys_out.Medium[0].projected;

		}
//		if (mainGraphWidget->model != 0) delete mainGraphWidget->model;
//		mainGraphWidget->model = new CGWA(GA.Sys_out);
//		mainGraphWidget->trackingUndo = true;
		//**************************************** local sensitivity ****************************************
		t0 = clock();
		//qDebug() << "10";
		CMCMC MCMC(GA);
		int mcmcstart = MCMC.n_chains;
		if (GA.justreadmcmc == false)
		{
			progress->setLabel("Local Sensitivity Analysis");
			if (MCMC.continue_mcmc == false)
				MCMC.initialize(GA.final_params);
			else
			{
				model->operator()().gw->log(QString("Loading MCMC data from: %1").arg(QString::fromStdString(MCMC.outputfilename)));
				model->operator()().gw->logW->writetotempfile();
				mcmcstart = MCMC.readfromfile(MCMC.outputfilename);
				//			mcmcstart = MCMC.readfromfile(system.outpathname + MCMC.outputfilename);
			}

			if (GA.sens_out == true && GA.continue_mcmc==false)
			{
				//CMatrix S_mat = MCMC.sensitivity_mat(GA.dp_sens, GA.final_params);
				//S_mat.writetofile(system.outpathname + "sensitivity_mat.txt");
				CMatrix S_mat_lumped = MCMC.sensitivity_mat_lumped(GA.dp_sens, GA.final_params);
				S_mat_lumped.writetofile(system().pathname+"sensitivity_mat_lumped.txt");
				mainGraphWidget->results->localSensitivityMatrix = S_mat_lumped;

			}
		}

		//**************************************** MCMC ****************************************
		if (GA.mcmc_run)
		{
			//qDebug() << "20";
			CBTCSet MCMCOut;
			bool haveMCMCOut = false;
			if (GA.justreadmcmc == false)
			{
				//progress = new runtimeWindow(mainGraphWidget, "MCMC");
				//progress->show();
				progress->setMode("MCMC");
				progress->setLabel("Stochastic Parameter Estimation");
				//			MCMC.step(mcmcstart, int((MCMC.nsamples - mcmcstart) / MCMC.n_chains)*MCMC.n_chains, system.outpathname + MCMC.outputfilename, progress);
				//qDebug() << 21;
				MCMC.step(mcmcstart, int((MCMC.nsamples - mcmcstart) / MCMC.n_chains)*MCMC.n_chains, MCMC.outputfilename, progress);
				progress->setLabel("Stochastic Parameter Estimation done");
				//qDebug() << 22;
				MCMCOut = CBTCSet(MCMC.Params, MCMC.writeinterval);
				for (int i = 0; i < GA.Sys.parameters.size(); i++)
					MCMCOut.setname(i, GA.Sys.parameters[i].name);
//				MCMCOut.writetofile(MCMC.outputfilename + "original");
//				CBTC(MCMC.logp).writefile(MCMC.outputfilename + "logp");
//				CBTC(MCMC.logp1).writefile(MCMC.outputfilename + "logp1");
				//qDebug() << 23;
				MCMCOut.append(CBTC(MCMC.logp, MCMC.writeinterval), "Posterior Probability");
				//qDebug() << 24;
				MCMCOut.append(CBTC(MCMC.logp1, MCMC.writeinterval), "Posterior Probability1");
				//qDebug() << 25;
				haveMCMCOut = true;
	//			MCMCOut.writetofile(MCMC.outputfilename + "combined");
				// MCMC run time :
				t1 = clock() - t0;
				float run_time = ((float)t1) / CLOCKS_PER_SEC;
				string st = " sec";
				if (run_time >= 60) { run_time /= 60; st = " min"; }
				if (run_time >= 60) { run_time /= 60; st = " hr"; }

				runtime_file << " MCMC Run Time :  " << run_time << st << endl;
			}
			//qDebug() << "30";
			if (!haveMCMCOut)
			{
				model->operator()().gw->log(QString("Loading MCMC data from: %1").arg(QString::fromStdString(MCMC.outputfilename)));
				model->operator()().gw->logW->writetotempfile();
				MCMCOut = CBTCSet(MCMC.outputfilename, false);
				MCMCOut.names = MCMC.paramname;
			}
			//		CBTCSet MCMCOut(system.outpathname + MCMC.outputfilename, false);
			//qDebug() << "31";
			//qDebug() << "32";

			//CBTCSet MCMC_red = MCMCOut.Extract(11);
			//MCMC_red.names=MCMCOut.names;
			//MCMC_red.writetofile_unif(system.pathname+"mcmc_red.txt");

			if (GA.calc_distributions == true)
			{
				//qDebug() << "33" << GA.no_bins << MCMC.paramname.size() << MCMC.n_burnout;
				progress->setLabel("Calculating Distributions");

				CBTCSet posterior_distribution = MCMCOut.distribution(GA.no_bins, MCMC.paramname.size(), MCMC.n_burnout);
				//qDebug() << 33.1;
				posterior_distribution.names = MCMC.paramname;
				//qDebug() << 33.2;
				posterior_distribution.writetofile(system().outpathname + "posterior_distribution.txt");
				//qDebug() << "34";

				mainGraphWidget->results->posteriors = posterior_distribution;
				//qDebug() << "35";

				CBTCSet prior_distribution = MCMC.prior_distribution(GA.no_bins * 10);
				prior_distribution.names = MCMC.paramname;
				prior_distribution.writetofile(system().outpathname + "prior_distribution.txt");
				//qDebug() << "36";

				mainGraphWidget->results->priors = prior_distribution;
			}



			//********************************* MCMC Percentile **************************************
			//qDebug() << "40";

			t0 = clock();
			char buffer[33];

			if (GA.calculate_percentile == true)
			{
				progress->setLabel("Calculating Percentiles");
				//qDebug() << 41;
				CBTCSet sorted = MCMCOut.sort(MCMC.n_burnout);
				//qDebug() << 41.05;

				vector<double> p2dot5 = sorted.percentile(0.025, 0);
				//qDebug() << 41.1;
				vector<double> p50 = sorted.percentile(0.5, 0);
				//qDebug() << 41.2;
				vector<double> mean = sorted.mean(0);
				//qDebug() << 41.3;
				vector<double> std = sorted.std(0);
				//qDebug() << 41.4;
				vector<double> p97dot5 = sorted.percentile(0.975, 0);
				//qDebug() << 42.5;
				FILE *filepercentile;
				filepercentile = fopen((system().outpathname + "percentiles.txt").c_str(), "w");
				//qDebug() << 43;
				for (int i = 0; i < MCMCOut.names.size(); i++)
					fprintf(filepercentile, "%s, ", MCMCOut.names[i].c_str());
				//qDebug() << 44;
				fprintf(filepercentile, "\n");

				CVector(p2dot5).writetofile(filepercentile);
				CVector(p50).writetofile(filepercentile);
				CVector(p97dot5).writetofile(filepercentile);
				CVector(mean).writetofile(filepercentile);
				CVector(std).writetofile(filepercentile);
				//qDebug() << 45;
				double mean_log = mean[MCMCOut.nvars - 4];
				double mean_likelihood = MCMCOut.BTC[MCMCOut.nvars - 4].Exp().average();
				fprintf(filepercentile, "mean log: %e\n", 2 * mean_log);
				//qDebug() << 46;
				double logp_mean = -GA.assignfitnesses(mean);
				fprintf(filepercentile, "f(mean): %e\n", 2 * logp_mean);

				double DIC = 4 * mean_log - 2 * logp_mean;
				fprintf(filepercentile, "DIC: %e\n", DIC);

				fclose(filepercentile);
				//qDebug() << 47;
				int n = MCMCOut.names.size();
				mainGraphWidget->results->percentiles.resize(n);
				//qDebug() << 48;
				for (int i = 0; i < n; i++)
				{
					mainGraphWidget->results->percentiles[i].parameter = MCMCOut.names[i];
					mainGraphWidget->results->percentiles[i].p25 = p2dot5[i];
					mainGraphWidget->results->percentiles[i].p50 = p50[i];
					mainGraphWidget->results->percentiles[i].mean = mean[i];
					mainGraphWidget->results->percentiles[i].p975 = p97dot5[i];
					mainGraphWidget->results->percentiles[i].std = std[i];
				}

				//********************************* MCMC mean out **************************************
				//qDebug() << 49;
				double out1 = GA.assignfitnesses(mean);

				GA.Sys_out.Medium[0].modeled.writetofile(system().outpathname + "BTC_mean.txt");

			}

			//********************************* MCMC Correlation **************************************

			if (GA.calculate_correlation == true)
			{
				//qDebug() << 49.5;
				CMatrix correlation_mat = MCMCOut.correlation(MCMC.n_burnout, MCMC.MCMCParam.size());
				correlation_mat.writetofile(system().outpathname + "correlation_mat.txt");
				mainGraphWidget->results->correlationMatrix = correlation_mat;
			}

			//********************************* MCMC realizations out **************************************
			//qDebug() << "50";

			if (GA.mcmc_realization == true)
			{
				progress->setLabel("Generating Posterior Realizations");
				vector<CBTCSet> modeled_samples; //each contain a measured quantity
				vector<CBTCSet> projected_samples; //each contain a tracer in a well
				//vector<CBTCSet> BTCout_obs_noise; //each contain a tracer in a well
				vector<CBTCSet> modeled_prcntl;  //each contain a tracer in a measured quantity
				CBTCSet modeled_prcntl_all(3);
				vector<CBTCSet> projected_prcntle;  //each contain a tracer in a well
				//vector<CBTCSet> BTCout_obs_prcntle_noise;
				vector<CBTCSet> Age_dist;
				vector<CBTCSet> Age_dist_prcntle;
				vector<CBTCSet> Age_dist_cum;
				vector<CBTCSet> Age_dist_cum_prcntle;

				int n_BTCout_obs = system().measured_quan.size();

				modeled_samples.resize(n_BTCout_obs);
				modeled_prcntl.resize(n_BTCout_obs);
				Age_dist.resize(system().Well.size());
				Age_dist_prcntle.resize(system().Well.size());
				Age_dist_cum.resize(system().Well.size());
				Age_dist_cum_prcntle.resize(system().Well.size());
				projected_samples.resize(system().Well.size()*system().Tracer.size());
				projected_prcntle.resize(system().Well.size()*system().Tracer.size());


				for (int i = 0; i < system().Well.size(); i++)
				{
					Age_dist[i] = CBTCSet(GA.nrealizations);
					Age_dist_cum[i] = CBTCSet(GA.nrealizations);
				}

				for (int i = 0; i < n_BTCout_obs; i++)
				{
					modeled_prcntl[i] = CBTCSet(GA.nrealizations);
					modeled_samples[i] = CBTCSet(GA.nrealizations);
				}

				for (int i = 0; i < system().Well.size()*system().Tracer.size(); i++)
				{
					projected_samples[i] = CBTCSet(GA.nrealizations);
					projected_prcntle[i] = CBTCSet(GA.nrealizations);
				}


				CBTCSet paramsList(MCMCOut.nvars);// , GA.nrealizations); // samples drawn from the posterior distribution
				paramsList.names = MCMCOut.names;
				vector<CMatrix> global_sens_mat;
				vector<CMatrix> global_sens_lumped;
				global_sens_lumped.resize(GA.nrealizations);
				CBTCSet R2_t(system().Tracer.size());
				CBTC R2_overall;
				CBTCSet R2_t_ln(system().Tracer.size());
				CBTC R2_overall_ln;
				CBTCSet all_realizations(2);
				CBTCSet all_realizations_t(system().Tracer.size());
				CBTCSet all_realizations_m(system().Tracer.size());

				omp_set_num_threads(MCMC.numberOfThreads);
				int bucket = MCMC.numberOfThreads * 2;
				vector<vector<double>> param(GA.nrealizations);

				vector<CBTCSet> tracers(GA.nrealizations);
				vector<CBTCSet> tracers_m(GA.nrealizations);

				CBTCSet overall(2);

				for (int samp = 0; samp < GA.nrealizations; samp += bucket)
				{
					QMap<QString, QVariant> vars;
					vars["mode"] = "MCMC";
					double progressPercent = double(samp) / double(GA.nrealizations)*95.0 + 5;
					vars["progress"] = progressPercent;
					progress->update(vars);

					vector<CGWA> sys;
					int endfor = min(bucket, GA.nrealizations - samp);
					sys.resize(endfor);

					for (int thread = 0; thread < endfor; thread++)
					{
						sys[thread] = GA.Sys();
						param[thread + samp] = MCMCOut.getrandom(MCMC.n_burnout);
						paramsList.append(samp + thread, param[thread + samp]);
						sys[thread].setparams(param[thread + samp], GA.nParam);
						tracers[thread + samp] = CBTCSet(system().Tracer.size());
						tracers_m[thread + samp] = CBTCSet(system().Tracer.size());
					}


#pragma omp parallel for

					for (int thread = 0; thread < endfor; thread++)
					{
						//qDebug() << "samp = " << samp + thread << thread << "started";
						cout << "Realization Sample No. : " << samp + thread << endl;

						sys[thread].doModel();
						//qDebug() << "samp = " << samp + thread << thread << "done";

#pragma omp critical
						if (MCMC.global_sensitivity == true)
						{
							//global_sens_mat.push_back(MCMC.sensitivity_mat(GA.dp_sens, param[thread + samp]));
							global_sens_lumped[samp + thread] = MCMC.sensitivity_mat_lumped(GA.dp_sens, param[thread + samp], sys[thread]);
						}

					}
					vars["mode"] = "MCMC";
					progressPercent = double(samp + endfor) / double(GA.nrealizations)*95.0 + 5;
					vars["progress"] = progressPercent;
					progress->update(vars);

					for (int thread = 0; thread < endfor; thread++)
					{
						for (int i = 0; i < n_BTCout_obs; i++)
						{
							modeled_samples[i].BTC[samp + thread] = sys[thread].modeled.BTC[i];
							modeled_samples[i].setname(samp + thread, sys[thread].modeled.names[i] + "_" + numbertostring(samp + thread));
							tracers[thread + samp].BTC[sys[thread].measured_quan[i].quan].append(sys[thread].modeled.BTC[i]);  // R2
							tracers_m[thread + samp].BTC[sys[thread].measured_quan[i].quan].append(sys[thread].measured_quan[i].observed_data); //R2
							overall.BTC[0].append(sys[thread].modeled.BTC[i]); //R2
							overall.BTC[1].append(sys[thread].measured_quan[i].observed_data); //R2
							all_realizations.BTC[0].append(sys[thread].modeled.BTC[i]); //R2
							all_realizations.BTC[1].append(sys[thread].measured_quan[i].observed_data); //R2
							all_realizations_t.BTC[sys[thread].measured_quan[i].quan].append(sys[thread].modeled.BTC[i]); //R2
							all_realizations_m.BTC[sys[thread].measured_quan[i].quan].append(sys[thread].measured_quan[i].observed_data); //R2
						}

						for (int i = 0; i < sys[thread].Tracer.size(); i++)
						{
							R2_t.BTC[i].append(samp + thread, R2_c(tracers[thread + samp].BTC[i], tracers_m[thread + samp].BTC[i]));  //R2
							R2_t_ln.BTC[i].append(samp + thread, R2_c(tracers[thread + samp].BTC[i].Log(1e-12), tracers_m[thread + samp].BTC[i].Log(1e-12))); //R2
						}


					{
						R2_overall.append(samp + thread, R2_c(overall.BTC[0], overall.BTC[1])); //R2
						R2_overall_ln.append(samp + thread, R2_c(overall.BTC[0].Log(1e-12), overall.BTC[1].Log(1e-12))); //R2
					}
					if (system().project == true)
						for (int i = 0; i < system().Well.size(); i++)
							for (int j = 0; j < system().Tracer.size(); j++)
							{
								projected_samples[j + i*system().Tracer.size()].BTC[samp + thread] = sys[thread].projected.BTC[j + i*system().Tracer.size()];
								projected_samples[j + i*system().Tracer.size()].setname(samp + thread, sys[thread].Well[i].name + "_" + sys[thread].Tracer[j].name + "_" + numbertostring(samp + thread));
							}

					for (int i = 0; i < system().Well.size(); i++)
					{
						Age_dist[i].BTC[samp + thread] = sys[thread].Well[i].young_age_distribution*(1 - sys[thread].Well[i].fraction_old);
						Age_dist[i].setname(samp + thread, sys[thread].Well[i].name + "_" + numbertostring(samp + thread));
						Age_dist_cum[i].BTC[samp + thread] = Age_dist[i].BTC[samp + thread].getcummulative();
						Age_dist_cum[i].setname(samp + thread, sys[thread].Well[i].name + "_" + numbertostring(samp + thread));

					}

					}
				}
				QMap<QString, QVariant> vars;
				vars["progress"] = 100;
				progress->update(vars);

				//qDebug() << "200";
				CBTCSet R2_all_realization(2 * all_realizations_t.BTC.size() + 2);
				//qDebug() << 201;
				R2_all_realization.names.push_back("R2 all realizations");
				//qDebug() << 202;
				R2_all_realization.BTC[0].append(R2_c(all_realizations.BTC[0], all_realizations.BTC[1]));
				//qDebug() << 203;
				R2_all_realization.names.push_back("R2_ln all realizations");
				//qDebug() << 204;
				R2_all_realization.BTC[1].append(R2_c(all_realizations.BTC[0].Log(1e-12), all_realizations.BTC[1].Log(1e-12)));
				//qDebug() << 205;

				for (int ii = 0; ii < all_realizations_t.BTC.size(); ii++)
				{
					R2_all_realization.names.push_back(system().Tracer[ii].name);
					R2_all_realization.BTC[2 * ii + 2].append(R2_c(all_realizations_t.BTC[ii], all_realizations_m.BTC[ii]));
					R2_all_realization.names.push_back(system().Tracer[ii].name + "(R2_ln)");
					R2_all_realization.BTC[2 * ii + 3].append(R2_c(all_realizations_t.BTC[ii].Log(1e-12), all_realizations_m.BTC[ii].Log(1e-12)));
				}
				//qDebug() << 206;
				for (int ij = 0; ij < system().Tracer.size(); ij++)
				{
					R2_t.names.push_back(system().Tracer[ij].name);
					R2_t_ln.names.push_back(system().Tracer[ij].name);
				}
				//qDebug() << 207;
				R2_t.writetofile(system().outpathname + "R2_tracers.txt");
				R2_t_ln.writetofile(system().outpathname + "R2_tracers_ln.txt");
				R2_overall.writefile(system().outpathname + "R2_overall.txt");
				R2_overall_ln.writefile(system().outpathname + "R2_overall_ln.txt");
				R2_all_realization.writetofile(system().outpathname + "R2_all_realization.txt");

				//qDebug() << 208;
				for (int i = 0; i < system().Well.size(); i++)
				{
					Age_dist[i].writetofile(system().outpathname + "Age_dist_" + GA.Sys_out.Medium[0].Well[i].name + ".txt");
					Age_dist_cum[i].writetofile(system().outpathname + "Age_dist_cum" + GA.Sys_out.Medium[0].Well[i].name + ".txt");
				}
				//qDebug() << 209;
				for (int i = 0; i < n_BTCout_obs; i++)
					modeled_samples[i].writetofile(system().outpathname + "modeled_" + GA.Sys_out.Medium[0].measured_quan[i].name + ".txt");
				//qDebug() << 210;
				for (int i = 0; i < system().Well.size(); i++)
					for (int j = 0; j < system().Tracer.size(); j++)
						projected_samples[j + i*system().Tracer.size()].writetofile(system().outpathname + "Projected_" + system().Well[i].name + "_" + system().Tracer[j].name + ".txt");

				//if (GA.noise_realization_writeout)
				//	for (int i=0; i<n_BTCout_obs; i++)
				//		for (int j=0; j<GA.Sys.n_time_series; j++)
				//			BTCout_obs_noise[j][i].writetofile(system.outpathname + "BTC_obs_noise" + string(_itoa(i,buffer,10)) + "_" + string(_itoa(j,buffer,10)) + ".txt",system.writeinterval);

				// calculating percentiles
				//qDebug() << "300";

				if (GA.calc_output_percentiles.size() > 0)
				{
					for (int i = 0; i < n_BTCout_obs; i++)
					{
						modeled_prcntl[i] = modeled_samples[i].getpercentiles(GA.calc_output_percentiles);
						modeled_prcntl_all.BTC[0].append(modeled_prcntl[i].BTC[0].t[0], modeled_prcntl[i].BTC[0].C[0]);
						modeled_prcntl_all.BTC[1].append(modeled_prcntl[i].BTC[1].t[0], modeled_prcntl[i].BTC[1].C[0]);
						modeled_prcntl_all.BTC[2].append(modeled_prcntl[i].BTC[2].t[0], modeled_prcntl[i].BTC[2].C[0]);
						modeled_prcntl[i].writetofile(system().outpathname + GA.Sys_out.Medium[0].measured_quan[i].name + "_prcntl.txt");
					}
					for (int i = 0; i < GA.Sys_out.Medium[0].Well.size(); i++)
						for (int j = 0; j < GA.Sys_out.Medium[0].Tracer.size(); j++)
						{
							projected_prcntle[j + i*GA.Sys_out.Medium[0].Tracer.size()] = projected_samples[j + i*GA.Sys_out.Medium[0].Tracer.size()].getpercentiles(GA.calc_output_percentiles);
							projected_prcntle[j + i*GA.Sys_out.Medium[0].Tracer.size()].writetofile(system().outpathname + "projected_prcntl" + GA.Sys_out().Tracer[j].name + "@" + GA.Sys_out.Medium[0].Well[i].name + "_" + ".txt");

							//BTCout_obs_prcntle_noise[j][i] = BTCout_obs_noise[j][i].getpercentiles(GA.calc_output_percentiles);
							//BTCout_obs_prcntle_noise[j][i].writetofile(system.outpathname + "BTC_obs_prcntl_noise" + string(_itoa(i,buffer,10)) + "_" + string(_itoa(j,buffer,10)) + ".txt",system.writeinterval);

						}

					mainGraphWidget->results->projectedTracerConcentrations = projected_samples;
					mainGraphWidget->results->projectedTracerConcentrationPercentiles = projected_prcntle;

					for (int i = 0; i < GA.Sys_out.Medium[0].Well.size(); i++)
					{
						Age_dist_prcntle[i] = Age_dist[i].getpercentiles(GA.calc_output_percentiles);
						Age_dist_cum_prcntle[i] = Age_dist_cum[i].getpercentiles(GA.calc_output_percentiles);
						Age_dist_prcntle[i].writetofile(system().outpathname + "Age_dist_prcntl_" + GA.Sys_out.Medium[0].Well[i].name + ".txt");
						Age_dist_cum_prcntle[i].writetofile(system().outpathname + "Age_dist_cum_prcntl_" + GA.Sys_out.Medium[0].Well[i].name + ".txt");
					}
				}
				//qDebug() << "400";

				mainGraphWidget->results->youngAgeDistributionRealizations = Age_dist;
				mainGraphWidget->results->cumulativeYoungAgeDistributionRealizations = Age_dist_cum;
				mainGraphWidget->results->youngAgeDistributionRealizationPercentiles = Age_dist_prcntle;
				mainGraphWidget->results->cumulativeYoungAgeDistributionRealizationPercentiles = Age_dist_cum_prcntle;


				modeled_prcntl_all.writetofile(system().outpathname + "modeled_percentile_all.txt");
				if (system().realizedparamfilename != "") paramsList.writetofile(system().outpathname + system().realizedparamfilename);


				if (MCMC.global_sensitivity == true)
				{
					//CMatrix global_sens_mat_avg = Average(global_sens_mat);
					CMatrix global_sens_lumped_avg = Average(global_sens_lumped);

					//global_sens_mat_avg.writetofile(system.outpathname + "global_sensitivity_mat_avg.txt");
					global_sens_lumped_avg.writetofile(system().outpathname + "global_sensitivity_lumped_avg.txt");
					mainGraphWidget->results->globalSensitivityMatrix = global_sens_lumped_avg;
				}
				//qDebug() << "500";

				// post-MCMC run time :
				t1 = clock() - t0;
				float run_time = ((float)t1) / CLOCKS_PER_SEC;
				string st = " sec";
				if (run_time >= 60) { run_time /= 60; st = " min"; }
				if (run_time >= 60) { run_time /= 60; st = " hr"; }

				runtime_file << " Post-MCMC Run Time :  " << run_time << st << endl;
				runtime_file.close();
			}
		}
		//CBTCSet a = CBTCSet(GA.Sys_out.modeled);
//		if (mainGraphWidget->model->modeled.nvars == 0)
//		{
//			delete mainGraphWidget->model;
//			mainGraphWidget->model = new CGWA(GA.Sys_out);
//		}

//		mainGraphWidget->results->ANS_obs = GA.Sys_out.modeled;
//		mainGraphWidget->results->projected = GA.Sys_out.projected;
		progress->setLabel("Done!");
		QMap<QString, QVariant> vars;
		vars["progress"] = 100;
		vars["finished"] = true;
		progress->update(vars);
	}

	return;
}
#endif

#ifdef CopulaGWA
int _tmain(int argc, _TCHAR* argv[])
{
	string filename;
	cout << "Input File Name: ";
	cin >> filename;
	if (filename == "1")
		filename = "IG_tracers_only.txt";
	else if (filename == "2")
		filename = "Input_IG_old_age_oneWell.txt";

	CGWA system(filename);

	if (system.inverse == false)
	{
		system.getmodeled();
		system.modeled.writetofile(system.outpathname + "modeled.txt");
		if (system.project == true) {
			system.Do_project();
			system.projected.writetofile(system.outpathname + "projected.txt");
		}
	}
	else
	{

		clock_t t0, t1;
		t0 = clock();

		ofstream runtime_file;
		runtime_file.open(system.outpathname + "Runing_times.txt");
		CGA GA(system.pathname + system.PE_info_filename, system); //initialize GA

		//**************************************** Genetic ****************************************

		if (GA.justreadmcmc == false && GA.continue_mcmc == false)
		{
			if (GA.getfromfilename == "")
			{
				GA.optimize();

				// Genetic run time :
				t1 = clock() - t0;
				float run_time = ((float)t1) / CLOCKS_PER_SEC;
				string st = " sec";
				if (run_time >= 60) { run_time /= 60; st = " min"; }
				if (run_time >= 60) { run_time /= 60; st = " hr"; }

				runtime_file << " Genetic Run Time :  " << run_time << st << endl;
			}
			else
			{
				CVector out(1);
				out[0] = GA.getfromoutput(system.outpathname + GA.getfromfilename);
				out.writetofile(system.outpathname + "likelihood.txt");
			}

			GA.Sys_out.modeled.writetofile(system.outpathname + GA.Sys.detoutfilename);
			if (GA.Sys_out.project == true)
			{
				GA.Sys_out.projected.writetofile(system.outpathname + "projected.txt");
			}
		}

		//**************************************** local sensitivity ****************************************
		t0 = clock();

		CMCMC MCMC(GA);
		int mcmcstart = MCMC.n_chains;
		if (GA.justreadmcmc == false)
		{
			if (MCMC.continue_mcmc == false)
				MCMC.initialize(GA.final_params);
			else
				mcmcstart = MCMC.readfromfile(system.outpathname + MCMC.outputfilename);

			if (GA.sens_out == true)
			{
				CMatrix S_mat = MCMC.sensitivity_mat(GA.dp_sens, GA.final_params);
				S_mat.writetofile(system.outpathname + "sensitivity_mat.txt");

				//CMatrix S_mat_lumped = MCMC.sensitivity_mat_lumped(GA.dp_sens, GA.final_params);
				//S_mat_lumped.writetofile(system.pathname+"sensitivity_mat_lumped.txt");

			}
		}

		//**************************************** MCMC ****************************************
		if (GA.justreadmcmc == false)
		{
			MCMC.step(mcmcstart, int((MCMC.nsamples - mcmcstart) / MCMC.n_chains)*MCMC.n_chains, system.outpathname + MCMC.outputfilename);

			// MCMC run time :
			t1 = clock() - t0;
			float run_time = ((float)t1) / CLOCKS_PER_SEC;
			string st = " sec";
			if (run_time >= 60) { run_time /= 60; st = " min"; }
			if (run_time >= 60) { run_time /= 60; st = " hr"; }

			runtime_file << " MCMC Run Time :  " << run_time << st << endl;
		}

		CBTCSet MCMCOut(system.outpathname + MCMC.outputfilename, false);
		MCMCOut.names = MCMC.paramname;

		//CBTCSet MCMC_red = MCMCOut.Extract(11);
		//MCMC_red.names=MCMCOut.names;
		//MCMC_red.writetofile_unif(system.pathname+"mcmc_red.txt");

		if (GA.calc_distributions == true)
		{
			CBTCSet posterior_distribution = MCMCOut.distribution(GA.no_bins, MCMC.paramname.size(), MCMC.n_burnout);
			posterior_distribution.names = MCMC.paramname;
			posterior_distribution.writetofile(system.outpathname + "posterior_distribution.txt");

			CBTCSet prior_distribution = MCMC.prior_distribution(GA.no_bins * 10);
			prior_distribution.names = MCMC.paramname;
			prior_distribution.writetofile(system.outpathname + "prior_distribution.txt");
		}

		//********************************* MCMC Percentile **************************************

		t0 = clock();
		char buffer[33];

		if (GA.calculate_percentile == true)
		{
			vector<double> p2dot5 = MCMCOut.percentile(0.025, MCMC.n_burnout);
			vector<double> p50 = MCMCOut.percentile(0.5, MCMC.n_burnout);
			vector<double> mean = MCMCOut.mean(MCMC.n_burnout);
			vector<double> std = MCMCOut.std(MCMC.n_burnout);
			vector<double> p97dot5 = MCMCOut.percentile(0.975, MCMC.n_burnout);

			FILE *filepercentile;
			filepercentile = fopen((system.outpathname + "percentiles.txt").c_str(), "w");

			for (int i = 0; i < MCMCOut.names.size(); i++)
				fprintf(filepercentile, "%s, ", MCMCOut.names[i].c_str());

			fprintf(filepercentile, "\n");

			CVector(p2dot5).writetofile(filepercentile);
			CVector(p50).writetofile(filepercentile);
			CVector(p97dot5).writetofile(filepercentile);
			CVector(mean).writetofile(filepercentile);
			CVector(std).writetofile(filepercentile);

			double mean_log = mean[MCMCOut.nvars - 4];
			double mean_likelihood = MCMCOut.BTC[MCMCOut.nvars - 4].Exp().average();
			fprintf(filepercentile, "mean log: %e\n", 2 * mean_log);

			double logp_mean = -GA.assignfitnesses(mean);
			fprintf(filepercentile, "f(mean): %e\n", 2 * logp_mean);

			double DIC = 4 * mean_log - 2 * logp_mean;
			fprintf(filepercentile, "DIC: %e\n", DIC);

			fclose(filepercentile);

			//********************************* MCMC mean out **************************************

			double out1 = GA.assignfitnesses(mean);

			GA.Sys_out.modeled.writetofile(system.outpathname + "BTC_mean.txt");

		}

		//********************************* MCMC Correlation **************************************

		if (GA.calculate_correlation == true)
		{
			CMatrix correlation_mat = MCMCOut.correlation(MCMC.n_burnout, MCMC.MCMCParam.size());
			correlation_mat.writetofile(system.outpathname + "correlation_mat.txt");
		}

		//********************************* MCMC realizations out **************************************

		if (GA.mcmc_realization == true)
		{
			vector<CBTCSet> modeled_samples; //each contain a measured quantity
			vector<CBTCSet> projected_samples; //each contain a tracer in a well
			//vector<CBTCSet> BTCout_obs_noise; //each contain a tracer in a well
			vector<CBTCSet> modeled_prcntl;  //each contain a tracer in a measured quantity
			CBTCSet modeled_prcntl_all(3);
			vector<CBTCSet> projected_prcntle;  //each contain a tracer in a well
			//vector<CBTCSet> BTCout_obs_prcntle_noise;
			vector<CBTCSet> Age_dist;
			vector<CBTCSet> Age_dist_prcntle;
			vector<CBTCSet> Age_dist_cum;
			vector<CBTCSet> Age_dist_cum_prcntle;

			int n_BTCout_obs = system.measured_quan.size();

			modeled_samples.resize(n_BTCout_obs);
			modeled_prcntl.resize(n_BTCout_obs);
			Age_dist.resize(system.Well.size());
			Age_dist_prcntle.resize(system.Well.size());
			Age_dist_cum.resize(system.Well.size());
			Age_dist_cum_prcntle.resize(system.Well.size());
			projected_samples.resize(system.Well.size()*system.Tracer.size());
			projected_prcntle.resize(system.Well.size()*system.Tracer.size());


			for (int i = 0; i < system.Well.size(); i++)
			{
				Age_dist[i] = CBTCSet(GA.nrealizations);
				Age_dist_cum[i] = CBTCSet(GA.nrealizations);
			}

			for (int i = 0; i < n_BTCout_obs; i++)
			{
				modeled_prcntl[i] = CBTCSet(GA.nrealizations);
				modeled_samples[i] = CBTCSet(GA.nrealizations);
			}

			for (int i = 0; i < system.Well.size()*system.Tracer.size(); i++)
			{
				projected_samples[i] = CBTCSet(GA.nrealizations);
				projected_prcntle[i] = CBTCSet(GA.nrealizations);
			}

			vector<double> param;
			CBTCSet paramsList(MCMCOut.nvars);
			paramsList.names = MCMCOut.names;
			vector<CMatrix> global_sens_mat;
			vector<CMatrix> global_sens_lumped;
			CBTCSet R2_t(system.Tracer.size());
			CBTC R2_overall;
			CBTCSet R2_t_ln(system.Tracer.size());
			CBTC R2_overall_ln;
			CBTCSet all_realizations(2);
			CBTCSet all_realizations_t(system.Tracer.size());
			CBTCSet all_realizations_m(system.Tracer.size());

			for (int samp = 0; samp < GA.nrealizations; samp++)
			{
				cout << "Realization Sample No. : " << samp << endl;
				CBTCSet tracers(system.Tracer.size());
				CBTCSet tracers_m(system.Tracer.size());
				CBTCSet overall(2);
				param = MCMCOut.getrandom(MCMC.n_burnout);
				paramsList.append(samp, param);
				GA.assignfitnesses(param);

				if (MCMC.global_sensitivity == true)
				{
					global_sens_mat.push_back(MCMC.sensitivity_mat(GA.dp_sens, param));
					global_sens_lumped.push_back(MCMC.sensitivity_mat_lumped(GA.dp_sens, param));
				}


				for (int i = 0; i < n_BTCout_obs; i++)
				{
					modeled_samples[i].BTC[samp] = GA.Sys_out.modeled.BTC[i];
					modeled_samples[i].names.push_back(GA.Sys_out.modeled.names[i] + "_" + string(_itoa(samp, buffer, 10)));
					tracers.BTC[GA.Sys_out.measured_quan[i].quan].append(GA.Sys_out.modeled.BTC[i]);
					tracers_m.BTC[GA.Sys_out.measured_quan[i].quan].append(GA.Sys_out.measured_quan[i].observed_data);
					overall.BTC[0].append(GA.Sys_out.modeled.BTC[i]);
					overall.BTC[1].append(GA.Sys_out.measured_quan[i].observed_data);
					all_realizations.BTC[0].append(GA.Sys_out.modeled.BTC[i]);
					all_realizations.BTC[1].append(GA.Sys_out.measured_quan[i].observed_data);
					all_realizations_t.BTC[GA.Sys_out.measured_quan[i].quan].append(GA.Sys_out.modeled.BTC[i]);
					all_realizations_m.BTC[GA.Sys_out.measured_quan[i].quan].append(GA.Sys_out.measured_quan[i].observed_data);
				}

				for (int i = 0; i < GA.Sys_out.Tracer.size(); i++)
				{
					R2_t.BTC[i].append(samp, R2_c(tracers.BTC[i], tracers_m.BTC[i]));
					R2_t_ln.BTC[i].append(samp, R2_c(tracers.BTC[i].Log(1e-12), tracers_m.BTC[i].Log(1e-12)));
				}

				R2_overall.append(samp, R2_c(overall.BTC[0], overall.BTC[1]));
				R2_overall_ln.append(samp, R2_c(overall.BTC[0].Log(1e-12), overall.BTC[1].Log(1e-12)));

				if (system.project == true)
					for (int i = 0; i < system.Well.size(); i++)
						for (int j = 0; j < system.Tracer.size(); j++)
						{
							projected_samples[j + i*system.Tracer.size()].BTC[samp] = GA.Sys_out.projected.BTC[j + i*system.Tracer.size()];
							projected_samples[j + i*system.Tracer.size()].names.push_back(GA.Sys_out.Well[i].name + "_" + GA.Sys_out.Tracer[j].name + "_" + string(_itoa(samp, buffer, 10)));
						}

				for (int i = 0; i < system.Well.size(); i++)
				{
					Age_dist[i].BTC[samp] = GA.Sys_out.Well[i].young_age_distribution*(1 - GA.Sys_out.Well[i].fraction_old);
					Age_dist[i].names.push_back(GA.Sys_out.Well[i].name + "_" + string(_itoa(samp, buffer, 10)));
					Age_dist_cum[i].BTC[samp] = Age_dist[i].BTC[samp].getcummulative();
					Age_dist_cum[i].names.push_back(GA.Sys_out.Well[i].name + "_" + string(_itoa(samp, buffer, 10)));

				}

			}

			CBTCSet R2_all_realization(2 * all_realizations_t.BTC.size() + 2);
			R2_all_realization.names.push_back("R2 all realizations");
			R2_all_realization.BTC[0].append(R2_c(all_realizations.BTC[0], all_realizations.BTC[1]));
			R2_all_realization.names.push_back("R2_ln all realizations");
			R2_all_realization.BTC[1].append(R2_c(all_realizations.BTC[0].Log(1e-12), all_realizations.BTC[1].Log(1e-12)));

			for (int ii = 0; ii < all_realizations_t.BTC.size(); ii++)
			{
				R2_all_realization.names.push_back(system.Tracer[ii].name);
				R2_all_realization.BTC[2 * ii + 2].append(R2_c(all_realizations_t.BTC[ii], all_realizations_m.BTC[ii]));
				R2_all_realization.names.push_back(system.Tracer[ii].name + "(R2_ln)");
				R2_all_realization.BTC[2 * ii + 3].append(R2_c(all_realizations_t.BTC[ii].Log(1e-12), all_realizations_m.BTC[ii].Log(1e-12)));
			}

			for (int ij = 0; ij < system.Tracer.size(); ij++)
			{
				R2_t.names.push_back(system.Tracer[ij].name);
				R2_t_ln.names.push_back(system.Tracer[ij].name);
			}

			R2_t.writetofile(system.outpathname + "R2_tracers.txt");
			R2_t_ln.writetofile(system.outpathname + "R2_tracers_ln.txt");
			R2_overall.writefile(system.outpathname + "R2_overall.txt");
			R2_overall_ln.writefile(system.outpathname + "R2_overall_ln.txt");
			R2_all_realization.writetofile(system.outpathname + "R2_all_realization.txt");


			for (int i = 0; i < system.Well.size(); i++)
			{
				Age_dist[i].writetofile(system.outpathname + "Age_dist_" + GA.Sys_out.Well[i].name + ".txt");
				Age_dist_cum[i].writetofile(system.outpathname + "Age_dist_cum" + GA.Sys_out.Well[i].name + ".txt");
			}

			for (int i = 0; i < n_BTCout_obs; i++)
				modeled_samples[i].writetofile(system.outpathname + "modeled_" + GA.Sys_out.measured_quan[i].name + ".txt");

			for (int i = 0; i < system.Well.size(); i++)
				for (int j = 0; j < system.Tracer.size(); j++)
					projected_samples[j + i*system.Tracer.size()].writetofile(system.outpathname + "Projected_" + system.Well[i].name + "_" + system.Tracer[j].name + ".txt");

			//if (GA.noise_realization_writeout)
			//	for (int i=0; i<n_BTCout_obs; i++)
			//		for (int j=0; j<GA.Sys.n_time_series; j++)
			//			BTCout_obs_noise[j][i].writetofile(system.outpathname + "BTC_obs_noise" + string(_itoa(i,buffer,10)) + "_" + string(_itoa(j,buffer,10)) + ".txt",system.writeinterval);

			// calculating percentiles

			if (GA.calc_output_percentiles.size() > 0)
			{
				for (int i = 0; i < n_BTCout_obs; i++)
				{
					modeled_prcntl[i] = modeled_samples[i].getpercentiles(GA.calc_output_percentiles);
					modeled_prcntl_all.BTC[0].append(modeled_prcntl[i].BTC[0].t[0], modeled_prcntl[i].BTC[0].C[0]);
					modeled_prcntl_all.BTC[1].append(modeled_prcntl[i].BTC[1].t[0], modeled_prcntl[i].BTC[1].C[0]);
					modeled_prcntl_all.BTC[2].append(modeled_prcntl[i].BTC[2].t[0], modeled_prcntl[i].BTC[2].C[0]);
					modeled_prcntl[i].writetofile(system.outpathname + GA.Sys_out.measured_quan[i].name + "_prcntl.txt");
				}
				for (int i = 0; i < GA.Sys_out.Well.size(); i++)
					for (int j = 0; j < GA.Sys_out.Tracer.size(); j++)
					{
						projected_prcntle[j + i*GA.Sys_out.Tracer.size()] = projected_samples[j + i*GA.Sys_out.Tracer.size()].getpercentiles(GA.calc_output_percentiles);
						projected_prcntle[j + i*GA.Sys_out.Tracer.size()].writetofile(system.outpathname + "projected_prcntl" + GA.Sys_out.Tracer[j].name + "_" + GA.Sys_out.Well[i].name + "_" + ".txt");

						//BTCout_obs_prcntle_noise[j][i] = BTCout_obs_noise[j][i].getpercentiles(GA.calc_output_percentiles);
						//BTCout_obs_prcntle_noise[j][i].writetofile(system.outpathname + "BTC_obs_prcntl_noise" + string(_itoa(i,buffer,10)) + "_" + string(_itoa(j,buffer,10)) + ".txt",system.writeinterval);

					}
				for (int i = 0; i < GA.Sys_out.Well.size(); i++)
				{
					Age_dist_prcntle[i] = Age_dist[i].getpercentiles(GA.calc_output_percentiles);
					Age_dist_cum_prcntle[i] = Age_dist_cum[i].getpercentiles(GA.calc_output_percentiles);
					Age_dist_prcntle[i].writetofile(system.outpathname + "Age_dist_prcntl_" + GA.Sys_out.Well[i].name + ".txt");
					Age_dist_cum_prcntle[i].writetofile(system.outpathname + "Age_dist_cum_prcntl_" + GA.Sys_out.Well[i].name + ".txt");
				}
			}


			modeled_prcntl_all.writetofile(system.outpathname + "modeled_percentile_all.txt");
			if (system.realizedparamfilename != "") paramsList.writetofile(system.outpathname + system.realizedparamfilename);


			if (MCMC.global_sensitivity == true)
			{
				CMatrix global_sens_mat_avg = Average(global_sens_mat);
				CMatrix global_sens_lumped_avg = Average(global_sens_lumped);

				global_sens_mat_avg.writetofile(system.outpathname + "global_sensitivity_mat_avg.txt");
				global_sens_lumped_avg.writetofile(system.outpathname + "global_sensitivity_lumped_avg.txt");
			}

			// post-MCMC run time :
			t1 = clock() - t0;
			float run_time = ((float)t1) / CLOCKS_PER_SEC;
			string st = " sec";
			if (run_time >= 60) { run_time /= 60; st = " min"; }
			if (run_time >= 60) { run_time /= 60; st = " hr"; }

			runtime_file << " Post-MCMC Run Time :  " << run_time << st << endl;
			runtime_file.close();
		}
	}


	return 0;
}
#endif

