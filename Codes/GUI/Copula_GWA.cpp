// Copula_GWA.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GWA.h"
#include "GA.h"
#include "MCMC.h"

/*int _tmain(int argc, _TCHAR* argv[])
{
	string filename;
	cout<<"Input File Name: ";
	cin>>filename;
	if (filename == "1")
		filename = "IG_tracers_only.txt";
	else if (filename == "2")
		filename = "Input_IG_old_age_oneWell.txt";

	CGWA system(filename);
		
	if (system.inverse==false)
	{
		system.getmodeled();		
		system.modeled.writetofile(system.outpathname+"modeled.txt");
		if (system.project==true) {
			system.Do_project();
			system.projected.writetofile(system.outpathname+"projected.txt"); 
		}
	}
	else
	{

		clock_t t0,t1;
		t0 = clock();

		ofstream runtime_file;
		runtime_file.open(system.outpathname +"Runing_times.txt");
		CGA GA(system.pathname +system.PE_info_filename,system); //initialize GA

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
					out[0] = GA.getfromoutput(system.outpathname+GA.getfromfilename);
					out.writetofile(system.outpathname +"likelihood.txt");
				}

				GA.Sys_out.modeled.writetofile(system.outpathname+GA.Sys.detoutfilename);
				if (GA.Sys_out.project==true)
				{
					GA.Sys_out.projected.writetofile(system.outpathname+"projected.txt");
				}
			}
		
		//**************************************** local sensitivity ****************************************
			t0 = clock();

			CMCMC MCMC(GA);
			int mcmcstart = MCMC.n_chains;
			if (GA.justreadmcmc==false) 
			{	
				if (MCMC.continue_mcmc == false)
					MCMC.initialize(GA.final_params);
				else
					mcmcstart = MCMC.readfromfile(system.outpathname+MCMC.outputfilename);

				if (GA.sens_out == true)
				{	
					CMatrix S_mat = MCMC.sensitivity_mat(GA.dp_sens, GA.final_params);
					S_mat.writetofile(system.outpathname+"sensitivity_mat.txt");				

					//CMatrix S_mat_lumped = MCMC.sensitivity_mat_lumped(GA.dp_sens, GA.final_params);
					//S_mat_lumped.writetofile(system.pathname+"sensitivity_mat_lumped.txt");

				}
			}
	
		//**************************************** MCMC ****************************************
		if (GA.justreadmcmc==false) 
		{
			MCMC.step(mcmcstart,int((MCMC.nsamples-mcmcstart)/MCMC.n_chains)*MCMC.n_chains,system.outpathname+MCMC.outputfilename);

			// MCMC run time :			
			t1 = clock() - t0;
			float run_time = ((float)t1)/CLOCKS_PER_SEC;
			string st = " sec";		
			if (run_time >= 60) {run_time /= 60; st = " min";}
			if (run_time >= 60) {run_time /= 60; st = " hr";}

			runtime_file << " MCMC Run Time :  " << run_time << st << endl;
		}

		CBTCSet MCMCOut(system.outpathname+MCMC.outputfilename, false);
		MCMCOut.names = MCMC.paramname;
		
		//CBTCSet MCMC_red = MCMCOut.Extract(11);
		//MCMC_red.names=MCMCOut.names;
		//MCMC_red.writetofile_unif(system.pathname+"mcmc_red.txt");
		
		if (GA.calc_distributions == true)
			{
				CBTCSet posterior_distribution = MCMCOut.distribution(GA.no_bins, MCMC.paramname.size(),MCMC.n_burnout);
				posterior_distribution.names = MCMC.paramname;
				posterior_distribution.writetofile(system.outpathname+"posterior_distribution.txt");

				CBTCSet prior_distribution = MCMC.prior_distribution(GA.no_bins*10);
				prior_distribution.names = MCMC.paramname;
				prior_distribution.writetofile(system.outpathname+"prior_distribution.txt");
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
				filepercentile = fopen((system.outpathname + "percentiles.txt").c_str(),"w");
	
				for (int i=0; i<MCMCOut.names.size(); i++)
					fprintf(filepercentile , "%s, ", MCMCOut.names[i].c_str());

				fprintf(filepercentile, "\n");

				CVector(p2dot5).writetofile(filepercentile);
				CVector(p50).writetofile(filepercentile);
				CVector(p97dot5).writetofile(filepercentile);
				CVector(mean).writetofile(filepercentile);
				CVector(std).writetofile(filepercentile);

				double mean_log = mean[MCMCOut.nvars-4];
				double mean_likelihood = MCMCOut.BTC[MCMCOut.nvars-4].Exp().average();
				fprintf(filepercentile, "mean log: %e\n", 2*mean_log);

				double logp_mean = -GA.assignfitnesses(mean);
				fprintf(filepercentile, "f(mean): %e\n", 2*logp_mean);

				double DIC = 4*mean_log - 2*logp_mean;
				fprintf(filepercentile, "DIC: %e\n", DIC);

				fclose(filepercentile);
		
				//********************************* MCMC mean out **************************************

				double out1 = GA.assignfitnesses(mean);			
		
				GA.Sys_out.modeled.writetofile(system.outpathname+"BTC_mean.txt");
			
			}
		
			//********************************* MCMC Correlation **************************************

			if (GA.calculate_correlation == true)
			{
				CMatrix correlation_mat = MCMCOut.correlation(MCMC.n_burnout,MCMC.MCMCParam.size());
				correlation_mat.writetofile(system.outpathname+"correlation_mat.txt");
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
				
			    
				for (int i=0; i<system.Well.size(); i++)
				{	Age_dist[i] = CBTCSet(GA.nrealizations);
					Age_dist_cum[i] = CBTCSet(GA.nrealizations);
				}

				for (int i=0; i<n_BTCout_obs; i++) 
				{	modeled_prcntl[i] = CBTCSet(GA.nrealizations);
					modeled_samples[i] = CBTCSet(GA.nrealizations);
				}

				for (int i=0; i<system.Well.size()*system.Tracer.size(); i++)
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
				
				for (int samp=0; samp<GA.nrealizations; samp++)
				{
					cout<< "Realization Sample No. : " <<samp<<endl;
					CBTCSet tracers(system.Tracer.size());
					CBTCSet tracers_m(system.Tracer.size());
					CBTCSet overall(2);
					param = MCMCOut.getrandom(MCMC.n_burnout);
					paramsList.append(samp,param);
					GA.assignfitnesses(param);

					if (MCMC.global_sensitivity == true)
					{					
						global_sens_mat.push_back(MCMC.sensitivity_mat(GA.dp_sens, param));
						global_sens_lumped.push_back(MCMC.sensitivity_mat_lumped(GA.dp_sens, param));
					}

			
					for (int i=0; i<n_BTCout_obs; i++)
					{	
						modeled_samples[i].BTC[samp] = GA.Sys_out.modeled.BTC[i];
						modeled_samples[i].names.push_back(GA.Sys_out.modeled.names[i]+"_"+string(_itoa(samp,buffer,10))); 
						tracers.BTC[GA.Sys_out.measured_quan[i].quan].append(GA.Sys_out.modeled.BTC[i]);
						tracers_m.BTC[GA.Sys_out.measured_quan[i].quan].append(GA.Sys_out.measured_quan[i].observed_data);
						overall.BTC[0].append(GA.Sys_out.modeled.BTC[i]);
						overall.BTC[1].append(GA.Sys_out.measured_quan[i].observed_data);
						all_realizations.BTC[0].append(GA.Sys_out.modeled.BTC[i]);
						all_realizations.BTC[1].append(GA.Sys_out.measured_quan[i].observed_data);
						all_realizations_t.BTC[GA.Sys_out.measured_quan[i].quan].append(GA.Sys_out.modeled.BTC[i]);
						all_realizations_m.BTC[GA.Sys_out.measured_quan[i].quan].append(GA.Sys_out.measured_quan[i].observed_data);
					}

					for (int i=0; i<GA.Sys_out.Tracer.size(); i++)
					{	R2_t.BTC[i].append(samp, R2_c(tracers.BTC[i],tracers_m.BTC[i]));
						R2_t_ln.BTC[i].append(samp, R2_c(tracers.BTC[i].Log(1e-12),tracers_m.BTC[i].Log(1e-12)));
					}
					
					R2_overall.append(samp,R2_c(overall.BTC[0],overall.BTC[1]));
					R2_overall_ln.append(samp,R2_c(overall.BTC[0].Log(1e-12),overall.BTC[1].Log(1e-12)));

					if (system.project==true)
					for (int i=0; i<system.Well.size(); i++)
						for (int j=0; j<system.Tracer.size(); j++)
					{		
						projected_samples[j+i*system.Tracer.size()].BTC[samp] = GA.Sys_out.projected.BTC[j+i*system.Tracer.size()];
						projected_samples[j+i*system.Tracer.size()].names.push_back(GA.Sys_out.Well[i].name+"_"+GA.Sys_out.Tracer[j].name+"_"+string(_itoa(samp,buffer,10)));
					}

					for (int i=0; i<system.Well.size(); i++)
					{	Age_dist[i].BTC[samp] = GA.Sys_out.Well[i].young_age_distribution*(1-GA.Sys_out.Well[i].fraction_old);
						Age_dist[i].names.push_back(GA.Sys_out.Well[i].name+"_"+string(_itoa(samp,buffer,10)));	
						Age_dist_cum[i].BTC[samp] = Age_dist[i].BTC[samp].getcummulative();
						Age_dist_cum[i].names.push_back(GA.Sys_out.Well[i].name+"_"+string(_itoa(samp,buffer,10)));	

					}
			
				}
		
				CBTCSet R2_all_realization(2*all_realizations_t.BTC.size()+2); 
				R2_all_realization.names.push_back("R2 all realizations");
				R2_all_realization.BTC[0].append(R2_c(all_realizations.BTC[0],all_realizations.BTC[1]));
				R2_all_realization.names.push_back("R2_ln all realizations");
				R2_all_realization.BTC[1].append(R2_c(all_realizations.BTC[0].Log(1e-12),all_realizations.BTC[1].Log(1e-12)));
				
				for (int ii=0; ii<all_realizations_t.BTC.size(); ii++)
				{
					R2_all_realization.names.push_back(system.Tracer[ii].name);
					R2_all_realization.BTC[2*ii+2].append(R2_c(all_realizations_t.BTC[ii],all_realizations_m.BTC[ii]));
					R2_all_realization.names.push_back(system.Tracer[ii].name +"(R2_ln)");
					R2_all_realization.BTC[2*ii+3].append(R2_c(all_realizations_t.BTC[ii].Log(1e-12),all_realizations_m.BTC[ii].Log(1e-12)));
				}

				for (int ij=0; ij<system.Tracer.size(); ij++)
				{
					R2_t.names.push_back(system.Tracer[ij].name);
					R2_t_ln.names.push_back(system.Tracer[ij].name);
				}
				
				R2_t.writetofile(system.outpathname +"R2_tracers.txt");
				R2_t_ln.writetofile(system.outpathname +"R2_tracers_ln.txt");
				R2_overall.writefile(system.outpathname +"R2_overall.txt");
				R2_overall_ln.writefile(system.outpathname +"R2_overall_ln.txt");
				R2_all_realization.writetofile(system.outpathname + "R2_all_realization.txt");
				

				for (int i=0; i<system.Well.size(); i++)
				{	Age_dist[i].writetofile(system.outpathname+ "Age_dist_"+GA.Sys_out.Well[i].name  + ".txt");
					Age_dist_cum[i].writetofile(system.outpathname+ "Age_dist_cum"+GA.Sys_out.Well[i].name  + ".txt");
				}
			
				for (int i=0; i<n_BTCout_obs; i++)
					modeled_samples[i].writetofile(system.outpathname + "modeled_" + GA.Sys_out.measured_quan[i].name  + ".txt");
        
				for (int i=0; i<system.Well.size(); i++)
					for (int j=0; j<system.Tracer.size(); j++)
						projected_samples[j+i*system.Tracer.size()].writetofile(system.outpathname + "Projected_" + system.Well[i].name + "_"+ system.Tracer[j].name  + ".txt");

				//if (GA.noise_realization_writeout)
				//	for (int i=0; i<n_BTCout_obs; i++)
				//		for (int j=0; j<GA.Sys.n_time_series; j++)
				//			BTCout_obs_noise[j][i].writetofile(system.outpathname + "BTC_obs_noise" + string(_itoa(i,buffer,10)) + "_" + string(_itoa(j,buffer,10)) + ".txt",system.writeinterval);
					
				// calculating percentiles
			
				if (GA.calc_output_percentiles.size()>0)
				{	
					for (int i=0; i<n_BTCout_obs; i++)
					{	modeled_prcntl[i] = modeled_samples[i].getpercentiles(GA.calc_output_percentiles);
						modeled_prcntl_all.BTC[0].append(modeled_prcntl[i].BTC[0].t[0],modeled_prcntl[i].BTC[0].C[0]);
						modeled_prcntl_all.BTC[1].append(modeled_prcntl[i].BTC[1].t[0],modeled_prcntl[i].BTC[1].C[0]);
						modeled_prcntl_all.BTC[2].append(modeled_prcntl[i].BTC[2].t[0],modeled_prcntl[i].BTC[2].C[0]);
						modeled_prcntl[i].writetofile(system.outpathname+GA.Sys_out.measured_quan[i].name + "_prcntl.txt");
					}
					for (int i=0; i<GA.Sys_out.Well.size(); i++)
						for (int j=0; j<GA.Sys_out.Tracer.size(); j++)
							{	
								projected_prcntle[j+i*GA.Sys_out.Tracer.size()] = projected_samples[j+i*GA.Sys_out.Tracer.size()].getpercentiles(GA.calc_output_percentiles);
								projected_prcntle[j+i*GA.Sys_out.Tracer.size()].writetofile(system.outpathname + "projected_prcntl" + GA.Sys_out.Tracer[j].name+"_"+GA.Sys_out.Well[i].name + "_"  + ".txt");
				
								//BTCout_obs_prcntle_noise[j][i] = BTCout_obs_noise[j][i].getpercentiles(GA.calc_output_percentiles);
								//BTCout_obs_prcntle_noise[j][i].writetofile(system.outpathname + "BTC_obs_prcntl_noise" + string(_itoa(i,buffer,10)) + "_" + string(_itoa(j,buffer,10)) + ".txt",system.writeinterval);
							
							}	
				for (int i=0; i<GA.Sys_out.Well.size(); i++)
					{
						Age_dist_prcntle[i] = Age_dist[i].getpercentiles(GA.calc_output_percentiles);
						Age_dist_cum_prcntle[i] = Age_dist_cum[i].getpercentiles(GA.calc_output_percentiles);
						Age_dist_prcntle[i].writetofile(system.outpathname+"Age_dist_prcntl_" + GA.Sys_out.Well[i].name + ".txt");
						Age_dist_cum_prcntle[i].writetofile(system.outpathname+"Age_dist_cum_prcntl_" + GA.Sys_out.Well[i].name + ".txt");
					}
				}
				
				
				modeled_prcntl_all.writetofile(system.outpathname+"modeled_percentile_all.txt");			
				if (system.realizedparamfilename!="") paramsList.writetofile(system.outpathname + system.realizedparamfilename);
			

				if (MCMC.global_sensitivity == true)
				{
					CMatrix global_sens_mat_avg = Average(global_sens_mat);
					CMatrix global_sens_lumped_avg = Average(global_sens_lumped);

					global_sens_mat_avg.writetofile(system.outpathname+"global_sensitivity_mat_avg.txt");	
					global_sens_lumped_avg.writetofile(system.outpathname+"global_sensitivity_lumped_avg.txt");			
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


	return 0;
}

*/