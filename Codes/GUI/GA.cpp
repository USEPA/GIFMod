// GA.cpp: implementation of the CGA class.
////////////////////////////////////////////////////////////////////////
#include "GA.h"
#include "StringOP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGA::CGA()
{
	maxpop = 100;
	Ind.resize(maxpop);
	Ind_old.resize(maxpop);
	fitdist = CDistribution(maxpop);
	N = 1;       //MM N=1 by default
	pcross = 1;
	cross_over_type = 1;
	MaxFitness = 0;
}

CGA::CGA(int n)
{
	maxpop = n;
	Ind.resize(maxpop);
	Ind_old.resize(maxpop);
	fitdist = CDistribution(maxpop);
	N = 1;
	pcross = 1;
	cross_over_type = 1;
	MaxFitness = 0;
}

CGA::CGA(int n, int nParam)
{
	maxpop = n;
	N = 1;
	pcross = 1;
	Ind.resize(maxpop);
	Ind_old.resize(maxpop);
	for (int i=0; i<n; i++)
	{
		Ind[i] = CIndividual(nParam);
		Ind_old[i] = CIndividual(nParam);

	}
	fitdist = CDistribution(maxpop);
	cross_over_type = 1;
	MaxFitness = 0;
}
#ifdef GIFMOD

CGA::CGA(string filename, CMediumSet &Syst)
{
	int ii;
	char buffer[33];
	Sys = Syst;
	ifstream file(filename);
	nParam = 0;
	totnParam = 0;
	vector<string> s;
	pcross = 1;
	N = 1;
	fixedstd = true;
	purt_fac = 0.1;
	noinipurt = false;
	sensbasedpurt=false;
	justreadmcmc=false;
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
	pathname = Syst.FI.outputpathname;
	readfromgafile = false;
	calc_distributions = false;	
	noise_realization_writeout = false;
	obs_realization_writeout = false;
	const_realization_writeout = false;
	influent_realization = false;
	no_bins = 10;
	mcmc_run = false;

	while (file.eof() == false)
	{
		s = getline(file);
		if (s.size()>0)
		{	if (s[0] == "maxpop") maxpop = atoi(s[1].c_str());
			if (s[0] == "ngen") nGen = atoi(s[1].c_str());
			if (s[0] == "pcross") pcross = atof(s[1].c_str());
			if (s[0] == "pmute") pmute = atof(s[1].c_str());
			if (s[0] == "shakescale") shakescale = atof(s[1].c_str());
			if (s[0] == "shakescalered") shakescalered = atof(s[1].c_str());
			if (s[0] == "outputfile") outputfilename = s[1];
			if (s[0] == "burnout") burnout = atoi(s[1].c_str());
			if (s[0] == "nchains") nchains = atoi(s[1].c_str());
			if (s[0] == "mcmcoutputfile") mcmcoutputfile = s[1];
			if (s[0] == "nmcmcsamples") nMCMCsamples = atoi(s[1].c_str());
			if (s[0] == "stucklimit") acceptance_rate = atof(s[1].c_str());
			if (s[0] == "purtscale") purtscale = atof(s[1].c_str());
			if (s[0] == "purtfac") purt_fac = atof(s[1].c_str());
			if (s[0] == "purt_fac") purt_fac = atof(s[1].c_str());
			if (s[0] == "writeinterval") writeinterval = atoi(s[1].c_str());
			if (s[0] == "noinipurt") noinipurt = atoi(s[1].c_str());
			if (s[0] == "getfromfilename") getfromfilename = s[1].c_str();
			if (s[0] == "sensbasedpurt") sensbasedpurt = atoi(s[1].c_str());
			if (s[0] == "justreadmcmc") justreadmcmc = atoi(s[1].c_str());
			if (s[0] == "nrealizations") nrealizations = atoi(s[1].c_str());
			if (s[0] == "calculate_sensitivity") sens_out = atoi(s[1].c_str());	
			if (s[0] == "dp_sensitivity") dp_sens = atof(s[1].c_str());	
			if (s[0] == "global_sensitivity") global_sensitivity = atoi(s[1].c_str());		
			if (s[0] == "continuemcmc") continue_mcmc = atoi(s[1].c_str());	
			if (s[0] == "calculate_percentile") calculate_percentile = atoi(s[1].c_str());
			if (s[0] == "calculate_correlation") calculate_correlation = atoi(s[1].c_str());
			if (s[0] == "mcmc_realization") mcmc_realization = atoi(s[1].c_str());			
			if (s[0] == "noise_realization_writeout") noise_realization_writeout = atoi(s[1].c_str());
			if (s[0] == "obs_realization_writeout") obs_realization_writeout = atoi(s[1].c_str());
			if (s[0] == "const_realization_writeout") const_realization_writeout = atoi(s[1].c_str());
			if (s[0] == "influent_realization") influent_realization = atoi(s[1].c_str());
			if (s[0] == "forward_montecarlo_filename") forward_inflow_ARIMA_params = pathname + s[1];
			if (s[0] == "initial_population") initialpopfilemame = s[1];
			if (s[0] == "readfromgafile") readfromgafile = atoi(s[1].c_str());
			if (s[0] == "calc_distributions") calc_distributions = atoi(s[1].c_str());
			if (s[0] == "no_bins") no_bins = atoi(s[1].c_str());			
			if (s[0] == "mcmc_run") mcmc_run = atoi(s[1].c_str());	
			if (s[0] == "calc_output_percentiles") 
			 for (int i=0; i<s.size()-1; i++) 
				calc_output_percentiles.push_back(atof(s[i+1].c_str()));			
		}
	}

	burnout = burnout / writeinterval;
	for (int i=0; i<Sys.parameters.size(); i++) 
		if (Sys.parameters[i].fixed == false)
		{
			if (Sys.parameters[i].applytoall == true)
			{	totnParam++;
				nParam++;
				params.push_back(i);
				if (Sys.parameters[i].log)
				{	minval.push_back(log10(Sys.parameters[i].low));
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
			else
			{
				totnParam+=1;
				nParam++;
				params.push_back(i);
				if (Sys.parameters[i].log)
				{	minval.push_back(log10(Sys.parameters[i].low));
					maxval.push_back(log10(Sys.parameters[i].high));
					
				}
				else
				{
					minval.push_back(Sys.parameters[i].low);
					maxval.push_back(Sys.parameters[i].high);
				}
				apply_to_all.push_back(false);
				loged.push_back(Sys.parameters[i].log);
				for (int j=0; j<1; j++)
				{	to_ts.push_back(j);
					paramname.push_back(Sys.parameters[i].name + "_" + string(_itoa(j,buffer,10)));
				}
			}
		}

	
	Ind.resize(maxpop);
	Ind_old.resize(maxpop);

	fitdist = CDistribution(maxpop);
	cross_over_type = 1;
	
	for (int i=0; i<maxpop; i++)
	{
		Ind[i] = CIndividual(totnParam);
		Ind_old[i] = CIndividual(totnParam);

	}

	for (int i = 0; i<totnParam; i++)
		Setminmax(i, minval[get_act_paramno(i)], maxval[get_act_paramno(i)],4);

	MaxFitness = 0;
}
#endif


void CGA::setnparams(int n_params)
{
	Ind.resize(maxpop);
	Ind_old.resize(maxpop);
	for (int i=0; i<maxpop; i++)
	{
		Ind[i] = CIndividual(n_params);
		Ind_old[i] = CIndividual(n_params);

	}
}


void CGA::setnumpop(int n)
{
	maxpop = n;
	CIndividual TempInd = Ind[0];
	
	int nParam = Ind[0].nParams;
	Ind.resize(maxpop);
	Ind_old.resize(maxpop);
	for (int i=0; i<n; i++)
	{
		Ind[i] = CIndividual(nParam);
		Ind_old[i] = CIndividual(nParam);
		for (int j = 0; j<nParam; j++)
		{
			Ind[i].minrange[j] = TempInd.minrange[j];
			Ind[i].maxrange[j] = TempInd.maxrange[j];
			Ind[i].precision[j] = TempInd.precision[j];
			Ind_old[i].minrange[j] = TempInd.minrange[j];
			Ind_old[i].maxrange[j] = TempInd.maxrange[j];
			Ind_old[i].precision[j] = TempInd.precision[j];
		}

	}
	fitdist = CDistribution(maxpop);
}

CGA::CGA(const CGA &C)
{
	maxpop = C.maxpop;
	Ind.resize(maxpop);
	Ind_old.resize(maxpop);
	for (int i=0; i<maxpop; i++)
	{
		Ind[i] = C.Ind[i];
		Ind_old[i] = C.Ind_old[i];
	}

	for (int i=0; i<25; i++)
	{
		params[i] = C.params[i];
		loged[i] = C.loged[i];
	}
	fitdist = C.fitdist;
	fitnesstype = C.fitnesstype;
	exponentcoeff = C.exponentcoeff;
	N = C.N;
	pcross = C.pcross;
	cross_over_type = C.cross_over_type;
	pmute = C.pmute;
	MaxFitness = C.MaxFitness;
	pertscale = C.pertscale;
	biasfact = C.biasfact;
	purt_fac = C.purt_fac;
	fixedstd = C.fixedstd;
	mixederror = C.mixederror;
	noinipurt = C.noinipurt;
	sensbasedpurt = C.sensbasedpurt;
	paramname = C.paramname;
	sens_out = C.sens_out;
	readfromgafile = C.readfromgafile;
	calc_distributions = C.calc_distributions;	
	noise_realization_writeout = C.noise_realization_writeout;
	obs_realization_writeout = C.obs_realization_writeout;
	const_realization_writeout = C.const_realization_writeout;
	no_bins = C.no_bins;
	influent_realization = C.influent_realization;
}


CGA CGA::operator=(CGA &C)
{
	maxpop = C.maxpop;
	Ind.resize(maxpop);
	Ind_old.resize(maxpop);
	for (int i=0; i<maxpop; i++)
	{
		Ind[i] = C.Ind[i];
		Ind_old[i] = C.Ind_old[i];
	}
	
	for (int i=0; i<25; i++)
	{
		params[i] = C.params[i];
		loged[i] = C.loged[i];
	}
	fitdist = C.fitdist;
	N = C.N;
	fitnesstype = C.fitnesstype;
	exponentcoeff = C.exponentcoeff;
	pcross = C.pcross;
	cross_over_type = C.cross_over_type;
	pmute = C.pmute;
	MaxFitness = C.MaxFitness;
	pertscale = C.pertscale;
	biasfact = C.biasfact;
	purt_fac = C.purt_fac;
	fixedstd = C.fixedstd;
	mixederror = C.mixederror;
	noinipurt = C.noinipurt;
	sensbasedpurt = C.sensbasedpurt;
	paramname = C.paramname;
	sens_out = C.sens_out;
	readfromgafile = C.readfromgafile;
	calc_distributions = C.calc_distributions;
	noise_realization_writeout = C.noise_realization_writeout;
	obs_realization_writeout = C.obs_realization_writeout;
	const_realization_writeout = C.const_realization_writeout;
	influent_realization = C.influent_realization;
	no_bins = C.no_bins;

	return *this;

}

CGA::~CGA()
{
	
}

void CGA::initialize()
{
	for (int i=0; i<maxpop; i++)
	{
		Ind[i].initialize();
	}

	if (initialpopfilemame!="")
	{
		getinifromoutput(pathname+initialpopfilemame);
		for (int i=0; i<initial_pop.size(); i++)
			for (int j=0; j<max(int(initial_pop[i].size()),totnParam); j++)
				if (loged[j]==1)
					Ind[i].x[j] = log10(initial_pop[i][j]);
				else
					Ind[i].x[j] = initial_pop[i][j];
	}
}

void CGA::Setminmax(int a, double minrange, double maxrange, int prec)
{
	for (int i=0; i<maxpop; i++)
	{	
		Ind[i].maxrange[a] = maxrange;
		Ind[i].minrange[a] = minrange;
		Ind[i].precision[a] = prec;
	}

}

void CGA::assignfitnesses()
{
	sumfitness = 0;
	
	vector<vector<double>> inp;
	
	inp.resize(maxpop); 
	

	for (int k=0; k<maxpop; k++)
		inp[k].resize(totnParam);	
	
	vector<double> time_(maxpop);
	vector<int> epochs(maxpop);
	clock_t t0,t1;
	
	for (int k = 0; k < maxpop; k++)
	{
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

		int l = 0;
		for (int i = 0; i < nParam; i++)
			Sys1[k].set_param(params[i], inp[k][i]);
		Sys1[k].finalize_set_param();
	}
		
omp_set_num_threads(numberOfThreads);
#pragma omp parallel for //private(ts,l)
		for (int k=0; k<maxpop; k++)
		{
			//int ts,l;
					
			FILE *FileOut;
#ifdef GIFMOD
			FileOut = fopen((Sys.FI.outputpathname+"detail_GA.txt").c_str(),"a");
#endif
#ifdef GWA
			FileOut = fopen((Sys.pathname + "detail_GA.txt").c_str(), "a");
#endif

			fprintf(FileOut, "%i, ", k);
			for (int l=0; l<Ind[0].nParams; l++)
				if (loged[get_act_paramno(l)]==1) 
					fprintf(FileOut, "%le, ", pow(10,Ind[k].x[l]));
				else
					fprintf(FileOut, "%le, ", Ind[k].x[l]);

			//fprintf(FileOut, "%le, %le, %i, %e, %i, %i", Ind[k].actual_fitness, Ind[k].fitness, Ind[k].rank, time_[k], threads_num[k],num_threads[k]);
			//fprintf(FileOut, "%le, %le, %i, %e", Ind[k].actual_fitness, Ind[k].fitness, Ind[k].rank, time_[k]);
			fprintf(FileOut, "\n");
			fclose(FileOut);			
			
			clock_t t0 = clock();
			for (int ts=0; ts<1; ts++)
			{
				Ind[k].actual_fitness -= Sys1[k].calc_log_likelihood(); 
				epochs[k] += Sys1[k].epoch_count();
			}	
			time_[k] = ((float)(clock() - t0))/CLOCKS_PER_SEC;
#ifdef GIFMOD
			FileOut = fopen((Sys.FI.pathname+"detail_GA.txt").c_str(),"a");
#endif
#ifdef GWA
			FileOut = fopen((Sys.pathname+"detail_GA.txt").c_str(),"a");
#endif
			fprintf(FileOut, "%i, fitness=%le, time=%e, epochs=%i\n", k, Ind[k].actual_fitness, time_[k], epochs[k]);
			fclose(FileOut);
		
		}
	
	Sys_out = Sys1[maxfitness()];
	

	inp.clear();	
	assignfitness_rank(N);  
	
}


void CGA::crossover()
{
	
	for (int i=0; i<maxpop; i++)
		Ind_old[i] = Ind[i];
	int a = maxfitness();
	Ind[0] = Ind_old[a];
	Ind[1] = Ind_old[a];
	for (int i=2; i<maxpop; i+=2)
	{
		
		int j1 = fitdist.GetRand();
		int j2 = fitdist.GetRand();
		double x = GetRndUniF(0,1);
		if (x<pcross)
			if (cross_over_type == 1) 
				cross(Ind_old[j1], Ind_old[j2], Ind[i], Ind[min(i+1,maxpop-1)]);   //1 Breaking point
			else
				cross2p(Ind_old[j1], Ind_old[j2], Ind[i], Ind[min(i + 1, maxpop - 1)]);  //2 Breaking point
		else
		{	
			Ind[i] = Ind_old[j1];
			Ind[i+1] = Ind_old[j2];
		}
		
	}

}

void CGA::crossoverRC()
{
	
	for (int i=0; i<maxpop; i++)
		Ind_old[i] = Ind[i];
	int a = maxfitness();
	Ind[0] = Ind_old[a];
	Ind[1] = Ind_old[a];
	for (int i=2; i<maxpop; i+=2)
	{
		int j1 = fitdist.GetRand();
		int j2 = fitdist.GetRand();
		double x = GetRndUnif(0,1);
		if (x<pcross)
			cross_RC_L(Ind_old[j1], Ind_old[j2], Ind[i], Ind[i+1]);
		else
		{	
			Ind[i] = Ind_old[j1];
			Ind[i+1] = Ind_old[j2];
		}		
	}
}

double CGA::avgfitness()
{
	double sum=0;
	for (int i=0; i<maxpop; i++)
		sum += Ind[i].fitness;
	return sum/maxpop;
}


int CGA::optimize()
{
#ifdef GIFMOD
	string RunFileName = Sys.FI.outputpathname + outputfilename;
#endif
#ifdef GWA
	string RunFileName = Sys.pathname + outputfilename;
#endif

	FILE *FileOut;
	FILE *FileOut1;

	FileOut = fopen(RunFileName.c_str(),"w");
	fclose(FileOut);
#ifdef GIFMOD
	FileOut1 = fopen((Sys.FI.outputpathname + "detail_GA.txt").c_str(), "w");
#endif
#ifdef GWA
	FileOut1 = fopen((Sys.pathname + "detail_GA.txt").c_str(), "w");
#endif
	fclose(FileOut1);

	double shakescaleini = shakescale;

	vector<double> X(Ind[0].nParams);

	Sys1.resize(maxpop);

	initialize();
	double ininumenhancements = numenhancements;
	numenhancements = 0;

	CMatrix Fitness(nGen, 3);

	for (int i=0; i<nGen; i++)
	{
		//Form1.label1->Text=gcnew System::String(("Generation: "+numbertostring(i)).c_str());
		//Form1.Update();
		assignfitnesses();
		FileOut = fopen(RunFileName.c_str(),"a");
		printf("Generation: %i\n", i);
		fprintf(FileOut, "Generation: %i\n", i);
		fprintf(FileOut, "ID, ");
		for (int k=0; k<Ind[0].nParams; k++)
			fprintf(FileOut, "%s, ", paramname[k].c_str());
		fprintf(FileOut, "%s, %s, %s", "likelihood", "Fitness", "Rank");
		fprintf(FileOut, "\n");

		for (int j1=0; j1<maxpop; j1++)
		{
			fprintf(FileOut, "%i, ", j1);


			for (int k=0; k<Ind[0].nParams; k++)
				if (loged[get_act_paramno(k)] == 1)
					fprintf(FileOut, "%le, ", pow(10, Ind[j1].x[k]));
				else
					fprintf(FileOut, "%le, ", Ind[j1].x[k]);

			fprintf(FileOut, "%le, %le, %i", Ind[j1].actual_fitness, Ind[j1].fitness, Ind[j1].rank);
			fprintf(FileOut, "\n");
		}
		fclose(FileOut);

		int j = maxfitness();


		Fitness[i][0] = Ind[j].actual_fitness;

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
		if (RCGA == true)
			crossoverRC();
		else
			crossover();


		mutate(pmute);
		shake();
	}
	//Form1.label1->Text=L"finalizing GA ";
	//Form1.Refresh();
	assignfitnesses();
	FileOut = fopen(RunFileName.c_str(), "a");
	fprintf(FileOut, "Final Enhancements\n");
	double l_MaxFitness = 1;
	int j = maxfitness();


	MaxFitness = Ind[j].actual_fitness;
	final_params.resize(totnParam);


	for (int k = 0; k<Ind[0].nParams; k++)
	{
		if (loged[get_act_paramno(k)] == 1) final_params[k] = pow(10, Ind[j].x[k]); else final_params[k] = Ind[j].x[k];
		fprintf(FileOut, "%s, ", paramname[k].c_str());
		fprintf(FileOut, "%le, ", final_params[k]);
		fprintf(FileOut, "%le, %le\n", Ind[j].actual_fitness, Ind[j].fitness);
	}
	fclose(FileOut);

	assignfitnesses(final_params);

	Sys1.clear();

	return maxfitness();
}
double CGA::assignfitnesses(vector<double> inp)
{

	double likelihood = 0;
	CMediumSet Sys1;


	Sys1 = Sys;

	int l = 0;
	for (int i = 0; i < nParam; i++)
		Sys1.set_param(i, inp[i]);
	Sys1.finalize_set_param();
	likelihood -= Sys1.calc_log_likelihood();


	Sys_out = Sys1;

	return likelihood;

}
vector<CMediumSet>& CGA::assignfitnesses_p(vector<double> inp)

{

	double likelihood = 0;
	vector<CMediumSet> Sys1(1);








	for (int ts = 0; ts<1; ts++)
	{
		Sys1[ts] = Sys;

		int l = 0;
		for (int i = 0; i<nParam; i++)
			Sys1[ts].set_param(params[i], inp[getparamno(i, ts)]);
		Sys1[ts].finalize_set_param();
		likelihood -= Sys1[ts].calc_log_likelihood();
















	}
	Sys1;

	return Sys1;

}
int CGA::getparamno(int i, int ts)
{
	int l = 0;
	for (int j = 0; j<i; j++)
		if (apply_to_all[j]) l++; else l += 1;

	if (apply_to_all[i])
		return l;
	else
		return l + ts;

}
int CGA::get_act_paramno(int i)
{
	int l = -1;
	for (int j = 0; j<nParam; j++)
	{
		if (apply_to_all[j]) l++; else l += 1;
		if (l >= i)
		{
			if (apply_to_all[j]) l -= 1; else l--;
			return j;
		}
	}
}
int CGA::get_time_series(int i)
{
	int l = 0;
	for (int j = 0; j<nParam; j++)
	{
		if (apply_to_all[j]) l += 1; else l += 1;
		if (l >= i)
		{
			if (apply_to_all[j]) l -= 1; else l--;
			return i - l;
		}
	}
}
double CGA::getfromoutput(string filename)

{

	ifstream file(filename);
	vector<string> s;
	final_params.resize(totnParam);
	while (file.eof() == false)







	{
		s = getline(file);
		if (s.size()>0)
		{
			if (s[0] == "Final Enhancements")
				for (int i = 0; i<totnParam; i++)
				{
					s = getline(file);
					if (s.size() == 0)
						Sys.writetolog("The number of parameters in GA output file does not match the number of unknown parameters");
					else
						final_params[i] = atof(s[1].c_str());

				}
		}










	}

	double ret = assignfitnesses(final_params);

	return ret;
}


#ifdef GWA
CGA::CGA(string filename, CGWA &Syst)

{
	int ii;
	char buffer[33];
	Sys = Syst;
	ifstream file(filename);
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
	pathname = Syst.pathname;
	readfromgafile = false;
	calc_distributions = false;
	noise_realization_writeout = false;
	obs_realization_writeout = false;
	const_realization_writeout = false;
	influent_realization = false;
	no_bins = 10;
	mcmc_run = false;

	while (file.eof() == false)
	{
		s = getline(file);
		if (s.size()>0)
		{
			if (s[0] == "maxpop") maxpop = atoi(s[1].c_str());
			if (s[0] == "ngen") nGen = atoi(s[1].c_str());
			if (s[0] == "pcross") pcross = atof(s[1].c_str());
			if (s[0] == "pmute") pmute = atof(s[1].c_str());
			if (s[0] == "shakescale") shakescale = atof(s[1].c_str());
			if (s[0] == "shakescalered") shakescalered = atof(s[1].c_str());
			if (s[0] == "outputfile") outputfilename = s[1];
			if (s[0] == "burnout") burnout = atoi(s[1].c_str());
			if (s[0] == "nchains") nchains = atoi(s[1].c_str());
			if (s[0] == "mcmcoutputfile") mcmcoutputfile = s[1];
			if (s[0] == "nmcmcsamples") nMCMCsamples = atoi(s[1].c_str());
			if (s[0] == "acceptance_rate") acceptance_rate = atof(s[1].c_str());
			if (s[0] == "purtscale") purtscale = atof(s[1].c_str());
			if (s[0] == "purtfac") purt_fac = atof(s[1].c_str());
			if (s[0] == "purt_fac") purt_fac = atof(s[1].c_str());
			if (s[0] == "writeinterval") writeinterval = atoi(s[1].c_str());
			if (s[0] == "noinipurt") noinipurt = atoi(s[1].c_str());
			if (s[0] == "getfromfilename") getfromfilename = s[1].c_str();
			if (s[0] == "sensbasedpurt") sensbasedpurt = atoi(s[1].c_str());
			if (s[0] == "justreadmcmc") justreadmcmc = atoi(s[1].c_str());
			if (s[0] == "nrealizations") nrealizations = atoi(s[1].c_str());
			if (s[0] == "calculate_sensitivity") sens_out = atoi(s[1].c_str());
			if (s[0] == "dp_sensitivity") dp_sens = atof(s[1].c_str());
			if (s[0] == "global_sensitivity") global_sensitivity = atoi(s[1].c_str());
			if (s[0] == "continuemcmc") continue_mcmc = atoi(s[1].c_str());
			if (s[0] == "calculate_percentile") calculate_percentile = atoi(s[1].c_str());
			if (s[0] == "calculate_correlation") calculate_correlation = atoi(s[1].c_str());
			if (s[0] == "mcmc_realization") mcmc_realization = atoi(s[1].c_str());
			if (s[0] == "noise_realization_writeout") noise_realization_writeout = atoi(s[1].c_str());
			if (s[0] == "obs_realization_writeout") obs_realization_writeout = atoi(s[1].c_str());
			if (s[0] == "const_realization_writeout") const_realization_writeout = atoi(s[1].c_str());
			if (s[0] == "influent_realization") influent_realization = atoi(s[1].c_str());
			if (s[0] == "forward_montecarlo_filename") forward_inflow_ARIMA_params = pathname + s[1];
			if (s[0] == "initial_population") initialpopfilemame = s[1];
			if (s[0] == "readfromgafile") readfromgafile = atoi(s[1].c_str());
			if (s[0] == "calc_distributions") calc_distributions = atoi(s[1].c_str());
			if (s[0] == "no_bins") no_bins = atoi(s[1].c_str());
			if (s[0] == "mcmc_run") mcmc_run = atoi(s[1].c_str());
			if (s[0] == "calc_output_percentiles")
				for (int i = 0; i<s.size() - 1; i++)
					calc_output_percentiles.push_back(atof(s[i + 1].c_str()));
		}
	}

	burnout = burnout / writeinterval;
	for (int i = 0; i<Sys.parameters.size(); i++)
		if (Sys.parameters[i].fixed == false)
		{
			if (Sys.parameters[i].applytoall == true)
			{
				totnParam++;
				nParam++;
				params.push_back(i);
				if (Sys.parameters[i].log)
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
			else
			{
				totnParam += 1; 
				nParam++;
				params.push_back(i);
				if (Sys.parameters[i].log)
				{
					minval.push_back(log10(Sys.parameters[i].low));
					maxval.push_back(log10(Sys.parameters[i].high));

				}
				else
				{
					minval.push_back(Sys.parameters[i].low);
					maxval.push_back(Sys.parameters[i].high);
				}
				apply_to_all.push_back(false);
				loged.push_back(Sys.parameters[i].log);
				for (int j = 0; j<1; j++)
				{
					to_ts.push_back(j);
					paramname.push_back(Sys.parameters[i].name + "_" + string(_itoa(j, buffer, 10)));
				}
			}
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

void CGA::assignfitnesses()
{
	sumfitness = 0;

	vector<vector<double>> inp;

	inp.resize(maxpop);


	for (int k = 0; k<maxpop; k++)
		inp[k].resize(totnParam);

	vector<double> time_(maxpop);
	vector<int> epochs(maxpop);
	clock_t t0, t1;

	for (int k = 0; k<maxpop; k++)
	{
		for (int i = 0; i<totnParam; i++)
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

		for (int ts = 0; ts<1; ts++) 
			{
	
				Sys1[k][ts] = Sys;

				for (int i = 0; i<nParam; i++)
					Sys1[k][ts].setparams(i, inp[k][getparamno(i, 0)]);

			int l = 0;
		}
	}

omp_set_num_threads(numberOfThreads);
#pragma omp parallel for //private(ts,l)
	for (int k = 0; k<maxpop; k++)
	{
		//int ts,l;

		FILE *FileOut;
		FileOut = fopen((Sys.pathname() + "detail_GA.txt").c_str(), "a");
		std::fprintf(FileOut, "%i, ", k);
		for (int l = 0; l<Ind[0].nParams; l++)
			if (loged[get_act_paramno(l)] == 1)
				std::fprintf(FileOut, "%le, ", pow(10, Ind[k].x[l]));
			else
				std::fprintf(FileOut, "%le, ", Ind[k].x[l]);

		//fprintf(FileOut, "%le, %le, %i, %e, %i, %i", Ind[k].actual_fitness, Ind[k].fitness, Ind[k].rank, time_[k], threads_num[k],num_threads[k]);
		//fprintf(FileOut, "%le, %le, %i, %e", Ind[k].actual_fitness, Ind[k].fitness, Ind[k].rank, time_[k]);
		std::fprintf(FileOut, "\n");
		std::fclose(FileOut);

		clock_t t0 = clock();
		for (int ts = 0; ts<1; ts++)
		{
			Ind[k].actual_fitness -= Sys1[k][ts].calc_log_likelihood();
			epochs[k] += Sys1[k][ts].epoch_count;
		}
		time_[k] = ((float)(clock() - t0)) / CLOCKS_PER_SEC;
		FileOut = fopen((Sys.pathname() + "detail_GA.txt").c_str(), "a");
		std::fprintf(FileOut, "%i, fitness=%le, time=%e, epochs=%i\n", k, Ind[k].actual_fitness, time_[k], epochs[k]);
		std::fclose(FileOut);

	}

	Sys_out = Sys1[maxfitness()];


	inp.clear();
	assignfitness_rank(N);

}

int CGA::optimize()
{
	string RunFileName = Sys.pathname() + outputfilename;


	FILE *FileOut;
	FILE *FileOut1;

	FileOut = fopen(RunFileName.c_str(), "w");
	std::fclose(FileOut);
	FileOut1 = fopen((Sys.pathname() + "detail_GA.txt").c_str(), "w");
	std::fclose(FileOut1);


	double shakescaleini = shakescale;


	vector<double> X(Ind[0].nParams);


	Sys1.resize(maxpop);
//	for (int i = 0; i<maxpop; i++) Sys1[i].resize(1); ASK ARASH
	for (int i = 0; i<maxpop; i++) Sys1[i].resize(1);

	initialize();
	double ininumenhancements = numenhancements;
	numenhancements = 0;


	CMatrix Fitness(nGen, 3);


	for (int i = 0; i<nGen; i++)
	{
		//Form1.label1->Text=gcnew System::String(("Generation: "+numbertostring(i)).c_str());
		//Form1.Update();
		assignfitnesses();
		FileOut = fopen(RunFileName.c_str(), "a");
		printf("Generation: %i\n", i);
		std::fprintf(FileOut, "Generation: %i\n", i);
		std::fprintf(FileOut, "ID, ");


		for (int k = 0; k<Ind[0].nParams; k++)
			std::fprintf(FileOut, "%s, ", paramname[k].c_str());
		std::fprintf(FileOut, "%s, %s, %s", "likelihood", "Fitness", "Rank");
		std::fprintf(FileOut, "\n");




		for (int j1 = 0; j1<maxpop; j1++)
		{
			std::fprintf(FileOut, "%i, ", j1);



			for (int k = 0; k<Ind[0].nParams; k++)
				if (loged[get_act_paramno(k)] == 1)
					std::fprintf(FileOut, "%le, ", pow(10, Ind[j1].x[k]));
				else
					std::fprintf(FileOut, "%le, ", Ind[j1].x[k]);


			std::fprintf(FileOut, "%le, %le, %i", Ind[j1].actual_fitness, Ind[j1].fitness, Ind[j1].rank);
			std::fprintf(FileOut, "\n");


		}
		std::fclose(FileOut);


		int j = maxfitness();


		Fitness[i][0] = Ind[j].actual_fitness;

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
		if (RCGA == true)
			crossoverRC();
		else
			crossover();


		mutate(pmute);
		shake();
	}
	//Form1.label1->Text=L"finalizing GA ";
	//Form1.Refresh();
	assignfitnesses();
	FileOut = fopen(RunFileName.c_str(), "a");
	std::fprintf(FileOut, "Final Enhancements\n");

	double l_MaxFitness = 1;
	int j = maxfitness();


	MaxFitness = Ind[j].actual_fitness;
	final_params.resize(totnParam);


	for (int k = 0; k<Ind[0].nParams; k++)
	{
		if (loged[get_act_paramno(k)] == 1) final_params[k] = pow(10, Ind[j].x[k]); else final_params[k] = Ind[j].x[k];
		std::fprintf(FileOut, "%s, ", paramname[k].c_str());
		std::fprintf(FileOut, "%le, ", final_params[k]);
		std::fprintf(FileOut, "%le, %le\n", Ind[j].actual_fitness, Ind[j].fitness);



	}
	std::fclose(FileOut);



	assignfitnesses(final_params);

	Sys1.clear();



	return maxfitness();
}

double CGA::assignfitnesses(vector<double> inp)
{

	double likelihood = 0;
	vector<CGWA> Sys1(1);


	for (int ts = 0; ts<1; ts++)
	{
		Sys1[ts] = Sys;

		int l = 0;
		for (int i = 0; i<nParam; i++)
			Sys1[ts].set_param(i, inp[getparamno(i, ts)]);
		Sys1[ts].finalize_set_param();
		likelihood -= Sys1[ts].calc_log_likelihood();

	}
	Sys_out = Sys1;

	return likelihood;

}

vector<CGWA>& CGA::assignfitnesses_p(vector<double> inp)
{

	double likelihood = 0;
	vector<CGWA> Sys1(1);


	for (int ts = 0; ts<1; ts++)
	{
		Sys1[ts] = Sys;

		int l = 0;
		for (int i = 0; i<nParam; i++)
			Sys1[ts].set_param(i, inp[getparamno(i, ts)]);
		Sys1[ts].finalize_set_param();
		likelihood -= Sys1[ts].calc_log_likelihood();

	}
	Sys1;

	return Sys1;

}

int CGA::getparamno(int i, int ts)
{
	int l = 0;
	for (int j = 0; j<i; j++)
		if (apply_to_all[j]) l++; else l += 1;

	if (apply_to_all[i])
		return l;
	else
		return l + ts;

}
int CGA::get_act_paramno(int i)
{
	int l = -1;
	for (int j = 0; j<nParam; j++)
	{
		if (apply_to_all[j]) l++; else l += 1;
		if (l >= i)
		{
			if (apply_to_all[j]) l -= 1; else l--;
			return j;
		}
	}
}
int CGA::get_time_series(int i)
{
	int l = 0;
	for (int j = 0; j<nParam; j++)
	{
		if (apply_to_all[j]) l += 1; else l += 1;
		if (l >= i)
		{
			if (apply_to_all[j]) l -= 1; else l--;
			return i - l;
		}
	}
}

double CGA::getfromoutput(string filename)
{

	ifstream file(filename);
	vector<string> s;
	final_params.resize(totnParam);
	while (file.eof() == false)
	{
		s = getline(file);
		if (s.size()>0)
		{
			if (s[0] == "Final Enhancements")
				for (int i = 0; i<totnParam; i++)
				{
					s = getline(file);
//					if (s.size() == 0)
//						Sys.writetolog("The number of parameters in GA output file does not match the number of unknown parameters");
//					else
						final_params[i] = atof(s[1].c_str());

				}
		}
	}

	double ret = assignfitnesses(final_params);

	return ret;
}
#endif

void CGA::shake()
{
	for (int i=1; i<maxpop; i++)
		Ind[i].shake(shakescale);

}

void CGA::mutate(double mu)
{
	for (int i=2; i<maxpop; i++)
		Ind[i].mutate(mu);

}


int CGA::maxfitness()
{
	double max_fitness = 1E+308 ;
	int i_max = 0;
	for (int i=0; i<maxpop; i++)
		if (max_fitness>Ind[i].actual_fitness)
		{	
			max_fitness = Ind[i].actual_fitness;
			i_max = i;
		}
	return i_max;

}

double CGA::variancefitness()
{
	double sum = 0;
	double a = avgfitness();
	for (int i=0; i<maxpop; i++)
		sum += (a - Ind[i].fitness)*(a - Ind[i].fitness);
	return sum;

}

double CGA::stdfitness()
{
	double sum = 0;
	double a = avg_inv_actual_fitness();
	for (int i=0; i<maxpop; i++)
		sum += (a - 1/Ind[i].actual_fitness)*(a - 1/Ind[i].actual_fitness);
	return sqrt(sum)/maxpop/a;

}

double CGA::avg_actual_fitness()
{
	double sum=0;
	for (int i=0; i<maxpop; i++)
		sum += Ind[i].actual_fitness;
	return sum/maxpop;

}

double CGA::avg_inv_actual_fitness()
{
	double sum=0;
	for (int i=0; i<maxpop; i++)
		sum += 1/Ind[i].actual_fitness;
	return sum/maxpop;

}



void CGA::assignrank()
{
	for (int i=0; i<maxpop; i++)
	{
		int r = 1;
		for (int j=0; j<maxpop; j++)
		{
			if (Ind[i].actual_fitness > Ind[j].actual_fitness) r++;
		}
		Ind[i].rank = r;
	}

}

void CGA::assignfitness_rank(double N)
{
	assignrank();
	for (int i=0; i<maxpop; i++)
	{
		Ind[i].fitness = pow(1.0/static_cast<double>(Ind[i].rank),N);
	}
}



void CGA::fillfitdist()
{
       double sum=0;
       for (int i=0; i<maxpop; i++)
       {
              sum+=Ind[i].fitness;
       }

       fitdist.s[0] = 0;
       fitdist.e[0] = Ind[0].fitness/sum;
       for (int i=1; i<maxpop-1; i++)
       {
              fitdist.e[i] = fitdist.e[i-1] + Ind[i].fitness/sum;
              fitdist.s[i] = fitdist.e[i-1];
       }
       fitdist.s[maxpop-1] = fitdist.e[maxpop-2];
       fitdist.e[maxpop-1] = 1;

}
































































double CGA::evaluateforward()
{
	vector<double> v(1);
	v[0] = 1;
	CVector out(1);
	int x_nParam = nParam;
	vector<int> x_params = params;
	nParam = 1;
	params.resize(1);
	params[0] = 100;
	out[0] = assignfitnesses(v);
#ifdef GIFMOD
	out.writetofile(Sys.FI.outputpathname + "likelihood.txt");
#endif
#ifdef GWA
	out.writetofile(Sys.pathname + "likelihood.txt");
#endif
	params = x_params;
	nParam = x_nParam;
	return out[0];
}

double CGA::evaluateforward_mixed(vector<double> v)
{
	CVector out(1);
	int x_nParam = nParam;
	vector<int> x_params = params;
	nParam = 2;
	params.resize(2);
	params[0] = 100;
	params[1] = 101;
	out[0] = assignfitnesses(v);
#ifdef GIFMOD
	out.writetofile(Sys.FI.outputpathname + "likelihood.txt");
#endif
#ifdef GWA
	out.writetofile(Sys.pathname + "likelihood.txt");
#endif
	params = x_params;
	nParam = x_nParam;
	return out[0];
}
































void CGA::getinifromoutput(string filename)
{
	ifstream file(filename);
	vector<string> s;
	initial_pop.resize(1);
	initial_pop[0].resize(totnParam);
	while (file.eof() == false)
	{
		s = getline(file);
		if (s.size()>0)
		{	if (s[0] == "Final Enhancements")
			for (int i=0; i<totnParam; i++)
			{
				s = getline(file);
				if (loged[get_act_paramno(i)]==1) 
					initial_pop[0][i] = atof(s[1].c_str()); 
				else 
					initial_pop[0][i] = atof(s[1].c_str());
				
			}
		}
	}	
		
}

void CGA::getinitialpop(string filename)
{
	ifstream file(filename);
	vector<string> s;
	
	while (file.eof() == false)
	{
		s = getline(file);
		
		if (s.size()>0)
		{
			vector<double> x;
			for (int j=0; j<s.size(); j++)
				initial_pop.push_back(ATOF(s));

		}
	}
	file.close();
}

