// Bioretmod_console.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "StringOP.h"
#include "MediumSet.h"
#include "GA.h"
#include "MCMC.h"
#include "stdafx.h"
#include <time.h>
#include <omp.h>


using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
    clock_t t0,t1;
	t0 = clock();

	string filename;
	cout<<"Input File:";
	cin>>filename;
	
	CMediumSet MS(filename);
	//CMedium system(filename, &MS);
	CMedium system = MS.Medium[0];

	if ((system.parameters().size()==0) || (system.forward()==true))
	{
		clock_t t0,t1;
		t0 = clock();
		ofstream runtime_file;
		runtime_file.open(system.outputpathname() +"Runing_times.txt");
		system.solve();
 		t1 = clock() - t0;
		float run_time = ((float)t1)/CLOCKS_PER_SEC;
		string st = " sec";		
		if (run_time >= 60) {run_time /= 60; st = " min";}
		if (run_time >= 60) {run_time /= 60; st = " hr";}
		runtime_file << "Run Time :  " << run_time << st << endl;
		runtime_file << "Epoch count: "<<system.epoch_count<<endl;
		system.ANS.writetofile(system.outputpathname()+system.detoutfilename_hydro[0]);
		system.ANS_obs.writetofile(system.outputpathname()+system.detoutfilename_obs());
		system.ANS_colloids.writetofile(system.outputpathname()+system.detoutfilename_prtcle[0]);
		system.ANS_constituents.writetofile(system.outputpathname()+system.detoutfilename_wq[0]);
		if (system.mass_balance_check()) system.ANS_MB.writetofile(system.outputpathname()+"output_MB.txt");
		system.Solution_dt.writetofile(system.outputpathname()+"dt.txt");
		runtime_file.close();	

	}
	else
	{
		ofstream runtime_file;
		runtime_file.open(system.outputpathname() +"Runing_times.txt");
		CGA GA(system.outputpathname() +system.PE_info_filename(),system); //initialize GA

		//**************************************** Genetic ****************************************

		if (GA.justreadmcmc == false && GA.continue_mcmc == false )
			{
				if (GA.getfromfilename == "")	
				{
					GA.optimize();
				
					// Genetic run time :
					t1 = clock() - t0;
					float run_time = ((float)t1)/CLOCKS_PER_SEC;
					string st = " sec";		
					if (run_time >= 60) {run_time /= 60; st = " min";}
					if (run_time >= 60) {run_time /= 60; st = " hr";}

					runtime_file << " Genetic Run Time :  " << run_time << st << endl;
				}
				else
				{	
					CVector out(1);
					out[0] = GA.getfromoutput(system.outputpathname()+GA.getfromfilename);
					//out.writetofile(system.outputpathname +"likelihood.txt");
				}

				if (system.detoutfilename_hydro.size()>0)
				{
					//Form1.label1->Text=L"Saving Deterministic Output ";
					//Form1.Refresh();

					for (int i = 0; i < 1; i++)
					{
						GA.Sys_out[i].ANS.writetofile(system.outputpathname() + GA.Sys.detoutfilename_hydro[i], system.writeinterval());
						GA.Sys_out[i].ANS_colloids.writetofile(system.outputpathname() + GA.Sys.detoutfilename_prtcle[i], system.writeinterval());
						GA.Sys_out[i].ANS_constituents.writetofile(system.outputpathname() + GA.Sys.detoutfilename_wq[i], system.writeinterval());
					}
				}

				if (system.detoutfilename_obs()!="")
				{
					//Form1.label1->Text=L"Saving Deterministic Output";
					//Form1.Refresh();

					for (int i=0; i<1; i++)
						GA.Sys_out[i].ANS_obs.writetofile(system.outputpathname()+GA.Sys.detoutfilename_obs(),system.writeinterval());
				}
			}
		
		//**************************************** local sensitivity ****************************************
			t0 = clock();

			CMCMC MCMC(GA);
			int mcmcstart = MCMC.n_chains;
			if (GA.justreadmcmc==false) 
			{	
				//Form1.label1->Text=L"MCMC Initialization";
				//	Form1.Refresh();
				if (MCMC.continue_mcmc == false)
					MCMC.initialize(GA.final_params);
				else
					mcmcstart = MCMC.readfromfile(system.outputpathname()+MCMC.outputfilename);

				if ((GA.sens_out == true) && (MCMC.continue_mcmc == false))
				{	
					//Form1.label1->Text=L"Sensitivity Analysis ";
					//Form1.Refresh();
				
					CMatrix S_mat = MCMC.sensitivity_mat(GA.dp_sens, GA.final_params);
					S_mat.writetofile(system.outputpathname()+"sensitivity_mat.txt");				

					CMatrix S_mat_lumped = MCMC.sensitivity_mat_lumped(GA.dp_sens, GA.final_params);
					S_mat_lumped.writetofile(system.outputpathname()+"sensitivity_mat_lumped.txt");

				}
			}
	
		//**************************************** MCMC ****************************************
		if (GA.mcmc_run==true)	//Switch for MCMC Section
		{

			if (GA.justreadmcmc==false) 
			{
				MCMC.step(mcmcstart,int((MCMC.nsamples-mcmcstart)/MCMC.n_chains)*MCMC.n_chains,system.outputpathname()+MCMC.outputfilename);

				// MCMC run time :			
				t1 = clock() - t0;
				float run_time = ((float)t1)/CLOCKS_PER_SEC;
				string st = " sec";		
				if (run_time >= 60) {run_time /= 60; st = " min";}
				if (run_time >= 60) {run_time /= 60; st = " hr";}

				runtime_file << " MCMC Run Time :  " << run_time << st << endl;
			}

			CBTCSet MCMCOut(system.outputpathname()+MCMC.outputfilename, false);
			MCMCOut.names = MCMC.paramname;
			if (GA.calc_distributions == true)
				{
					CBTCSet posterior_distribution = MCMCOut.distribution(GA.no_bins, MCMC.paramname.size(),MCMC.n_burnout);
					posterior_distribution.names = MCMC.paramname;
					posterior_distribution.writetofile(system.outputpathname()+"posterior_distribution.txt");

					CBTCSet prior_distribution = MCMC.prior_distribution(GA.no_bins*10);
					prior_distribution.names = MCMC.paramname;
					prior_distribution.writetofile(system.outputpathname()+"prior_distribution.txt");
				}

				//********************************* MCMC Percentile **************************************
		
				t0 = clock();
				char buffer[33];
		
				if (GA.calculate_percentile == true)
				{
					vector<double> p2dot5 = MCMCOut.percentile(0.025,MCMC.n_burnout);
					vector<double> p50 = MCMCOut.percentile(0.5,MCMC.n_burnout);
					vector<double> mean = MCMCOut.mean(MCMC.n_burnout);
					vector<double> std = MCMCOut.std(MCMC.n_burnout);
					vector<double> p97dot5 = MCMCOut.percentile(0.975,MCMC.n_burnout);

					FILE *filepercentile;
					filepercentile = fopen((system.outputpathname() + "percentiles.txt").c_str(),"w");
	
					for (int i=0; i<MCMCOut.names.size(); i++)
						fprintf(filepercentile , "%s, ", MCMCOut.names[i].c_str());

					fprintf(filepercentile, "\n");

					CVector(p2dot5).writetofile(filepercentile);
					CVector(p50).writetofile(filepercentile);
					CVector(p97dot5).writetofile(filepercentile);
					CVector(mean).writetofile(filepercentile);
					CVector(std).writetofile(filepercentile);

					double mean_log = mean[MCMCOut.nvars-4];
					fprintf(filepercentile, "mean log: %e\n", 2*mean_log);

					double logp_mean = -GA.assignfitnesses(mean);
					fprintf(filepercentile, "f(mean): %e\n", 2*logp_mean);

					double DIC = 4*mean_log - 2*logp_mean;
					fprintf(filepercentile, "DIC: %e\n", DIC);

					fclose(filepercentile);
		
					//********************************* MCMC mean out **************************************

					
		
					for (int i=0; i<1; i++)
						GA.Sys_out[i].ANS.writetofile(system.outputpathname()+"BTC_mean" + string(_itoa(i,buffer,10)) + ".txt",system.writeinterval());
					for (int i=0; i<1; i++)
						GA.Sys_out[i].ANS_obs.writetofile(system.outputpathname()+"BTC_Obs_mean" + string(_itoa(i,buffer,10)) + ".txt",system.writeinterval());
				}
		
				//********************************* MCMC Correlation **************************************

				if (GA.calculate_correlation == true)
				{
					CMatrix correlation_mat = MCMCOut.correlation(MCMC.n_burnout,MCMC.MCMCParam.size());
					correlation_mat.writetofile(system.outputpathname()+"correlation_mat.txt");
				}

				//********************************* MCMC realizations out **************************************
							
				if (GA.mcmc_realization == true)
				{
					
					MCMC.get_outputpercentiles(MCMCOut);
   
					
					if (GA.obs_realization_writeout)
					{
						cout << "Saving realizations" << endl;
						for (int i = 0; i < MCMC.G.measured_quan.size(); i++)
							for (int j = 0; j < 1; j++)
								MCMC.BTCout_obs[j][i].writetofile(system.outputpathname() + "BTC_obs" + string(_itoa(i, buffer, 10)) + "_" + string(_itoa(j, buffer, 10)) + ".txt", system.writeinterval());

						if (GA.noise_realization_writeout)
							for (int i = 0; i < MCMC.G.measured_quan.size(); i++)
								for (int j = 0; j < 1; j++)
									MCMC.BTCout_obs_noise[j][i].writetofile(system.outputpathname() + "BTC_obs_noise" + string(_itoa(i, buffer, 10)) + "_" + string(_itoa(j, buffer, 10)) + ".txt", system.writeinterval());
					}
					// calculating percentiles
			
					if (system.realizeparamfilename()!="") MCMC.paramsList.writetofile(system.outputpathname() + system.realizeparamfilename());
		

					if (MCMC.global_sensitivity == true)
					{
						CMatrix global_sens_lumped_avg = Average(MCMC.global_sens_lumped);
						global_sens_lumped_avg.writetofile(system.outputpathname()+"global_sensitivity_lumped_avg.txt");			
					}

					// post-MCMC run time :		
					t1 = clock() - t0;
					float run_time = ((float)t1)/CLOCKS_PER_SEC;
					string st = " sec";		
					if (run_time >= 60) {run_time /= 60; st = " min";}
					if (run_time >= 60) {run_time /= 60; st = " hr";}

					runtime_file << " Post-MCMC Run Time :  " << run_time << st << endl;
					runtime_file.close();
				}
			}
	}
	return 0;
}
